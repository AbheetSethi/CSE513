/* Name: 29_message.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to remove the message queue.
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`
#include <stdio.h>      // Import for `perror`, `printf`
#include <stdlib.h>     // Import for `exit`

int main() {
    key_t key;
    int msgid;

    // Generate a unique key for the message queue
    key = ftok("29_sample", 65);  // Use the same file and identifier as used for queue creation
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Create the message queue
    msgid = msgget(key, IPC_CREAT | 0666);  // Use IPC_CREAT to create the queue if it does not exist
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Message queue created successfully with ID: %d\n", msgid);

    return 0;
}

/*

Command: ./29_message

Output:
Message queue created successfully with ID: 8

Command: ./29

Output:
Message queue removed successfully.

*/