// admin_operations.h
#ifndef ADMIN_OPERATIONS_H
#define ADMIN_OPERATIONS_H
#include "user_management.h"

int add_new_customer(User *customer);
int add_new_employee(User *employee);
int modify_user_details(int user_id, User *new_details);
int manage_user_roles(int user_id, int new_role);
int get_next_user_id();

#endif