#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comlib.h"
#include "termlib.h"
#include <string.h>

typedef enum __crlf_conv_enum {
    NO_CRLF_CONVERT,
    CONVERT_CRLF_TO_LF,
    REMOVE_CRLF,
} crlf_conv_enum;

typedef enum __ctrm_conv_enum {
    NO_CTRM_CONVERT,
    CONVERT_CTRM_TO_LF,
    CONVERT_CTRM_TO_CRLF,
    REMOVE_CTRM,
} ctrm_conv_enum;

#define BUF_LEN 1024
#define WHILE_LOOP
#define MAXNO(__A__, __B__) ((__A__ > __B__) ? __A__ : __B__)
static char comdev[100] = "/dev/ttyUSB0";

int remove_crlf(char *str, int len, int crlfconv, char *outstr) {

    char *crlfptr;
    int crlflen = 0, remove_len = crlfconv;
    strncpy(outstr, str, len);
    crlfptr = strchr(outstr,'\r');

    if(crlfptr && (crlfptr[1] == '\n')) {
        crlflen = crlfptr - outstr;
        strncpy(crlfptr, crlfptr + remove_len, len - crlflen - remove_len);
        return len - crlfconv;
    }
    return len;
}

void showstr(char *str, int len) {
    int i;
    for(i = 0;i < len;i++) {
        printf("0x%02x ", str[i]);
    }
    printf("\n");
}


//#define TEST_SEQ
int main(int argc, char*argv[]) {
    int com_fd;
    int term_fd;
    int maxfd;
    int activefd;
    char buf[BUF_LEN + 1];
    int len = 0;
    fd_set fds;
    int debug_mode = 0, conn_session = 1;
    unsigned long baudrate = 57600;
    char opt;
    crlf_conv_enum crlfconv = NO_CRLF_CONVERT;
    ctrm_conv_enum ctrmconv = NO_CTRM_CONVERT;
    
    while((opt = getopt(argc, argv, "b:c:dhn:m:")) != EOF) {
        switch(opt) {
            case 'b':
                baudrate = strtol(optarg, NULL, 10);
                break;
            case 'd':
                debug_mode = 1;
                break;
            case 'c':
                strcpy(comdev, optarg);
                break;
            case 'n':
                switch(optarg[0]) {
                    case '1':
                        crlfconv = CONVERT_CRLF_TO_LF;
                        break;
                    case '2':
                        crlfconv = REMOVE_CRLF;
                        break;
                    default:
                        crlfconv = NO_CRLF_CONVERT;
                        break;
                }
                break;
            case 'm':
                switch(optarg[0]) {
                    case '1':
                        ctrmconv = CONVERT_CTRM_TO_LF;
                        break;
                    case '2':
                        ctrmconv = CONVERT_CTRM_TO_CRLF;
                        break;
                    case '3':
                        ctrmconv = REMOVE_CTRM;
                        break;
                    default:
                        ctrmconv = NO_CTRM_CONVERT;
                        break;
                }
                break;
            case 'h':
                printf("Usage : comnetserv [-b baudrate] [-c com_device] [-d] [-h]\n");
                printf("-b boadrate : use baudrate ie. -b 57600\n");
                printf("-c com_device : use com device : ie. -c /dev/ttyUSB0\n");
                printf("-n 1,2 : 1:convert all \\r\\n to \\n 2: remove all \\r\\n \n");
                printf("-m 1,2,3 : 1: convert ^M to \\n 2: convert ^M \\r\\n 3. remove all ^M \n");
                printf("-d : enabling debug mode\n");
                printf("-h : show this message\n");
                return 0;
            default:
                break;
        }
    }
    com_fd = open_com(comdev, baudrate);
    if(com_fd <= 0) {
        printf("Open com device failed.\n");
        return 1;
    }
    term_fd = open_term();
    if(term_fd <= 0) {
        printf("Open terminal device failed.\n");
        return 1;
    }
    FD_ZERO(&fds);
    FD_SET(com_fd, &fds);
    FD_SET(term_fd, &fds);
    maxfd = MAXNO(com_fd, term_fd);
#ifdef WHILE_LOOP
    while(1) {
        if((activefd = select(maxfd+1, &fds, NULL, NULL, NULL)) > 0) {
            if(FD_ISSET(com_fd, &fds)) {
                len = read(com_fd, buf, BUF_LEN);
                buf[len] = 0;
                write(term_fd, buf, len);
            }
            if(FD_ISSET(term_fd, &fds)) {
                len = read(term_fd, buf, BUF_LEN);
		if((buf[len-1] == '\n') && (buf[len-2] != '\r')) {
		    buf[len-1] = '\r';
		    buf[len] = '\n';
		    len ++;
		}
                buf[len] = 0;
                if((crlfconv == CONVERT_CRLF_TO_LF) || (crlfconv == REMOVE_CRLF)) {
                    char outbuf[BUF_LEN + 1];
                    int outbuflen;
                    outbuflen = remove_crlf(buf, len, crlfconv, outbuf);
                    outbuf[outbuflen] = 0;
                    strcpy(buf, outbuf);
                }
                if(!strncmp(buf, "doexit", 6)) {
                    conn_session = 0;
                    break;
                }
                if(debug_mode) {
                    int idx;
                    printf("Received net request:\n");
                    for(idx = 0;idx < len;idx++) {
                        printf("%c:%x\n", buf[idx], buf[idx]);
                    }
                    printf("\n");
                }
                write(com_fd, buf, len);
            }
            FD_SET(com_fd, &fds);
            FD_SET(term_fd, &fds);
        }
    }
#endif
    close_com(com_fd);
    close_term(term_fd);
    return 0;
}
