/* Name: 3.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to set (any one) system resource limit. Use setrlimit system call.

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <errno.h>

int main() 
{
    struct rlimit limit;
    
    // Get the current limit for the number of file descriptors (RLIMIT_NOFILE)
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) 
    {
        perror("Error getting RLIMIT_NOFILE");
        exit(EXIT_FAILURE);
    }
    
    printf("Current soft limit: %lu\n", limit.rlim_cur);
    printf("Current hard limit: %lu\n", limit.rlim_max);
    
    // Set new soft and hard limits for RLIMIT_NOFILE (number of file descriptors)
    // For example, set the soft limit to 2048 and the hard limit to 4096
    limit.rlim_cur = 2048; // new soft limit
    limit.rlim_max = 4096; // new hard limit

    // Set the new limits using setrlimit
    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) 
    {
        perror("Error setting RLIMIT_NOFILE");
        if (errno == EPERM) 
        {
            printf("Permission denied. You may need root privileges to increase the hard limit.\n");
        }
        exit(EXIT_FAILURE);
    }

    // Confirm the new limits have been set
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) 
    {
        perror("Error getting new RLIMIT_NOFILE");
        exit(EXIT_FAILURE);
    }

    printf("New soft limit: %lu\n", limit.rlim_cur);
    printf("New hard limit: %lu\n", limit.rlim_max);

    return 0;
}

/*

Command: ./3

Output:
Current soft limit: 1024
Current hard limit: 1048576
New soft limit: 2048
New hard limit: 4096

*/