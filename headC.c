#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    int buffer[BUFFER_SIZE / sizeof(int)];  // Buffer to send integer array

    // Creating socket file descriptor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get input from the user
    printf("Enter the list of numbers (space-separated): ");
    for (int i = 0; i < BUFFER_SIZE / sizeof(int); i++) {
        scanf("%d", &buffer[i]);
    }

    // Send the input to the server
    send(client_socket, buffer, sizeof(buffer), 0);

    // Receive the result from the server
    if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    int headCount = sizeof(buffer) / sizeof(int);

    // Print the result
    printf("Received HEAD NUMBERS from server: ");
    for (int i = 0; i < headCount; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // Closing the socket
    close(client_socket);

    return 0;
}
