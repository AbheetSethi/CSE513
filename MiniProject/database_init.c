#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "user_management.h"
#include "account_operations.h"
#include "loan_management.h"

#define USER_DB "users.dat"
#define ACCOUNT_DB "accounts.dat"
#define LOAN_DB "loans.dat"
#define TRANSACTION_DB "transactions.dat"
#define FEEDBACK_FILE "feedback.txt"

// Function to create the initial admin user
void create_initial_user() {
    FILE *fp = fopen(USER_DB, "ab");  // Open in append mode
    if (fp == NULL) {
        perror("Failed to create user database");
        return;
    }
    User admin = {1, "admin", "admin123", 4, 1};  // Role 4 assumed for admin
    fwrite(&admin, sizeof(User), 1, fp);
    fclose(fp);
    printf("Created initial admin user with ID: %d\n", admin.id);
}

// Function to initialize the database and ensure admin and manager users are created
int initialize_database() {
    struct stat st = {0};

    // Check and create users database
    if (stat(USER_DB, &st) == -1) {
        int fd = open(USER_DB, O_RDWR | O_CREAT, 0644);
        if (fd == -1) {
            perror("Failed to create users database");
            return -1;
        }
        close(fd);
        printf("Created users database.\n");
    }

    // Open the user database for reading
    User temp_user;
    FILE *fp = fopen(USER_DB, "rb");
    if (fp == NULL) {
        perror("Failed to open users database for reading");
        return -1;
    }

    int admin_exists = 0;

    // Check if admin and manager users already exist
    while (fread(&temp_user, sizeof(User), 1, fp)) {
        printf("Checking existing user: %s\n", temp_user.username); // Debug line
        if (strcmp(temp_user.username, "admin") == 0) {
            admin_exists = 1;
        }
    }
    fclose(fp);

    // Create admin if not exists
    if (!admin_exists) {
        create_initial_user();
    } else {
        printf("Admin user already exists.\n");
    }

    return 0;
}
