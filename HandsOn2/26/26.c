/* Name: 26.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to send messages to the message queue. check $ipcs -q
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgsnd`, `msgget`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgsnd`
#include <stdio.h>      
#include <stdlib.h>     // Import for `exit`
#include <string.h>     

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
    key = ftok("26_sample", 65);  // "progfile" is any existing file, 65 is an arbitrary identifier
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // Create or access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Prepare the message
    msg.message_type = 1;  // Message type (must be > 0)
    strcpy(msg.message_text, "Hello, this is a test message!");

    // Send the message to the queue
    if (msgsnd(msgid, &msg, sizeof(msg.message_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Message sent successfully to the queue!\n");

    return 0;
}

/*

Command: ./26
Message sent successfully to the queue!

Command: ipcs -q

Output:
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x410722c1 0          abheet     666        0            0           
0x410722c6 1          abheet     666        0            0           
0x410722e3 2          abheet     666        100          1   

*/
