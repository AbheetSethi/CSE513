/* Name: 17b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to execute ls -l | wc.
                         b. use dup2
 * Date: 12 Sept 2024
*/

#include <stdio.h>    
#include <unistd.h>   // Import for `fork`, `pipe`, `dup2`, `execvp`
#include <sys/wait.h> // Import for `wait`

int main() 
{
    int pipefd[2];  // Pipe file descriptors
    pid_t pid;

    // Create a pipe
    if (pipe(pipefd) == -1) 
    {
        perror("Pipe creation failed");
        return 1;
    }

    // Fork the process
    pid = fork();

    if (pid == -1) 
    {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) 
    {
        // Child process: Executes `ls -l`
        close(pipefd[0]);  // Close the read end of the pipe

        // Redirect standard output (fd 1) to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);  // STDOUT_FILENO is 1

        close(pipefd[1]);  // Close the original write end of the pipe

        // Execute `ls -l`
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("Exec ls failed");  // If execlp fails
    } 
    
    else 
    {
        // Parent process: Executes `wc`
        close(pipefd[1]);  // Close the write end of the pipe

        // Redirect standard input (fd 0) to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);  // STDIN_FILENO is 0

        close(pipefd[0]);  // Close the original read end of the pipe

        // Execute `wc`
        execlp("wc", "wc", (char *)NULL);
        perror("Exec wc failed");  // If execlp fails

        // Wait for the child to finish
        wait(NULL);
    }

    return 0;
}

/*

Command: ./17b
      
Output:
7      56     315

*/