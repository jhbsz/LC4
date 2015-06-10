
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

void usage(void)
{
	fprintf(stderr, "Usage: b64 [options] <string> \n\n"
	"b64 recognizes these commands:\n"
	"        -e                      base 64 encode ,\n"
	"        -d                      base 64 decode \n");
	exit(1);
}

#define BUF_LEN 128
int main(int argc, char **argv)
{
	int ch;
	char tmp[BUF_LEN] = {0};
	char buf[BUF_LEN] = {0};
    unsigned long outlen;
    int res;

    
	if (argc < 3)
		usage();
	while ((ch = getopt(argc, argv, "ed:")) != -1)
		switch (ch) {
			case 'e':
                strcpy(tmp, argv[2]);
                outlen = BUF_LEN;
                res = base64_encode((unsigned char *)tmp,strlen(tmp),
                       (unsigned char *)buf, &outlen);
                printf("%s\n", buf);
                break;
			case 'd':
                strcpy(tmp, argv[2]);
                outlen = BUF_LEN;
                res = base64_decode((unsigned char *)tmp, strlen(tmp),
                       (unsigned char *)buf, &outlen);
                printf("%s\n", buf);
                break;
			default:
				usage();
		}

    return 0;
	
}
