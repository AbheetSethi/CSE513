#include "loan_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/socket.h>  
#include <string.h>


#define LOAN_DB "loans.txt"

int apply_for_loan(int user_id, double amount) {
    FILE *fp = fopen(LOAN_DB, "a");  // Append mode to add new loans
    if (fp == NULL) {
        perror("Failed to open loans database");
        return -1;
    }

    Loan new_loan;
    new_loan.id = rand() % 1000000;
    new_loan.user_id = user_id;
    new_loan.amount = amount;
    new_loan.status = 0;  // Pending status

    // Write loan to the file
    fprintf(fp, "%d %d %.2f %d\n", new_loan.id, new_loan.user_id, new_loan.amount, new_loan.status);
    fclose(fp);

    printf("Loan application successful. Loan ID: %d\n", new_loan.id);
    return new_loan.id;
}

int process_approve_reject_loan(int loan_id, int decision, int client_socket) {
    FILE *fp = fopen(LOAN_DB, "r");
    if (fp == NULL) {
        perror("Failed to open loans database");
        return -1;
    }

    FILE *temp_fp = fopen("temp_loans.dat", "w");
    if (temp_fp == NULL) {
        perror("Failed to open temporary loans database");
        fclose(fp);
        return -1;
    }

    Loan loan;
    int found = 0;
    while (fscanf(fp, "%d %d %lf %d", &loan.id, &loan.user_id, &loan.amount, &loan.status) != EOF) {
        if (loan.id == loan_id) {
            found = 1;
            if (decision == 1) {
                // Approve the loan (remove it from file)
                loan.status = 1;
                const char *success_message = "Loan ID approved.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            } else if (decision == 2) {
                // Reject the loan, but keep it in the file
                loan.status = 2;
                const char *rejected_message = "Loan ID rejected.\n";
                send(client_socket, rejected_message, strlen(rejected_message), 0);
                fprintf(temp_fp, "%d %d %.2f %d\n", loan.id, loan.user_id, loan.amount, loan.status);
            }
        } else {
            // Keep all other loans unchanged
            fprintf(temp_fp, "%d %d %.2f %d\n", loan.id, loan.user_id, loan.amount, loan.status);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // Replace old loan file with the new temp file
    remove(LOAN_DB);
    rename("temp_loans.dat", LOAN_DB);

    if (found == 0) {
        const char *fail_message = "Loan ID not found.\n";
        send(client_socket, fail_message, strlen(fail_message), 0);
        return -1;
    }

    const char *processed_message = "Loan processed successfully.\n";
    send(client_socket, processed_message, strlen(processed_message), 0);

    return 0;
}

void view_all_loans(int client_socket) {
    FILE *fp = fopen(LOAN_DB, "r");
    if (fp == NULL) {
        perror("Failed to open loans database");
        return;
    }

    Loan loan;
    char buffer[256];
    const char *header = "All Loan Applications:\n";
    send(client_socket, header, strlen(header), 0);

    while (fscanf(fp, "%d %d %lf %d", &loan.id, &loan.user_id, &loan.amount, &loan.status) != EOF) {
        snprintf(buffer, sizeof(buffer), "Loan ID: %d, Amount: %.2f, Status: %d\n", loan.id, loan.amount, loan.status);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    fclose(fp);
    const char *end_message = "All loans displayed.\n";
    send(client_socket, end_message, strlen(end_message), 0);
}