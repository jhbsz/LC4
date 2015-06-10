#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "md5.h"
#include "nas_hash.h"

#define NAS_HASH_MD5 1

static void md5_hash (char *in, unsigned char *digest)
{
	MD5_CTX md5ctx;

	MD5Init(&md5ctx);
	MD5Update(&md5ctx, in, strlen(in));
	MD5Final(digest, &md5ctx);
}

static void md5_hash_hex (char *in, char *out, int size)
{
	int i=0, count=0;
	unsigned char digest[16], buf[3];

	md5_hash (in, digest);
	for (i = 0; i < 16; i++, count+=2) {
		if (count > size) break;
		sprintf(buf, "%2x", (int)digest[i]);
		(i == 0)? strcpy(out, (char *)buf): strcat(out, (char *)buf);
	}
	out[size-1] = '\0';
}

static void md5_hash_hex_ex (char *in, char *out, int size)
{
	int i=0, count=0;
	unsigned char digest[16], buf[3];

	md5_hash (in, digest);
	for (i = 0; i < 16; i++, count+=2) {
		if (count > size) break;
		sprintf(buf, "%.2x", (int)digest[i]);
		(i == 0)? strcpy(out, (char *)buf): strcat(out, (char *)buf);
	}
	out[size-1] = '\0';
}

static void md5_hash_ascii (char *in, char *out, int size)
{
	unsigned char digest[16];

	md5_hash (in, digest);
	snprintf(out, size, "%s", digest);
}

/* return hash string in hex */
void NAS_Hash_Hex (char *in, char *out, int o_size, int algm)
{
	if (algm == NAS_HASH_MD5)
		md5_hash_hex(in, out, o_size);
}

/* return hash string in hex */
void NAS_Hash_Hex_Ex (char *in, char *out, int o_size, int algm)
{
	if (algm == NAS_HASH_MD5)
		md5_hash_hex_ex(in, out, o_size);
}

/* return hash string in ascii */
void NAS_Hash_Ascii (char *in, char *out, int o_size, int algm)
{
	if (algm == NAS_HASH_MD5)
		md5_hash_ascii(in, out, o_size);
}

