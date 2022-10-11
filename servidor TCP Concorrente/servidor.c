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
#define MAXLINE 4096

int main (int argc, char **argv) {
    int    listenfd, connfd, id;
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddr2;
    char   ip[16];
    char   commands[5][MAXLINE];
    char   received_msg[MAXLINE];
    char   error[MAXLINE + 1];
    pid_t pid;
    time_t ticks;
    unsigned int port, received_port;
    socklen_t nAddr2Len;
    nAddr2Len = sizeof(struct sockaddr_in);
    FILE *fp;
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
            // printf("Client: %d\n", id);
		    // printf("IP: %s\n", ip);
		    // printf("Porta: %u\n", port);
            ticks = time(NULL);
            // printf("Connection Time: %.24s from Client %d\r\n", ctime(&ticks), id);


            // Escrevendo no arquivo
            fp = fopen("result.txt", "a+");
            if (fp == NULL) {
                printf("Problemas na CRIACAO do arquivo\n");
                exit(1);
            }
            fprintf(fp, "---------- Client: %d ----------\n", id);
            fprintf(fp, "IP: %s\n", ip);
            fprintf(fp, "Porta: %d\n", port);
            fprintf(fp, "Connection Time: %.24s from Client %d\r\n", ctime(&ticks), id);
            fclose(fp);

            // commands
            strcpy(commands[0], "pwd\0"); 
            strcpy(commands[1], "ls\0");
            strcpy(commands[2], "hostname\0");
            // commands[3] -> Para entrega
            strcpy(commands[3], "uname -a\0");
            // commands[3] -> Para testes
            // strcpy(commands[3], "neofetch\0");
            strcpy(commands[4], "exit\0");

            // Send commands
            for (int i=0; i<5; i++) {
                // Progress per client
                printf("Client: %d\n", id);
		        printf("IP: %s\n", ip);
		        printf("Porta: %u\n", port);
                printf("Exec: %s\n", commands[i]);
                printf("--------------------\n");
                // Send command[i]
                write(connfd, commands[i], strlen(commands[i]));

                // Read command
                read(connfd, received_msg, MAXLINE);

                fp = fopen("result.txt", "a+");
                if (fp == NULL) {
                    printf("Problemas na CRIACAO do arquivo\n");
                    exit(1);
                }

                fprintf(fp, "---------- Client: %d ----------\n", id);
                fprintf(fp, "Result: %s\n%s", commands[i], received_msg);

                fclose(fp);

                memset(&received_msg, 0, sizeof(received_msg));
            }

            ticks = time(NULL);
            // printf("Disconnection Time: %.24s from Client %d\r\n", ctime(&ticks), id);

            // Escrevendo no arquivo
            fp = fopen("result.txt", "a+");
            if (fp == NULL) {
                printf("Problemas na CRIACAO do arquivo\n");
                exit(1);
            }
            fprintf(fp, "Disconnection Time: %.24s from Client %d\r\n", ctime(&ticks), id);
            fclose(fp);

            exit(0);
        }
    }
    
    return 0;
}
