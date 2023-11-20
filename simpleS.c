#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

int compareChar(const void *a, const void *b);


void checkAnagram(char *str1, char *str2, char *result) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // Check if the lengths are different
    if (len1 != len2) {
        strcpy(result, "not anagram");
        return;
    }

    // Sort the characters in both strings
    qsort(str1, len1, sizeof(char), compareChar);
    qsort(str2, len2, sizeof(char), compareChar);

    // Compare the sorted strings
    if (strcmp(str1, str2) == 0) {
        strcpy(result, "anagram");
    } else {
        strcpy(result, "not anagram");
    }
}

int compareChar(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in));

    listen(serverSocket, 5);

    printf("Server listening on port 8888...\n");

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);

    // Receive data from the client
    char buffer[MAX_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);

    // Process the received data
    char str1[MAX_BUFFER_SIZE], str2[MAX_BUFFER_SIZE], result[MAX_BUFFER_SIZE];
    sscanf(buffer, "%s %s", str1, str2);

    // Check for anagram and calculate total number of characters
    checkAnagram(str1, str2, result);
    int totalChars = strlen(str1) + strlen(str2);

    // Send the result back to the client
    sprintf(buffer, "%s\ntotal number of characters %d\n", result, totalChars);
    send(clientSocket, buffer, strlen(buffer), 0);

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
