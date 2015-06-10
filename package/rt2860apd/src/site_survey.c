#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


static const char base64_str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64_pad = '=';

static const signed char base64_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

static int base64decode_block(unsigned char *target, const char *data, size_t data_size)
{
    int w1,w2,w3,w4;
    int i;
    size_t n;

    if (!data || (data_size <= 0)) {
        return 0;
    }

    n = 0;
    i = 0;
    while (n < data_size-3) {
        w1 = base64_table[(int)data[n]];
        w2 = base64_table[(int)data[n+1]];
        w3 = base64_table[(int)data[n+2]];
        w4 = base64_table[(int)data[n+3]];

        if (w2 >= 0) {
            target[i++] = (char)((w1*4 + (w2 >> 4)) & 255);
        }
        if (w3 >= 0) {
            target[i++] = (char)((w2*16 + (w3 >> 2)) & 255);
        }
        if (w4 >= 0) {
            target[i++] = (char)((w3*64 + w4) & 255);
        }
        n+=4;
    }
    return i;
}

unsigned char *base64decode(const char *buf, size_t *size)
{
    if (!buf) return NULL;
    size_t len = strlen(buf);
    if (len <= 0) return NULL;
    unsigned char *outbuf = (unsigned char*)malloc((len/4)*3+3);
    const char *ptr = buf;
    int p = 0;

    do {
        ptr += strspn(ptr, "\r\n\t ");
        if (*ptr == '\0') {
            break;
        }
        len = strcspn(ptr, "\r\n\t ");
        if (len > 0) {
            p+=base64decode_block(outbuf+p, ptr, len);
            ptr += len;
        } else {
            break;
        }
    } while (1);

    outbuf[p] = 0;
    *size = p;
    return outbuf;
}

int get_ssid_bssid(char *survey_line, char *bssidbuf, char *ssidbuf) {
    char *pos1 = survey_line, *pos2 = survey_line;
    char cachechar;
    /* Rule : 
     * 1. find, from backward, first ':', as bssid position.
     * 2. from that position, find first backword $ and forward $, then we get
     * bssid field. */
    /*1$QG-E130D8$00:08:9b:e1:30:d8$WPA2PSK/AES$81$11b/g/n$NONE$In$*/
    if((pos1 = strrchr(pos2, ':'))) {
        cachechar = pos1[1];
        pos1[1] = 0;
        pos2 = strrchr(survey_line, '$') + 1;
        pos1[1] = cachechar;
        pos1 = strchr(pos2, '$');
    } else {
        return -1;
    }
    strncpy(bssidbuf, pos2, pos1 - pos2);
    bssidbuf[pos1 - pos2] = 0;
    pos1 = strchr(survey_line, '$') + 1;
    strncpy(ssidbuf, pos1, pos2 - pos1 - 1);
    ssidbuf[pos2 - pos1 - 1] = 0;
    return 0;
}

int get_bssid_ssid_level(char *survey_line, char **bssidpos, char **ssidpos, int *level) {
    char *pos1 = survey_line, *pos2 = survey_line;
    char cachechar;
    int i;
    /* Rule : 
     * 1. find, from backward, first ':', as bssid position.
     * 2. from that position, find first backword $ and forward $, then we get
     * bssid field. */
    /*1$QG-E130D8$00:08:9b:e1:30:d8$WPA2PSK/AES$81$11b/g/n$NONE$In$*/
    if((pos1 = strrchr(pos2, ':'))) {
        cachechar = pos1[1];
        pos1[1] = 0;
        pos2 = strrchr(survey_line, '$') + 1;
        pos1[1] = cachechar;
    } else {
        return -1;
    }
    if(bssidpos)
        *bssidpos = pos2;
    if(ssidpos)
        *ssidpos = strchr(survey_line, '$') + 1;
    if(level) {
        /* Get security */
        pos1 = strchr(pos2, '$') + 1;
        /* Get sig level */
        pos1 = strchr(pos1, '$') + 1;
        *level = atoi(pos1);
    }
    return 0;
}

typedef struct _survey_link_list {
    struct _survey_link_list *prev, *next;
    char *survey_line;
    int level;
    char *bssid;
    char *ssid;
} survey_link_list;

survey_link_list *slist_head = NULL, *slist_tail = NULL;
int printcnt = 0, addcnt = 0;


void print_node(survey_link_list * node) {
    printf("line:%s level:%d\n", node->survey_line, node->level);
    // printf("node:%p line:%s bssid:%s ssid:%s level:%d prev:%p next:%p\n", node, node->survey_line, node->bssid, node->ssid, node->level,  node->prev, node->next);
}

void print_list() {
    survey_link_list *node;
    printcnt = 0;
    for(node = slist_head; node; node = node->next) {
        print_node(node);
        printcnt ++;
    }
    printf("total print:%d\n", printcnt);
}

int search_list(char *ssid, char *bssid) {
    survey_link_list *node;
    char sbuf[64];
    for(node = slist_head; node; node = node->next) {
        snprintf(sbuf, sizeof(sbuf), "%s$", ssid);
        // printf("node->bssid:%s node->ssid:%s\n", node->bssid, node->ssid);
        if(((bssid) && !strncmp(node->bssid, bssid, strlen(bssid))) || 
           ((ssid) && !strncmp(node->ssid, sbuf, strlen(sbuf)))) {
            return 1;
        }
    }
    return 0;
}

void write_list(char *filename) {
    FILE *fn = fopen(filename ,"w+");
    survey_link_list *node;
    if(fn) {
        for(node = slist_head; node; node = node->next) {
            fwrite(node->survey_line, strlen(node->survey_line), sizeof(char), fn);
        }
        fclose(fn);
    }
}

survey_link_list* add_survey_line(char *line, char *target_ssid) {
    survey_link_list *node = (survey_link_list*)malloc(sizeof(survey_link_list));
    char sbuf[64];
    addcnt ++;
    node->survey_line = strdup(line);
    get_bssid_ssid_level(node->survey_line, &node->bssid, &node->ssid, &node->level);
    /* Here is a trick, if we want our activated ssid be the first item, set
     * to max power. */
    if(target_ssid && *target_ssid) {
        int slen;
        slen = snprintf(sbuf, sizeof(sbuf), "%s$", target_ssid);
        if(!strncmp(node->ssid, target_ssid, slen - 1)) {
            node->level = 110;
        }
    }
    node->prev = node->next = NULL;
    if(!slist_head) {
        slist_head = node;
        slist_tail = node;
        node->prev = node->next = NULL;
    } else {
        survey_link_list *travel_node, *peek_node;
        for(travel_node = slist_head; travel_node ; travel_node = travel_node->next) {
            peek_node = travel_node->next;
            if((node->level > travel_node->level) || 
               (node->level == travel_node->level && strcmp(node->ssid, travel_node->ssid) <= 0)){
                node->prev = travel_node->prev;
                travel_node->prev = node;
                node->next = travel_node;
                if(!node->prev)
                    slist_head = node;
                else
                    node->prev->next = node;
                break;
            } 
        }
        if(travel_node == NULL) {
            node->prev = slist_tail;
            slist_tail->next = node;
            slist_tail = node;
        }
    }
    return node;
}

int del_survey_node(survey_link_list **node) {
    if(!node || !*node) return -1;
    if((*node)->survey_line) free((*node)->survey_line);
    free(*node);
    *node = NULL;
    return 0;
}

void del_list() {
    survey_link_list *node, *node1 = NULL;
    for(node = slist_head; node; node = node1) {
        node1 = node->next;
        del_survey_node(&node);
    }
}

int main()
{
    char *decode_ssid = NULL;
    char cur_ssid[64]={0};
    char cur_bssid[20]={0};
    char bssidbuf[20];
    char ssidbuf[64];
    char buf[512]={0}/*, wifi_buf[512]={0}, pre_ssid[256]={0}*/;
    // char bssid_check_buf[2048] = "";
    char *delim = "$";
    char *tmp = NULL;
    int i, time_interval, pre_sig, pre_wave;

    FILE *p, *time, *wifi_tmp, *new_survey_record;
    time = fopen("/tmp/wifi_site_time", "r");
    ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", buf, sizeof(buf), 0);
    /* Check if wisp started */
    if(buf[0] == '1') {
        size_t len = 0;
        ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", cur_ssid, sizeof(cur_ssid), 0);
        ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", cur_bssid, sizeof(cur_bssid), 0);
        if(cur_ssid[0] != '\0') {
            decode_ssid = base64decode(cur_ssid, &len);
        }
    }

    struct timespec cur_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);

    if(time!=NULL) {
        fgets(buf, sizeof(buf), time);
        double previous_time = atof(buf);
        time_interval = cur_time.tv_sec - previous_time;
        fclose(time);
    }
    sprintf(buf, "echo \'%ld\' > /tmp/wifi_site_time", cur_time.tv_sec);
    system(buf);

#define SS_FILE1    "/tmp/wifi_site1"
#define SS_FILE2    "/tmp/wifi_site2"
#define SS_FILE3    "/tmp/wifi_site3"
#define SS_FILE4    "/tmp/wifi_site4"
#define SS_FILE5    "/tmp/wifi_site5"
#define SS_FILE6    "/tmp/wifi_site6"
#define SS_COMBINE  "/tmp/wifi_site_not_encode"
    char *wifi_site_files[] = {
        SS_FILE1,
        SS_FILE2,
        SS_FILE3,
        SS_FILE4,
        SS_FILE5,
        SS_FILE6,
    };
#define LASTREC(_RECARR_)     (sizeof(_RECARR_)/sizeof(_RECARR_[0]) - 1)
    char *wifi_site_current = wifi_site_files[0];
    struct stat statbuf;

    if(time_interval>300) {
        unlink(SS_COMBINE);
        for(i = 0; i <= LASTREC(wifi_site_files); i++) {
            unlink(wifi_site_files[i]);
        }
    }
    /* First, if the last record exist, remove it */
    if(stat(wifi_site_files[LASTREC(wifi_site_files)], &statbuf) == 0) {
        // printf("unlink %s\n", wifi_site_files[LASTREC(wifi_site_files)]);
        unlink(wifi_site_files[LASTREC(wifi_site_files)]);
    }
    /* Second, we move all record downward one : 1->2, 2->3 */
    for(i = LASTREC(wifi_site_files);i > 0; i--) {
        if(stat(wifi_site_files[i - 1], &statbuf) == 0) {
            // printf("move %d:%s to %d:%s\n", i-1, wifi_site_files[i - 1], i, wifi_site_files[i]);
            rename(wifi_site_files[i - 1], wifi_site_files[i]);
        }
    }
    /* Third, do the next survey */ 
    snprintf(buf, sizeof(buf), "/sbin/getsysinfo site_survey > %s", wifi_site_current);
    system(buf);
    /* Third, filter out duplicated ssid records and add into a sorted
     * link list */
    for(i = LASTREC(wifi_site_files);i > -1; i--) {
        if((p = fopen(wifi_site_files[i], "r")))
        {
            while(fgets(buf, sizeof(buf), p)) {
                get_ssid_bssid(buf, bssidbuf, ssidbuf);
                if(!search_list(ssidbuf, bssidbuf)) {
                    survey_link_list* node;
                    node = add_survey_line(buf, decode_ssid);
                }
            }
            fclose(p);
        }
    }
    write_list(SS_COMBINE);
    del_list();
    if(decode_ssid)
        free(decode_ssid);
    return 0;
}
