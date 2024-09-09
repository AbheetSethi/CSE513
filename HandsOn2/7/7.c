/* Name: 7.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a simple program to print the created thread ids.
 * Date: 5 Sept 2024
*/

#include <stdio.h>      
#include <stdlib.h>     // For exit() and general utilities
#include <pthread.h>    // For pthread functions
#include <unistd.h>     // For sleep() function

// Thread function that prints the thread ID
void* thread_function() {
    printf("Thread ID: %lu is running\n", pthread_self());  // Print the thread ID
}

int main() {
    pthread_t threads[3];  // Array to store thread identifiers
    int i;

    // Create 3 threads
    for (i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join error");
            exit(EXIT_FAILURE);
        }
    }

    printf("All threads have completed\n");
    return 0;
}

/*

Command: ./7

Output:
Thread ID: 139837369743040 is running
Thread ID: 139837361350336 is running
Thread ID: 139837352957632 is running
All threads have completed

*/
