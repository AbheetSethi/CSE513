#ifndef LOAN_MANAGEMENT_H
#define LOAN_MANAGEMENT_H

typedef struct {
    int id;
    int user_id;
    double amount;
    int status;  // 0: Pending, 1: Approved, 2: Rejected
} Loan;

int apply_for_loan(int user_id, double amount);
int process_approve_reject_loan(int loan_id, int decision, int client_socket);
void view_all_loans();  // Updated function to view all loans, without employee filtering

#endif