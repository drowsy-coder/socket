#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    char buffer[BUFFER_SIZE];
    time_t t;
    struct tm *timestamp;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections
    if (listen(server_fd, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accepting connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // t = time(NULL);
    // timestamp = localtime(&t);

    // Send "Hi" message with timestamp to the client
    
    // snprintf(buffer, BUFFER_SIZE, "Hi from server! Timestamp: %s", asctime(timestamp));
    // send(new_socket, buffer, strlen(buffer), 0);

    // printf("Message sent to the client: %s", buffer);


    while (1) {
        // Receive data from the client
        if (recv(new_socket, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("Connection closed by the client");
            break;
        }

        // Print the received message
        printf("Client: %s", buffer);

        // Get current time
        t = time(NULL);
        timestamp = localtime(&t);

        // Send a reply to the client with a timestamp
        printf("Enter your message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // Closing the sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
