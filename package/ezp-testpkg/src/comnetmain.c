#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "comlib.h"
#include <string.h>

#define BUF_LEN 1024
#define WHILE_LOOP
#define MAXNO(__A__, __B__) ((__A__ > __B__) ? __A__ : __B__)
static char comdev[100] = "/dev/ttyUSB0";
//#define TEST_SEQ
int main(int argc, char*argv[]) {
    int com_fd = -1;
    int net_serv_fd, net_conn_fd;
    int maxfd;
    int activefd;
    char buf[BUF_LEN + 1];
    int len = 0;
    fd_set fds;
    int conn_session = 0;
    char opt;
    unsigned short serv_port = 6789;
    int debug_mode = 0;

    while((opt = getopt(argc, argv, "p:c:dh")) != EOF) {
        switch(opt) {
            case 'p':
                serv_port = strtol(optarg, NULL, 10);
                break;
            case 'd':
                debug_mode = 1;
                break;
            case 'c':
                strcpy(comdev, optarg);
                break;
            case 'h':
                printf("Usage : comnetserv [-p port] [-d] [-h]\n");
                printf("-p port : use port for server listening\n");
                printf("-d : enabling debug mode\n");
                printf("-h : show this message\n");
                return 0;
            default:
                break;
        }
    }

    net_serv_fd = open_tcp_server(serv_port);
    if(net_serv_fd < 1) {
        printf("Open server listening socket failed\n");
        return -1;
    }
    while(1) {
        net_conn_fd = wait_for_connection_serv(net_serv_fd);
        if(net_conn_fd < 1) {
            printf("Can not get proper connection\n");
            return -1;
        }
        if(com_fd < 1) {
            com_fd = open_com("/dev/ttyUSB0", 57600);
            if(com_fd <= 0) {
                printf("Open com device failed.\n");
                return 1;
            }
        }
        conn_session = 1;
        FD_ZERO(&fds);
        FD_SET(com_fd, &fds);
        FD_SET(net_conn_fd, &fds);
        maxfd = MAXNO(com_fd, net_conn_fd);
#ifdef WHILE_LOOP
        while(conn_session) {
            if((activefd = select(maxfd+1, &fds, NULL, NULL, NULL)) > 0) {
                if(FD_ISSET(com_fd, &fds)) {
                    len = read(com_fd, buf, BUF_LEN);
                    buf[len] = 0;
                    write(net_conn_fd, buf, len);
                }
                if(FD_ISSET(net_conn_fd, &fds)) {
                    int idx;
                    len = read(net_conn_fd, buf, BUF_LEN);
                    buf[len] = 0;
                    if(!strncmp(buf, "exit", 4)) {
                        conn_session = 0;
                        break;
                    }
                    if(debug_mode) {
                        printf("Received net request:\n");
                        for(idx = 0;idx < len;idx++) {
                            printf("%c:%x\n", buf[idx], buf[idx]);
                        }
                        printf("\n");
                    }
                    // write(term_fd, buf, len);
                    write(com_fd, buf, len);
                }
                FD_SET(com_fd, &fds);
                FD_SET(net_conn_fd, &fds);
            }
        }
#endif
        close_connection_serv(net_conn_fd);
    }
    close_com(com_fd);
    close_tcp_server(net_serv_fd);
    return 0;
}
