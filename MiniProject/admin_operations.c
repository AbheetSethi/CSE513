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
int add_new_employee(User *employee) {
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
    
    // Debug: Confirm employee addition with ID
    printf("Added new employee: ID = %d, Username = %s, Role = %d\n", employee->id, employee->username, employee->role);
    close(fd);
    return 0; // Success
}

// Function to modify user details
int modify_user_details(int user_id, User *new_details) {
    int fd = open(USER_DB, O_RDWR);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1;
    }

    User user;
    int found = 0;

    while (read(fd, &user, sizeof(User)) == sizeof(User)) {
        if (user.id == user_id) {
            lseek(fd, -sizeof(User), SEEK_CUR); // Move cursor back to the position of the user
            if (write(fd, new_details, sizeof(User)) != sizeof(User)) {
                perror("Failed to write updated user data");
                close(fd);
                return -1;
            }
            found = 1;
            break;
        }
    }

    close(fd);
    return found ? 0 : -1; // Return 0 if found and modified, -1 if not found
}

// Function to manage user roles
int manage_user_roles(int user_id, int new_role) {
    int fd = open(USER_DB, O_RDWR);
    if (fd == -1) {
        perror("Failed to open user database");
        return -1;
    }

    User user;
    int found = 0;

    while (read(fd, &user, sizeof(User)) == sizeof(User)) {
        if (user.id == user_id) {
            user.role = new_role; // Update the user role
            lseek(fd, -sizeof(User), SEEK_CUR); // Move cursor back to the position of the user
            if (write(fd, &user, sizeof(User)) != sizeof(User)) {
                perror("Failed to write updated user data");
                close(fd);
                return -1;
            }
            found = 1;
            break;
        }
    }

    close(fd);
    return found ? 0 : -1; // Return 0 if found and modified, -1 if not found
}