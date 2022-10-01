#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 4096
#define MAXCHAR 1000

int main(int argc, char **argv) {
    int    sockfd, n;
    char   recvline[MAXLINE + 1];
    char   error[MAXLINE + 1];
    unsigned int port;
    char   ip[16];
    char   msg[MAXCHAR];
    struct sockaddr_in servaddr;
    socklen_t nAddrLen;
    nAddrLen = sizeof(struct sockaddr_in);
    

    if (argc != 3) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," <IPaddress>");
        strcat(error," <Port>");
        perror(error);
        exit(1);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(2022);
    
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // Get Ip and Port of Server
    getsockname(sockfd, (struct sockaddr *) &servaddr, &nAddrLen);
    inet_ntop(AF_INET, &servaddr.sin_addr, ip, sizeof(ip));
    port = ntohs(servaddr.sin_port);
    printf("IP: %s\n", ip);
    printf("Porta local: %u\n", port);

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            perror("fputs error");
            exit(1);
        }

        // Read msg
        printf("Enter with message: ");
        //scanf("%s", msg);
        fgets(msg, MAXCHAR, stdin);

        // Send msg
        if(send(sockfd, msg, strlen(msg), 0) < 0) {
            perror("send error");
            exit(1);
        }
    }

    if (n < 0) {
        perror("read error");
        exit(1);
    }

    exit(0);
}
