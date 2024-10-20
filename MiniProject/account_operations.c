#include "account_operations.h"
#include "transaction_history.h"
#include "user_management.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>  // For file locking

#define ACCOUNT_DB "accounts.dat"
#define USER_DB "users.dat"
#define TRANSACTION_DB "transactions.dat"
#define FEEDBACK_FILE "feedback.txt"
#define MAX_ACCOUNTS 100

int get_new_account_id() {
    FILE *fp = fopen(ACCOUNT_DB, "rb");
    if (fp == NULL) {
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

int get_new_user_id() {
    FILE *fp = fopen("users.dat", "rb");  // Open the user database file
    if (fp == NULL) {
        return 1;  // Start with ID 1 if no users exist
    }

    User user;
    int last_id = 0;

    // Iterate over all users to find the last user ID
    while (fread(&user, sizeof(User), 1, fp)) {
        last_id = user.id;  // Assuming the User structure has an id field
    }

    fclose(fp);

    // Return the next available user ID
    return last_id + 1;
}

int create_account(int user_id, double initial_balance, const char* name, const char* mobile) {
    int account_id = get_new_account_id();  // Generate a new account ID

    FILE *fp = fopen(ACCOUNT_DB, "ab");  // Open account DB in append mode
    if (fp == NULL) {
        perror("Failed to open accounts database");
        return -1;
    }

    Account new_account;
    new_account.id = account_id;  // Assign the new account ID
    new_account.user_id = user_id;  // Use the provided user ID
    new_account.balance = initial_balance;

    // Write the account to the database
    if (fwrite(&new_account, sizeof(Account), 1, fp) != 1) {
        perror("Failed to write new account to accounts database");
        fclose(fp);
        return -1;
    }
    fclose(fp);

    // Save name and mobile number in info.txt
    FILE *info_fp = fopen("info.txt", "a");
    if (info_fp == NULL) {
        perror("Failed to open info.txt");
        return -1;
    }

    fprintf(info_fp, "Account ID: %d, User ID: %d, Name: %s, Mobile: %s\n", account_id, user_id, name, mobile);
    fclose(info_fp);

    printf("Created new account with Account ID: %d, User ID: %d, Initial Balance: %.2f, Name: %s, Mobile: %s\n", 
            account_id, user_id, initial_balance, name, mobile);

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

int deposit(int user_id, double amount, int client_socket) {
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
            if (fwrite(&account, sizeof(Account), 1, fp) != 1) {
                perror("Failed to update account balance");
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;
            }
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

int withdraw(int user_id, double amount, int client_socket) {
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
    int found = 0;
    while (fread(&account, sizeof(Account), 1, fp)) {
        if (account.user_id == user_id) {  // Use user_id to find the correct account
            found = 1;
            if (account.balance < amount) {
                printf("Insufficient funds in account for User ID %d. Available balance: %.2f\n", user_id, account.balance);
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;  // Insufficient funds
            }

            account.balance -= amount;
            fseek(fp, -sizeof(Account), SEEK_CUR);  // Move back to update account
            if (fwrite(&account, sizeof(Account), 1, fp) != 1) {
                perror("Failed to update account balance");
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;
            }
            printf("Withdrew %.2f from account for User ID %d\n", amount, user_id);

            // Add the transaction to the history
            if (add_transaction(account.id, -amount, "Withdraw") != 0) {
                printf("Failed to log transaction.\n");
            }

            flock(fileno(fp), LOCK_UN);  // Unlock the file
            fclose(fp);
            return 0;  // Successful withdrawal
        }
    }

    if (!found) {
        printf("No account found for User ID %d.\n", user_id);
    }

    flock(fileno(fp), LOCK_UN);  // Unlock file in case of failure
    fclose(fp);
    return -1;  // Account not found
}

int transfer(int from_account_id, int to_account_id, double amount) {
    // Check for a valid transfer amount
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

    Account accounts[MAX_ACCOUNTS];  // Array to hold accounts
    int account_count = 0;

    // Read all accounts into a temporary array for easier access
    while (fread(&accounts[account_count], sizeof(Account), 1, fp)) {
        account_count++;
    }

    Account *from_account = NULL;
    Account *to_account = NULL;

    // Search for both accounts in the temporary array
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == from_account_id) {
            from_account = &accounts[i];  // Found from account
            if (from_account->balance < amount) {
                printf("Insufficient funds in account ID %d. Available balance: %.2f\n", from_account_id, from_account->balance);
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;  // Insufficient funds
            }
        }
        if (accounts[i].id == to_account_id) {
            to_account = &accounts[i];  // Found to account
        }
    }

    // Update balances if both accounts were found
    if (from_account && to_account) {
        from_account->balance -= amount;  // Deduct from sender's account
        to_account->balance += amount;  // Add to recipient's account

        // Seek to the beginning and write the updated accounts back
        fseek(fp, 0, SEEK_SET);  // Move back to the start of the file
        fwrite(accounts, sizeof(Account), account_count, fp);  // Write back all accounts

        printf("Transferred %.2f from account ID %d to account ID %d\n", amount, from_account_id, to_account_id);
        
        // Add transactions to history
        if (add_transaction(from_account->id, -amount, "Transfer Out") != 0) {
            printf("Failed to log transfer out transaction.\n");
        }
        if (add_transaction(to_account->id, amount, "Transfer In") != 0) {
            printf("Failed to log transfer in transaction.\n");
        }

    } else {
        // Error messages if accounts are not found
        if (!from_account) {
            printf("Sender account ID %d not found.\n", from_account_id);
        }
        if (!to_account) {
            printf("Recipient account ID %d not found.\n", to_account_id);
        }
    }

    flock(fileno(fp), LOCK_UN);  // Unlock the file
    fclose(fp);
    return (from_account && to_account) ? 0 : -1;  // Successful transfer only if both accounts are found
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

int activate_deactivate_account(int account_id, int action) {
    // action: 1 for activate, 0 for deactivate
    User users[MAX_ACCOUNTS]; // Use a defined constant for maximum users
    int count = 0;

    // Read all users from the database
    FILE *file = fopen(USER_DB, "rb");
    if (!file) {
        perror("Failed to open users database");
        return -1;
    }

    // Load existing users
    while (fread(&users[count], sizeof(User), 1, file) == 1 && count < MAX_ACCOUNTS) {
        count++;
    }
    fclose(file);

    // Find the user with the specified account_id
    for (int i = 0; i < count; i++) {
        if (users[i].id == account_id) {
            if (action == 1) {
                // Activate account
                users[i].active = 1; // Set active status to 1
                printf("Activating account ID: %d\n", account_id);
            } else if (action == 0) {
                // Deactivate account
                users[i].active = 0; // Set active status to 0
                printf("Deactivating account ID: %d\n", account_id);
            }
            break; // Exit loop once the user is found
        }
    }

    // Write updated users back to the database
    file = fopen(USER_DB, "wb");
    if (!file) {
        perror("Failed to open users database for writing");
        return -1;
    }

    fwrite(users, sizeof(User), count, file); // Write all users back to the file
    fclose(file);

    return 0; // Success
}

void prompt_account_action() {
    int account_id, action;

    // Ask user for the action
    printf("Do you want to activate (1) or deactivate (0) an account? ");
    scanf("%d", &action);

    printf("Enter the account ID: ");
    scanf("%d", &account_id);

    // Call the function with the user input
    if (activate_deactivate_account(account_id, action) == 0) {
        if (action == 1) {
            printf("Account activated successfully.\n");
        } else {
            printf("Account deactivated successfully.\n");
        }
    } else {
        printf("Failed to update account status.\n");
    }
}

