#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

static int connfd = -1;
int open_tcp_client(char *ip_addr, unsigned short int port)
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return -1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return -2;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return -3;
    } 
    connfd = sockfd;
    return sockfd;
}

int close_tcp_client(int clifd) {
    if(clifd < 1) {
        printf("Not a volid connection socket:%d\n", clifd);
        return -1;
    }
    close(clifd);
    connfd = -1;
    return 0;
}
