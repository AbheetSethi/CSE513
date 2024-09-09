/* Name: 2.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to print the system resource limits. Use getrlimit system call.

 * Date: 5 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

// Function to print the resource limit
void print_limit(int resource, const char *name) 
{
    struct rlimit limit;
    
    // Get the limit for the resource
    if (getrlimit(resource, &limit) == -1) 
    {
        perror("Error getting resource limit");
        return;
    }
    
    // Print the soft and hard limits
    printf("%-20s Soft limit: ", name);
    if (limit.rlim_cur == RLIM_INFINITY) 
    {
        printf("Unlimited");
    } 
    
    else 
    {
        printf("%lu", limit.rlim_cur);
    }

    printf(", Hard limit: ");
    if (limit.rlim_max == RLIM_INFINITY) 
    {
        printf("Unlimited\n");
    } 
    
    else 
    {
        printf("%lu\n", limit.rlim_max);
    }
}

int main() 
{
    printf("System Resource Limits:\n");

    // Print limits for various system resources
    print_limit(RLIMIT_CPU, "CPU time (seconds)");
    print_limit(RLIMIT_FSIZE, "File size (bytes)");
    print_limit(RLIMIT_DATA, "Data size (bytes)");
    print_limit(RLIMIT_STACK, "Stack size (bytes)");
    print_limit(RLIMIT_CORE, "Core file size (bytes)");
    print_limit(RLIMIT_RSS, "Resident set size (bytes)");
    print_limit(RLIMIT_NOFILE, "Number of file descriptors");
    print_limit(RLIMIT_AS, "Address space (bytes)");
    print_limit(RLIMIT_NPROC, "Number of processes");
    print_limit(RLIMIT_MEMLOCK, "Locked memory (bytes)");
    print_limit(RLIMIT_LOCKS, "File locks");
    print_limit(RLIMIT_SIGPENDING, "Pending signals");
    print_limit(RLIMIT_MSGQUEUE, "Message queue bytes");
    print_limit(RLIMIT_NICE, "Max nice priority");
    print_limit(RLIMIT_RTPRIO, "Real-time priority");
    print_limit(RLIMIT_RTTIME, "Real-time timeout (us)");

    return 0;
}

/*

Command: ./2

Output:
System Resource Limits:
CPU time (seconds)   Soft limit: Unlimited, Hard limit: Unlimited
File size (bytes)    Soft limit: Unlimited, Hard limit: Unlimited
Data size (bytes)    Soft limit: Unlimited, Hard limit: Unlimited
Stack size (bytes)   Soft limit: 8388608, Hard limit: Unlimited
Core file size (bytes) Soft limit: 0, Hard limit: Unlimited
Resident set size (bytes) Soft limit: Unlimited, Hard limit: Unlimited
Number of file descriptors Soft limit: 1024, Hard limit: 1048576
Address space (bytes) Soft limit: Unlimited, Hard limit: Unlimited
Number of processes  Soft limit: 62724, Hard limit: 62724
Locked memory (bytes) Soft limit: 2064936960, Hard limit: 2064936960
File locks           Soft limit: Unlimited, Hard limit: Unlimited
Pending signals      Soft limit: 62724, Hard limit: 62724
Message queue bytes  Soft limit: 819200, Hard limit: 819200
Max nice priority    Soft limit: 0, Hard limit: 0
Real-time priority   Soft limit: 0, Hard limit: 0
Real-time timeout (us) Soft limit: Unlimited, Hard limit: Unlimited

*/