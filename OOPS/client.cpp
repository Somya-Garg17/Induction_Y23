#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

void welcome();
void login_as_branch_manager();
void sign_up();
void login_as_account_holder();
void menu(string username, string password);
void operations(string num, double balance, string openingDate, string type);

struct Transaction
{
    string date;
    string type;
    int amount;
};

struct Account
{
    string accountNumber;
    string accountType;
    string openingDate;
    vector<Transaction> transactions;
    double balance;
    double interestRate;
};

struct User
{
    string username;
    string password;
    string userType;
    vector<Account> accounts;
};

vector<User> accountHolders;
vector<User> branchManagers;

User getUserByUsername(const string &username)
{
    for (const auto &user : accountHolders)
    {
        if (user.username == username)
        {
            return user;
        }
    }
    return User();
}
void updateDataFile(const string &accountNumber, double balance, const string &transactionType, double transactionAmount)
{
    ifstream inFile("data.txt");
    ofstream tempFile("temp.txt");

    if (!inFile.is_open() || !tempFile.is_open())
    {
        cerr << "Error: Unable to open files for updating data" << endl;
        return;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);
    string transactionDate = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string user, password, type, currentAccountNumber, accountType, openingDate;
        double currentBalance, interestRate;
        ss >> user >> password >> type >> currentAccountNumber >> accountType >> openingDate >> currentBalance >> interestRate;
        if (currentAccountNumber == accountNumber)
        {
            tempFile << user << " " << password << " " << type << " " << currentAccountNumber << " " << accountType << " " << openingDate << " " << balance << " " << interestRate << endl;
            tempFile << transactionDate << " " << transactionType << " " << transactionAmount << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    remove("data.txt");
    rename("temp.txt", "data.txt");
}

class BranchManager
{
private:
    string username;
    string password;

public:
    BranchManager(const string &uname, const string &pwd) : username(uname), password(pwd) {}

    void getStatementOfAccount(const string &accountNumber)

    {
        ifstream inFile("data.txt");

        if (!inFile.is_open())
        {
            cerr << "Error: Unable to open file data.txt" << endl;
            return;
        }

        string line;
        bool accountFound = false;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string username, password, userType, currentAccountNumber, accountType, openingDate;
            double balance, rate;
            ss >> username >> password >> userType;
            ss >> currentAccountNumber >> accountType >> openingDate >> balance >> rate;
            if (currentAccountNumber == accountNumber && userType == "AccountHolder")
            {
                accountFound = true;
                cout << "Username: " << username << endl;
                cout << "Account Number: " << currentAccountNumber << endl;
                cout << "Account Type: " << accountType << endl;
                cout << "Opening Date: " << openingDate << endl;
                cout << "Balance: " << balance << endl;
                cout << "Interest Rate: " << rate << endl;
                cout << "Transaction History:" << endl;
                cout << "Date      Type    Amount" << endl;
                while (getline(inFile, line))
                {
                    stringstream nextLine(line);
                    string word;
                    int argCount = 0;
                    while (nextLine >> word)
                    {
                        argCount++;
                    }
                    if (argCount > 3)
                    {
                        inFile.seekg(-(line.length() + 1), ios::cur);
                        break;
                    }
                    cout << line << endl;
                }

                cout << endl;
                break;
            }
        }

        inFile.close();

        if (!accountFound)
        {
            cout << "Account not found." << endl;
        }
    }

    void getAccountHolders()
    {
        ifstream inFile("data.txt");

        if (!inFile.is_open())
        {
            cerr << "Error: Unable to open file data.txt" << endl;
            return;
        }

        string line;
        cout << "Account Holders:" << endl;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string username, password, userType, currentAccountNumber, accountType, openingDate;
            double balance, rate;
            ss >> username >> password >> userType;
            ss >> currentAccountNumber >> accountType >> openingDate >> balance >> rate;
            if (currentAccountNumber != "")
            {
                cout << "Username: " << username << ", Account Number: " << currentAccountNumber << endl;
            }
        }

        inFile.close();
    }
};

class BankAccount
{
private:
    string accountNumber;
    double balance;
    string openDate;
    string accountType;

public:
    BankAccount(const string &accNumber, double initialBalance, const string &openDate, const string &type)
        : accountNumber(accNumber), balance(initialBalance), openDate(openDate), accountType(type) {}

    void deposit(double amount)
    {
        balance += amount;
        cout << "Deposit of $" << amount << " successful." << endl;
        updateDataFile(accountNumber, balance, "Deposit", amount);
        cout << "Directing you back to the operations you can perform...." << endl;
        operations(accountNumber, balance, openDate, accountType);
    }

    void withdraw(double amount)
    {
        if (balance >= amount)
        {
            balance -= amount;
            updateDataFile(accountNumber, balance, "Withdraw", amount);
            cout << "Withdrawal of $" << amount << " successful." << endl;
            cout << "Directing you back to the operations you can perform...." << endl;
            operations(accountNumber, balance, openDate, accountType);
        }
        else
        {
            cout << "Insufficient funds. Withdrawal unsuccessful." << endl;
            cout << "Directing you back to the operations you can perform...." << endl;
            operations(accountNumber, balance, openDate, accountType);
        }
    }

    void transfer(double amount)
    {
        string recipientAccountNo;
        BankAccount recipient(recipientAccountNo, 0, "", "");
        cout << "Enter the account number you want to send money to:";
        cin >> recipientAccountNo;
        bool accountFound = false;
        ifstream file("data.txt");
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string existingUsername, password, userType, accountno;
                ss >> existingUsername >> password >> userType >> accountno;
                if (recipientAccountNo == accountno)
                {
                    accountFound = true;
                    break;
                }
            }
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open file data.txt for reading" << endl;
            return;
        }

        if (!accountFound)
        {
            cout << "Account not found. Directing you back to the operations you can perform...." << endl;
            operations(accountNumber, balance, openDate, accountType);
            return;
        }
        if (balance >= amount)
        {
            balance -= amount;
            updateDataFile(accountNumber, balance, "Withdraw", amount);
            cout << "Transfer of $" << amount << " successful." << endl;
            recipient.balance += amount;
            updateDataFile(recipientAccountNo, recipient.balance, "Deposit", amount);
            cout << "Directing you back to the operations you can perform...." << endl;
            operations(accountNumber, balance, openDate, accountType);
        }
        else
        {
            cout << "Insufficient funds. Transfer unsuccessful." << endl;
            cout << "Directing you back to the operations you can perform...." << endl;
            operations(accountNumber, balance, openDate, accountType);
        }
    }
};
class SavingsAccount : public BankAccount
{
private:
    double interestRate;

public:
    SavingsAccount(const string &accNumber, double initialBalance, const string &openDate, const string &type, double rate)
        : BankAccount(accNumber, initialBalance, openDate, type), interestRate(rate) {}

    double getInterestRate() const
    {
        return interestRate;
    }
};
class CurrentAccount : public BankAccount
{
private:
    double interestRate;

public:
    CurrentAccount(const string &accNumber, double initialBalance, const string &openDate, const string &type, double rate)
        : BankAccount(accNumber, initialBalance, openDate, type), interestRate(rate) {}

    double getInterestRate() const
    {
        return interestRate;
    }
};

void readUserData(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        User currentUser;
        iss >> currentUser.username >> currentUser.password >> currentUser.userType;
        string accountNumber, accountType, openingDate, balance;
        double rate;
        while (iss >> accountNumber >> accountType >> openingDate >> balance >> rate)
        {
            Account currentAccount;
            currentAccount.accountNumber = accountNumber;
            currentAccount.accountType = accountType;
            currentAccount.openingDate = openingDate;
            currentAccount.balance = std::stod(balance);
            currentAccount.interestRate = rate;

            string transaction;
            while (iss >> transaction)
            {
                if (transaction == "transaction")
                {
                    Transaction trans;
                    iss >> trans.date >> trans.type >> trans.amount;
                    currentAccount.transactions.push_back(trans);
                }
            }
            currentUser.accounts.push_back(currentAccount);
        }
        if (currentUser.userType == "AccountHolder")
        {
            accountHolders.push_back(currentUser);
        }
        else if (currentUser.userType == "BranchManager")
        {
            branchManagers.push_back(currentUser);
        }
    }

    file.close();
}
bool authenticateAccountHolder(const string &username, const string &password)
{
    for (const auto &user : accountHolders)
    {
        if (user.username == username && user.password == password)
        {
            return true;
        }
    }
    return false;
}

bool authenticateBranchManager(const string &username, const string &password)
{
    for (const auto &user : branchManagers)
    {
        if (user.username == username && user.password == password)
        {
            return true;
        }
    }
    return false;
}

void sign_up()
{
    string username;
    bool usernameExists = false;
    cout << endl
         << "Enter username: ";
    cin >> username;

    ifstream file("data.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string existingUsername, password, userType;
            ss >> existingUsername >> password >> userType;
            if (existingUsername == username)
            {
                cout << "Username already exists. Please choose another username." << endl;
                usernameExists = true;
                break;
            }
        }
        file.close();
    }
    else
    {
        cerr << "Error: Unable to open file data.txt for reading" << endl;
        return;
    }

    if (usernameExists)
    {
        sign_up();
        return;
    }

    cout << "Enter password: ";
    string password;
    cin >> password;

    ofstream outFile("data.txt", ios::app);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file data.txt for writing" << endl;
        return;
    }

    outFile << username << " " << password << " AccountHolder" << endl;
    outFile.close();

    accountHolders.push_back({username, password});

    cout << "Sign up successful. Welcome to Dobby Banking, " << username << "!" << endl;
    cout << endl
         << "Now you can login with your username and password to proceed" << endl;
    login_as_account_holder();
}

void login_as_branch_manager()
{
    cout << endl
         << "Enter username: ";
    string username;
    cin >> username;

    cout << "Enter password: ";
    string password;
    cin >> password;

    if (authenticateBranchManager(username, password))
    {
        cout << "Login successful. Welcome, " << username << "!" << endl;
        cout << "How can I help you today?" << endl
             << "1. Get statement of an account" << endl
             << "2. Get list of account holders" << endl;
        int ch;
        cin >> ch;
        BranchManager man(username, password);
        if (ch == 1)
        {
            cout << "Enter account number you want the statements of: ";
            string no;
            cin >> no;
            man.getStatementOfAccount(no);
            cout << "Redirecting you to welcome page." << endl;
            welcome();
        }
        else if (ch == 2)
        {
            man.getAccountHolders();
            cout << "Redirecting you to welcome page." << endl;
            welcome();
        }
        else
        {
            cout << "Invalid Choice" << endl;
            cout << "Redirecting you to welcome page." << endl;
            welcome();
        }
    }
    else
    {
        cout << "Login failed. Invalid username or password." << endl;
        cout << "Redirecting you to welcome page." << endl;
        welcome();
    }
}

void login_as_account_holder()
{
    cout << endl
         << "Enter username: ";
    string username;
    cin >> username;

    cout << "Enter password: ";
    string password;
    cin >> password;

    if (authenticateAccountHolder(username, password))
    {
        cout << "Login successful. Welcome, " << username << "!" << endl;
        cout << endl
             << "What can I help you with today?" << endl;
        menu(username, password);
    }
    else
    {
        cout << "Login failed. Invalid username or password." << endl;
        cout << "Do you want to sign up as a new user" << endl;
        cout << "[y/n]";
        char c;
        cin >> c;
        if (c == 'y')
        {
            sign_up();
        }
        else if (c == 'n')
        {
            cout << "Directing you back to the welcome page..." << endl;
            welcome();
        }
        else
        {
            cout << "Invalid Choice. Directing you back to the welcome page..." << endl;
            welcome();
        }
    }
}

class BankAccountHolder
{
private:
    string username;
    string password;

public:
    BankAccountHolder(const string &uname, const string &pwd) : username(uname), password(pwd) {}

    void getAccounts(const User &user) const;
    string createAccount();

    void updatePasswordInDataFile(const string &new_password)
    {
        ifstream inFile("data.txt");
        ofstream outFile("temp.txt");

        if (!inFile.is_open() || !outFile.is_open())
        {
            cerr << "Error: Unable to open files for updating password" << endl;
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string current_username, current_password, userType;

            if (ss >> current_username >> current_password >> userType)
            {
                if (current_username == username)
                {
                    outFile << username << " " << new_password << " " << userType;

                    string restOfLine;
                    getline(ss, restOfLine);
                    outFile << restOfLine << endl;
                }
                else
                {
                    outFile << line << endl;
                }
            }
            else
            {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();

        remove("data.txt");
        rename("temp.txt", "data.txt");

        password = new_password;
    }

    void changePassword()
    {
        string new_password, confirm_password;

        cout << "Enter new password: ";
        cin >> new_password;

        cout << "Confirm new password: ";
        cin >> confirm_password;

        if (new_password == confirm_password)
        {

            password = new_password;

            updatePasswordInDataFile(new_password);

            cout << "Password changed successfully!" << endl;
            cout << endl
                 << "What else would you like me to do?" << endl;
            menu(username, password);
        }
        else
        {
            cout << "Passwords do not match. Please try again." << endl;
            changePassword();
        }
    }
};
void BankAccountHolder::getAccounts(const User &user) const
{
    if (!user.accounts.empty())
    {
        for (const auto &account : user.accounts)
        {
            cout << endl
                 << "Account Type: " << account.accountType << endl;
            cout << "Account Number: " << account.accountNumber << endl;
            cout << "Opening Date of this account: " << account.openingDate << endl;
            cout << "Balance in this Account: " << account.balance << endl;
            if (account.accountType == "Savings")
                cout << "Interest Rate: " << account.interestRate << endl;
        }
    }
    else
    {
        cout << "No accounts found for user " << user.username << endl;
    }
}
string BankAccountHolder::createAccount()
{
    string accountType;
    int n;
    double openingBalance, rate;

    cout << "What type of account you want to create: " << endl;
    cout << "1. Savings Account" << endl;
    cout << "2. Current/Checking Account" << endl;
    cin >> n;
    if (n == 1)
    {
        accountType = "Savings";
        rate = 2;
    }
    else if (n == 2)
    {
        accountType = "Current";
        rate = 0;
    }
    else
    {
        cout << "Invalid choice. Try Again";
        createAccount();
    }

    cout << "Enter the opening balance for the account: ";
    cin >> openingBalance;
    srand(time(0));
    string accountNumber = to_string(rand() % 900000000 + 100000000);

    time_t now = time(0);
    tm *ltm = localtime(&now);
    string openingDate = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);

    Account newAccount;
    newAccount.accountNumber = accountNumber;
    newAccount.accountType = accountType;
    newAccount.openingDate = openingDate;
    newAccount.balance = openingBalance;
    newAccount.interestRate = rate;

    Transaction openingTrans;
    openingTrans.date = openingDate;
    openingTrans.type = "Deposit";
    openingTrans.amount = openingBalance;
    newAccount.transactions.push_back(openingTrans);

    for (auto &user : accountHolders)
    {
        if (user.username == username)
        {
            user.accounts.push_back(newAccount);
            break;
        }
    }

    ifstream inFile("data.txt");
    ofstream tempFile("temp.txt");

    if (!inFile.is_open() || !tempFile.is_open())
    {
        cerr << "Error: Unable to open files for updating data" << endl;
        return "";
    }

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string currentUsername, currentPassword, currentUserType;

        ss >> currentUsername >> currentPassword >> currentUserType;

        if (ss.eof() && (currentUsername == username && currentPassword == password && currentUserType == "AccountHolder"))
        {
            continue;
        }
        tempFile << line << endl;
    }

    inFile.close();
    tempFile.close();

    remove("data.txt");
    rename("temp.txt", "data.txt");

    ofstream outFile("data.txt", ios::app);
    if (!outFile.is_open())
    {
        cerr << "Error: Unable to open file data.txt for writing" << endl;
        return "";
    }
    outFile << username << " " << password << " AccountHolder " << accountNumber << " " << accountType << " " << openingDate << " " << openingBalance << " " << rate << endl;
    for (const auto &trans : newAccount.transactions)
    {
        outFile << trans.date << " " << trans.type << " " << trans.amount << endl;
    }

    outFile.close();

    return accountNumber;
}

void operations(string num, double balance, string openingDate, string type)
{
    BankAccount account(num, balance, openingDate, type);
    cout << "What would you like to do?" << endl
         << "1. Deposit" << endl
         << "2. Withdraw" << endl
         << "3. Transfer money to another account" << endl
         << "4. Get interest rate of the account." << endl
         << "5. Logout" << endl;
    const double upperTransactionLimit = 50000;
    int n1;
    cin >> n1;
    if (n1 == 5)
        welcome();
    else if (n1 == 4)
    {
        if (type == "Savings")
        {
            SavingsAccount savings(num, balance, openingDate, type, 2);
            cout << "Interest Rate is: " << savings.getInterestRate() << "%" << endl;
            cout << "Redirecting you to operations ypu can perform" << endl;
            operations(num, balance, openingDate, type);
        }
        else
        {
            CurrentAccount current(num, balance, openingDate, type, 0);
            cout << "Interest Rate is: " << current.getInterestRate() << "%" << endl;
            cout << "Redirecting you to operations ypu can perform" << endl;
            operations(num, balance, openingDate, type);
        }
    }
    else if (n1 != 1 && n1 != 2 && n1 != 3)
    {
        cout << "Invalid Choice. Please choose from the options provided: " << endl;
        operations(num, balance, openingDate, type);
    }
    else
    {
        double amount;
        cout << "Enter the amount you want to deposit: ";
        cin >> amount;
        if (type == "Savings")
        {
            if (amount > upperTransactionLimit)
            {
                cout << "Amount exceeds the Upper Transaction limit for your savings account" << endl
                     << "Redirecting you to operations ypu can perform" << endl;
                operations(num, balance, openingDate, type);
                return;
            }
        }
        if (n1 == 1)
        {
            account.deposit(amount);
        }
        else if (n1 == 2)
        {
            account.withdraw(amount);
        }
        else if (n1 == 3)
            account.transfer(amount);
    }
}

void menu(string username, string password)
{
    BankAccountHolder userAccount(username, password);

    cout << "1. See what accounts you have." << endl;
    cout << "2. Create a new account." << endl;
    cout << "3. Change password." << endl;
    cout << "4. Logout." << endl;
    int n;
    cin >> n;
    if (n == 1)
    {
        cout << "Accounts for the user are as follows: " << endl;
        bool found = false;
        for (const auto &user : accountHolders)
        {
            if (user.username == username)
            {
                userAccount.getAccounts(user);
                found = true;
            }
        }
        if (!found)
        {
            cout << "User not found." << endl;
        }
        cout << endl
             << "Do you want to perform any actions on your existing account? " << endl;
        cout << "[y/n]" << endl;
        char c1;
        cin >> c1;
        if (c1 == 'y')
        {
            cout << "Enter account number: ";
            string num;
            cin >> num;

            bool accountFound = false;
            string openingDate, type;
            double balance, rate;
            for (const auto &user : accountHolders)
            {
                for (const auto &account : user.accounts)
                {
                    if (account.accountNumber == num)
                    {
                        accountFound = true;
                        openingDate = account.openingDate;
                        balance = account.balance;
                        type = account.accountType;
                        rate = account.interestRate;
                        break;
                    }
                }
            }

            if (!accountFound)
            {
                cout << "Account not found. Directing you back to menu." << endl;
                menu(username, password);
            }
            else
            {
                operations(num, balance, openingDate, type);
            }
        }

        else if (c1 == 'n')
        {
            cout << "What else can I help you with?" << endl;
            menu(username, password);
        }
        else
        {
            cout << "Invalid choice. Directing you back to menu..." << endl;
            menu(username, password);
        }
    }
    else if (n == 3)
        userAccount.changePassword();
    else if (n == 2)
    {
        string newAccountNumber = userAccount.createAccount();
        if (!newAccountNumber.empty())
        {
            cout << "Account created successfully! Your new account number is: " << newAccountNumber << endl;
        }

        cout << endl
             << "What else can I help you with? " << endl;
        menu(username, password);
    }
    else if (n == 4)
    {
        cout << "Logging out." << endl
             << endl;
        welcome();
    }
    else
    {
        cout << "Invalid Choice." << endl;
        cout << "Please choose from the options provided." << endl;
        menu(username, password);
    }
}

void welcome()
{
    cout << "Welcome to Dooby Banking" << endl
         << endl;
    cout << "Press 1 to login as an Account Holder" << endl;
    cout << "Press 2 to Sign Up as a new Account Holder" << endl;
    cout << "Press 3 to login as a Branch Manager" << endl;
    cout << "Press 0 to exit" << endl;
    int choice;
    cin >> choice;
    if (choice == 1)
        login_as_account_holder();
    else if (choice == 2)
        sign_up();
    else if (choice == 0)
        return;
    else if (choice == 3)
        login_as_branch_manager();
    else
    {
        cout << endl
             << "Invalid Choice" << endl;
        cout << "Please choose from the options provided" << endl
             << endl;
        welcome();
    }
}

int main()
{
    readUserData("data.txt");
    welcome();
    return 0;
}