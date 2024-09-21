/* Name: 25.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
                        a. access permission
                        b. uid, gid
                        c. time of last message sent and received 
                        d. time of last change in the message queue
                        d. size of the queue
                        f. number of messages in the queue 
                        g. maximum number of bytes allowed 
                        h. pid of the msgsnd and msgrcv
 * Date: 19 Sept 2024
*/

#include <sys/types.h>  // Import for `key_t`, `msgctl`, `msgget`
#include <sys/ipc.h>    // Import for `ftok`
#include <sys/msg.h>    // Import for `msgctl`
#include <stdio.h>      
#include <stdlib.h>     // Import for `exit`
#include <time.h>       // Import for `ctime` to format time

int main() 
{
    key_t key;
    int msgid;
    struct msqid_ds buf; // Structure to hold queue information
    struct ipc_perm perm; // Structure for access permission details

    // Generate a unique key for the message queue
    key = ftok("25_sample", 65); // "progfile" is any existing file, 65 is an arbitrary identifier
    if (key == -1) 
    {
        perror("ftok failed");
        exit(1);
    }

    // Create or access the message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) 
    {
        perror("msgget failed");
        exit(1);
    }

    // Get the details of the message queue
    if (msgctl(msgid, IPC_STAT, &buf) == -1)
    {
        perror("msgctl failed");
        exit(1);
    }

    // Access permission and ownership details
    perm = buf.msg_perm;
    printf("Access permissions: %o\n", perm.mode);  // Permissions in octal
    printf("UID: %d\n", perm.uid);
    printf("GID: %d\n", perm.gid);

    // Time of last message sent
    if (buf.msg_stime != 0)
        printf("Time of last message sent: %s", ctime(&(buf.msg_stime)));
    else
        printf("No message sent yet.\n");

    // Time of last message received
    if (buf.msg_rtime != 0)
        printf("Time of last message received: %s", ctime(&(buf.msg_rtime)));
    else
        printf("No message received yet.\n");

    // Time of last change in the queue
    printf("Time of last change: %s", ctime(&(buf.msg_ctime)));

    // Size of the message queue (total number of bytes)
    printf("Size of the queue (bytes): %lu\n", buf.__msg_cbytes);

    // Number of messages in the queue
    printf("Number of messages in the queue: %lu\n", buf.msg_qnum);

    // Maximum number of bytes allowed in the queue
    printf("Maximum number of bytes allowed: %lu\n", buf.msg_qbytes);

    // PIDs of the last send and receive operations
    printf("PID of last msgsnd: %d\n", buf.msg_lspid);
    printf("PID of last msgrcv: %d\n", buf.msg_lrpid);

    return 0;
}

/*

Command: ./25

Output:
Access permissions: 666
UID: 1000
GID: 1000
No message sent yet.
No message received yet.
Time of last change: Tue Sep 10 17:52:53 2024
Size of the queue (bytes): 0
Number of messages in the queue: 0
Maximum number of bytes allowed: 16384
PID of last msgsnd: 0
PID of last msgrcv: 0

*/
