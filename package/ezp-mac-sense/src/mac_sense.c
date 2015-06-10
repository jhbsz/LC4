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

#define DATA_LEN 896
static int connfd = -1;
static int debug=0;
static char *data;
void message_print(char *str);
int open_tcp_client(char *ip_addr, unsigned short int port)
{
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        message_print("\n Error : Could not create socket \n");
        return -1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0)
    {
        message_print("\n inet_pton error occured\n");
        return -2;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        message_print("\n Error : Connect Failed \n");
        return -3;
    } 
    connfd = sockfd;
    return sockfd;
}

int close_tcp_client(int clifd) {
    if(clifd < 1) {
        message_print("Not a volid connection socket\n");
        return -1;
    }
    close(clifd);
    connfd = -1;
    return 0;
}

void get_data(char *ap_addr)
{
    FILE * fp;
    char buff[1024];
    char mac[24],mac_ap[24],time[32],count[32],rssi[8];

    fp = popen("iwpriv ra0 sensekick","r");
    if( feof( fp ) || fp < 0)
    {    
        message_print("File Error!!\n");
        return;
    }
    for(;strlen(fgets(buff, sizeof (buff),fp)) > 1;)
    {
        if (strlen(buff) < 24 )
            continue;
        sscanf(buff,"%s\t%s\t%s\t%s\t%s\n",mac,rssi,time,count,mac_ap);
        sprintf(data+strlen(data),"%s,%s,%s,%s,%s;",mac,time,rssi,count,ap_addr);
    }
    pclose(fp); 
    sprintf(data+strlen(data),"&");
}

void message_print(char *str)
{
    if (str && debug == 1)
        printf("%s\n",str);
}

int main( int argc, char **argv )
{
    int status,x=0,len = 0,y;
    char *send_data,*tmp_data;

    if(!argv[1] || !argv[2] || !argv[3] || !argv[4] || !argv[5])
    {
        printf("ERROR!!\n");
        exit(1);
    }

    data = (char *)malloc(4096);
    send_data = (char *)malloc(4096);
    tmp_data = (char *)malloc(4096);

    sprintf(send_data,"client_id=4765272503474547&client_secret=niUQ2nYjRu8dBVvNENwELqtouWM3eqKB&cmd=am&UUID=%s&location=",argv[3]);
    while(1)
    {
        if(connfd < 0)
        {
            connfd = open_tcp_client(argv[1], atoi(argv[2]));
        }
        if(connfd > 0 && x > atoi(argv[4]))
        {
            struct timeval timeout = {1,0};
            setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));

            x=0;
            memset(data,0,sizeof(data));
            get_data(argv[3]);
            len = strlen(data);

            for (y = 0;len > (y * DATA_LEN);y++)
            {
                strcpy(tmp_data,send_data);
                strcat(tmp_data,(data + (y * DATA_LEN)));
                status = send(connfd,tmp_data,strlen(tmp_data),0);
                message_print(tmp_data);
                printf("send %d bytes\n", status);
            }
            status = send(connfd, "\0", 1, 0);
            printf("send %d bytes\n", status);
            
            if(status > 0) {
                message_print("OK\n");
                sleep(2);
                x += 2;
            }
            else
            {
                message_print("Fail\n");
                sleep(atoi(argv[5]));
            }
            close_tcp_client(connfd);
        }
        x++;
        sleep(1);
    }
}
