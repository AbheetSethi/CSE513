#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#define TRANSACTION_FILE "transactions.txt"

typedef struct {
    int account_id;
    double amount;
    char transaction_type[20];
    char timestamp[30];
} Transaction;

int add_transaction(int account_id, double amount, const char *transaction_type);
int view_transaction_history(int account_id, int client_socket);  // Updated signature
void view_history(int account_id, int client_socket);  // Updated signature

#endif