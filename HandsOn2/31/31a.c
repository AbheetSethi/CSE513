/* Name: 31a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create a semaphore and initialize value to the semaphore.
                        a. create a binary semaphore
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int main() 
{
    sem_t bin_sem;

    // Initialize a binary semaphore with value 1
    if (sem_init(&bin_sem, 0, 1) != 0) 
    {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }

    printf("Binary semaphore created and initialized successfully.\n");

    // Destroy the semaphore when done
    sem_destroy(&bin_sem);

    return 0;
}

/*

Command: ./31a

Output:
Binary semaphore created and initialized successfully.

*/
