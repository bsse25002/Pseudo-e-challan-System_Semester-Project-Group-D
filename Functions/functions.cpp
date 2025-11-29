#include "functions.h"

//**********Registrations**********\\

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

//Registered Vehicles
void registeredVehicles() {
    json j;
    ifstream read("./Challans/Vehicle Registration.json");
    if (read.is_open()) {
        read >> j;
        read.close();
    }

    int rows = 0;
    for (auto& entry : j) {
        rows++;
    }

    int cols = 5; // email, owner, Vehicle Number, Type(Wheels), address

    string** table = new string*[rows];
    for (int i = 0; i < rows; i++)
        table[i] = new string[cols];

    int r = 0;
    for (auto& entry : j) {
        table[r][0] = entry["email"];
        table[r][1] = entry["owner"];
        table[r][2] = entry["vehicleNum"];
        table[r][3] = "\t\t" + to_string(entry["wheels"].get<int>());
        table[r][4] = entry["address"];
        r++;
    }

    // Print table using escape sequences (\t)
    cout << "E-Mail\t\t\tOwner\t\tVehicle Number\tType(Wheels)\tAddress\n";
    cout << "--------------------------------------------------------------------------------\n";
    for (int i = 0; i < rows; i++) {
        for (int c = 0; c < cols; c++) {
            cout << table[i][c] << "\t";
        }
        cout << "\n";
    }

    for (int i = 0; i < rows; i++) {
        delete[] table[i];
    }

    delete[] table;
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