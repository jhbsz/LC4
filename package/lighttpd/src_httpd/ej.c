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

#include "lang.h"
#include "fcgi_common.h"
#include "fcgi_stdio.h"
#include "http_common.h"

static char *get_arg(char *args, char **next);
void call(char *func, INPUT *input);
char *unqstrstr(char *haystack, char *needle);
static char *get_arg(char *args, char **next);

/* Look for unquoted character within a string */
char *
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

void
call(char *func, INPUT *input)
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

            handler->output(0, input, argc, argv);
            return;
        }
    }

    /* EZP: handler->pattern must be NULL after finishing the loop. */
#ifdef DEBUG
    printf("Not found handler: %s\n", func);
#endif
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

int pattern_is_newline = 0;

void
do_ej(char *file, INPUT *input, ...)
{
    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));
    inital_prod();

    FILE *fp;
    char http_folder[128]={0}, pattern[512]={0};
    char *asp = NULL, *func = NULL, *end = NULL;
    int c, len = 0;

#define DO_EJ_PARA_MAX  8

    char paras[DO_EJ_PARA_MAX][SHORT_BUF_LEN];
    int i=0;
    va_list ap;
    char *para;

    if(!input) input = CGI_INIT();
    strcpy(http_folder, "/tmp/mnt/");
    strcat(http_folder, file);

    va_start(ap, input);
    para = va_arg(ap, char *);
    while (para && *para) {
        strcpy(paras[i], para);
        i++;
        if (i >= DO_EJ_PARA_MAX) {
            break;
        }
        para = va_arg(ap, char *);
    }

    fp = fopen(http_folder, "r");
    if( fp!=NULL ) {
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
                } 
                else {
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
            if (!asp && !strncmp(pattern, "<%", len)) {
                if (len == 2)
                    asp = pattern + 2;
                continue;
            }

            if (asp) {
                if (unqstrstr(asp, "%>")) {
                    /* The iterator could be ended before the string "%>". */
                    for (func = asp; func < &pattern[len - 3]; func = end) {
                        /* Skip initial whitespace */
                        for (; isspace((int) *func); func++);
                            if (!(end = unqstrstr(func, ";")))
                                break;
                            *end++ = '\0';
                            call(func, input);
                    }
                    asp = NULL;
                    len = 0;
                }
                continue;
            }
            len = 0;
            //if(!strcmp(pattern,"\n")) {
            if((c == '\n') || (c == '\r')) {
                if(!pattern_is_newline) {
                    printf("%s", pattern);
                }
                pattern_is_newline = 1;
            } 
            else {
                pattern_is_newline = 0;
                printf("%s", pattern);
            }

        }
        fclose(fp);
    }
}
