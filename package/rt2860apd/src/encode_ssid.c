#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *base64encode(const unsigned char *buf, size_t *size);

static const char base64_str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_pad = '=';

static const signed char base64_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int main()
{
    char *base64enc = NULL;
    FILE *xFile, *oFile;
    unsigned char buf[1024];
    char *delim = "$";
    char *tmp = NULL;

    xFile = fopen("/tmp/wifi_site_not_encode", "r");
	oFile = fopen("/tmp/wifi_site","w");

    if(xFile==NULL) {
        printf("open wifi_site_not_encode fail\n");
    }
    else {
        while(fgets(buf, 1024, xFile)!=NULL) {
            tmp = strtok(buf, delim);
            int counter = 0;
            while(tmp!=NULL) {
                size_t len = strlen(tmp);
                if(counter==1) {
                    base64enc = base64encode(tmp, &len);
                    fprintf(oFile, "%s$", base64enc);
                }
                else
                    fprintf(oFile, "%s", tmp);
                tmp = strtok(NULL, delim);
				if(tmp!=NULL)
                    fprintf(oFile, "$", tmp);
                counter++;
            }
        }
		fclose(xFile);
    }
	fclose(oFile);

    return 0;
}

char *base64encode(const unsigned char *buf, size_t *size)
{
    if (!buf || !size || !(*size > 0)) return NULL;
    int outlen = (*size / 3) * 4;
    char *outbuf = (char*)malloc(outlen+5); // 4 spare bytes + 1 for '\0'
    size_t n = 0;
    size_t m = 0;
    unsigned char input[3];
    unsigned int output[4];
    while (n < *size) {
        input[0] = buf[n];
        input[1] = (n+1 < *size) ? buf[n+1] : 0;
        input[2] = (n+2 < *size) ? buf[n+2] : 0;
        output[0] = input[0] >> 2;
        output[1] = ((input[0] & 3) << 4) + (input[1] >> 4);
        output[2] = ((input[1] & 15) << 2) + (input[2] >> 6);
        output[3] = input[2] & 63;
        outbuf[m++] = base64_str[(int)output[0]];
        outbuf[m++] = base64_str[(int)output[1]];
        outbuf[m++] = (n+1 < *size) ? base64_str[(int)output[2]] : base64_pad;
        outbuf[m++] = (n+2 < *size) ? base64_str[(int)output[3]] : base64_pad;
        n+=3;
    }
    outbuf[m] = 0; // 0-termination!
    *size = m;
    return outbuf;
}

