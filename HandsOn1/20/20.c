/* Name: 20.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Find out the priority of your running program. Modify the prioriy with nice command

 * Date: 22 Aug 2024
*/

#include <unistd.h> // Contains `nice` system call
#include <stdio.h>  
#include <stdlib.h> // Contains `atoi`- it converts from string to int

void main(int argc, char *argv[])
{
    int priority, newp;
    if (argc != 2)
        printf("Please pass the integer value to be added to the current nice value\n");
    else
    {
        newp = atoi(argv[1]);
        priority = nice(0); // Get the current priority by adding 0 to the current nice value
        printf("Current priority: %d\n", priority);
        priority = nice(newp); // Add `newp` to the current nice value
        printf("New priority: %d\n", priority);
    }
}

/*

Command: ./20

Output:
Please pass the integer value to be added to the current nice value

Command: ./20 10

Output:
Current priority: 0
New priority: 10

*/
