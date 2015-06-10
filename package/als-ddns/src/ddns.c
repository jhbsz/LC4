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

static char* host = "www.mgddns.com";
static int port = 80;
static char buff[4096],message[2048];
static int connfd = -1;

int open_tcp_client(char *ip_addr, unsigned short int port)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    struct hostent * remoteHost = NULL;

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return -1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if( (remoteHost = gethostbyname(ip_addr)) == 0 )
    {
        printf("Error resolving host\n");
        return -2;
    }

    serv_addr.sin_addr.s_addr = ( (struct in_addr *)(remoteHost->h_addr) )->s_addr;

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

char* str_change(char str[])
{
    int i;

    for(i=0;i < strlen(str);i++)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i]+=32;
    }
    return str;
}    

void set_header(char *api)
{
    sprintf(message,"GET %s HTTP/1.1\r\nHost:%s\r\n",api,host);
    strcat(message,"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:29.0) Gecko/20100101 Firefox/29.0\r\n");
    strcat(message,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    strcat(message,"Accept-Language: zh-tw,zh;q=0.8,en-us;q=0.5,en;q=0.3\r\n");
    strcat(message,"Connection: keep-alive\r\n");
    strcat(message, "\r\n\r\n");
}



void get_web(int sockfd)
{
    char test1[1024],test2[256];
    char *delim = "\r\n",*str = NULL;

    for(;recv(sockfd, buff, sizeof(buff), 0);)
    {
        if(strstr(buff,"\r\n\r\n"))
            break;
    }

    str = strtok(buff,delim);

    while(str != NULL)
    {
        if (!strncmp(str,"{",1))
        {    
            printf("%s\n",str);
        }
        str = strtok(NULL,delim);
    }

    close_tcp_client(sockfd);
}

int main( int argc, char **argv )
{
    char command[1024] = {0};
    int sockfd;

    if(!argv[1]) exit(1);

    if ((sockfd = open_tcp_client(host,port)) < 0 )
    {
        printf("Error opening socket!\n");
        exit(1);
    }

    memset(message,0,sizeof(message));
    memset(command,0,sizeof(command));

    if ( !strcmp(argv[1],"register")){ // /api/createaccount.php?mac=
        sprintf(command,"/api/createaccount.php?mac=%s",argv[2]); 
        set_header(command);
    }else if ( !strcmp(argv[1],"update")){ // /api/updateddns.php?id=&pass=&subname=
        sprintf(command,"/api/updateddns.php?id=%s&pass=%s&subname=%s",argv[2],argv[3],argv[4]); 
        set_header(command);
    }

    if( send(sockfd, message, strlen(message), 0) == -1)
    {
        printf("Error in send\n");
        exit(1);
    }

    struct timeval timeout = {1,0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

    get_web(sockfd); //get web information
    close_tcp_client(sockfd);

    return 0;
}
