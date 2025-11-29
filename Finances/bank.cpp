#include "../Functions/functions.h"

void bankMenu(string email) {
    cout << "\tWhat type of Transaction you want to do?\n\t\t0) Close The Bank\n\t\t1) Deposit Money\n\t\t2) Credit Money" << endl;

    int choice;
    cin >> choice;

    if (choice == 0) { //Recursion Base Case
        cout << "Thank you for Using our Bank!" << endl;
        return;
    } else if (choice == 1) {
        cout << "Money Deposit Selected!" << endl;

        int deposit;
        
        cout << "Enter the amount of Money you want to Deposit: ";
        cin >> deposit;

        moneyDeposit(email, deposit);
    } else if (choice == 2) {
        cout << "Money Credit Selected!" << endl;

        int credit;
        
        cout << "Enter the amount of Money you want to Credit: ";
        cin >> credit;

        moneyCredit(email, credit);
    } else {
        cout << "Invalid Choice! Try Again" << endl;
    }

    bankMenu(email); //Recursiive Call
}

int main() {
    cout << "\tWelcome to Pseudo-Bank!" << endl;

    string email, password;

    cout << "Enter Your E-Mail you have given to Bank: ";
    cin >> email;

    if(accExist(email)) {
        cout << "Enter your Bank Password: ";
        cin >> password;
        
        if(bankLogin(email, password)) {
            bankMenu(email);
        } else {
            cout << "Incorrect Password, Login Failed!" << endl;
        }
    } else {
        cout << "Account Does not Exist. Please Register First!" << endl;
    }

    return 0;
}