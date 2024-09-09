/* Name: 8e.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        e. SIGALRM (use setitimer system call)
                        
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Signal handler for SIGALRM
void sigalrm_handler(int signum) 
{
    printf("Caught SIGALRM: Timer expired\n");
}

int main() 
{
    struct itimerval timer;

    // Register the SIGALRM handler
    signal(SIGALRM, sigalrm_handler);

    // Configure the timer to expire after 2 seconds, then every 2 seconds
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 2;
    timer.it_interval.tv_usec = 0;

    // Set the timer
    setitimer(ITIMER_REAL, &timer, NULL);

    // Infinite loop waiting for alarm signal
    printf("Waiting for timer to expire...\n");
    while (1);   // Do nothing, just wait for the signal

    return 0;
}

/*

Command: ./8e

Output:
Waiting for timer to expire...
Caught SIGALRM: Timer expired
Caught SIGALRM: Timer expired
.
.
.

*/