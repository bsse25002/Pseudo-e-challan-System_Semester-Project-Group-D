#include "functions.h"

void Challan(const string& vehicleNum, int challanNo, int violationType, string filename) { //Updating Challan Database
    int amount;

    if(violationType == 0) {
        amount = 1000;
    } else {
        amount = 500;
    }

    string owner, email, address;
    getVehicleDetails(vehicleNum, owner, email, address);

    string subject, message;

    if(vehicleNum == "NONE") {
        subject = "Violator Identification Request";
        message = "Request for Identification of the Violator. Challan Amount = " + to_string(amount) + "\n See Attached Image";
    } else {
        subject = "Traffic Violation Notice";
        message = "Dear, " + owner + " a new traffic violation has been registered for your vehicle " + vehicleNum + ". Challan Amount = " + to_string(amount) + "\nSee Attached Image!";
    }

    sendEmailAsync(email, subject, message, filename);

    Challans challan{vehicleNum, challanNo, owner, email, address, amount};

    json j;
    ifstream read("./Challans/Challans.json");
    if(read.is_open()) {
        read >> j;
        read.close();
    }

    j.push_back(challan);

    ofstream outFile("./Challans/Challans.json");
    outFile << j.dump(4);
    outFile.close();

    cout << "Challan Generated for Vehicle: " << vehicleNum << " | Amount: " << amount << " | Owner: " << owner << endl;
}

//Config
AppConfig CONFIG;

Vector<string> readClasses(const string& path) {
    Vector<string> cls;
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Cannot open classes file: " << path << endl;
        return cls;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            cls.add(line);
        }
    }

    return cls;
}

int readChallanCounter(const string& filePath) { //Ensures challan IDs continue from previous session
    int counter = 1;
    ifstream fin(filePath);
    if (fin.is_open()) { 
        fin >> counter; 
        fin.close(); 
    }

    return counter;
}

void writeChallanCounter(const string& filePath, int counter) { //Writes updated challan number back to file after generating a new violation
    ofstream fout(filePath);
    if (fout.is_open()) { 
        fout << counter; 
        fout.close(); 
    }
}

void processVideo(const string& videoSource) { //Processing Given VideoSource
    // YOLO setup
    Net net = readNetFromDarknet(CONFIG.yoloCfg, CONFIG.yoloWeights);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);
    vector<string> outNames = net.getUnconnectedOutLayersNames();

    // Read classes
    Vector<string> classes = readClasses(CONFIG.classFile);
    int personClass=-1, helmetClass=-1, motorbikeClass=-1, bicycleClass=-1, carClass=-1, trafficLightClass=-1;
    
    for(int i=0;i<classes.size;i++) {
        string c=classes.get(i);

        if(c=="person") {
            personClass=i;
        } else if(c=="helmet") {
            helmetClass=i;
        } else if(c=="motorbike") {
            motorbikeClass=i;
        } else if(c=="bicycle") {
            bicycleClass=i;
        } else if(c=="car") {
            carClass=i;
        } else if(c=="traffic_light") {
            trafficLightClass=i;
        }
    }
    if(personClass==-1 || helmetClass==-1) { 
        cout<<"Required classes missing\n"; 
        return; 
    }

    // Video capture
    VideoCapture cap(videoSource, cv::CAP_FFMPEG);
    if(!cap.isOpened()) { 
        cout<<"Failed to open video\n"; 
        return; 
    }

    // Plate detection
    CascadeClassifier plateCascade;
    if(!plateCascade.load(CONFIG.plateCascade)) { 
        cout << "Cannot load Haar cascade xml\n"; 
        return; 
    }

    tesseract::TessBaseAPI ocr;
    if (ocr.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) { 
        cout << "Cannot initialize Tesseract\n"; 
        return; 
    }

    ocr.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    ocr.SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    int challanCounter = readChallanCounter(CONFIG.challanCounterFile);
    int frameIndex=0, detectionInterval=5, nextTrackId=0;

    Vector<Rect> lastPersonBoxes,lastHelmetBoxes,lastMotorbikeBoxes,lastBicycleBoxes,lastCarBoxes,lastTrafficLightBoxes;
    Vector<VehicleTrack> trackedVehicles;
    Vector<int> issuedHelmetChallans, issuedRedLightChallans;

    Mat frame;
    while(cap.read(frame)) {
        frameIndex++;
        bool runDetection = (frameIndex % detectionInterval == 0);
        Mat cleanFrame = frame.clone();

        if(runDetection) {
            detectAndTrack(frame, net, outNames, lastPersonBoxes, lastHelmetBoxes, lastMotorbikeBoxes, lastBicycleBoxes, lastCarBoxes, lastTrafficLightBoxes, trackedVehicles, nextTrackId, personClass, helmetClass, motorbikeClass, bicycleClass, carClass, trafficLightClass);
        }

        processViolations(frame, cleanFrame, plateCascade, ocr, lastPersonBoxes, lastHelmetBoxes, lastMotorbikeBoxes, lastBicycleBoxes, lastCarBoxes, lastTrafficLightBoxes, trackedVehicles, issuedHelmetChallans, issuedRedLightChallans, challanCounter, CONFIG.violationDir, CONFIG.challanCounterFile);

        imshow("Traffic Violation Detection", frame);
        if(waitKey(10)==27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    ocr.End();

    writeChallanCounter(CONFIG.challanCounterFile, challanCounter);
}