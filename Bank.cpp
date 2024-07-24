#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>

using namespace std;

//---------------------------------------------------------------------------------------------------//

// Constants for formatting output
const int maxName = 50;       // Maximum length for account holder name
const int maxMoney = 7;       // Maximum width for money in report
const int maxNumber = 12;     // Maximum width for account number
const int typeSize = 4;       // Width for account type in report
const string sep = " |";      // Separator for report columns
const int totalLen = maxName + maxNumber + typeSize + maxMoney + sep.size() * 4; // Total length of report line
const string sepLine = sep + string(totalLen - 1, '=') + '|'; // Separator line for reports

class Bank {
private:
    unsigned long long account_number; // Account number
    string holder_name;                // Account holder name
    int deposit;                       // Deposit amount
    char type;                         // Account type ('S' for Savings, 'C' for Current)
    string password;                   // Account password

public:
    // Function declarations
    void System_clear();
    void Get_Data();
    void Write_Data();
    void deposit_withdraw(int n, int option);
    void Display_Invidual_Records(unsigned long long n);
    void report() const;
    void Show_account() const; // Display account details
    void dep(int);             // Add to deposit
    void draw(int x);          // Subtract from deposit
    unsigned long long retacno() const; // Return account number
    int retdeposit() const;    // Return balance amount
    char rettype() const;      // Return account type
    void Modification_data();  // Modify account data
    bool verifyPassword() const; // Verify password
};

// Function to verify password
bool Bank::verifyPassword() const {
    string inputPassword;
    cout << "Enter Password: ";
    cin >> inputPassword;
    return inputPassword == password;
}

// Function to get account details from the user
void Bank::Get_Data() {
    cout << "\n\n===========CREATE BANK ACCOUNT===========\n\n";
    cout << "\nEnter the Account Number: ";
    cin >> account_number;
    // Validate account number length
    while (to_string(account_number).length() != 12) { 
        cout << "The account number has to be of 12 digits. Please try again.\n";
        cin.clear();            // Clear the error flag
        cin.ignore(10000, '\n'); // Discard invalid input
        cin >> account_number;
    }
    cout << "\n\nEnter Account Holder Name: ";
    cin.ignore(); // Clear input buffer
    getline(cin, holder_name); // Read entire line for name
    // Validate name length
    while (holder_name.length() > maxName) {
        cout << "The name is too long. Please enter a shorter name: ";
        getline(cin, holder_name);
    }
    cout << "\nSet Password for the Account: ";
    cin >> password; // Set account password
    cout << "\nWhich type of Account \n[S for Saving Account] (or)[C for Current Account]: ";
    cin >> type;
    type = toupper(type); // Convert to uppercase
    // Validate account type
    while (type != 'S' && type != 'C') {
        cout << "Please enter either S or C to avoid data corruption: ";
        cin >> type;
        type = toupper(type);
    }
    if (type == 'S') { // Savings account
        cout << "\nEnter the Initial Amount for Saving Account [Minimum 500/-]: ";
        cin >> deposit;
        // Validate deposit amount
        while (deposit < 500 || deposit >= 1000000) {
            if (deposit < 500) {
                cout << "Minimum amount for Saving Account is 500/-. Please try again: ";
            } else {
                cout << "Maximum money is 1000000/-. Please try again: ";
            }
            cin >> deposit;
        }
        cout << "\n\nAccount has been Created Successfully.";
    } else if (type == 'C') { // Current account
        cout << "\nEnter the Initial Amount for Current Account [Minimum 1000/-]: ";
        cin >> deposit;
        // Validate deposit amount
        while (deposit < 1000 || deposit >= 1000000) {
            if (deposit < 1000) {
                cout << "Minimum amount for Current Account is 1000/-. Please try again: ";
            } else {
                cout << "Maximum money is 1000000/-. Please try again: ";
            }
            cin >> deposit;
        }
        cout << "\n\nAccount has been Created Successfully.";
    }
}

// Function to write account data to a file
void Bank::Write_Data() {
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app); // Open file in binary append mode
    Get_Data(); // Get account details
    outFile.write(reinterpret_cast<char*>(this), sizeof(*this)); // Write account details to file
    outFile.close();
}

// Function to modify account data
void Bank::Modification_data() {
    cout << "\nAccount No: " << account_number;
    cout << "\nModify Account Holder Name: ";
    cin.ignore();
    getline(cin, holder_name);
    // Validate name length
    while (holder_name.length() > maxName) {
        cout << "The name is too long. Please enter a shorter name: ";
        getline(cin, holder_name);
    }
    cout << "\nModify Type of Account (S/C): ";
    cin >> type;
    type = toupper(type);
    // Validate account type
    while (type != 'S' && type != 'C') {
        cout << "Invalid account type. Please enter S for Savings or C for Current: ";
        cin >> type;
        type = toupper(type);
    }
    cout << "\nModify Total Balance Amount: ";
    cin >> deposit;
    // Validate deposit amount
    while (deposit >= 1000000) {
        cout << "Maximum money is 1000000/-. Please try again: ";
        cin >> deposit;
    }
    cout << "\nModify Password: ";
    cin >> password;
}

// Function to display individual account records
void Bank::Display_Invidual_Records(unsigned long long n) {
    bool flag = false; // Flag to check if record is found
    ifstream inFile;
    inFile.open("account.dat", ios::binary); // Open file in binary mode
    if (!inFile) {
        cout << "\n\nAn Error Occurred. Please try again.";
        return;
    }
    cout << "\nBALANCE DETAILS\n";

    // Read account records from file
    while (inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
        if (retacno() == n) { // If account number matches
            if (verifyPassword()) { // Verify password
                Show_account();  // Display account details
                flag = true;
            } else {
                cout << "\nIncorrect Password.";
                flag = true;
            }
        }
    }
    inFile.close();
    if (!flag) {
        cout << "\n\nAccounts Details not Found.";
    }
}

// Function to display account details in report format
void Bank::report() const {
    cout << sep
         << setw(maxNumber) << account_number << sep
         << setw(maxName) << holder_name << sep
         << setw(typeSize) << type << sep
         << setw(maxMoney) << deposit << sep << endl << sepLine << endl;
}

// Function to deposit amount into account
void Bank::dep(int x) {
    deposit += x;
}

// Function to withdraw amount from account
void Bank::draw(int x) {
    deposit -= x;
}

// Function to display account details
void Bank::Show_account() const {
    cout << "\nAccount No: " << account_number;
    cout << "\nAccount Holder Name: ";
    cout << holder_name;
    if (type == 'S') {
        cout << "\nType of Account: Saving ";
    } else {
        cout << "\nType of Account: Current ";
    }
    cout << "\nTotal Balance: " << deposit;
}

// Function to return account number
unsigned long long Bank::retacno() const {
    return account_number;
}

// Function to return deposit amount
int Bank::retdeposit() const {
    return deposit;
}

// Function to return account type
char Bank::rettype() const {
    return type;
}

// Function to clear the console screen (system dependent)
// Uncomment if you want to use this on Windows
void Bank::System_clear() {
    // system("cls"); 
}

//-------------------------------------------------------------------------------------------------------//

// Function declarations
void Delete_Account(unsigned long long);
void system_clear();
void Get_All_Data();
void Deposit_Withdraw(unsigned long long, int);
void Credit();
void Account_Modification(unsigned long long);

int main() {
    char my_url;
    char choice;
    unsigned long long account_number;
    Bank call;
    Credit(); // Display project credit
    do {
        system_clear(); // Clear screen
        cout << "\n===========================";
        cout << "\n  BANKING RECORD SYSTEM";
        cout << "\n===========================\n\n";
        cout << "\n\n01. CREATE NEW ACCOUNT";
        cout << "\n\n02. MODIFY AN ACCOUNT";
        cout << "\n\n03. BALANCE ENQUIRY OF ACCOUNT";
        cout << "\n\n04. DEPOSIT IN ACCOUNT";
        cout << "\n\n05. WITHDRAW FROM ACCOUNT";
        cout << "\n\n06. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n07. CLOSE AN ACCOUNT";
        cout << "\n\n08. EXIT FROM PROGRAM";
        cout << "\n\nSELECT THE OPTION: ";
        cin >> choice;
        system_clear();
        switch (choice) {
        case '1':
            call.Write_Data();
            break;
        case '2':
            cout << "\n\n\tAccount No: ";
            cin >> account_number;
            Account_Modification(account_number);
            break;
        case '3':
            cout << "\n\n\tAccount No: ";
            cin >> account_number;
            call.Display_Invidual_Records(account_number);
            break;
        case '4':
            cout << "\n\n\tAccount No: ";
            cin >> account_number;
            Deposit_Withdraw(account_number, 1);
            break;
        case '5':
            cout << "\n\n\tAccount No: ";
            cin >> account_number;
            Deposit_Withdraw(account_number, 2);
            break;
        case '6':
            Get_All_Data();
            break;
        case '7':
            cout << "\n\n\tAccount No: ";
            cin >> account_number;
            Delete_Account(account_number);
            break;
        case '8':
            cout << "\n\nThank you for using the Banking Record System. Goodbye!";
            break;
        default:
            cout << "\n\nInvalid option. Please select a valid option.";
            break;
        }
        cin.ignore();
        cin.get();
    } while (choice != '8'); // Exit the program when '8' is selected
    return 0;
}

// Function to clear the screen (system dependent)
void system_clear() {
    // Uncomment if you want to use this on Windows
    // system("cls");
}

// Function to delete an account
void Delete_Account(unsigned long long n) {
    Bank call;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\n\nAn Error Occurred. Please try again.";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg); // Set position to beginning of file
    while (inFile.read(reinterpret_cast<char*>(&call), sizeof(Bank))) {
        if (call.retacno() != n) {
            outFile.write(reinterpret_cast<char*>(&call), sizeof(Bank)); // Copy all except the target account
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\nAccount deleted successfully.";
}

// Function to retrieve all account data
void Get_All_Data() {
    Bank call;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\n\nAn Error Occurred. Please try again.";
        return;
    }
    cout << "\n\n\tACCOUNT HOLDER LIST\n\n";
    cout << sepLine << endl;
    cout << sep
         << setw(maxNumber) << "A/C No" << sep
         << setw(maxName) << "NAME" << sep
         << setw(typeSize) << "Type" << sep
         << setw(maxMoney) << "Balance" << sep << endl;
    cout << sepLine << endl;
    while (inFile.read(reinterpret_cast<char*>(&call), sizeof(Bank))) {
        call.report();
    }
    inFile.close();
}

// Function to deposit or withdraw money from an account
void Deposit_Withdraw(unsigned long long n, int option) {
    int amt;
    bool found = false;
    Bank call;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "\n\nAn Error Occurred. Please try again.";
        return;
    }
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char*>(&call), sizeof(Bank));
        if (call.retacno() == n) {
            if (call.verifyPassword()) { // Verify password
                call.Show_account();
                if (option == 1) { // Deposit
                    cout << "\n\n\tTO DEPOSITSS AMOUNT ";
                    cout << "\n\nEnter the amount to be deposited: ";
                    cin >> amt;
                    call.dep(amt);
                }
                if (option == 2) { // Withdraw
                    cout << "\n\n\tTO WITHDRAW AMOUNT ";
                    cout << "\n\nEnter the amount to be withdrawn: ";
                    cin >> amt;
                    int balance = call.retdeposit() - amt;
                    if ((balance < 500 && call.rettype() == 'S') || (balance < 1000 && call.rettype() == 'C')) {
                        cout << "Insufficient balance.";
                    } else {
                        call.draw(amt);
                    }
                }
                int pos = (-1) * static_cast<int>(sizeof(call)); // Move pointer back to update record
                File.seekp(pos, ios::cur);
                File.write(reinterpret_cast<char*>(&call), sizeof(Bank)); // Update the record
                cout << "\n\nAccount updated successfully.";
            } else {
                cout << "\nIncorrect Password.";
            }
            found = true;
        }
    }
    File.close();
    if (!found) {
        cout << "\n\nAccount Not Found.";
    }
}

// Function to display project credit
void Credit() {
    cout << "\n\n\tProject by: YOUR NAME\n\n";
}

// Function to modify account details
void Account_Modification(unsigned long long n) {
    bool found = false;
    Bank call;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "\n\nAn Error Occurred. Please try again.";
        return;
    }
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char*>(&call), sizeof(Bank));
        if (call.retacno() == n) {
            if (call.verifyPassword()) { // Verify password
                call.Show_account();
                cout << "\n\nEnter The New Details of account: " << endl;
                call.Modification_data();
                int pos = (-1) * static_cast<int>(sizeof(Bank));
                File.seekp(pos, ios::cur);
                File.write(reinterpret_cast<char*>(&call), sizeof(Bank));
                cout << "\n\n\tRecord Updated Successfully.";
            } else {
                cout << "\nIncorrect Password.";
            }
            found = true;
        }
    }
    File.close();
    if (!found) {
        cout << "\n\n\tRecord Not Found.";
    }
}

