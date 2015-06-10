#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <string.h>


static struct termios oldtio;
int open_term(void) {
    struct termios newtio;       //place for old and new port settings for serial port
    int consolefd = open("/dev/tty", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(consolefd > 0) {
        // tcgetattr(consolefd,&oldtio);
        // tcgetattr(consolefd,&newtio);
        // newtio.c_cflag |= ECHO;
        // newtio.c_iflag = IGNPAR;
        // newtio.c_oflag = 0;
        // newtio.c_lflag &= ~ECHO;
        // newtio.c_cc[VMIN]=1;
        // newtio.c_cc[VTIME]=0;
        // tcflush(consolefd, TCIFLUSH);
        // tcsetattr(consolefd,TCSANOW,&newtio);
        return consolefd;
    }
    printf("Console open failed\n");
    return -1;
}

int close_term(int consolefd) {
    if(consolefd > 0) {
        printf("Invalid console fd to close:%d\n", consolefd);
        return -1;
    }
    return 0;
}

