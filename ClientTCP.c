#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect number of arguments passed. Shutting down.\n");
        exit(0);
    }


    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
        printf("An error occurred while creating the client socket. Shutting down.\n");
        exit(0);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, '0', sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, gethostbyname(argv[1]), &serverAddress.sin_addr) <= 0) {
        printf("The given server address is not supported. Shutting down.\n");
        exit(0);
    }

    if (connect(sockFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        printf("An error occurred while connecting to the server. Shutting down.\n");
        exit(0);
    }
}

