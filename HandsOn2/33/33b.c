/* Name: 33b.c
 * Author: Abheet Sethi
 * Registration No.: MT2024004

 * Problem Statement: Write a program to communicate between two machines using socket.
 * Date: 19 Sept 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *message = "Hello from client - MT2024004";

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {  // Change "127.0.0.1" to the server's IP address
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Send a message to the server
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");

    // Read the server's response
    read(sock, buffer, BUFFER_SIZE);
    printf("Message from server: %s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}

/*

Terminal 1:
Command: ./33a

Output:
Server is listening on port 8080...

Terminal 2:
Command: ./33b

Output:
./33b
Message sent to server
Message from server: Hello from server - MT2024004

Terminal 1:
Output:
Message from client: Hello from client - MT2024004
Hello message sent to client

*/
