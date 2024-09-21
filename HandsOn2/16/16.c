/* Name: 16.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to send and receive data from parent to child vice versa. Use two way communication.
 * Date: 12 Sept 2024
*/

#include <stdio.h>    
#include <unistd.h>   // Import for `fork`, `pipe`, `read`, `write`
#include <string.h>   
#include <sys/wait.h> // Import for `wait`

int main() 
{
    int pipe1[2], pipe2[2];  // Two pipes: pipe1 for parent to child, pipe2 for child to parent
    pid_t pid;
    char parentMessage[] = "Hello from parent - MT2024004";
    char childMessage[] = "Hello from child - MT2024004";
    char readBuffer[100];

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) 
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
        // Child process

        // Close unused pipe ends
        close(pipe1[1]);  // Close the write end of pipe1 (child reads from pipe1)
        close(pipe2[0]);  // Close the read end of pipe2 (child writes to pipe2)

        // Read message from parent
        read(pipe1[0], readBuffer, sizeof(readBuffer));
        printf("Child received: %s\n", readBuffer);

        // Send message back to parent
        write(pipe2[1], childMessage, strlen(childMessage) + 1);  // +1 to include null terminator

        // Close the pipe ends used by child
        close(pipe1[0]);
        close(pipe2[1]);
    } 
    
    else 
    {
        // Parent process

        // Close unused pipe ends
        close(pipe1[0]);  // Close the read end of pipe1 (parent writes to pipe1)
        close(pipe2[1]);  // Close the write end of pipe2 (parent reads from pipe2)

        // Send message to child
        write(pipe1[1], parentMessage, strlen(parentMessage) + 1);  // +1 to include null terminator

        // Wait for the child to send a message back
        read(pipe2[0], readBuffer, sizeof(readBuffer));
        printf("Parent received: %s\n", readBuffer);

        // Close the pipe ends used by parent
        close(pipe1[1]);
        close(pipe2[0]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}

/*

Command: ./16

Output:
Child received: Hello from parent - MT2024004
Parent received: Hello from child - MT2024004

*/
