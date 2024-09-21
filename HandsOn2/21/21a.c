/* Name: 21a.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write two programs so that both can communicate by FIFO - Use two way commmunication.
 * Date: 12 Sept 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO1 "21_a"
#define FIFO2 "21_b"
#define BUFFER_SIZE 100

int main() {
    int writeFd, readFd;
    char sendMessage[BUFFER_SIZE] = "Hello from Program A - MT2024004!";
    char receiveMessage[BUFFER_SIZE];

    // Open FIFO1 for writing
    writeFd = open(FIFO1, O_WRONLY);
    if (writeFd == -1) {
        perror("Failed to open FIFO1 for writing");
        return 1;
    }

    // Write message to FIFO1
    write(writeFd, sendMessage, strlen(sendMessage));
    printf("Program A: Sent message: %s\n", sendMessage);
    close(writeFd);

    // Open FIFO2 for reading
    readFd = open(FIFO2, O_RDONLY);
    if (readFd == -1) {
        perror("Failed to open FIFO2 for reading");
        return 1;
    }

    // Read message from FIFO2
    ssize_t bytesRead = read(readFd, receiveMessage, sizeof(receiveMessage) - 1);
    if (bytesRead > 0) {
        receiveMessage[bytesRead] = '\0'; // Null-terminate the received string
        printf("Program A: Received message: %s\n", receiveMessage);
    }
    close(readFd);

    return 0;
}

/*

Terminal 1:

Command: ./21a

Output:
Program A: Sent message: Hello from Program A - MT2024004!
Program A: Received message: Hello from Program B - MT2024004

Terminal 2:

Command: ./21b

Output:
Program B: Received message: Hello from Program A - MT2024004!
Program B: Sent message: Hello from Program B - MT2024004

*/
