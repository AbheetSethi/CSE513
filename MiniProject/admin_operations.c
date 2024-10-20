// admin_operations.c

#include "admin_operations.h"
#include "user_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define USER_DB "users.dat"
#define ACCOUNT_DB "accounts.dat"

int get_next_user_id() {
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open user database");
        return 1; // Return 1 if the file does not exist or is empty
    }

    User user;
    user.active = 1;
    int max_id = 0;

    while (fread(&user, sizeof(User), 1, fp)) {
        if (user.id > max_id) {
            max_id = user.id;
        }
    }

    fclose(fp);
    return max_id + 1; // Return the next available ID
}

// Function to add a new customer
int add_new_customer(User *customer) {
    // Trim any newline characters from the username and password before adding
    trim_newline(customer->username);
    trim_newline(customer->password);

    // Debugging log to confirm customer details
    printf("Adding new customer: Username = '%s', Password = '%s', Role = %d\n", customer->username, customer->password, customer->role);
    
    int fd = open(USER_DB, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1;
    }
    
    if (write(fd, customer, sizeof(User)) != sizeof(User)) {
        perror("Failed to add new customer");
        close(fd);
        return -1;
    }

    close(fd);
    return 0; // Success
}

// Function to add a new employee
int add_new_employee(User *employee, const char *name, const char *mobile) {
    // Assign a new unique ID to the employee
    employee->id = get_next_user_id(); 
    
    // Trim newline characters
    trim_newline(employee->username);
    trim_newline(employee->password);

    int fd = open(USER_DB, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1;
    }
    
    if (write(fd, employee, sizeof(User)) != sizeof(User)) {
        perror("Failed to add new employee");
        close(fd);
        return -1;
    }

    // Save name and mobile in info.txt
    FILE *info_file = fopen("info.txt", "a");
    if (info_file == NULL) {
        perror("Failed to open info.txt");
        close(fd);
        return -1;
    }
    // Ensure the format is consistent
    fprintf(info_file, "Employee ID: %d, Name: %s, Mobile: %s\n", employee->id, name, mobile);
    fclose(info_file);
    
    // Debug: Confirm employee addition with ID
    printf("Added new employee: ID = %d, Username = %s, Role = %d\n", employee->id, employee->username, employee->role);
    close(fd);
    return 0; // Success
}

// Function to modify user details
int modify_user_info(int user_id, const char* new_name, const char* new_mobile, int is_employee) {
    FILE *fp = fopen("info.txt", "r+");  // Open file in read/write mode
    if (fp == NULL) {
        perror("Failed to open info.txt");
        return -1;
    }

    char buffer[1024];
    long pos = -1;  // Position for updating
    int found = 0;

    // Read each line and search for the user ID (customer or employee)
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, is_employee ? "Employee ID:" : "Account ID:")) {
            int id;
            sscanf(buffer, is_employee ? "Employee ID: %d," : "Account ID: %d,", &id);  // Extract the user ID
            if (id == user_id) {
                found = 1;
                pos = ftell(fp);  // Get the current position in the file
                break;
            }
        }
    }

    if (!found) {
        printf("%s ID %d not found.\n", is_employee ? "Employee" : "Account", user_id);
        fclose(fp);
        return -1;
    }

    // Move back to the start of the matching line
    fseek(fp, pos - strlen(buffer), SEEK_SET);

    // Update the line with new name and mobile number
    if (is_employee) {
        fprintf(fp, "Employee ID: %d, Name: %s, Mobile: %s\n", user_id, new_name, new_mobile);
    } else {
        fprintf(fp, "Account ID: %d, Name: %s, Mobile: %s\n", user_id, new_name, new_mobile);
    }

    fclose(fp);
    return 0;  // Success
}

// Function to manage user roles
int manage_user_roles(int user_id, int new_role) {
    int fd = open(USER_DB, O_RDWR);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1; // Error opening file
    }

    User user;
    int found = 0;

    // Read each user record from the file
    while (read(fd, &user, sizeof(User)) == sizeof(User)) {
        if (user.id == user_id) {
            user.role = new_role; // Update the user role

            // Move the file pointer to the position of the current user record
            if (lseek(fd, -sizeof(User), SEEK_CUR) == -1) {
                perror("Failed to seek back to user position");
                close(fd);
                return -1;
            }

            // Write the updated user role to the file
            if (write(fd, &user, sizeof(User)) != sizeof(User)) {
                perror("Failed to write updated user data");
                close(fd);
                return -1; // Error writing back to file
            }

            found = 1; // Mark the user as found and updated
            break; // Exit the loop once the user is updated
        }
    }

    close(fd); // Close the file

    return found ? 0 : -1; // Return 0 if found and modified, -1 if not found
}

int add_new_manager(User *manager) {
    // Assign a new unique ID to the manager
    manager->id = get_next_user_id(); 
    
    // Trim newline characters
    trim_newline(manager->username);
    trim_newline(manager->password);

    int fd = open(USER_DB, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1;
    }
    
    // Write the manager information to the database
    if (write(fd, manager, sizeof(User)) != sizeof(User)) {
        perror("Failed to add new manager");
        close(fd);
        return -1;
    }

    // Debug: Confirm manager addition with ID
    printf("Added new manager: ID = %d, Username = %s, Role = %d\n", manager->id, manager->username, manager->role);
    close(fd);
    return 0; // Success
}