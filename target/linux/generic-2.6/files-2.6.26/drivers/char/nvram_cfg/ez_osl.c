/*
 * Linux OS Independent Layer
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

#define LINUX_OSL

#include <linux/module.h>
#include <linux/delay.h>
#include "linuxver.h"
#include "ez_osl.h"
#include "typedefs.h"
#include "bcmendian.h"
#include "bcmutils.h"

#define OS_HANDLE_MAGIC		0x1234abcd
#define BCM_MEM_FILENAME_LEN 	24

typedef struct bcm_mem_link {
    struct bcm_mem_link *prev;
    struct bcm_mem_link *next;
    uint    size;
    int     line;
    char    file[BCM_MEM_FILENAME_LEN];
} bcm_mem_link_t;

typedef struct os_handle {
    uint magic;
    void *pdev;
    uint malloced;
    uint failed;
    bcm_mem_link_t *dbgmem_list;
} os_handle_t;



void *
osl_attach(void *pdev)
{
	os_handle_t *osh;

	osh = kmalloc(sizeof(os_handle_t), GFP_ATOMIC);
	ASSERT(osh);

	osh->magic = OS_HANDLE_MAGIC;
	osh->malloced = 0;
	osh->failed = 0;
	osh->dbgmem_list = NULL;
	osh->pdev = pdev;

	return osh;
}

void
osl_detach(void *osh)
{
	ASSERT((osh && (((os_handle_t *)osh)->magic == OS_HANDLE_MAGIC)));
	kfree(osh);
}


void*
osl_malloc(void *osh, uint size)
{
	os_handle_t *h = (os_handle_t *)osh;
	void *addr;

	ASSERT((h && (h->magic == OS_HANDLE_MAGIC)));
	h->malloced += size;
	addr = kmalloc(size, GFP_ATOMIC);
	if (!addr)
		h->failed++;
	return (addr);
}

void
osl_mfree(void *osh, void *addr, uint size)
{
	os_handle_t *h = (os_handle_t *)osh;

	ASSERT((h && (h->magic == OS_HANDLE_MAGIC)));
	h->malloced -= size;
	kfree(addr);
}

uint
osl_malloced(void *osh)
{
	os_handle_t *h = (os_handle_t *)osh;

	ASSERT((h && (h->magic == OS_HANDLE_MAGIC)));
	return (h->malloced);
}

uint osl_malloc_failed(void *osh)
{
	os_handle_t *h = (os_handle_t *)osh;

	ASSERT((h && (h->magic == OS_HANDLE_MAGIC)));
	return (h->failed);
}

