#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>  // Required for socket functions like 'send'
#include "transaction_history.h"

int add_transaction(int account_id, double amount, const char *transaction_type) {
    FILE *fp = fopen(TRANSACTION_FILE, "a");
    if (fp == NULL) {
        perror("Failed to open transaction history file");
        return -1;
    }

    Transaction new_transaction;
    new_transaction.account_id = account_id;
    new_transaction.amount = amount;
    strncpy(new_transaction.transaction_type, transaction_type, sizeof(new_transaction.transaction_type) - 1);
    new_transaction.transaction_type[sizeof(new_transaction.transaction_type) - 1] = '\0';  // Ensure null-termination

    // Get current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(new_transaction.timestamp, sizeof(new_transaction.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    if (fprintf(fp, "%d, %.2f, %s, %s\n", 
                new_transaction.account_id, new_transaction.amount,
                new_transaction.transaction_type, new_transaction.timestamp) < 0) {
        perror("Failed to write transaction to file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int view_transaction_history(int account_id, int client_socket) {
    FILE *fp = fopen(TRANSACTION_FILE, "r");
    if (fp == NULL) {
        perror("Failed to open transaction history file");
        return -1;
    }

    char line[300];
    int found = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        int id;
        sscanf(line, "%d,", &id);
        if (id == account_id) {  // Make sure it is the correct account ID
            // Debugging output
            printf("Transaction found for Account ID: %d\n", id);
            send(client_socket, line, strlen(line), 0);  // Send the transaction over the socket
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found for account ID %d\n", account_id);
        char error_msg[] = "No transactions found for the given account ID.\n";
        send(client_socket, error_msg, strlen(error_msg), 0);
    }

    fclose(fp);
    return found ? 0 : -1;
}

void view_history(int account_id, int client_socket) {
    printf("Viewing transaction history for Account ID: %d\n", account_id);
    
    // Debugging output
    printf("Using Account ID: %d to retrieve transaction history.\n", account_id);

    if (view_transaction_history(account_id, client_socket) != 0) {
        // If no transactions are found, send a message over the socket
        char error_msg[] = "No transaction history found.\n";
        send(client_socket, error_msg, strlen(error_msg), 0);
    }
}