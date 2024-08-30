/* Name: 25.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
  
 * Problem Statement: Write a program to create three child processes. The parent should wait for a 
                      particular child (use waitpid system call).
 * Date: 28 Aug 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int child_1, child_2, child_3;
    int particular_child;
    int status;

    child_1 = fork();
    if (child_1 == 0) 
    {
        printf("First child process (PID: %d) is running.\n", getpid());
        printf("Puttting child 1 to sleep for 2 seconds \n");
        sleep(2);  // Simulate some work
        exit(0);   // Exit with status 0
    }
    
    else if (child_1 < 0) {
        perror("Fork failed for the first child");
        return 1;
    }

    child_2 = fork();
    if (child_2 == 0) {
        printf("Second child process (PID: %d) is running.\n", getpid());
        printf("Putting child 2 to sleep for 4 seconds \n");
        sleep(4);  // Simulate some work
        exit(0);   // Exit with status 0
    } 
    
    else if (child_2 < 0) {
        perror("Fork failed for the second child");
        return 1;
    }

    child_3 = fork();
    if (child_3 == 0) {
        printf("Third child process (PID: %d) is running.\n", getpid());
        printf("Putting child 3 to sleep for 6 seconds \n");
        sleep(6);  // Simulate some work
        exit(0);   // Exit with status 0
    } 
    
    else if (child_3 < 0) {
        perror("Fork failed for the third child");
        return 1;
    }

    // Parent process: Wait for the second child process specifically
    particular_child = child_2;
    printf("Parent process (PID: %d) is waiting for the second child (PID: %d) to finish.\n", getpid(), particular_child);
    waitpid(particular_child, &status, 0);

    if (WIFEXITED(status)) {
        printf("Second child process (PID: %d) terminated with exit status: %d\n", particular_child, WEXITSTATUS(status));
    } 
    
    else {
        printf("Second child process (PID: %d) did not terminate normally.\n", particular_child);
    }

    // Wait for the remaining children to terminate
    waitpid(child_1, NULL, 0);
    waitpid(child_3, NULL, 0);

    return 0;
}

/*

Command: ./25

Output:

First child process (PID: 17021) is running.
Puttting child 1 to sleep for 2 seconds 
Parent process (PID: 17020) is waiting for the second child (PID: 17022) to finish.
Second child process (PID: 17022) is running.
Putting child 2 to sleep for 4 seconds 
Third child process (PID: 17023) is running.
Putting child 3 to sleep for 6 seconds 
Second child process (PID: 17022) terminated with exit status: 0

*/
