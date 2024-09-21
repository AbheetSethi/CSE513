/* Name: 28.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to change the existing message queue permission. (use msqid_ds structure)
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`, `msgctl`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`, `msgctl`
#include <stdio.h>      
#include <stdlib.h>     // Import for `exit`

int main() {
    key_t key;
    int msgid;
    struct msqid_ds msgq_ds;

    // Generate a unique key for the message queue
    key = ftok("28_sample", 65);  // "progfile" is any existing file, 65 is an arbitrary identifier
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Access the message queue
    msgid = msgget(key, 0666);  // Use the same key and mode as when creating the queue
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Get the current message queue attributes
    if (msgctl(msgid, IPC_STAT, &msgq_ds) == -1) {
        perror("msgctl IPC_STAT failed");
        exit(1);
    }

    // Print the current permissions
    printf("Current permissions: %o\n", msgq_ds.msg_perm.mode);

    // Modify the permissions (e.g., set new permissions to 0644)
    msgq_ds.msg_perm.mode = 0644;

    // Set the new permissions
    if (msgctl(msgid, IPC_SET, &msgq_ds) == -1) {
        perror("msgctl IPC_SET failed");
        exit(1);
    }

    // Print the updated permissions
    printf("Updated permissions: %o\n", msgq_ds.msg_perm.mode);

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