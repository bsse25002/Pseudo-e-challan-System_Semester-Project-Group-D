#include "functions.h"

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
            if((entry["money"].get<int>()-credit) >= 0) {
                entry["money"] = entry["money"].get<int>()-credit;
                sendEmailAsync(email, "Money Credit Notification", "An amount of " + to_string(credit) + " is creditted from your account!");
            } else {
                cout << "Not Enough Money!" << endl;
                sendEmailAsync(email, "Money Credit Limit Exceed Notification", "Not Enough Money to Credit!");
            }
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

    for(int i = 0;i < rowsCh;i++) {
        if(challan[i][3] == email) {
            rowCh = i;
            break;
        }
    }

    if(rowCh == -1) {
        cout << "No Challan against email: " << email << endl;
        sendEmailAsync(email, "Challan Payment", "You Currently have No Challans!");
        return;
    }

    int money = stringToInt(acc[rowAcc][2]);
    int fine  = stringToInt(challan[rowCh][1]);

    if(money < fine) {
        acc[rowAcc][2] = "0";
        challan[rowCh][1] = to_string(fine - money);
        cout << "Payment Successful!\tRemaining Fine: " << challan[rowCh][1] << endl;
        sendEmailAsync(email, "Challan Payment", "Payment Successful!\tRemaining Fine: " + challan[rowCh][1]);
    } else {
        challan[rowCh][1] = "0";
        acc[rowAcc][2] = to_string(money - fine);
        cout << "Payment Successful!\tAll Dues Cleared." << endl;
        sendEmailAsync(email, "Challan Payment", "Payment Successful!\tAll Dues Cleared");
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