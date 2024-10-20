#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <ctype.h>
#include "account_operations.h"  // For balance, deposit, and withdraw
#include "transaction_history.h" // For viewing transaction history
#include "feedback.h"            // For adding feedback
#include "menu.h"
#include "user_management.h"  // For user-related functions
#include "loan_management.h"  // For loan-related functions
#include "admin_operations.h" // For admin functions

#define USER_DB "users.dat"
#define LOAN_DB "loans.dat"

// Function to trim leading and trailing spaces or newline characters from a string
void trim_input(char *str) {
    // Trim trailing spaces or newlines
    int length = strlen(str);
    while (length > 0 && (str[length - 1] == ' ' || str[length - 1] == '\n' || str[length - 1] == '\t')) {
        str[length - 1] = '\0';
        length--;
    }

    // Trim leading spaces
    char *start = str;
    while (*start && isspace(*start)) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int display_customer_menu(int client_socket, int account_id)
{
    int option;
    char buffer[1024];

    do
    {
        // Send the customer menu to the client
        const char *menu = "\n--- Customer Menu ---\n"
                           "1. View Balance\n"
                           "2. Deposit Money\n"
                           "3. Withdraw Money\n"
                           "4. View Transaction History\n"
                           "5. Apply for loan\n"
                           "6. Transfer Funds\n"    // Added transfer option
                           "7. Add Feedback\n"
                           "8. Change password\n"
                           "9. Logout\n"
                           "10. Exit\n"
                           "Select an option: ";
        send(client_socket, menu, strlen(menu), 0);

        // Receive the user's option from the client
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0)
        {
            perror("Error receiving data from client");
            close(client_socket);
            return -1; // Error case
        }
        buffer[bytes_read] = '\0'; // Null-terminate the input
        option = atoi(buffer);     // Convert the input to an integer

        switch (option)
        {
        case 1:
        {
            // View balance
            double balance = view_balance(account_id);
            snprintf(buffer, sizeof(buffer), "Your current balance is: %.2f\n", balance);
            send(client_socket, buffer, strlen(buffer), 0);
            print_all_accounts();
            break;
        }
       case 2: {  // Deposit money
            print_all_accounts(); // Show accounts
            const char *deposit_prompt = "Enter amount to deposit: ";
            send(client_socket, deposit_prompt, strlen(deposit_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0) {
                perror("Error receiving data from client");
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            double amount = atof(buffer);

            // Call deposit with the correct user_id and client_socket
            if (deposit(account_id, amount, client_socket) == 0) {
                add_transaction(account_id, amount, "Deposit");  // Ensure account_id or user_id is used correctly
                const char *success_msg = "Deposit successful.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Deposit failed. Please try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 3: {  // Withdraw money
            print_all_accounts();  // Show accounts
            const char *withdraw_prompt = "Enter amount to withdraw: ";
            send(client_socket, withdraw_prompt, strlen(withdraw_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0) {
                perror("Error receiving data from client");
                close(client_socket);
                return -1;  // Error case
            }
            buffer[bytes_read] = '\0';
            double amount = atof(buffer);

            // Call withdraw with the correct user_id and client_socket
            if (withdraw(account_id, amount, client_socket) == 0) {  // Make sure to pass client_socket
                add_transaction(account_id, amount, "Withdrawal");  // Ensure account_id is used correctly
                const char *success_msg = "Withdrawal successful.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Withdrawal failed. Please try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 4:
        {
            // View transaction history
            printf("Viewing transaction history for your account.\n");
            
            // Debug: Print the account_id to ensure it's correct
            printf("Using Account ID: %d to view transaction history.\n", account_id);
            
            view_history(account_id, client_socket);  // Use the actual account ID
            break;
        }

        case 5: 
        {  
            // Apply for Loan
            const char *loan_prompt = "Enter loan amount: ";
            send(client_socket, loan_prompt, strlen(loan_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0) {
                perror("Error receiving data from client");
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            double loan_amount = atof(buffer);

            // Call the apply_for_loan function
            int loan_id = apply_for_loan(account_id, loan_amount);
            if (loan_id > 0) {
                char success_msg[128];
                snprintf(success_msg, sizeof(success_msg), "Loan application successful. Loan ID: %d\n", loan_id);
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Loan application failed. Please try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }

        case 6:
        {
            // Transfer funds logic
            int to_account_id;
            double amount;

            // Prompt for receiver account ID
            const char *prompt_receiver = "Enter the account ID to transfer funds to: ";
            send(client_socket, prompt_receiver, strlen(prompt_receiver), 0);
            recv(client_socket, buffer, sizeof(buffer), 0);
            to_account_id = atoi(buffer);

            // Prompt for transfer amount
            const char *prompt_amount = "Enter the amount to transfer: ";
            send(client_socket, prompt_amount, strlen(prompt_amount), 0);
            recv(client_socket, buffer, sizeof(buffer), 0);
            amount = atof(buffer);

            // Perform transfer
            if (transfer(account_id, to_account_id, amount) == 0)
            {
                const char *success_message = "Funds transferred successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to transfer funds. Please check the details.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
        }
        break;

        case 7:
        {
            // Add feedback
            const char *feedback_prompt = "Enter your feedback: ";
            send(client_socket, feedback_prompt, strlen(feedback_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0)
            {
                perror("Error receiving data from client");
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            add_feedback(account_id, buffer);
            const char *feedback_success_msg = "Feedback submitted successfully.\n";
            send(client_socket, feedback_success_msg, strlen(feedback_success_msg), 0);
            break;
        }
        case 8: 
        {  
            // Change password
            char old_password[128], new_password[128];

            // Ask for old password
            const char *old_pass_prompt = "Enter current password: ";
            send(client_socket, old_pass_prompt, strlen(old_pass_prompt), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';
            trim_newline(old_password);  // Trim newline

            // Ask for new password
            const char *new_pass_prompt = "Enter new password: ";
            send(client_socket, new_pass_prompt, strlen(new_pass_prompt), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';
            trim_newline(new_password);  // Trim newline

            // Change password for the currently logged-in user
            if (change_password(account_id, old_password, new_password, client_socket)) {
                const char *success_msg = "Password changed successfully.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Failed to change password. Please check your current password and try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 9:
        {
            send(client_socket, "Logging out...\n\n", 15, 0);
            return 1; // Return value for logout
        }
        case 10:
        {
            send(client_socket, "Exiting...\n\n", 11, 0);
            return 2; // Return value for exit
        }

        default:
            const char *invalid_msg = "Invalid option. Please try again.\n";
            send(client_socket, invalid_msg, strlen(invalid_msg), 0);
            break;
        }
    } while (option != 6 && option != 7);

    return 0; // This line won't be reached due to the loop, but it's good practice to return an int.
}

int read_input_from_socket(int client_socket) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));  // Clear the buffer

    // Read from the socket
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        return atoi(buffer);  // Convert to integer
    } else {
        return -1;  // Error reading input
    }
}

int display_employee_menu(int client_socket, int user_id)
{
    int choice;
    char buffer[1024];
    int employee_id = user_id; // Use the passed user_id as employee_id

    while (1)
    {
        // Send the Employee Menu to the client
        const char *menu = "\nEmployee Menu:\n"
                           "1. Add New Customer\n"
                           "2. Modify Customer Details\n"
                           "3. Approve/Reject Loans\n"
                           "4. View Assigned Loan Applications\n"
                           "5. View Customer Transactions\n" 
                           "6. Change Password\n"
                           "7. Logout\n"
                           "8. Exit\n"
                           "Enter your choice: ";
        send(client_socket, menu, strlen(menu), 0);

        // Receive the user's choice from the client
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0)
        {
            perror("Error receiving data from client");
            close(client_socket);
            return -1; // Error case
        }
        buffer[bytes_read] = '\0'; // Null-terminate the input
        choice = atoi(buffer);     // Convert input to integer

        switch (choice)
        {
        case 1: {
            User new_customer;

            new_customer.active = 1;

            // Get customer username
            const char *prompt_username = "Enter customer username: ";
            send(client_socket, prompt_username, strlen(prompt_username), 0);
            bytes_read = recv(client_socket, new_customer.username, sizeof(new_customer.username), 0);
            new_customer.username[bytes_read] = '\0';

            // Get customer password
            const char *prompt_password = "Enter customer password: ";
            send(client_socket, prompt_password, strlen(prompt_password), 0);
            bytes_read = recv(client_socket, new_customer.password, sizeof(new_customer.password), 0);
            new_customer.password[bytes_read] = '\0';

            // Get customer role
            const char *prompt_role = "Enter customer role (0 for customer): ";
            send(client_socket, prompt_role, strlen(prompt_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            new_customer.role = atoi(buffer);

            // Get customer name
            const char *prompt_name = "Enter customer name: ";
            send(client_socket, prompt_name, strlen(prompt_name), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_read] = '\0';
            char name[100];
            strncpy(name, buffer, sizeof(name));

            // Get customer mobile number
            const char *prompt_mobile = "Enter customer mobile number: ";
            send(client_socket, prompt_mobile, strlen(prompt_mobile), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_read] = '\0';
            char mobile[15];
            strncpy(mobile, buffer, sizeof(mobile));

            // Assign new unique user_id
            new_customer.id = get_new_user_id();          // Generate a new user ID

            // Add new customer
            if (add_new_customer(&new_customer) == 0) {
                // Create the account with the unique user_id and initial balance
                if (create_account(new_customer.id, 0.0, name, mobile) == 0) {
                    char success_message[256];
                    snprintf(success_message, sizeof(success_message), 
                            "Customer added successfully with account ID: %d.\n", new_customer.id); // Adjust based on how account IDs are generated
                    send(client_socket, success_message, strlen(success_message), 0);
                } else {
                    const char *failure_message = "Failed to create account.\n";
                    send(client_socket, failure_message, strlen(failure_message), 0);
                }
            } else {
                const char *failure_message = "Failed to add customer.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }
            break;
        }
        
        case 2: {  // Modify Customer Details
            const char *prompt_account_id = "Enter the customer's account ID: ";
            send(client_socket, prompt_account_id, strlen(prompt_account_id), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_read] = '\0';
            int account_id = atoi(buffer);  // Get account ID

            // Ask for the new name
            const char *prompt_name = "Enter new customer name: ";
            send(client_socket, prompt_name, strlen(prompt_name), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_read] = '\0';
            char new_name[100];
            strncpy(new_name, buffer, sizeof(new_name));

            // Ask for the new mobile number
            const char *prompt_mobile = "Enter new customer mobile number: ";
            send(client_socket, prompt_mobile, strlen(prompt_mobile), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            buffer[bytes_read] = '\0';
            char new_mobile[15];
            strncpy(new_mobile, buffer, sizeof(new_mobile));

            // Modify the info.txt file for the given account_id (set is_employee = 0 for customer)
            if (modify_user_info(account_id, new_name, new_mobile, 0) == 0) {
                const char *success_msg = "Customer details updated successfully.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Failed to update customer details.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 3:  // Approve/Reject Loans
        {
            int loan_id, decision;

            // Send prompt to Terminal 2
            const char *prompt_message = "Enter Loan ID to process: ";
            send(client_socket, prompt_message, strlen(prompt_message), 0);

            // Read Loan ID from Terminal 2
            // You need to implement a function to read the input from the socket
            loan_id = read_input_from_socket(client_socket);  // This function needs to be implemented

            // Send prompt to Terminal 2 for decision
            const char *decision_prompt = "Enter 1 to approve or 2 to reject: ";
            send(client_socket, decision_prompt, strlen(decision_prompt), 0);

            // Read decision from Terminal 2
            decision = read_input_from_socket(client_socket);  // This function needs to be implemented

            // Process the loan with the decision
            if (process_approve_reject_loan(loan_id, decision, client_socket) == 0) {
                const char *success_message = "Loan processed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            } else {
                const char *fail_message = "Failed to process loan.\n";
                send(client_socket, fail_message, strlen(fail_message), 0);
            }
            break;
        }
        case 4:  // View All Loans
        {
            view_all_loans(client_socket);  // Display all loans with ID, Amount, and Status on Terminal 2
            const char *success_message = "All loans displayed.\n";
            send(client_socket, success_message, strlen(success_message), 0);
            break;
        }
        case 5: // View customer transactions
        {
            char account_id_str[128];
            const char *prompt = "Enter the customer account ID to view transactions: ";
            send(client_socket, prompt, strlen(prompt), 0);

            // Receive the account ID from the employee
            bytes_read = recv(client_socket, account_id_str, sizeof(account_id_str), 0);
            account_id_str[bytes_read] = '\0';
            int account_id = atoi(account_id_str); // Convert the input to an integer

            // View the customer's transaction history
            view_history(account_id, client_socket);
            break;
        }

        case 6: 
        {
            char old_password[128], new_password[128];

            // Ask for old password
            const char *old_pass_prompt = "Enter current password: ";
            send(client_socket, old_pass_prompt, strlen(old_pass_prompt), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';
            trim_input(old_password);  // Trim input

            // Ask for new password
            const char *new_pass_prompt = "Enter new password: ";
            send(client_socket, new_pass_prompt, strlen(new_pass_prompt), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';
            trim_input(new_password);  // Trim input

            // Debug log for passwords
            printf("Attempting to change password for user ID: %d\n", user_id);
            printf("Entered old password: '%s', new password: '%s'\n", old_password, new_password);

            // Change password for the currently logged-in user
            if (change_password(user_id, old_password, new_password, client_socket)) {
                const char *success_msg = "Password changed successfully.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Failed to change password. Please check your current password and try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 7:
            send(client_socket, "Logging out...\n\n", 15, 0);
            return 1; // Return value for logout
        case 8:
            send(client_socket, "Exiting...\n\n", 11, 0);
            return 2; // Return value for exit
        default:
            send(client_socket, "Invalid choice. Please try again.\n", 34, 0);
            break;
        }
    }
}

int display_manager_menu(int client_socket, int user_id)
{
    int choice;
    char buffer[1024];

    while (1)
    {
        // Send the Manager Menu to the client
        const char *menu = "\nManager Menu:\n"
                           "1. Activate/Deactivate Customer Accounts\n"
                           "2. Review Customer Feedback\n"
                           "3. Change Password\n"
                           "4. Logout\n"
                           "5. Exit\n"
                           "Enter your choice: ";
        send(client_socket, menu, strlen(menu), 0);

        // Receive the user's choice from the client
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0)
        {
            perror("Error receiving data from client");
            close(client_socket);
            return -1; // Error case
        }
        buffer[bytes_read] = '\0'; // Null-terminate the input
        choice = atoi(buffer);     // Convert the input to an integer

        switch (choice)
        {
        case 1: {
            int user_id, status;
            char prompt[256];

            // Prompt for user ID
            snprintf(prompt, sizeof(prompt), "Enter customer user ID to activate/deactivate: ");
            send(client_socket, prompt, strlen(prompt), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer);

            // Prompt for status (activate/deactivate)
            snprintf(prompt, sizeof(prompt), "Enter status (1 to activate, 0 to deactivate): ");
            send(client_socket, prompt, strlen(prompt), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            buffer[bytes_read] = '\0';
            status = atoi(buffer);

            // Call function to update user status
            update_user_status(user_id, status); // Call the new function
            
            // Send confirmation message to the client
            if (status == 1) {
                const char *success_message = "Customer account activated successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            } else {
                const char *success_message = "Customer account deactivated successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            break;
        }
        /*
        case 2: {
            int loan_id, employee_id;
            printf("Enter Loan ID to assign: ");
            scanf("%d", &loan_id);
            printf("Enter Employee ID to assign the loan process: ");
            scanf("%d", &employee_id);
            // Call function to assign loan application processes
            if (assign_loan_to_employee(loan_id, employee_id) == 0) {
                printf("Loan application assigned successfully.\n");
            } else {
                printf("Failed to assign loan application.\n");
            }
            break;
        }
        */
        case 2: {
            const char *review_message = "Reviewing customer feedback...\n";
            send(client_socket, review_message, strlen(review_message), 0);

            // Request the customer ID from the manager
            const char *request_id_message = "Enter Customer ID to view feedback: ";
            send(client_socket, request_id_message, strlen(request_id_message), 0);

            // Receive customer ID from manager
            int customer_id;
            recv(client_socket, &customer_id, sizeof(customer_id), 0);

            // Call function to review customer feedback based on customer ID
            review_customer_feedback(client_socket, customer_id);

            break;
        }

        case 3: {
            char old_password[128], new_password[128];

            // Ask for old password
            const char *old_pass_prompt = "Enter current password: ";
            send(client_socket, old_pass_prompt, strlen(old_pass_prompt), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';
            trim_input(old_password);  // Trim input

            // Ask for new password
            const char *new_pass_prompt = "Enter new password: ";
            send(client_socket, new_pass_prompt, strlen(new_pass_prompt), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';
            trim_input(new_password);  // Trim input

            // Call change_password with the current user_id
            if (change_password(user_id, old_password, new_password, client_socket)) {
                const char *success_msg = "Password changed successfully.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Failed to change password. Please check your current password and try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }

        case 4:
            send(client_socket, "Logging out...\n\n", 15, 0);
            return 1; // Return value for logout

        case 5:
            send(client_socket, "Exiting...\n\n", 11, 0);
            return 2; // Return value for exit

        default:
            send(client_socket, "Invalid choice. Please try again.\n", 34, 0);
            break;
        }
    }
}

int display_admin_menu(int client_socket)
{
    int choice;
    char buffer[1024];

    while (1)
    {
        // Send the Administrator Menu to the client
        const char *menu = "\nAdministrator Menu:\n"
                           "1. Add New Bank Employee\n"
                           "2. Add New Bank Manager\n"
                           "3. Modify Customer/Employee Details\n"
                           "4. Manage User Roles\n"
                           "5. Logout\n"
                           "6. Exit\n"
                           "Enter your choice: ";
        send(client_socket, menu, strlen(menu), 0);

        // Receive the user's choice from the client
        ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0)
        {
            perror("Error receiving data from client");
            close(client_socket);
            return -1; // Error case
        }
        buffer[bytes_read] = '\0'; // Null-terminate the input
        choice = atoi(buffer);     // Convert the input to an integer

        switch (choice)
        {
        case 1: {  // Add New Employee
            User new_employee;
            char name[100], mobile[15];
            new_employee.active = 1;
            // Prompt for employee username
            const char *prompt_username = "Enter employee username: ";
            send(client_socket, prompt_username, strlen(prompt_username), 0);
            bytes_read = recv(client_socket, new_employee.username, sizeof(new_employee.username) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            new_employee.username[bytes_read] = '\0';

            // Prompt for employee password
            const char *prompt_password = "Enter employee password: ";
            send(client_socket, prompt_password, strlen(prompt_password), 0);
            bytes_read = recv(client_socket, new_employee.password, sizeof(new_employee.password) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            new_employee.password[bytes_read] = '\0';

            // Prompt for employee role
            const char *prompt_role = "Enter employee role (1 for employee): ";
            send(client_socket, prompt_role, strlen(prompt_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            new_employee.role = atoi(buffer);

            // Prompt for employee name
            const char *prompt_name = "Enter employee name: ";
            send(client_socket, prompt_name, strlen(prompt_name), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            strncpy(name, buffer, sizeof(name));

            // Prompt for employee mobile number
            const char *prompt_mobile = "Enter employee mobile number: ";
            send(client_socket, prompt_mobile, strlen(prompt_mobile), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            strncpy(mobile, buffer, sizeof(mobile));

            // Add the new employee
            if (add_new_employee(&new_employee, name, mobile) == 0) {
                const char *success_message = "Bank employee added successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);

                // Send the newly created employee ID back
                char employee_id_message[128];
                sprintf(employee_id_message, "New employee ID: %d\n", new_employee.id);
                send(client_socket, employee_id_message, strlen(employee_id_message), 0);
            } else {
                const char *error_message = "Failed to add bank employee.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 2: {  // Add New Manager
            User new_manager;

            new_manager.active = 1;

            // Prompt for manager username
            const char *prompt_username = "Enter manager username: ";
            send(client_socket, prompt_username, strlen(prompt_username), 0);
            bytes_read = recv(client_socket, new_manager.username, sizeof(new_manager.username) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            new_manager.username[bytes_read] = '\0';

            // Prompt for manager password
            const char *prompt_password = "Enter manager password: ";
            send(client_socket, prompt_password, strlen(prompt_password), 0);
            bytes_read = recv(client_socket, new_manager.password, sizeof(new_manager.password) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error case
            }
            new_manager.password[bytes_read] = '\0';

            // Manager's role is fixed as '2' (indicating a manager)
            new_manager.role = 2; 

            // Add the new manager
            if (add_new_manager(&new_manager) == 0) {
                const char *success_message = "Bank manager added successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);

                // Send the newly created manager ID back
                char manager_id_message[128];
                sprintf(manager_id_message, "New manager ID: %d\n", new_manager.id);
                send(client_socket, manager_id_message, strlen(manager_id_message), 0);
            } else {
                const char *error_message = "Failed to add bank manager.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 3:
        {
            const char *prompt_modify_type = "Do you want to modify details for (1) Customer or (2) Employee? ";
            send(client_socket, prompt_modify_type, strlen(prompt_modify_type), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            int modify_type = atoi(buffer);

            int user_id;
            char new_name[128], new_mobile[64];

            // Prompt for User ID
            const char *prompt_user_id = modify_type == 1 ? "Enter Customer Account ID to modify: " : "Enter Employee ID to modify: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer);

            // Prompt for new name
            const char *prompt_new_name = "Enter new name: ";
            send(client_socket, prompt_new_name, strlen(prompt_new_name), 0);
            bytes_read = recv(client_socket, new_name, sizeof(new_name) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            new_name[bytes_read] = '\0';

            // Prompt for new mobile number
            const char *prompt_new_mobile = "Enter new mobile number: ";
            send(client_socket, prompt_new_mobile, strlen(prompt_new_mobile), 0);
            bytes_read = recv(client_socket, new_mobile, sizeof(new_mobile) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            new_mobile[bytes_read] = '\0';

            // Modify the user details (customer or employee)
            if (modify_user_info(user_id, new_name, new_mobile, modify_type == 2) == 0) // modify_type == 2 means employee
            {
                const char *success_message = "User details modified successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to modify user details.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 4:
        {
            int user_id, new_role;

            // Prompt for user ID to change role
            const char *prompt_user_id = "Enter user ID to change role: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error receiving user ID
            }
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer); // Convert user ID from string to integer

            // Prompt for new role
            const char *prompt_new_role = "Enter new role (0 for customer, 1 for employee): ";
            send(client_socket, prompt_new_role, strlen(prompt_new_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0) {
                close(client_socket);
                return -1; // Error receiving new role
            }
            buffer[bytes_read] = '\0';
            new_role = atoi(buffer); // Convert new role from string to integer

            // Change user role
            if (manage_user_roles(user_id, new_role) == 0) {
                const char *success_message = "User role changed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            } else {
                const char *error_message = "Failed to change user role. Please check if the user ID exists.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        /*
        case 4: 
        {
            char old_password[128], new_password[128];

            // Ask for old password
            const char *old_pass_prompt = "Enter current password: ";
            send(client_socket, old_pass_prompt, strlen(old_pass_prompt), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';

            // Ask for new password
            const char *new_pass_prompt = "Enter new password: ";
            send(client_socket, new_pass_prompt, strlen(new_pass_prompt), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';

            // Change password for the currently logged-in user
            if (change_password(account_id, old_password, new_password, client_socket)) {
                const char *success_msg = "Password changed successfully.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            } else {
                const char *error_msg = "Failed to change password. Please check your current password and try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        */
        case 5:
            send(client_socket, "Logging out...\n\n", 15, 0);
            return 1; // Return value for logout
        case 6:
            send(client_socket, "Exiting...\n\n", 11, 0);
            return 2; // Return value for exit
        default:
            send(client_socket, "Invalid choice. Please try again.\n", 34, 0);
            break;
        }
    }
}