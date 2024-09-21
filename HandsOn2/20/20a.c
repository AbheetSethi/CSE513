/* Name: 20a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write two programs so that both can communicate by FIFO - Use one way commmunication.
 * Date: 12 Sept 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "20"

int main() {
    int fd;
    const char *message = "Hello from the writer! - MT2024004\n";

    // Create the FIFO (named pipe)
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo failed");
        return 1;
    }

    // Open the FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open FIFO for writing");
        return 1;
    }

    // Write the message to the FIFO
    write(fd, message, strlen(message));
    printf("Message sent: %s", message);

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