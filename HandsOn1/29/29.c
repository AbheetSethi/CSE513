/* Name: 29.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to get scheduling policy and modifying the scheduling policy (SCHED_FIFO, SCHED_RR)
 * Date: 28 Aug 2024
*/

#include <sched.h> // Import for 'sched_getscheduler' and 'sched_setscheduler'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void print_policy(int policy) 
{
    switch(policy) 
    {
        case SCHED_OTHER:
            printf("Current Scheduling Policy: SCHED_OTHER\n");
            break;

        case SCHED_FIFO:
            printf("Current Scheduling Policy: SCHED_FIFO\n");
            break;

        case SCHED_RR:
            printf("Current Scheduling Policy: SCHED_RR\n");
            break;

        default:
            printf("Unknown Scheduling Policy\n");
    }
}

int main() 
{
    int pid = getpid();
    int policy = sched_getscheduler(pid);

    if (policy == -1) 
    {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    print_policy(policy);

    // Modify the scheduling policy to SCHED_FIFO
    struct sched_param priority;
    priority.sched_priority = 10;  // Set priority

    if (sched_setscheduler(pid, SCHED_FIFO, &priority) == -1) 
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    printf("Scheduling policy changed to SCHED_FIFO with priority %d\n", priority.sched_priority);

    // Verify the change
    policy = sched_getscheduler(pid);
    if (policy == -1) 
    {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    print_policy(policy);

    // Modify the scheduling policy to SCHED_RR
    priority.sched_priority = 5;  // Set priority (1 is minimum real-time priority)

    if (sched_setscheduler(pid, SCHED_RR, &priority) == -1) 
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }

    printf("Scheduling policy changed to SCHED_RR with priority %d\n", priority.sched_priority);

    // Verify the change
    policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    print_policy(policy);

    return 0;
}

/*

Command: ./29

Output:

Current Scheduling Policy: SCHED_OTHER
sched_setscheduler: Operation not permitted

Command: sudo ./29

Output:

[sudo] password for abheet: 
Current Scheduling Policy: SCHED_OTHER
Scheduling policy changed to SCHED_FIFO with priority 10
Current Scheduling Policy: SCHED_FIFO
Scheduling policy changed to SCHED_RR with priority 5
Current Scheduling Policy: SCHED_RR

*/
