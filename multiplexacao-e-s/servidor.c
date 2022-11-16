#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define MAXLINE 4096
#define LISTENQ 10

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main (int argc, char **argv) {
    int    listenfd, connfd, id;
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddr2;
    char   ip[16];
    char   commands[MAXLINE];
    char   temp[MAXLINE];
    char   received_msg[MAXLINE];
    char   error[MAXLINE + 1];
    pid_t pid;
    time_t ticks;
    unsigned int port, received_port;
    socklen_t nAddr2Len;
    nAddr2Len = sizeof(struct sockaddr_in);
    // FILE *fp;
    id = 0;

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

    signal (SIGCHLD, sig_chld); /* para chamar waitpid() */
    while (1) {
        if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
            perror("accept");
            exit(1);
        }
        id++;

        if( (pid = fork()) == 0) {
            close(listenfd); /* child closes listening socket */

            // Get Ip and Port of Client
            getpeername(connfd, (struct sockaddr * )&servaddr2, &nAddr2Len);
		    inet_ntop(AF_INET, &servaddr2.sin_addr, ip, sizeof(ip));
		    port = ntohs(servaddr2.sin_port);
            // info of client
            printf("Client: %d\n", id);
		    printf("IP: %s\n", ip);
		    printf("Porta: %u\n", port);
            ticks = time(NULL);
            printf("Connection Time: %.24s from Client %d\r\n", ctime(&ticks), id);
            printf("--------------------\n");

            // commands
            strcat(commands, "Hello from server to client in:\n");
            snprintf(temp, sizeof(temp), "IP: %s\n", ip);
            strcat(commands, temp);
            snprintf(temp, sizeof(temp), "Porta: %u\n", port);
            strcat(commands, temp);
            snprintf(temp, sizeof(temp), "Time: %.24s\r\n", ctime(&ticks));
            strcat(commands, temp);
            
            write(connfd, commands, strlen(commands));

            while (read(connfd, received_msg, MAXLINE) != -1) {
                if (strcmp(received_msg, "END") == 0)
                    break;

                write(connfd, received_msg, strlen(received_msg));

                memset(&received_msg, 0, sizeof(received_msg));
            }

            ticks = time(NULL);
            printf("Disconnection Time: %.24s from Client %d\r\n--------------------\n", ctime(&ticks), id);

            close(connfd);

            exit(0);
        }
    }
    
    return 0;
}
