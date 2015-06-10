/*
 *  linux/drivers/mtd/nand/ox810_nand.c
 *
 *  Copyright (C) 2008 Oxford Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Overview:
 *   This is a device driver for the NAND flash device found on the
 *   OX810 demo board.
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/arch-oxnas/hardware.h>

#define	OX810_NAND_NAME	"OX810_NAND";
#define	OX810_NAND_BASE	STATIC_CS1_BASE	// base address of NAND chip on static bus
#define	OX810_NAND_DATA				OX810_NAND_BASE + 0x0000
//#define	OX810_NAND_ADDRESS_LATCH	OX810_NAND_BASE + 0x1000
#define	OX810_NAND_ADDRESS_LATCH	OX810_NAND_BASE + 0x8000
//#define	OX810_NAND_COMMAND_LATCH	OX810_NAND_BASE + 0x2000
#define	OX810_NAND_COMMAND_LATCH	OX810_NAND_BASE + 0x4000

// commands
#define	OX810_NAND_COMMAND_READ_CYCLE1	0x00
#define	OX810_NAND_COMMAND_WRITE_CYCLE2	0x10
#define	OX810_NAND_COMMAND_READ_CYCLE2	0x30
#define	OX810_NAND_COMMAND_CACHE_READ	0x31
#define	OX810_NAND_COMMAND_BLOCK_ERASE	0x60
#define	OX810_NAND_COMMAND_READ_STATUS	0x70
#define	OX810_NAND_COMMAND_READ_ID		0x90
#define	OX810_NAND_COMMAND_STATUS		0x70
#define	OX810_NAND_COMMAND_WRITE_CYCLE1	0x80
#define	OX810_NAND_COMMAND_ERASE_CONFIRM	0xd0
#define	OX810_NAND_COMMAND_PARAMETER_PAGE	0xec
#define	OX810_NAND_COMMAND_RESET		0xff

// status register bits
#define	OX810_NAND_STATUS_FAIL			(1 << 0)
#define	OX810_NAND_STATUS_READY			(1 << 6)

#ifdef	CONFIG_MTD_PARTITIONS

#define	NUM_PARTITIONS	2

static struct mtd_partition partition_info[] =
{
		{
				.name = "Boot partition",
				.offset = 0,
				.size = 1024 * 1024 * 64
		},
		{
				.name = "Data Partition",
				.offset = MTDPART_OFS_NXTBLK,
				.size = MTDPART_SIZ_FULL
		}
};

#endif

static struct priv {
	struct mtd_info *mtd;
} priv;

static void ox810_nand_write_command(u_int8_t command)
{
	writeb(command, OX810_NAND_COMMAND_LATCH);
}

static u_int8_t ox810_nand_read_data(void)
{
	return readb(OX810_NAND_DATA);
}

static uint8_t ox810_nand_wait_for_ready(void)
{
	int timeout = 100;
	uint8_t status;

	ox810_nand_write_command(OX810_NAND_COMMAND_STATUS);
	status = ox810_nand_read_data();
	if (status & OX810_NAND_STATUS_READY)
		return status;

	udelay(100);

	while (timeout--) {
		status = ox810_nand_read_data();
		if (status & OX810_NAND_STATUS_READY)
			return status;
		msleep(1);
	}
	printk(KERN_ERR "OX810 NAND Timeout waiting for ready\n");

	return OX810_NAND_STATUS_FAIL;
}

static void ox810_nand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = (struct nand_chip *)priv.mtd->priv;
	unsigned long bits = 0;
	char *addr = this->IO_ADDR_W;

	if (ctrl & NAND_CLE)
		bits |= (OX810_NAND_COMMAND_LATCH - OX810_NAND_BASE);
	if (ctrl & NAND_ALE)
		bits |= (OX810_NAND_ADDRESS_LATCH - OX810_NAND_BASE);

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, addr + bits);
}

static int ox810_nand_init(void)
{
	int err,i ;
	struct nand_chip *this;

	priv.mtd = kzalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
	if (!priv.mtd)
		return -ENOMEM;

	this = (struct nand_chip *)((char *)(priv.mtd) + sizeof(struct mtd_info));
	priv.mtd->priv = this;
	priv.mtd->owner = THIS_MODULE;
	this->IO_ADDR_R = (void __iomem *)OX810_NAND_DATA;
	this->IO_ADDR_W = (void __iomem *)OX810_NAND_DATA;
	this->cmd_ctrl = ox810_nand_hwcontrol;
	this->dev_ready = NULL;
	this->ecc.mode = NAND_ECC_SOFT;

	// enable CS_1
	*(volatile u32*)SYS_CTRL_GPIO_PRIMSEL_CTRL_0 |=  0x00100000;
	*(volatile u32*)SYS_CTRL_GPIO_SECSEL_CTRL_0  &= ~0x00100000;
	*(volatile u32*)SYS_CTRL_GPIO_TERTSEL_CTRL_0 &= ~0x00100000;
	// reset
	ox810_nand_write_command(OX810_NAND_COMMAND_RESET);
	ox810_nand_wait_for_ready();
	ox810_nand_write_command(OX810_NAND_COMMAND_PARAMETER_PAGE);
	ox810_nand_wait_for_ready();
	ox810_nand_write_command(OX810_NAND_COMMAND_READ_CYCLE1);
	for (i = 0; i < 137; i++) { // skip to max page read time parameter
		ox810_nand_read_data();
	}
	this->chip_delay = (ox810_nand_read_data() + 256 * ox810_nand_read_data()) / 1000;
#ifdef	CONFIG_MTD_DEBUG
	printk("Page read time %dms\n", this->chip_delay);
#endif
	if (nand_scan(priv.mtd, 1)) {
		err = -ENXIO;
		goto error;
	}

	err = add_mtd_device(priv.mtd);
	if (err) {
		err = -ENFILE;
		goto error;
	}
#ifdef	CONFIG_MTD_PARTITIONS
	add_mtd_partitions(priv.mtd, partition_info, NUM_PARTITIONS);
#endif

	return 0;
error:
	kfree(priv.mtd);
	return err;
}

static void ox810_nand_exit(void)
{
	if (priv.mtd) {
		del_mtd_device(priv.mtd);
		nand_release(priv.mtd);
		kfree(priv.mtd);
	}
}

module_init(ox810_nand_init);
module_exit(ox810_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oxford Semiconductor");
MODULE_DESCRIPTION("NAND flash driver 810 demo board");
