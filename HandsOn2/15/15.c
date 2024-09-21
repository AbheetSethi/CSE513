/* Name: 15.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a simple program to send some data from parent to the child process
 * Date: 12 Sept 2024
*/

#include <stdio.h>    
#include <unistd.h>   // Import for `fork`, `pipe`, `read`, `write`
#include <string.h>   
#include <sys/wait.h> // Import for 'wait()'
 
int main() {
    int pipefd[2];            // Array to hold the pipe file descriptors (pipefd[0] for reading, pipefd[1] for writing)
    pid_t pid;                // Process ID for `fork()`
    char writeMessage[] = "Hello from the parent - MT2024004";
    char readBuffer[100];      // Buffer to hold the data read by the child

    // Create a pipe
    if (pipe(pipefd) == -1) 
    {
        perror("Pipe creation failed");
        return 1;
    }

    // Create a child process
    pid = fork();

    if (pid == -1) 
    {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) 
    {
        // Child process: read from the pipe
        close(pipefd[1]);  // Close the write end of the pipe since child only reads

        // Read the message from the pipe
        read(pipefd[0], readBuffer, sizeof(readBuffer));

        // Display the received message
        printf("Child received: %s\n", readBuffer);

        // Close the read end
        close(pipefd[0]);
    } 
    
    else 
    {
        // Parent process: write to the pipe
        close(pipefd[0]);  // Close the read end of the pipe since parent only writes

        // Write the message to the pipe
        write(pipefd[1], writeMessage, strlen(writeMessage) + 1);  // +1 to include the null terminator

        // Close the write end
        close(pipefd[1]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}

/*

Command: ./15

Output:
Child received: Hello from the parent - MT2024004

*/
