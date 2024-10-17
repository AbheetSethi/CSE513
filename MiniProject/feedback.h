#ifndef FEEDBACK_H
#define FEEDBACK_H

#define FEEDBACK_FILE "feedback.txt"

typedef struct {
    int user_id;
    char feedback[256];
} Feedback;

int add_feedback(int user_id, const char *feedback);
void view_feedback();
void review_customer_feedback();

#endif
