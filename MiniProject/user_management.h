// user_management.h
#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdbool.h>
#include <stdio.h>    // For printf and perror
#include <string.h>   // For strcmp and strncpy

typedef struct {
    int id;                 // User ID
    char username[50];      // User's username
    char password[50];      // User's password
    int role;               // User role: 1: Customer, 2: Employee, 3: Manager, 4: Admin
    int active;             // Account status: 1 for active, 0 for inactive
} User;

int login(char *username, char *password);
int logout(int user_id);
bool change_password(int user_id, const char* old_password, const char* new_password, int client_socket);
int change_user_role(int user_id, int new_role);
void print_user_database();
void print_users();
void trim_newline(char *str);
void update_user_status(int user_id, int status) ;

#endif
