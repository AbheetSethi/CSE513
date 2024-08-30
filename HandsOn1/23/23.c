/* Name: 23.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
  
 * Problem Statement: Write a program to create a Zombie state of the running program
 
 * Date: 28 Aug 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    
    int pid = fork();

    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    } 
    
    else if (pid == 0) 
    {
        printf("Child process (PID: %d) is exiting.\n", getpid());
        exit(0);  // Child exits immediately
    } 
    
    else 
    {
        printf("Parent process (PID: %d) is sleeping.\n", getpid());
        sleep(10);  // Parent sleeps, leaving child in Zombie state

        // Now parent process calls wait to reap the child process
        int status;
        wait(&status);
        printf("Parent process reaped child (PID: %d).\n", pid);
    }

    return 0;
}

/*

Command: ./23

Output:
Parent process (PID: 11489) is sleeping.
Child process (PID: 11490) is exiting.
Parent process reaped child (PID: 11490).

*/
