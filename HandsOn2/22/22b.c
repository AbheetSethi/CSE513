/* Name: 22b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
 * Date: 12 Sept 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>

#define FIFO_NAME "22"
#define BUFFER_SIZE 100

int main() 
{
    int fd;
    fd_set readfds;
    struct timeval timeout;
    char buffer[BUFFER_SIZE];
    
    // Open the FIFO for reading with non-blocking mode
    fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd == -1) 
    {
        perror("Failed to open FIFO for reading");
        return 1;
    }

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // Set the timeout to 10 seconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    // Use select to wait for data to be available
    int retval = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if (retval == -1) 
    {
        perror("select() failed");
        close(fd);
        return 1;
    } 
    
    else if (retval == 0) 
    {
        // Timeout occurred
        printf("No data received within 10 seconds.\n");
    } 
    
    else 
    {
        // Data is available
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        
        // Check if there was no data, which could be due to non-blocking mode
        if (bytesRead == -1 && errno == EAGAIN) 
        {
            printf("No data to read.\n");
        } 
        
        else if (bytesRead > 0) 
        {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            printf("Message received: %s\n", buffer);
        } 
        
        else 
        {
            perror("Failed to read from FIFO");
        }
    }

    // Close the FIFO
    close(fd);

    return 0;
}

/*

Terminal 1:

Command: ./22a

Output:
BLANK

Terminal 2:

Command: ./22b

Output:
Message received: Hello from writer!

Terminal 1:

Output:
Message sent: Hello from writer!

Terminal 2:

Command:
* IF YOU PASS NOTHING FOR 10 SECONDS THEN YOU'LL GET THE FOLLOWING OUTPUT
No data received within 10 seconds.

*/
