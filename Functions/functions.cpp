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