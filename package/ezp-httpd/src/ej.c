/*
 * Tiny Embedded JavaScript parser
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: ej.c,v 1.9 2005/03/07 08:35:32 kanki Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "ezp.h"
#include "common.h"

static char *get_arg(char *args, char **next);
static void call(char *func, webs_t stream);

/* Internal variable to determine whether HTML code is allowed to be printed. */
static int ej_print = 1;

/* Look for unquoted character within a string */
static char *
unqstrstr(char *haystack, char *needle)
{
    char *cur;
    int q;
    int hlen = strlen(haystack);
    int nlen = strlen(needle);

    for (cur = haystack, q = 0;
         cur < &haystack[hlen] && !(!q && !strncmp(needle, cur, nlen));
         cur++) {
        if (*cur == '"')
            q ? q-- : q++;
    }
    return (cur < &haystack[hlen]) ? cur : NULL;
}

static char *
get_arg(char *args, char **next)
{
    char *arg, *end;

    /* Parse out arg, ... */
    if (!(end = unqstrstr(args, ","))) {
        end = args + strlen(args);
        *next = NULL;
    } else
        *next = end + 1;

    /* Skip whitespace and quotation marks on either end of arg */
    for (arg = args; isspace((int) *arg) || *arg == '"'; arg++);
    for (*end-- = '\0'; isspace((int) *end) || *end == '"'; end--)
        *end = '\0';

    return arg;
}

static void
call(char *func, webs_t stream)
{
    char *args, *end, *next;
    int argc;
    char *argv[16];
    struct ej_handler *handler;

    /* Parse out ( args ) */
    if (!(args = strchr(func, '(')))
        return;
    if (!(end = unqstrstr(func, ")")))
        return;
    *args++ = *end = '\0';

    /* Set up argv list */
    for (argc = 0; args && *args && argc < 16; argc++, args = next) {
        if (!(argv[argc] = get_arg(args, &next)))
            break;
    }

    /* Call handler */
    for (handler = &ej_handlers[0]; handler->pattern; handler++) {
        if (strcmp(handler->pattern, func) == 0) {
#ifdef DEBUG
            printf("Found handler: %s\n", func);
#endif

            handler->output(0, stream, argc, argv);
            return;
        }
    }

    /* EZP: handler->pattern must be NULL after finishing the loop. */
#ifdef DEBUG
    printf("Not found handler: %s\n", func);
#endif
}

void 
als_do_ej(char *file , webs_t stream , ...)
{
    char path[512],pattern[512];
    char p_1[8]="1",p_2[8]="0";

    sscanf(file,"%[^?]?%s",path,pattern);
    sscanf(pattern,"%[^&]&%s",p_1,p_2);

    do_ej(&path,stream,p_1,p_2,"");
}

void
do_ej(char *file, webs_t stream, ...)
{
    FILE *fp;
    int c;
    char pattern[512], *asp = NULL, *func = NULL, *end = NULL;
#define DO_EJ_PARA_MAX  8
    /* At most 'DO_EJ_PARA_MAX' parameters are allowed.
     * DO_EJ_PARA_MAX could not be more than 10 because $10 will be recognized
     * as $1.
     */
    char paras[DO_EJ_PARA_MAX][SHORT_BUF_LEN];
    int len = 0;
    int i = 0;
    va_list ap;
    char *para;

    if (!(fp = fopen(file, "r")))
        return;

    /* NOTE: va_arg() doesn't correctly return NULL if there is no parameter.
     * Therefore, we put an empty string as the last parameter for every
     * do_ej() function.
     * eg., do_ej("file.asp, "");
     */
    va_start(ap, stream);
    para = va_arg(ap, char *);
    while (para && *para) {
        strcpy(paras[i], para);
        i++;
        if (i >= DO_EJ_PARA_MAX) {
            break;
        }
        para = va_arg(ap, char *);
    } 
    
    /* i indicates the number of parameters for replacement. */
    while ((c = getc(fp)) != EOF) {
        if (c == '$') {
            c = getc(fp);
            if (c == EOF) {
                break;
            }
            if ((c - '0') < i && c - '0' >= 0) {
                /* $0 ~ $i */
                strcpy(&pattern[len], paras[c - '0']);
                len += strlen(paras[c - '0']);
            } else {
                /* Not what we want to replace. */
                sprintf(&pattern[len], "$%c", c);
                len += 2;
            }
            pattern[len] = '\0';
            continue;
        }
        /* Add to pattern space */
        pattern[len++] = c;
        pattern[len] = '\0';
        if (len == (sizeof(pattern) - 1))
            goto release;

        /* Look for <% ... */

        /* EZP: If "<%" appears, it will store in the "pattern buffer". */
       
        if (!asp && !strncmp(pattern, "<%", len)) {
            if (len == 2)
                asp = pattern + 2;
            continue;
        }

        /* Look for ... %> */
        if (asp) {
            if (unqstrstr(asp, "%>")) {
                /* The iterator could be ended before the string "%>". */
                for (func = asp; func < &pattern[len - 3]; func = end) {
                    /* Skip initial whitespace */
                    for (; isspace((int) *func); func++);
                    if (!(end = unqstrstr(func, ";")))
                        break;
                    *end++ = '\0';
                    /* 
                     * Either if allowed to print or if "do_print_end()", 
                     * let's execute the ej function. 
                     */
                    if (ej_print || !strncmp(func, "do_print_end", 
                                strlen("do_print_end"))) {
                        call(func, stream);
                    }
                }
                asp = NULL;
                len = 0;
            }
            continue;
        }
release:
        /* Release pattern space */
        if (ej_print) {
            if (wfputs(pattern, stream) == EOF) {
                printf("wfputs(pattern, stream) Error : code %d\n", ferror(stream));
                break;
            }
        }
        len = 0;
    }

    fclose(fp);
}

int
ejArgs(int argc, char **argv, char *fmt, ...)
{
    va_list ap;
    int arg;
    char *c;

    if (!argv)
        return 0;

    va_start(ap, fmt);
    for (arg = 0, c = fmt; c && *c && arg < argc;) {
        if (*c++ != '%')
            continue;
        switch (*c) {
        case 'd':
            /* For some special reason, we might exceptionally feed 
             * parameter(s) to the ej functions which generally don't accept
             * the paramer(s).
             */ 
            if (arg >= argc) {
                *(va_arg(ap, int *)) = 0xFFFFFFFF;
            } else {
                *(va_arg(ap, int *)) = atoi(argv[arg]);
            }
            break;
        case 's':
            if (arg >= argc) {
                *(va_arg(ap, char **)) = NULL;
            } else {
                *(va_arg(ap, char **)) = argv[arg];
            }
            break;
        }
        arg++;
    }
    va_end(ap);

    return arg;
}

int
ej_do_print_begin(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *str;
    int level, cur_level;

    if (ejArgs(argc, argv, "%s", &str) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sscanf(str, "EZP_PRODUCT_LEVEL_%d", &level);
    
    str = nvram_safe_get("prod_level");
    if (!*str)
        cur_level = 1;
    cur_level = atoi(str);

    if (cur_level < level) {
        ej_print = 0;
    }

    return 0;
}

int
ej_do_print_end(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *str;
    int level, cur_level;

    if (ejArgs(argc, argv, "%s", &str) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sscanf(str, "EZP_PRODUCT_LEVEL_%d", &level);
    
    str = nvram_safe_get("prod_level");
    if (!*str)
        cur_level = 1;
    cur_level = atoi(str);

    if (cur_level < level) {
        ej_print = 1;
    }

    return 0;
}
