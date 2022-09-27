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

#define LISTENQ 10
#define MAXDATASIZE 100

int main (int argc, char **argv) {
    int    listenfd, connfd;
    struct sockaddr_in servaddr;
    char   buf[MAXDATASIZE];
    time_t ticks;
    char   ipp[16];
    unsigned int portT;
    char client_message[1000];
    int read_size;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port        = htons(2022);   
    
    

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd, LISTENQ) == -1) {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in servaddr2;

    for ( ; ; ) {
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
        perror("accept");
        exit(1);
        }
	
		//
		bzero(&servaddr2, sizeof(servaddr2));
		socklen_t len = sizeof(servaddr2);
		getpeername(connfd, (struct sockaddr * )&servaddr2, &len);
		inet_ntop(AF_INET, &servaddr2.sin_addr, ipp, sizeof(ipp));
		portT = ntohs(servaddr2.sin_port);
		printf("IP: %s\n", ipp);
		printf("Porta: %u\n", portT);
		//

        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "Hello from server!\nTime: %.24s\r\n", ctime(&ticks));
        write(connfd, buf, strlen(buf));

        read(connfd, client_message, 1000);
        printf("recieved message: %s \n", client_message);
        sleep(6);
        close(connfd);

    }
    return(0);
}
