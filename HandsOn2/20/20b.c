/* Name: 20b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write two programs so that both can communicate by FIFO - Use one way commmunication.
 * Date: 12 Sept 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_NAME "20"
#define BUFFER_SIZE 100

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // Retry loop to open the FIFO
    while ((fd = open(FIFO_NAME, O_RDONLY)) == -1) {
        perror("Failed to open FIFO for reading");
        sleep(1);  // Wait for 1 second before retrying
    }

    // Read the message from the FIFO
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Failed to read from FIFO");
        close(fd);
        return 1;
    }

    // Null-terminate the string and print it
    buffer[bytesRead] = '\0';
    printf("Message received: %s\n", buffer);

    // Close the FIFO
    close(fd);

    return 0;
}

/*

Terminal 1:                                             Terminal 2:
Command: ./20a                                          

Output:                                                 Command: ./20b
* BLANK                                                 Output:
Message sent: Hello from the writer! - MT2024004        Message received: Hello from the writer! - MT2024004

*/