/* milli_httpd - pretty small HTTP server
** A combination of
** micro_httpd - really small HTTP server
** and
** mini_httpd - small HTTP server
**
** Copyright � 1999,2000 by Jef Poskanzer <jef@acme.com>.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdarg.h>
#include <syslog.h>

#include "httpd.h"
#include <bcmnvram.h>
#include <utils.h>
#include <shutils.h>

#if defined (HTTPS_SUPPORT)
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#include <error.h>
#include <sys/signal.h>

#include "common.h"
#include "lang.h"
#include "ezp-lib.h"
#include "shared.h"

/* A multi-family sockaddr. */
typedef union
{
    struct sockaddr sa;
    struct sockaddr_in sa_in;
} usockaddr;

typedef struct {
    struct in_addr ipaddr;
    char mac[20];
}peer;

/* Globals. */
#if defined(HTTPS_SUPPORT)
static SSL *ssl;
#endif

#ifdef DEBUG_CIPHER
int get_ciphers = 0;
char *set_ciphers = NULL;
#endif
int do_ssl;
int refuse_request = 0;
char pid_file[TMP_LEN];
static webs_t conn_fp;
static char auth_userid[AUTH_LEN];
static char auth_passwd[AUTH_LEN];
static char auth_realm[AUTH_LEN];
int auth_state = 1;
int post;
int httpd_level;
int server_port;
char server_ipaddr[32];
peer client;
int conn_fd;

static int initialize_listen_socket(usockaddr * usaP);
/*
static int auth_check(char *dirname, char *authorization);
static void send_authenticate(char *realm);*/
static void send_error(int status, char *title, char *extra_header,
                       char *text);
static void send_headers(int status, char *title, char *extra_header,
                         char *mime_type);
static int match(const char *pattern, const char *string);
static int match_one(const char *pattern, int patternlen,
                     const char *string);
static void handle_request(void);

unsigned long PROD_CAT_BITMAP;
int ezp_prod_subcat;
char ezp_prod_cat[4];
char ezp_prod_subsubcat[4];
char *prod_subsubcat;
void inital_prod();

static int
initialize_listen_socket(usockaddr * usaP)
{
    int listen_fd;
    int i;

    memset(usaP, 0, sizeof(usockaddr));
    usaP->sa.sa_family = AF_INET;
    if (*server_ipaddr) {
        /* Bind the internal interface. */
        usaP->sa_in.sin_addr.s_addr = inet_addr(server_ipaddr);
    } else {
        /* Bind the external interface. */
        usaP->sa_in.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    usaP->sa_in.sin_port = htons(server_port);

    listen_fd = socket(usaP->sa.sa_family, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        return -1;
    }
    (void) fcntl(listen_fd, F_SETFD, 1);
    i = 1;
    if (setsockopt
        (listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &i,
         sizeof(i)) < 0) {
        perror("setsockopt");
        return -1;
    }
    if (bind(listen_fd, &usaP->sa, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(listen_fd, 1024) < 0) {
        perror("listen");
        return -1;
    }
    return listen_fd;
}

/* We don't use this checking anymore */
#if 0
static int
auth_check(char *dirname, char *authorization)
{
    char authinfo[500];
    char *authpass;
    int l;

    /* Basic authorization info? */
    if (!authorization || strncmp(authorization, "Basic ", 6) != 0) {
        //send_authenticate( dirname );
#ifdef EZPACKET_LOG
        ct_syslog(LOG_INFO, httpd_level, "Authentication fail");
#endif 
        return 0;
    }

    /* Decode it. */
    l = b64_decode(&(authorization[6]), authinfo, sizeof(authinfo));
    authinfo[l] = '\0';
    /* Split into user and passwd. */
    authpass = strchr(authinfo, ':');
    if (authpass == (char *) 0) {
        /* No colon?  Bogus auth info. */
        //send_authenticate( dirname );
        return 0;
    }
    *authpass++ = '\0';

    /* Is this the right user and passwd? */
    if (!strcmp(auth_userid, authinfo) && !strcmp(auth_passwd, authpass)) {
        return 1;
    }
    //send_authenticate( dirname );
    return 0;
}
#endif

static void getCookie(char * cookie, char * name, char * value)
{
    char tar[TMP_LEN];
    char *pos_S, *pos_E;
    strcpy(tar, name);
    strcat(tar, "=");
    if ((pos_S = strstr(cookie, tar)) != NULL) {
        pos_S += strlen(name) + 1;
        if ((pos_E = strstr(pos_S, ";")) == NULL) {
            pos_E = cookie + strlen(cookie) - 1;
        }
        strncpy(value, pos_S, pos_E - pos_S);
    }
}

static void parse_cookie_info(char * cookie, char * auth)
{
    if (strlen(cookie) == 0)
        return;

    unescape(cookie);
    getCookie(cookie, "auth", auth);
}

static int
auth_check2(char *dirname, char *authorization)
{
    char authinfo[TMP_LEN] = {0};
    char b64_auth_passwd[TMP_LEN] = {0};
    char user[TMP_LEN] = {0}, pw[TMP_LEN] = {0};
    char *pos, togo_passwd[TMP_LEN];
    unsigned long l = TMP_LEN;

    /* Decode it. */
    base64_decode((unsigned char *)authorization, strlen(authorization),
                                   (unsigned char *)authinfo, &l);
    authinfo[l] = '\0';

    /* Split into user and passwd. */
    if ((pos = strchr(authinfo, ':')) == NULL) {
        return 1;
    }
    /* get user */
    strncpy(user, authinfo, pos - authinfo);
    /* get password */
    strcpy(pw, pos + 1);

    /* Is this the right user and passwd? */
    ezplib_get_attr_val("lan_hwaddr_rule_default", 0, "hwaddr", togo_passwd, 
            TMP_LEN, EZPLIB_USE_CLI);
    l = TMP_LEN;
    if (base64_decode((unsigned char *)auth_passwd, strlen(auth_passwd),
                                   (unsigned char *)b64_auth_passwd, &l)) {
        strncpy(b64_auth_passwd,auth_passwd,strlen(auth_passwd));
    } else {
        b64_auth_passwd[l] = '\0';
    }

#ifdef EZP_SUB_BRAND_GARMIN
/* we add a default user "nji56vfe" & passwd "xdr12yjm" for GARMIN project GMN device use */
    if ((strcmp("nji56vfe", user)==0) && (strcmp("xdr12yjm", pw)==0)) {
        return 0;
    }
#endif 
    if (strcmp(auth_userid, user)) {
        if ((strcmp("togo", user) || strcmp(togo_passwd, pw))) return 1;
    } else if (strcmp(b64_auth_passwd, pw) && strcmp(auth_passwd, pw)) {
        return 2;
    }
    return 0;
}

/* We don't use this checking anymore 
static void
send_authenticate(char *realm)
{
    char header[10000];

    (void) snprintf(header, sizeof(header),
            "WWW-Authenticate: Basic realm=\"%s\"", realm);
    send_error(401, "Unauthorized", header, "Authorization required.");
}
*/

static void
send_error(int status, char *title, char *extra_header, char *text)
{

    send_headers(status, title, extra_header, "text/html");
    (void) wfprintf(conn_fp,
                    "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\n"
                    "<BODY><H2>%d %s</H2>\n",
                    status, title, status, title);
    (void) wfprintf(conn_fp, "%s\n", text);
    (void) wfprintf(conn_fp, "</BODY></HTML>\n");
    (void) wfflush(conn_fp);
}

static void
send_headers(int status, char *title, char *extra_header, char *mime_type)
{
    time_t now;
    char timebuf[100];

    wfprintf(conn_fp, "%s %d %s\r\n", PROTOCOL, status, title);
    wfprintf(conn_fp, "Server: %s\r\n", SERVER_NAME);
    now = time((time_t *) 0);
    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
    wfprintf(conn_fp, "Date: %s\r\n", timebuf);
    if (extra_header != (char *) 0 && *extra_header)
        wfprintf(conn_fp, "%s\r\n", extra_header);
    if (mime_type != (char *) 0)
        wfprintf(conn_fp, "Content-Type: %s; charset=%s\r\n", mime_type, 
        lang_charset);
    wfprintf(conn_fp, "Connection: close\r\n");
    wfprintf(conn_fp, "\r\n");
}


/* Simple shell-style filename matcher.  Only does ? * and **, and multiple
 * patterns separated by |.  Returns 1 or 0.
 */
static int
match(const char *pattern, const char *string)
{
    const char *or;

    for (;;) {
        or = strchr(pattern, '|');
        if (or == (char *) 0)
            return match_one(pattern, strlen(pattern), string);
        if (match_one(pattern, or - pattern, string))
            return 1;
        pattern = or + 1;
    }
}


static int
match_one(const char *pattern, int patternlen, const char *string)
{
    const char *p;

    for (p = pattern; p - pattern < patternlen; ++p, ++string) {
        if (*p == '?' && *string != '\0')
            continue;
        if (*p == '*') {
            int i, pl;
            ++p;
            if (*p == '*') {
                /* Double-wildcard matches anything. */
                ++p;
                i = strlen(string);
            } else
                /* Single-wildcard matches anything but slash. */
                i = strcspn(string, "/");
            pl = patternlen - (p - pattern);
            for (; i >= 0; --i)
                if (match_one(p, pl, &(string[i])))
                    return 1;
            return 0;
        }
        if (*p != *string)
            return 0;
    }
    if (*string == '\0')
        return 1;
    return 0;
}


void
do_file(char *path, webs_t stream, ...)
{
    FILE *fp;
    int c;

    if (!(fp = fopen(path, "r")))
        return;
    while ((c = getc(fp)) != EOF) {
        if(wfputc(c, stream) == EOF) {
            printf("wfputc(c, stream) Error , code : %d\n", ferror(stream));
            break;
        }
    }
    fclose(fp);
}

static void
handle_request(void)
{
    char line[PAGE_SIZE], *cur;
    char *method, *path, *protocol, *boundary;
    char *cookie, authorization[TMP_LEN] = {0};
    char *cp;
    char *file;
    char wan_state[5];
    struct mime_handler *handler;
    int cl = 0, flags;

    if (refuse_request) {
        return;
    }

    /* Initialize the request variables. */
    cookie = boundary = NULL;
    bzero(line, sizeof line);

    /* Parse the first line of the request. */
    if (wfgets(line, sizeof(line), conn_fp) == (char *) 0) {
        send_error(400, "Bad Request", (char *) 0, "No request found.");
        return;
    }

    /* Only GET/POST is accepted. */
    if (!strncmp(line, "GET", 3) && !strncasecmp(line, "POST", 4)) {
        send_error(501, "Not Implemented", (char *) 0,
                   "That method is not implemented.");
        return;
    }

    /* Path. */
    method = path =line;
    strsep(&path, " ");
    if (!path) {
        send_error(400, "Bad Request", (char *) 0, "Illegal path.");
        return;
    } 
    while (*path == ' ') {
        path++;
    }

    /* Protocol. */
    protocol = path;
    strsep(&protocol, " ");
    if (!protocol) {    
        send_error(400, "Bad Request", (char *) 0, "Illegal protocol.");
        return;
    }
    while (*protocol == ' ')
        protocol++;

    /* Other headers. */
    cp = protocol;
    strsep(&cp, " ");
    cur = protocol + strlen(protocol) + 1;

    while (wfgets(cur, line + sizeof(line) - cur, conn_fp) != 0) {
        if (*cur == '\n' || strcmp(cur, "\r\n") == 0) {
            /* The end of header: \r\n\r\n or \n\n */
            break;
        } else if (strncasecmp(cur, COOKIE, COOKIE_LEN) == 0) {
            /* cookie */
            cp = &cur[COOKIE_LEN];
            cp += strspn(cp, " \t");
            cookie = cp;
            cur = cp + strlen(cp) + 1;
        } else if (strncasecmp(cur, CONTENT_LENGTH, CONTENT_LENGTH_LEN) == 0) {
            /* Content-Length header */
            cp = &cur[CONTENT_LENGTH_LEN];
            cp += strspn(cp, " \t");
            cl = strtoul(cp, NULL, 0);
        } else if ((cp = strstr(cur, BOUNDARY))) {
            /* Boundary header */
            boundary = &cp[BOUNDARY_LEN];
            for (cp = cp + BOUNDARY_LEN; *cp && *cp != '\r' && *cp != '\n'; cp++);
            *cp = '\0';
            cur = ++cp;
        }
    }

    if (*path != '/') {
        send_error(400, "Bad Request", (char *) 0, "Illegal path.");
        return;
    }

    file = path + 1;
    if ((*file == '/' && *(file+1)) || strstr(file, "..") != 0) {
        send_error(400, "Bad Request", (char *) 0, "Illegal filename.");
        return;
    }

    ezplib_get_attr_val("wan_status_rule", 0, "state", wan_state, sizeof(wan_state), EZPLIB_USE_CLI);

    if (!*file) {
        /* If the file path is empty, the default path is "wan.asp" */
#ifdef EZP_SUB_BRAND_SONY
        file = "wl_basic.asp";
#else        
        if(atoi(wan_state) == 4 ) {
            file = "st_basic.asp";
        } else {
            file = "wan.asp";
        }
#endif        
    }

    /* parse cookie info */
    if(cookie != NULL) {
        parse_cookie_info(cookie, authorization);
    }

#if defined(EZP_PROD_BRAND_AXIM)
REDIRECT_LOGIN:
#endif

    for (handler = &mime_handlers[0]; handler->pattern; handler++) {
        if (match(handler->pattern, file)) {
#ifdef DEBUG
            printf("handler: %s\n", handler->pattern);
#endif
            if (handler->auth) {
                handler->auth(auth_userid, auth_passwd, auth_realm);
                if((auth_state = auth_check2(auth_realm, authorization))) {
                    /* login window */
                    /* send_authenticate(auth_realm); */
                    /* Send login page to user */
                    file = "login.asp";

                    /* Redirect to login page */
                    if (strcmp("apply.cgi*", handler->pattern)) {
                        for (handler = &mime_handlers[0]; handler->pattern;
                                handler++) {
                            if (match(handler->pattern, file)) break; 
                        }
                    }
                }
            }
            post = 0;
            if (strcmp(method, "POST") == 0) {
                post = 1;
            }
            if (handler->input) {
                handler->input(file, conn_fp, cl, boundary);
#if defined(linux)
                if ((flags = fcntl(fileno(conn_fp), F_GETFL)) != -1 &&
                    fcntl(fileno(conn_fp), F_SETFL, flags | O_NONBLOCK) != -1) {
                    /* Read up to two more characters */
                    if (fgetc(conn_fp) != EOF) {
                        (void) fgetc(conn_fp);
                    }
                    fcntl(fileno(conn_fp), F_SETFL, flags);
                }
#endif
            }
#if defined (EZP_PROD_BRAND_PROX)
            if (!strcmp("**.cfg",handler->pattern)) {
                char prox_cfg_type_prefix[]=
                    "application/octet-stream\r\nContent-Disposition: attachment; filename=";
                char prox_cfg_type[32];
                time_t result;
                struct tm *t;
                result=time(NULL);
                t=localtime(&result);
                sprintf(prox_cfg_type,"%s\"%s-%.4d-%.2d-%.2d.cfg\"", 
                        prox_cfg_type_prefix,nvram_safe_get("prox_serial"), 
                        t->tm_year+1900, t->tm_mon+1, t->tm_mday);
                send_headers(200, "OK", handler->extra_header,prox_cfg_type);
            }
            else
                send_headers(200, "OK", handler->extra_header, handler->mime_type);
#else
            send_headers(200, "OK", handler->extra_header, handler->mime_type);

#endif            
            if (get_postact_restart() == 2) {
                system("/etc/rc.common /etc/rc.d/S45httpd start");
            } else if (handler->output) {
                handler->output(file, conn_fp, "");
            }
            return;
        }
    }

    /* handler->pattern must be NULL here. */
    send_error(404, "Not Matched", (char *) 0, "MIME type not found.");
}

void                            // add by honor 2003-04-16
get_client_ipaddr_hwaddr(int conn_fp)
{
    struct sockaddr_in sa;
    int len = sizeof(struct sockaddr_in);
    char *m;

    getpeername(conn_fp, (struct sockaddr *) &sa, (socklen_t *)&len);
    nvram_safe_set("http_client_ipaddr", inet_ntoa(sa.sin_addr));
    m = get_hwaddr_by_ipaddr(inet_ntoa(sa.sin_addr));
    nvram_safe_set("http_client_hwaddr", m);
    client.ipaddr = sa.sin_addr;
    strcpy(client.mac ,m);
}

void
free_client(peer * client)
{
    free(client);
}

#ifdef EZPACKET_LOG
static void
handle_server_sig_int(int sig)
{
    ct_syslog(LOG_INFO, httpd_level, "httpd server %sshutdown",
              do_ssl ? "(ssl support) " : "");
    exit(0);
}
#endif

void
settimeouts(int sock, int secs)
{
    struct timeval tv;

    tv.tv_sec = secs;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
        perror("setsockopt(SO_SNDTIMEO)");
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
        perror("setsockopt(SO_RCVTIMEO)");
}

int
main(int argc, char **argv)
{
    usockaddr usa;
    int listen_fd;
    socklen_t sz = sizeof(usa);
    int c;
    int timeout = TIMEOUT;
    int n = 0;
#if defined(HTTPS_SUPPORT)      //jimmy, https, 8/4/2003
    BIO *sbio;
    SSL_CTX *ctx = NULL;
    int r;
    BIO *ssl_bio;
#endif
    int ret;
    inital_prod();
    strcpy(pid_file, "/var/run/httpd.pid");

    server_port = DEFAULT_HTTP_PORT;
    server_ipaddr[0] = '\0';

    while ((c = getopt(argc, argv, "Sp:i:t:s:ghd:")) != -1)
        switch (c) {
#if defined(HTTPS_SUPPORT)      //honor, https, 8/14/2003
        case 'S':
            do_ssl = 1;
            server_port = DEFAULT_HTTPS_PORT;
            strcpy(pid_file, "/var/run/httpsd.pid");
            break;
#endif
        case 'p':
            server_port = atoi(optarg);
            break;
        case 'i':
            /* Bind a specific interface. */
            strcpy(server_ipaddr, optarg);
            break;
        case 't':
            timeout = atoi(optarg);
            break;
#ifdef DEBUG_CIPHER
        case 's':
            set_ciphers = optarg;
            break;
        case 'g':
            get_ciphers = 1;
            break;
#endif
        case 'h':
            fprintf(stderr, "Usage: %s [-S] [-p port]\n"
                    "	-S : Support https (port 443)\n"
                    "	-p port : Which port to listen?\n"
                    "	-d directory : Which working directory?\n"
                    "	-i ipaddr : Which ipaddr to be bound?\n"
                    "	-t secs : How many seconds to wait before timing out?\n"
                    "	-s ciphers: set cipher lists\n"
                    "	-d directory: the file directory\n"
                    "	-g: get cipher lists\n", argv[0]);
            exit(0);
            break;
        case 'd':
            if (chdir(optarg) != 0) {
                fprintf(stderr, "httpsd: %s doesn't exist.\n", optarg);
            }
            break;
        default:
            break;
        }

#ifdef EZPACKET_LOG
    httpd_level =
        ct_openlog("httpd", LOG_PID | LOG_NDELAY, LOG_DAEMON, "LOG_HTTPD");

    ct_syslog(LOG_INFO, httpd_level, "httpd server %sstarted at port %d\n",
              do_ssl ? "(ssl support) " : "", server_port);
#endif

    lang_construct_translator(LANG_MAP_FILE, nvram_safe_get("lang"));

    /* Ignore broken pipes */
    signal(SIGPIPE, SIG_IGN);
#ifdef EZPACKET_LOG
    signal(SIGTERM, handle_server_sig_int);     // kill
#endif


#if defined(HTTPS_SUPPORT)      //jimmy, https, 8/4/2003
    /* Build our SSL context */
    if (do_ssl) {
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();
        ctx = SSL_CTX_new(SSLv23_server_method());
        if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM)
            == 0) {
            cprintf("Cann't read %s\n", CERT_FILE);
            ERR_print_errors_fp(stderr);
            exit(1);

        }
        if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) ==
            0) {
            cprintf("Cann't read %s\n", KEY_FILE);
            ERR_print_errors_fp(stderr);
            exit(1);

        }
        if (SSL_CTX_check_private_key(ctx) == 0) {
            cprintf("Check private key fail\n");
            ERR_print_errors_fp(stderr);
            exit(1);
        }
    }
#endif
    /* Initialize listen socket */
retry:
    if ((listen_fd = initialize_listen_socket(&usa)) < 0) {
#ifdef EZPACKET_LOG
        ct_syslog(LOG_ERR, httpd_level, "Cann't bind to any address");
#endif
        close(listen_fd);

        sleep(2);
        if (n++ <= 5) {
            cprintf("Something error occur, retry %d\n", n);
            goto retry;
        }

        exit(errno);
    }
#ifndef DEBUG
	{
		pid_t sid;
		int tPid = fork();
		if(tPid < 0)
		{
			exit(1); // Error on fork
		}
		else if(tPid > 0)
		{
			exit(0);
		}
		else
		{
			sid = setsid();
   			if (sid < 0) 
       	 		exit(EXIT_FAILURE);
   			freopen( "/dev/null", "r", stdin);
    		freopen( "/dev/null", "w", stdout);
    		freopen( "/dev/null", "w", stderr);
        }
/*
			setsid();
			int tIdx = 0;
			for(tIdx = getdtablesize(); tIdx >= 0; --tIdx)
			{
				close(tIdx);
			}
			tIdx = open(DEVNULL, O_RDWR);
			dup(tIdx);
			dup(tIdx);

        FILE *pid_fp;
         Daemonize and log PID 
        if (daemon(1, 1) == -1) {
            perror("daemon");
            exit(errno);
        }
        if (!(pid_fp = fopen(pid_file, "w"))) {
            perror(pid_file);
            return errno;
        }
        fprintf(pid_fp, "%d", getpid());
        fclose(pid_fp);
*/
    }
#endif

    /* Loop forever handling requests */
    for (;;) {
        if (get_postact_restart() == 2) {
            system("/etc/rc.common /etc/rc.d/S45httpd start");
        }
        if ((conn_fd = accept(listen_fd, &usa.sa, &sz)) < 0) {
            perror("accept");
            return errno;
        }

        /* Make sure we don't linger a long time if the other end disappears */
        settimeouts(conn_fd, timeout);

#if defined(HTTPS_SUPPORT)      //jimmy, https, 8/4/2003
        if (do_ssl) {

            sbio = BIO_new_socket(conn_fd, BIO_NOCLOSE);
            ssl = SSL_new(ctx);

#ifdef DEBUG_CIPHER
            check_cipher();
#endif

            SSL_set_bio(ssl, sbio, sbio);

            if ((r = SSL_accept(ssl) <= 0)) {
                //berr_exit("SSL accept error");
                ct_syslog(LOG_ERR, httpd_level, "SSL accept error");
                close(conn_fd);
                continue;
            }

            conn_fp = (webs_t) BIO_new(BIO_f_buffer());
            ssl_bio = BIO_new(BIO_f_ssl());
            BIO_set_ssl(ssl_bio, ssl, BIO_CLOSE);
            BIO_push((BIO *) conn_fp, ssl_bio);
        } else
#endif
        {
            if (!(conn_fp = fdopen(conn_fd, "r+"))) {
                perror("fdopen");
                return errno;
            }
        }
        /* Get HTTP client's IP address and MAC address. */
        get_client_ipaddr_hwaddr(conn_fd);

        handle_request();

        wfflush(conn_fp);
        ret = wfclose(conn_fp);
        /* wfclose(), to invoke fclose(), will terminate the file stream and 
         * file descriptor.
        close(conn_fd);
        */
    }

    shutdown(listen_fd, 2);
    close(listen_fd);

    return 0;
}
void inital_prod(){
    if (nvram_get("prod_cat")) {
        strncpy(ezp_prod_cat, nvram_get("prod_cat"), sizeof(ezp_prod_cat));
    }
    else {
        printf("Unassigned nvram variable prod_cat\n");
        ezp_prod_cat[0] = '\0';
    }
    ezp_prod_subcat=(atoi(nvram_safe_get("prod_subcat")));
    if (nvram_get("prod_subsubcat")) {
        strncpy(ezp_prod_subsubcat, nvram_get("prod_subsubcat"), 
                sizeof(ezp_prod_subsubcat));
    }
    else {
        printf("Unassigned nvram variable prod_subsubcat\n");
        ezp_prod_subsubcat[0] = '\0';
    }
    if (!strcmp(ezp_prod_cat, "B")) PROD_CAT_BITMAP=PROD_CAT_B;
    else if (!strcmp(ezp_prod_cat, "L")) PROD_CAT_BITMAP=PROD_CAT_L;
    else if (!strcmp(ezp_prod_cat, "M")) PROD_CAT_BITMAP=PROD_CAT_M;
    else if (!strcmp(ezp_prod_cat, "N")) PROD_CAT_BITMAP=PROD_CAT_N;
    else if (!strcmp(ezp_prod_cat, "C")) PROD_CAT_BITMAP=PROD_CAT_C;
    else if (!strcmp(ezp_prod_cat, "G")) PROD_CAT_BITMAP=PROD_CAT_G;
    else if (!strcmp(ezp_prod_cat, "I")) PROD_CAT_BITMAP=PROD_CAT_I;
    else if (!strcmp(ezp_prod_cat, "D")) PROD_CAT_BITMAP=PROD_CAT_D;
    else if (!strcmp(ezp_prod_cat, "A")) PROD_CAT_BITMAP=PROD_CAT_A;
    else if (!strcmp(ezp_prod_cat, "T")) PROD_CAT_BITMAP=PROD_CAT_T;
    else if (!strcmp(ezp_prod_cat, "V")) PROD_CAT_BITMAP=PROD_CAT_V;
    else if (!strcmp(ezp_prod_cat, "P")) PROD_CAT_BITMAP=PROD_CAT_P;
    else if (!strcmp(ezp_prod_cat, "R")) PROD_CAT_BITMAP=PROD_CAT_R;
    else if (!strcmp(ezp_prod_cat, "W")) PROD_CAT_BITMAP=PROD_CAT_W;
    else if (!strcmp(ezp_prod_cat, "K")) PROD_CAT_BITMAP=PROD_CAT_K;
    else if (!strcmp(ezp_prod_cat, "U")) PROD_CAT_BITMAP=PROD_CAT_U;
    else if (!strcmp(ezp_prod_cat, "O")) PROD_CAT_BITMAP=PROD_CAT_O;
    if ((ezp_prod_subsubcat[0]) && !strcmp(ezp_prod_subsubcat, "L")) prod_subsubcat="PROD_SUBSUBCAT_L";
    else prod_subsubcat="NOT_SET";
}

char *
wfgets(char *buf, int len, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return (char *) BIO_gets((BIO *) fp, buf, len);
    else
#endif
        return fgets(buf, len, fp);
}

int
wfputc(char c, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_printf((BIO *) fp, "%c", c);
    else
#endif
        return fputc(c, fp);
}

int
wfputs(char *buf, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_puts((BIO *) fp, buf);
    else
#endif
        return fputs(buf, fp);
}

int
wfprintf(FILE * fp, char *fmt, ...)
{
    va_list args;
    /* FIXME: 4096 might be insufficient */
    char buf[4096];
    int ret;

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        ret = BIO_printf((BIO *) fp, "%s", buf);
    else
#endif
        ret = fprintf(fp, "%s", buf);
    va_end(args);
    return ret;
}

size_t
wfwrite(char *buf, int size, int n, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_write((BIO *) fp, buf, n * size);
    else
#endif
        return fwrite(buf, size, n, fp);
}

size_t
wfread(char *buf, int size, int n, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_read((BIO *) fp, buf, n * size);
    else
#endif
        return fread(buf, size, n, fp);
}

int
wfflush(FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl) {
        BIO_flush((BIO *) fp);
        return 1;
    } else
#endif
    return fflush(fp);
}

int
wfclose(FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl) {
        BIO_free_all((BIO *) fp);
        return 1;
    } else
#endif
    return fclose(fp);
}

#ifdef DEBUG_CIPHER
void
check_cipher(void)
{
    STACK_OF(SSL_CIPHER) * sk;
    char buf[512];
    BIO *STDout = NULL;
    int i;
    static BIO *bio_stdout = NULL;
    X509 *peer;
    static BIO *bio_s_out = NULL;
    SSL_CIPHER *ciph;

    if (set_ciphers) {
        /* Set supported cipher lists */
        SSL_set_cipher_list(ssl, set_ciphers);
    }
    if (get_ciphers) {
        /* Show supported cipher lists */
        sk = SSL_get_ciphers(ssl);

        for (i = 0; i < sk_SSL_CIPHER_num(sk); i++) {
            BIO_puts(STDout,
                     SSL_CIPHER_description(sk_SSL_CIPHER_value(sk, i),
                                            buf, 512));
            printf("%d: %s", i, buf);
        }
        if (STDout != NULL)
            BIO_free_all(STDout);
    }
    peer = SSL_get_peer_certificate(ssl);
    if (peer != NULL) {
        BIO_printf(bio_s_out, "Client certificate\n");
        PEM_write_bio_X509(bio_s_out, peer);
        X509_NAME_oneline(X509_get_subject_name(peer), buf, sizeof buf);
        BIO_printf(bio_s_out, "subject=%s\n", buf);
        X509_NAME_oneline(X509_get_issuer_name(peer), buf, sizeof buf);
        BIO_printf(bio_s_out, "issuer=%s\n", buf);
        X509_free(peer);
    }

    if (SSL_get_shared_ciphers(ssl, buf, sizeof buf) != NULL)
        BIO_printf(bio_s_out, "Shared ciphers:%s\n", buf);

    bio_stdout = BIO_new_fp(stdout, BIO_NOCLOSE);
    ciph = SSL_get_current_cipher(ssl);
    BIO_printf(bio_stdout, "%s%s, cipher %s %s\n",
               "",
               SSL_get_version(ssl),
               SSL_CIPHER_get_version(ciph), SSL_CIPHER_get_name(ciph));
}
#endif
