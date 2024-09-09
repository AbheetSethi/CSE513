/* Name: 13b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write two programs: first program is waiting to catch SIGSTOP signal, the second program 
                      will send the signal (using kill system call). Find out whether the first program is able to catch 
                      the signal or not.
 * Date: 5 Sept 2024
*/

#include <sys/types.h> // Import for `kill`
#include <signal.h>    // Import for `kill`
#include <unistd.h>    // Import for `sleep`, `_exit`
#include <stdio.h>     // Import for `perror` & `printf`
#include <stdlib.h> // Import for `atoi`

void main(int argc, char *argv[])
{
    int killStatus; // Determines success of `kill`
    pid_t pid;

    if (argc != 2)
    {
        printf("Pass the PID of the process to whom the SIGSTOP signal is to be sent!\n");
        _exit(0);
    }

    pid = atoi(argv[1]);

    killStatus = kill(pid, SIGSTOP);

    if(!killStatus) 
        printf("Successfully sent SIGSTOP signal to process (%d)\n", pid);
    else 
        perror("Error while sending signal!");
}

/*

Terminal 1:
Command: ./13a

Output: 
Process ID: 5034
Error while catching signal!: Invalid argument
Putting the process to sleep for 15s

Terminal 2:
Command: ./13b 5034

Output:
Successfully sent SIGSTOP signal to process (5034)

Terminal 1:
Output:
[1]+  Stopped                 ./13a

*/