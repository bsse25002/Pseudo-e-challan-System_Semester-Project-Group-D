#include "functions.h"

//**********Registrations**********\\

//Vehicle
struct Vehicle {
    string vehicleNum;
    string wheels;
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
    string address;
};

void to_json(json& j, const Account& a) {
    j = json{{"owner", a.owner}, 
        {"email", a.email}, 
        {"address", a.address}};
}

void from_json(const json& j, Account& a) {
    j.at("owner").get_to(a.owner);
    j.at("email").get_to(a.email);
    j.at("address").get_to(a.address);
}

void vehicleRegistration(string vehicleNum,string wheels, string owner,string email,string address) {
    Vehicle vehicle{vehicleNum, wheels, owner, email, address};

    json j;
    ifstream read("./Registrations/Vehicle Registration.json");
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

    ofstream write("./Registrations/Vehicle Registration.json");
    write << j.dump(4);
    write.close();

    cout << "Vehicle Registered Successfully!" << endl;
}

void accRegistration(string owner,string email,string address) {
    Account acc{owner, email, address};

    json j;
    ifstream read("./Registrations/Accounts.json");
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

    ofstream outFile("./Registrations/Accounts.json");
    outFile << j.dump(4);
    outFile.close();

    cout << "Account registered successfully!" << endl;
}