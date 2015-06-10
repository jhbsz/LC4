/*
 * NVRAM variable manipulation (common)
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id$
 */

#define ezpacket

#include <linux/mtd/mtd.h>
#include "osl.h"
#include "typedefs.h"
#include "bcmutils.h"
#include "bcmnvram.h"
#include "bcmendian.h"
#if defined(RUSS)
#include "sbsdram.h"
#endif /* RUSS */

extern struct nvram_tuple * BCMINIT(_nvram_realloc)(struct nvram_tuple *t, const char *name, const char *value);
extern void BCMINIT(_nvram_free)(struct nvram_tuple *t);
extern int BCMINIT(_nvram_read)(struct mtd_info *tmp_mtd, void *buf);

char * BCMINIT(_nvram_get)(const char *name);
int BCMINIT(_nvram_set)(const char *name, const char *value);
int BCMINIT(_nvram_unset)(const char *name);
int BCMINIT(_nvram_getall)(char *buf, int count);
int BCMINIT(_nvram_commit)(struct nvram_header *header);
int BCMINIT(_nvram_init)(struct mtd_info *tmp_mtd, int failsafe);
void BCMINIT(_nvram_exit)(void);

static struct nvram_tuple * BCMINITDATA(nvram_hash)[257];
static struct nvram_tuple * nvram_dead;

/* Free all tuples. Should be locked. */
static void  
BCMINITFN(nvram_free)(void)
{
	uint i;
	struct nvram_tuple *t, *next;

	/* Free hash table */
	for (i = 0; i < ARRAYSIZE(BCMINIT(nvram_hash)); i++) {
		for (t = BCMINIT(nvram_hash)[i]; t; t = next) {
			next = t->next;
			BCMINIT(_nvram_free)(t);
		}
		BCMINIT(nvram_hash)[i] = NULL;
	}

	/* Free dead table */
	for (t = nvram_dead; t; t = next) {
		next = t->next;
		BCMINIT(_nvram_free)(t);
	}
	nvram_dead = NULL;

	/* Indicate to per-port code that all tuples have been freed */
	BCMINIT(_nvram_free)(NULL);
}

/* String hash */
static INLINE uint
hash(const char *s)
{
	uint hash = 0;

	while (*s)
		hash = 31 * hash + *s++;

	return hash;
}

/* (Re)initialize the hash table. Should be locked. */
static int 
BCMINITFN(nvram_rehash)(struct nvram_header *header)
{
#ifdef RUSS
	char buf[] = "0xXXXXXXXX";
#endif
	char *name, *value, *end, *eq;

	/* (Re)initialize hash table */
	BCMINIT(nvram_free)();

	/* Parse and set "name=value\0 ... \0\0" */
	name = (char *) &header[1];
	end = (char *) header + NVRAM_SPACE - 2;
	end[0] = end[1] = '\0';
	for (; *name; name = value + strlen(value) + 1) {
		if (!(eq = strchr(name, '=')))
			break;
		*eq = '\0';
		value = eq + 1;
		BCMINIT(_nvram_set)(name, value);
		*eq = '=';
	}

#ifdef RUSS
	/* These are platform-dependent */

	/* Set special SDRAM parameters */
	if (!BCMINIT(_nvram_get)("sdram_init")) {
		sprintf(buf, "0x%04X", (uint16)(ltoh32(header->crc_ver_init) >> 16));
		BCMINIT(_nvram_set)("sdram_init", buf);
	}
	if (!BCMINIT(_nvram_get)("sdram_config")) {
		sprintf(buf, "0x%04X", (uint16)(ltoh32(header->config_refresh) & 0xffff));
		BCMINIT(_nvram_set)("sdram_config", buf);
	}
	if (!BCMINIT(_nvram_get)("sdram_refresh")) {
		sprintf(buf, "0x%04X", (uint16)((ltoh32(header->config_refresh) >> 16) & 0xffff));
		BCMINIT(_nvram_set)("sdram_refresh", buf);
	}
	if (!BCMINIT(_nvram_get)("sdram_ncdl")) {
		sprintf(buf, "0x%08X", ltoh32(header->config_ncdl));
		BCMINIT(_nvram_set)("sdram_ncdl", buf);
	}
#endif /* RUSS */

	return 0;
}

/* Get the value of an NVRAM variable. Should be locked. */
char * 
BCMINITFN(_nvram_get)(const char *name)
{
	uint i;
	struct nvram_tuple *t;
	char *value;

	if (!name)
		return NULL;

	/* Hash the name */
	i = hash(name) % ARRAYSIZE(BCMINIT(nvram_hash));

	/* Find the associated tuple in the hash table */
	for (t = BCMINIT(nvram_hash)[i]; t && strcmp(t->name, name); t = t->next);

	value = t ? t->value : NULL;

	return value;
}

/* Get the value of an NVRAM variable. Should be locked. */
int 
BCMINITFN(_nvram_set)(const char *name, const char *value)
{
	uint i;
	struct nvram_tuple *t, *u, **prev;

	/* Hash the name */
	i = hash(name) % ARRAYSIZE(BCMINIT(nvram_hash));

	/* Find the associated tuple in the hash table */
	for (prev = &BCMINIT(nvram_hash)[i], t = *prev; t && strcmp(t->name, name); prev = &t->next, t = *prev);

	/* (Re)allocate tuple */
	if (!(u = BCMINIT(_nvram_realloc)(t, name, value)))
		return -12; /* -ENOMEM */

	/* Value reallocated */
	if (t && t == u)
		return 0;

	/* Move old tuple to the dead table */
	if (t) {
		*prev = t->next;
		t->next = nvram_dead;
		nvram_dead = t;
	}

	/* Add new tuple to the hash table */
	u->next = BCMINIT(nvram_hash)[i];
	BCMINIT(nvram_hash)[i] = u;

	return 0;
}

/* Unset the value of an NVRAM variable. Should be locked. */
int 
BCMINITFN(_nvram_unset)(const char *name)
{
	uint i;
	struct nvram_tuple *t, **prev;

	if (!name)
		return 0;

	/* Hash the name */
	i = hash(name) % ARRAYSIZE(BCMINIT(nvram_hash));

	/* Find the associated tuple in the hash table */
	for (prev = &BCMINIT(nvram_hash)[i], t = *prev; t && strcmp(t->name, name); prev = &t->next, t = *prev);

	/* Move it to the dead table */
	if (t) {
		*prev = t->next;
		t->next = nvram_dead;
		nvram_dead = t;
	}

	return 0;
}

/* Get all NVRAM variables. Should be locked. */
int 
BCMINITFN(_nvram_getall)(char *buf, int count)
{
	uint i;
	struct nvram_tuple *t;
	int len = 0;

	bzero(buf, count);

	/* Write name=value\0 ... \0\0 */
	for (i = 0; i < ARRAYSIZE(BCMINIT(nvram_hash)); i++) {
		for (t = BCMINIT(nvram_hash)[i]; t; t = t->next) {
			if ((count - len) > (strlen(t->name) + 1 + strlen(t->value) + 1))
				len += sprintf(buf + len, "%s=%s", t->name, t->value) + 1;
			else
				break;
		}
	}

	return 0;
}

/* Validate the NVRAM header and checksum. */
int 
BCMINITFN(_nvram_valid)(struct nvram_header *header, int failsafe)
{
	uint8 crc;

	/* Check the magic value. */
	if (header->magic != htol32(NVRAM_MAGIC)) {
		printk("NVRAM magic value invalid\n");
		return 1;
	}
	/* Check the checksum. The least significant byte. */

	/* Calculate the checksum. */
	/* Little-endian CRC8 over the last 11 bytes of the header */
	/* crc = hndcrc8(((char *) &header) + 9, sizeof(struct nvram_header) - 9, CRC8_INIT_VALUE); */
	/* Continue CRC8 over data bytes */
	/* crc = hndcrc8((char *) &header[1], ltoh32(header->len) - sizeof(struct nvram_header), crc); */
    /* 
     * EZP: For some reason, the _nvram_valid cannot read the right value if
     * we take the header into account. 
     */
	crc = hndcrc8((char *) &header[1], ltoh32(header->len) - sizeof(struct nvram_header), CRC8_INIT_VALUE);

	if (!failsafe && crc != (uint8) ltoh32(header->crc_ver_init)) {
		printk("NVRAM CRC invalid\n");
		return 1;
	}

	return 0;
}

/* Regenerate NVRAM. Should be locked. */
int
BCMINITFN(_nvram_commit)(struct nvram_header *header)
{
	char *init, *config, *refresh, *ncdl;
	char *ptr, *end;
	int i;
	struct nvram_tuple *t;
	uint8 crc;

	/* Regenerate header */
	header->magic = htol32(NVRAM_MAGIC);
	header->crc_ver_init = htol32((NVRAM_VERSION << 8));
	if (!(init = BCMINIT(_nvram_get)("sdram_init")) ||
	    !(config = BCMINIT(_nvram_get)("sdram_config")) ||
	    !(refresh = BCMINIT(_nvram_get)("sdram_refresh")) ||
	    !(ncdl = BCMINIT(_nvram_get)("sdram_ncdl"))) {
#if defined(RUSS)
		header->crc_ver_init |= htol32(SDRAM_INIT << 16);
		header->config_refresh = htol32(SDRAM_CONFIG);
		header->config_refresh |= htol32(SDRAM_REFRESH << 16);
		header->config_ncdl = htol32(0);
#else
		header->crc_ver_init |= htol32(0) ;
		header->config_refresh = htol32(0) ;
		header->config_ncdl |= htol32(0);
#endif
	} else {
		header->crc_ver_init |= htol32((bcm_strtoul(init, NULL, 0) & 0xffff) << 16);
		header->config_refresh = htol32(bcm_strtoul(config, NULL, 0) & 0xffff);
		header->config_refresh |= htol32((bcm_strtoul(refresh, NULL, 0) & 0xffff) << 16);
		header->config_ncdl = htol32(bcm_strtoul(ncdl, NULL, 0));
	}

	/* Clear data area */
	ptr = (char *) header + sizeof(struct nvram_header);
	bzero(ptr, NVRAM_SPACE - sizeof(struct nvram_header));

	/* Leave space for a double NUL at the end */
	end = (char *) header + NVRAM_SPACE - 2;

	/* Write out all tuples */
	for (i = 0; i < ARRAYSIZE(BCMINIT(nvram_hash)); i++) {
		for (t = BCMINIT(nvram_hash)[i]; t; t = t->next) {
			if ((ptr + strlen(t->name) + 1 + strlen(t->value) + 1) > end)
				break;
			ptr += sprintf(ptr, "%s=%s", t->name, t->value) + 1;
		}
	}

	/* End with a double NUL */
	ptr += 2;

	/* Set new length */
	header->len = htol32(ROUNDUP(ptr - (char *) header, 4));

	/* Little-endian CRC8 over the last 11 bytes of the header */
	/* EZP: nvram_header is in the little endian format.
	struct nvram_header tmp;
        tmp.crc_ver_init = htol32(header->crc_ver_init);
	tmp.config_refresh = htol32(header->config_refresh);
	tmp.config_ncdl = htol32(header->config_ncdl);
	*/

	/* crc = hndcrc8(((char *) &header) + 9, sizeof(struct nvram_header) - 9, CRC8_INIT_VALUE); */
	/* Continue CRC8 over data bytes */
	/* crc = hndcrc8((char *) &header[1], ltoh32(header->len) - sizeof(struct nvram_header), crc); */
	crc = hndcrc8((char *) &header[1], ltoh32(header->len) - sizeof(struct nvram_header), CRC8_INIT_VALUE);

	/* Set new CRC8 ("OR" the least significant byte) */
	header->crc_ver_init |= htol32(crc);

	/* Reinitialize hash table */
	return BCMINIT(nvram_rehash)(header);
}

/* Initialize hash table. Should be locked. */
int 
BCMINITFN(_nvram_init)(struct mtd_info *tmp_mtd, int failsafe)
{
	struct nvram_header *header;
	int ret;
	void *osh;

	/* get kernel osl handler */
	osh = osl_attach(NULL);

	if (!(header = (struct nvram_header *) MALLOC(osh, NVRAM_SPACE))) {
		printf("nvram_init: out of memory, malloced %d bytes\n", MALLOCED(osh));
		return -12; /* -ENOMEM */
	}

	/* Read the header first. Validate the header.*/
	if ((ret = BCMINIT(_nvram_read)(tmp_mtd, header)) == 0 &&
	    (ret = BCMINIT(_nvram_valid)(header, failsafe)) == 0)
		BCMINIT(nvram_rehash)(header);

	MFREE(osh, header, NVRAM_SPACE);
	return ret;
}

/* Free hash table. Should be locked. */
void 
BCMINITFN(_nvram_exit)(void)
{
	BCMINIT(nvram_free)();
}

