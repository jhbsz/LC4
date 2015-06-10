/*
 * buff.h
 *
 *  Created on: Sep 30, 2012
 *      Author: ch
 */

#ifndef BUFF_H_
#define BUFF_H_

#include <stdint.h>

typedef struct
{
    size_t len;        // current length of buffer (used bytes)
    size_t limit;      // maximum length of buffer (allocated)
    uint8_t *data;     // insert bytes here
} buf_t;


buf_t * buf_size(buf_t *buf, size_t len);
void buf_push(buf_t *buf, uint8_t c);
int buf_concat(buf_t *buf, uint8_t *data, size_t len);
char * buf_tostr(buf_t *buf);
void buf_destory(buf_t *buf);

#endif /* BUFF_H_ */
