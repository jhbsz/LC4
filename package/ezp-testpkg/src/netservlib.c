#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

static int serv_listen_socket = -1;
static int serv_conn_socket = -1;
int open_tcp_server(unsigned short port)
{
    int listenfd = 0, connfd = 0, res;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd <= 0) {
        printf("Create socket failed\n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    res = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(res < 0) {
        printf("Bind to specify port %d failed\n", port);
        return -2;
    }
    serv_listen_socket = listenfd;
    return listenfd;
    /*
    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
    }
    */
}

int wait_for_connection_serv(int listenfd) {
    int connfd;
    if(listenfd < 1) {
        printf("Not a valid server listening socket :%d\n", listenfd);
        return -1;
    }
    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        serv_conn_socket = connfd;
        return connfd;
    }
    return 0;
}

int close_connection_serv(int connfd) {
    if(connfd < 1) {
        printf("Not a valid connection socket :%d\n", connfd);
        return -1;
    }
    close(connfd);
    serv_conn_socket = -1;
    return 0;
}

int close_tcp_server(int listenfd) {
    if(listenfd < 1) {
        printf("Not a valid server listening socket :%d\n", listenfd);
        return -1;
    }
    close(listenfd);
    serv_listen_socket = -1;
    return 0;
}
