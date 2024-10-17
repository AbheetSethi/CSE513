#ifndef MENU_H
#define MENU_H

int display_customer_menu(int client_socket, int account_id);
int display_employee_menu(int client_socket, int user_id);
int display_manager_menu(int client_socket);
int display_admin_menu(int client_socket);
void review_customer_feedback();
int change_user_role(int user_id, int new_role);

#endif // MENU_H