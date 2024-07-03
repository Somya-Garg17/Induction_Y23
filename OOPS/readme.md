# Dooby Banking

Welcome to my CLI banking client.

## What can I do?

1. You can sign up as a new user.
2. You can login if you have already signed up as an user before.
3. You can create new accounts or get the details of your account if it already exists including the interest rate depending on what type of account it is. A single user can have multiple accounts.
4. You can perform operations like deposit, withdraw and transfer money from one existing account to another existing account.
5. This bank have two branch managers, to login as a branch managers<br>
   username: manager1; password: passman1<br>
   username: manager2; password: passman2<br>
   Once you have the branch manager privileges, you can get the list of all account holders and also get the transaction history of any account by entering the account number.

## Run Locally

Clone the project

```bash
  git clone --branch somya_g https://github.com/Somya-Garg17/Induction_Y23.git
```

Go to the project directory

```bash
  cd Induction_Y23 && cd banking_client
```

Create a binary file

```bash
  g++ client.cpp -o client
```

Run the program

```bash
  ./client
```
