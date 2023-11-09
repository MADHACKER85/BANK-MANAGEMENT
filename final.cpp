#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class BankAccount {
public:
    string accountNumber;
    string name;
    string password;
    string dateOfBirth;
    double balance;

public:
    BankAccount() : balance(0.0) {}

    BankAccount(const string& accNum, const string& n, const string& pw, const string& dob, double& balance)
        : accountNumber(accNum), name(n), password(pw), dateOfBirth(dob), balance(0.0) {}

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit successful. New balance: " << balance << endl;
        }
        else {
            throw invalid_argument("Invalid deposit amount. Amount must be positive.");
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful. New balance: " << balance << endl;
        }
        else {
            throw invalid_argument("Invalid withdrawal amount or insufficient balance.");
        }
    }

    void displayDetails() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Date of Birth: " << dateOfBirth << endl;
        cout << "Balance: " << balance << endl;
    }

    const string& getName() const {
        return name;
    }

    friend istream& operator>>(istream& is, BankAccount& account) {
        cout << "Enter Account Number: ";
        is >> account.accountNumber;
        cout << "Enter Name: ";
        is.ignore();
        getline(is, account.name);
        cout << "Enter Password: ";
        is >> account.password;
        cout << "Enter Date of Birth: ";
        is.ignore();
        getline(is, account.dateOfBirth);
        return is;
    }

    friend ostream& operator<<(ostream& os, const BankAccount& account) {
        os << "Account Number: " << account.accountNumber << endl;
        os << "Name: " << account.name << endl;
        os << "Date of Birth: " << account.dateOfBirth << endl;
        os << "Balance: " << account.balance << endl;
        return os;
    }

    const string& getAccountNumber() const {
        return accountNumber;
    }

    const string& getPassword() const {
        return password;
    }
};

class Manager {
protected:
    string bankID;
    string managerPassword;

public:
    Manager(const string& id, const string& pw) : bankID(id), managerPassword(pw) {}

    bool authenticate(const string& id, const string& pw) const {
        return (bankID == id && managerPassword == pw);
    }

    virtual void viewAccountDetails(const BankAccount& account) const = 0;
};

class Register : public Manager {
public:
    Register(const string& id, const string& pw) : Manager(id, pw) {}

    BankAccount* registerAccount() {
        BankAccount* newAccount = new BankAccount();
        cin >> *newAccount;
        cout << "Account created successfully." << endl;
        return newAccount;
    }

    void viewAccountDetails(const BankAccount& account) const override {
        cout << "Account Details:" << endl;
        cout << account;
    }
};

class Login : public Register {
public:
    Login(const string& id, const string& pw) : Register(id, pw) {}

    BankAccount* login(BankAccount** accounts, int numAccounts, const string& accountNumber, const string& password) {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accountNumber && accounts[i]->getPassword() == password) {
                return accounts[i];
            }
        }
        return nullptr;
    }

    void handleLoggedIn(BankAccount* account) {
        while (true) {
            int choice;
            cout << "\nLogged In as: " << account->getName() << endl;
            cout << "1. Deposit" << endl;
            cout << "2. Withdraw" << endl;
            cout << "3. Display Account Details" << endl;
            cout << "4. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                double amount;
                cout << "Enter the amount to deposit: ";
                cin >> amount;
                try {
                    account->deposit(amount);
                }
                catch (const exception& ex) {
                    cout << "Error: " << ex.what() << endl;
                }
                break;
            }
            case 2: {
                double amount;
                cout << "Enter the amount to withdraw: ";
                cin >> amount;
                try {
                    account->withdraw(amount);
                }
                catch (const exception& ex) {
                    cout << "Error: " << ex.what() << endl;
                }
                break;
            }
            case 3: {
                account->displayDetails();
                break;
            }
            case 4: {
                cout << "Logging out..." << endl;
                return;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }
};
// Function to save account details to a file
void saveAccountDetails(const BankAccount& account) {
    ofstream outFile("accounts.txt", ios::app);
    if (outFile.is_open()) {
        outFile << account.getAccountNumber() << ","
                << account.getName() << ","
                << account.getPassword() << ","
                << account.dateOfBirth << ","
                << account.balance << "\n";
        outFile.close();
    }
    else {
        cout << "Error: Unable to save account details." << endl;
    }
}

// Function to read account details from the file
void loadAccountDetails(BankAccount* Accounts[], int& numAccounts) {
    ifstream inFile("accounts.txt");
    if (inFile.is_open()) {
        string line = "hello";
        while (getline(inFile, line)) {
            stringstream ss(line);
            string accNum, name, password, dob;
            double balance;
            getline(ss, accNum, ',');
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, dob, ',');
            ss>>balance;
            Accounts[numAccounts++] = new BankAccount(accNum, name, password, dob, balance);
        }
        inFile.close();
    }
    else {
        cout << "Error: Unable to load account details." << endl;
    }
}

// Function to save manager credentials to a file
void saveManagerCredentials(const string& bankID, const string& managerPassword) {
    ofstream outFile("manager.txt");
    if (outFile.is_open()) {
        outFile << bankID << "," << managerPassword << "\n";
        outFile.close();
    }
    else {
        cout << "Error: Unable to save manager credentials." << endl;
    }
}

// Function to read manager credentials from the file
void loadManagerCredentials(string& bankID, string& managerPassword) {
    ifstream inFile("manager.txt");
    if (inFile.is_open()) {
        getline(inFile, bankID, ',');
        getline(inFile, managerPassword, ',');
        inFile.close();
    }
    else {
        cout << "Error: Unable to load manager credentials." << endl;
    }
}

int main() {
    const int MAX_ACCOUNTS = 100;
    BankAccount* accounts[MAX_ACCOUNTS];
    int numAccounts = 0;
    string bankID, managerPassword;
    loadManagerCredentials(bankID, managerPassword);

    Login login(bankID, managerPassword);

    loadAccountDetails(accounts, numAccounts);


    //string bankID, managerPassword;
    cout << "Enter Bank ID: ";
    cin >> bankID;
    cout << "Enter Manager Password: ";
    cin >> managerPassword;

    //Login login(bankID, managerPassword);

    while (true) {
        int choice;
        cout << "\nWelcome to the Bank Account Management System!" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Manager Login" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string accountNumber, password;
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Password: ";
            cin >> password;
            BankAccount* account = login.login(accounts, numAccounts, accountNumber, password);
            if (account != nullptr) {
                login.handleLoggedIn(account);
            }
            else {
                cout << "Invalid account details. Please login again or register." << endl;
            }
            break;
        }
        case 2: {
            if (numAccounts < MAX_ACCOUNTS) {
                Register registerManager(bankID, managerPassword);
                accounts[numAccounts] = registerManager.registerAccount();
                ++numAccounts;
            }
            else {
                cout << "Maximum number of accounts reached. Cannot register more accounts." << endl;
            }
            break;
        }
          case 3: {
    string managerBankID, managerPassword;
    cout << "Manager Login" << endl;
    cout << "Enter Bank ID: ";
    cin >> managerBankID;
    cout << "Enter Manager Password: ";
    cin >> managerPassword;
    if (login.authenticate(managerBankID, managerPassword)) {
        string accountNumber;
        cout << "Enter the account number to view details: ";
        cin >> accountNumber;
        BankAccount* account = nullptr;
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accountNumber) {
                account = accounts[i];
                break;
            }
        }
        if (account != nullptr) {
            login.viewAccountDetails(*account);
        }
        else {
            cout << "Account not found." << endl;
        }
    }
    else {
        cout << "Invalid manager credentials." << endl;
    }
    break;
}
        case 4: {
            cout << "Exiting the Bank Account Management System. Goodbye!" << endl;
            // Release the allocated memory for accounts
            for (int i = 0; i < numAccounts; ++i) {
                delete accounts[i];
            }
            return 0;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    // Release the allocated memory for accounts
    for (int i = 0; i < numAccounts; ++i) {
        saveAccountDetails(*accounts[i]);
        delete accounts[i];
    }
    saveManagerCredentials(bankID, managerPassword);

    return 0;
}
