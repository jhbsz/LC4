/*******************************************************************************************************************************
*
*      Copyright (c) 2013  QNAP Systems, Inc.  All Rights Reserved.
*
*      FILE:
*              encrypt.h
*
*      Abstract:
*              Common definition header to encryption function.
*
*      HISTORY:
*              04/24/2013        iInitial version by Y.T. Lee
*
********************************************************************************************************************************/

#ifndef __ENCRYPT_INCLUDE_H__
#define __ENCRYPT_INCLUDE_H__
#ifdef WIN32
#include "winsock2.h"
#else //WIN32
#include<sys/time.h>
#include<unistd.h>
#include "dbg.h"
#endif //WIN32

#define MAX_CONCURRENT_CLIENTS 16
#define MAX_BUF_SIZE 128
#define MAX_STR_SIZE 32
#define MAX_DATA_SIZE32 8 
#define KEY_LENGTH32 4 //16 bytes
#define KEY_TABLE_AGING_SEC 3600  //1 hour before expiring the entry in key table
#define USERNAME_KEY 'U'  //XOR key for user name
#define PASSWORD_KEY 'P'  //XOR key for password

typedef struct //8 byte alignment
{
    struct  timeval refresh_time; //update when recieving packet from this client
    unsigned long long mac_hash; //MAC of finder
    unsigned long key[KEY_LENGTH32];
} KEY_TYPE;
 
void get_16byte_key(unsigned long *output_key); 

unsigned char ascii2byte(char *val);

void tea_decrypt(unsigned long *v, unsigned long *k); 

void XORStrByKey(unsigned char *target_string, unsigned char key_character, int length);

int get_decrypt_output_length(const char* szInput);

void _to_decrypt_str(unsigned long *key, unsigned char key_character, char *szInput, int nInputLength, unsigned char *szOutput, int nOutputLength);

#endif
