/* Name: 17c.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to execute ls -l | wc.
                         c. use fcntl
 * Date: 12 Sept 2024
*/

#include <stdio.h>    
#include <unistd.h>   // Import for `fork`, `pipe`, `execvp`
#include <fcntl.h>    // Import for `fcntl`
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

        // Use fcntl to duplicate the file descriptor to stdout (fd 1)
        if (fcntl(pipefd[1], F_DUPFD, STDOUT_FILENO) == -1) 
        {
            perror("fcntl failed");
            return 1;
        }

        close(pipefd[1]);  // Close the original write end of the pipe

        // Execute `ls -l`
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("Exec ls failed");  // If execlp fails
    } 
    
    else 
    {
        // Parent process: Executes `wc`
        close(pipefd[1]);  // Close the write end of the pipe

        // Use fcntl to duplicate the file descriptor to stdin (fd 0)
        if (fcntl(pipefd[0], F_DUPFD, STDIN_FILENO) == -1) 
        {
            perror("fcntl failed");
            return 1;
        }

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

Command: ./17c

Output:
total 60
-rwxrwxr-x 1 abheet abheet 16256 Sep  9 23:14 17a
-rw-rw-r-- 1 abheet abheet  2122 Sep  9 23:20 17a.c
-rwxrwxr-x 1 abheet abheet 16264 Sep  9 23:14 17b
-rw-rw-r-- 1 abheet abheet  1658 Sep  9 23:12 17b.c
-rwxrwxr-x 1 abheet abheet 16264 Sep  9 23:14 17c
-rw-rw-r-- 1 abheet abheet  1874 Sep  9 23:16 17c.c

*/