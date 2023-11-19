#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to find and return HEAD NUMBERS
int* findHeadNumbers(int* numbers, int n, int* headCount) {
    int* headNumbers = malloc(n * sizeof(int));  // Allocate memory to store the head numbers
    *headCount = 0;

    for (int i = 1; i < n - 1; i++) {
        if (numbers[i] > numbers[i - 1] && numbers[i] > numbers[i + 1]) {
            headNumbers[*headCount] = numbers[i];
            (*headCount)++;
        }
    }

    return headNumbers;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    int buffer[BUFFER_SIZE / sizeof(int)];  // Buffer to receive integer array
    int headCount;
    int* headNumbers;

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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

    // Listening for connections
    if (listen(server_socket, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP Server listening on port %d...\n", PORT);

    // Accepting connections
    if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client
    if (recv(new_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    int n = sizeof(buffer) / sizeof(int);

    // Find HEAD NUMBERS
    headNumbers = findHeadNumbers(buffer, n, &headCount);

    // Send the result back to the client
    send(new_socket, headNumbers, headCount * sizeof(int), 0);

    printf("HEAD NUMBERS: ");
    for (int i = 0; i < headCount; i++) {
        printf("%d ", headNumbers[i]);
    }
    printf("\n");

    // Free allocated memory and close the sockets
    free(headNumbers);
    close(new_socket);
    close(server_socket);

    return 0;
}
