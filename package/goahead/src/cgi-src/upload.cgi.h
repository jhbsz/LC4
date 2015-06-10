#ifndef __UPLOAD_CGI_H__
#define __UPLOAD_CGI_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Tommy Added
// 8M flash - (uboot + config +RF) = 0x800000 - 0x50000
#define CONFIG_MTD_KERNEL_PART_SIZ 0x7b0000

// Tommy marked
#if 0
#include "include/linux/autoconf.h"		/* !!! for CONFIG_MTD_KERNEL_PART_SIZ  !!! */
                                        /*   CONFIG_RT2880_ROOTFS_IN_FLASH */
                                        /*   CONFIG_RT2880_ROOTFS_IN_RAM   */
#endif

inline void mtd_write_firmware(char *filename, int offset, int len)
{
    char cmd[512];

#if defined (CONFIG_RT2880_FLASH_8M) || defined (CONFIG_RT2880_FLASH_16M)
    /* workaround: erase 8k sector by myself instead of mtd_erase */
    /* this is for bottom 8M NOR flash only */
    snprintf(cmd, 512, "/bin/flash -f 0x400000 -l 0x40ffff");
    system(cmd);
#endif

#if defined (CONFIG_RT2880_ROOTFS_IN_RAM)
    snprintf(cmd, 512, "/bin/mtd_write -o %d -l %d write %s Kernel", offset, len, filename);
    system(cmd);
#elif defined (CONFIG_RT2880_ROOTFS_IN_FLASH)

#if 1
	snprintf(cmd, 512, "/usr/sbin/mtd_write -o %d -l %d write %s linux", offset, len, filename);
    system(cmd);
#else
    snprintf(cmd, 512, "/bin/mtd_write -o %d -l %d write %s Kernel", offset,  CONFIG_MTD_KERNEL_PART_SIZ, filename);
    system(cmd);
    if(len > CONFIG_MTD_KERNEL_PART_SIZ ){
    snprintf(cmd, 512, "/bin/mtd_write -o %d -l %d write %s RootFS", offset + CONFIG_MTD_KERNEL_PART_SIZ, len - CONFIG_MTD_KERNEL_PART_SIZ, filename);
    system(cmd);
    }
#endif
#else
    fprintf(stderr, "goahead: no CONFIG_RT2880_ROOTFS defined!");
#endif
    return ;
}

inline void mtd_write_bootloader(char *filename, int offset, int len)
{
    char cmd[512];
    snprintf(cmd, 512, "/bin/mtd_write -o %d -l %d write %s Bootloader", offset, len, filename);
	printf("write bootloader");
	system(cmd);
    return ;
}

#endif /* __UPLOAD_CGI_H__ */
