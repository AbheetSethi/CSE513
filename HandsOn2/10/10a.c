/* Name: 10a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using sigaction system call to catch the following signals
                        a. SIGSEGV
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGSEGV
void sigsegv_handler(int signum) {
    printf("Caught SIGSEGV: Segmentation Fault\n");
    exit(1);
}

int main() {
    struct sigaction sa;

    // Set up the sigaction structure
    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Install the SIGSEGV handler
    sigaction(SIGSEGV, &sa, NULL);

    // Cause a segmentation fault
    int *ptr = NULL;
    *ptr = 42;  // This will cause a segmentation fault

    return 0;
}

/*

Command: ./10a

Output:

Caught SIGSEGV: Segmentation Fault

*/

