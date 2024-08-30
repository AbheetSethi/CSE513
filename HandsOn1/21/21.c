/* Name: 21.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program, call fork and print the parent and child process id

 * Date: 22 Aug 2024
*/

#include <stdio.h>
#include <unistd.h> // For fork(), getpid(), getppid()

int main() {
    int pid = fork(); 

    if (pid < 0) 
    {
        printf("Fork failed!\n");
        return 1;
    } 
    
    else if (pid == 0) 
    {
        printf("Child process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());
    } 
    
    else 
    {
        printf("Parent process:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
    }

    return 0;
}

/*

Command: ./21

Output:

Parent process:
Parent PID: 8145
Child PID: 8146
Child process:
Child PID: 8146
Parent PID: 8145

*/
