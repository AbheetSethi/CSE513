/* Name: 17a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to execute ls -l | wc.
                         a. use dup
 * Date: 12 Sept 2024
*/

#include <stdio.h>    // Import for `printf`, `perror`
#include <unistd.h>   // Import for `fork`, `pipe`, `dup`, `execvp`
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

        // Redirect standard output to the write end of the pipe
        dup(pipefd[1]);  // `dup` copies pipefd[1] to the lowest available file descriptor, which is 1 (stdout)

        close(pipefd[1]);  // Close the original write end of the pipe

        // Execute `ls -l`
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("Exec ls failed");  // If execlp fails
    } 
    
    else 
    {
        // Parent process: Executes `wc`
        close(pipefd[1]);  // Close the write end of the pipe

        // Redirect standard input to the read end of the pipe
        dup(pipefd[0]);  // `dup` copies pipefd[0] to the lowest available file descriptor, which is 0 (stdin)

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

Command: ./17a

Output:
total 60
-rwxrwxr-x 1 abheet abheet 16256 Sep  9 23:14 17a
-rw-rw-r-- 1 abheet abheet  1775 Sep  9 23:11 17a.c
-rwxrwxr-x 1 abheet abheet 16264 Sep  9 23:14 17b
-rw-rw-r-- 1 abheet abheet  1658 Sep  9 23:12 17b.c
-rwxrwxr-x 1 abheet abheet 16264 Sep  9 23:14 17c
-rw-rw-r-- 1 abheet abheet  1874 Sep  9 23:16 17c.c

*/
