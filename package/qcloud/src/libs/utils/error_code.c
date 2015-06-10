/*
 * error_code.c
 *
 *  Created on: Apr 18, 2011
 *      Author: ch
 */


#include "error_code.h"
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#define E(code, s) { code, (s "[" #code "]") }
#else
#define E(code, s) { code, ("[" #code "]") }
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define UNKNOWN_ERROR_RESP(e)       "unknown error\0"
static char unknown_error[128] =    "\0";

typedef struct error_set{
    int code;
    const char *msg;
}error_set;

error_set generic_errorstr[] = {
    E(ERR_GENERIC, "generic error"),
    E(ERR_NULL_POINTER, "pointer is null"),
    E(ERR_NO_MEMORY, "data is not found"),
    E(ERR_RANGE, "data is out of range"),
    E(ERR_NO_CONNECTION, "connection to server failed"),
    E(ERR_MMC_TAG, "invalid tag"),
    E(ERR_MMC_KEY, "invalid key"),
    E(ERR_OVERFLOW, "buffer is not big enough"),
    E(ERR_SYSTEM, "system error"),
    E(ERR_INVALID_FORMAT, "invalid data format"),
    E(ERR_NO_FILE, "file doesn't exist"),
    E(ERR_INVALID_PORT, "invalid tcp/udp port"),
    E(ERR_AUTH_FAIL, "authentication failed"),
    E(ERR_NO_SITE, "does not belong to any site"),
    E(ERR_MAC_MISMATCH, "mac is not the right one"),
    E(ERR_INPUT_LEN, "invalid packet length"),
    E(ERR_INVALID_CMD, "invalid request command"),
    E(ERR_ATTRIBUTE, "missing required attributes"),
    E(ERR_EXIST, " entry exists already"),
    E(ERR_RANDOM_STRING, "ERR_RANDOM_STRING"),
    E(ERR_MISMATCH, "encoded string mismatches"),
    E(ERR_TO_CLOSE, "error to close connection"),
    E(ERR_CONNECT_SERVER, ""),
    E(ERR_NO_INTERNET, ""),
    E(ERR_INVALID_CREDENTIAL, ""),
    E(ERR_BUFFER_LEN, ""),
    E(ERR_GET_DEVICE_INFO, "error to collect device information"),
};

error_set buff_errorstr[] = {
    E(ERR_BUFF_INVALID, "invalid parameter"),
    E(ERR_BUFF_OUTOFMEM, "not enough memory"),
    E(ERR_BUFF_SHUFFLE_FAILED, "memory moving failed"),
    E(ERR_BUFF_ALLOC_FAILED, "allocating memory failed")
};

#ifdef DEBUG
error_set sock_errorstr[] = {
    E(ERR_SOCK_BADTYPE, "invalid socket type"),
    E(ERR_SOCK_BADSOCK, "invalid socket"),
    E(ERR_SOCK_TIMEOUT, "request timeout"),
    E(ERR_SOCK_REFUSED, "request is refused"),
    E(ERR_SOCK_NOCONN, "connection is not available"),
    E(ERR_SOCK_NOBIND, "binding operation failed"),
    E(ERR_SOCK_BADADDR, "invalid socket addr"),
    E(ERR_SOCK_NOLISTEN, "listening operation failed"),
    E(ERR_SOCK_NOACCEPT, "accepting operation failed"),
    E(ERR_SOCK_WOULDBLOCK, "current operation would be blocked for a while"),
    E(ERR_SOCK_BADOPT, "invalid socket operation"),
    E(ERR_SOCK_BADFD, "invalid file description"),
    E(ERR_SOCK_INVAL, "invalid parameter"),
    E(ERR_SOCK_UNCONN, "connection not available"),
    E(ERR_SOCK_INPROGRESS, "operation is working"),
    E(ERR_SOCK_RECV, "receiving operation failed"),
    E(ERR_SOCK_SEND, "sending operation failed"),
    E(ERR_SOCK_OUTBOUND, ""),
    E(ERR_SOCK_STOP, "socket thread stopped"),
};
#else
error_set sock_errorstr[] = {};
#endif

static const char * _strerror(error_set *set, int set_size, int e){
    int i = 0;
    for(; i < set_size; i++){
        if(e == set[i].code)
            return set[i].msg;
    }
    sprintf(unknown_error, "%s: %d", (char*)strerror (e), e);
    return unknown_error;
}

const char* get_strerror(int e){
    error_set *set = 0;
    int set_size = 0;

    /*
     * TODO
     *  let string query more efficiently
     */
    if(e == S_OK){
        return "OK";
    }else if(e > ERR_GENERIC_END){
        set_size = ARRAY_SIZE(generic_errorstr);
        set = generic_errorstr;
    }else if(e > ERR_BUFF_END){
        set_size = ARRAY_SIZE(buff_errorstr);
        set = buff_errorstr;
    }else if(e > ERR_SOCK_END){
        set_size = ARRAY_SIZE(sock_errorstr);
        set = sock_errorstr;
    }else{
        sprintf(unknown_error, "%s: %d", (char*)strerror (e), e);
        return unknown_error;       
    }

    return _strerror(set, set_size, e);
}
