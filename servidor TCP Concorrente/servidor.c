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

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main (int argc, char **argv) {
    int    listenfd, listenq, connfd, id, i;
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddr2;
    char   ip[16];
    char   commands[10][MAXLINE];
    char   received_msg[MAXLINE];
    char   error[MAXLINE + 1];
    pid_t pid;
    time_t ticks;
    unsigned int port, received_port;
    socklen_t nAddr2Len;
    nAddr2Len = sizeof(struct sockaddr_in);
    FILE *fp;
    id = 0;

    if (argc != 3) {
        strcpy(error,"uso: ");
        strcat(error,argv[0]);
        strcat(error," <Port>");
        strcat(error," <Backlog>");
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

    listenq = atoi(argv[2]);
    if (listen(listenfd, listenq) == -1) {
        perror("listen");
        exit(1);
    }

    signal (SIGCHLD, sig_chld); /* para chamar waitpid() */
    while (1) {
        // sleep(5); Not necessery
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
            strcpy(commands[0], "pwd"); 
            strcpy(commands[1], "ls");
            strcpy(commands[2], "hostname");
            strcpy(commands[3], "uname -a");
            strcpy(commands[4], "lscpu");
            strcpy(commands[5], "lsblk");
            strcpy(commands[6], "lsusb");
            strcpy(commands[7], "lspci");
            strcpy(commands[8], "df -H");
            strcpy(commands[9], "exit");

            // Send commands
            for (int j=0; j<4; j++) {
                srand((int)time(&ticks) % getpid());
                if (j == 3)
                    i = 9;
                else
                    i = (rand() * strlen(commands[j])) % 9;
                // Send 3 commands random
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
            printf("Disconnection Time: %.24s from Client %d\r\n--------------------\n", ctime(&ticks), id);

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
