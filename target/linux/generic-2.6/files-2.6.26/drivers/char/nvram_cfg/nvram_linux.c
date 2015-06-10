/*
 * NVRAM variable manipulation (Linux kernel half)
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

#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/bootmem.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mtd/mtd.h>
#include <linux/autoconf.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "typedefs.h"
#include "bcmendian.h"
#include "bcmnvram.h"
#include "bcmutils.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
#include <linux/wrapper.h> 
#else
#include <linux/page-flags.h> 
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 16))
#include <linux/device.h>
#include <asm/cacheflush.h>
#endif

#if !defined(NVRAMDBG)
static int nvram_dbg = 0 ;
#define RUSSDBG(args...)
#else
static int nvram_dbg = 1 ;
#define RUSSDBG(fmt...)	if(en){ printk("%s: ", __func__); printk(fmt);}
#endif /* #if defined(RUSS) */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
MODULE_PARM(nvram_dbg,"i");
MODULE_PARM_DESC(nvram_dbg, "Turn on/off debugging messages") ;
#else
module_param(nvram_dbg, int, 0);
MODULE_PARM_DESC(nvram_dbg, "Turn on/off debugging messages") ;
#endif

/* In BSS to minimize text size and page aligned so it can be mmap()-ed */
static char nvram_buf[NVRAM_SPACE] __attribute__((aligned(PAGE_SIZE)));

#define early_nvram_get(name) nvram_get(name)

extern char * _nvram_get(const char *name);
extern int _nvram_set(const char *name, const char *value);
extern int _nvram_unset(const char *name);
extern int _nvram_getall(char *buf, int count);
extern int _nvram_commit(struct nvram_header *header);
extern int _nvram_init(struct mtd_info *tmp_mtd, int failsafe);
extern void _nvram_exit(void);

/* Globals */
static spinlock_t nvram_lock = SPIN_LOCK_UNLOCKED;
static struct semaphore nvram_sem;
static unsigned long nvram_offset = 0;
static int nvram_major = -1;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
static devfs_handle_t nvram_handle = NULL;
#else
static struct class *nvram_class = NULL;
#endif
static struct mtd_info *nvram_mtd = NULL, *nvram_backup_mtd = NULL, *tmp_mtd;

int
_nvram_read(struct mtd_info *tmp_mtd, char *buf)
{
	size_t len;

	if (!tmp_mtd ||
	    tmp_mtd->read(tmp_mtd, tmp_mtd->size - NVRAM_SPACE, NVRAM_SPACE, &len, buf) ||
	    len != NVRAM_SPACE) {
		/* Maybe we can recover some data from early initialization */
		memcpy(buf, nvram_buf, NVRAM_SPACE);
	}

	return 0;
}

struct nvram_tuple *
_nvram_realloc(struct nvram_tuple *t, const char *name, const char *value)
{
	if ((nvram_offset + strlen(value) + 1) > NVRAM_SPACE)
		return NULL;

	if (!t) {
		if (!(t = kmalloc(sizeof(struct nvram_tuple) + strlen(name) + 1, GFP_ATOMIC)))
			return NULL;

		/* Copy name */
		t->name = (char *) &t[1];
		strcpy(t->name, name);

		t->value = NULL;
	}

	/* Copy value */
	if (!t->value || strcmp(t->value, value)) {
		t->value = &nvram_buf[nvram_offset];
		strcpy(t->value, value);
		nvram_offset += strlen(value) + 1;
	}

	return t;
}

void
_nvram_free(struct nvram_tuple *t)
{
	if (!t)
		nvram_offset = 0;
	else
		kfree(t);
}

int
nvram_set(const char *name, const char *value)
{
	unsigned long flags;
	int ret;
	struct nvram_header *header;

	// spin_lock_irqsave(&nvram_lock, flags);
	down(&nvram_sem);
	if ((ret = _nvram_set(name, value))) {
		/* Consolidate space and try again */
		if ((header = kmalloc(NVRAM_SPACE, GFP_ATOMIC))) {
			if (_nvram_commit(header) == 0)
				ret = _nvram_set(name, value);
			kfree(header);
		}
	}
	up(&nvram_sem);
	//spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

char *
real_nvram_get(const char *name)
{
	unsigned long flags;
	char *value;

	// spin_lock_irqsave(&nvram_lock, flags);
	down(&nvram_sem);
	value = _nvram_get(name);
	up(&nvram_sem);
	// spin_unlock_irqrestore(&nvram_lock, flags);

	return value;
}

char *
nvram_get(const char *name)
{
	if (nvram_major >= 0)
		return real_nvram_get(name);
	else
		return early_nvram_get(name);
}

int
nvram_unset(const char *name)
{
	unsigned long flags;
	int ret;

	// spin_lock_irqsave(&nvram_lock, flags);
	down(&nvram_sem);
	ret = _nvram_unset(name);
	up(&nvram_sem);
	// spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

static void
erase_callback(struct erase_info *done)
{
	wait_queue_head_t *wait_q = (wait_queue_head_t *) done->priv;
	wake_up(wait_q);
}

int
nvram_mtd_commit(struct mtd_info *tmp_mtd)
{
	char *buf;
	int ret;
	struct nvram_header *header;
	size_t erasesize, len;
	unsigned int i;
	unsigned long flags;
	u_int32_t offset;
	DECLARE_WAITQUEUE(wait, current);
	wait_queue_head_t wait_q;
	struct erase_info erase;

	/* Backup sector blocks to be erased */
	erasesize = ROUNDUP(NVRAM_SPACE, tmp_mtd->erasesize);
	if (!(buf = kmalloc(erasesize, GFP_KERNEL))) {
		printk("%s: out of memory\n", __func__);
		ret = -ENOMEM;
        goto done;
	}

	/* Find out the offset and the header position in a MTD partition. */
	if ((i = erasesize - NVRAM_SPACE) > 0) {
		offset = tmp_mtd->size - erasesize;
		len = 0;
		ret = tmp_mtd->read(tmp_mtd, offset, i, &len, buf);
		if (ret || len != i) {
			printk("%s: read error ret = %d, len = %d/%d\n", __func__, ret, len, i);
			ret = -EIO;
            goto done;
		}
		header = (struct nvram_header *)(buf + i);
	} else {
		offset = tmp_mtd->size - NVRAM_SPACE;
		header = (struct nvram_header *)buf;
	}

	/* Regenerate NVRAM */
	// spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_commit(header);
	// spin_unlock_irqrestore(&nvram_lock, flags);
	if (ret)
        goto done;

	/* Erase sector blocks */
	init_waitqueue_head(&wait_q);
	for (; offset < tmp_mtd->size - NVRAM_SPACE + ltoh32(header->len); offset += tmp_mtd->erasesize) {
		erase.mtd = tmp_mtd;
		erase.addr = offset;
		erase.len = tmp_mtd->erasesize;
		erase.callback = erase_callback;
		erase.priv = (u_long) &wait_q;

		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&wait_q, &wait);

		/* Unlock sector blocks */
		if (tmp_mtd->unlock)
			tmp_mtd->unlock(tmp_mtd, offset, tmp_mtd->erasesize);

		if ((ret = tmp_mtd->erase(tmp_mtd, &erase))) {
			set_current_state(TASK_RUNNING);
			remove_wait_queue(&wait_q, &wait);
			printk("%s: erase error\n", __func__);
            goto done;
		}

		/* Wait for erase to finish */
		schedule();
		remove_wait_queue(&wait_q, &wait);
	}

	/* Write partition up to end of data area */
	offset = tmp_mtd->size - erasesize;
	i = erasesize - NVRAM_SPACE + ltoh32(header->len);
	ret = tmp_mtd->write(tmp_mtd, offset, i, &len, buf);
	if (ret || len != i) {
		printk("%s: write error\n", __func__);
		ret = -EIO;
		goto done;
	}

	offset = tmp_mtd->size - erasesize;
	ret = tmp_mtd->read(tmp_mtd, offset, 4, &len, buf);

done:
	if (buf) {
		kfree(buf);
	}
	return ret;
}

int
nvram_commit(void)
{
	int ret;

	if (!nvram_mtd) {
		printk("nvram_commit: NVRAM not found\n");
		return -ENODEV;
	}

	if (!nvram_backup_mtd) {
		/* Nice to have. If none, live with it :) */
		printk("nvram_commit: NVRAM backup not found\n");
	}

	if (in_interrupt()) {
		printk("nvram_commit: not committing in interrupt\n");
		return -EINVAL;
	}

	down(&nvram_sem);
	// spin_lock(&nvram_lock);
	/* Backup NVRAM partition. */
	if (nvram_backup_mtd && (ret = nvram_mtd_commit(nvram_backup_mtd))) {
		printk("%s: nvram_backup_mtd commit failed\n", __func__);
	}
	/* Primary NVRAM partition. */
	if ((ret = nvram_mtd_commit(nvram_mtd))) {
		printk("%s: nvram_mtd commit failed\n", __func__);
	}
	/* XXX: ret is overwritten in the second nvram_mtd_commit(). */
	up(&nvram_sem);
	// spin_unlock(&nvram_lock);
	return ret;
}

int
nvram_getall(char *buf, int count)
{
	unsigned long flags;
	int ret;

	// spin_lock_irqsave(&nvram_lock, flags);
	down(&nvram_sem);
	ret = _nvram_getall(buf, count);
	up(&nvram_sem);
	// spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

EXPORT_SYMBOL(nvram_get);
EXPORT_SYMBOL(nvram_getall);
EXPORT_SYMBOL(nvram_set);
EXPORT_SYMBOL(nvram_unset);
EXPORT_SYMBOL(nvram_commit);

/* User mode interface below */

static ssize_t
dev_nvram_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	char tmp[100], *name = tmp, *value;
	ssize_t ret, vallen;
	unsigned long off;

	if (count > sizeof(tmp)) {
		if (!(name = kmalloc(count, GFP_KERNEL)))
			return -ENOMEM;
	}

	if (copy_from_user(name, buf, count)) {
		ret = -EFAULT;
		goto done;
	}

	if (*name == '\0') {
		/* Get all variables */
		ret = nvram_getall(name, count);
		if (ret == 0) {
			if (copy_to_user(buf, name, count)) {
				ret = -EFAULT;
				goto done;
			}
			ret = count;
		}
	} else {
		if (!(value = nvram_get(name))) {
			ret = 0;
			goto done;
		}

		RUSSDBG("nvram_buf(%p)='%s'\n", nvram_buf, nvram_buf)
		RUSSDBG("value(%8p)='%s'\n", value, value);

		/* Provide the offset into mmap() space */
		off = (unsigned long) value - (unsigned long) nvram_buf;
		RUSSDBG("off='%lu'\n", off);

#ifndef RUSS
		if (put_user(off, (unsigned long *) buf)) {
			ret = -EFAULT;
			goto done;
		}
		ret = sizeof(unsigned long);
#else
		vallen=strlen(value) + 1 ;
		if(vallen > count)
			vallen=count ;
		copy_to_user(buf, value, vallen) ;
		ret = vallen ;
#endif

	}

	flush_cache_all();	
 
done:
	if (name != tmp)
		kfree(name);

	return ret;
}

static ssize_t
dev_nvram_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	char tmp[100], *name = tmp, *value;
	ssize_t ret;

	if (count > sizeof(tmp)) {
		if (!(name = kmalloc(count, GFP_KERNEL)))
			return -ENOMEM;
	}

	if (copy_from_user(name, buf, count)) {
		ret = -EFAULT;
		goto done;
	}

	value = name;
	name = strsep(&value, "=");
    /* EZP: if NULL value, unset. */
	if (value && *value)
		ret = nvram_set(name, value) ? : count;
	else
		ret = nvram_unset(name) ? : count;

 done:
	if (name != tmp)
		kfree(name);

	return ret;
}	

static int
dev_nvram_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	if (cmd != NVRAM_MAGIC)
		return -EINVAL;
	return nvram_commit();
}

#ifndef RUSS
static int
dev_nvram_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = virt_to_phys(nvram_buf);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
	if (remap_page_range(vma->vm_start, offset, vma->vm_end-vma->vm_start,
			     vma->vm_page_prot))
#else
	if (remap_pfn_range(vma, vma->vm_start, offset >> PAGE_SHIFT, 
                vma->vm_end-vma->vm_start, vma->vm_page_prot))
#endif
		return -EAGAIN;

	RUSSDBG(1, "nvram_buf(%p)='%s'\n", nvram_buf, nvram_buf)
	RUSSDBG(1, "vma->vm_start=%08lX, offset=%08lX, vma->vm_end=%08lX\n",
		vma->vm_start, offset, vma->vm_end)

	return 0;
}
#endif

static int
dev_nvram_open(struct inode *inode, struct file * file)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
	MOD_INC_USE_COUNT;
#else
    try_module_get(THIS_MODULE);
#endif
	return 0;
}

static int
dev_nvram_release(struct inode *inode, struct file * file)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
	MOD_DEC_USE_COUNT;
#else
    module_put(THIS_MODULE);
#endif
	return 0;
}

static struct file_operations dev_nvram_fops = {
	owner:		THIS_MODULE,
	open:		dev_nvram_open,
	release:	dev_nvram_release,
	read:		dev_nvram_read,
	write:		dev_nvram_write,
	ioctl:		dev_nvram_ioctl,
#ifndef RUSS
	mmap:		dev_nvram_mmap,
#endif
};

static void
dev_nvram_exit(void)
{
	int order = 0;
	struct page *page, *end;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
	if (nvram_handle)
		devfs_unregister(nvram_handle);
#else
/* Unregister. */
    device_destroy(nvram_class, MKDEV(nvram_major, 0));
    class_destroy(nvram_class);
#endif

	if (nvram_major >= 0)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
		devfs_unregister_chrdev(nvram_major, "nvram");
#else
    unregister_chrdev(nvram_major, "nvram");
#endif

	if (nvram_mtd)
		put_mtd_device(nvram_mtd);

	if (nvram_backup_mtd)
		put_mtd_device(nvram_backup_mtd);

	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
		mem_map_unreserve(page);
#else
        ClearPageReserved(page);
#endif

	_nvram_exit();
}

static int __init
dev_nvram_init(void)
{
	int order = 0, ret = 0;
	struct page *page, *end;
	unsigned int i;

	/* Allocate and reserve memory to mmap() */
	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
		mem_map_reserve(page);
#else
        SetPageReserved(page);
#endif
    }

#if defined(CONFIG_MTD) || defined(CONFIG_MTD_MODULE)
	/* Find associated MTD device */
	for (i = 0; i < MAX_MTD_DEVICES; i++) {
		tmp_mtd = get_mtd_device(NULL, i);
        if (IS_ERR(tmp_mtd)) {
            break;
        }
		if (tmp_mtd) {
			if (!strcmp(tmp_mtd->name, "nvram") &&
			    tmp_mtd->size >= NVRAM_SPACE)
				nvram_mtd = tmp_mtd;
			else if (!strcmp(tmp_mtd->name, "nvram_backup") &&
			    tmp_mtd->size >= NVRAM_SPACE)
				nvram_backup_mtd = tmp_mtd;
			else
				put_mtd_device(tmp_mtd);
		}
	}
	if(!nvram_mtd) {
		printk("%s: cannot locate nvram_mtd\n", __func__);
        goto err;
    }
	if(!nvram_backup_mtd) {
		printk("%s: cannot locate nvram_backup_mtd\n", __func__);
    }
#else
#error nvram_cfg needs CONFIG_MTD
#endif

	/* Initialize hash table lock */
	spin_lock_init(&nvram_lock);

	/* Initialize commit semaphore */
	init_MUTEX(&nvram_sem);

	/* Initialize hash table */
	if ((ret = _nvram_init(nvram_mtd, 0))) {
		printk("%s: invalid nvram_mtd\n", __func__);
		if (nvram_backup_mtd) {
            if ((ret = _nvram_init(nvram_backup_mtd, 0))) {
                /* The worst case. Read whatever nvram has (also failover). */
                printk("%s: invalid nvram_backup_mtd\n", __func__);
                printk("%s: access nvram_mtd (failsafe)\n", __func__);
                _nvram_init(nvram_mtd, 1);
            } else {
                /* Recover nvram_mtd. */
                printk("%s: access nvram_backup_mtd\n", __func__);
                printk("%s: recover nvram_mtd\n", __func__);
                if ((ret = nvram_mtd_commit(nvram_mtd))) {
                    printk("%s: nvram_mtd commit failed\n", __func__);
                }
            }
        } else {
                printk("%s: no nvram_backup_mtd found\n", __func__);
                printk("%s: access nvram_mtd (failsafe)\n", __func__);
                _nvram_init(nvram_mtd, 1);
        }
	} else {
        /* Try to validate nvram_backup.*/
		printk("%s: access nvram_mtd\n", __func__);
        if (nvram_backup_mtd && (ret = _nvram_init(nvram_backup_mtd, 0))) {
            /* Failed and commit into nvram_backup.*/
            ret = _nvram_init(nvram_mtd, 0);
            printk("%s: recover nvram_backup_mtd[%d]\n", __func__, ret);
            if ((ret = nvram_mtd_commit(nvram_backup_mtd))) {
                printk("%s: nvram_backup_mtd commit failed\n", __func__);
            }
        }
    }

	/* Register char device */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
    if ((nvram_major = devfs_register_chrdev(0, "nvram", &dev_nvram_fops)) < 0) {
#else
    /* Register: XXX: */
    if ((nvram_major = register_chrdev(0, "nvram", &dev_nvram_fops)) < 0) {
#endif
        ret = nvram_major;
        goto err;
    }

	RUSSDBG("%s: nvram_major = %d\n", __func__, nvram_major) ;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
	/* Create /dev/nvram handle */
	nvram_handle = devfs_register(NULL, "nvram", DEVFS_FL_NONE, nvram_major, 0,
				      S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP, &dev_nvram_fops, NULL);
#else
    /* Register: XXX: */
    nvram_class = class_create(THIS_MODULE, "nvram_class");
    device_create(nvram_class, NULL, MKDEV(nvram_major, 0), "nvram");
#endif

#if defined(RUSS)
	/* Set the SDRAM NCDL value into NVRAM if not already done */
	if (getintvar(NULL, "sdram_ncdl") == 0) {
		unsigned int ncdl;
		char buf[] = "0x00000000";

		if ((ncdl = sb_memc_get_ncdl(sbh))) {
			sprintf(buf, "0x%08x", ncdl);
			nvram_set("sdram_ncdl", buf);
			nvram_commit();
		}
	}
#endif /* RUSS */
	RUSSDBG(1, "nvram_buf=%8p\n", nvram_buf)

	return 0;

 err:
	dev_nvram_exit();
	return ret;
}

module_init(dev_nvram_init);
module_exit(dev_nvram_exit);

MODULE_DESCRIPTION("EZ Packet NVRAM Module");
MODULE_AUTHOR("CK Wang <ckjwang@ezpacket.com>");
MODULE_SUPPORTED_DEVICE("NVRAM");
MODULE_VERSION(MOD_VERSION);
MODULE_LICENSE("GPL");
