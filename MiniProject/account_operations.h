// account_operations.h
#ifndef ACCOUNT_OPERATIONS_H
#define ACCOUNT_OPERATIONS_H

typedef struct {
    int id;
    int user_id;      // Link to user
    double balance;
    int employee_id;  // Optional: Employee managing the account
} Account;

int get_new_account_id();
int get_new_user_id();
int create_account(int user_id, double initial_balance, const char* name, const char* mobile);
double view_balance(int account_id);
int deposit(int user_id, double amount, int client_socket);
int withdraw(int user_id, double amount, int client_socket);
int transfer(int from_account_id, int to_account_id, double amount);
int add_feedback(int user_id, const char *feedback);
int view_transaction_history(int account_id, int client_socket);  // Updated signature
void print_all_accounts();
int activate_deactivate_account(int user_id, int status);

#endif
