/*
 * Broadcom Home Gateway Reference Design
 * Web Page Configuration Support Routines
 *
 * Copyright 2001-2003, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: upgrade.c,v 1.23 2005/05/05 03:36:32 honor Exp $
 */


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <cyutils.h>
#include <cy_conf.h>

#include "ezp.h"
#include "shared.h"

#define CODE_PATTERN_ERROR 9999
#define CONFIG	1
#define	FIRMWARE	2
static int upgrade_result;
static int upgrade_type;
extern int refuse_request;

void
do_export(char *path, webs_t stream, ...)
{
    FILE *ifp, *ofp;
    int c;
    char cfg_file[] = "/tmp/export.cfg";
    char encoded_file[] = "/tmp/encoded.cfg";
    char *write_argv[4];

    write_argv[0] = "/usr/sbin/nvram";
    write_argv[1] = "export";
    write_argv[2] = cfg_file;
    write_argv[3] = NULL;

    _eval(write_argv, NULL, 10, NULL) ;

    if (!(ifp = fopen(cfg_file, "r"))) {
        return;
    }
    if (!(ofp = fopen(encoded_file, "w"))) {
        fclose(ifp);
        unlink(cfg_file);
        return;
    }

    /* Encode. */
    /* Note: c must be an integer to compare with EOF. */
    while ((c = fgetc(ifp)) != EOF) {
        encode((char *)&c, 1);
        fputc(c, ofp);
    }
    fclose(ifp);
    fclose(ofp);

    /* Export the file itself. */
    do_file(encoded_file, stream, NULL);

    unlink(cfg_file);
    unlink(encoded_file);
}
#include <sys/reboot.h>
void
do_upgrade_cgi(char *url, webs_t wp, ...)
{

    if (upgrade_type == FIRMWARE) { /* FIRMWARE. */
        if (upgrade_result == 1) {
            refuse_request = 0;
            do_ej("resp_upgrade_fail.asp", wp, "");
            websDone(wp, 200);
            wfflush(wp);
            wfclose(wp);
            sleep(5);
#if defined(PLATFORM_AXA)
            system("ezp-i2c gauge host booting");
#endif
            reboot(RB_AUTOBOOT);
#if defined(PLATFORM_LIS)
        } else if (upgrade_result == 2) {
            printf("upgrade_result=2\n");
            refuse_request = 0;
            do_ej("resp_upgrade_low_power.asp", wp, "");
            websDone(wp, 200);
            wfflush(wp);
#endif
        } else {
            /* Reboot if successful. */
            do_ej("/tmp/resp_upgrade_success.asp", wp, "");
            websDone(wp, 200);
	    /* Seprate reboot from busybox, and put it into tmp */
            /* EZP
             * It will immediately restart the router. So we can't 
             * wait the processing finish and close the connection
             * socket. */
            wfflush(wp);
            wfclose(wp);
            /* Wait 5 seconds and do reboot directly from the system call. */
            sleep(20);
#if defined(PLATFORM_AXA)
            system("ezp-i2c gauge host booting");
#endif
            reboot(RB_AUTOBOOT);
        }
    }
    else if (upgrade_type == CONFIG) { /* Configuration.*/
        if (upgrade_result) {
            refuse_request = 0;
            do_ej("resp_fail.asp", wp, "");
            websDone(wp, 200);
        } else {
            nvram_commit();
            do_ej("resp_reboot_wait.asp", wp, "");
            websDone(wp, 200);
	    /* Seprate reboot from busybox, and put it into tmp */
#if defined(PLATFORM_AXA)
            system("ezp-i2c gauge host booting");
#endif
            system("reboot");
        }
    }
    else { /* Unknown upgrade type */
            refuse_request = 0;
            do_ej("resp_unknown_fail.asp", wp, "");
            websDone(wp, 200);
    }

}

/* Copy file from fsrc to fdest */
static int
ezp_copy_file(char *fsrc, char *fdest) {
    FILE *srcfp = NULL, *destfp = NULL;
    unsigned char *buf = NULL;
    int len, retval = 0;


    if (!fsrc || !fdest || !*fsrc || !*fdest) {
        retval = 1;
        goto src_err;
    }
    srcfp = fopen(fsrc, "r");
    if (!srcfp) {
        retval = 2;
        goto src_err;
    }
    destfp = fopen(fdest, "w");
    if (!destfp) {
        retval = 3;
        goto dest_err;
    }
    buf = (unsigned char*)malloc(512);
    if (!buf) {
        retval = 4;
        goto alloc_err;
    }
    while ((len = fread(buf, 1, 512, srcfp)) > 0) {
        fwrite(buf, 1, len, destfp);
    }
    free(buf);
alloc_err:
    fclose(destfp);
dest_err:
    fclose(srcfp);
src_err:
    return retval;
}

int
run_prog(char *command, char *argu, FILE *loghandle) {
    char *write_argv[4];
    int status;
    pid_t pid;
    int ret = 0;

    write_argv[0] = command;
    write_argv[1] = argu;
    write_argv[2] = NULL;
    if (loghandle) {
        fprintf(loghandle,"%s %s:%d pid:%d %s %s\n", 
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    else {
        printf("%s %s:%d pid:%d %s %s\n", 
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    _eval(write_argv, NULL, 0, &pid) ;
    if (loghandle) {
        fprintf(loghandle,"%s %s:%d pid:%d %s %s is running\n", 
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    else {
        printf("%s %s:%d pid:%d %s %s is running\n", 
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    if(waitpid(pid, &status, 0) < 0) {
        ret = errno;
    }
    if (loghandle) {
        fprintf(loghandle,"%s %s:%d pid:%d %s %s finished successfully\n", 
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    else {
        printf("%s %s:%d pid:%d %s %s finished successfully\n",
                __FILE__, __FUNCTION__, __LINE__, pid, write_argv[0], write_argv[1]);
    }
    return ret;
}


int
import_config(char *url, webs_t stream, int *total)
{
    char upload_file[] = "/tmp/ezp_cfg_XXXXXX";
    char decoded_file[] = "/tmp/decoded_cfg.cfg";
    FILE *fifo = NULL;
    FILE *tmp_fifo = NULL;
    char *buf = NULL;
    int c;
    int count; 
    int ret = 0;
    long flags = -1;
    int size = BUFSIZ;

    mkstemp(upload_file) ;
    if (!(fifo = fopen(upload_file, "w"))) {
        ret = errno;
        goto err;
    }
#ifdef DEBUG
    printf("Random filename %s\n", upload_file);
#endif

    /* Set nonblock on the socket so we can timeout */
    if (!do_ssl) {
        if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
                fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
            ret = errno;
            goto err;
        }
    }

    /*
     ** The buffer must be at least as big as what the stream file is
     ** using so that it can read all the data that has been buffered 
     ** in the stream file. Otherwise it would be out of sync with fn
     ** select specially at the end of the data stream in which case
     ** the select tells there is no more data available but there in 
     ** fact is data buffered in the stream file's buffer. Since no
     ** one has changed the default stream file's buffer size, let's
     ** use the constant BUFSIZ until someone changes it.
     **/

    if ((buf = malloc(size)) == NULL) {
        ret = ENOMEM;
        goto err;
    }

#ifdef DEBUG
    printf("Uploading the configuration file %s\n", upload_file);
#endif
    /* Upload the configuration file first. */
    while (total && *total) {
        if (do_ssl) {
            if (size > *total)
                size = *total;
            count = wfread(buf, 1, size, stream);
        } else {
            if (waitfor(fileno(stream), 5) <= 0) {
                cprintf("waitfor timeout 5 secs\n");
                break;
            }
            count = safe_fread(buf, 1, size, stream);
            if (!count && (ferror(stream) || feof(stream)))
                break;
        }

        safe_fwrite(buf, 1, count, fifo);
        *total -= count;
        printf("total:%d\n", *total);
#ifdef DEBUG
        printf(".");
#endif
    }
    fclose(fifo);
    fifo = NULL;

    /* cfg file needs to be decoded. */
    if (!(fifo = fopen(upload_file, "r"))) {
        ret = errno;
        goto err;
    }

    if (!(tmp_fifo = fopen(decoded_file, "w"))) {
        ret = errno;
        goto err;
    }

    /* Decoding procedure */
    /* Note: c must be an integer to compare with EOF. */
    while ((c = fgetc(fifo)) != EOF) {
        decode((char *)&c, 1);
        fputc(c, tmp_fifo);
    }

    fclose(fifo);
    fifo = NULL;
    fclose(tmp_fifo);
    tmp_fifo = NULL;

    if (!do_ssl) {
        /* Reset nonblock on the socket */
        if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
            ret = errno;
            goto err;
        }
    }

   ret=nvram_import(decoded_file); 

#ifdef DEBUG
    printf("done\n");
#endif

err:
    if (buf)        { free(buf); }
    if (fifo)       { fclose(fifo); }
    if (tmp_fifo)   { fclose(tmp_fifo); }

#ifdef DEBUG
    if (ret) { perror(NULL); }
#endif

    return ret;
}

int
do_upgrade(char *url, webs_t stream, int *total)
{
    char upload_file[] = "/tmp/ezp_firmware_XXXXXX";
    FILE *fifo = NULL;
    FILE *tmp_fifo = NULL;
    char *write_argv[4];
    pid_t pid;
    char *buf = NULL;
    int count; 
    int ret = 0;
    long flags = -1;
    int size = BUFSIZ;
    int status;
    struct stat st;
#if defined(PLATFORM_LIS)
    FILE *pFile;
    char result[24]={0};
    int percentage=0;
    if ((pFile = popen("ezp-i2c gauge percentage", "r"))) {
        fgets(result, sizeof(result), pFile);
        sscanf(result,"Battery: %d %%", &percentage);
    }
    // terminate
    pclose (pFile);
    if (percentage < 60) {
        ret=2;
        goto err;
    }
#endif

    if (appmgr_check("storage")){
        system("ACTION=\"remove\" /sbin/hotplug-call block");
    }
    /* Disable dropbear, crond, hotplug2, dnsmasq and udps */
    run_prog("/usr/bin/killall", "dropbear", NULL);
    system("kill -9 $(pidof watchdog)");
    system("/usr/sbin/watchdog -c /etc/watchdog.upgrade");
    /* Stop all VPN services */
    system("/etc/rc.common /etc/init.d/pptpd stop");
    system("/etc/rc.common /etc/init.d/xl2tpd stop");
    system("/etc/rc.common /etc/init.d/ipsec stop");
    system("/etc/rc.common /etc/init.d/vncrepeater stop");
    system("/sbin/stop_services.sh");

    /* FIRMWARE*/
    system("rmmod hw_nat");
#if defined(PLATFORM_LIS)
    system("gpioctl led 13 1 0 0 0 0");
#endif
    write_argv[0] = "/usr/sbin/ezpup";
    write_argv[1] = upload_file;
    write_argv[2] = NULL;

    /* Disable network service */
    system("echo \"0\" > /proc/sys/net/ipv4/ip_forward");
    /* For reboot normal. Because firmware upgraade will produce squashfs
     * error when system execute reboot from flash. */
    /* The following files are required after firmware and file system 
     * upgraded.
     *
     * It includes
     *   1. busybox and related symbolic links
     *   2. uclibc libraries
     *   3. nvram libraries
     *   4. upgrade finishing page
     * */
    if (ezp_copy_file("/bin/busybox", "/tmp/busybox")) {
        printf("copy /bin/busybox to /tmp/busybox failed\n");
    }
    if (chmod("/tmp/busybox", 0x0777)) {
        printf("chmod /tmp/busybox to 0777 failed\n");
    }
    if (unlink("/bin/busybox")) {
        printf("Remove /bin/busybox failed\n");
    }
    if (symlink("/tmp/busybox", "/bin/busybox")){
        printf("Make busybox symbolic link failed\n");
    }
    unlink("/sbin/reboot");
    symlink("/bin/busybox","/tmp/reboot");
    symlink("/bin/busybox","/sbin/reboot");
    char *libfiles[] = {
        "libuClibc-0.9.29.so",
        "ld-uClibc-0.9.29.so",
        "libdl-0.9.29.so",
        "libm-0.9.29.so",
        "libnsl-0.9.29.so",
        "libutil-0.9.29.so",
        NULL
    };
    char *usrlibfiles[] = {
        "libnvram.so",
        "libshared.so",
        NULL,
    };
    char *tempfile = NULL;
    int i = 0, res;
    char oldfile[50], newfile[50];

    for(i = 0, tempfile = libfiles[i]; tempfile != NULL; i++, tempfile = libfiles[i]) {
        if(tempfile) {
            snprintf(oldfile,sizeof(oldfile), "/lib/%s", tempfile);
            snprintf(newfile,sizeof(newfile), "/tmp/%s", tempfile);
            printf("Backup %s for upgrade usage ...\n", oldfile);
            if ((res = ezp_copy_file(oldfile, newfile)) != 0) {
                printf("copy %s to %s failed, ret code:%d\n", oldfile, newfile, res);
            }
            if (chmod(newfile, 0x0777)) {
                printf("chmod %s to 0777 failed\n", newfile);
            }
            unlink(oldfile);
            if (symlink(newfile, oldfile)){
                printf("Make symbolic link %s->%s failed\n", oldfile, newfile);
            }
        }
    }
    for(i = 0, tempfile = usrlibfiles[i]; tempfile != NULL; i++, tempfile = usrlibfiles[i]) {
        if(tempfile) {
            snprintf(oldfile,sizeof(oldfile), "/usr/lib/%s", tempfile);
            snprintf(newfile,sizeof(newfile), "/tmp/%s", tempfile);
            printf("Backup %s for upgrade usage ...\n", oldfile);
            if (ezp_copy_file(oldfile, newfile)) {
                printf("copy %s to %s failed\n", oldfile, newfile);
            }
            if (chmod(newfile, 0x0777)) {
                printf("chmod %s to 0777 failed\n", newfile);
            }
            unlink(oldfile);
            if (symlink(newfile, oldfile)){
                printf("Make symbolic link %s->%s failed\n", oldfile, newfile);
            }
        }
    }
    if (ezp_copy_file("/www/resp_upgrade_success.asp","/tmp/resp_upgrade_success.asp")) {
        printf("copy /www/resp_upgrade_success.asp /tmp/resp_upgrade_success.asp failed\n");
    }

    mkstemp(upload_file) ;
    if (!(fifo = fopen(upload_file, "w"))) {
        ret = errno;
        goto err;
    }
#ifdef DEBUG
    printf("Random filename %s\n", upload_file);
#endif

    /* Set nonblock on the socket so we can timeout */
    if (!do_ssl) {
        if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
                fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
            ret = errno;
            goto err;
        }
    }

    /*
     ** The buffer must be at least as big as what the stream file is
     ** using so that it can read all the data that has been buffered 
     ** in the stream file. Otherwise it would be out of sync with fn
     ** select specially at the end of the data stream in which case
     ** the select tells there is no more data available but there in 
     ** fact is data buffered in the stream file's buffer. Since no
     ** one has changed the default stream file's buffer size, let's
     ** use the constant BUFSIZ until someone changes it.
     **/

    if ((buf = malloc(size)) == NULL) {
        ret = ENOMEM;
        goto err;
    }

#ifdef DEBUG
    printf("Uploading the firmware %s\n", upload_file);
#endif
    /* Upload the file first. */
    while (total && *total) {
        if (do_ssl) {
            if (size > *total)
                size = *total;
            count = wfread(buf, 1, size, stream);
        } else {
            if (waitfor(fileno(stream), 5) <= 0) {
                cprintf("waitfor timeout 5 secs\n");
                ret = errno;
                goto err;
            }
            count = safe_fread(buf, 1, size, stream);
            if (!count && (ferror(stream) || feof(stream))) {
                ret = errno;
                goto err;
            }
        }

        safe_fwrite(buf, 1, count, fifo);
        *total -= count;
#ifdef DEBUG
        printf(".");
#endif
    }
    fclose(fifo);
    fifo = NULL;

    if (!do_ssl) {
        /* Reset nonblock on the socket */
        if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
            ret = errno;
            goto err;
        }
    }
    /* Data transfer finished, bring down devices. */
    system("/sbin/ifdown wan0");
    system("/sbin/ifdown wan1");
    //system("/sbin/ifdown lan0");
    _eval(write_argv, NULL, 0, &pid) ;
    if(waitpid(pid, &status, 0) < 0) {
        ret = errno;
        goto err;
    }
    if(stat("/tmp/fw_incorrect",&st) == 0) ret=errno; 
    unlink("/tmp/fw_incorrect");
#ifdef DEBUG
    printf("done\n");
#endif

err:
    if (buf) {
        free(buf);
    }
    if (fifo) {
        fclose(fifo);
    }
    if (tmp_fifo) {
        fclose(tmp_fifo);
    }
#ifdef DEBUG
    if (ret) {
        perror(NULL);
    }
#endif

    return ret;
}



void
do_upgrade_post(char *url, webs_t stream, int len, char *boundary)
{
    char buf[POST_BUF_SIZE];

    upgrade_result = EINVAL;
    upgrade_type = 0;

    /* Turn on the flag to omit the follwoing HTTP request. */
    refuse_request = 1;

    /* Look for our part */
    while (len > 0) {
        if (!wfgets(buf, MIN(len + 1, sizeof(buf)), stream))
            return;
        len -= strlen(buf);
        if (!strncasecmp(buf, "Content-Disposition:", 20)) {
            if (strstr(buf, "name=\"binfile\"")) { 
                /* upgrade image */
                upgrade_type = FIRMWARE;
                break;
            } else if (strstr(buf, "name=\"cfgfile\"")) {   
                /* import configuration */
                upgrade_type = CONFIG;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("%s upgrade\n", (upgrade_type == FIRMWARE) ? "firmware" : "config" );
#endif

    /* Skip boundary and headers */
    while (len > 0) {
        if (!wfgets(buf, MIN(len + 1, sizeof(buf)), stream))
            return;
#ifdef DEBUG
    printf("[%d]%s", strlen(buf), buf);
#endif
        len -= strlen(buf);
        if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
            break;
    }

#ifdef DEBUG
    printf("content length[%d]\n", len);
#endif

    if ( upgrade_type == FIRMWARE ) {
        upgrade_result = do_upgrade(NULL, stream, &len);
    } 

    if ( upgrade_type == CONFIG ) {
        upgrade_result = import_config(NULL, stream, &len);
    }
    
    /* Slurp anything remaining in the request */
    while (len--)
#if defined(HTTPS_SUPPORT)
        if (do_ssl)
            BIO_gets((BIO *) stream, buf, 1);
        else
#endif
            (void) fgetc(stream);
}

