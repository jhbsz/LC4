/* 
 * Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 *
 * Camelot specific serial flash driver
 *
 * Derived from MTD SPI driver for ST M25Pxx (and similar) serial flash chips
 *
 * Author: Mike Lavender, mike@steroidmicros.com
 *
 * Copyright (c) 2005, Intec Automation Inc.
 *
 * Some parts are based on lart.c by Abraham Van Der Merwe
 *
 * Cleaned up and generalized based on mtd_dataflash.c
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/delay.h>


#define FLASH_PAGESIZE		256

/* Flash opcodes. */
#define OPCODE_WRDIS    0x04    /* Write disable */
#define	OPCODE_WREN		0x06	/* Write enable */
#define	OPCODE_RDSR		0x05	/* Read status register */
#define	OPCODE_WRSR		0x01	/* Write status register 1 byte */
#define	OPCODE_NORM_READ	0x03	/* Read data bytes (low frequency) */
#define	OPCODE_FAST_READ	0x0b	/* Read data bytes (high frequency) */
#define	OPCODE_PP		0x02	/* Page program (up to 256 bytes) */
#define	OPCODE_BE_4K 		0x20	/* Erase 4KiB block */
#define	OPCODE_BE_32K		0x52	/* Erase 32KiB block */
#define	OPCODE_SE		0xd8	/* Sector erase (usually 64KiB) */
#define	OPCODE_RDID		0x9f	/* Read JEDEC ID */

/* Status Register bits. */
#define	SR_WIP			1	/* Write in progress */
#define	SR_WEL			2	/* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define	SR_BP0			4	/* Block protect 0 */
#define	SR_BP1			8	/* Block protect 1 */
#define	SR_BP2			0x10	/* Block protect 2 */
#define	SR_SRWD			0x80	/* SR write protect */

#define	CMD_SIZE		4

#ifdef CONFIG_M25PXX_USE_FAST_READ
#define OPCODE_READ 	OPCODE_FAST_READ
#define FAST_READ_DUMMY_BYTE 1
#else
#define OPCODE_READ 	OPCODE_NORM_READ
#define FAST_READ_DUMMY_BYTE 0
#endif

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()	(1)
#else
#define	mtd_has_partitions()	(0)
#endif

struct flash_platform_data {
	char		*name;
	struct mtd_partition *parts;
	unsigned int	nr_parts;

	char		*type;

	/* we'll likely add more ... use JEDEC IDs, etc */
};

#define USE_CAMELOT_DIRECT_READ

/*=============================================================================+
| Define                                                                       |
+=============================================================================*/

#define	SF_REG_CR	0	/** Control Register */
#define	SF_REG_AR	4	/** Address Register */
#define	SF_REG_DR	8	/** Data Register */

#define SF_CMD_S	24	/** CR command field shift */
#define	SF_DATA_S	0	/** CR data field shift */

#ifdef	CONFIG_SROM1_INIT
#define	SF_CR1_INIT	CONFIG_SROM1_INIT
#else
#define	SF_CR1_INIT	0x330
#endif

#ifdef	CONFIG_SROM2_INIT
#define	SF_CR2_INIT	CONFIG_SROM2_INIT
#else
#define	SF_CR2_INIT	0x330
#endif

#define	SFREG(cb,reg)	(*(volatile unsigned long*)(cb+reg))

#define	SF_PROG_TO	100000	/** wait loop before program timeout, 1.5 ~5 ms */ 
#define	SF_ERASE_TO	5000000	/** wait loop before sector erase timeout, 0.8~ 2 sec*/ 

struct sf_dev
{
	unsigned long fbase;	/** flash memory base */
	unsigned long cbase;	/** control register base */
	unsigned long size;	/** flash size */
	unsigned long cr_init;	/** initial cr value */
};

/*=============================================================================+
| Variables						                       |
+=============================================================================*/
struct sf_dev g_sfd[2]=
{
    {
    	fbase: 0xbfc00000,
    	cbase: 0xbffffff0,
    	cr_init: SF_CR1_INIT,
    },
    {
    	fbase: 0xbf000000,
    	cbase: 0xbfbffff0,
    	cr_init: SF_CR2_INIT,
    }
};


/****************************************************************************/

struct m25p {
	struct platform_device	*dev;
	struct mutex		lock;
	struct mtd_info		mtd;
	unsigned		partitioned:1;
	u8			erase_opcode;
	u8			command[CMD_SIZE + FAST_READ_DUMMY_BYTE];
};

static inline struct m25p *mtd_to_m25p(struct mtd_info *mtd)
{
	return container_of(mtd, struct m25p, mtd);
}

/****************************************************************************/

/*
 * Internal helper functions
 */

/*
 * Read the status register, returning its value in the location
 * Return the status register value.
 * Returns negative if error occurred.
 */
static int read_sr(struct m25p *flash)
{
	u8 code = OPCODE_RDSR;
	u8 val;
    struct sf_dev *sfd = &g_sfd[flash->dev->id];

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
            (code << SF_CMD_S)| (1 << SF_DATA_S);

    mb();

    val = SFREG(sfd->cbase,SF_REG_DR)>>24;

    /* any possible error checking? */
    // dev_err(&flash->dev->dev, "error %d reading SR\n", (int) retval);

	return val;
}

/*
 * Set write enable latch with Write Enable command.
 * Returns negative if error occurred.
 */
static inline int write_enable(struct m25p *flash)
{
	u8	code = OPCODE_WREN;
    struct sf_dev *sfd = &g_sfd[flash->dev->id];

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
                (code << SF_CMD_S)| (0 << SF_DATA_S);

    mb();

    /* any possible error checking? */

	return 0;
}

static inline int write_disable(struct m25p *flash)
{
	u8	code = OPCODE_WRDIS;
    struct sf_dev *sfd = &g_sfd[flash->dev->id];

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
                (code << SF_CMD_S)| (0 << SF_DATA_S);

    mb();

    /* any possible error checking? */

	return 0;
}


/*
 * Write status register 1 byte
 * Returns negative if error occurred.
 */
static int write_sr(struct m25p *flash, u8 val)
{
	u8	code = OPCODE_WRSR;
    struct sf_dev *sfd = &g_sfd[flash->dev->id];

    write_enable(flash);

    SFREG(sfd->cbase,SF_REG_DR) = ((u32) val ) << 24;

    mb();

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
            (code << SF_CMD_S)| (1 << SF_DATA_S);

    /* any possible error checking? */

	return 0;
}


/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(struct m25p *flash, int maxcount)
{
	int count;
	int sr;

	/* one chip guarantees max 5 msec wait here after page writes,
	 * but potentially three seconds (!) after page erase.
	 */
	for (count = 0; count < maxcount; count++) {
		if ((sr = read_sr(flash)) < 0)
			break;
		else if (!(sr & SR_WIP))
			return 0;

        udelay(10);
		/* REVISIT sometimes sleeping would be best */
	}

    dev_err(&flash->dev->dev, "wait_till_ready timeout\n");
	return 1;
}


/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_sector(struct m25p *flash, u32 offset)
{
    struct sf_dev *sfd = &g_sfd[flash->dev->id];

	DEBUG(MTD_DEBUG_LEVEL3, "%s: %s %dKiB at 0x%08x\n",
			flash->dev->dev.bus_id, __func__,
			flash->mtd.erasesize / 1024, offset);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash, SF_ERASE_TO))
		return 1;

	/* Send write enable, then erase commands. */
	write_enable(flash);

    SFREG(sfd->cbase,SF_REG_AR) = offset;

    mb();

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
            (flash->erase_opcode << SF_CMD_S)| (0 << SF_DATA_S);

	return 0;
}

/****************************************************************************/

/*
 * MTD implementation
 */

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int camelot_flash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct m25p *flash = mtd_to_m25p(mtd);
	u32 addr;
	int len;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %d\n",
			flash->dev->dev.bus_id, __func__, "at",
			(u32)instr->addr, instr->len);

	/* sanity checks */
	if (instr->addr + instr->len > flash->mtd.size)
		return -EINVAL;
	if ((instr->addr % mtd->erasesize) != 0
			|| (instr->len % mtd->erasesize) != 0) {
		return -EINVAL;
	}

	addr = instr->addr;
	len = (int) instr->len;

	mutex_lock(&flash->lock);

	/* REVISIT in some cases we could speed up erasing large regions
	 * by using OPCODE_SE instead of OPCODE_BE_4K
	 */

	/* now erase those sectors */
	while (len>0) {
		if (erase_sector(flash, addr)) {
			instr->state = MTD_ERASE_FAILED;
			mutex_unlock(&flash->lock);
			return -EIO;
		}

		addr += mtd->erasesize;
		len -= mtd->erasesize;
	}

	mutex_unlock(&flash->lock);

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);

	return 0;
}

#if defined(USE_CAMELOT_DIRECT_READ)

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
static int camelot_flash_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
	struct m25p *flash = mtd_to_m25p(mtd);
    u_char *src, *dst;
    int i;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %zd\n",
			flash->dev->dev.bus_id, __func__, "from",
			(u32)from, len);

	/* sanity checks */
	if (len <= 0)
		return 0;

	if (from + len > flash->mtd.size)
		return -EINVAL;

	/* Byte count starts at zero. */
	if (retlen)
		*retlen = 0;

	mutex_lock(&flash->lock);

	/* Wait till previous write/erase is done. */
	if (wait_till_ready(flash, SF_PROG_TO)) {
		/* REVISIT status return?? */
		mutex_unlock(&flash->lock);
		return 1;
	}

    dst = buf;
    src = (u_char*)(g_sfd[flash->dev->id].fbase + (u32)from);

    for(i=0;i<len;i++)
        *dst++ = *src++;

    *retlen = len;

	mutex_unlock(&flash->lock);

	return 0;
}

#else

static int camelot_flash_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
	struct m25p *flash = mtd_to_m25p(mtd);
    struct sf_dev *sfd = &g_sfd[flash->dev->id];
    int offset = 0;
    int len_left, read_bytes;
    u32 data;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %zd\n",
			flash->dev->dev.bus_id, __func__, "from",
			(u32)from, len);

	/* sanity checks */
	if (!len)
		return 0;

	if (from + len > flash->mtd.size)
		return -EINVAL;

	/* Byte count starts at zero. */
	if (retlen)
		*retlen = 0;

	mutex_lock(&flash->lock);

	/* Wait till previous write/erase is done. */
	if (wait_till_ready(flash, SF_PROG_TO)) {
		/* REVISIT status return?? */
		mutex_unlock(&flash->lock);
		return 1;
	}


    #define BYTES_PER_READ  4

    len_left = (int) len;
    while(len_left > 0)
    {
        if(len_left < BYTES_PER_READ)
            read_bytes = (len % BYTES_PER_READ);
        else
            read_bytes = BYTES_PER_READ;

        SFREG(sfd->cbase,SF_REG_AR) = from + offset;
    
        mb();

        SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
                (OPCODE_READ << SF_CMD_S)| (read_bytes << SF_DATA_S);
    
        mb();

        data = SFREG(sfd->cbase,SF_REG_DR);

        memcpy(&buf[offset], &data, read_bytes);

        offset += BYTES_PER_READ;
        len_left -= BYTES_PER_READ;
    }

    if (retlen)
        *retlen = len;

	mutex_unlock(&flash->lock);

	return 0;
}

#endif

/*
 * Write an address range to the flash chip.  Data must be written in
 * FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
static int camelot_flash_write(struct mtd_info *mtd, loff_t to, size_t len,
	size_t *retlen, const u_char *buf)
{
	struct m25p *flash = mtd_to_m25p(mtd);
    struct sf_dev *sfd = &g_sfd[flash->dev->id];
	int aligned = 1;
    int datalen;
    int __len = (int) len;
	u32 data;

    u32 addr = (u32) to;
	unsigned char *src = (unsigned char *) buf;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %zd\n",
			flash->dev->dev.bus_id, __func__, "to",
			(u32)to, len);

	if (retlen)
		*retlen = 0;

	/* sanity checks */
	if (!len)
		return(0);

	if (to + len > flash->mtd.size)
		return -EINVAL;

	mutex_lock(&flash->lock);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash, SF_PROG_TO)) {
		mutex_unlock(&flash->lock);
		return 1;
	}

    if ((u32)buf&1)
        aligned = 0;

    for( ; __len>0 ; addr+=4 ) 
    {
        write_enable(flash);

        if (aligned && len>=4)
        {
            data = ((*(u16 *)src) << 16)  | ((*(u16 *)(src+2)));
            __len -= 4;
            src += 4;
            datalen = 4;
        }
        else
        {
            data  = 0;
            for (datalen=0; datalen < 4 && __len >0;__len-- , datalen++)
                data += (*src++) << ((3-datalen)<<3);
        }

        SFREG(sfd->cbase,SF_REG_AR) = addr;
        SFREG(sfd->cbase,SF_REG_DR) = data;

        mb();

        SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
                                        (OPCODE_PP << SF_CMD_S)| (datalen << SF_DATA_S);

        mb();

        wait_till_ready(flash, SF_PROG_TO);
    }

    if (retlen)
        *retlen = len;

	mutex_unlock(&flash->lock);

	return 0;
}


/****************************************************************************/

/*
 * SPI device driver setup and teardown
 */

struct flash_info {
	char		*name;

	/* JEDEC id zero means "no ID" (most older chips); otherwise it has
	 * a high byte of zero plus three data bytes: the manufacturer id,
	 * then a two byte device id.
	 */
	u32		jedec_id;

	/* The size listed here is what works with OPCODE_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	unsigned	sector_size;
	u16		n_sectors;

	u16		flags;
#define	SECT_4K		0x01		/* OPCODE_BE_4K works uniformly */
};


/* NOTE: double check command sets and memory organization when you add
 * more flash chips.  This current list focusses on newer chips, which
 * have been converging on command sets which including JEDEC ID.
 */
static struct flash_info __devinitdata m25p_data [] = {

	/* Atmel -- some are (confusingly) marketed as "DataFlash" */
	{ "at25fs010",  0x1f6601, 32 * 1024, 4, SECT_4K, },
	{ "at25fs040",  0x1f6604, 64 * 1024, 8, SECT_4K, },

	{ "at25df041a", 0x1f4401, 64 * 1024, 8, SECT_4K, },
	{ "at25df641",  0x1f4800, 64 * 1024, 128, SECT_4K, },

	{ "at26f004",   0x1f0400, 64 * 1024, 8, SECT_4K, },
	{ "at26df081a", 0x1f4501, 64 * 1024, 16, SECT_4K, },
	{ "at26df161a", 0x1f4601, 64 * 1024, 32, SECT_4K, },
	{ "at26df321",  0x1f4701, 64 * 1024, 64, SECT_4K, },

	/* Spansion -- single (large) sector size only, at least
	 * for the chips listed here (without boot sectors).
	 */
	{ "s25sl004a", 0x010212, 64 * 1024, 8, },
	{ "s25sl008a", 0x010213, 64 * 1024, 16, },
	{ "s25sl016a", 0x010214, 64 * 1024, 32, },
	{ "s25sl032a", 0x010215, 64 * 1024, 64, },
	{ "s25sl064a", 0x010216, 64 * 1024, 128, },

	/* SST -- large erase sizes are "overlays", "sectors" are 4K */
	{ "sst25vf040b", 0xbf258d, 64 * 1024, 8, SECT_4K, },
	{ "sst25vf080b", 0xbf258e, 64 * 1024, 16, SECT_4K, },
	{ "sst25vf016b", 0xbf2541, 64 * 1024, 32, SECT_4K, },
	{ "sst25vf032b", 0xbf254a, 64 * 1024, 64, SECT_4K, },

	/* EON -- en25px */  
	{ "en25p32", 0x1c2016, 64 * 1024, 64, }, 
	{ "en25p16", 0x1c2015, 64 * 1024, 32, },
    { "en25p80", 0x1c2014, 64 * 1024, 16, }, 
    { "en25f80", 0x1c3114, 64 * 1024, 16, },

	/* ST Microelectronics -- newer production may have feature updates */
	{ "m25p05",  0x202010,  32 * 1024, 2, },
	{ "m25p10",  0x202011,  32 * 1024, 4, },
	{ "m25p20",  0x202012,  64 * 1024, 4, },
	{ "m25p40",  0x202013,  64 * 1024, 8, },
	{ "m25p80",         0,  64 * 1024, 16, },
	{ "m25p16",  0x202015,  64 * 1024, 32, },
	{ "m25p32",  0x202016,  64 * 1024, 64, },
	{ "m25p64",  0x202017,  64 * 1024, 128, },
	{ "m25p128", 0x202018, 256 * 1024, 64, },

	{ "m45pe80", 0x204014,  64 * 1024, 16, },
	{ "m45pe16", 0x204015,  64 * 1024, 32, },

	{ "m25pe80", 0x208014,  64 * 1024, 16, },
	{ "m25pe16", 0x208015,  64 * 1024, 32, SECT_4K, },

	/* Winbond -- w25x "blocks" are 64K, "sectors" are 4KiB */
	{ "w25x10", 0xef3011, 64 * 1024, 2, SECT_4K, },
	{ "w25x20", 0xef3012, 64 * 1024, 4, SECT_4K, },
	{ "w25x40", 0xef3013, 64 * 1024, 8, SECT_4K, },
	{ "w25x80", 0xef3014, 64 * 1024, 16, SECT_4K, },
	{ "w25x16", 0xef3015, 64 * 1024, 32, SECT_4K, },
	{ "w25x32", 0xef3016, 64 * 1024, 64, SECT_4K, },
	{ "w25x64", 0xef3017, 64 * 1024, 128, SECT_4K, },
};

static struct flash_info *__devinit jedec_probe(struct platform_device *dev)
{
	int			tmp;
	u8			code = OPCODE_RDID;
	u8			id[3];
    u32         regval;
	u32			jedec;
	struct flash_info	*info;
    struct sf_dev *sfd = &g_sfd[dev->id];

	/* JEDEC also defines an optional "extended device information"
	 * string for after vendor-specific data, after the three bytes
	 * we use here.  Supporting some chips might require using it.
	 */

    SFREG(sfd->cbase,SF_REG_CR) = sfd->cr_init |
                (code << SF_CMD_S)| (3 << SF_DATA_S);

    mb();

    regval = SFREG(sfd->cbase,SF_REG_DR);
    memcpy(&id, &regval, 3);

	jedec = id[0];
	jedec = jedec << 8;
	jedec |= id[1];
	jedec = jedec << 8;
	jedec |= id[2];

	for (tmp = 0, info = m25p_data;
			tmp < ARRAY_SIZE(m25p_data);
			tmp++, info++) {
		if (info->jedec_id == jedec)
			return info;
	}
	dev_err(&dev->dev, "unrecognized JEDEC id %06x\n", jedec);
	return NULL;
}


/*
 * board specific setup should have ensured the SPI clock used here
 * matches what the READ command supports, at least until this driver
 * understands FAST_READ (for clocks over 25 MHz).
 */
static int __devinit camelot_flash_probe(struct platform_device *dev)
{
	struct flash_platform_data	*data;
	struct m25p			*flash;
	struct flash_info		*info;
	unsigned			i;

    /* Platform data helps sort out which chip type we have, as
	 * well as how this board partitions it.  If we don't have
	 * a chip ID, try the JEDEC id commands; they'll work for most
	 * newer chips, even if we don't recognize the particular chip.
	 */
	data = dev->dev.platform_data;
	if (data && data->type) {
		for (i = 0, info = m25p_data;
				i < ARRAY_SIZE(m25p_data);
				i++, info++) {
			if (strcmp(data->type, info->name) == 0)
				break;
		}

		/* unrecognized chip? */
		if (i == ARRAY_SIZE(m25p_data)) {
			DEBUG(MTD_DEBUG_LEVEL0, "%s: unrecognized id %s\n",
					dev->dev.bus_id, data->type);
			info = NULL;

		/* recognized; is that chip really what's there? */
		} else if (info->jedec_id) {
			struct flash_info	*chip = jedec_probe(dev);

			if (!chip || chip != info) {
				dev_warn(&dev->dev, "found %s, expected %s\n",
						chip ? chip->name : "UNKNOWN",
						info->name);
				info = NULL;
			}
		}
	} else
		info = jedec_probe(dev);

	if (!info)
		return -ENODEV;

	flash = kzalloc(sizeof *flash, GFP_KERNEL);
	if (!flash)
		return -ENOMEM;

	flash->dev = dev;
	mutex_init(&flash->lock);
	dev_set_drvdata(&dev->dev, flash);

	/*
	 * Atmel serial flash tend to power up
	 * with the software protection bits set
	 */

	if (info->jedec_id >> 16 == 0x1f) {
		write_enable(flash);
		write_sr(flash, 0);
	}

	if (data && data->name)
		flash->mtd.name = data->name;
	else
		flash->mtd.name = dev->dev.bus_id;

	flash->mtd.type = MTD_NORFLASH;
	flash->mtd.writesize = 1;
	flash->mtd.flags = MTD_CAP_NORFLASH;
	flash->mtd.size = info->sector_size * info->n_sectors;
	flash->mtd.erase = camelot_flash_erase;
	flash->mtd.read = camelot_flash_read;
	flash->mtd.write = camelot_flash_write;

	/* prefer "small sector" erase if possible */
	if (info->flags & SECT_4K) {
		flash->erase_opcode = OPCODE_BE_4K;
		flash->mtd.erasesize = 4096;
	} else {
		flash->erase_opcode = OPCODE_SE;
		flash->mtd.erasesize = info->sector_size;
	}

	dev_info(&dev->dev, "%s (%d Kbytes)\n", info->name,
			flash->mtd.size / 1024);

	DEBUG(MTD_DEBUG_LEVEL2,
		"mtd .name = %s, .size = 0x%.8x (%uMiB) "
			".erasesize = 0x%.8x (%uKiB) .numeraseregions = %d\n",
		flash->mtd.name,
		flash->mtd.size, flash->mtd.size / (1024*1024),
		flash->mtd.erasesize, flash->mtd.erasesize / 1024,
		flash->mtd.numeraseregions);

	if (flash->mtd.numeraseregions)
		for (i = 0; i < flash->mtd.numeraseregions; i++)
			DEBUG(MTD_DEBUG_LEVEL2,
				"mtd.eraseregions[%d] = { .offset = 0x%.8x, "
				".erasesize = 0x%.8x (%uKiB), "
				".numblocks = %d }\n",
				i, flash->mtd.eraseregions[i].offset,
				flash->mtd.eraseregions[i].erasesize,
				flash->mtd.eraseregions[i].erasesize / 1024,
				flash->mtd.eraseregions[i].numblocks);


	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */
	if (mtd_has_partitions()) {
		struct mtd_partition	*parts = NULL;
		int			nr_parts = 0;

#ifdef CONFIG_MTD_CMDLINE_PARTS
		static const char *part_probes[] = { "cmdlinepart", NULL, };

		nr_parts = parse_mtd_partitions(&flash->mtd,
				part_probes, &parts, 0);
#endif

		if (nr_parts <= 0 && data && data->parts) {
			parts = data->parts;
			nr_parts = data->nr_parts;
		}

		if (nr_parts > 0) {
			for (i = 0; i < nr_parts; i++) {
				DEBUG(MTD_DEBUG_LEVEL2, "partitions[%d] = "
					"{.name = %s, .offset = 0x%.8x, "
						".size = 0x%.8x (%uKiB) }\n",
					i, parts[i].name,
					parts[i].offset,
					parts[i].size,
					parts[i].size / 1024);
			}
			flash->partitioned = 1;
			return add_mtd_partitions(&flash->mtd, parts, nr_parts);
		}
	} else if (data->nr_parts)
		dev_warn(&dev->dev, "ignoring %d default partitions on %s\n",
				data->nr_parts, data->name);

	return add_mtd_device(&flash->mtd) == 1 ? -ENODEV : 0;
}

static int __devexit camelot_flash_remove(struct platform_device *dev)
{
	struct m25p	*flash = dev_get_drvdata(&dev->dev);
	int		status;

	/* Clean up MTD stuff. */
	if (mtd_has_partitions() && flash->partitioned)
		status = del_mtd_partitions(&flash->mtd);
	else
		status = del_mtd_device(&flash->mtd);
	if (status == 0)
		kfree(flash);
	return 0;
}

#ifdef CONFIG_PM

#error PM in camelot flash not support yet

static int physmap_flash_suspend(struct platform_device *dev, pm_message_t state)
{
	return 0;
}

static int physmap_flash_resume(struct platform_device *dev)
{
	return 0;
}

static void physmap_flash_shutdown(struct platform_device *dev)
{
	return;
}

#else
#define camelot_flash_suspend NULL
#define camelot_flash_resume NULL
#define camelot_flash_shutdown NULL
#endif


static struct platform_driver camelot_flash_driver = {
        .probe          = camelot_flash_probe,
        .remove         = camelot_flash_remove,
        .suspend        = camelot_flash_suspend,
        .resume         = camelot_flash_resume,
        .shutdown       = camelot_flash_shutdown,
        .driver         = {
                .name   = "camelot-flash",
                .owner  = THIS_MODULE,
        },
};

static struct mtd_partition camelot_partitions[] = {
	{
		.name		= "boot",
		.size		= 0x10000,
		.offset		= 0,
        .mask_flags = MTD_WRITEABLE,
	},
	{
		.name		= "config",
		.size		= 0x10000,
		.offset		= 0x10000,
	},
	{
		.name		= "linux",
		.size		= 0x3B0000,
		.offset		= 0x20000,
	},
	{
		.name		= "rootfs",
		.size		= 0x2D0000,
		.offset		= 0x100000,
	},
	{
		.name		= "nvram",
		.size		= 0x10000,
		.offset		= 0x3D0000,
	},
	{
		.name		= "nvram_backup",
		.size		= 0x10000,
		.offset		= 0x3E0000,
	},
};

static struct flash_platform_data camelot_flash_data = {
	.name	= "camelot-flash",
	.parts		= camelot_partitions,
	.nr_parts	= ARRAY_SIZE(camelot_partitions),
	.type	= "en25p32",			/* auto-probe if the specific type is not assigned */
};

static struct platform_device camelot_flash = {
	.name		= "camelot-flash",
	.id		= 0,
	.dev		= {
		.platform_data	= &camelot_flash_data,
	},
};

static int camelot_flash_init(void)
{
	int err;

    err = platform_driver_register(&camelot_flash_driver);
	if (err == 0)
			platform_device_register(&camelot_flash);

    return err;
}

static void camelot_flash_exit(void)
{
	platform_device_unregister(&camelot_flash);

    platform_driver_unregister(&camelot_flash_driver);
}


module_init(camelot_flash_init);
module_exit(camelot_flash_exit);

