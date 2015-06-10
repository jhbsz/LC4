/*
 * (C) Copyright 2000-2004
 * DENX Software Engineering
 * Wolfgang Denk, wd@denx.de
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __WIN32__
#include <netinet/in.h>		/* for host / network byte order conversions	*/
#endif
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


#if defined(__BEOS__) || defined(__NetBSD__) || defined(__APPLE__)
#include <inttypes.h>
#endif

#ifdef __WIN32__
typedef unsigned int __u32;

#define SWAP_LONG(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))
typedef		unsigned char	uint8_t;
typedef		unsigned short	uint16_t;
typedef		unsigned int	uint32_t;

#define     ntohl(a)	SWAP_LONG(a)
#define     htonl(a)	SWAP_LONG(a)
#endif	/* __WIN32__ */

#ifndef	O_BINARY		/* should be define'd on __WIN32__ */
#define O_BINARY	0
#endif

#include "image-as500.h"

extern int errno;

#ifndef MAP_FAILED
#define MAP_FAILED (-1)
#endif

char *cmdname;

typedef struct table_entry {
	int	val;		/* as defined in image.h	*/
	char	*sname;		/* short (input) name		*/
	char	*lname;		/* long (output) name		*/
} table_entry_t;

table_entry_t arch_name[] = {
    {	IH_CPU_INVALID,		NULL,		"Invalid CPU",	},
    {	IH_CPU_ALPHA,		"alpha",	"Alpha",	},
    {	IH_CPU_ARM,		"arm",		"ARM",		},
    {	IH_CPU_I386,		"x86",		"Intel x86",	},
    {	IH_CPU_IA64,		"ia64",		"IA64",		},
    {	IH_CPU_M68K,		"m68k",		"MC68000",	},
    {	IH_CPU_MICROBLAZE,	"microblaze",	"MicroBlaze",	},
    {	IH_CPU_MIPS,		"mips",		"MIPS",		},
    {	IH_CPU_MIPS64,		"mips64",	"MIPS 64 Bit",	},
    {	IH_CPU_PPC,		"ppc",		"PowerPC",	},
    {	IH_CPU_S390,		"s390",		"IBM S390",	},
    {	IH_CPU_SH,		"sh",		"SuperH",	},
    {	IH_CPU_SPARC,		"sparc",	"SPARC",	},
    {	IH_CPU_SPARC64,		"sparc64",	"SPARC 64 Bit",	},
    {	-1,			"",		"",		},
};

table_entry_t os_name[] = {
    {	IH_OS_INVALID,	NULL,		"Invalid OS",		},
    {	IH_OS_4_4BSD,	"4_4bsd",	"4_4BSD",		},
    {	IH_OS_ARTOS,	"artos",	"ARTOS",		},
    {	IH_OS_DELL,	"dell",		"Dell",			},
    {	IH_OS_ESIX,	"esix",		"Esix",			},
    {	IH_OS_FREEBSD,	"freebsd",	"FreeBSD",		},
    {	IH_OS_IRIX,	"irix",		"Irix",			},
    {	IH_OS_LINUX,	"linux",	"Linux",		},
    {	IH_OS_LYNXOS,	"lynxos",	"LynxOS",		},
    {	IH_OS_NCR,	"ncr",		"NCR",			},
    {	IH_OS_NETBSD,	"netbsd",	"NetBSD",		},
    {	IH_OS_OPENBSD,	"openbsd",	"OpenBSD",		},
    {	IH_OS_PSOS,	"psos",		"pSOS",			},
    {	IH_OS_QNX,	"qnx",		"QNX",			},
    {	IH_OS_RTEMS,	"rtems",	"RTEMS",		},
    {	IH_OS_SCO,	"sco",		"SCO",			},
    {	IH_OS_SOLARIS,	"solaris",	"Solaris",		},
    {	IH_OS_SVR4,	"svr4",		"SVR4",			},
    {	IH_OS_U_BOOT,	"u-boot",	"U-Boot",		},
    {	IH_OS_VXWORKS,	"vxworks",	"VxWorks",		},
    {	-1,		"",		"",			},
};

table_entry_t type_name[] = {
    {	IH_TYPE_INVALID,    NULL,	  "Invalid Image",	},
    {	IH_TYPE_FILESYSTEM, "filesystem", "Filesystem Image",	},
    {	IH_TYPE_FIRMWARE,   "firmware",	  "Firmware",		},
    {	IH_TYPE_KERNEL,	    "kernel",	  "Kernel Image",	},
    {	IH_TYPE_MULTI,	    "multi",	  "Multi-File Image",	},
    {	IH_TYPE_RAMDISK,    "ramdisk",	  "RAMDisk Image",	},
    {	IH_TYPE_SCRIPT,     "script",	  "Script",		},
    {	IH_TYPE_STANDALONE, "standalone", "Standalone Program", },
    {	-1,		    "",		  "",			},
};

table_entry_t comp_name[] = {
    {	IH_COMP_NONE,	"none",		"uncompressed",		},
    {	IH_COMP_BZIP2,	"bzip2",	"bzip2 compressed",	},
    {	IH_COMP_GZIP,	"gzip",		"gzip compressed",	},
    {   IH_COMP_LZMA,   "lzma",         "lzma compressed",      },	
    {	-1,		"",		"",			},
};

static	void	copy_file (int, const char *, int);
static  void    write_padding(int ifd, int pad_len, char pad_val);
static	void	usage	(void);
static	void	print_header (image_header_t *);
#if EZPACKET
static	void	print_type (image_header_t *);
#endif
static	char	*put_table_entry (table_entry_t *, char *, int);
static	char	*put_arch (int);
static	char	*put_type (int);
static	char	*put_os   (int);
static	char	*put_comp (int);
static	int	get_table_entry (table_entry_t *, char *, char *);
static	int	get_arch(char *);
static	int	get_comp(char *);
static	int	get_os  (char *);
static	int	get_type(char *);


char	*datafile;
char	*imagefile;

int dflag    = 0;
int eflag    = 0;
int lflag    = 0;
int vflag    = 0;
int xflag    = 0;
int fflag    = 0;
int opt_os   = IH_OS_LINUX;
int opt_arch = IH_CPU_PPC;
int opt_type = IH_TYPE_KERNEL;
int opt_comp = IH_COMP_GZIP;

image_header_t header;
image_header_t *hdr = &header;

int
main (int argc, char **argv)
{
	int ifd;
	uint32_t checksum;
	uint32_t addr;
	uint32_t ep;
	struct stat sbuf;
	unsigned char *ptr;
	char *name = "";
	char cat = '\0';
	char subcat = '\0';
	char major = '\0';
	char minor = '\0';
	char subminor = '\0';
	int fs_padding = 0;
	int kn_offset = 0;
	int fs_offset = 0;
	char *fs_name = "";
    char *tmp;

	cmdname = *argv;

	addr = ep = 0;

	while (--argc > 0 && **++argv == '-') {
		while (*++*argv) {
			switch (**argv) {
			case 'l':
				lflag = 1;
				break;
			case 'A':
				if ((--argc <= 0) ||
				    (opt_arch = get_arch(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'C':
				if ((--argc <= 0) ||
				    (opt_comp = get_comp(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'O':
				if ((--argc <= 0) ||
				    (opt_os = get_os(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'T':
				if ((--argc <= 0) ||
				    (opt_type = get_type(*++argv)) < 0)
					usage ();
				goto NXTARG;
			case 'a':
				if (--argc <= 0)
					usage ();
				addr = strtoul (*++argv, (char **)&ptr, 16);
				if (*ptr) {
					fprintf (stderr,
						"%s: invalid load address %s\n",
						cmdname, *argv);
					exit (EXIT_FAILURE);
				}
				goto NXTARG;
			case 'd':
				if (--argc <= 0)
					usage ();
				datafile = *++argv;
				dflag = 1;
				goto NXTARG;
			case 'e':
				if (--argc <= 0)
					usage ();
				ep = strtoul (*++argv, (char **)&ptr, 16);
				if (*ptr) {
					fprintf (stderr,
						"%s: invalid entry point %s\n",
						cmdname, *argv);
					exit (EXIT_FAILURE);
				}
				eflag = 1;
				goto NXTARG;
			case 'n':
				if (--argc <= 0)
					usage ();
				name = *++argv;
				goto NXTARG;
			case 'v':
				vflag++;
				break;
			case 'x':
				xflag++;
				break;
            case 'k':
                if (--argc <= 0) {
                    usage ();
                }
                kn_offset = strtoul(*++argv, (char **)&ptr, 16);
                if (*ptr) {
                    fprintf (stderr,
                        "%s: invalid entry point %s\n",
                        cmdname, *argv);
                    exit (EXIT_FAILURE);
                }
				goto NXTARG;
            case 'c':
                if (--argc <= 0)
                    usage ();
                cat = **++argv;
				goto NXTARG;
            case 's':
                if (--argc <= 0)
                    usage ();
                subcat = **++argv;
				goto NXTARG;
            case 'w':
                if (--argc <= 0)
                    usage ();
                major = (char) atoi(*++argv);
				goto NXTARG;
            case 'y':
                if (--argc <= 0)
                    usage ();
                minor = (char) atoi(*++argv);
				goto NXTARG;
            case 'z':
                if (--argc <= 0)
                    usage ();
                subminor = (char) atoi(*++argv);
				goto NXTARG;
            case 'f':
                if (--argc <= 0)
                    usage ();
                fs_name = *++argv;
                fflag++;
				goto NXTARG;
            case 'o':
                if (--argc <= 0)
                    usage ();
                fs_offset = strtoul(*++argv, (char **)&ptr, 16);
                if (*ptr) {
                    fprintf (stderr,
                        "%s: invalid entry point %s\n",
                        cmdname, *argv);
                    exit (EXIT_FAILURE);
                }
				goto NXTARG;
			default:
				usage ();
			}
		}
NXTARG:		;
	}

	if ((argc != 1) || ((lflag ^ dflag) == 0))
		usage();

    /* Check kn_offset is larger than the header length */
    if (dflag && kn_offset < sizeof(image_header_t)) {
        kn_offset = sizeof(image_header_t);
    } 
    
    /**
     * Once fflag is enabled:
     * 1. ensure fs_offset and kn_offset are given.
     * 2. ensure the value of fs_offset larger than the kernel length plus the
     *    header length.
     */
    if (fflag) {
        if (!fs_offset) {
		    usage();
            exit(EXIT_FAILURE);
        }

        if (stat(datafile, &sbuf) < 0) {
            fprintf (stderr, "%s: Can't stat %s: %s\n",
                cmdname, datafile, strerror(errno));
            exit (EXIT_FAILURE);
        }

        if (sbuf.st_size + sizeof(image_header_t) > fs_offset) {
            fprintf(stderr, 
                    "%s: The kernel will exceed to the rootfs's partition\n",
                    cmdname);
            exit (EXIT_FAILURE);
        }

        fs_padding = fs_offset - (sbuf.st_size + sizeof(image_header_t));
    }

	if (!eflag) {
		ep = addr;
		/* If XIP, entry point must be after the U-Boot header */
		if (xflag)
			ep += sizeof(image_header_t);
	}

	/*
	 * If XIP, ensure the entry point is equal to the load address plus
	 * the size of the U-Boot header.
	 */
	if (xflag) {
		if (ep != addr + sizeof(image_header_t)) {
			fprintf (stderr, "%s: For XIP, the entry point must be the load addr + %lu\n",
				cmdname,
				(unsigned long)sizeof(image_header_t));
			exit (EXIT_FAILURE);
		}
	}

	imagefile = *argv;

	if (lflag) {
		ifd = open(imagefile, O_RDONLY|O_BINARY);
	} else {
		ifd = open(imagefile, O_RDWR|O_CREAT|O_TRUNC|O_BINARY, 0666);
	}

	if (ifd < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

#if EZP
	if (lflag) {
		int len;
		char *data;
		/*
		 * list header information of existing image
		 */
		if (fstat(ifd, &sbuf) < 0) {
			fprintf (stderr, "%s: Can't stat %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		if ((unsigned)sbuf.st_size < sizeof(image_header_t)) {
			fprintf (stderr,
				"%s: Bad size: \"%s\" is no valid image\n",
				cmdname, imagefile);
			exit (EXIT_FAILURE);
		}

		ptr = (unsigned char *)mmap(0, sbuf.st_size,
					    PROT_READ, MAP_SHARED, ifd, 0);
		if ((caddr_t)ptr == (caddr_t)-1) {
			fprintf (stderr, "%s: Can't read %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}

		/*
		 * create copy of header so that we can blank out the
		 * checksum field for checking - this can't be done
		 * on the PROT_READ mapped data.
		 */
		memcpy (hdr, ptr, sizeof(image_header_t));

		if (ntohl(hdr->ih_magic) != IH_MAGIC) {
			fprintf (stderr,
				"%s: Bad Magic Number: \"%s\" is no valid image\n",
				cmdname, imagefile);
			exit (EXIT_FAILURE);
		}

		data = (char *)hdr;
		len  = sizeof(image_header_t);

		checksum = ntohl(hdr->ih_hcrc);
		hdr->ih_hcrc = htonl(0);	/* clear for re-calculation */

		if (crc32(0, data, len) != checksum) {
			fprintf (stderr,
				"*** Warning: \"%s\" has bad header checksum!\n",
				imagefile);
		}

		data = (char *)(ptr + sizeof(image_header_t));
		len  = sbuf.st_size - sizeof(image_header_t) ;

		if (crc32(0, data, len) != ntohl(hdr->ih_dcrc)) {
			fprintf (stderr,
				"*** Warning: \"%s\" has corrupted data!\n",
				imagefile);
		}

		/* for multi-file images we need the data part, too */
		print_header ((image_header_t *)ptr);

		(void) munmap((void *)ptr, sbuf.st_size);
		(void) close (ifd);

		exit (EXIT_SUCCESS);
	}
#endif

	/*
	 * Must be -w then:
	 *
	 * write dummy header, to be fixed later
	 */
	memset (hdr, 0, sizeof(image_header_t));

	if (write(ifd, hdr, sizeof(image_header_t)) != sizeof(image_header_t)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (opt_type == IH_TYPE_MULTI || opt_type == IH_TYPE_SCRIPT) {
		char *file = datafile;
		unsigned long size;

		for (;;) {
			char *sep = NULL;

			if (file) {
				if ((sep = strchr(file, ':')) != NULL) {
					*sep = '\0';
				}

				if (stat (file, &sbuf) < 0) {
					fprintf (stderr, "%s: Can't stat %s: %s\n",
						cmdname, file, strerror(errno));
					exit (EXIT_FAILURE);
				}
				size = htonl(sbuf.st_size);
			} else {
				size = 0;
			}

			if (write(ifd, (char *)&size, sizeof(size)) != sizeof(size)) {
				fprintf (stderr, "%s: Write error on %s: %s\n",
					cmdname, imagefile, strerror(errno));
				exit (EXIT_FAILURE);
			}

			if (!file) {
				break;
			}

			if (sep) {
				*sep = ':';
				file = sep + 1;
			} else {
				file = NULL;
			}
		}

		file = datafile;

		for (;;) {
			char *sep = strchr(file, ':');
			if (sep) {
				*sep = '\0';
				copy_file (ifd, file, 1);
				*sep++ = ':';
				file = sep;
			} else {
				copy_file (ifd, file, 0);
				break;
			}
		}
	} else {
        /* EZP: add a given length of padding bytes before the kernel. */
        write_padding(ifd, kn_offset - sizeof(image_header_t), '\0');
		copy_file(ifd, datafile, 0);
	}

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (fstat(ifd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ|PROT_WRITE, MAP_SHARED, ifd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't map %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	hdr = (image_header_t *)ptr;

	checksum = crc32(0,
			  (const char *)(ptr + sizeof(image_header_t)),
			  sbuf.st_size - sizeof(image_header_t)
			 );

	/* EZP: Build new header */
	hdr->ih_magic[0] = 'A';
	hdr->ih_magic[1] = 'I';
	hdr->ih_magic[2] = 'H';
	hdr->ih_hlen  = sizeof(image_header_t);
    tmp = (char *)&hdr->ih_time;
    tmp[0] = cat; 
    tmp[1] = subcat;
    tmp[2] = (major << 4) | (minor & 0xF);
    tmp[3] = (subminor << 4);
	hdr->ih_run  = htonl(ep);
	hdr->ih_size  = htonl(sbuf.st_size - sizeof(image_header_t));
	hdr->ih_next  = htonl(0);
	hdr->ih_flags  = htonl(0);
	hdr->ih_mid  = htonl(0);
	hdr->ih_ver  = htonl(0);
	hdr->ih_resv  = htonl(0);
	snprintf(hdr->ih_desc, 16, "EZPacket Linux");

    /* EZP:
	strncpy((char *)hdr->ih_name, name, IH_NMLEN);
    */

	checksum = crc32(0,(const char *)hdr,sizeof(image_header_t));

    /* EZP: 
	hdr->ih_hcrc = htonl(checksum);
    */

	print_header (hdr);

    if (fflag) {
        int len;
        /* EZP: append rootfs with the given padding if it needs. */
        len = lseek(ifd, 0L, SEEK_SET);
        len = lseek(ifd, sbuf.st_size, SEEK_SET);
        write_padding(ifd, fs_padding, '\0');
        copy_file(ifd, fs_name, 0);
    }

	(void) munmap((void *)ptr, sbuf.st_size);

	/* We're a bit of paranoid */
#if defined(_POSIX_SYNCHRONIZED_IO) && !defined(__sun__) && !defined(__FreeBSD__)
	(void) fdatasync (ifd);
#else
	(void) fsync (ifd);
#endif

	if (close(ifd)) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	exit (EXIT_SUCCESS);
}

static void
write_padding(int ifd, int pad_len, char pad_val)
{
    int i;
    for (i = 0; i < pad_len; i++) {
        write(ifd, &pad_val, 1);
    }
}

static void
copy_file (int ifd, const char *datafile, int pad)
{
	int dfd;
	struct stat sbuf;
	unsigned char *ptr;
	int tail;
	int zero = 0;
	int offset = 0;
	int size;

	if (vflag) {
		fprintf (stderr, "Adding Image %s\n", datafile);
	}

	if ((dfd = open(datafile, O_RDONLY|O_BINARY)) < 0) {
		fprintf (stderr, "%s: Can't open %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (fstat(dfd, &sbuf) < 0) {
		fprintf (stderr, "%s: Can't stat %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	ptr = (unsigned char *)mmap(0, sbuf.st_size,
				    PROT_READ, MAP_SHARED, dfd, 0);
	if (ptr == (unsigned char *)MAP_FAILED) {
		fprintf (stderr, "%s: Can't read %s: %s\n",
			cmdname, datafile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (xflag) {
		unsigned char *p = NULL;
		/*
		 * XIP: do not append the image_header_t at the
		 * beginning of the file, but consume the space
		 * reserved for it.
		 */

		if ((unsigned)sbuf.st_size < sizeof(image_header_t)) {
			fprintf (stderr,
				"%s: Bad size: \"%s\" is too small for XIP\n",
				cmdname, datafile);
			exit (EXIT_FAILURE);
		}

		for (p=ptr; p < ptr+sizeof(image_header_t); p++) {
			if ( *p != 0xff ) {
				fprintf (stderr,
					"%s: Bad file: \"%s\" has invalid buffer for XIP\n",
					cmdname, datafile);
				exit (EXIT_FAILURE);
			}
		}

		offset = sizeof(image_header_t);
	}

	size = sbuf.st_size - offset;
	if (write(ifd, ptr + offset, size) != size) {
		fprintf (stderr, "%s: Write error on %s: %s\n",
			cmdname, imagefile, strerror(errno));
		exit (EXIT_FAILURE);
	}

	if (pad && ((tail = size % 4) != 0)) {

		if (write(ifd, (char *)&zero, 4-tail) != 4-tail) {
			fprintf (stderr, "%s: Write error on %s: %s\n",
				cmdname, imagefile, strerror(errno));
			exit (EXIT_FAILURE);
		}
	}

	(void) munmap((void *)ptr, sbuf.st_size);
	(void) close (dfd);
}

void
usage ()
{
	fprintf (stderr, "Usage: %s -l image\n"
			 "          -l ==> list image header information\n"
			 "       %s [-x] -A arch -O os -T type -C comp "
			 "-a addr -e ep -n name -d data_file[:data_file...] image\n",
		cmdname, cmdname);
	fprintf (stderr, "          -A ==> set architecture to 'arch'\n"
			 "          -O ==> set operating system to 'os'\n"
			 "          -T ==> set image type to 'type'\n"
			 "          -C ==> set compression type 'comp'\n"
			 "          -a ==> set load address to 'addr' (hex)\n"
			 "          -e ==> set entry point to 'ep' (hex)\n"
			 "          -n ==> set image name to 'name'\n"
			 "          -d ==> use image data from 'datafile'\n"
			 "          -x ==> set XIP (execute in place)\n"
			 "          -k ==> set MTD start addr for kernel to 'kn_offset'\n"
             "          -c ==> set category of the product to 'cat'\n"
             "          -s ==> set subcategory of the product to 'subcat'\n"
             "          -w ==> set major of the product to 'major'\n"
             "          -y ==> set minor of the product to 'minor'\n"
             "          -z ==> set subminor of the product to 'subminor'\n"
             "          -f ==> use rootfs data from 'fs_name'\n"
             "          -o ==> use MTD start addr for rootfs to 'fs_offset'\n"
		);
	exit (EXIT_FAILURE);
}

static void
print_header (image_header_t *hdr)
{
#if EZPACKET
	time_t timestamp;
#endif
	uint32_t size;
    char *tmp;

#if EZPACKET
	timestamp = (time_t)ntohl(hdr->ih_time);
#endif
    tmp = (char *)&hdr->ih_time;
	size = ntohl(hdr->ih_size);

	printf ("Image Name:   %.*s\n", 16, hdr->ih_desc);
#if EZPACKET
	printf ("Created:      %s", ctime(&timestamp));
#endif
    printf("Product Inform: category (%c) subcategory (%c) major (%d) minor (%d) subminor (%d) \n",
           tmp[0], tmp[1], (tmp[2] >> 4), (tmp[2] & 0xF), (tmp[3] >> 4), (tmp[3] & 0xF));
#if EZPACKET
	printf ("Image Type:   "); print_type(hdr);
#endif
	printf ("Data Size:    %d Bytes = %.2f kB = %.2f MB\n",
		size, (double)size / 1.024e3, (double)size / 1.048576e6 );
#if EZPACKET
	printf ("Load Address: 0x%08X\n", ntohl(hdr->ih_load));
	printf ("Entry Point:  0x%08X\n", ntohl(hdr->ih_ep));
#endif
	printf ("Entry Point:  0x%08X\n", ntohl(hdr->ih_run));

#if EZPACKET
	if (hdr->ih_type == IH_TYPE_MULTI || hdr->ih_type == IH_TYPE_SCRIPT) {
		int i, ptrs;
		uint32_t pos;
		unsigned long *len_ptr = (unsigned long *) (
					(unsigned long)hdr + sizeof(image_header_t)
				);

		/* determine number of images first (to calculate image offsets) */
		for (i=0; len_ptr[i]; ++i)	/* null pointer terminates list */
			;
		ptrs = i;		/* null pointer terminates list */

		pos = sizeof(image_header_t) + ptrs * sizeof(long);
		printf ("Contents:\n");
		for (i=0; len_ptr[i]; ++i) {
			size = ntohl(len_ptr[i]);

			printf ("   Image %d: %8d Bytes = %4d kB = %d MB\n",
				i, size, size>>10, size>>20);
			if (hdr->ih_type == IH_TYPE_SCRIPT && i > 0) {
				/*
				 * the user may need to know offsets
				 * if planning to do something with
				 * multiple files
				 */
				printf ("    Offset = %08X\n", pos);
			}
			/* copy_file() will pad the first files to even word align */
			size += 3;
			size &= ~3;
			pos += size;
		}
	}
#endif
}


#if EZPACKET
static void
print_type (image_header_t *hdr)
{
	printf ("%s %s %s (%s)\n",
		put_arch (hdr->ih_arch),
		put_os   (hdr->ih_os  ),
		put_type (hdr->ih_type),
		put_comp (hdr->ih_comp)
	);
}
#endif

static char *put_arch (int arch)
{
	return (put_table_entry(arch_name, "Unknown Architecture", arch));
}

static char *put_os (int os)
{
	return (put_table_entry(os_name, "Unknown OS", os));
}

static char *put_type (int type)
{
	return (put_table_entry(type_name, "Unknown Image", type));
}

static char *put_comp (int comp)
{
	return (put_table_entry(comp_name, "Unknown Compression", comp));
}

static char *put_table_entry (table_entry_t *table, char *msg, int type)
{
	for (; table->val>=0; ++table) {
		if (table->val == type)
			return (table->lname);
	}
	return (msg);
}

static int get_arch(char *name)
{
	return (get_table_entry(arch_name, "CPU", name));
}


static int get_comp(char *name)
{
	return (get_table_entry(comp_name, "Compression", name));
}


static int get_os (char *name)
{
	return (get_table_entry(os_name, "OS", name));
}


static int get_type(char *name)
{
	return (get_table_entry(type_name, "Image", name));
}

static int get_table_entry (table_entry_t *table, char *msg, char *name)
{
	table_entry_t *t;
	int first = 1;

	for (t=table; t->val>=0; ++t) {
		if (t->sname && strcasecmp(t->sname, name)==0)
			return (t->val);
	}
	fprintf (stderr, "\nInvalid %s Type - valid names are", msg);
	for (t=table; t->val>=0; ++t) {
		if (t->sname == NULL)
			continue;
		fprintf (stderr, "%c %s", (first) ? ':' : ',', t->sname);
		first = 0;
	}
	fprintf (stderr, "\n");
	return (-1);
}
