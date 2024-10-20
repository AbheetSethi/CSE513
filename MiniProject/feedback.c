#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>  // For close function
#include "feedback.h"

void review_customer_feedback(int client_socket) {
    FILE *fp = fopen("feedback.txt", "r");
    if (fp == NULL) {
        perror("Failed to open feedback file");
        const char *error_message = "Error: Could not open feedback file.\n";
        send(client_socket, error_message, strlen(error_message), 0);
        return;
    }

    int customer_id;
    char buffer[100];
    // Ask for the customer ID
    const char *id_prompt = "Enter the Customer ID to review feedback: ";
    send(client_socket, id_prompt, strlen(id_prompt), 0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    customer_id = atoi(buffer);  // Convert the input to an integer

    char line[256];
    char feedback[1000] = "";
    int feedback_found = 0;

    // Temporary file to store feedback we retain
    FILE *temp_fp = fopen("temp_feedback.txt", "w");

    // Read each line of feedback
    while (fgets(line, sizeof(line), fp) != NULL) {
        int feedback_id;
        sscanf(line, "Customer ID: %d", &feedback_id);

        // If the feedback belongs to the specified customer, show it
        if (feedback_id == customer_id) {
            feedback_found = 1;
            strcat(feedback, line);
        } else {
            // Write the feedback to the temporary file if it doesn't match
            fputs(line, temp_fp);
        }
    }

    if (feedback_found) {
        send(client_socket, feedback, strlen(feedback), 0);

        // Ask if the manager wants to mark as reviewed or review later
        const char *choice_prompt = "\n1. Reviewed\n2. Will review later\nEnter your choice: ";
        send(client_socket, choice_prompt, strlen(choice_prompt), 0);
        recv(client_socket, buffer, sizeof(buffer), 0);
        int choice = atoi(buffer);

        if (choice == 1) {
            const char *reviewed_message = "Feedback marked as reviewed and deleted.\n";
            send(client_socket, reviewed_message, strlen(reviewed_message), 0);
            // Do not copy the feedback to the temp file, it's already removed
        } else {
            const char *later_message = "Feedback will be reviewed later and retained.\n";
            send(client_socket, later_message, strlen(later_message), 0);
            // Write feedback back to the temp file since it is retained
            fputs(feedback, temp_fp);
        }
    } else {
        const char *not_found_message = "No feedback found for this Customer ID.\n";
        send(client_socket, not_found_message, strlen(not_found_message), 0);
    }

    fclose(fp);
    fclose(temp_fp);

    // Replace the old feedback file with the updated one
    remove("feedback.txt");
    rename("temp_feedback.txt", "feedback.txt");

    const char *end_of_feedback = "\nEnd of Feedback Review.\n";
    send(client_socket, end_of_feedback, strlen(end_of_feedback), 0);
}

int add_feedback(int user_id, const char *feedback) {
    FILE *fp = fopen(FEEDBACK_FILE, "a");
    if (fp == NULL) {
        perror("Failed to open feedback file");
        return -1;
    }

    Feedback new_feedback;
    new_feedback.user_id = user_id;
    strncpy(new_feedback.feedback, feedback, sizeof(new_feedback.feedback) - 1);
    new_feedback.feedback[sizeof(new_feedback.feedback) - 1] = '\0';  // Ensure null-termination

    // Change to "Customer ID" for consistency
    if (fprintf(fp, "Customer ID: %d, Feedback: %s\n", new_feedback.user_id, new_feedback.feedback) < 0) {
        perror("Failed to write feedback to file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

void view_feedback() {
    FILE *fp = fopen(FEEDBACK_FILE, "r");
    if (fp == NULL) {
        perror("Failed to open feedback file");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
}
