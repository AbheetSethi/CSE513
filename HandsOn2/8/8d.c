/* Name: 8d.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        d. SIGALRM (use alarm system call)
                        
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGALRM
void sigalrm_handler(int signum) 
{
    printf("Caught SIGALRM: Alarm triggered\n");
}

int main() 
{
    // Register the SIGALRM handler
    signal(SIGALRM, sigalrm_handler);

    // Set an alarm to trigger after 5 seconds
    alarm(5);

    // Infinite loop waiting for alarm signal
    printf("Waiting for alarm...\n");
    while (1);   // Do nothing, just wait for the signal
    
    return 0;
}

/*

Command: ./8d

Output:

Waiting for alarm...
Caught SIGALRM: Alarm triggered

*/
