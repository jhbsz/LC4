/*******************************************************************************************************************************
*
*      Copyright (c) 2013  QNAP Systems, Inc.  All Rights Reserved.
*
*      FILE:
*              encrypt.c
*
*      Abstract:
*              Common encryption utility function implementations.
*
*      HISTORY:
*              04/24/2013        Initial version by Y.T. Lee
*
********************************************************************************************************************************/

#define __INCLUDE_FROM_FINDER_PROTOCOLS_IMPLEMENTATION__

#include<stdio.h>
#include<string.h>
#ifdef WIN32
#include <time.h>
#else
#include<unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#endif //WIN32

#include "encrypt.h"
 
void get_16byte_key(unsigned long *output_key) {
#define DEBUGKEY
#ifdef DEBUGKEY
    output_key[0] = 1;
    output_key[1] = 2;
    output_key[2] = 3;
    output_key[3] = 4;
#else //DEBUGKEY
#ifdef WIN32
    srand( (unsigned)time( NULL ) );

    output_key[0] = rand() * unsigned long(-1) / RAND_MAX;
    output_key[1] = rand() * unsigned long(-1) / RAND_MAX;
    output_key[2] = rand() * unsigned long(-1) / RAND_MAX;
    output_key[3] = rand() * unsigned long(-1) / RAND_MAX;
#else //WIN32
    int fd;
    struct ifreq ifr;

    char mac[20];
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv,&tz);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
 
    ioctl(fd, SIOCGIFHWADDR, &ifr);

    close(fd);
 
    sprintf(mac,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
         (unsigned char)ifr.ifr_hwaddr.sa_data[0],
         (unsigned char)ifr.ifr_hwaddr.sa_data[1],
         (unsigned char)ifr.ifr_hwaddr.sa_data[2],
         (unsigned char)ifr.ifr_hwaddr.sa_data[3],
         (unsigned char)ifr.ifr_hwaddr.sa_data[4],
         (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    printf("eth0 mac address:%s\n", mac);
    output_key[0]=(unsigned long)ifr.ifr_hwaddr.sa_data[2]^tv.tv_usec;
    output_key[1]=(unsigned long)ifr.ifr_hwaddr.sa_data[3]^tv.tv_usec;
    output_key[2]=(unsigned long)ifr.ifr_hwaddr.sa_data[4]^tv.tv_usec;
    output_key[3]=(unsigned long)ifr.ifr_hwaddr.sa_data[5]^tv.tv_usec;
#endif //WIN32
#endif //DEBUGKEY
}


unsigned char ascii2byte(char *val)
{
    unsigned char temp = *val;

    if(temp > 0x60) temp -= 39;  // convert chars a-f
    temp -= 48;  // convert chars 0-9
    temp *= 16;

    temp += *(val+1);
    if(*(val+1) > 0x60) temp -= 39;  // convert chars a-f
    temp -= 48;  // convert chars 0-9   

    return temp;

}


void tea_decrypt (unsigned long *v, unsigned long *k) {

    unsigned long v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */

    unsigned long delta=0x9e3779b9;                     /* a key schedule constant */

    unsigned long k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */

    for (i=0; i<32; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              /* end cycle */

    v[0]=v0; v[1]=v1;
}

// XOR the string with character
// 
void XORStrByKey(unsigned char *target_string, unsigned char key_character, int length)
{
    int i;
    for(i=0;i<length;i++)
    {
        target_string[i]^=key_character;
    }
}

int get_decrypt_output_length(const char* szInput)
{
    int output_len = 0;
	
    if (szInput)
    {
        output_len = strlen(szInput) / 2;
    }
    return output_len;
}

void _to_decrypt_str(unsigned long *key, unsigned char key_character, char *szInput, int nInputLength, unsigned char *szOutput, int nOutputLength)
{
    int i = 0;
	
    if (szOutput)
    {
        memset(szOutput, 0, nOutputLength);
        for (i = 0; i < nOutputLength; i++) 
        {
            szOutput[i] = (char)ascii2byte(szInput + (i * 2));
        }
		
        XORStrByKey(szOutput, key_character, nOutputLength);
		
        for (i = 0; i < nOutputLength; i += 8)
        {	//every two 32bit 
            tea_decrypt((unsigned long*)(szOutput + i), key);
        }
    }
}

