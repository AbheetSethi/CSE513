/* Name: 8b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        b. SIGINT
                        
 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// Signal handler for SIGINT
void sigint_handler(int signum) 
{
    printf("Caught SIGINT: Interrupt from keyboard\n");
    exit(0);
}

int main() 
{
    // Register the SIGINT handler
    signal(SIGINT, sigint_handler);

    // Infinite loop waiting for signal
    while (1) 
    {
        printf("Running... Press Ctrl+C to exit\n");
    }

    return 0;
}

/*

Command: ./8b

Output:

Running... Press Ctrl+C to exit
Running... Press Ctrl+C to exit
^CRunning... Press Ctrl+C to exit
Caught SIGINT: Interrupt from keyboard

*/