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
#include <cy_conf.h>
#include <sys/sem.h>
#include "ezp.h"
#include "common.h"
#include "lang.h"
#include "shared.h"
#include "ezp-lib.h"
#include "shutils.h"

#define BT_DIR "/tmp/sda1/download"
#define TMP_TEPLATE_NAME "/tmp/ezp_tmp_seed.XXXXXXXXXX"
#define BT_OP "transmission-remote 127.0.0.1 -t"
#define BT_STATUS "/var/stats/stats-widget-bt-upgrade1"
#define BT_COMPLETED "download/session/bt-cpl"

static char filename[LONG_BUF_LEN];
static char tmp_file[] = TMP_TEPLATE_NAME;

enum {
    BT_ENABLE = 0,
    BT_PEER_NUM,
    BT_MAX_PEER_NUM,
    BT_PORT,
    BT_DHT_PORT,
    BT_MAX_DOWNLOAD_RATE,
    BT_MAX_UPLOAD_RATE
};

enum {
    DOWNLOADING = 0,
    STOPPED,
    PAUSED,
    WAITING,
    HASHING,
    COMPLETED,
    STOPPING,
    PAUSING,
    STARTING,
    DELETING,
    RESUMING,
    HASH_FOR_CPL
};

static struct variable bt_variables[] = {
    {longname: "BT Enable", argv:ARGV("0", "1"), nullok: FALSE},
    {longname: "BT Peer Num", argv:ARGV("10", "1000"), nullok: FALSE},
    {longname: "BT Peer Global", argv:ARGV("10", "1000"), nullok: FALSE},
    {longname: "BT Port", argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "BT DHT Port", argv:ARGV("1", "65535"), nullok: FALSE},
    {longname: "BT Max Download Rate", argv:ARGV("0", "10240"), nullok: FALSE},
    {longname: "BT Max Upload Rate", argv:ARGV("0", "10240"), nullok: FALSE}
};
void
bt_op_cgi(webs_t wp, char *op, char *cmd)
{
    FILE *fp;
    char tmp[LONG_BUF_LEN], buf[LONG_BUF_LEN], res[LONG_BUF_LEN];

    if (!(fp = popen(cmd, "r"))) {
        snprintf(tmp, sizeof(tmp), "{\"failure\": \"true\", \"msg\": \"%s: %s\"}",
                op, lang_translate("operation failed"));
        websWrite(wp, "%s: %s", op, tmp);
        goto err;
    }

    if(fgets(tmp, sizeof(tmp), fp)) {
        sscanf(tmp, "%*[^\"]\"%[^\"]" , res);
        if(strcmp(res, "success")) {
            snprintf(buf, sizeof(buf), "{\"failure\": \"true\", \"msg\": \"%s: %s (%s)\"}",
                    op, lang_translate("operation failed"), res);
            websWrite(wp, "%s: %s", op, buf);
            goto err;
        }
    } else {
        snprintf(buf, sizeof(buf), "{\"failure\": \"true\", \"msg\": \"%s: %s (%s)\"}",
                op, lang_translate("operation failed"), "BT engine busy");
        websWrite(wp, "%s: %s", op, buf);
        goto err;
    }
    snprintf(buf, sizeof(buf), "{\"success\": \"true\", \"msg\": \"%s: %s\"}",
               op, lang_translate("succeeded"));
    websWrite(wp, "%s", buf);
err:
    if (fp) {
        pclose(fp); 
    }
    return;
}

void
dumpinfo2buf(FILE *fp, char *buf1, int *total_used, int *residual, bool finish)
{
    char tmp[LONG_BUF_LEN];
    int n = 2,  used = 0;
    char *ptr, *buf2;
    ptr = &buf1[0] + *total_used;
    while (fgets(tmp, sizeof(tmp), fp)) {
        int str_len;
        /* if data is not completeness */
        if (strlen(tmp) <= 8) {
            continue;
        }

        /* Remove '\n' */
        tmp[strlen(tmp) - 1] = '\0';
    
        str_len = strlen(tmp);

        while ((*residual < str_len + 1)) {
            /* Insufficient buffer size */
            buf2 = realloc(buf1, n * LONG_BUF_LEN);
            *residual = LONG_BUF_LEN * (n++) - *total_used;
            /* realloc() might return different address space from the orginal
             * one. Therefore, we have to copy the content of the old one to the
             * new one and reindicate ptr to continue the processing.
             */
            if (buf1 != buf2) {
                /* XXX: memcpy is unnecessary. But, with regards to the man
                 * page, " newly allocated memory will be uninitialized."
                memcpy(buf2, buf1, sizeof(buf1)); 
                */
                ptr = &buf2[0] + *total_used; 
                buf1 = buf2;
            }
        }
        used = snprintf(ptr, *residual, "%s|", tmp);
        ptr += used;
        *residual -= used;
        *total_used += used;
    }
    if (finish) {
        /* if ptr points to buf[0], it means there is no result generated in the
         * while loop above.
         */
        if (ptr != &buf1[0]) {
            ptr--;
        }
        /* Remove the last '|' or set the buff as empty string */
        *ptr = '\0';
    }
}

void
bt_update_cgi(webs_t wp)
{
    FILE *fp1 = NULL, *fp2 = NULL;
    char *buf;
    char tmp[TMP_LEN], dev[SHORT_BUF_LEN];
    int total_used = 0, residual = LONG_BUF_LEN;
 
    if (!process_is_running("transmission-daemon")) {
        websWrite(wp, "{\"success\": \"true\", \"msg\": \"\"}");
        return;
    }
    if (!(fp1 = fopen(BT_STATUS, "r"))) {
        /* TODO: error return */
        goto err;
    }
    ezplib_get_attr_val("storage_state_rule", 0, "dev_name", dev, sizeof(dev),
            EZPLIB_USE_CLI);
    snprintf(tmp, sizeof(tmp), "/tmp/%s/%s", dev, BT_COMPLETED);
    if (!(fp2 = fopen(tmp, "r"))) {
        /* TODO: error return */
        goto err;
    }
    buf = calloc(LONG_BUF_LEN, sizeof(char));
    dumpinfo2buf(fp1, buf, &total_used, &residual, 0);
    dumpinfo2buf(fp2, buf, &total_used, &residual, 1);

    websWrite2(wp, "{\"success\": \"true\", \"msg\": \"%s\"}", buf);
    free(buf);
err:
    if (fp1) {
        fclose(fp1);
    }
    if (fp2) {
        fclose(fp2);
    }
    /* Flush all key-value pairs in the hash table. */
    init_cgi(NULL);
}

void do_bt_action(webs_t wp, int action, char *hash,  char *unchange, char *buf)
{
    char tmp[LONG_BUF_LEN];
    switch (action)
    {
        case DELETING:
            snprintf(tmp, sizeof(tmp), "%s%s --remove-and-delete", BT_OP, hash);
            bt_op_cgi(wp, lang_translate("Delete"), tmp);
            snprintf(tmp, sizeof(tmp), "%s%s^%d^%s\n",
                    unchange, "Deleting", DELETING, hash);
            break;
        case STOPPING:
            snprintf(tmp, sizeof(tmp), "%s%s -S", BT_OP, hash);
            bt_op_cgi(wp, lang_translate("Stop"), tmp);
            snprintf(tmp, sizeof(tmp), "%s%s^%d^%s\n",
                    unchange, "Stopping", STOPPING, hash);
            break;
        case STARTING:
            snprintf(tmp, sizeof(tmp), "%s%s -s", BT_OP, hash);
            bt_op_cgi(wp, lang_translate("Start"), tmp);
            snprintf(tmp, sizeof(tmp), "%s%s^%d^%s\n",
                    unchange, "Starting", STARTING, hash);
            break;
    }
    strcat(buf ,tmp);
}

void
get_unchange(char *source, int len, char *buf)
{
 int i;
 int test = 0;
 for (i = 0; i < len; i++) {
     buf[i] = source[i];
     if (source[i] == '^') 
         test++;
     if(test == 7)
         break; 
 }
 buf[i + 1] = '\0';
 return;
}

void
do_bt_cgi(char *url, webs_t wp, ...)
{
    char *value, *hash;
    char *buf = NULL;
    FILE *fp = NULL;
    char tmp[LONG_BUF_LEN], bt_cpl_file[TMP_LEN], dev[SHORT_BUF_LEN];
    int total_len = 0, buf_len = LONG_BUF_LEN;
    int n = 1;

    /* config download completed info file name */
    ezplib_get_attr_val("storage_state_rule", 0,
            "dev_name", dev, sizeof(dev), EZPLIB_USE_CLI);
    snprintf(bt_cpl_file, sizeof(bt_cpl_file),
                 "/tmp/%s/%s", dev, BT_COMPLETED);

    hash = websGetVar(wp, "hash", "");

    /* Find out the value of "action". */
    value = websGetVar(wp, "action", "");

    if (!*value) {
        /* No action! */
        /* TODO:XXX: Send out "action required" message. */
    } else if (!strcmp(value, "Update")) {
        bt_update_cgi(wp);
        return;
    } else if (!strcmp(value, "delallcpl")) {
        if (process_is_running("transmission-daemon")) {
            fp = fopen(bt_cpl_file, "r");
            if(fp) {
                int res;
                res = fgetc(fp);
                fclose(fp);
                if(res != EOF) {
                    unlink(bt_cpl_file);
                    fp = fopen(bt_cpl_file, "w");
                    fclose(fp);
                    websWrite(wp, "{\"success\": \"true\", \"msg\": \"\"}");
                } else {
                    websWrite(wp, "{\"failure\": \"true\", \"msg\": \"\"}");
                }
            }
        }
        return;
    }
    /* if action is delete, search completed download task in completed file */
    if (!strcmp(value, "Delete")) {
        /* open completed data file */
        fp = fopen(bt_cpl_file, "r");
        buf = calloc(LONG_BUF_LEN, sizeof(char));
        if (fp) { 
            /* create a buffer to save completed data */
            while (fgets(tmp, sizeof(tmp), fp)) { 
                char task_hash[64];
                if (strlen(tmp) <= 1) {
                    continue;
                }
                sscanf(tmp, "%*[^^]^%*[^^]^%*[^^]^%*[^^]^^^^%*[^^]^%*[^^]^%[^\n]"                      , task_hash);
                /* compare data hash */
                if (strcmp(hash, task_hash)) {
                    /* if the data want to save */
                    total_len += strlen(tmp);
                    /* check buffer size can save all data */
                    if (total_len >= (buf_len - 4))  {
                        /* if data size big than buffer size 
                         * realloc buffer */
                        ++n;
                        buf = realloc(buf, LONG_BUF_LEN * n);
                        buf_len = LONG_BUF_LEN * n;
                    }
                    /* add data into buffer */
                    strcat(buf, tmp);
                } else {
                    websWrite(wp, "{\"success\": \"true\", \"msg\": \"%s: %s\"}",
                            "Delete", lang_translate("succeeded"));
                }
            }
            fclose(fp);
        }
        /* write the data that we want to save into file */
        fp = fopen(bt_cpl_file, "w");
        if (fp) {
            fprintf(fp, "%s", buf);
            fclose(fp);
        }
        free(buf);
    }
    fp = fopen(BT_STATUS, "r");
    buf = calloc(LONG_BUF_LEN , sizeof(char));
    if (fp) {
        total_len = 0;
        buf_len = LONG_BUF_LEN;
        n = 1;
        while (fgets(tmp, sizeof(tmp), fp)) { 
            char task_hash[64];
            char str_status[64];
            char unchange[LONG_BUF_LEN];
            unsigned int status;
            get_unchange(tmp, strlen(tmp), unchange);
            sscanf(tmp, "%*[^^]^%*[^^]^%*[^^]^%*[^^]^%*[^^]^%*[^^]^%*[^^]^%[^^]^%d^%[^\n]", str_status, &status, task_hash);
            total_len += strlen(tmp);
            if (total_len >= (buf_len - 4))  {
                /* if data size big than buffer size 
                 * realloc buffer */
                ++n;
                buf = realloc(buf, LONG_BUF_LEN * n);
                buf_len = LONG_BUF_LEN * n;
            }
            if (!strcmp(hash, task_hash)) {
                switch (status) {
                    case STOPPING:
                    case PAUSING:
                    case STARTING:
                    case DELETING:
                    case HASH_FOR_CPL:
                        strcat(buf, tmp);
                        break;
                    case COMPLETED:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        }
                        break;
                    case DOWNLOADING:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        } else if (!strcmp(value, "Stop")) {
                            do_bt_action(wp ,STOPPING, hash, unchange, buf);
                        }
                        break;
                    case STOPPED:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        } else if (!strcmp(value, "Start")) {
                            do_bt_action(wp, STARTING, hash, unchange, buf);
                        }
                        break;
                    case PAUSED:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        } else if (!strcmp(value, "Start")) {
                            do_bt_action(wp, STARTING, hash, unchange, buf);
                        } else if (!strcmp(value, "Stop")) {
                            do_bt_action(wp, STOPPING, hash, unchange, buf);
                        }
                        break;
                    case WAITING:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        } else if (!strcmp(value, "Stop")) {
                            do_bt_action(wp, STOPPING, hash, unchange, buf);
                        }
                        break;
                    case HASHING:
                        if(!strcmp(value, "Delete")) {
                            do_bt_action(wp, DELETING, hash, unchange, buf);
                        } else if (!strcmp(value, "Stop")) {
                            do_bt_action(wp, STOPPING, hash, unchange, buf);
                        }
                        break;
                    default :
                        /* Undefined action. */
                        websWrite(wp, "Undefined action %s<br>", value);
                }
            } else {
                /* Not action torrent */
                strcat(buf, tmp);
            }
        }
        fclose(fp);
    }
    if ((fp = fopen(BT_STATUS, "w"))) {
        fprintf(fp, "%s", buf);
        fclose(fp);
    }
    free(buf);
    init_cgi(NULL);
}

void
do_bt_upload_cgi(char *url, webs_t wp, ...)
{
    struct stat fstat;
    FILE *fp;
    char cmd[LONG_BUF_LEN], buf1[TMP_LEN], buf2[TMP_LEN], bt_dir[LONG_BUF_LEN];
    char res[LONG_BUF_LEN];

    /* TODO: examine the seed */

    /* Move the seed into the folder which rtorrent utilizes if the folder
     * exists.
     */
    ezplib_get_attr_val("storage_state_rule", 0, "dev_name", buf1, sizeof(buf1),
            EZPLIB_USE_CLI);
    ezplib_get_attr_val("bt_rule", 0, "load_start", buf2, sizeof(buf2),
            EZPLIB_USE_CLI);
    if (buf2[0] != 0) {
        snprintf(bt_dir, sizeof(bt_dir), "/tmp/%s/%s", buf1, buf2);
    } else {
        snprintf(bt_dir, sizeof(bt_dir), "%s", BT_DIR);
    }
    if (stat(bt_dir, &fstat) != 0) {
        websWrite(wp, "{\"failure\": \"true\", \"msg\": \"Operation error: %s/(%d)\"}",
                bt_dir, errno);
        goto error;
    }
    if (!(fstat.st_mode & S_IFDIR)) {
        websWrite(wp,
                "{\"failure\": \"true\", \"msg\": \"Directory doesn't exist: %s/(%d)\"}",
                bt_dir, errno);
        goto error;
    } else if (f_exists(tmp_file)) {
        snprintf(cmd, sizeof(cmd), "mv %s %s ", tmp_file, filename);
        if (!(fp = popen(cmd, "r"))) {
            pclose(fp);
            websWrite(wp, 
                    "{\"failure\": \"true\", \"msg\": \"Torrent move error: %s/(%d)\"}",
                    bt_dir, errno);
            goto error;
        }
        pclose(fp);
        snprintf(cmd, sizeof(cmd), "transmission-remote 127.0.0.1 -a %s",
                filename);
        fp = popen(cmd, "r");
        if(fp) {
            if(fgets(buf1, sizeof(buf1), fp)) {
                pclose(fp);
                sscanf(buf1, "%*[^\"]\"%[^\"]" , res);
                if(strcmp(res, "success")) {
                    websWrite(wp,
                        "{\"failure\": \"true\", \"msg\": \"torrent add fail. %s\"}", res);
                    goto error;
                }
           } else {
               websWrite(wp,
                    "{\"failure\": \"true\", \"msg\": \"torrent add fail.download engine is busy\"}");
               goto error;
           }
        }
        websWrite(wp, "{\"success\": \"true\", \"msg\": \"The seed has been uploaded\"}");
        unlink(filename);
        return;
    } else {
        websWrite(wp,
                "{\"failure\": \"true\", \"msg\": \"Upload file doesn't exist: %s(%d)\"}",
                tmp_file, errno);
        goto error;
    }

error:
    if (f_exists(tmp_file)) {
        unlink(tmp_file);
    }
    return;
}

int
do_bt_upload(char *fname, webs_t stream, int *total, char *boundary)
{
    char bottom[LONG_BUF_LEN];
    FILE *fifo = NULL;
    char *ptr;
    char *buf = NULL;
    int count; 
    int ret = 0;
    long flags = -1;
    int size = BUFSIZ;
    char target_filename[] = "ezp_seed.XXXXXXXXXX";
    char buf1[TMP_LEN], buf2[TMP_LEN], bt_dir[LONG_BUF_LEN];

    mkstemp(target_filename);

    /* BT directory */
    ezplib_get_attr_val("storage_state_rule", 0, "dev_name", buf1, sizeof(buf1),
            EZPLIB_USE_CLI);
    ezplib_get_attr_val("bt_rule", 0, "load_start", buf2, sizeof(buf2),
            EZPLIB_USE_CLI);
    if (buf2[0] != 0) {
        snprintf(bt_dir, sizeof(bt_dir), "/tmp/%s/%s", buf1, buf2);
    } else {
        snprintf(bt_dir, sizeof(bt_dir), "%s", BT_DIR);
    }

    snprintf(filename, sizeof(filename), "%s/%s.torrent", bt_dir, target_filename);

    strcpy(tmp_file, TMP_TEPLATE_NAME);
    mkstemp(tmp_file);
    if (!(fifo = fopen(tmp_file, "w"))) {
        ret = errno;
        goto err;
    }

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

    if ((buf = calloc(size, sizeof(char))) == NULL) {
        ret = ENOMEM;
        goto err;
    }

#ifdef DEBUG
    printf("Uploading the file %s\n", tmp_file);
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
                break;
            }
            count = safe_fread(buf, 1, size, stream);
            if (!count && (ferror(stream) || feof(stream)))
                break;
        }

        /* Different from the boundary got in the header, the up boundary 
         * would have two hyphens in front of the boundary string, and the
         * bottom boundary would has more two hyphens at the end.
         * eg., 
         * -----------------------------121871446228343 (from header)
         * -------------------------------121871446228343 (up)
         * -------------------------------121871446228343-- (bottom)
         */
        snprintf(bottom, sizeof(bottom), "--%s--", boundary);

        *total -= count;
        if (!(ptr = strstr(buf, bottom))) {
            safe_fwrite(buf, 1, count, fifo);
        } else {
            /* Stop at the bottom boundary */
            *ptr = '\0';
            count = ptr - &buf[0];
            safe_fwrite(buf, 1, count, fifo);
            break;
        }
#ifdef DEBUG
        printf(".");
#endif
    }
    fclose(fifo);
    fifo = NULL;

#ifdef DEBUG
    printf("done\n");
#endif

    if (!do_ssl) {
        /* Reset nonblock on the socket */
        if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
            ret = errno;
            goto err;
        }
    }

err:
    if (buf) {
        free(buf);
    }
    if (fifo) {
        fclose(fifo);
    }

#ifdef DEBUG
    if (ret) {
        perror(NULL);
    }
#endif

    return ret;
}

void
do_bt_upload_post(char *url, webs_t stream, int len, char *boundary)
{
    char buf[POST_BUF_SIZE];
    char fname[LONG_BUF_LEN];
    char *ptr1, *ptr2;

    /* Look for our part */
    while (len > 0) {
        if (!wfgets(buf, MIN(len + 1, sizeof(buf)), stream))
            return;
        len -= strlen(buf);
        if (!strncasecmp(buf, "Content-Disposition:", 20)) {
            if (!(ptr1 = strstr(buf, "filename="))) {
                continue;
            }
            ptr1 += strlen("filename=\"");
            ptr2 = strchr(ptr1, '\"');
            snprintf(fname, ptr2 - ptr1 + 1, "%s\n", ptr1);
            break;
        }
    }

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

    do_bt_upload(fname, stream, &len, boundary);

    /* Slurp anything remaining in the request */

    while (len--)
#if defined(HTTPS_SUPPORT)
        if (do_ssl)
            BIO_gets((BIO *) stream, buf, 1);
        else
#endif
            (void) fgetc(stream);
}
/* If data in buf is "a^b^c|d^e^g", this function will translate data to 
 * "a','b','c'],['d','e','g" and put data to buf2 */
void
trans2web(char *buf, int len, char *buf2)
{
    int i, j;
    for (i = 0, j = 0; i < len; i++, j++) {
        if(buf[i] == '^') {
            buf2[j] = '\'';
            buf2[j + 1] = ',';
            buf2[j + 2] = '\'';
            j+=2;
        } else if (buf[i] == '|') {
            buf2[j] = '\'';
            buf2[j + 1] = ']';
            buf2[j + 2] = ',';
            buf2[j + 3] = '[';
            buf2[j + 4] = '\'';
            j+=4;
        } else if ((buf[i] == '\'') || (buf[i] == '\"')) {
            /* some character will occur java script execute issue 
             * replace these character to a blank space */
            buf2[j] = ' ';
        } else {
            buf2[j] = buf[i];
        }
    }
}

int
ej_bt_show_content(int eid, webs_t wp, int argc, char_t **argv)
{
    FILE *fp1 = NULL, *fp2 = NULL;
    char *buf, *buf2;
    char bt_cpl_file[TMP_LEN], dev[SHORT_BUF_LEN];
    int total_used = 0, residual = LONG_BUF_LEN;

    ezplib_get_attr_val("storage_state_rule", 0, "dev_name", dev, sizeof(dev),
            EZPLIB_USE_CLI);
    snprintf(bt_cpl_file, sizeof(bt_cpl_file), "/tmp/%s/%s", dev, BT_COMPLETED);

#ifdef EZP_PROD_BRAND_AXIM
    if (!process_is_running("transmission-daemon")) {
        return 0;
    }
    if (!(fp1 = fopen(BT_STATUS, "r"))) {
        return 0;
    }
    if (!(fp2 = fopen(bt_cpl_file, "r"))) {
        return 4;
    }

#else
    websWrite(wp, "Ext.grid.dummyData = [\n");
    if (!process_is_running("transmission-daemon")) {
        websWrite(wp, "];");
        return 0;
    }
    if (!(fp1 = fopen(BT_STATUS, "r"))) {
        websWrite(wp, "];");
        return 0;
    }
    if (!(fp2 = fopen(bt_cpl_file, "r"))) {
        websWrite(wp, "];");
        return 4;
    }
#endif
    buf = calloc(LONG_BUF_LEN, sizeof(char));
    dumpinfo2buf(fp1, buf, &total_used, &residual, 0);
    dumpinfo2buf(fp2, buf, &total_used, &residual, 1);
    buf2 = calloc(total_used + LONG_BUF_LEN, sizeof(char));
    trans2web(buf, total_used, buf2);

#ifdef EZP_PROD_BRAND_AXIM
    websWrite2(wp, "dumpData = \"%s\";\n", buf);
#else
    websWrite2(wp, "['%s']];", buf2);
#endif

    free(buf);
    free(buf2);
    if (fp1){
        fclose(fp1);
    }
    if (fp2) {
        fclose(fp2);
    }
    return 0;    
}

int
valid_bt(webs_t wp, char *value, struct variable *v)
{
    char *val;
    char tmp[TMP_LEN];

    /* Enable */
    snprintf(tmp, sizeof(tmp), "bt_enable");
    val = websGetVar(wp, tmp, "");
    if (valid_choice(wp, val, &bt_variables[BT_ENABLE]) == FALSE) {
        return FALSE;
    }

    /* Peer Num Per Torrent*/
    snprintf(tmp, sizeof(tmp), "bt_peer_num");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_PEER_NUM]) == FALSE) {
        return FALSE;
    }

    /* Max Peer Num Global*/
    snprintf(tmp, sizeof(tmp), "bt_max_peer_num");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_MAX_PEER_NUM]) == FALSE) {
        return FALSE;
    }

    /* Port Range Start */
    snprintf(tmp, sizeof(tmp), "bt_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_PORT]) == FALSE) {
        return FALSE;
    }

    /* DHT Port */
    snprintf(tmp, sizeof(tmp), "bt_dht_port");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_DHT_PORT]) == FALSE) {
        return FALSE;
    }

    /* Max Download Rate */
    snprintf(tmp, sizeof(tmp), "bt_dl_rate");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_MAX_DOWNLOAD_RATE]) == FALSE) {
        return FALSE;
    }

    /* Max Upload Rate */
    snprintf(tmp, sizeof(tmp), "bt_ul_rate");
    val = websGetVar(wp, tmp, "");
    if (valid_range(wp, val, &bt_variables[BT_MAX_UPLOAD_RATE]) == FALSE) {
        return FALSE;
    }

    return TRUE;
}

int
save_bt(webs_t wp, char *value, struct variable *v, struct service *s)
{
    /* variale from user settting, get from web */
    char *enable, *peer, *max_peer, *download_rate, *upload_rate,
         *port_start, *dht_port;
    char *rule_set = "bt_rule";
    /* variable user can not setting, get from nvram rule */
    char tmp[TMP_LEN],  max_task[TMP_LEN], port_end[TMP_LEN],
         directory[TMP_LEN], session[TMP_LEN], load_start[TMP_LEN],
         check_hash[TMP_LEN];
    int change = 0;
    int64_t map = 0;

    /* Enable */
    snprintf(tmp, sizeof(tmp), "bt_enable");
    enable = websGetVar(wp, tmp, "");
         
    /* Peer Per Torrent*/
    snprintf(tmp, sizeof(tmp), "bt_peer_num");
    peer = websGetVar(wp, tmp, "");

    /* Max Peer All Torrent */
    snprintf(tmp, sizeof(tmp), "bt_max_peer_num");
    max_peer = websGetVar(wp, tmp, "");

    /* Max Download Tasks */
    ezplib_get_attr_val(rule_set, 0, "max_task", max_task,
            sizeof(max_task), EZPLIB_USE_CLI);

    /* Port Range Start */
    snprintf(tmp, sizeof(tmp), "bt_port");
    port_start = websGetVar(wp, tmp, "");

    /* Port Range End */
    ezplib_get_attr_val(rule_set, 0, "port_range_end", port_end,
            sizeof(port_end), EZPLIB_USE_CLI);

    /* DHT Port */
    snprintf(tmp, sizeof(tmp), "bt_dht_port");
    dht_port = websGetVar(wp, tmp, "");

    /* Max Download Rate */
    snprintf(tmp, sizeof(tmp), "bt_dl_rate");
    download_rate = websGetVar(wp, tmp, "");

    /* Max Upload Rate */
    snprintf(tmp, sizeof(tmp), "bt_ul_rate");
    upload_rate = websGetVar(wp, tmp, "");

    /* Check Hash */
    ezplib_get_attr_val(rule_set, 0, "check_hash", check_hash,
            sizeof(check_hash), EZPLIB_USE_CLI);

    /* Direction */
    ezplib_get_attr_val(rule_set, 0, "directory", directory,
            sizeof(directory), EZPLIB_USE_CLI);

    /* Session */
    ezplib_get_attr_val(rule_set, 0, "session", session,
            sizeof(session), EZPLIB_USE_CLI);

    /* Load Start */
    ezplib_get_attr_val(rule_set, 0, "load_start", load_start,
            sizeof(load_start), EZPLIB_USE_CLI);

    snprintf(tmp, sizeof(tmp), "%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s^%s",
            enable, peer, max_peer, max_task, download_rate,
            upload_rate, directory, session, load_start, port_start,
            port_end, dht_port, check_hash);

    if (strcmp(tmp, nvram_safe_get(rule_set))) {
        config_preaction(&map, v, s, "NUM=0", ""); 
        nvram_set(rule_set, tmp);
        change = 1;
    }

    if (change) {
        config_postaction(map, s, "NUM=0", "");
    }

    return change;
}
