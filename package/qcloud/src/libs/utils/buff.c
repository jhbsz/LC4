/*
 * buff.c
 *
 */

#include <stdlib.h>
#include <string.h>
#include "buff.h"

buf_t * buf_size(buf_t *buf, size_t len)
{
  uint8_t *buff;

  if (buf == NULL) {
    buf = calloc(1, sizeof(buf_t));
    if(!buf){
      return buf;
    }
    buf->data = NULL;
    buf->len = 0;
  }

  /*
   * If ptr is NULL, the call is equivalent to malloc(size); if size is equal to
   * zero, the call is equivalent to free(ptr).
   */
  buff = realloc(buf->data, len);
  if(buff == NULL){
    /*
     * If realloc() fails the original block is left untouched; it is not freed or moved.
     */
    return buf;
  }
  buf->data = buff;
  if (buf->len > len) buf->len = len;
  buf->limit = len;

  return buf;
}

int buf_concat(buf_t *dst, uint8_t *src, size_t len)
{
  size_t i = 0;

  if(dst){
#if 1
    i = dst->limit - dst->len;
    if (i < len) {
      // resize buffer.
      dst = buf_size(dst, dst->limit + len);
      if (dst->limit - dst->len < len) {
        //QNAP_LOG_ERR_NL("allocate buff failed");
        return -1;
      }
    }
#endif
    len = ((dst->limit - dst->len) < len) ? (dst->limit - dst->len) : len;
    for (i = 0; i < len; i++)
      dst->data[dst->len++] = src[i];
    return 0;
  }
  return -1;
}

char * buf_tostr(buf_t *buf)
{
  char *str = malloc(buf->len + 1);
  memcpy(str, buf->data, buf->len);
  str[buf->len] = '\0';
  return str;
}

void buf_destory(buf_t *buf)
{
  if(buf){
    if(buf->data)
      free(buf->data);
    free(buf);
  }
}
