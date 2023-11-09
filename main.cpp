#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

// Define a class to represent a bank account
class BankAccount {
    private:
        string name;
        int accountNumber;
        string password;
        string dateOfBirth;
        double balance;
    public:
        // Constructor to initialize the account
        BankAccount(string n, int a, string p, string d, double b) {
            name = n;
            accountNumber = a;
            password = p;
            dateOfBirth = d;
            balance = b;
        }
        // Function to deposit money into the account
        void deposit(double amount) {
            balance += amount;
        }
        // Function to withdraw money from the account
        void withdraw(double amount) {
            if (balance >= amount) {
                balance -= amount;
            } else {
                cout << "Insufficient balance." << endl;
            }
        }
        // Function to display the account details
        void display() {
            cout << "Name: " << name << endl;
            cout << "Account Number: " << accountNumber << endl;
            cout << "Balance: " << balance << endl;
        }
        // Function to check if the account number and password match
        bool checkCredentials(int a, string p) {
            return (accountNumber == a && password == p);
        }
};

// Function to create a new account
void createAccount() {
    string name, password, dateOfBirth;
    int accountNumber;
    double balance;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your account number: ";
    cin >> accountNumber;
    cout << "Enter your password: ";
    cin >> password;
    cout << "Enter your date of birth: ";
    cin >> dateOfBirth;
    cout << "Enter your initial balance: ";
    cin >> balance;
    BankAccount account(name, accountNumber, password, dateOfBirth, balance);
    ofstream outfile;
    outfile.open("accounts.txt", ios::app);
    outfile << name << " " << accountNumber << " " << password << " " << dateOfBirth << " " << balance << endl;
    outfile.close();
    cout << "Account created!" << endl;
}

// Function to log in to an existing account
void login() {
    int accountNumber;
    string password;
    cout << "Enter your account number: ";
    cin >> accountNumber;
    cout << "Enter your password: ";
    cin >> password;
    ifstream infile;
    infile.open("accounts.txt");
    string n, p, d;
    int a;
    double b;
    bool found = false;
    while (infile >> n >> a >> p >> d >> b) {
        if (a == accountNumber && p == password) {
            BankAccount account(n, a, p, d, b);
            found = true;
            char choice;
            do {
                cout << "********* BANKING MENU *********" << endl << endl;
                cout << "d. Deposit Money" << endl;
                cout << "w. Withdraw Money" << endl;
                cout << "v. View Account Details" << endl;
                cout << "q. Quit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                switch (choice) {
                    case 'd':
                        double amount;
                        cout << "Enter the amount to deposit: ";
                        cin >> amount;
                        account.deposit(amount);
                        break;
                    case 'w':
                        double amount2;
                        cout << "Enter the amount to withdraw: ";
                        cin >> amount2;
                        account.withdraw(amount2);
                        break;
                    case 'v':
                        account.display();
                        break;
                    case 'q':
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            } while (choice != 'q');
        }
    }
    infile.close();
    if (!found) {
        cout << "Invalid account details. Please try again." << endl;
    }
}

// Main function
int main() {
    char choice;
    do {
        cout << "********* MAIN MENU *********" << endl << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "q. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case '1':
                login();
                break;
            case '2':
                createAccount();
                break;
            case 'q':
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 'q');
    return 0;
}
