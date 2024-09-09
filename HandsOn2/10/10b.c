/* Name: 10b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using sigaction system call to catch the following signals
                        b. SIGINT
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGINT
void sigint_handler(int signum) {
    printf("Caught SIGINT: Interrupt from keyboard\n");
    exit(0);
}

int main() {
    struct sigaction sa;

    // Set up the sigaction structure
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Install the SIGINT handler
    sigaction(SIGINT, &sa, NULL);

    // Infinite loop waiting for SIGINT signal
    while (1) {
        printf("Running... Press Ctrl+C to trigger SIGINT\n");
        sleep(1);
    }

    return 0;
}

/*

Command: ./10b

Output:
Running... Press Ctrl+C to trigger SIGINT
Running... Press Ctrl+C to trigger SIGINT
Running... Press Ctrl+C to trigger SIGINT
Running... Press Ctrl+C to trigger SIGINT
.
.
.
^CCaught SIGINT: Interrupt from keyboard

*/