#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "optlib.h"

crlf_conv_enum crlfconv = NO_CRLF_CONVERT;
ctrm_conv_enum ctrmconv = NO_CTRM_CONVERT;
unsigned long baudrate = 57600;
int debug_mode = 0;
char comdev[100] = "/dev/ttyUSB0";
unsigned short int serv_port = 6789;

void print_options(void) {
    printf("Baudrate:%ld\n", baudrate);
    printf("COM device:%s\n", comdev);
    printf("Crlfconv:%s\n",((crlfconv == CONVERT_CRLF_TO_LF) ? 
                "Convert CR/LF to \\n":
                ((crlfconv == REMOVE_CRLF) ? 
                   "Remove all CRLF":
                   "Do not convert CR/LF")));
    printf("^M convertion:%s\n", ((ctrmconv == CONVERT_CTRM_TO_LF) ? 
                "Convert ^M to \\n" : 
                ((ctrmconv == CONVERT_CTRM_TO_CRLF)?
                   "Convert ^M to \\r\\n":
                   ((ctrmconv == REMOVE_CTRM)?
                      "Ignore ^M" :
                      "Transfer ^M to device and do not convert"))));
    printf("port:%d\n", serv_port);
    printf("debug mode:%s\n", ((debug_mode == 1) ? "Enabled" : "Disabled"));
}

int get_opt_item(int argc, char *argv[]) {
    char opt;
    while((opt = getopt(argc, argv, "p:b:c:dhn:m:")) != EOF) {
        switch(opt) {
            case 'p':
                serv_port = strtol(optarg, NULL, 10);
                break;
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
    return 0;
}

