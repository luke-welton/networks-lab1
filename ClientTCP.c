#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect number of arguments passed. Shutting down.");
        exit(0);
    }


    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
        printf("An error occurred while creating the client socket. Shutting down.");
        exit(0);
    }


}

