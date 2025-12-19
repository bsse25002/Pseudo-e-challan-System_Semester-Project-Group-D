#include"functions.h"


string base64_encode(const string &in) { //base64 Encoder
    static const char* chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string out;

    int val=0, valb=-6;
    for (unsigned char c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) {
            out.push_back(chars[(val>>valb)&0x3F]);
            valb-=6;
        }
    }

    if (valb>-6) {
        out.push_back(chars[((val<<8)>>(valb+8))&0x3F]);
    }

    while (out.size()%4) {
        out.push_back('=');
    }

    return out;
}

size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) { //Callback for CURL to read the email content
    string payload = (string)userp;
    size_t max = size * nmemb;
    if(payload->empty()) {
        return 0;
    }
size_t len = std::min(max, payload->size());
    memcpy(ptr, payload->c_str(), len);
    payload->erase(0, len);

    return len;
}

void sendEmail(const string &to, const string &subject, const string &body, const string &imagePath) {
    bool hasAttachment = !imagePath.empty();
    string encodedImage;

    if (hasAttachment) {
        ifstream file(imagePath, ios::binary);
        if (!file.is_open()) {
            cerr << "Cannot open file\n";
            return;
        }

        ostringstream oss;
        oss << file.rdbuf();
        encodedImage = base64_encode(oss.str());
    }

    ostringstream msg;
    msg << "From: E-Challan <muhammad.priv001@gmail.com>\r\n";
    msg << "To: " << to << "\r\n";
    msg << "Subject: " << subject << "\r\n";
    msg << "MIME-Version: 1.0\r\n";

    if (!hasAttachment) {
        // ---- Plain text email ----
        msg << "Content-Type: text/plain; charset=UTF-8\r\n\r\n";
        msg << body << "\r\n";
    } else {
        // ---- Email with attachment ----
        msg << "Content-Type: multipart/mixed; boundary=\"BOUNDARY\"\r\n\r\n";

        msg << "--BOUNDARY\r\n";
        msg << "Content-Type: text/plain; charset=UTF-8\r\n\r\n";
        msg << body << "\r\n";

        msg << "--BOUNDARY\r\n";
        msg << "Content-Type: image/jpeg; name=\"attachment.jpg\"\r\n";
        msg << "Content-Disposition: attachment; filename=\"attachment.jpg\"\r\n";
        msg << "Content-Transfer-Encoding: base64\r\n\r\n";
        msg << encodedImage << "\r\n";

        msg << "--BOUNDARY--\r\n";
    }

    string emailData = msg.str();

    CURL* curl = curl_easy_init();
    if (!curl) return;

    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "muhammad.priv001@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "nrxtdhszknqkvtke");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<muhammad.priv001@gmail.com>");

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &emailData);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "Email failed: " << curl_easy_strerror(res) << "\n";
    }

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
}

void sendEmailAsync(const string &to, const string &subject, const string &body, const string &imagePath) { //Async wrapper (non-blocking)
    thread([=](){ sendEmail(to, subject, body, imagePath); }).detach();
}

void getVehicleDetails(const string& vehicleNum, string& owner, string& email, string& address) {
    //Getting Vehicle Details
    if(vehicleNum == "NONE") {
        owner = "UNKNOWN";
        email = "pseudo-nadra@gmail.com";
        address = "UNKNOWN";
        return;
    }

    string file = "./Challans/Vehicle Registration.json";
    int rows = 0, cols = 0;

    try {
        getSize(file, rows, cols);

        string *arr = new string[rows];
        for(int i = 0; i < rows; i++)
            arr[i] = new string[cols];

        toArray(file, arr, rows, cols);

        owner = "UNKNOWN";
        email = "pseudo-nadra@gmail.com";
        address = "UNKNOWN";

        for(int i = 1; i < rows; i++) {
            if(arr[i][3] == vehicleNum) {
                address = arr[i][0];
                email   = arr[i][1];
                owner   = arr[i][2];
                break;
            }
        }

        for(int i = 0; i < rows; i++)
            delete[] arr[i];
        delete[] arr;

    } catch(...) {
        owner = "UNKNOWN";
        email = "pseudo-nadra@gmail.com";
        address = "UNKNOWN"
    }
}
