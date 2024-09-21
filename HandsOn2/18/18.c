/* Name: 18.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to find out total number of directories on the pwd.
                        execute ls -l | grep ^d | wc ? Use only dup2.
 * Date: 12 Sept 2024
*/

#include <stdio.h>    
#include <unistd.h>   // Import for `fork`, `pipe`, `dup2`, `execlp`
#include <sys/wait.h> // Import for `wait`

int main() 
{
    int pipe1[2];  // Pipe between `ls -l` and `grep ^d`
    int pipe2[2];  // Pipe between `grep ^d` and `wc -l`
    pid_t pid1, pid2, pid3;

    // Create the first pipe
    if (pipe(pipe1) == -1) 
    {
        perror("Pipe1 creation failed");
        return 1;
    }

    // Fork first child process for `ls -l`
    pid1 = fork();

    if (pid1 == -1) 
    {
        perror("Fork for ls failed");
        return 1;
    }

    if (pid1 == 0) 
    {
        // Child 1: Execute `ls -l`

        // Redirect stdout to the write end of pipe1
        dup2(pipe1[1], STDOUT_FILENO);

        // Close unused file descriptors
        close(pipe1[0]);  // Close read end of pipe1
        close(pipe1[1]);  // Close original write end after duplication

        // Execute `ls -l`
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("Exec ls failed");
        return 1;
    }

    // Create the second pipe
    if (pipe(pipe2) == -1) 
    {
        perror("Pipe2 creation failed");
        return 1;
    }

    // Fork second child process for `grep ^d`
    pid2 = fork();

    if (pid2 == -1) 
    {
        perror("Fork for grep failed");
        return 1;
    }

    if (pid2 == 0) 
    {
        // Child 2: Execute `grep ^d`

        // Redirect stdin to the read end of pipe1
        dup2(pipe1[0], STDIN_FILENO);

        // Redirect stdout to the write end of pipe2
        dup2(pipe2[1], STDOUT_FILENO);

        // Close unused file descriptors
        close(pipe1[0]);
        close(pipe1[1]);  // Close both ends of pipe1
        close(pipe2[0]);  // Close read end of pipe2
        close(pipe2[1]);  // Close original write end of pipe2 after duplication

        // Execute `grep ^d`
        execlp("grep", "grep", "^d", (char *)NULL);
        perror("Exec grep failed");
        return 1;
    }

    // Fork third child process for `wc -l`
    pid3 = fork();

    if (pid3 == -1) 
    {
        perror("Fork for wc failed");
        return 1;
    }

    if (pid3 == 0) 
    {
        // Child 3: Execute `wc -l`

        // Redirect stdin to the read end of pipe2
        dup2(pipe2[0], STDIN_FILENO);

        // Close unused file descriptors
        close(pipe1[0]);  // Close both ends of pipe1
        close(pipe1[1]);
        close(pipe2[0]);  // Close both ends of pipe2
        close(pipe2[1]);

        // Execute `wc -l`
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("Exec wc failed");
        return 1;
    }

    // Parent process: Close all pipes
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Wait for all child processes to finish
    wait(NULL);  // Wait for `ls`
    wait(NULL);  // Wait for `grep`
    wait(NULL);  // Wait for `wc`

    return 0;
}

/*

Command: mkdir 18_1 18_2

Command: ./18

Output:
2

*/
