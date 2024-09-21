/* Name: 32b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to implement semaphore to protect any critical section.
                        b. protect shared memory from concurrent write access.
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

char shared_memory[100];  // Shared memory
sem_t sem;

void* write_to_memory(void* arg) {
    sem_wait(&sem);  // Lock the semaphore

    // Critical section: write to shared memory
    snprintf(shared_memory, sizeof(shared_memory), "Thread %ld was here!", (long)arg);
    printf("Thread %ld wrote to shared memory: %s\n", (long)arg, shared_memory);

    sem_post(&sem);  // Unlock the semaphore
    return NULL;
}

int main() {
    pthread_t threads[3];

    // Initialize semaphore as binary
    sem_init(&sem, 0, 1);

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, write_to_memory, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);  // Destroy semaphore
    return 0;
}

/*

Command: ./32b

Output:
Thread 0 wrote to shared memory: Thread 0 was here!
Thread 2 wrote to shared memory: Thread 2 was here!
Thread 1 wrote to shared memory: Thread 1 was here!

*/