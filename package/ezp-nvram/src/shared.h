#ifndef __SHARED_H__
#define __SHARED_H__

#include <netinet/in.h>
#include <stdint.h>

#define Y2K			946684800UL		// seconds since 1970

#define ASIZE(array)	(sizeof(array) / sizeof(array[0]))

// process.c
extern char *psname(int pid, char *buffer, int maxlen);
extern int pidof(const char *name);
extern int killall(const char *name, int sig);

// files.c
#define FW_CREATE	0
#define FW_APPEND	1
#define FW_NEWLINE	2

extern unsigned long f_size(const char *path);
extern int f_exists(const char *file);
extern int f_read(const char *file, void *buffer, int max);												// returns bytes read
extern int f_write(const char *file, const void *buffer, int len, unsigned flags, unsigned cmode);		//
extern int f_read_string(const char *file, char *buffer, int max);										// returns bytes read, not including term; max includes term
extern int f_write_string(const char *file, const char *buffer, unsigned flags, unsigned cmode);		//
extern int f_read_alloc(const char *path, char **buffer, int max);
extern int f_read_alloc_string(const char *path, char **buffer, int max);


// base64.c
extern int base64_encode(const unsigned char *in,  unsigned long inlen,
        unsigned char *out, unsigned long *outlen);
extern int base64_decode(const unsigned char *in,  unsigned long inlen,
        unsigned char *out, unsigned long *outlen);

// strings.c
extern int find_word(const char *buffer, const char *word);

// crc32.o
extern unsigned long crc32(unsigned long, const unsigned char *, unsigned int);

// nvram_linux.c
extern int nvram_import(char *filename);

// shutils.c
int process_is_running(char *program);
#endif
