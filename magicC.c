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
    int buffer[BUFFER_SIZE / sizeof(int)]; // Buffer to send integer array

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
    printf("Enter the number of rows for the square: ");
    scanf("%d", &buffer[0]);
    printf("Enter the number of columns for the square: ");
    scanf("%d", &buffer[1]);

    // Populate the square array
    printf("Enter the elements of the square:\n");
    for (int i = 2; i < buffer[0] * buffer[1] + 2; i++) {
        scanf("%d", &buffer[i]);
    }

    // Send the input to the server
    send(client_socket, buffer, sizeof(buffer), 0);

    // Receive the result from the server
    // char result[BUFFER_SIZE];
    // if (recv(client_socket, result, sizeof(result), 0) == -1) {
    //     perror("Receive failed");
    //     exit(EXIT_FAILURE);
    // }

    // // Print the result
    // printf("Received result from server: %s\n", result);

    // Closing the socket
    close(client_socket);

    return 0;
}
