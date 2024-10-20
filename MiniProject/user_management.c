// user_management.c
#include "user_management.h"
#define USER_DB "users.dat"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

/*
int login(char *username, char *password) {
    // TODO: Implement login logic
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open users database");
        return -1;
    }

    User user;
    while (fread(&user, sizeof(User), 1, fp)) {
        // Check if the username matches
        if (strcmp(user.username, username) == 0) {
            // Check if the password matches
            if (strcmp(user.password, password) == 0) {
                printf("Login successful for user: %s\n", username);
                fclose(fp);
                return user.id;  // Return the user ID on successful login
            } else {
                printf("Incorrect password for user: %s\n", username);
                fclose(fp);
                return -1;  // Password mismatch
            }
        }
    }

    printf("User not found: %s\n", username);
    fclose(fp);
    return -1;  // Username not found
}
*/

void print_users() {
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open users database");
        return;
    }

    User user;
    while (fread(&user, sizeof(User), 1, fp)) {
        printf("User ID: %d, Username: %s, Password: %s, Role: %d\n", user.id, user.username, user.password, user.role);
    }

    fclose(fp);
}

void print_ascii(const char *str) {
    while (*str) {
        printf("%c: %d ", *str, *str);
        str++;
    }
    printf("\n");
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';  // Replace newline with null terminator
    }
}

int login(char *username, char *password) {
    // Trim any newline characters from the username and password
    trim_newline(username);
    trim_newline(password);

    // Debugging log to print input values
    printf("Attempting login for Username: '%s', Password: '%s'\n", username, password);
    
    // Open the user database
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open users database");
        return -1; // Error opening file
    }

    User user;

    while (fread(&user, sizeof(User), 1, fp)) {
        // Debugging log to print user data read from the database
        printf("Read user: ID = %d, Username = '%s', Password = '%s', Active = %d\n", user.id, user.username, user.password, user.active);
        
        // Check if the username matches
        if (strcmp(user.username, username) == 0) {
            // Check if the password matches
            if (strcmp(user.password, password) == 0) {
                // Check if the account is active
                if (user.active == 1) {
                    printf("Login successful for user: %s\n", username);
                    fclose(fp);
                    return user.id;  // Return the user ID on successful login
                } else {
                    printf("Account is deactivated for user: %s. Cannot login.\n", username);
                    fclose(fp);
                    return -3;  // Account is deactivated
                }
            } else {
                printf("Incorrect password for user: %s\n", username);
                fclose(fp);
                return -2;  // Password mismatch
            }
        }
    }

    fclose(fp);
    return -1;  // Username not found
}


int logout(int user_id) {
    // TODO: Implement logout logic
    printf("User with ID %d logged out.\n", user_id);
    return 0;  // Indicate successful logout
}

bool change_password(int user_id, const char* old_password, const char* new_password, int client_socket) {
    printf("Attempting to change password for user ID: %d\n", user_id);  // Debug log
    int fd = open(USER_DB, O_RDWR);
    if (fd == -1) {
        perror("Failed to open user database");
        return false;
    }

    User user;
    bool success = false;

    while (read(fd, &user, sizeof(User)) == sizeof(User)) {
        if (user.id == user_id) {
            printf("User ID found: %d. Checking password...\n", user.id);  // Debug log
            if (strcmp(user.password, old_password) == 0) {
                // Ensure new password is copied safely
                strncpy(user.password, new_password, sizeof(user.password) - 1);
                user.password[sizeof(user.password) - 1] = '\0';  // Null-termination

                lseek(fd, -sizeof(User), SEEK_CUR);
                if (write(fd, &user, sizeof(User)) != sizeof(User)) {
                    perror("Failed to write updated user data");
                } else {
                    success = true;
                }
            } else {
                printf("Old password is incorrect. Entered: %s, Expected: %s\n", old_password, user.password);  // Debug log
            }
            break;
        }
    }

    if (!success) {
        printf("User ID %d not found.\n", user_id);  // Debug log
    }

    close(fd);
    return success;
}

int change_user_role(int user_id, int new_role) {
    FILE *fp = fopen(USER_DB, "rb+");
    if (fp == NULL) {
        perror("Failed to open user database");
        return -1;
    }

    User user;
    int found = 0;

    while (fread(&user, sizeof(User), 1, fp) == 1) {
        if (user.id == user_id) {
            user.role = new_role; // Update role

            fseek(fp, -sizeof(User), SEEK_CUR); // Move the file pointer to overwrite this user
            if (fwrite(&user, sizeof(User), 1, fp) != 1) {
                perror("Failed to update user role");
                fclose(fp);
                return -1;
            }
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("User ID %d not found\n", user_id);
        return -1;
    }

    printf("User ID %d role changed to %d\n", user_id, new_role);
    return 0;
}

void print_user_database() {
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open user database");
        return;
    }

    User user;
    printf("Current users in the database:\n");
    while (fread(&user, sizeof(User), 1, fp)) {
        printf("ID: %d, Username: %s, Password: %s, Role: %d\n",
               user.id, user.username, user.password, user.role);
    }

    fclose(fp);
}

void update_user_status(int user_id, int status) {
    FILE *fp = fopen(USER_DB, "rb+");
    if (fp == NULL) {
        perror("Failed to open users database");
        return;
    }

    User user;
    int found = 0; // To track if the user was found
    while (fread(&user, sizeof(User), 1, fp)) {
        if (user.id == user_id) {
            user.active = status; // Set active status
            fseek(fp, -sizeof(User), SEEK_CUR); // Go back to the position of the current user
            fwrite(&user, sizeof(User), 1, fp); // Write the updated user back to the file
            printf("Updated user ID %d to active status %d.\n", user_id, status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("User ID %d not found for status update.\n", user_id);
    }

    fclose(fp);
}