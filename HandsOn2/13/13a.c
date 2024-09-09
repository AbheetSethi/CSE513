/* Name: 13a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write two programs: first program is waiting to catch SIGSTOP signal, the second program 
                      will send the signal (using kill system call). Find out whether the first program is able to catch 
                      the signal or not.
 * Date: 5 Sept 2024
*/

#include <signal.h> // Import for `signal`
#include <unistd.h> // Import for `sleep`, `_exit`, `getpid`
#include <stdio.h>  // Import for `perror` & `printf`
#include <stdlib.h> // Import for `atoi`

void customHandler()
{
    printf("Received SIGSTOP signal!\n");
    _exit(0);
}

void main()
{
    __sighandler_t signalStatus; // Determines success of `signal`
    pid_t pid = getpid();

    printf("Process ID: %d\n", pid);

    signalStatus = signal(SIGSTOP, customHandler);
    if (signalStatus == SIG_ERR)
        perror("Error while catching signal!");

    printf("Putting the process to sleep for 15s\n");
    sleep(15);
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
