#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "feedback.h"

void review_customer_feedback() {
    FILE *fp = fopen("feedback.txt", "r");
    if (fp == NULL) {
        perror("Failed to open feedback file");
        return;
    }

    char line[256];
    printf("Customer Feedback:\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
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

    if (fprintf(fp, "User ID: %d, Feedback: %s\n", new_feedback.user_id, new_feedback.feedback) < 0) {
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
