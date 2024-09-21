/* Name: 31b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create a semaphore and initialize value to the semaphore.
                        b. create a counting semaphore
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int main() {
    sem_t count_sem;
    unsigned int initial_value = 5; // Example initial value for counting semaphore

    // Initialize a counting semaphore with the specified initial value
    if (sem_init(&count_sem, 0, initial_value) != 0) {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }

    printf("Counting semaphore created and initialized with value %u.\n", initial_value);

    // Destroy the semaphore when done
    sem_destroy(&count_sem);

    return 0;
}

/*

Command: ./31b

Output:
Counting semaphore created and initialized with value 5.

*/
