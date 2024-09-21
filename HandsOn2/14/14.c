/* Name: 14.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor
 * Date: 12 Sept 2024
*/

#include <stdio.h>  
#include <unistd.h>  // Import for `pipe`, `write`, `read`
#include <string.h>  

int main() {
    int pipefd[2];    // Array to hold the pipe file descriptors (pipefd[0] for reading, pipefd[1] for writing)
    char writeMessage[] = "Hello from the pipe - MT2024004";
    char readBuffer[100];
    int bytesRead;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Write to the pipe
    printf("Writing to the pipe: %s\n", writeMessage);
    write(pipefd[1], writeMessage, strlen(writeMessage) + 1);  // +1 to include null terminator

    // Read from the pipe
    bytesRead = read(pipefd[0], readBuffer, sizeof(readBuffer));

    // Display the read message on the monitor
    printf("Read from the pipe: %s\n", readBuffer);

    return 0;
}

/*

Command: ./14

Output:
Writing to the pipe: Hello from the pipe - MT2024004
Read from the pipe: Hello from the pipe - MT2024004

*/