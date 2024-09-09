/* Name: 8f.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        f. SIGVTALRM (use setitimer system call)
                        
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Signal handler for SIGVTALRM
void sigvtalrm_handler(int signum) 
{
    printf("Caught SIGVTALRM: Virtual timer expired\n");
}

int main() 
{
    struct itimerval timer;

    // Register the SIGVTALRM handler
    signal(SIGVTALRM, sigvtalrm_handler);

    // Configure the timer to expire after 1 second, then every 1 second
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // Set the virtual timer
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    // Infinite loop waiting for virtual timer signal
    printf("Waiting for virtual timer...\n");
    while (1);  // Simulate some CPU-bound work

    return 0;
}

/*

Command: ./8f

Output:
Waiting for virtual timer...
Caught SIGVTALRM: Virtual timer expired
Caught SIGVTALRM: Virtual timer expired
Caught SIGVTALRM: Virtual timer expired
.
.
.

*/