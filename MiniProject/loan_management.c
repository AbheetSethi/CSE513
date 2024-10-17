// loan_management.c
#include "loan_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

#define LOAN_DB "loans.dat"

int apply_for_loan(int user_id, double amount) {
    // TODO: Implement loan application logic

    FILE *fp = fopen(LOAN_DB, "ab");  // Append mode to add new loans
    if (fp == NULL) {
        perror("Failed to open loans database");
        return -1;
    }

    // Create new loan entry
    Loan new_loan;
    new_loan.id = rand() % 1000000;  // Generating a random loan ID (can be improved)
    new_loan.user_id = user_id;
    new_loan.amount = amount;
    new_loan.status = 0;  // Status 0: Pending
    new_loan.employee_id = -1;  // Initialize employee_id to -1 or any default value

    if (fwrite(&new_loan, sizeof(Loan), 1, fp) != 1) {
        perror("Failed to write new loan to database");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    printf("Loan application submitted successfully. Loan ID: %d\n", new_loan.id);
    return new_loan.id;
}

int process_loan(int loan_id, int employee_id) {
    // TODO: Implement loan processing logic

    FILE *fp = fopen(LOAN_DB, "r+b");
    if (fp == NULL) {
        perror("Failed to open loans database");
        return -1;
    }

    flock(fileno(fp), LOCK_EX);  // Lock file for safe read/write

    Loan loan;
    while (fread(&loan, sizeof(Loan), 1, fp)) {
        if (loan.id == loan_id) {
            // Loan found, now it’s being processed by the employee
            printf("Loan ID %d is being processed by Employee ID %d\n", loan_id, employee_id);
            // Here, we could add more processing logic (e.g., verifying loan documents)
            flock(fileno(fp), LOCK_UN);  // Unlock the file
            fclose(fp);
            return 0;
        }
    }

    printf("Loan ID %d not found.\n", loan_id);
    flock(fileno(fp), LOCK_UN);  // Unlock file in case of failure
    fclose(fp);
    return -1;
}

int approve_reject_loan(int loan_id, int manager_id, int decision) {
    // TODO: Implement loan approval/rejection logic

    FILE *fp = fopen(LOAN_DB, "r+b");
    if (fp == NULL) {
        perror("Failed to open loans database");
        return -1;
    }

    flock(fileno(fp), LOCK_EX);  // Lock file for safe read/write

    Loan loan;
    while (fread(&loan, sizeof(Loan), 1, fp)) {
        if (loan.id == loan_id) {
            if (decision == 1) {
                loan.status = 1;  // Status 1: Approved
                printf("Loan ID %d approved by Manager ID %d\n", loan_id, manager_id);
            } else if (decision == 2) {
                loan.status = 2;  // Status 2: Rejected
                printf("Loan ID %d rejected by Manager ID %d\n", loan_id, manager_id);
            } else {
                printf("Invalid decision. Use 1 for approval, 2 for rejection.\n");
                flock(fileno(fp), LOCK_UN);  // Unlock the file
                fclose(fp);
                return -1;
            }

            fseek(fp, -sizeof(Loan), SEEK_CUR);  // Move file pointer back to update loan
            fwrite(&loan, sizeof(Loan), 1, fp);
            flock(fileno(fp), LOCK_UN);  // Unlock the file
            fclose(fp);
            return 0;
        }
    }

    printf("Loan ID %d not found.\n", loan_id);
    flock(fileno(fp), LOCK_UN);  // Unlock file in case of failure
    fclose(fp);
    return -1;
}

void view_assigned_loans(int employee_id) {
    FILE *fp = fopen(LOAN_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open loan database");
        return;
    }

    Loan loan;
    printf("Assigned Loan Applications:\n");
    while (fread(&loan, sizeof(Loan), 1, fp)) {
        if (loan.employee_id == employee_id) {  // Check if the loan is assigned to this employee
            printf("Loan ID: %d, User ID: %d, Amount: %.2f, Status: %d\n",
                   loan.id, loan.user_id, loan.amount, loan.status);
        }
    }
    fclose(fp);
}
