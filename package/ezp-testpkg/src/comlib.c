#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>
static struct termios oldtio;
int open_com(char *comdev, unsigned long baudrate) {
    int comfd;
    long baudspec;
    int Data_Bits = 8;              // Number of data bits
    int Stop_Bits = 1;              // Number of stop bits
    long DATABITS = CS8;
    long STOPBITS = 0;
    long PARITYON = 0;
    long PARITY = 0;
    int Format = 1;
    struct termios newtio;       //place for old and new port settings for serial port
    if(!comdev) {
        printf("Must specify a com device, for example:/dev/ttyUSB0.\n");
        return -1;
    }
    switch(baudrate) {
        case 38400:
            baudspec = B38400;
            break;
        case 57600:
            baudspec = B57600;
            break;
        case 115200:
            baudspec = B115200;
            break;
        case 230400:
            baudspec = B230400;
            break;
        case 460800:
            baudspec = B460800;
            break;
        case 1152000:
            baudspec = B1152000;
            break;
        case 1500000:
            baudspec = B1500000;
            break;
        defaut:
            printf("Not supported baudrate:%ld, supported rate 38400, 57600, 115200, 230400, 460800, 1152000, 1500000\n", baudrate);
            return -1;
    }
    comfd = open(comdev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    // comfd = open(comdev, O_RDWR | O_NOCTTY);
    oldtio.c_cflag = baudspec | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
    oldtio.c_iflag = IGNPAR;
    oldtio.c_oflag = 0;
    oldtio.c_lflag = 0;
    oldtio.c_cc[VMIN]=1;
    oldtio.c_cc[VTIME]=0;
    if(comfd > 0) {
        tcgetattr(comfd,&oldtio);
        newtio.c_cflag = baudspec | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;
        newtio.c_cc[VMIN]=1;
        newtio.c_cc[VTIME]=0;
        tcflush(comfd, TCIFLUSH);
        tcsetattr(comfd,TCSANOW,&newtio);
        return comfd;
    }
    return -1;
}


int close_com(int comfd) {
    if(comfd < 0) {
        printf("INvalid comfd:%d for closing\n", comfd);
        return -1;
    }
    tcsetattr(comfd,TCSANOW,&oldtio);
    close(comfd);        //close the com port
    comfd = -1; 
    return 0;
}
