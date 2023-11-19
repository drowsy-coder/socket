#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    while (1) {
        // Receive data from the client
        if (recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len) == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Print the received message
        printf("Client: %s", buffer);

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));

        // Send a reply to the client
        printf("Enter your message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len);

        // Clear the buffer again for the next iteration
        memset(buffer, 0, sizeof(buffer));
    }


    // Closing the socket
    close(server_socket);

    return 0;
}
