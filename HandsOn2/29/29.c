/* Name: 29.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to remove the message queue.
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgget`, `msgctl`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgget`, `msgctl`
#include <stdio.h>      
#include <stdlib.h>     // Import for `exit`

int main() 
{
    key_t key;
    int msgid;

    // Generate a unique key for the message queue
    key = ftok("29_sample", 65);  // Use the same file and identifier as used for queue creation
    if (key == -1) 
    {
        perror("ftok failed");
        exit(1);
    }

    // Access the message queue
    msgid = msgget(key, 0666);  // Use the same key and mode as when accessing the queue
    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(1);
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) 
    {
        perror("msgctl IPC_RMID failed");
        exit(1);
    }

    printf("Message queue removed successfully.\n");

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
