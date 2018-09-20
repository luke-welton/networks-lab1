/*
** Original  listener.c by Beej -- a datagram sockets "server" demo
** Modified by Saad Biaz 08/29/2014
** Modified by Jesse Roach 09/19/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXBUFLEN 100

typedef struct ReceivedMessageBody {
    int tml;
    int id;
    int opCode;
    int numOps;
    int op1;
    int op2;
}ReceivedMessageBody;

void displayBuffer(char *Buffer, int length);
MessageBody bytesToInts(char *Buffer, int length);
int addition(int op1, int op2);
int subtraction(int op1, int op2);
int bitwiseOr(int op1, int op2);
int bitwiseAnd(int op1, int op2);
int shiftLeft(int op1, int op2);
int shiftRight(int op1, int op2);
int notFunction(int op1);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main (int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    //check for command line args with port number
    if (argc != 2) {
        fprintf(stderr,"usage: ServerUDP Port# \n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1] , &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);


    while (1){
        printf("\n >>>> listener: waiting for a datagram...\n");

        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }

        printf("listener: got packet from %s\n",
               inet_ntop(their_addr.ss_family,
                         get_in_addr((struct sockaddr *)&their_addr),
                         s, sizeof s));
        printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buf);
        displayBuffer(buf,numbytes);
        //add code for calcs here
        switch() {
            case //add:
            addition();
            break;
        }
    }
}


//display content of buffer in hexadecimal
void displayBuffer(char *Buffer, int length){
    int currentByte, column;

    currentByte = 0;
    printf("\n>>>>>>>>>>>> Content in hexadecimal <<<<<<<<<<<\n");
    while (currentByte < length){
        printf("%3d: ", currentByte);
        column =0;
        while ((currentByte < length) && (column < 10)){
            printf("%2x ",Buffer[currentByte]);
            column++;
            currentByte++;
        }
        printf("\n");
    }
    printf("\n\n");
}

ReceivedMessageBody bytesToInts(char *Buffer, int length) {
    int temp_tml = ;
    int temp_id;
    int temp_opCode;
    int temp_numOps;
    int temp_op1;
    int temp_op2;
    if ()
}

int addition(int op1, int op2) {
    return op1 + op2;
}
int subtraction(int op1, int op2) {
    return op1 - op2;
}
int bitwiseOr(int op1, int op2) {
    return op1 | op2;
}
int bitwiseAnd(int op1, int op2) {
    return op1 & op2;
}
int shiftLeft(int op1, int op2) {
    return op1 << op2;
}
int shiftRight(int op1, int op2) {
    return op1 >> op2;
}
int notFunction(int op1){
    return ~op1;
}