#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <stdlib.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define FALSE 0
#define TRUE 1
#define CURL_STR1  "curl -X POST -H \"X-Parse-Application-Id: NdA7uttdlgXRcz208dWYynrro4w3zCMcV7pXjKvh\" -H \"X-Parse-REST-API-Key: ZJMaOX79GbXATZuXYIpckGqYFv12mLKI5QHRafH9\" -H \"Content-Type: application/json\" -d \'{ \"channel\": \"togoPower\", \"data\": { \"alert\": \"Power "
#define CURL_STR2 "!\",\"sound\":\"default\" } }\' https://api.parse.com/1/push &"

volatile int STOP=FALSE;

void signal_handler_IO (int status);    //definition of signal handler
int wait_flag=TRUE;                     //TRUE while no signal received
int status;
char power_on=-1;
enum con_type{
    CH341=0
};
    

main(int Parm_Count, char *Parms[])
{
    int fd;
    struct termios oldtio, newtio;       //place for old and new port settings for serial port
    struct sigaction saio;               //definition of signal action
    char buf_s[255];                       //buffer for where data is put

    int input = socket(AF_INET, SOCK_DGRAM, 0);  
    struct sockaddr_in addr;  
    struct sockaddr_in from;  
    addr.sin_family = AF_INET;  
    addr.sin_port = htons(6666);  
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    status = bind(input,(struct sockaddr*)&addr,sizeof(addr));  
    if(status==-1)  
    {  
        perror("bind");  
        close(input);  
        exit(-1);  
    }  
    socklen_t len;  
    len = sizeof(from);  

    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror(MODEMDEVICE);
        exit(-1);
    }

    //install the serial handler before making the device asynchronous
    saio.sa_handler = signal_handler_IO;
    sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO,&saio,NULL);

    // allow the process to receive SIGIO
    fcntl(fd, F_SETOWN, getpid());
    // Make the file descriptor asynchronous (the manual page says only
    // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
    fcntl(fd, F_SETFL, FASYNC);

    tcgetattr(fd,&oldtio); // save current port settings 
    // set new port settings for canonical input processing 
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | 0 | 0 | 0 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;       //ICANON;
    newtio.c_cc[VMIN]=1;
    newtio.c_cc[VTIME]=0;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    // loop while waiting for input. normally we would do something useful here
    while (STOP==FALSE)
    {
        status = recvfrom(input,buf_s,sizeof(buf_s)-1,0,(struct sockaddr*)&from,&len);
        snprintf(buf_s,status+1,"%s",buf_s);
        /*
        if (!strcmp(buf_s,"ch341")) {
            type=CH341;
            status=0;
        }*/
        if (status > 0)
        {   
            int i;
            unsigned char out_s[255];
            char Key;
            char type;
            sscanf(buf_s, "type=%d,%s", &type, &buf_s);
            switch (type)
            {
                case CH341: 
                    sscanf(buf_s, "id=%2X%2X%2X%2X%2X,action=%2X,checksum=%2X,", &out_s[0],&out_s[1],&out_s[2],&out_s[3],&out_s[4],&out_s[5],&out_s[6]);
                    for (i=0;i<7;i++) {
                        write(fd,&out_s[i],1);
                    }
                    break;
            }
            Key=atoi(buf_s);
            if (Key == -2) STOP=TRUE;
            type=NULL;
        }  //end if a key was hit
        // after receiving SIGIO, wait_flag = FALSE, input is available and can be read

        if (wait_flag==FALSE)  //if input is available
        {
            int res;
            char buf[255];                       //buffer for where data is put
            res = read(fd,buf,255);
            if (res>0)
            {
                char event[1024];
                if (power_on != buf[res-1]) {
                    if (buf[res-1]==1) snprintf(event,sizeof(event),"%son%s",CURL_STR1,CURL_STR2);
                    else snprintf(event,sizeof(event),"%soff%s",CURL_STR1,CURL_STR2);
                    system(event);
                    power_on = buf[res-1];
                }
            }  //end if res>0
            wait_flag = TRUE;      /* wait for new input */
        }  //end if wait flag == FALSE
    }  //while stop==FALSE
    // restore old port settings
    tcsetattr(fd,TCSANOW,&oldtio);
    close(input);
    close(fd);        //close the com port
}  //end of main

/***************************************************************************
 * signal handler. sets wait_flag to FALSE, to indicate above loop that     *
 * characters have been received.                                           *
 ***************************************************************************/

void signal_handler_IO (int status)
{
    wait_flag = FALSE;
}

