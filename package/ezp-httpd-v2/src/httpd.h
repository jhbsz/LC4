/*
 * milli_httpd - pretty small HTTP server
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: httpd.h,v 1.13 2005/03/07 08:35:32 kanki Exp $
 */

#ifndef _HTTPD_H_
#define _HTTPD_H_

#if defined(DEBUG) && defined(DMALLOC)
#include <dmalloc.h>
#endif

#if defined (HTTPS_SUPPORT)
#include <openssl/ssl.h>
extern BIO *bio_err;
#endif

/* Basic authorization userid and passwd limit */
#define AUTH_LEN            64

#define LINE_SIZE           80
#define PAGE_SIZE           8192
#define AUTHORIZATION       "Authorization:"
#define AUTHORIZATION_LEN   (sizeof("Authorization:") - 1)
#define COOKIE              "cookie:"
#define COOKIE_LEN          (sizeof("cookie:") - 1)
#define CONTENT_LENGTH      "Content-Length:"
#define CONTENT_LENGTH_LEN  (sizeof("Content-Length:") - 1)
#define BOUNDARY            "boundary="
#define BOUNDARY_LEN        (sizeof("boundary=") - 1)

#define DEFAULT_HTTPS_PORT  443
#define CERT_FILE           "/var/cert.pem"
#define KEY_FILE            "/var/key.pem"

#define SERVER_NAME         "EZPacket Web Server"
#define PROTOCOL            "HTTP/1.1"
#define RFC1123FMT          "%a, %d %b %Y %H:%M:%S GMT"
#define TIMEOUT	            15
#define DEFAULT_HTTP_PORT   80

#ifdef AXIMDDNS
#define EZP_RETURN_MESSAGE      "9"
#endif
#define EZP_RETURN_RESTART      "3"
#define EZP_RETURN_LOGIN_SUCCESS      "2"
#define EZP_RETURN_REBOOT       "1"
#define EZP_RETURN_SUCCESS      "0"
#define EZP_RETURN_NOCHANGE     "-1"
#define EZP_RETURN_FAIL         "-2"

typedef FILE *webs_t;
extern char *wfgets_timeout(char *buf, int len, FILE * fp, int timeout_sec);
extern char *wfgets(char *buf, int len, FILE * fp);
extern int wfputc(char c, FILE * fp);
extern int wfputs(char *buf, FILE * fp);
extern int wfprintf(FILE * fp, char *fmt, ...);
extern size_t wfwrite(char *buf, int size, int n, FILE * fp);
extern size_t wfread(char *buf, int size, int n, FILE * fp);
extern int wfflush(FILE * fp);
extern int wfclose(FILE * fp);

extern int do_ssl;
extern char dir[LINE_SIZE];

/* enum for web access permission */
enum {
    DISALLOW = 1,
    GUEST,
    FULL_ALLOW
};


/* Generic MIME type handler */
struct mime_handler
{
    char *pattern;
    char *mime_type;
    char *extra_header;
    void (*input) (char *path, FILE * stream, int len, char *boundary);
    void (*output) (char *path,  FILE * stream, ...);
    void (*auth) (char *userid, char *passwd, char *realm);
};
extern struct mime_handler mime_handlers[];

/* CGI helper functions */
extern void init_cgi(char *query);
extern char *get_cgi(char *name);
extern void set_cgi(char *name, char *value);
extern int count_cgi();
extern void unescape(char *s);
extern int get_postact_restart(void);

/* Regular file handler */
extern void do_file(char *path, webs_t stream, ...);

/* GoAhead 2.1 compatibility */
typedef char char_t;
#define T(s) (s)
#define __TMPVAR(x) tmpvar ## x
#define _TMPVAR(x) __TMPVAR(x)
#define TMPVAR _TMPVAR(__LINE__)
#define websWrite(wp, fmt, args...) ({ int TMPVAR = wfprintf(wp, fmt, ## args); wfflush(wp); TMPVAR; })
#define websWrite2 fprintf
#define websDebugWrite(wp, fmt, args...)                            \
            ({ error_value = 1;                                     \
               if(!config_validate_msg)                             \
                    wfputs("message=", wp);                            \
               int TMPVAR = wfprintf(wp, fmt, ## args);             \
               if(!config_validate_msg) wfputs("\n", wp);          \
               wfflush(wp); TMPVAR; })
#define websError(wp, code, msg, args...) printf(msg, ## args)
#define websHeader(wp) wfputs("<html lang=\"en\">", wp)
#define websFooter(wp) wfputs("</html>", wp)
#define websDone(wp, code) wfflush(wp)
#define websGetVar(wp, var, default) (get_cgi(var) ? : default)
#define websSetVar(wp, var, value) set_cgi(var, value)
#define websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query) ({ do_ej(path, wp); fflush(wp); 1; })
#define websWriteData(wp, buf, nChars) ({ int TMPVAR = wfwrite(buf, 1, nChars, wp); wfflush(wp); TMPVAR; })
#define websWriteDataNonBlock websWriteData
#define a_assert(a)

extern int ejArgs(int argc, char_t ** argv, char_t * fmt, ...);

/* GoAhead 2.1 Embedded JavaScript compatibility */
extern void do_ej(char *path, webs_t stream, ...);
struct ej_handler
{
    char *pattern;
    int (*output) (int eid, webs_t wp, int argc, char_t ** argv);
};
extern struct ej_handler ej_handlers[];

#define PROD_CAT_B (0x01L << 0)   /* PG */
#define PROD_CAT_L (0x01L << 1)   /* Load Balance */
#define PROD_CAT_M (0x01L << 2)   /* MR */
#define PROD_CAT_N (0x01L << 3)   /* MR Load Balance */
#define PROD_CAT_C (0x01L << 4)   /* PGP */
#define PROD_CAT_D (0x01L << 5)   /* PGP Dual WANs*/
#define PROD_CAT_A (0x01L << 6)   /* AG */
#define PROD_CAT_T (0x01L << 7)   /* PGP Wired */
#define PROD_CAT_V (0x01L << 8)   /* VLAN AP */
#define PROD_CAT_P (0x01L << 9)   /* GOGO MR */
#define PROD_CAT_K (0x01L << 10)   /* NAS 101 */
#define PROD_CAT_U (0x01L << 11)   /* MR-105NL */
#define PROD_CAT_G (0x01L << 12)   /* PGP GIGA*/
#define PROD_CAT_I (0x01L << 13)   /* MR GIGA */ 
#define PROD_CAT_W (0x01L << 14)   /* MR-102N WHITE */ 
#define PROD_CAT_O (0x01L << 15)   /* For Planet */
#define PROD_CAT_R (0x01L << 16)   /* For Sony */
/* For TOGOBox and TOGOMusic, like Mosra, they don't have submenus, only
 * single page for each menu */
#define PROD_CAR_ONE_PAGE_SUBMENU  (0x01L << (sizeof(int)*8 - 1))
#endif /* _HTTPD_H_ */
