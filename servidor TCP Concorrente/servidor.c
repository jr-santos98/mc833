#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LISTENQ 10
#define MAXCHAR 100
#define MAXLINE 4096

int main (int argc, char **argv) {
    int    listenfd, connfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddr2;
    char   buf[MAXCHAR];
    char   ip[16];
    char   commands[5][MAXCHAR];
    char   received_msg[MAXLINE];
    char   error[MAXCHAR + 1];
    time_t ticks;
    unsigned int port, received_port;
    socklen_t nAddr2Len;
    nAddr2Len = sizeof(struct sockaddr_in);

    if (argc != 2) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," <Port>");
        perror(error);
        exit(1);
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    received_port = atoi(argv[1]);
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(received_port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) == -1) {
        perror("listen");
        exit(1);
    }

    for ( ; ; ) {
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
        perror("accept");
        exit(1);
        }

        // Get Ip and Port of Client
        getpeername(connfd, (struct sockaddr * )&servaddr2, &nAddr2Len);
		inet_ntop(AF_INET, &servaddr2.sin_addr, ip, sizeof(ip));
		port = ntohs(servaddr2.sin_port);
		printf("IP: %s\n", ip);
		printf("Porta: %u\n", port);
        ticks = time(NULL);
        printf("Connection Time: %.24s\r\n", ctime(&ticks));

        // Write message
        snprintf(buf, sizeof(buf), "Hello from server!\nTime: %.24s\r\n", ctime(&ticks));
        write(connfd, buf, strlen(buf));

        // commands
        strcpy(commands[0], "pwd\0"); 
        strcpy(commands[1], "ls\0");
        strcpy(commands[2], "hostname\0");
        strcpy(commands[3], "neofetch\0");
        strcpy(commands[4], "exit\0");

        // Send commands
        for (int i=0; i<5; i++) {
            write(connfd, commands[i], strlen(commands[i]));

            // Read command
            read(connfd, received_msg, MAXCHAR);
            printf("Result: %s\n%s", commands[i], received_msg);
            memset(&received_msg, 0, sizeof(received_msg));
        }

        // sleep(8);
        ticks = time(NULL);
        printf("Disconnection Time: %.24s\r\n", ctime(&ticks));
        // close(connfd);
    }
    
    return 0;
}
