/* Name: 27.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to receive messages from the message queue.
                        a. with 0 as a flag
                        b. with IPC_NOWAIT as a flag
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`, `msgsnd`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`, `msgsnd`
#include <stdio.h>      // Import for `perror`, `printf`
#include <stdlib.h>     // Import for `exit`
#include <string.h>     // Import for `strcpy`

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

    // Create a message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Prepare the message to send
    msg.message_type = 1;  // Message type must be greater than 0
    strcpy(msg.message_text, "Hello from the sender - MT2024004");

    // Send the message to the queue
    if (msgsnd(msgid, &msg, sizeof(msg.message_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Message sent: %s\n", msg.message_text);

    return 0;
}

/*

Command: ./27a

Output:
* The program waits for a message to arrive if no message is immediately available.

Command: ./27
Message sent: Hello from the sender - MT2024004

Command: ./27a

Output:
Message received: Hello from the sender - MT2024004

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
