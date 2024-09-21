/* Name: 28_message_queue.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to change the existing message queue permission. (use msqid_ds structure)
 * Date: 19 Sept 2024
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    key_t key;
    int msgid;

    // Generate a unique key for the message queue
    key = ftok("28_sample", 65);  // Use the same file and identifier as in the permissions change program
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Create a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Message queue created with ID: %d\n", msgid);

    return 0;
}

/*

Command: ./28_message_queue

Output:
Message queue created with ID: 7

Command: ./28

Output:
Current permissions: 666
Updated permissions: 644

*/