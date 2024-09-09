/* Name: 8g.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        g. SIGPROF (use setitimer system call)
                        
 * Date: 5 Sept 2024
*/


#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Signal handler for SIGPROF
void sigprof_handler(int signum) 
{
    printf("Caught SIGPROF: Profiling timer expired\n");
}

int main() 
{
    struct itimerval timer;

    // Register the SIGPROF handler
    signal(SIGPROF, sigprof_handler);

    // Configure the profiling timer to expire after 3 seconds, then every 3 seconds
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;

    // Set the profiling timer
    setitimer(ITIMER_PROF, &timer, NULL);

    // Infinite loop waiting for profiling timer signal
    printf("Waiting for profiling timer...\n");
    while (1);      // Simulate some CPU-bound work

    return 0;
}

/*

Command: ./8g

Output:
Waiting for profiling timer...
Caught SIGPROF: Profiling timer expired
Caught SIGPROF: Profiling timer expired
.
.
.

*/