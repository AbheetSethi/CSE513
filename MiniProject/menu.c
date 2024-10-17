#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "account_operations.h"  // For balance, deposit, and withdraw
#include "transaction_history.h" // For viewing transaction history
#include "feedback.h"            // For adding feedback
#include "menu.h"
#include "user_management.h"  // For user-related functions
#include "loan_management.h"  // For loan-related functions
#include "admin_operations.h" // For admin functions

#define USER_DB "users.dat"
#define LOAN_DB "loans.dat"

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
                           "5. Add Feedback\n"
                           "6. Logout\n"
                           "7. Exit\n"
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
       case 2:
        {
            // Deposit money
            print_all_accounts();
            const char *deposit_prompt = "Enter amount to deposit: ";
            send(client_socket, deposit_prompt, strlen(deposit_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0)
            {
                perror("Error receiving data from client");
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            double amount = atof(buffer);

            // Call deposit function with user_id
            if (deposit(user_id, amount) == 0) // Use user_id here instead of account_id
            {
                // Add transaction after successful deposit
                add_transaction(user_id, amount, "Deposit"); // You may need to adjust this if your add_transaction function requires account_id

                const char *success_msg = "Deposit successful.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            }
            else
            {
                const char *error_msg = "Deposit failed. Please try again.\n";
                send(client_socket, error_msg, strlen(error_msg), 0);
            }
            break;
        }
        case 3:
        {
            print_all_accounts();
            // Withdraw money
            const char *withdraw_prompt = "Enter amount to withdraw: ";
            send(client_socket, withdraw_prompt, strlen(withdraw_prompt), 0);

            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_read <= 0)
            {
                perror("Error receiving data from client");
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            double amount = atof(buffer);

            if (withdraw(account_id, amount) == 0)
            {
                // Add transaction after successful withdrawal
                add_transaction(account_id, amount, "Withdrawal");

                const char *success_msg = "Withdrawal successful.\n";
                send(client_socket, success_msg, strlen(success_msg), 0);
            }
            else
            {
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
        case 6:
        {
            // Logout
            const char *logout_msg = "You have been logged out.\n";
            send(client_socket, logout_msg, strlen(logout_msg), 0);
            // Handle logout logic if needed
            break;
        }
        case 7:
        {
            // Exit
            const char *exit_msg = "Exiting...\n";
            send(client_socket, exit_msg, strlen(exit_msg), 0);
            close(client_socket);
            exit(0); // Exit the program
        }

        default:
            const char *invalid_msg = "Invalid option. Please try again.\n";
            send(client_socket, invalid_msg, strlen(invalid_msg), 0);
            break;
        }
    } while (option != 6 && option != 7);

    return 0; // This line won't be reached due to the loop, but it's good practice to return an int.
}

void register_customer(int client_socket, int user_id) {
    int account_id;
    char buffer[1024];
    double initial_balance;
    
    // Generate a new account ID
    account_id = get_new_account_id();

    // Ask for initial balance for the customer through the socket
    const char *prompt_balance = "Enter initial balance for customer: ";
    send(client_socket, prompt_balance, strlen(prompt_balance), 0);

    // Receive the initial balance from the client
    int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0) {
        printf("Error receiving initial balance.\n");
        return;
    }
    buffer[bytes_read] = '\0';  // Null-terminate the input
    initial_balance = atof(buffer);  // Convert the received string to a double

    // Create the account for the new user with the initial balance
    int result = create_account(account_id, user_id, initial_balance);
    if (result == 0) {
        const char *success_message = "Account successfully created.\n";
        send(client_socket, success_message, strlen(success_message), 0);
        printf("Account successfully created for User ID %d with Account ID %d.\n", user_id, account_id);
    } else {
        const char *failure_message = "Failed to create account.\n";
        send(client_socket, failure_message, strlen(failure_message), 0);
        printf("Failed to create account for User ID %d.\n", user_id);
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
                           "3. Process Loan Applications\n"
                           "4. Approve/Reject Loans\n"
                           "5. View Assigned Loan Applications\n"
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
        case 1:
        {
            User new_customer;

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

            // Add new customer
            if (add_new_customer(&new_customer) == 0)
            {
                const char *success_message = "Customer added successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);

                // Register the account for the new customer (asks for initial balance)
                register_customer(client_socket, new_customer.id); // Pass the client_socket to handle I/O
            }
            else
            {
                const char *failure_message = "Failed to add customer.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }

            break;
        }
        case 2:
        {
            int customer_id;
            User updated_details;

            // Get customer ID to modify
            const char *prompt_id = "Enter customer ID to modify: ";
            send(client_socket, prompt_id, strlen(prompt_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            customer_id = atoi(buffer);

            // Get new customer username
            const char *prompt_username = "Enter new username: ";
            send(client_socket, prompt_username, strlen(prompt_username), 0);
            bytes_read = recv(client_socket, updated_details.username, sizeof(updated_details.username), 0);
            updated_details.username[bytes_read] = '\0';

            // Get new customer password
            const char *prompt_password = "Enter new password: ";
            send(client_socket, prompt_password, strlen(prompt_password), 0);
            bytes_read = recv(client_socket, updated_details.password, sizeof(updated_details.password), 0);
            updated_details.password[bytes_read] = '\0';

            // Get new role
            const char *prompt_role = "Enter new role (0 for customer, 1 for employee): ";
            send(client_socket, prompt_role, strlen(prompt_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            updated_details.role = atoi(buffer);

            // Modify customer details
            if (modify_user_details(customer_id, &updated_details) == 0)
            {
                const char *success_message = "Customer details modified successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *failure_message = "Failed to modify customer details.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }
            break;
        }
        case 3:
        {
            int loan_id;

            // Get loan ID to process
            const char *prompt_loan_id = "Enter Loan ID to process: ";
            send(client_socket, prompt_loan_id, strlen(prompt_loan_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            loan_id = atoi(buffer);

            // Process loan
            if (process_loan(loan_id, employee_id) == 0)
            {
                const char *success_message = "Loan processed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *failure_message = "Failed to process loan.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }
            break;
        }
        case 4:
        {
            int loan_id, manager_id, decision;

            // Get loan ID to approve/reject
            const char *prompt_loan_id = "Enter Loan ID to approve/reject: ";
            send(client_socket, prompt_loan_id, strlen(prompt_loan_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            loan_id = atoi(buffer);

            // Get manager ID
            const char *prompt_manager_id = "Enter your manager ID: ";
            send(client_socket, prompt_manager_id, strlen(prompt_manager_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            manager_id = atoi(buffer);

            // Get decision (1 for approve, 2 for reject)
            const char *prompt_decision = "Enter 1 to approve or 2 to reject: ";
            send(client_socket, prompt_decision, strlen(prompt_decision), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
            decision = atoi(buffer);

            // Approve/reject loan
            if (approve_reject_loan(loan_id, manager_id, decision) == 0)
            {
                const char *success_message = "Loan processed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *failure_message = "Failed to process loan.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }
            break;
        }
        case 5:
        {
            const char *view_message = "Viewing assigned loan applications...\n";
            send(client_socket, view_message, strlen(view_message), 0);

            // View assigned loans for the employee
            view_assigned_loans(employee_id);
            break;
        }
        case 6:
        {
            char old_password[50], new_password[50];

            // Get old password
            const char *prompt_old_password = "Enter your old password: ";
            send(client_socket, prompt_old_password, strlen(prompt_old_password), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';

            // Get new password
            const char *prompt_new_password = "Enter your new password: ";
            send(client_socket, prompt_new_password, strlen(prompt_new_password), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';

            // Change password
            if (change_password(user_id, old_password, new_password) == 0)
            {
                const char *success_message = "Password changed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *failure_message = "Failed to change password.\n";
                send(client_socket, failure_message, strlen(failure_message), 0);
            }
            break;
        }
        case 7:
            send(client_socket, "Logging out...\n", 15, 0);
            return 1; // Return value for logout
        case 8:
            send(client_socket, "Exiting...\n", 11, 0);
            return 2; // Return value for exit
        default:
            send(client_socket, "Invalid choice. Please try again.\n", 34, 0);
            break;
        }
    }
}

int display_manager_menu(int client_socket)
{
    int choice;
    char buffer[1024];

    while (1)
    {
        // Send the Manager Menu to the client
        const char *menu = "\nManager Menu:\n"
                           "1. Review Customer Feedback\n"
                           "2. Change Password\n"
                           "3. Logout\n"
                           "4. Exit\n"
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
        /*
        case 1: {
            int user_id, status;
            printf("Enter customer user ID to activate/deactivate: ");
            scanf("%d", &user_id);
            printf("Enter status (1 to activate, 0 to deactivate): ");
            scanf("%d", &status);
            // Call function to activate/deactivate customer accounts
            if (activate_deactivate_account(user_id, status) == 0) {
                printf("Customer account updated successfully.\n");
            } else {
                printf("Failed to update customer account.\n");
            }
            break;
        }
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
        case 1:
        {
            // Review customer feedback
            const char *review_message = "Reviewing customer feedback...\n";
            send(client_socket, review_message, strlen(review_message), 0);
            review_customer_feedback(); // Call function to review customer feedback
            break;
        }
        case 2:
        {
            int user_id;
            char old_password[50], new_password[50];

            // Prompt for user ID
            const char *prompt_user_id = "Enter your user ID: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            user_id = atoi(buffer);

            // Prompt for old password
            const char *prompt_old_password = "Enter your old password: ";
            send(client_socket, prompt_old_password, strlen(prompt_old_password), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password), 0);
            old_password[bytes_read] = '\0';

            // Prompt for new password
            const char *prompt_new_password = "Enter your new password: ";
            send(client_socket, prompt_new_password, strlen(prompt_new_password), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password), 0);
            new_password[bytes_read] = '\0';

            // Change the password
            if (change_password(user_id, old_password, new_password) == 0)
            {
                const char *success_message = "Password changed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to change password.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 3:
            send(client_socket, "Logging out...\n", 15, 0);
            return 1; // Return value for logout
        case 4:
            send(client_socket, "Exiting...\n", 11, 0);
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
                           "2. Modify Customer/Employee Details\n"
                           "3. Manage User Roles\n"
                           "4. Change Password\n"
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
        case 1:
        { // Add New Employee
            User new_employee;
            // Prompt for employee username
            const char *prompt_username = "Enter employee username: ";
            send(client_socket, prompt_username, strlen(prompt_username), 0);
            bytes_read = recv(client_socket, new_employee.username, sizeof(new_employee.username) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            new_employee.username[bytes_read] = '\0';

            // Prompt for employee password
            const char *prompt_password = "Enter employee password: ";
            send(client_socket, prompt_password, strlen(prompt_password), 0);
            bytes_read = recv(client_socket, new_employee.password, sizeof(new_employee.password) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            new_employee.password[bytes_read] = '\0';

            // Prompt for employee role
            const char *prompt_role = "Enter employee role (1 for employee): ";
            send(client_socket, prompt_role, strlen(prompt_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            new_employee.role = atoi(buffer);

            // Debug: Confirm new employee details before adding
            printf("New employee details: Username = %s, Password = %s, Role = %d\n",
                   new_employee.username, new_employee.password, new_employee.role);

            // Add the new employee
            if (add_new_employee(&new_employee) == 0)
            {
                const char *success_message = "Bank employee added successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to add bank employee.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }

            print_user_database();
            break;
        }
        case 2:
        {
            int user_id;
            User updated_details;

            // Prompt for user ID
            const char *prompt_user_id = "Enter user ID to modify: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer);

            // Prompt for new username
            const char *prompt_new_username = "Enter new username: ";
            send(client_socket, prompt_new_username, strlen(prompt_new_username), 0);
            bytes_read = recv(client_socket, updated_details.username, sizeof(updated_details.username) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            updated_details.username[bytes_read] = '\0';

            // Prompt for new password
            const char *prompt_new_password = "Enter new password: ";
            send(client_socket, prompt_new_password, strlen(prompt_new_password), 0);
            bytes_read = recv(client_socket, updated_details.password, sizeof(updated_details.password) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            updated_details.password[bytes_read] = '\0';

            // Prompt for new role
            const char *prompt_new_role = "Enter new role (0 for customer, 1 for employee): ";
            send(client_socket, prompt_new_role, strlen(prompt_new_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            updated_details.role = atoi(buffer);

            // Modify user details
            if (modify_user_details(user_id, &updated_details) == 0)
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
        case 3:
        {
            int user_id, new_role;

            // Prompt for user ID to change role
            const char *prompt_user_id = "Enter user ID to change role: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer);

            // Prompt for new role
            const char *prompt_new_role = "Enter new role (0 for customer, 1 for employee): ";
            send(client_socket, prompt_new_role, strlen(prompt_new_role), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            new_role = atoi(buffer);

            // Change user role
            if (change_user_role(user_id, new_role) == 0)
            {
                const char *success_message = "User role changed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to change user role.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 4:
        {
            int user_id;
            char old_password[50], new_password[50];

            // Prompt for user ID
            const char *prompt_user_id = "Enter your user ID: ";
            send(client_socket, prompt_user_id, strlen(prompt_user_id), 0);
            bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            buffer[bytes_read] = '\0';
            user_id = atoi(buffer);

            // Prompt for old password
            const char *prompt_old_password = "Enter your old password: ";
            send(client_socket, prompt_old_password, strlen(prompt_old_password), 0);
            bytes_read = recv(client_socket, old_password, sizeof(old_password) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            old_password[bytes_read] = '\0';

            // Prompt for new password
            const char *prompt_new_password = "Enter your new password: ";
            send(client_socket, prompt_new_password, strlen(prompt_new_password), 0);
            bytes_read = recv(client_socket, new_password, sizeof(new_password) - 1, 0);
            if (bytes_read <= 0)
            {
                close(client_socket);
                return -1; // Error case
            }
            new_password[bytes_read] = '\0';

            // Change the password
            if (change_password(user_id, old_password, new_password) == 0)
            {
                const char *success_message = "Password changed successfully.\n";
                send(client_socket, success_message, strlen(success_message), 0);
            }
            else
            {
                const char *error_message = "Failed to change password.\n";
                send(client_socket, error_message, strlen(error_message), 0);
            }
            break;
        }
        case 5:
            send(client_socket, "Logging out...\n", 15, 0);
            return 1; // Return value for logout
        case 6:
            send(client_socket, "Exiting...\n", 11, 0);
            return 2; // Return value for exit
        default:
            send(client_socket, "Invalid choice. Please try again.\n", 34, 0);
            break;
        }
    }
}