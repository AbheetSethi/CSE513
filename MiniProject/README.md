# Design and Development of Banking Management System

The Banking Management System aims to simulate the core functionalities of a bank, including customer account management, transactions, loan processing, and administrative oversight. The system features secure login mechanisms and role-based access for customers, employees, managers, and administrators. The focus is on ensuring data consistency, security, and correct handling of race conditions using file management, locking mechanisms, and process synchronization.
Roles:
    
## 1. Customer: 
Customers log in using their credentials and have access to banking operations such as viewing account balances, depositing or withdrawing money, transferring funds, and applying for loans. The system ensures proper locking mechanisms to avoid race conditions when performing concurrent operations.

1. Login System (one session per user)
2. View Account Balance
3. Deposit Money
4. Withdraw Money
5. Transfer Funds
6. Apply for a Loan
7. Change Password
8. Adding Feedback
9. View Transaction History
10. Logout
11. Exit

## 2. Bank Employee: 
Bank employees manage customer accounts, including opening, modifying, or closing accounts, as well as processing loans.

1. Login System (one session per user)
2. Add New Customer
3. Modify Customer Details
4. Process Loan Applications
5. Approve/Reject Loans
6. View Assigned Loan Applications
7. View Customer Transactions( Passbook Kind of feature)
8. Change Password
9. Logout
10. Exit

## 3. Manager:

1. Login System (one session per user)
2. Activate/Deactivate Customer Accounts
3. Assign Loan Application Processes to Employees
4. Review Customer Feedback
5. Change Password
6. Logout
7. Exit

## 4. Administrator: 
The Administrator has the highest level of access and manages both customer and employee accounts.

1. Login System (one session per user)
2. Add New Bank Employee
3. Modify Customer/Employee Details
4. Manage User Roles
5. Change Password
6. Logout
7. Exit

## Technical Requirements:

    • Socket Programming: The server maintains the database and serves multiple clients concurrently. Clients can connect to the server and access their specific account details.
    • System Calls: Use system calls instead of library functions wherever possible, including process management, file management, file locking, semaphores, multithreading, and inter-process communication mechanisms.


# How To Execute:

1. Open terminal, and type this:

gcc -o my_program main.c user_management.c account_operations.c loan_management.c admin_operations.c database_init.c menu.c feedback.c transaction_history.c -lpthread

2. After successful compilation, type this:

./my_program

3. Open another terminal and type this:

nc localhost 8080
