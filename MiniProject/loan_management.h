// loan_management.h
#ifndef LOAN_MANAGEMENT_H
#define LOAN_MANAGEMENT_H

typedef struct {
    int id;
    int user_id;
    double amount;
    int status; // 0: Pending, 1: Approved, 2: Rejected
    int employee_id;
} Loan;

int apply_for_loan(int user_id, double amount);
int process_loan(int loan_id, int employee_id);
int approve_reject_loan(int loan_id, int manager_id, int decision);
void view_assigned_loans(int employee_id);

#endif

