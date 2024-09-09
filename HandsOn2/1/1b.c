/* Name: 1b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program (for each time domain) to set a interval timer in 10 sec and 10 micro second
                            b. ITIMER_VIRTUAL

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void handle_alarm(int sig) {
    printf("ITIMER_VIRTUAL triggered\n");
}

int main() {
    struct itimerval timer;

    // Set up signal handler
    signal(SIGVTALRM, handle_alarm);
    // This timer sends the SIGVTALRM signal when the process spends the given amount of time in user mode.

    // Set initial delay to 10 seconds and interval to 10 microseconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10;

    // Start ITIMER_VIRTUAL
    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("Error setting ITIMER_VIRTUAL");
        exit(EXIT_FAILURE);
    }

    // Infinite loop to keep the program running
    while (1);

    return 0;
}