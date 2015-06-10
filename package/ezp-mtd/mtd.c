/*
 * mtd - simple memory technology device manipulation tool
 *
 * Copyright (C) 2005 Waldemar Brodkorb <wbx@dass-it.de>,
 *	                  Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * $Id: mtd.c 3385 2006-03-16 17:11:35Z nbd $
 *
 * The code is based on the linux-mtd examples.
 */

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <string.h>

#include "mtd-api.h"
/*
 * nvram related borrowed from "bcmnvram.h"
 */
extern char * nvram_get(const char *name);
#define nvram_safe_get(name) (nvram_get(name) ? : "")

#define TRX_MAGIC       0x30524448      /* "HDR0" */
#define BUFSIZE (16 * 1024)
#define NVRAM_DOWNGRADE_CMD_BUFSIZE (128)
#define MAX_ARGS 8

#define DEBUG

#ifdef EZPACKET
#define SYSTYPE_UNKNOWN     0
#define SYSTYPE_BROADCOM    1
#define SYSTYPE_REALTEK     2
#define SYSTYPE_FVT         3
/* to be continued */
#endif

/* Be aware buf[] cannot used by any other purpose!!! */
char buf[BUFSIZE];
int buflen;
/* nvram downgrade cmd string. */
char nvram_downgrade_cmd[NVRAM_DOWNGRADE_CMD_BUFSIZE];
char major, minor, subminor;

int mtd_open(const char *mtd, int block)
{
    FILE *fp;
    char dev[PATH_MAX];
    int i;
    int ret;
    int flags = O_RDWR | O_SYNC;

    if ((fp = fopen("/proc/mtd", "r"))) {
        while (fgets(dev, sizeof(dev), fp)) {
            if (sscanf(dev, "mtd%d:", &i) && strstr(dev, mtd)) {
                snprintf(dev, sizeof(dev), "/dev/mtd%s/%d", (block ? "block" : "  "), i);
                if ((ret=open(dev, flags))<0) {
                    snprintf(dev, sizeof(dev), "/dev/mtd%s%d", (block ? "block"   : ""), i);
                    ret=open(dev, flags);
                }
                fclose(fp);
                return ret;
            }
        }
        fclose(fp);
    }
    return open(mtd, flags);
}
#ifdef CONFIG_EZP_ARCH_BRCM
/*
 * EZP: prod_cat/prod_subcat is embedded into flag_version.
 */
/* Broadcom firmware header */
struct trx_header {
	u_int32_t magic;		/* "HDR0" */
	u_int32_t len;		/* Length of file including header */
	u_int32_t crc32;		/* 32-bit CRC from flag_version to end of file */
	u_int32_t flag_version;	/* 0:15 flags, 16:31 version */
	u_int32_t offsets[3];    /* Offsets of partitions from start of header */
};

int
image_check(int imagefd, const char *mtd)
{
	struct trx_header *trx = (struct trx_header *) buf;
	struct mtd_info_user mtdInfo;
	int fd;
    char *prod, *fv;
    

	buflen = read(imagefd, buf, 32);
	if (buflen < 32) {
		fprintf(stdout, "Could not get image header, file too small (%ld bytes)\n", buflen);
		return 0;
	}

	switch(trx->magic) {
		case 0x47343557: /* W54G */
		case 0x53343557: /* W54S */
		case 0x73343557: /* W54s */
		case 0x46343557: /* W54F */
		case 0x55343557: /* W54U */
			/* ignore the first 32 bytes */
			buflen = read(imagefd, buf, sizeof(struct trx_header));
			break;
	}
    fv = (char *) &trx->flag_version;
    prod = nvram_safe_get("prod_cat");
    if (fv[0] != prod[0]) {
		fprintf(stderr, "Incorrect product category [%c,%c]\n", 
                prod[0],fv[0]);
        system("touch /tmp/fw_incorrect");
		exit(1);
    }
    prod = nvram_safe_get("prod_subcat");
    if (fv[1] != prod[0]) {
		fprintf(stderr, "Incorrect product subcategory [%c,%c]\n", 
                prod[0], fv[0]);
        system("touch /tmp/fw_incorrect");
		exit(1);
    }
    major = fv[2] >> 4;
    minor = (fv[2] & 0xF);
    subminor = fv[3] >> 4;
	
	if (trx->magic != TRX_MAGIC || trx->len < sizeof(struct trx_header)) {
		fprintf(stderr, "Bad trx header\n");
		fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
				"original firmware files are, use following command to convert to trx:\n"
				"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
		return 0;
	}

	/* check if image fits to mtd device */
    /* EZP:Modified mtd_open(const char *mtd, int flags) 
     *          to mtd_open(const char *mtd,int block) */
	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		exit(1);
	}
		
	if(mtdInfo.size < trx->len) {
		fprintf(stderr, "Image too big for partition: %s\n", mtd);
		close(fd);
		return 0;
	}	
	
	close(fd);
	return 1;
}
#endif

#ifdef CONFIG_EZP_ARCH_RTL865X
/* Realtek firmware header */
#include "rtl_types.h"
#include "rtl_image.h"

int
image_check(int imagefd, const char *mtd)
{
	struct fileImageHeader_s *fih = (struct fileImageHeader_s *) buf;
	struct mtd_info_user mtdInfo;
	int fd, i;
    uint32 chksum, orig_chksum;
    unsigned char *pch, *prod;

	buflen = read(imagefd, buf, sizeof(*fih));
	if (buflen < sizeof(*fih)) {
		fprintf(stdout, "Could not get image header, file too small (%ld bytes)\n", buflen);
		return 0;
	}

	if (fih->productMagic != RTL_PRODUCT_MAGIC) {
		fprintf(stderr, "Bad fih header due to RTL magic number. \n");
		return 0;
	}

    /* EZP: TODO: imageLen is supposed to be checked w/ min and max length! */

    /* check the header checksum */
    /* save the original checksum in the header */
    orig_chksum = fih->imageHdrCksm;
    /* reset the checksum before calculation */
    fih->imageHdrCksm = 0;
    pch = (unsigned char *) fih;
    chksum = 0;
    for (i=0; i<(sizeof(*fih)-1); i++)
        chksum ^= *pch++;

    if (orig_chksum != chksum) {
		fprintf(stderr, "Incorrect image header checksum [0x%x,0x%x]\n", 
                orig_chksum, chksum);
        system("touch /tmp/fw_incorrect");
		exit(1);
    }
    /* Restore the checksum value. Otherwise, 0x00 is written later. */
    fih->imageHdrCksm = chksum;

	/* check if image fits to the hardware */
    pch = (unsigned char *) &fih->time;
    prod = nvram_safe_get("prod_cat");
    if (pch[0] != prod[0]) {
		fprintf(stderr, "Incorrect product category [%c,%c]\n", 
                prod[0], pch[0]);
        system("touch /tmp/fw_incorrect");
		exit(1);
    }
    prod = nvram_safe_get("prod_subcat");
    if (pch[1] != prod[0]) {
		fprintf(stderr, "Incorrect product subcategory [%c,%c]\n", 
                prod[0], pch[1]);
        system("touch /tmp/fw_incorrect");
		exit(1);
    }
    major = pch[2] >> 4;
    minor = (pch[2] & 0xF);
    subminor = pch[3] >> 4;

	/* check if image fits to mtd device */
    /* EZP:Modified mtd_open(const char *mtd, int flags) 
     *          to mtd_open(const char *mtd,int block) */
	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		exit(1);
	}
		
	if(mtdInfo.size < (fih->imageLen + sizeof(*fih))) {
		fprintf(stderr, "Image too big for partition: %s\n", mtd);
		close(fd);
		return 0;
	}	
	
	close(fd);
	return 1;
}
#endif

/* U-Boot based header */
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_OX810) || defined(CONFIG_EZP_ARCH_FV13XX)
#include "image.h"
#include "shared.h"

#define CRC_BUF_SIZE 4096

int
image_check(int imagefd, const char *mtd)
{
    image_header_t *fih = (image_header_t *)buf;
    struct mtd_info_user mtdInfo;
    int fd, i;
    uint32_t chksum, orig_chksum;
    unsigned char *pch, *prod, platform[10] = {0};

    /* EZP:If mtd name is not linux ,we do not check image header. */
    buflen = read(imagefd, buf, sizeof(*fih));
    if (buflen < sizeof(*fih)) {
        fprintf(stdout, "Could not get image header, file too small "
                        "(%ld bytes)\n", buflen);
        return 0;
    }

    if (fih->ih_magic != htonl(IH_MAGIC)) {
        fprintf(stderr, "Bad fih header due to UBoot magic number. \n");
        return 0;
    }

    /* check the header checksum */
    /* save the original checksum in the header */
    orig_chksum = fih->ih_hcrc;
    /* reset the checksum before calculation */
    fih->ih_hcrc = 0;
    pch = (unsigned char *)fih;
    chksum = crc32(0, (const char *)(fih), sizeof(*fih));
    chksum = htonl(chksum);
    if (orig_chksum != chksum) {
        fprintf(stderr, "Incorrect image header checksum [0x%x,0x%x]\n", 
                orig_chksum, chksum);
        system("touch /tmp/fw_incorrect");
        exit(1);
    }
    /* Restore the checksum value. Otherwise, 0x00 is written later. */
    fih->ih_hcrc = orig_chksum;

    /* check if image fits to the hardware */
    pch = (unsigned char *) &fih->ih_time;
    /* The first byte of fih->ih_time is reserved for jffs2 flag. */
    /* For Xchange series, we check super category first, then product
     * sub category*/
    prod = nvram_safe_get("prod_supercat");
    if(!prod || !*prod) {
        prod = nvram_safe_get("prod_cat");
    }
    if (pch[0] != prod[0]) {
        fprintf(stderr, "Incorrect product category [%c,%c]\n", 
                prod[0], pch[0]);
        system("touch /tmp/fw_incorrect");
        exit(1);
    }
    prod = nvram_safe_get("prod_subcat");
    if (pch[1] != prod[0]) {
        fprintf(stderr, "Incorrect product subcategory [%c,%c]\n", 
                prod[0], pch[1]);
        system("touch /tmp/fw_incorrect");
        exit(1);
    }

    major = pch[2] >> 4;
    minor = (pch[2] & 0xF); 
    subminor = pch[3] >> 4;

    if((prod = strstr(fih->ih_name, "&"))) {
        strcpy(platform, prod + 1);
        if(strcmp(platform, nvram_safe_get("platform"))) {
            fprintf(stderr, "Incorrect platform [%s]\n", platform);
            exit(1);
        system("touch /tmp/fw_incorrect");
        }
    }

    /* check if image fits to mtd device */
    /* EZP:Modified mtd_open(const char *mtd, int flags) 
     *          to mtd_open(const char *mtd,int block) */
	fd = mtd_open(mtd, 0);
    if(fd < 0) {
        fprintf(stderr, "Could not open mtd device: %s\n", mtd);
        exit(1);
    }

    if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
        fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
        exit(1);
    }
        
    if(mtdInfo.size < (ntohl(fih->ih_size) + sizeof(*fih))) {
        fprintf(stderr, "Image too big for partition: %s\n", mtd);
        close(fd);
        return 0;
    }	

    close(fd);
    return 1;
}
#if 0 
int
image_verify(int imagefd)
{
    image_header_t *fih = (image_header_t *)buf;
    uint32_t chksum;
    char crc_buf[CRC_BUF_SIZE];
    uint32_t datalen, len;

    /* EZP:If mtd name is not linux ,we do not check image header. */
    len = read(imagefd, buf, sizeof(*fih));

    if (len < sizeof(*fih)) {
        fprintf(stdout, "Could not get image header, file too small "
                        "(%ld bytes)\n", len);
       return 1;
    }

    /* EZP: read into the buf and do the data checksum. */
    chksum = 0;
    datalen = ntohl(fih->ih_size);
    while(1) {
        len = read(imagefd, crc_buf, CRC_BUF_SIZE);
        /* if checksum data length less than firmware or
         * data be read less than CRC_BUF_SIZE */
        if (datalen < len) {
            /* if firmware size big than checksum data length
             * datalen is 0
             * read file continuously until end of file. */
            if(datalen != 0) {
                /* partial read */
                chksum = crc32(chksum, crc_buf, datalen);
                datalen = 0;
            }
            /* we read file continuously until end of file */
        } else {
            /* datalen >= len */
            if (len == 0) {
                /* image file size is less than fih->ih_size.
                 * firmware file is not complete 
                 * or read end of file
                 fprintf(stderr, 
                 "Image file small than fih->ih_size or read EOL\n"); */
                break;
            } else {
                /* full read */
                datalen -= len;
                chksum = crc32(chksum, crc_buf, len);
            }
        }
    }

    if (fih->ih_dcrc != htonl(chksum)) {
        fprintf(stderr, "Incorrect image data checksum [0x%x,0x%x]\n", 
                fih->ih_dcrc, htonl(chksum));
        return 1; 
    } else {
        /* fprintf(stderr, "image data checksum [0x%x,0x%x]\n", 
                fih->ih_dcrc, htonl(chksum)); */
        return 0;
    }
}
#endif

int
image_verify(int imagefd)
{
    image_header_t *fih = (image_header_t *)buf;
    uint32_t chksum;
    char crc_buf[CRC_BUF_SIZE];
    uint32_t datalen, len, val;

    /* EZP:If mtd name is not linux ,we do not check image header. */
    len = read(imagefd, buf, sizeof(*fih));

    if (len < sizeof(*fih)) {
        fprintf(stdout, "Could not get image header, file too small "
                        "(%ld bytes)\n", len);
       return 1;
    }

    /* EZP: read into the buf and do the data checksum. */
    chksum = 0;
    datalen = ntohl(fih->ih_size);
    while(1) {
        len = read(imagefd, crc_buf, CRC_BUF_SIZE);
        if (len == 0) {
            /* image file size is less than fih->ih_size.
             * firmware file is not complete 
             * or read end of file */
                break;
        }
        if (datalen > 0) {
            /* if data size big than 0, we need to calculate
             * crc checksum */
            /* if datalen < len , this is partial read */
            val = (datalen < len) ? datalen : len;
            chksum = crc32(chksum, crc_buf, val);
            datalen -= val;
            }
        /* we read file continuously until end of file */
    } 

    if (fih->ih_dcrc != htonl(chksum)) {
        fprintf(stderr, "Incorrect image data checksum [0x%x,0x%x]\n", 
                fih->ih_dcrc, htonl(chksum));
        system("touch /tmp/fw_incorrect");
        return 1; 
    } else {
        /* fprintf(stderr, "image data checksum [0x%x,0x%x]\n", 
                fih->ih_dcrc, htonl(chksum)); */
        return 0;
    }
}
#endif

int mtd_check(char *mtd)
{
	struct mtd_info_user mtdInfo;
	int fd;
    /* EZP:Modified mtd_open(const char *mtd, int flags) 
     *          to mtd_open(const char *mtd,int block) */
	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		return 0;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}

int
mtd_unlock(const char *mtd)
{
	int fd;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdLockInfo;

	//fd = mtd_open(mtd, O_RDWR | O_SYNC);
	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}

	mtdLockInfo.start = 0;
	mtdLockInfo.length = mtdInfo.size;
	if(ioctl(fd, MEMUNLOCK, &mtdLockInfo)) {
		close(fd);
		return 0;
	}
		
	close(fd);
	return 0;
}

int
mtd_erase(const char *mtd)
{
	int fd;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdEraseInfo;

    /* EZP:Modified mtd_open(const char *mtd, int flags) 
     *          to mtd_open(const char *mtd,int block) */
	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}

	mtdEraseInfo.length = mtdInfo.erasesize;

	for (mtdEraseInfo.start = 0;
		 mtdEraseInfo.start < mtdInfo.size;
		 mtdEraseInfo.start += mtdInfo.erasesize) {
		
		ioctl(fd, MEMUNLOCK, &mtdEraseInfo);
		if(ioctl(fd, MEMERASE, &mtdEraseInfo)) {
			fprintf(stderr, "Could not erase MTD device: %s\n", mtd);
			close(fd);
			exit(1);
		}
	}		

	close(fd);
	return 0;

}

int
mtd_write(int imagefd, const char *mtd, int quiet)
{
	int fd, i, result;
	size_t r, w, e;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdEraseInfo;
	int ret = 0;

	/* EZP:Modified mtd_open(const char *mtd, int flags) 
	 *          to mtd_open(const char *mtd,int block) */

	fd = mtd_open(mtd, 0);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}
		
	r = w = e = 0;
	if (!quiet)
		fprintf(stderr, " [ ]");

	for (;;) {
		/* buffer may contain data already (from trx check) */
		r = buflen;
		r += read(imagefd, buf + buflen, BUFSIZE - buflen);
		w += r;

		/* EOF */
		if (r <= 0) break;

		/* need to erase the next block before writing data to it */
		while (w > e) {
			mtdEraseInfo.start = e;
			mtdEraseInfo.length = mtdInfo.erasesize;

			if (!quiet)
				fprintf(stderr, "\b\b\b[e]");
			/* erase the chunk */
			if (ioctl (fd,MEMERASE,&mtdEraseInfo) < 0) {
				fprintf(stderr, "Erasing mtd failed: %s\n", mtd);
				exit(1);
			}
			e += mtdInfo.erasesize;
		}
		
		if (!quiet)
			fprintf(stderr, "\b\b\b[w]");
		
		if ((result = write(fd, buf, r)) < r) {
			if (result < 0) {
				fprintf(stderr, "Error writing image.\n");
				exit(1);
			} else {
				fprintf(stderr, "Insufficient space.\n");
				exit(1);
			}
		}
		
		buflen = 0;
	}
	if (!quiet)
		fprintf(stderr, "\b\b\b\b");

	close(fd);
	return 0;
}

void usage(void)
{
	fprintf(stderr, "Usage: mtd [<options> ...] <command> [<arguments> ...] <device>\n\n"
	"The device is in the format of mtdX (eg: mtd4) or its label.\n"
	"mtd recognizes these commands:\n"
	"        unlock                  unlock the device\n"
	"        erase                   erase all data on device\n"
	"        write <imagefile>|-     write <imagefile> (use - for stdin) to device\n"
	"Following options are available:\n"
	"        -q                      quiet mode (once: no [w] on writing,\n"
	"                                           twice: no status messages)\n"
	"        -r                      reboot after successful command\n"
	"        -f                      force write without [trx|bin] checks\n"
	"        -e <device>             erase <device> before executing the command\n\n"
	"Example: To write linux.trx to mtd4 labeled as linux and reboot afterwards\n"
	"         mtd -r write linux.trx linux\n\n");
	exit(1);
}

int main (int argc, char **argv)
{
	int ch, i, boot, unlock, imagefd, force, quiet, unlocked;
	char *erase[MAX_ARGS], *device, *imagefile;
	enum {
		CMD_ERASE,
		CMD_WRITE,
		CMD_UNLOCK
	} cmd;
	
	erase[0] = NULL;
	boot = 0;
	force = 0;
	buflen = 0;
	quiet = 0;

	while ((ch = getopt(argc, argv, "frqe:")) != -1)
		switch (ch) {
			case 'f':
				force = 1;
				break;
			case 'r':
				boot = 1;
				break;
			case 'q':
				quiet++;
				break;
			case 'e':
				i = 0;
				while ((erase[i] != NULL) && ((i + 1) < MAX_ARGS))
					i++;
					
				erase[i++] = optarg;
				erase[i] = NULL;
				break;
			
			case '?':
			default:
				usage();
		}
	argc -= optind;
	argv += optind;
	
	if (argc < 2)
		usage();

	if ((strcmp(argv[0], "unlock") == 0) && (argc == 2)) {
		cmd = CMD_UNLOCK;
		device = argv[1];
	} else if ((strcmp(argv[0], "erase") == 0) && (argc == 2)) {
		cmd = CMD_ERASE;
		device = argv[1];
	} else if ((strcmp(argv[0], "write") == 0) && (argc == 3)) {
		cmd = CMD_WRITE;
		device = argv[2];
	
		if (strcmp(argv[1], "-") == 0) {
			imagefile = "<stdin>";
			imagefd = 0;
		} else {
			imagefile = argv[1];
			if ((imagefd = open(argv[1], O_RDONLY)) < 0) {
				fprintf(stderr, "Couldn't open image file: %s!\n", imagefile);
				exit(1);
			}
		}
	
		/* check trx file before erasing or writing anything */
        if (!strcmp(device,"linux")) {
            if (!image_check(imagefd, device)) {
                if ((quiet < 2) || !force)
                    fprintf(stderr, "[TRX|BIN] check failed!\n");
                if (!force)
                    exit(1);
            } 
        }
		if (!mtd_check(device)) {
			fprintf(stderr, "Can't open device for writing!\n");
			exit(1);
		}

        /* nvram downgrade */
        snprintf(nvram_downgrade_cmd, NVRAM_DOWNGRADE_CMD_BUFSIZE, 
                "/usr/sbin/nvram downgrade %d.%d.%d", major, minor, subminor);

        printf(nvram_downgrade_cmd);
        printf("\n");
        system(nvram_downgrade_cmd);
#if defined(CONFIG_EZP_ARCH_RT305X) || defined(CONFIG_EZP_ARCH_OX810) || defined(CONFIG_EZP_ARCH_FV13XX)
	} else if ((strcmp(argv[0], "verify") == 0) && (argc == 2)) {
        if (strcmp(argv[1], "-") == 0) {
            imagefile = "<stdin>";
            imagefd = 0;
        } else {
			imagefile = argv[1];
			if ((imagefd = open(argv[1], O_RDONLY)) < 0) {
				fprintf(stderr, "Couldn't open image file: %s!\n", imagefile);
				exit(1);
			}
        }

        if(!image_verify(imagefd)) {
            printf("verify:ok");
            exit(0);
        } else {
            printf("verify:failed");
            exit(1);
        }
#endif
	} else {
		usage();
	}

	sync();
	
	i = 0;
	unlocked = 0;
	while (erase[i] != NULL) {
		if (quiet < 2)
			fprintf(stderr, "Unlocking %s ...\n", erase[i]);
		mtd_unlock(erase[i]);
		if (quiet < 2)
			fprintf(stderr, "Erasing %s ...\n", erase[i]);
		mtd_erase(erase[i]);
		if (strcmp(erase[i], device) == 0)
			unlocked = 1;
		i++;
	}
	
	if (!unlocked) {
		if (quiet < 2) 
			fprintf(stderr, "Unlocking %s ...\n", device);
		mtd_unlock(device);
	}
		
	switch (cmd) {
		case CMD_UNLOCK:
			break;
		case CMD_ERASE:
			if (quiet < 2)
				fprintf(stderr, "Erasing %s ...\n", device);
			mtd_erase(device);
			break;
		case CMD_WRITE:
			if (quiet < 2)
				fprintf(stderr, "Writing from %s to %s ... ", imagefile, device);
			mtd_write(imagefd, device, quiet);
			if (quiet < 2)
				fprintf(stderr, "\n");
			break;
	}

	sync();
	
	if (boot)
		kill(1, 15); // send SIGTERM to init for reboot

	return 0;
}
