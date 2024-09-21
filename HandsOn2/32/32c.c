/* Name: 32c.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to implement semaphore to protect any critical section.
                        c. protect multiple pseudo resources (may be two) using counting semaphore
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem_resources;

void* access_resources(void* arg) {
    sem_wait(&sem_resources);  // Acquire resource

    printf("Thread %ld is accessing a resource.\n", (long)arg);
    sleep(1);  // Simulate resource usage
    printf("Thread %ld has finished using the resource.\n", (long)arg);

    sem_post(&sem_resources);  // Release resource
    return NULL;
}

int main() {
    pthread_t threads[5];

    // Initialize counting semaphore with 2 resources
    sem_init(&sem_resources, 0, 2);

    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, access_resources, (void*)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_resources);  // Destroy semaphore
    return 0;
}

/*

Command: ./32c

Output:
Thread 0 is accessing a resource.
Thread 1 is accessing a resource.
Thread 0 has finished using the resource.
Thread 1 has finished using the resource.
Thread 2 is accessing a resource.
Thread 3 is accessing a resource.
Thread 3 has finished using the resource.
Thread 2 has finished using the resource.
Thread 4 is accessing a resource.
Thread 4 has finished using the resource.

*/
