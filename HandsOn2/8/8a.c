/* Name: 8a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a separate program using signal system call to catch the following signals.
                        a. SIGSEGV

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// Signal handler for SIGSEGV
void sigsegv_handler(int signum) 
{
    printf("Caught SIGSEGV: Segmentation Fault\n");
    exit(1);
}

int main() 
{
    // Register the SIGSEGV handler
    signal(SIGSEGV, sigsegv_handler);

    // Cause a segmentation fault
    int *ptr = NULL;
    *ptr = 42;  // This will cause a segmentation fault

    return 0;
}

/*

Command: ./8a

Output:

Caught SIGSEGV: Segmentation Fault

*/
