/* Name: 28.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to get maximum and minimum real time priority
 * Date: 28 Aug 2024
*/

#include <sched.h> // Import for `sched_get_priority_*` functions
#include <stdio.h>

int main()
{
    int max_Priority, min_Priority;

    max_Priority = sched_get_priority_max(SCHED_RR);
    if (max_Priority == -1)
    {
        perror("Error while finding max priority\n");
    }    
    else
    {
        printf("The max priority with RR Scheduling Policy is : %d\n", max_Priority);
    }

    min_Priority = sched_get_priority_min(SCHED_RR);
    if (min_Priority == -1)
    {
        perror("Error while finding min priority\n");
    }
    else
    {
        printf("The min priority with RR Scheduling Policy is : %d\n", min_Priority);
    }

    max_Priority = sched_get_priority_max(SCHED_FIFO);
    if (max_Priority == -1)
    {
        perror("Error while finding max priority\n");
    }    
    else
    {
        printf("The max priority with FIFO Scheduling Policy is : %d\n", max_Priority);
    }

    min_Priority = sched_get_priority_min(SCHED_FIFO);
    if (min_Priority == -1)
    {
        perror("Error while finding min priority\n");
    }
    else
    {
        printf("The min priority with FIFO Scheduling Policy is : %d\n", min_Priority);
    }

    return 0;
}

/*

Command: ./28

Output:

The max priority with RR Scheduling Policy is : 99
The min priority with RR Scheduling Policy is : 1
The max priority with FIFO Scheduling Policy is : 99
The min priority with FIFO Scheduling Policy is : 1

*/
