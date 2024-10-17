#include "account_operations.h"
#include "transaction_history.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>  // For file locking

#define ACCOUNT_DB "accounts.dat"
#define TRANSACTION_DB "transactions.dat"
#define FEEDBACK_FILE "feedback.txt"

int get_new_account_id() {
    FILE *fp = fopen(ACCOUNT_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return 1;  // Start from ID 1 if no accounts exist
    }

    Account account;
    int last_id = 0;
    
    // Iterate over all accounts to find the last account ID
    while (fread(&account, sizeof(Account), 1, fp)) {
        last_id = account.id;
    }

    fclose(fp);

    // Return the next available account ID
    return last_id + 1;
}

int create_account(int account_id, int user_id, double initial_balance) {
    FILE *fp = fopen(ACCOUNT_DB, "ab");  // Append mode to add new account
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    Account new_account;
    new_account.id = account_id;
    new_account.user_id = user_id;
    new_account.balance = initial_balance;

    // Write the account to file
    if (fwrite(&new_account, sizeof(Account), 1, fp) != 1) {
        perror("Failed to write new account to accounts database");
        fclose(fp);
        return -1;
    }

    fclose(fp);

    printf("Created new account with Account ID: %d, User ID: %d, Initial Balance: %.2f\n", 
            account_id, user_id, initial_balance);

    return 0;  // Success
}

double view_balance(int user_id) {
    FILE *fp = fopen(ACCOUNT_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    Account account;
    while (fread(&account, sizeof(Account), 1, fp)) {
        if (account.user_id == user_id) {  // Use user_id to find the correct account
            printf("Account ID %d found with balance: %.2f\n", account.id, account.balance);
            fclose(fp);
            return account.balance;
        }
    }

    printf("No account found for User ID %d.\n", user_id);
    fclose(fp);
    return -1;  // Account not found
}

int deposit(int user_id, double amount, int client_socket) {  // Add client_socket as a parameter
    if (amount <= 0) {
        printf("Invalid deposit amount. Must be greater than zero.\n");
        return -1;  // Early exit for invalid amount
    }

    FILE *fp = fopen(ACCOUNT_DB, "r+b");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    flock(fileno(fp), LOCK_EX);  // Lock file for writing

    Account account;
    int found = 0;
    while (fread(&account, sizeof(Account), 1, fp)) {
        if (account.user_id == user_id) {  // Use user_id to find the correct account
            found = 1;
            account.balance += amount;
            fseek(fp, -sizeof(Account), SEEK_CUR);  // Move back to update account
            fwrite(&account, sizeof(Account), 1, fp);
            printf("Deposited %.2f into account ID %d\n", amount, account.id);

            // Add the transaction to the history
            if (add_transaction(account.id, amount, "Deposit") != 0) {
                printf("Failed to log transaction.\n");
            }

            flock(fileno(fp), LOCK_UN);  // Unlock the file
            fclose(fp);
            return 0;  // Successful deposit
        }
    }

    if (!found) {
        printf("No account found for User ID %d.\n", user_id);
    }

    flock(fileno(fp), LOCK_UN);  // Unlock file in case of failure
    fclose(fp);
    return -1;  // Account not found
}


int withdraw(int account_id, double amount) {
    if (amount <= 0) {
        printf("Invalid withdrawal amount. Must be greater than zero.\n");
        return -1;  // Early exit for invalid amount
    }

    FILE *fp = fopen(ACCOUNT_DB, "r+b");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    flock(fileno(fp), LOCK_EX);  // Lock file for writing

    Account account;
    while (fread(&account, sizeof(Account), 1, fp)) {
        if (account.id == account_id) {
            if (account.balance < amount) {
                printf("Insufficient funds in account ID %d. Available balance: %.2f\n", account_id, account.balance);
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;  // Insufficient funds
            }

            account.balance -= amount;
            fseek(fp, -sizeof(Account), SEEK_CUR);  // Move back to update account
            fwrite(&account, sizeof(Account), 1, fp);
            printf("Withdrew %.2f from account ID %d\n", amount, account_id);
            flock(fileno(fp), LOCK_UN);  // Unlock the file
            fclose(fp);
            return 0;  // Success
        }
    }

    printf("Account ID %d not found.\n", account_id);
    flock(fileno(fp), LOCK_UN);  // Unlock the file in case of failure
    fclose(fp);
    return -1;  // Account not found
}

int transfer(int from_account_id, int to_account_id, double amount) {
    if (amount <= 0) {
        printf("Invalid transfer amount. Must be greater than zero.\n");
        return -1;  // Early exit for invalid amount
    }

    FILE *fp = fopen(ACCOUNT_DB, "r+b");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    flock(fileno(fp), LOCK_EX);  // Lock file for writing

    Account from_account, to_account;
    int from_found = 0, to_found = 0;

    // First find both accounts in a single loop for efficiency
    while (fread(&from_account, sizeof(Account), 1, fp)) {
        if (from_account.id == from_account_id) {
            from_found = 1;
            if (from_account.balance < amount) {
                printf("Insufficient funds in account ID %d. Available balance: %.2f\n", from_account_id, from_account.balance);
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;  // Insufficient funds
            }
            from_account.balance -= amount;  // Deduct from sender's account
        }

        if (from_account.id == to_account_id) {
            to_found = 1;
            to_account = from_account;  // Copy the same structure, will update later
            to_account.balance += amount;  // Add to receiver's account
        }

        if (from_found && to_found) break;
    }

    if (from_found && to_found) {
        // Go back and update both accounts
        fseek(fp, -sizeof(Account) * 2, SEEK_CUR);  // Move back for both accounts
        fwrite(&from_account, sizeof(Account), 1, fp);  // Update from account
        fwrite(&to_account, sizeof(Account), 1, fp);    // Update to account

        printf("Transferred %.2f from account ID %d to account ID %d\n", amount, from_account_id, to_account_id);
        flock(fileno(fp), LOCK_UN);  // Unlock the file
        fclose(fp);
        return 0;  // Success
    } else {
        if (!from_found) printf("Account ID %d not found.\n", from_account_id);
        if (!to_found) printf("Account ID %d not found.\n", to_account_id);
        flock(fileno(fp), LOCK_UN);  // Unlock the file
        fclose(fp);
        return -1;  // One or both accounts not found
    }
}

void print_all_accounts() {
    FILE *fp = fopen(ACCOUNT_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return;
    }

    Account account;
    printf("Listing all accounts:\n");
    while (fread(&account, sizeof(Account), 1, fp)) {
        printf("Account ID: %d, User ID: %d, Balance: %.2f\n", account.id, account.user_id, account.balance);
    }

    fclose(fp);
}
/*
int add_feedback(int user_id, const char *feedback) {
    // TODO: Implement feedback addition logic

    FILE *fp = fopen(FEEDBACK_FILE, "a");
    if (fp == NULL) {
        perror("Failed to open feedback file");
        return -1;
    }

    fprintf(fp, "User ID %d: %s\n", user_id, feedback);
    fclose(fp);
    printf("Feedback from user ID %d added successfully.\n", user_id);
    return 0;
}
*/

/*

int view_transaction_history(int account_id) {
    // TODO: Implement transaction history viewing logic

    FILE *fp = fopen(TRANSACTION_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open transactions database");
        return -1;
    }

    int found = 0;
    char transaction[256];
    while (fread(transaction, sizeof(transaction), 1, fp)) {
        char account_id_str[20];
        sprintf(account_id_str, "%d", account_id);
        if (strstr(transaction, account_id_str) != NULL) {
            printf("%s", transaction);  // Print the transaction related to the account
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found for account ID %d\n", account_id);
    }

    fclose(fp);
    return found ? 0 : -1;
}
*/
