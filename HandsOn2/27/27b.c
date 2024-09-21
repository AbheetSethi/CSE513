/* Name: 27b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to receive messages from the message queue.
                        b. with IPC_NOWAIT as a flag
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`, `msgrcv`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`, `msgrcv`
#include <stdio.h>      // Import for `perror` and `printf`
#include <stdlib.h>     // Import for `exit`

// Define a structure for the message
struct message_buffer {
    long message_type;
    char message_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct message_buffer msg;

    // Generate a unique key for the message queue
    key = ftok("27_sample", 65);  // "progfile" is any existing file, 65 is an arbitrary identifier
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Try to receive a message from the queue with IPC_NOWAIT flag (non-blocking)
    if (msgrcv(msgid, &msg, sizeof(msg.message_text), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv failed (queue might be empty)");
        exit(1);
    }

    // Print the received message
    printf("Message received: %s\n", msg.message_text);

    return 0;
}

/*

Command: ./27b

Output:
msgrcv failed (queue might be empty): No message of desired type

Command: ./27

Output:
Message sent: Hello from the sender - MT2024004

Command: ./27b

Output:
Message received: Hello from the sender - MT2024004

*/
