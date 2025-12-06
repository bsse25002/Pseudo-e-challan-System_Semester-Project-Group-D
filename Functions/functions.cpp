#include "functions.h"

//**********Pseudo-Databses**********\\

//String to Integer (Handling String Arrays)
int stringToInt(string num) {
    int sign = 1, result = 0;

    if(num == "") {
        return 0;
    }

    int i = 0;
    if(num[i] == '-') {
        sign = -1;
        i++;
    }

    for(;(num[i] != '\0') && (num[i] >= '0' && num[i] <= '9');i++) {
        result = result*10 + (num[i] - '0');
    }

    return result*sign;
}

//Vehicle
struct Vehicle {
    string vehicleNum;
    int wheels;
    string owner;
    string email;
    string address;
};

void to_json(json& j, const Vehicle& v) {
    j = json{{"vehicleNum", v.vehicleNum},
            {"wheels", v.wheels}, 
            {"owner", v.owner}, 
            {"email", v.email}, 
            {"address", v.address}};
}

void from_json(const json& j, Vehicle& v) {
    j.at("vehicleNum").get_to(v.vehicleNum);
    j.at("wheels").get_to(v.wheels);
    j.at("owner").get_to(v.owner);
    j.at("email").get_to(v.email);
    j.at("address").get_to(v.address);
}

//Account
struct Account {
    string owner;
    string email;
    string password;
    string address;
    int money;
};

void to_json(json& j, const Account& a) {
    j = json{{"owner", a.owner}, 
        {"email", a.email},
        {"password", a.password}, 
        {"address", a.address},
        {"money", a.money}};
}

void from_json(const json& j, Account& a) {
    j.at("owner").get_to(a.owner);
    j.at("email").get_to(a.email);
    j.at("password").get_to(a.password);
    j.at("address").get_to(a.address);
    j.at("money").get_to(a.money);
}

//Challans
struct Challans {
    string vehicleNum;
    int challanNo;
    string owner;
    string email;
    string address;
    int amount;
};

void to_json(json& j, const Challans& c) {
    j = json{{"vehicleNum", c.vehicleNum},
            {"challanNo", c.challanNo},
            {"owner", c.owner}, 
            {"email", c.email}, 
            {"address", c.address},
            {"amount", c.amount}};
}

void from_json(const json& j, Challans& c) {
    j.at("vehicleNum").get_to(c.vehicleNum);
    j.at("challanNo").get_to(c.challanNo);
    j.at("owner").get_to(c.owner);
    j.at("email").get_to(c.email);
    j.at("address").get_to(c.address);
    j.at("amount").get_to(c.amount);
}

//Size of Array for Json
void getSize(string file, int &rows, int &cols) {
    json j;
    ifstream read(file);

    if (read.is_open())
        read >> j;

    read.close();

    rows = 1;
    for (auto &entry : j)
        rows++;

    cols = 0;
    for (auto &item : j.at(0).items())
        cols++;
}

//Json to Dynamic 2D Array
void toArray(string file, string **arr, int rows, int cols) {
    json j;
    ifstream read(file);

    if (read.is_open())
        read >> j;

    read.close();

    int c = 0;
    for (auto &item : j.at(0).items())
        arr[0][c++] = item.key();

    for (int r = 1; r < rows; r++) {
        int col = 0;
        for (auto &item : j.at(r - 1).items())
            if(item.value().is_string()) {
                arr[r][col++] = item.value().get<string>();
            }
            else {
                arr[r][col++] = to_string(item.value().get<int>());
            }
    }
}

//User Defined Vectors
class Vector {
    public:
        int size;
        int capacity;
        int *arr;

        Vector() {
            size = 0;
            capacity = 1;
            arr = new int[1];
        }

        void add(int ele) {
            if(size == capacity) {
                capacity *= 2;
                int *temp = new int[capacity];
                for(int i = 0;i < size;i++) {
                    temp[i] = arr[i];
                }

                arr = temp;
            }

            arr[size++] = ele;
        }

        void remove() {
            if(size == 0) {
                cout << "Empty Vector" << endl;
            }

            size--;
        }

        int get(int idx) {
            if(size == 0) {
                cout << "Empty Vector" << endl;
                return -1;
            }
            if(idx >= size || idx < 0) {
                cout << "Invalid Index" << endl;
                return -1;
            }

            return arr[idx];
        }

        int Size() {
            return size;
        }
};


//**********Admin Controls**********\\

//check if admin exists
bool adminExist(string admin) {
    json j;
    ifstream read("./Admin/Admin.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (const auto& entry : j) {
        if (entry["admin"] == admin) {
            return true;
        }
    }

    return false;
}

//Check Password
bool adminLogin(string admin, string password) {
    json j;
    ifstream read("./Admin/Admin.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (const auto& entry : j) {
        if (entry["admin"] == admin && entry["password"] == password) {  
            return true;
        }
    }

    return false;
}


//**********Registrations**********\\

//Vehicle Registration
void vehicleRegistration(string vehicleNum,int wheels, string owner,string email,string address) {
    Vehicle vehicle{vehicleNum, wheels, owner, email, address};

    json j;
    ifstream read("./Challans/Vehicle Registration.json");
    if(read.is_open()) {
        read >> j;
        read.close();
    }

    bool exists = false;
    for (const auto& entry : j) {
        if (entry["vehicleNum"] == vehicle.vehicleNum) {
            exists = true;
            break;
        }
    }

    if (exists) {
        cout << "Vehicle with number " << vehicleNum << " is already registered!" << endl;
        return;
    }

    j.push_back(vehicle);

    ofstream write("./Challans/Vehicle Registration.json");
    write << j.dump(4);
    write.close();

    cout << "Vehicle Registered Successfully!" << endl;
}

//Account Registration
void accRegistration(string owner, string email, string password, string address, int money) {
    Account acc{owner, email, password, address, money};

    json j;
    ifstream read("./Finances/Accounts.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    bool exists = false;
    for (const auto& entry : j) {
        if (entry["email"] == acc.email) {
            exists = true;
            break;
        }
    }

    if (exists) {
        cout << "Account with email " << email << " already exists!" << endl;
        return;
    }

    j.push_back(acc);

    ofstream outFile("./Finances/Accounts.json");
    outFile << j.dump(4);
    outFile.close();

    cout << "Account registered successfully!" << endl;
}


//**********Pseudo-Bank System**********\\

//Check if Account Exist
bool accExist(const string& email) {
    json j;
    ifstream read("./Finances/Accounts.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (const auto& entry : j) {
        if (entry["email"] == email) {
            return true;
        }
    }

    return false;
}

//Check Password
bool bankLogin(string email, string password) {
    json j;
    ifstream read("./Finances/Accounts.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (const auto& entry : j) {
        if (entry["email"] == email && entry["password"] == password) {  
            return true;
        }
    }

    return false;
}

//Money Deposit
void moneyDeposit(string email, int deposit) {
    json j;
    ifstream read("./Finances/Accounts.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (auto& entry : j) {
        if (entry["email"] == email) {  
            entry["money"] = entry["money"].get<int>()+deposit;
        }
    }

    ofstream write("./Finances/Accounts.json");
    if (write.is_open()) {
        write << j.dump(4);
        write.close();
    }
}

//Money Credit
void moneyCredit(string email, int credit) {
    json j;
    ifstream read("./Finances/Accounts.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    for (auto& entry : j) {
        if (entry["email"] == email) {  
            entry["money"] = entry["money"].get<int>()-credit;
        }
    }

    ofstream write("./Finances/Accounts.json");
    if (write.is_open()) {
        write << j.dump(4);
        write.close();
    }
}

//Challan Payment
void payment(string email, string **acc, int rowsAcc, string **challan, int rowsCh) {
    int rowAcc = -1, rowCh = -1, colAcc = 0, colCh = 0;
    
    for(int i = 1;i < rowsAcc;i++) {
        if(acc[i][1] == email) {
            rowAcc = i;
            break;
        }
    }

    if(rowAcc == -1) {
        cout << "No Account Against: " << email << endl;
        return;
    }

    for(int i = 0;i < rowsCh;i++) {
        if(challan[i][3] == email) {
            rowCh = i;
            break;
        }
    }

    if(rowCh == -1) {
        cout << "No Challan against email: " << email << endl;
        return;
    }

    int money = stringToInt(acc[rowAcc][2]);
    int fine  = stringToInt(challan[rowCh][1]);

    if(money < fine) {
        acc[rowAcc][2] = "0";
        challan[rowCh][1] = to_string(fine - money);
        cout << "Payment Successful!\tRemaining Fine: " << challan[rowCh][1] << endl;
    } else {
        challan[rowCh][1] = "0";
        acc[rowAcc][2] = to_string(money - fine);
        cout << "Payment Successful!\tAll Dues Cleared." << endl;
    }

    Account account;
    Challans newChallan;

    json jAcc;

    for(int i = 1;i < rowsAcc;i++) {
        account.address = acc[i][0];
        account.email = acc[i][1];
        account.money = stringToInt(acc[i][2]);
        account.owner = acc[i][3];
        account.password = acc[i][4];

        jAcc.push_back(account);
    }

    ofstream writeAcc("./Finances/Accounts.json");
    writeAcc << jAcc.dump(4);
    writeAcc.close();

    json jCh;

    for(int i = 1;i < rowsCh;i++) {
        if(challan[i][1] == "0") {
            continue;
        }

        newChallan.address = challan[i][0];
        newChallan.amount = stringToInt(challan[i][1]);
        newChallan.challanNo = stringToInt(challan[i][2]);
        newChallan.email = challan[i][3];
        newChallan.owner = challan[i][4];
        newChallan.vehicleNum = challan[i][5];

        jCh.push_back(newChallan);
    }

    ofstream writeCh("./Challans/Challans.json");
    writeCh << jCh.dump(4);
    writeCh.close();
}


//**********E-Challan**********\\

string vehicleExist(string vehicleNum) {
    string Nadra = "Pseudo-Nadra@gmail.com", email;

    string file = "./Challans/Vehicle Registration.json";
    bool vehicle = false;
        
        int rows, cols;
        getSize(file, rows, cols);

        string **arr = new string*[rows];
        for (int i = 0; i < rows; i++)
            arr[i] = new string[cols];

        toArray(file, arr, rows, cols);

        for(int i = 1;i < rows;i++) {
            if(arr[i][3] == vehicleNum) {
                email = arr[i][1];
                vehicle = true;
            }
        }

        if(vehicle) {
            return email;
        } else {
            return Nadra;
        }

        for (int i = 0; i < rows; i++)
            delete[] arr[i];
        delete[] arr;

        return "";
}