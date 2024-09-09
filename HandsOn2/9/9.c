/* Name: 9.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal- Use signal system call.
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGINT (can be used later if needed)
void sigint_handler(int signum) {
    printf("Caught SIGINT (Ctrl+C), but ignoring it!\n");
}

int main() {
    // Ignore the SIGINT signal (Ctrl+C)
    signal(SIGINT, SIG_IGN);
    printf("SIGINT (Ctrl+C) is now being ignored. Try pressing Ctrl+C...\n");

    // Sleep for 10 seconds while SIGINT is ignored
    sleep(10);

    // Reset the signal handler to default action
    signal(SIGINT, SIG_DFL);
    printf("SIGINT (Ctrl+C) is now back to the default action. Try pressing Ctrl+C again...\n");

    // Sleep for another 10 seconds, during which SIGINT is handled by default action
    sleep(10);

    printf("Program completed without SIGINT interrupt.\n");

    return 0;
}

/*

Command: ./9

Output:
SIGINT (Ctrl+C) is now being ignored. Try pressing Ctrl+C...
SIGINT (Ctrl+C) is now back to the default action. Try pressing Ctrl+C again...
Program completed without SIGINT interrupt.

Command: ./9

Output:
SIGINT (Ctrl+C) is now being ignored. Try pressing Ctrl+C...

Command: ^C

Output:
^C^C^C^CSIGINT (Ctrl+C) is now back to the default action. Try pressing Ctrl+C again...

Command: ^C

Output:

* Program terminated

*/