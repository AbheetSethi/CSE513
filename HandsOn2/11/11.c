/* Name: 11.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal using sigaction system call.
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h> // For sleep function

// Function to ignore the SIGINT signal
void ignore_signal(int signum) 
{
    printf("SIGINT signal ignored (signal number: %d)\n", signum);
}

int main() 
{
    struct sigaction action, reset_action;
    int status;

    // Step 1: Set the handler to ignore SIGINT
    action.sa_handler = SIG_IGN;           // Ignore the signal
    sigemptyset(&action.sa_mask);          // Block no signals during handler execution
    action.sa_flags = 0;                   // No special flags

    // Apply the action for SIGINT
    status = sigaction(SIGINT, &action, NULL);
    if (status == -1) 
    {
        perror("Error while setting signal handler to ignore SIGINT");
        return 1;
    }

    printf("SIGINT is now being ignored for 10 seconds. Try pressing Ctrl+C.\n");
    sleep(10);  // Give time to test ignoring SIGINT

    // Step 2: Reset SIGINT to default behavior
    reset_action.sa_handler = SIG_DFL;     // Default action for SIGINT
    sigemptyset(&reset_action.sa_mask);    // Block no signals during handler execution
    reset_action.sa_flags = 0;             // No special flags

    // Apply the reset action for SIGINT
    status = sigaction(SIGINT, &reset_action, NULL);
    if (status == -1) 
    {
        perror("Error while resetting signal handler to default for SIGINT");
        return 1;
    }

    printf("SIGINT is now back to its default action. Try pressing Ctrl+C again.\n");
    sleep(10);  // Give time to test default SIGINT handling

    return 0;
}

/*

Command: ./11

Output:
SIGINT is now being ignored for 10 seconds. Try pressing Ctrl+C.
^C^C^CSIGINT is now back to its default action. Try pressing Ctrl+C again.
^C

*/
