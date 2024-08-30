/* Name: 27e.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004
 
 * Problem Statement: Write a program to execute ls -Rl by the following system calls
  		      
  		      e. execvp
*/

#include <unistd.h>    // Import for exec functions
#include <stdlib.h>    // Import for exit
#include <stdio.h>
#include <sys/wait.h>  // For waitpid

int main() 
{
    int pid;
    int status;

    pid = fork();
    if (pid == 0) 
    {
        printf("Executing `ls -Rl` using execvp\n");
        char *args[] = {"ls", "-Rl", "./27_e_sample", NULL};
        execvp("ls", args);

        // If execvp returns, it means there was an error
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) 
    {
        // Parent process waits for the child to complete
        waitpid(pid, &status, 0);
    }

    return 0;
}

/*

Command: ./27_e

Output:

Executing `ls -Rl` using execvp
-rwxrwxr-x 1 abheet abheet 15968 Aug 30 09:44 ./27_e_sample

*/
