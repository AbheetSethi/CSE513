/* Name: 24.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create a message queue and print the key and message queue id.
 * Date: 12 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`
#include <stdio.h>      
#include <stdlib.h>     // Import for `exit`

int main()
{
    key_t key;          // Key for the message queue
    int msgid;          // Message queue ID

    // Generate a unique key for the message queue
    key = ftok("24_sample", 65);  // "progfile" is any existing file, 65 is an arbitrary project identifier

    if (key == -1) 
    {
        perror("ftok failed");
        exit(1);
    }

    // Create a message queue with read/write permissions for user
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(1);
    }

    // Print the generated key and message queue ID
    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}

/*

Command: ./24

Output:
Message Queue Key: 1090986689
Message Queue ID: 0

*/