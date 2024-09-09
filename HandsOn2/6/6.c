/* Name: 6.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a simple program to create three threads.
 * Date: 5 Sept 2024
*/

#include <pthread.h> // Import for `pthread_create`, `pthread_self`
#include <stdio.h>   // Import for `perror` & `printf`
#include<unistd.h> // Import `sleep`

void *sampleFunction(void *data)
{
    printf("Running in thread: %lu\n", pthread_self());
}

void main()
{
    pthread_t threadOne, threadTwo, threadThree;

    // Create three threads
    if (pthread_create(&threadOne, NULL, sampleFunction, NULL))
        perror("Error while creating thread one");
    if (pthread_create(&threadTwo, NULL, sampleFunction, NULL))
        perror("Error while creating thread two");
    if (pthread_create(&threadThree, NULL, sampleFunction, NULL))
        perror("Error while creating thread three");


    // Wait for the threads to terminate and then terminate the main process
    pthread_exit(NULL);
}

/*

Command: ./6

Output:

Running in thread: 139957456860864
Running in thread: 139957448468160
Running in thread: 139957440075456

*/