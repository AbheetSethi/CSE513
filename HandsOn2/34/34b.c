/* Name: 34b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to create a concurrent server.
                        b. use pthread_create
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *handle_client(void *client_sock) {
    int sock = *(int*)client_sock;
    char buffer[256];
    int n;

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0) {
        perror("Error reading from socket");
        pthread_exit(NULL);
    }

    printf("Client message: %s", buffer);

    n = write(sock, "Message received\n", 18);
    if (n < 0) {
        perror("Error writing to socket");
        pthread_exit(NULL);
    }

    close(sock);
    free(client_sock);  // Free the allocated memory for client socket descriptor
    pthread_exit(NULL);
}

int main() {
    int sockfd, newsockfd, portno = 5001;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Initialize server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Server listening on port %d...\n", portno);

    // Accept and handle connections in a loop
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }

        // Allocate memory for the client socket descriptor
        int *client_sock = malloc(sizeof(int));
        if (client_sock == NULL) {
            perror("Failed to allocate memory");
            exit(1);
        }
        *client_sock = newsockfd;

        // Create a thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_sock) < 0) {
            perror("Error creating thread");
            free(client_sock);
            exit(1);
        }

        pthread_detach(thread_id);  // Automatically free thread resources on exit
    }

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