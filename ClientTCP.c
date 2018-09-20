#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int query(int sockfd);

int main (int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect number of arguments passed. Shutting down.\n");
        exit(0);
    }

    struct addrinfo sockFD, *serverInfo;
    memset(&sockFD, 0, sizeof sockFD);
    sockFD.ai_family = AF_INET;
    sockFD.ai_socktype = SOCK_STREAM;

    int returnValue = getaddrinfo(argv[1], argv[2], &sockFD, &serverInfo);
    if (returnValue != 0) {
        fprintf(stderr, "Error with getaddrinfo: %s\n", gai_strerror(returnValue));
    }

    struct addrinfo *p;
    int socketID = 0;

    for (p = serverInfo; p != NULL; p = p->ai_next) {
        socketID = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketID == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(socketID, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        printf("An error occurred while connecting to the server. Shutting Down.");
        exit(0);
    }

    freeaddrinfo(serverInfo);

    char userInput [50];
    do {
        query(socketID);

        printf("\nWould you like to send another query (Y/N)?\t");
        scanf("%c", userInput);
    } while (userInput[0] == 'Y' || userInput[0] == 'y');
}

int query(int socketID) {

}

