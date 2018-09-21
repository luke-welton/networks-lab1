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
#define INVALIDREQUEST 127
#define VALIDREQUEST 0

typedef struct ReceivedMessageBody {
    unsigned tml;
    unsigned id;
    unsigned opCode;
    unsigned numOps;
    int op1;
    int op2;
}ReceivedMessageBody;


void displayBuffer(char *Buffer, int length);
ReceivedMessageBody bytesToInts(char *Buffer);
char * intsToBytes(int tml, int requestID, int errorCode, int result);
int addition(int op1, int op2);
int subtraction(int op1, int op2);
int bitwiseOr(int op1, int op2);
int bitwiseAnd(int op1, int op2);
int shiftRight(int op1, int op2);
int shiftLeft(int op1, int op2);
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
            perror("ServerUDP: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("ServerUDP: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "ServerUDP: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);


    while (1){
        printf("\n >>>> ServerUDP: waiting for a datagram...\n");

        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }

        printf("ServerUDP: got packet from %s\n",
               inet_ntop(their_addr.ss_family,
                         get_in_addr((struct sockaddr *)&their_addr),
                         s, sizeof s));
        printf("ServerUDP: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("ServerUDP: packet contains \"%s\"\n", buf);
        displayBuffer(buf,numbytes);

        ReceivedMessageBody rcvdMsgObj = bytesToInts(buf);
        int responseTML = 7;
        int responseRequestID = rcvdMsgObj.id;
        int responseErrCode = INVALIDREQUEST;
        int responseResult = 0;

        switch(rcvdMsgObj.opCode) {
            case 0:
                responseResult = addition(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 1:
                responseResult = subtraction(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 2:
                responseResult = bitwiseOr(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 3:
                responseResult = bitwiseAnd(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 4:
                responseResult = shiftRight(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 5:
                responseResult = shiftLeft(rcvdMsgObj.op1, rcvdMsgObj.op2);
                responseErrCode = VALIDREQUEST;
                break;
            case 6:
                responseResult = notFunction(rcvdMsgObj.op1);
                responseErrCode = VALIDREQUEST;
                break;
            default:
                responseErrCode = INVALIDREQUEST;
        }

        char *sendBuffer = intsToBytes(responseTML, responseRequestID, responseErrCode, responseResult);
        if ((numbytes = sendto(sockfd, sendBuffer, 7, 0,
                               p->ai_addr, p->ai_addrlen)) == -1) {
            perror("ServerUDP: sendto");
            exit(1);
        }
        memset(&buf[0], 0, sizeof(buf));
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

// convert byte array buffer into integers and store the message in a struct.
ReceivedMessageBody bytesToInts(char *Buffer) {
    ReceivedMessageBody temp_body;
    temp_body.tml = 0;
    temp_body.tml = Buffer[0] << 8;
    temp_body.id = 0;
    temp_body.id = Buffer[1] << 8;
    temp_body.opCode = 0;
    temp_body.opCode = Buffer[2] << 8;
    temp_body.numOps = 0;
    temp_body.numOps = Buffer[3] << 8;
    temp_body.op1 = 0;
    temp_body.op1 = ((Buffer[4] << 16) | (Buffer[5] << 8));
    temp_body.op2 = 0;
    if(temp_body.numOps == 2) {
        temp_body.op2 = ((Buffer[6] << 16) | (Buffer[7] << 8));
    }
    return temp_body;
}

char * intsToBytes(int tml, int requestID, int errorCode, int result) {
    static char sendingBuf[7];
    sendingBuf[0] = tml;
    sendingBuf[1] = requestID;
    sendingBuf[2] = errorCode;
    sendingBuf[3] = result;
    sendingBuf[4] = result >> 8;
    sendingBuf[5] = result >> 16;
    sendingBuf[6] = result >> 24;
    return sendingBuf;
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
int shiftRight(int op1, int op2) {
    return op1 >> op2;
}
int shiftLeft(int op1, int op2) {
    return op1 << op2;
}
int notFunction(int op1){
    return ~op1;
}