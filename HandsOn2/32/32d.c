/* Name: 32d.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to implement semaphore to protect any critical section.
                        d. remove the created semaphore.
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int main() {
    sem_t sem;

    // Initialize semaphore
    if (sem_init(&sem, 0, 1) == -1) {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore created and initialized.\n");

    // Destroy semaphore
    if (sem_destroy(&sem) == -1) {
        perror("sem_destroy failed");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore destroyed successfully.\n");

    return 0;
}

/*

Command: ./32d

Output:
Semaphore created and initialized.
Semaphore destroyed successfully.

*/