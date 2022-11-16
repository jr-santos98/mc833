#include <sys/select.h>
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
#define MAX(a,b) ((a) > (b) ? (a) : (b))

int maximum(int a, int b, int c) {
    int max;
    if((a > b) && (a > c))
        max = a;
    else if((b > a) && (b > c))
        max = b;
    else if((c > a) && (c > b))
        max = c;
    return max;
}

int main(int argc, char **argv) {
    int    sockfd, sockfd2;
    int    b_read;
    int    maxfdp1;
    char   recvline[MAXLINE + 1];
    char   error[MAXLINE + 1];
    char   ip[16], ip2[16];
    unsigned int port, received_port;
    unsigned int port2, received_port2;
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddr2;
    socklen_t nAddrLen;
    socklen_t nAddrLen2;
    fd_set rset;
    nAddrLen = sizeof(struct sockaddr_in);
    nAddrLen2 = sizeof(struct sockaddr_in);
    b_read = 1;

    if (argc != 4) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," <IPaddress_1>");
        strcat(error," <Port_1>");
        strcat(error," <Port_2>");
        perror(error);
        exit(1);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    if ( (sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    received_port = atoi(argv[2]);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(received_port);

    bzero(&servaddr2, sizeof(servaddr2));
    received_port2 = atoi(argv[3]);
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_port   = htons(received_port2);
    
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (inet_pton(AF_INET, argv[1], &servaddr2.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    if (connect(sockfd2, (struct sockaddr *) &servaddr2, sizeof(servaddr2)) < 0) {
        perror("connect error");
        exit(1);
    }

    // Get Ip and Port of Client
    getsockname(sockfd, (struct sockaddr *) &servaddr, &nAddrLen);
    inet_ntop(AF_INET, &servaddr.sin_addr, ip, sizeof(ip));
    port = ntohs(servaddr.sin_port);
    getsockname(sockfd2, (struct sockaddr *) &servaddr2, &nAddrLen2);
    inet_ntop(AF_INET, &servaddr2.sin_addr, ip2, sizeof(ip2));
    port2 = ntohs(servaddr2.sin_port);
    printf("Socket 1: \n");
    printf("IP: %s\n", ip);
    printf("Porta local: %u\n", port);
    printf("Server 1: \n");
    printf("IP: %s\n", argv[1]);
    printf("Porta local: %u\n", received_port);
    printf("-------------------------\n");
    printf("Socket 2: \n");
    printf("IP: %s\n", ip2);
    printf("Porta local: %u\n", port2);
    printf("Server 2: \n");
    printf("IP: %s\n", argv[1]);
    printf("Porta local: %u\n", received_port2);
    printf("-------------------------\n");

    FD_ZERO(&rset);
    while (1) {
        FD_SET(0, &rset);
        FD_SET(sockfd, &rset);
        FD_SET(sockfd2, &rset);
        if (b_read)
            maxfdp1 = maximum(0, sockfd, sockfd2) + 1;
        else
            maxfdp1 = MAX(sockfd, sockfd2) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);
        memset(&recvline, 0, sizeof(recvline));
        if (FD_ISSET(sockfd, &rset)) { /* atividade no socket_1 */
            if (read(sockfd, recvline, MAXLINE) == 0)
                perror("main: server_1 terminated prematurely");
            fputs(recvline, stdout);
        }
        memset(&recvline, 0, sizeof(recvline));
        if (FD_ISSET(sockfd2, &rset)) { /* atividade no socket_2 */
            if (read(sockfd2, recvline, MAXLINE) == 0)
                perror("main: server_2 terminated prematurely");
            
            fputs(recvline, stdout);
        }
        if (FD_ISSET(0, &rset)) { /* atividade na entrada padrão */
            if (fgets(recvline, sizeof(recvline), stdin) == NULL) {
                // // End clients
                strcpy(recvline, "END");
                send(sockfd, recvline, strlen(recvline), 0);
                send(sockfd2, recvline, strlen(recvline), 0);
                shutdown(sockfd, SHUT_WR);
                shutdown(sockfd2, SHUT_WR);
                FD_CLR(0, &rset);
                b_read = 0;
                break;
            }
            send(sockfd, recvline, strlen(recvline), 0);
            send(sockfd2, recvline, strlen(recvline), 0);
            sleep(1);
        }
    }

    return 0;
}
