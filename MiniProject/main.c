#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "user_management.h"
#include "account_operations.h"
#include "loan_management.h"
#include "admin_operations.h"
#include "database_init.h"
#include "menu.h"  // Include menu header for menu functions

#define PORT 8080
#define BACKLOG 5  // Number of connections allowed on the incoming queue

int initialize_server_socket() {
    int server_fd;
    struct sockaddr_in server_addr;

    // Create the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket created successfully.\n");

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(PORT);  // Convert port number to network byte order

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }
    printf("Binding successful.\n");

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }
    printf("Server listening on port %d...\n", PORT);

    return server_fd;  // Return the file descriptor for the server socket
}

void *handle_client(void *arg) {
    int client_socket = *(int*)arg;
    char buffer[1024];
    ssize_t bytes_read;
    int role_choice;
    char username[50];
    char password[50];

    while (1) {  // Loop to allow returning to role selection
        // Send a message to the client asking to select their role
        const char *role_menu = "Select your role:\n"
                                "1. Customer\n"
                                "2. Employee\n"
                                "3. Manager\n"
                                "4. Administrator\n"
                                "Enter your choice: ";
        send(client_socket, role_menu, strlen(role_menu), 0);

        // Receive the role choice from the client
        bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read < 0) {
            perror("Error receiving data");
            close(client_socket);
            free(arg);
            pthread_exit(NULL);
        }
        buffer[bytes_read] = '\0';  // Null-terminate the received string

        // Convert the received string to an integer (role_choice)
        role_choice = atoi(buffer);
        printf("Role choice received: %d\n", role_choice);  // Debug output

        // Prompt for username
        send(client_socket, "Enter your username: ", 21, 0);
        bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        buffer[bytes_read] = '\0';  // Null-terminate the received string
        strcpy(username, buffer);  // Store the username
        printf("Username received: %s\n", username);  // Debug output

        // Prompt for password
        send(client_socket, "Enter your password: ", 21, 0);
        bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        buffer[bytes_read] = '\0';  // Null-terminate the received string
        strcpy(password, buffer);  // Store the password
        printf("Password received for user: %s\n", username);  // Debug output

        // Attempt to log in using the existing logic in user_management.c
        int user_id = login(username, password);
        if (user_id < 0) {
            printf("Login failed for user: %s\n", username);
            const char *login_failed = "Login failed. Disconnecting.\n";
            send(client_socket, login_failed, strlen(login_failed), 0);
            close(client_socket);
            free(arg);
            pthread_exit(NULL);
        }
        printf("Login successful for user ID: %d\n", user_id);  // Debug output

        // Use a switch-case to display the appropriate menu based on the choice
        switch (role_choice) {
        case 1:
            printf("Customer selected.\n");
            int customer_menu_result = display_customer_menu(client_socket, user_id);
            if (customer_menu_result == 1) {
                // Handle logout: return to role selection
                printf("Customer logging out. Returning to role selection...\n");
            } else if (customer_menu_result == 2) {
                // Handle exit: close application
                printf("Exiting application...\n");
                exit(0);
            }
            break;
        case 2:
            printf("Employee selected.\n");
            int employee_menu_result = display_employee_menu(client_socket, user_id);
            if (employee_menu_result == 1) {
                // Handle logout: return to role selection
                printf("Employee logging out. Returning to role selection...\n");
            } else if (employee_menu_result == 2) {
                // Handle exit: close application
                printf("Exiting application...\n");
                exit(0);
            }
            break;
        case 3:
            printf("Manager selected.\n");
            int manager_menu_result = display_manager_menu(client_socket, user_id);
            if (manager_menu_result == 1) {
                // Handle logout: return to role selection
                printf("Manager logged out. Returning to role selection...\n");
                continue; // Return to the beginning of the main menu loop
            } else if (manager_menu_result == 2) {
                // Handle exit: cleanup and terminate if necessary
                printf("Exiting application...\n");
                close(client_socket); // Close socket or perform other cleanup
                exit(0);
            }
            break;
        case 4:
            printf("Administrator selected.\n");
            int admin_menu_result = display_admin_menu(client_socket);
            if (admin_menu_result == 1) {
                // Handle logout: return to role selection
                printf("Administrator logging out. Returning to role selection...\n");
                continue; // Return to the beginning of the main menu loop
            } else if (admin_menu_result == 2) {
                // Handle exit: cleanup and terminate if necessary
                printf("Exiting application...\n");
                close(client_socket); // Close socket or perform other cleanup
                exit(0);
            }
            break;
        default:
            printf("Invalid role selected. Disconnecting client.\n");
            const char *invalid_role = "Invalid role selection. Disconnecting.\n";
            send(client_socket, invalid_role, strlen(invalid_role), 0);
            break;
        }

        // After handling the role, optionally prompt to return to role selection
        const char *return_prompt = "Returning to role selection...\n";
        send(client_socket, return_prompt, strlen(return_prompt), 0);
    }

    // Close the socket and clean up
    close(client_socket);
    free(arg);  // Free the allocated memory for client_socket
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (initialize_database() != 0) {
        fprintf(stderr, "Failed to initialize database. Exiting.\n");
        return 1;
    }

    // Initialize server socket
    int server_socket = initialize_server_socket();
    if (server_socket < 0) {
        return 1;  // Exit if socket initialization fails
    }

    // Accept client connections
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t tid;

    while (1) {
        int *client_socket = malloc(sizeof(int));  // Allocate memory for client socket
        if (client_socket == NULL) {
            perror("Failed to allocate memory for client socket");
            continue;
        }

        *client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (*client_socket < 0) {
            perror("Failed to accept client connection");
            free(client_socket);
            continue; // Continue to the next iteration to accept more connections
        }

        printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client request
        if (pthread_create(&tid, NULL, handle_client, client_socket) != 0) {
            perror("Failed to create thread for client");
            close(*client_socket);
            free(client_socket);
            continue; // Continue to accept more clients
        }

        // Detach the thread to allow it to clean up after itself
        pthread_detach(tid);
    }

    close(server_socket); // This line is technically unreachable due to the infinite loop
    return 0;
}