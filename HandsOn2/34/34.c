/* Name: 34.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create a concurrent server.
                        a. use fork
                        b. use pthread_create
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    int sockfd, portno = 5001;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Get server by name
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        exit(1);
    }

    // Set up the server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    // Send a message to the server
    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    if (write(sockfd, buffer, strlen(buffer)) < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    // Read the server's response
    bzero(buffer, 256);
    if (read(sockfd, buffer, 255) < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Server reply: %s\n", buffer);

    // Close the socket
    close(sockfd);
    return 0;
}

/*

Terminal 1:
Command: ./34a

Output:
Server listening on port 5001...

Terminal 2:
Command: ./34

Output:
Please enter the message: MT2024004
Server reply: Message received

Terminal 1:
Output:
Client message: MT2024004

*/