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
    unsigned char opCode;
    short op1, op2;

    printf("-----------------------------\n");
    printf("| 0 | 1 | 2 | 3 | 4 | 5 | 6 |\n");
    printf("-----------------------------\n");
    printf("|Add|Sub| Or|And|Shr|Shl|Not|\n");
    printf("-----------------------------\n\n");

    printf("Enter the Operand Number:\t");
    scanf("%hhd", &opCode);

    printf("Enter the first operand:\t");
    scanf("%hd", &op1);

    if (opCode != 6) {
        printf("Enter the second operand:\t");
        scanf("%hd", &op2);
    }

    unsigned char tml = (unsigned char) (opCode == 6 ? 6 : 8);
    unsigned char id = (unsigned char) (rand() % 128);
    unsigned char numOps = (unsigned char) (opCode == 6 ? 1 : 2);

    //send
    char toSend[tml];
    toSend[0] = tml;
    toSend[1] = id;
    toSend[2] = opCode;
    toSend[3] = numOps;

    toSend[4] = (op1 >> 8) & 0xff;
    toSend[5] = op1 & 0xff;

    if (tml == 8) {
        toSend[6] = (op2 >> 8) & 0xff;
        toSend[7] = op2 & 0xff;
    }

    printf("Message in hexadecimal:\n");
    for (unsigned i = 0; i < tml; i++) {
        printf("0x%02X ", toSend[i]);
    }
    printf("\n");

    clock_t start = clock();
    write(socketID, toSend, sizeof(toSend));
    //receive

    clock_t end = clock();

    double time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time Taken: %f", time);
}

