/* Name: 22a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
 * Date: 12 Sept 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "22"

int main() 
{
    int fd;
    const char *message = "Hello from writer - MT2024004";
    
    // Open the FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) 
    {
        perror("Failed to open FIFO for writing");
        return 1;
    }

    // Write the message to the FIFO
    write(fd, message, strlen(message));
    printf("Message sent: %s\n", message);

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
