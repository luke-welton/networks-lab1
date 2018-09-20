#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>

int query(int socketID);

int main (int argc, char *argv[]) {
    srand(time(0));

    if (argc < 2) {
        printf("Incorrect number of arguments passed. Shutting down.\n");
        exit(0);
    }

    struct addrinfo sockFD, *serverInfo;
    memset(&sockFD, 0, sizeof sockFD);
    sockFD.ai_family = AF_INET;
    sockFD.ai_socktype = SOCK_STREAM;

    int n = getaddrinfo(argv[1], argv[2], &sockFD, &serverInfo);
    if (n < 0) {
        fprintf(stderr, "Error with getaddrinfo: %s\n", gai_strerror(n));
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
        printf("An error occurred while connecting to the server. Shutting Down.\n");
        exit(0);
    }

    freeaddrinfo(serverInfo);

    char userInput [50];
    do {
        query(socketID);

        printf("\nWould you like to send another query (Y/N)?\t");
        scanf("%c", userInput);
    } while (userInput[0] == 'Y' || userInput[0] == 'y');

    n = close(socketID);
    if (n < 0) {
        printf("An error occurred while closing the connection with the server. Shutting down.");
        exit(0);
    }

    exit(1);
}

int query(int socketID) {
    unsigned char opCode;
    short op1, op2;

    printf("\n\n");
    printf("-----------------------------\n");
    printf("| 0 | 1 | 2 | 3 | 4 | 5 | 6 |\n");
    printf("-----------------------------\n");
    printf("|Add|Sub| Or|And|Shr|Shl|Not|\n");
    printf("-----------------------------\n");
    printf("\n\n");

    printf("Enter the Operand Number:\t");
    scanf("%hhd", &opCode);

    printf("Enter the first operand:\t");
    scanf("%hd", &op1);

    if (opCode != 6) {
        printf("Enter the second operand:\t");
        scanf("%hd", &op2);
    }

    printf("\n\n");

    unsigned char tml = (unsigned char) (opCode == 6 ? 6 : 8);
    unsigned char id = (unsigned char) (rand() % 128);
    unsigned char numOps = (unsigned char) (opCode == 6 ? 1 : 2);

    //send
    char toSend[tml];
    toSend[0] = tml;
    toSend[1] = id;
    toSend[2] = opCode;
    toSend[3] = numOps;

    toSend[4] = (char) ((op1 >> 8) & 0xff);
    toSend[5] = (char) (op1 & 0xff);

    if (tml == 8) {
        toSend[6] = (char) ((op2 >> 8) & 0xff);
        toSend[7] = (char) (op2 & 0xff);
    }

    printf("Message in hexadecimal:\n");
    for (unsigned i = 0; i < tml; i++) {
        printf("0x%02X ", toSend[i]);
    }
    printf("\n\n");

    clock_t start = clock();
    ssize_t n = write(socketID, toSend, sizeof(toSend));
    if (n < 0) {
        printf("An error occurred while writing to the server. Shutting down.\n");
        exit(0);
    }

    int responseTML = 0;
    n = read(socketID, (void*) (&responseTML), 1);
    clock_t end = clock();
    if (n < 0) {
        printf("An error occurred while reading the server's initial response. Shutting down.\n");
        exit(0);
    }

    int response[responseTML];
    response[0] = responseTML;

    for (unsigned i = 1; i < responseTML; i++) {
        int byteIn = 0;
        n = read(socketID, (void*)(&byteIn), 1);
        if (n < 0) {
            printf("An error occurred while reading the server's following response (#%d). Shutting down.\n", i);
            exit(0);
        }

        response[i] = byteIn;
    }

    printf("Response in hexadecimal:\n");
    for (unsigned i = 0; i < responseTML; i++) {
        printf("0x%02X ", response[i]);
    }
    printf("\n\n");

    int answer = (response[3] << (8 * 3)) +
                 (response[4] << (8 * 2)) +
                 (response[5] << (8 * 1)) +
                 (response[6] << (8 * 0));
    int responseID = response[1];

    printf("Request #%d\n", responseID);
    printf("Answer: %d\n", answer);

    double time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time Taken: %f\n", time);
}

