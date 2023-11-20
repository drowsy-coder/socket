#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address structure

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address
    serverAddr.sin_port = htons(PORT);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));

        char str1[MAX_BUFFER_SIZE], str2[MAX_BUFFER_SIZE], buffer[MAX_BUFFER_SIZE];
    printf("Enter the first string: ");
    scanf("%s", str1);
    printf("Enter the second string: ");
    scanf("%s", str2);

    // Send data to server
    sprintf(buffer, "%s %s", str1, str2);
    send(clientSocket, buffer, strlen(buffer), 0);

    // Receive and display the result from the server
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Output from Server:\n%s\n", buffer);

    // Close socket
    close(clientSocket);

    return 0;
}

