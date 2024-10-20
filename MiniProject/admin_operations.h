// admin_operations.h
#ifndef ADMIN_OPERATIONS_H
#define ADMIN_OPERATIONS_H
#include "user_management.h"

int add_new_customer(User *customer);
int add_new_employee(User *employee, const char *name, const char *mobile);
int modify_user_info(int user_id, const char* new_name, const char* new_mobile, int is_employee);
int manage_user_roles(int user_id, int new_role);
int get_next_user_id();
int add_new_manager(User *manager);

#endif