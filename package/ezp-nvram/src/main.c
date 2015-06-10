/*
 * Frontend command-line utility for Linux NVRAM layer
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: main.c 3089 2006-01-31 15:22:43Z wbx $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <typedefs.h>
#include <bcmnvram.h>
#include "ezp-lib.h"

static void
usage(void)
{
	fprintf(stderr, "usage: nvram [get name] [set name=value] \n [unset name] [show] [default] [factory] \n [import filename] [export filename]\n [upgrade] [downgrade version]");
	exit(0);
}

/* hack for some PMON default nvram values which have '\r' appended */
static void
puts_trim_cr(char *str)
{
	int len;
    
    if (!str)   return;

    len = strlen(str);
	if (len && (str[len-1] == '\r')) len--;
	printf("%.*s\n", len, str);
}

/* NVRAM utility */
int
main(int argc, char **argv)
{
	char *name, *value, buf[NVRAM_SPACE];
    char res[EZPLIB_BUF_LEN];
	int size;

	/* Skip program name */
	--argc;
	++argv;

	if (!*argv) {
		usage();
	}

	/* Process the remaining arguments. */
    if (!strncmp(*argv, "get", 3)) {
        if (*++argv) {
            if ((value = nvram_get(*argv))) {
                puts_trim_cr(value);
#if !defined(NVRAM_MMAP) || NVRAM_MMAP == 0
                free(value) ;
#endif
            }
            return 0;
        } else {
            return 1;
        }
    } else if (!strncmp(*argv, "set", 3)) {
        if (*++argv) {
            strncpy(value = buf, *argv, sizeof(buf));
            name = strsep(&value, "=");
            /* Honor write permission. */
            nvram_set(name, value);
            return 0;
        } else {
            return 1;
        }
    } else if (!strncmp(*argv, "fset", 4)) {
        if (*++argv) {
            strncpy(value = buf, *argv, sizeof(buf));
            name = strsep(&value, "=");
            /* Force write action. */
            nvram_fset(name, value);
            return 0;
        } else {
            return 1;
        }
    } else if (!strncmp(*argv, "unset", 5)) {
        if (*++argv) {
            nvram_unset(*argv);
            return 0;
        } else {
            return 1;
        }
    } else if (!strncmp(*argv, "commit", 5)) {
        nvram_commit();
        return 0;
    } else if (!strncmp(*argv, "show", 4)) {
        --argc;
        ++argv;

        if (argc == 0) {
            /* nvram show */
            nvram_getall(buf, sizeof(buf));
            for (name = buf; *name; name += strlen(name) + 1)
                puts_trim_cr(name);
            size = sizeof(struct nvram_header) + (int) name - (int) buf;
            fprintf(stderr, "size: %d bytes (%d left)\n", 
                    size, NVRAM_SPACE - size);
            return 0;
        } else if (argc == 2) {
            /* nvram show <rule-set> <nth> */
            ezplib_get_rule(argv[0], atoi(argv[1]), res, EZPLIB_BUF_LEN);
            puts_trim_cr(res);
            /* TODO: fix the return value. */
            return 0;
        } else if (argc == 3) {
            /* nvram show <rule-set> <nth> <attr-type> */
            ezplib_get_attr_val(argv[0], atoi(argv[1]), argv[2], res, 
                    EZPLIB_BUF_LEN, EZPLIB_USE_CLI);
            puts_trim_cr(res);
            /* TODO: fix the return value. */
            return 0;
        } else if (argc == 5 && !strncmp(argv[2], "subrule", strlen(argv[1]))) {
            /* nvram show <rule-set> <nth> subrule <start> <end> */
            ezplib_get_subrule(argv[0], atoi(argv[1]), atoi(argv[3]), 
                    atoi(argv[4]), res, EZPLIB_BUF_LEN);
            puts_trim_cr(res);
            /* TODO: fix the return value. */
            return 0;
        }
    } else if (!strncmp(argv[0], "replace", strlen(argv[0]))) {
        if (!strncmp(argv[1], "rule", strlen(argv[1]))) {
            argc -= 2;
            argv += 2;
            if (argc == 3) {
                int ret;

                /* nvram replace rule <rule-set> <nth> <new-rule> */
                ret = 
                    ezplib_replace_rule(argv[0], atoi(argv[1]), 
                            argv[2]);
                if (ret < 0) {
                    printf("NVRAM replace rule failed: %s\n", ret);
                    return 1;
                } else  {
                    return 0;
                }
            }
        } else if (!strncmp(argv[1], "attribute", strlen(argv[1]))) {
            argc -= 2;
            argv += 2;
            if (argc == 4) {
                int ret;

                /* nvram replace attr <rule-set> <nth> <attr> 
                 * <new-rule> 
                 */
                ret = 
                    ezplib_replace_attr(argv[0], atoi(argv[1]), argv[2], 
                            argv[3]);
                if (ret < 0) {
                    printf("NVRAM replace attribute failed: %d\n", ret);
                    return 1;
                } else  {
                    return 0;
                }
            }
        }
    } else if (!strncmp(argv[0], "append", strlen(argv[0])) && 
            !strncmp(argv[1], "rule", strlen(argv[1]))) {
        argc -= 2;
        argv += 2;
        if (argc == 2) {
            int ret;

            /* nvram append rule <rule-set> <new-rule> */
            ret = ezplib_append_rule(argv[0], argv[1]);
            if (ret < 0) {
                printf("NVRAM append rule failed: %s\n", ret);
                return 1;
            } else  {
                return 0;
            }
        }
    } else if (!strncmp(argv[0], "prepend", strlen(argv[0])) && 
            !strncmp(argv[1], "rule", strlen(argv[1]))) {
        argc -= 2;
        argv += 2;
        if (argc == 2) {
            int ret;

            /* nvram prepend rule <rule-set> <new-rule> */
            ret = ezplib_prepend_rule(argv[0], argv[1]);
            if (ret < 0) {
                printf("NVRAM prepend rule failed: %s\n", ret);
                return 1;
            } else  {
                return 0;
            }
        }
    } else if (!strncmp(argv[0], "add", strlen(argv[0])) && 
            !strncmp(argv[1], "rule", strlen(argv[1]))) {
        argc -= 2;
        argv += 2;
        if (argc == 3) {
            int ret;

            /* nvram add rule <rule-set> <nth> <new-rule> */
            ret = ezplib_add_rule(argv[0], atoi(argv[1]), argv[2]);
            if (ret < 0) {
                printf("NVRAM add rule failed: %s\n", ret);
                return 1;
            } else  {
                return 0;
            }
        }
    } else if (!strncmp(argv[0], "delete", strlen(argv[0])) && 
            !strncmp(argv[1], "rule", strlen(argv[1]))) {
        argc -= 2;
        argv += 2;
        if (argc == 2) {
            int ret;

            /* nvram delete rule <rule-set> <nth> */
            ret = ezplib_delete_rule(argv[0], atoi(argv[1]));
            if (ret < 0) {
                printf("NVRAM delete rule failed: %s\n", ret);
                return 1;
            } else {
                return 0;
            }
        }
    } else if (!strncmp(argv[0], "rule", strlen(argv[0])) && 
            !strncmp(argv[1], "num", strlen(argv[1]))) {
        argc -= 2;
        argv += 2;
        if (argc == 1) {
            int ret;

            /* nvram rule num <rule-set> */
            ret = ezplib_get_rule_num(argv[0]);
            if (ret < 0) {
                printf("NVRAM delete rule failed: %s\n", ret);
                return ret;
            } else  {
                return 0;
            }
        }
    }
    /* EZP: Set default values for EZ Packet Products. */
    else if (!strncmp(*argv, "boot", 4)) {
        nvram_boot(); 
        return 0;
    }
    /* EZP: Upgrade NVRAM settings to newer version. */
    else if (!strncmp(*argv, "upgrade", 7)) {
        argc -= 1;
        argv += 1;
        if (nvram_upgrade((argc >= 1)? argv[0]: NULL)) {
            nvram_commit();
        }
        return 0;
    }
    /* EZP: Upgrade NVRAM settings to older version. */
    else if (!strncmp(*argv, "downgrade", 9)) {
        argc -= 1;
        argv += 1;
        if (argc == 1) {
            if (nvram_downgrade(argv[0])) {
                nvram_commit();
            }
        } else {
            fprintf(stderr, "nvram downgrade error\n");
        }
        return 0;
    }
    else if (!strncmp(*argv, "default", 7)) {
        if(argc == 1) {
            nvram_default(); 
        }
        else {
            /* Added for single rule default */
            argv++;
            nvram_default_rule(*argv); 
        }
        return 0;
    }
    else if (!strncmp(*argv, "factory", 7)) {
        nvram_default(); 
        nvram_commit();
        /* send SIGTERM to init for reboot */
        kill(1, 15);
        return 0;
    }
    else if (!strncmp(argv[0], "import", 6)) {
        return nvram_import(argv[1]); 
    }
    else if (!strncmp(argv[0], "export", 6)) {
        nvram_export(argv[1]); 
        return 0;
    }

	return 1;
}	
