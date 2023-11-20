#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to check if the given square is a magic square
int isMagicSquare(int** square, int n) {
    int sum = 0;

    // Calculate the sum of the first row and use it as the reference sum
    for (int j = 0; j < n; j++) {
        sum += square[0][j];
    }

    // Check the sum of rows
    for (int i = 1; i < n; i++) {
        int rowSum = 0;
        for (int j = 0; j < n; j++) {
            rowSum += square[i][j];
        }
        if (rowSum != sum) {
            return 0; // Not a magic square
        }
    }

    // Check the sum of columns
    for (int j = 0; j < n; j++) {
        int colSum = 0;
        for (int i = 0; i < n; i++) {
            colSum += square[i][j];
        }
        if (colSum != sum) {
            return 0; // Not a magic square
        }
    }

    // Check the sum of the main diagonal
    int diagSum1 = 0;
    for (int i = 0; i < n; i++) {
        diagSum1 += square[i][i];
    }
    if (diagSum1 != sum) {
        return 0; // Not a magic square
    }

    // Check the sum of the secondary diagonal
    int diagSum2 = 0;
    for (int i = 0; i < n; i++) {
        diagSum2 += square[i][n - i - 1];
    }
    if (diagSum2 != sum) {
        return 0; // Not a magic square
    }

    return 1; // It is a magic square
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    int buffer[BUFFER_SIZE / sizeof(int)]; // Buffer to receive integer array
    int** square;
    int n;

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

    // Receive data from the client (square dimensions and elements)
    if (recv(new_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    // Extract dimensions and create the square matrix
    n = buffer[0];
    square = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        square[i] = (int *)malloc(n * sizeof(int));
    }

    int k = 2; // Start index of elements in the buffer
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            square[i][j] = buffer[k++];
        }
    }

    // Check if it is a magic square
    int result = isMagicSquare(square, n);

    // Send the result back to the client
    if (result) {
        // send(new_socket, "It is a magic square", strlen("It is a magic square"), 0);
        printf("It's a magic square");
    } else {
        // send(new_socket, "It is not a magic square", strlen("It is not a magic square"), 0);
        printf("No Magic");
    }

    // Free allocated memory and close the sockets
    for (int i = 0; i < n; i++) {
        free(square[i]);
    }
    free(square);
    close(new_socket);
    close(server_socket);

    return 0;
}
