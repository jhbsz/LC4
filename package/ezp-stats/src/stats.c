/*
 *	stats
 *	Copyright (C) 2006-2007 EZ Packet Technologies
 */
/*

	rstats
	Copyright (C) 2006-2007 Jonathan Zarate

	Licensed under GNU GPL v2 or later.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <stdint.h>
#include <syslog.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <bcmnvram.h>
#include <shutils.h>
#include <shared.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>

#include "ezp-lib.h"
#include "httpd.h"

#define DEBUG_NOISY 

#ifdef DEBUG_NOISY
#define _dprintf(args...)	printf(args)
#else
#define _dprintf(args...)	do { } while (0)
#endif

#define K 1024
#define M (1024 * 1024)
#define G (1024 * 1024 * 1024)

#define SMIN	60
#define	SHOUR	(60 * 60)
#define	SDAY	(60 * 60 * 24)
#define	SWEEK	(60 * 60 * 24 * 7)
#define	SMONTH	(60 * 60 * 24 * 30)
#define Y2K		946684800UL

#define WIDGET_INTERVAL	5
#define INTERVAL		60
#define TIMER_DEVIATION 6

#define UNIT_NHOUR      (INTERVAL)                 /* 1 min interval */
#define MAX_NHOUR	    ((2 * SHOUR) / UNIT_NHOUR)
#define UNIT_NDAY       (10 * INTERVAL)            /* 10 min interval */
#define MAX_NDAY	    (SDAY / UNIT_NDAY) 
#define UNIT_NWEEK      (SHOUR)                    /* 60 min interval */
#define MAX_NWEEK	    (SWEEK / UNIT_NWEEK)   
#define UNIT_NMONTH     (6 * SHOUR)                /* 360 min interval */
#define MAX_NMONTH	    (SMONTH / UNIT_NMONTH)

#define MAX_NDAILY		62
#define MAX_NMONTHLY	25
#define MAX_SPEED_IF	16
#define MAX_CT_PROTO    4
#define MAX_ROLLOVER	(225 * M)

#define MAX_COUNTER	2

#define MAX_NINTERVAL     4

#define TWOHOURLY   0
#define DAILY       1
#define WEEKLY      2
#define MONTHLY     3	

#define ID_V0		0x30305352
#define ID_V1		0x31305352
#define CURRENT_ID	ID_V1

#define SHORT_BUF_LEN 32
#define MIDDLE_BUF_LEN 128
#define LONG_BUF_LEN 256

#define XMLRPCTIMEOUT 5

#define STORAGR_MODEL "C|T|D|K|G"
#define WWAN_MODEL "M|N|P|U|I|A"

#define TMP_LEN 256

enum {
    THP = 0,
    CT
};

typedef enum {
    ST_INFO_TOTAL = 2,
    ST_INFO_USED = 3, 
    ST_INFO_FREE = 4,
} storage_info_enum;

typedef struct {
	uint32_t xtime;
	uint64_t counter[MAX_COUNTER];
} data_t;

typedef struct {
	uint32_t id;
	
	data_t daily[MAX_NDAILY];
	int dailyp;

	data_t monthly[MAX_NMONTHLY];
	int monthlyp;
} hist_t;

typedef struct {
	uint32_t id;
	
	data_t daily[62];
	int dailyp;

	data_t monthly[12];
	int monthlyp;
} hist_v0_t;

typedef struct {
	char name[15];      // thp: ifname, ct: proto name
	char sync;

	int hour_tail;
	int day_tail;
	int week_tail;
	int month_tail;

	long hour_utime;
	long day_utime;
	long week_utime;
	long month_utime;

	unsigned long hour_last[MAX_COUNTER];
	unsigned long day_last[MAX_COUNTER];
	unsigned long week_last[MAX_COUNTER];
	unsigned long month_last[MAX_COUNTER];

	unsigned long hour[MAX_NHOUR][MAX_COUNTER];
	unsigned long day[MAX_NDAY][MAX_COUNTER];
	unsigned long week[MAX_NWEEK][MAX_COUNTER];
	unsigned long month[MAX_NMONTH][MAX_COUNTER];

    unsigned long lifetime_max;
} mrtg_t;

char ct_proto_name [][SHORT_BUF_LEN] = {
    "total", "tcp", "udp", "icmp"
};

#if 0
/* For linux kernel 2.4 */
char ct_proto_open_fn[][MIDDLE_BUF_LEN] = {
    "ip_conntrack_open_count",
    "ip_conntrack_tcp_open_count",
    "ip_conntrack_udp_open_count",
    "ip_conntrack_icmp_open_count"
};

char ct_proto_close_fn[][MIDDLE_BUF_LEN] = {
    "ip_conntrack_close_count",
    "ip_conntrack_tcp_close_count",
    "ip_conntrack_udp_close_count",
    "ip_conntrack_icmp_close_count"
};
#endif

char ct_proto_open_fn[][MIDDLE_BUF_LEN] = {
    "nf_conntrack_open_count",
    "nf_conntrack_tcp_open_count",
    "nf_conntrack_udp_open_count",
    "nf_conntrack_icmp_open_count"
};

char ct_proto_close_fn[][MIDDLE_BUF_LEN] = {
    "nf_conntrack_close_count",
    "nf_conntrack_tcp_close_count",
    "nf_conntrack_udp_close_count",
    "nf_conntrack_icmp_close_count"
};

hist_t hist;
mrtg_t thp[MAX_SPEED_IF];
mrtg_t ct[MAX_CT_PROTO];
int nwan;
char **wan_ifname;
unsigned long **cnt_now;
unsigned long *cnt_rx_max;
unsigned long *cnt_tx_max;
int thp_count;
long save_utime;
long start_utime;
char save_path[MIDDLE_BUF_LEN];
int gothup = 0;
int gotuser = 0;
int gotterm = 0;

unsigned long PROD_CAT_BITMAP;
int ezp_prod_subcat;
char ezp_prod_cat[4];
char ezp_prod_subsubcat[4];
char *prod_subsubcat;
void inital_prod();

const char pid_fn[] = "/var/run/stats.pid";
const char dir_fn[] = "/var/stats";
const char thp_fn[] = "/var/stats/stats-mrtg";
const char mrtgjs_fn[] =      "/var/stats/stats-mrtg.js";
const char hist_fn[] = "/var/stats/stats-hist";
const char histjs_fn[] =        "/var/stats/stats-hist.js";
const char histjs_tmp_fn[] =    "/var/stats/stats-hist.js.tmp";
const char uncomp_fn[] = "/var/stats/stats-uncomp";
const char source_fn[] = "/var/stats/stats-source";
const char stime_fn[] = "/var/stats/stats-stime";
const char load_fn[] = "/var/stats/stats-load";
const char widget_system_fn[] = "/var/stats/stats-widget-system";
const char widget_sig_strength_fn[] = "/var/stats/stats-widget-wwan";
const char widget_sig_strength_tmp_fn[] = "/var/stats/stats-widget-wwan.tmp";

#if 0
/* For linux kernel 2.4 */
const char proto_count_dir[] = "/proc/sys/net/ipv4/netfilter/";
#endif
const char proto_count_dir[] = "/proc/sys/net/netfilter/";

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
    else if (!strcmp(ezp_prod_cat, "K")) PROD_CAT_BITMAP=PROD_CAT_K;
    else if (!strcmp(ezp_prod_cat, "U")) PROD_CAT_BITMAP=PROD_CAT_U;
    if ((ezp_prod_subsubcat[0]) && !strcmp(ezp_prod_subsubcat, "L")) prod_subsubcat  ="PROD_SUBSUBCAT_L";
    else prod_subsubcat="NOT_SET";
}

void
get_storage_space_info(char *mount_string, storage_info_enum info_type, char *result_buffer, int buffer_size, int space_plus_ro) {
    char tmp3[TMP_LEN]={0}, tmp4[TMP_LEN]={0};
    FILE *file, *file2;
    snprintf(tmp3, sizeof(tmp3), "mount | grep %s", mount_string);
    if ((file2 = popen(tmp3, "r"))) {
        fgets(tmp4, sizeof(tmp4), file2);
    }
    pclose(file2);
    /* The df format is 
     * /dev/sdb1              3944740   3182596    762144 81% /tmp/mnt/USB 
     * $1                     $2:total  $3:used    $4:free
     * */
    if (strcmp(tmp4, "")) {
        if (space_plus_ro || strstr(tmp4, "rw")) {
            snprintf(tmp3, sizeof(tmp3), "df | grep %s | awk '{print $%d}'", mount_string, info_type);
            if ((file = popen(tmp3, "r"))) {
                if(space_plus_ro && (!strstr(tmp4, "rw"))) {
                    snprintf(result_buffer, buffer_size, "Read Only");
                } else {
                    snprintf(result_buffer, buffer_size, "Read/Write");
                }
                pclose(file);
            } else {
                snprintf(result_buffer, buffer_size, "Not Available");
            }
        } else {
            snprintf(result_buffer, buffer_size, "Read Only");
        }
    } else {
        snprintf(result_buffer, buffer_size, "Not Available");
    }
}

void
adj_size(float val, char *buf, int blen)
{
    if (val < 1024) {
        snprintf(buf, blen,"%.1fKB", val);
    } else {
        val = val / 1024;
        if (val < 1024) {
        snprintf(buf, blen,"%.1fMB", val);
        } else {
            val = val / 1024;
            snprintf(buf, blen,"%.1fGB", val);
        }
    }
    return;
}

int
show_uptime(char *ptr, int length)
{
    int n, updays, uphours, upminutes;
    struct sysinfo info;
    sysinfo(&info);
    updays = (int) (info.uptime / (60*60*24));

    if (updays) {
        n = snprintf(ptr, length, "%d day%s, ", updays,
                                    (updays != 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    }
    upminutes = (int) info.uptime / 60;
    uphours = (upminutes / 60) % 24;
    upminutes %= 60;
    if(uphours) {
        n = snprintf(ptr, length, "%2d hour%s, ", uphours,
                (uphours > 1) ? "s" : "");
        if (n <= 0)
            return 1;
        ptr += n;
        length -=n;
    }

    n = snprintf(ptr, length, "%2d min%s", upminutes,
            (upminutes > 1) ? "s" : "");
    if (n <= 0)
        return 1;
    ptr += n;
    length -=n;

    return 0;
}

int
show_localtime(char *buf, int buf_len)
{       
#ifdef EZP_PROD_BRAND_PROX
    /* char tzarray[13][3] = {"00", "01", "02", "03", "04", "05", "06", \ */
    /* "07", "08", "09", "10", "11", "12"}; */
    /* char tzmin[][3] = {"00", "30"}; */
    long dtime = 0;
    char *strtm, *strzone;
    time_t tm1, tm2;
    struct tm *t1, *t2;
#else    
    struct tm tm;
    time_t timep;
#endif
    
    
        
#ifdef EZP_PROD_BRAND_PROX
    strtm = malloc(120 * sizeof(char));
    strzone = malloc(50 * sizeof(char));
    tm1 = time(NULL);
    t2 = gmtime(&tm1);
    tm2 = mktime(t2);
    t1 = localtime(&tm1);
    dtime = (long)(tm1 - tm2);

    /* Print local time as a string */
#if 0
    strftime(strtm, 100, "%a %b %d %Y %H:%M:%S GMT", t1);
    if (dtime >= 0 )    
        strcat(strtm, "+");
    else 
        strcat(strtm, "-");
    strcat(strtm, tzarray[(short)(abs(dtime) / 3600)]);
    if (1800 == (short)(abs(dtime) % 3600))
        strcat(strtm, tzmin[1]);
    else
        strcat(strtm, tzmin[0]);
#else
    strftime(strtm, 100, "%a %b %d %Y %H:%M:%S %Z", t1);
#endif
    snprintf(buf, buf_len - 1, "%s", strtm);
    free(strtm);
    free(strzone);
#else
    timep = time(NULL);
    memcpy(&tm, localtime(&timep), sizeof(struct tm));
    strftime(buf, buf_len - 1, "%a, %d %b %Y %H:%M:%S", &tm);
#endif
    return 0;
}


static int get_stime(void)
{
	int t;
    char tmp[SHORT_BUF_LEN];
    ezplib_get_attr_val("stats_rule", 0, "stime", tmp, SHORT_BUF_LEN,
            EZPLIB_USE_CLI);
	t = atoi(tmp);
	if (t < 1) 
        t = 60;
    return t;
}

static int comp(const char *path, void *buffer, int size)
{
	char s[LONG_BUF_LEN];

	if (f_write(path, buffer, size, 0, 0) != size) return 0;
	
	sprintf(s, "%s.gz", path);
	unlink(s);

	sprintf(s, "gzip %s", path);
	return system(s) == 0;
}

int base64_encoded_len(int len)
{
        return ((len + 2) / 3) * 4;
}

int base64_decoded_len(int len)
{
        return ((len + 3) / 4) * 3;
}

static void save(int quick)
{
	int i;
	char *bi, *bo;
	unsigned long  n;
	char hgz[LONG_BUF_LEN];
	char tmp[LONG_BUF_LEN];

	_dprintf("%s: quick=%d\n", __FUNCTION__, quick);

	f_write(stime_fn, &save_utime, sizeof(save_utime), 0, 0);

	comp(thp_fn, thp, sizeof(thp[0]) * thp_count);

/*	
	if ((now = time(0)) < Y2K) {
		_dprintf("%s: time not set\n", __FUNCTION__);
		return;
	}
*/

	comp(hist_fn, &hist, sizeof(hist));
	
	_dprintf("%s: write source=%s\n", __FUNCTION__, save_path);
	f_write_string(source_fn, save_path, 0, 0);
	
	if (quick) {
		return;
	}

	sprintf(hgz, "%s.gz", hist_fn);
	
	if (strcmp(save_path, "*nvram") == 0) {
        /*
		if (!wait_action_idle(10)) {
			_dprintf("%s: busy, not saving\n", __FUNCTION__);
			return;
		} */

		if ((n = f_read_alloc(hgz, &bi, 20 * 1024)) > 0) {
			if ((bo = malloc(base64_encoded_len(n) + 1)) != NULL) {
                n = base64_encoded_len(n) + 1;
                base64_encode((unsigned char *)bi,strlen(bi),
                                               (unsigned char *)bo, &n);
				bo[n] = 0;
				nvram_set("stats_data", bo);
				if (!nvram_match("debug_nocommit", "1")) nvram_commit();
				
				_dprintf("%s: nvram commit\n", __FUNCTION__);
				
				free(bo);
			}
		}
		free(bi);
	}
	else if (save_path[0] != 0) {
		strcpy(tmp, save_path);
		strcat(tmp, ".tmp");
		
		for (i = 15; i > 0; --i) {
            /*
			if (!wait_action_idle(10)) {
				_dprintf("%s: busy, not saving\n", __FUNCTION__);
			} 
			else  */
            {
				_dprintf("%s: cp %s %s\n", __FUNCTION__, hgz, tmp);
				if (eval("cp", hgz, tmp) == 0) {
					_dprintf("%s: copy ok\n", __FUNCTION__);
					_dprintf("%s: rename %s %s\n", __FUNCTION__, tmp, save_path);
					if (rename(tmp, save_path) == 0) {
						_dprintf("%s: rename ok\n", __FUNCTION__);
						break;
					}
				}
			}

			// might not be ready
			sleep(3);
			if (gotterm) break;
		}
	}
}

static int decomp(const char *fname, void *buffer, int size, int max)
{
	char s[LONG_BUF_LEN];
	int n;

	_dprintf("%s: fname=%s\n", __FUNCTION__, fname);

	unlink(uncomp_fn);

	n = 0;
	sprintf(s, "gzip -dc %s > %s", fname, uncomp_fn);
	if (system(s) == 0) {
		n = f_read(uncomp_fn, buffer, size * max);
		_dprintf("%s: n=%d\n", __FUNCTION__, n);
		if (n <= 0) n = 0;
			else n = n / size;
	}
	else {
		_dprintf("%s: %s != 0\n", __FUNCTION__, s);
	}
	unlink(uncomp_fn);
	memset((char *)buffer + (size * n), 0, (max - n) * size);	
	return n;
}

static void clear_hist(void)
{
	memset(&hist, 0, sizeof(hist));
	hist.id = CURRENT_ID;
}

static int load_hist(const char *fname)
{
	hist_t hist;

	_dprintf("%s: fname=%s\n", __FUNCTION__, fname);

	if ((decomp(fname, &hist, sizeof(hist), 1) != 1) || (hist.id != CURRENT_ID)) {
		hist_v0_t v0;
		
		if ((decomp(fname, &v0, sizeof(v0), 1) != 1) || (v0.id != ID_V0)) {
			_dprintf("%s: load failed\n", __FUNCTION__);
			return 0;
		}
		else {
			// --- temp conversion ---
			clear_hist();

			// V0 -> V1
			hist.id = CURRENT_ID;
			memcpy(hist.daily, v0.daily, sizeof(hist.daily));
			hist.dailyp = v0.dailyp;
			memcpy(hist.monthly, v0.monthly, sizeof(v0.monthly));	// v0 is just shorter
			hist.monthlyp = v0.monthlyp;
		}
	} 
#if 0
    /* XXX: statement with no effect */
    else {
		memcpy(&hist, &hist, sizeof(hist));
	}
#endif

	_dprintf("%s: dailyp=%d monthlyp=%d\n", __FUNCTION__, hist.dailyp, hist.monthlyp);
	return 1;
}

static void load_new(void)
{
	char hgz[LONG_BUF_LEN];

	sprintf(hgz, "%s.gz.new", hist_fn);
	if (load_hist(hgz)) save(0);
	unlink(hgz);
}

static void load(long uptime, int new)
{
	int i;
	long t;
	char *bi, *bo;
	unsigned long n;
	char hgz[LONG_BUF_LEN];
	char sp[sizeof(save_path)];
	unsigned char mac[6];

	strlcpy(save_path, nvram_safe_get("stats_path"), sizeof(save_path) - 32);
	if (((n = strlen(save_path)) > 0) && (save_path[n - 1] == '/')) {
		ether_atoe(nvram_safe_get("et0macaddr"), mac);
		sprintf(save_path + n, "stats_%02x%02x%02x%02x%02x%02x.gz",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}

	if (f_read(stime_fn, &save_utime, sizeof(save_utime)) != sizeof(save_utime)) {
		save_utime = 0;
	}
	t = uptime + get_stime();
	if ((save_utime < uptime) || (save_utime > t)) save_utime = t;
	_dprintf("%s: uptime = %lu, save_utime = %lu\n", __FUNCTION__, uptime, save_utime);

	sprintf(hgz, "%s.gz", thp_fn);
	thp_count = decomp(hgz, thp, sizeof(thp[0]), MAX_SPEED_IF);
	_dprintf("%s: thp_count = %d\n", __FUNCTION__, thp_count);

	sprintf(hgz, "%s.gz", hist_fn);

	if (new) {
		unlink(hgz);
		save_utime = 0;
		return;
	}
	
	f_read_string(source_fn, sp, sizeof(sp));	// always terminated
	_dprintf("%s: read source=%s save_path=%s\n", __FUNCTION__, sp, save_path);
	if ((strcmp(sp, save_path) == 0) && (load_hist(hgz))) {
		_dprintf("%s: using local file\n", __FUNCTION__);
		return;
	}
	
	if (save_path[0] != 0) {
		if (strcmp(save_path, "*nvram") == 0) {
            /*
			if (!wait_action_idle(60)) exit(0);
            */
			
			bi = nvram_safe_get("stats_data");
			if ((n = strlen(bi)) > 0) {
				if ((bo = malloc(base64_decoded_len(n))) != NULL) {
                    n = base64_decoded_len(n);
                    base64_decode((unsigned char *)bi, strlen(bi),
                            (unsigned char *)bo, &n);
					_dprintf("%s: nvram n=%lu\n", __FUNCTION__, n);
					f_write(hgz, bo, n, 0, 0);
					free(bo);
					load_hist(hgz);
				}
			}
		}
		else {
			i = 0;
			while (1) {
            /*
				if (wait_action_idle(10)) {

					// try forcing retrieval of fresh directory entries
					eval("ls", save_path);

					if (load_hist(save_path)) {
						f_write_string(source_fn, save_path, 0, 0);
						break;
					}
				}
                */
				
				// not ready...
				sleep(2);

				if (gotterm) {
					save_path[0] = 0;
					return;
				}

				_dprintf("%s: not ready i=%d\n", __FUNCTION__, i);
				if ((++i % 600) == 90) {	// aprox. every 20m, 3m into it
					syslog(LOG_ERR, "Problem loading %s. Still trying...", save_path);
				}
			}
		}
	}
}

static void
save_mrtgjs(long uptime, const char *file, int count, mrtg_t *pp, int type,
        char *name)
{
	int i, j, k, l;
	mrtg_t *sp;
	int q, p;
	FILE *f;
	uint64_t tsum;
	uint64_t tmax;
	unsigned long n = 0;
    long tdiff;
    char *tmp;
    int time, tindex;
    int num, match = 0;
    unsigned long *array;
    char tmpfile[SHORT_BUF_LEN];
    uint32_t sum[MAX_COUNTER], max[MAX_CT_PROTO];
    char *targname = nvram_safe_get("stats_ifname");

    if(!targname || !*targname) {
        targname = "br0";
    }
    snprintf(tmpfile, SHORT_BUF_LEN, "%s.tmp", file);

	if ((f = fopen(tmpfile, "w")) == NULL) 
        return;

    /* stats_interval determine the displayed contents. */
    tmp = nvram_safe_get("stats_interval");
    _dprintf("%s: stats_interval = %s\n", __FUNCTION__, tmp);
    time = atoi(tmp);

    /* determine the chosen name, or all names */
	fprintf(f, "names = [\n");
    if(count > 0) {
        for (i = 0, l = 0; i < count; ++i) {
            sp = &pp[i];
            if (strcmp(name, "all") && strcmp(sp->name, name)) {
                /* If not matched, move to next. */
                continue;
            }
            fprintf(f, "%s\"%s\"", (l) ? ", ": "", sp->name);
            l++;
        }     
    } else {
        fprintf(f, "\"%s\"", targname);
    }
	fprintf(f, "];\n");
	
    /* data */
	fprintf(f, "mrtg_data = {\n");
    if(count > 0) {
        for (i = 0, k = 0; i < count; ++i) {
            sp = &pp[i];
            if (strcmp(name, "all") && strcmp(sp->name, name)) {
                /* If not matched, move to next. */
                continue;
            }
            /* Hour/Day/Week/Month. */
            if (time == UNIT_NHOUR) {
                num = MAX_NHOUR;
                q = sp->hour_tail;
                array = &sp->hour[0][0];
                tindex = TWOHOURLY;    
            } else if (time == UNIT_NDAY) {
                num = MAX_NDAY;
                q = sp->day_tail;
                array = &sp->day[0][0];
                tindex = DAILY;    
            } else if (time == UNIT_NWEEK) {
                num = MAX_NWEEK;
                q = sp->week_tail;
                array = &sp->week[0][0];
                tindex = WEEKLY;    
            } else if (time == UNIT_NMONTH) {
                num = MAX_NMONTH;
                q = sp->month_tail;
                array = &sp->month[0][0];
                tindex = MONTHLY;    
            } else {
                assert(0);
                return;
            }

            _dprintf("%s: [%s] stats prints out\n", __FUNCTION__, name);
            _dprintf("%s: num[%d] tail[%d]\n", __FUNCTION__, num, q);

            /* Real tracking uptime for calculation. */
            tdiff = (uptime > time * num)?  (num * time) : (uptime - start_utime);
            _dprintf("%s: tdiff %lu\n", __FUNCTION__, tdiff);

            /* Content. */
            fprintf(f, "%s'%s': {\n", k ? "},\n" : "", sp->name);
            /* There is an interface matched. */
            match = 1;

            for (j = 0; j < MAX_COUNTER; ++j) {
                /* thp: xxx_0 for RX, xxx_1 for TX
                 * ct: xxx_0 for the increased session number,
                 *     xxx_1 for the extant session number.
                 */
                tsum = tmax = 0;
                p = q;
                fprintf(f, " points_%d: [", j);
                for (k = 0; k < num; ++k) {
                    /* Slots. */
                    p = (p + 1) % num;
                    n = *(array + (p * MAX_COUNTER) + j);
                    fprintf(f, "%s%lu", k ? "," : "", n);
                    tsum += n * time;
                    if (n > tmax) {
                        tmax = n;
                    }
                }
                fprintf(f, "],\n");
                max[j] = tmax;

                if (type == THP) {
                    fprintf(f, 
                            " cur_%d: %lu,\n avg_%d: %llu,\n max_%d: %llu,\n"
                            " total_%d: %llu%s",
                            j, n, j, tsum / tdiff, j, tmax, j, tsum,
                            (j == MAX_COUNTER - 1) ? "" : ",\n");
                }
                /* tsum = n(0)*time + n(1)*time + n(2)*time + ... + n(num)*time
                 *      = (n(0) + n(1) + ... + n(num)) * time
                 *      = sum(n) * time
                 * so, sum(n) = tsum / time
                 */
                sum[j] = tsum / time;
            }

            if (type == CT) {
                /* max[1] represents the max session numbers displayed in the
                 * interval.
                 * sum[0] represents the total opened session numbers in all time
                 * slices. */
                fprintf(f, " cur: %lu,\n lifetime_max: %lu,\n max: %d,\n opened: %d",
                        n, sp->lifetime_max, max[1], sum[0]);
            }
            k++;
        }
    } else {
        if (time == UNIT_NHOUR) {
            num = MAX_NHOUR;
        } else if (time == UNIT_NDAY) {
            num = MAX_NDAY;
        } else if (time == UNIT_NWEEK) {
            num = MAX_NWEEK;
        } else if (time == UNIT_NMONTH) {
            num = MAX_NMONTH;
        } else {
            num = MAX_NHOUR;
        }
        fprintf(f, "'%s': {\n points_0: [0", targname);
        for(i = 0;i < num;i++) {
            fprintf(f, ",%d", 0);
        }
        fprintf(f, "],\ncur_0: 0,\navg_0: 0,\nmax_0: 0,\ntotal_0: 0,\npoints_1: [0");
        for(i = 0;i < num;i++) {
            fprintf(f, ",%d", 0);
        }
        fprintf(f, "],\ncur_1: 0,\navg_1: 0,\nmax_1: 0,\ntotal_1: 0}\n");
    }
    /* For example, if 'br0' is matched, the output will be shown as,
     *     mrtg_data = { 'br0': { .......
     * Therefore, it needs '}' to close the definition of 'br0' before '};'.
     * Contrarily, if there is no interface matched, the output will be shown
     * as,
     *     mrtg_data = {
     * Therefore, only one '};' is needed.
     */
    fprintf(f, "%s};", (count && match)  ? "}\n" : "");
	fclose(f);
    rename(tmpfile, file);
}

static void save_datajs(FILE *f, int mode)
{
	data_t *data;
	int p;
	int max;
	int k, kn;

	fprintf(f, "\n%s_data = [\n", (mode == DAILY) ? "daily" : "monthly");

	if (mode == DAILY) {
		data = hist.daily;
		p = hist.dailyp;
		max = MAX_NDAILY;
	}
	else {
		data = hist.monthly;
		p = hist.monthlyp;
		max = MAX_NMONTHLY;
	}
	kn = 0;
	for (k = max; k > 0; --k) {
		p = (p + 1) % max;
		if (data[p].xtime == 0) continue;
		fprintf(f, "%s[0x%lx,0x%llx,0x%llx]", kn ? "," : "",
			(unsigned long)data[p].xtime, data[p].counter[0] / K, data[p].counter[1] / K);
		++kn;
	}
	fprintf(f, "];\n");
}

static void save_histjs(void)
{
	FILE *f;

	if ((f = fopen(histjs_tmp_fn, "w")) != NULL) {
		save_datajs(f, DAILY);
		save_datajs(f, MONTHLY);
		fclose(f);
		rename(histjs_tmp_fn, histjs_fn);
	}
}


static void bump(data_t *data, int *tail, int max, uint32_t xnow, unsigned long *counter)
{
	int t, i;

	t = *tail;
	if (data[t].xtime != xnow) {
		for (i = max - 1; i >= 0; --i) {
			if (data[i].xtime == xnow) {
				t = i;
				break;
			}
		}
		if (i < 0) {
			*tail = t = (t + 1) % max;
			data[t].xtime = xnow;
			memset(data[t].counter, 0, sizeof(data[0].counter));
		}
	}
	for (i = 0; i < MAX_COUNTER; ++i) {
		data[t].counter[i] += counter[i];
	}
}

static unsigned long
diff_complement(unsigned long c, unsigned long sc)
{
    unsigned long diff;
    if (c < sc) {
        diff = (0xFFFFFFFF - sc) + c;
    } else {
        diff = c - sc;
    }
    return diff;
}

static void
get_usbinfo(FILE *fo)
{
    if (strstr(STORAGR_MODEL, ezp_prod_cat)) {
        char device_name[SHORT_BUF_LEN];
        ezplib_get_attr_val("storage_state_rule", 0, "dev_name",
                device_name, sizeof(device_name), EZPLIB_USE_CLI);
        if (device_name[0] == 0) {
            fprintf(fo, "USB=unloaded\n");
        } else {
            fprintf(fo, "USB=loaded\n");
        }
    } else if (strstr(WWAN_MODEL, ezp_prod_cat)) {
        char tmp1[SHORT_BUF_LEN], tmp2[SHORT_BUF_LEN], buf[SHORT_BUF_LEN];

        ezplib_get_attr_val("wan_wwan_probe_rule", 0,
                "ctrltty", tmp2, sizeof(tmp2), EZPLIB_USE_CLI);
        ezplib_get_attr_val("wan_wwan_probe_rule", 0,
                "devname", tmp1, sizeof(tmp1), EZPLIB_USE_CLI);
        snprintf(buf, sizeof(buf), "/dev/%s%s", tmp1, tmp2);
        if (f_exists(buf)) {
            fprintf(fo, "USB=loaded\n");
        } else {
            fprintf(fo, "USB=unloaded\n");
        }
    }
}

static void
get_throughput_last(FILE *fo, long uptime_interval, int uptime_now) {
    unsigned long *array;
	int i, num, tail_index;
    int time;
    char *tmp;
    char *name;
    mrtg_t *sp;

    /* stats_interval determine the displayed contents. */
    tmp = nvram_safe_get("stats_interval");
    time = atoi(tmp);

    name = nvram_safe_get("stats_ifname");
    _dprintf("%s: stats_interval = %s thp_count:%d ifname:%s\n", __FUNCTION__, tmp, thp_count, name);
    if(!thp_count) {
        fprintf(fo, "last_thp_timestamp=%lu\nlast_thp_tx=%d\nlast_thp_rx=%u\n", uptime_interval, 0, 0);
    }
    /* data */
	for (i = 0; i < thp_count; ++i) {
		sp = &thp[i];
        if (strcmp(name, "all") && strcmp(sp->name, name)) {
            /* If not matched, move to next. */
            continue;
        }

        _dprintf("%s: [%s] stats prints out\n", __FUNCTION__, name);
        /* Hour/Day/Week/Month. */
        if (time == UNIT_NHOUR) {
            num = MAX_NHOUR;
            array = &sp->hour[0][0];
            tail_index = sp->hour_tail;
        } else if (time == UNIT_NDAY) {
            num = MAX_NDAY;
            array = &sp->day[0][0];
            tail_index = sp->day_tail;
        } else if (time == UNIT_NWEEK) {
            num = MAX_NWEEK;
            array = &sp->week[0][0];
            tail_index = sp->week_tail;
        } else if (time == UNIT_NMONTH) {
            num = MAX_NMONTH;
            array = &sp->month[0][0];
            tail_index = sp->month_tail;
        } else {
            num = MAX_NHOUR;
            array = &sp->hour[0][0];
            tail_index = sp->hour_tail;
        }
        fprintf(fo, "last_thp_timestamp=%lu\nlast_thp_rx=%ld\nlast_thp_tx=%lu\n", uptime_interval, array[sp->hour_tail * MAX_COUNTER], array[sp->hour_tail * MAX_COUNTER + 1]);
        break;
	}
}

static void
get_link_status(FILE *fo)
{
    char swap_proto[TMP_LEN]={0}, link_state[TMP_LEN]={0}, not_decode_ssid[TMP_LEN]={0}, wisp_plug[TMP_LEN]={0};
    char usb_plug[TMP_LEN]={0}, ether_plug[TMP_LEN]={0};
    int ether_connect = 0, wisp_connect = 0, usb_connect = 0;
    char *proto, *ip_addr;
    char ssid[TMP_LEN]={0};
    unsigned long n = TMP_LEN;

    proto = nvram_get("wan0_proto");

    ezplib_get_attr_val("plug_rule", 0, "ethernet",
            ether_plug, sizeof(ether_plug), EZPLIB_USE_CLI);
    ezplib_get_attr_val("plug_rule", 0, "usb",
            usb_plug, sizeof(usb_plug), EZPLIB_USE_CLI);
    ezplib_get_attr_val("wan_status_rule", 0, "state",
            link_state, sizeof(link_state), EZPLIB_USE_CLI);
    
    ip_addr = nvram_get("wan0_ipaddr");

    if(!strcmp(proto, "dhcp")) {
        strcpy(swap_proto, "Wired Internet (DHCP)");
        ether_connect = 1;
    } else if(!strcmp(proto, "pppoe")) {
        strcpy(swap_proto, "Wired Internet (PPPoE)");
        ether_connect = 1;
    } else if(!strcmp(proto, "static")) {
        strcpy(swap_proto, "Wired Internet (Static IP)");
        ether_connect = 1;
    } else if(!strcmp(proto, "wisp")) {
        ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid",
            not_decode_ssid, sizeof(not_decode_ssid), EZPLIB_USE_CLI);
        /* we decode ssid using base64 */
        base64_decode((unsigned char *)not_decode_ssid, strlen(not_decode_ssid), (unsigned char *)ssid, &n);
        ezplib_get_attr_val("plug_rule", 0, "wisp",
            wisp_plug, sizeof(wisp_plug), EZPLIB_USE_CLI);
        if(!strcmp(wisp_plug, "1")) {
            strcpy(link_state, "-1");
        }
        fprintf(fo, "ssid=%s\n", ssid);
        strcpy(swap_proto, "Wireless Internet");
        wisp_connect = 1;
    } else if(!strcmp(proto, "iphone")) {
        strcpy(swap_proto, "USB Internet (Apple iPhone)");
        usb_connect = 1;
    } else if(!strcmp(proto, "htc")) {
        strcpy(swap_proto, "USB Internet (Android phone)");
        usb_connect = 1;
    } else if(!strcmp(proto, "wwan")) {
        strcpy(swap_proto, "USB Internet (3G/4G Mobile Internet)");
        usb_connect = 1;
    }
    //fprintf(fo, "connection_type=%s\n", swap_proto);
    fprintf(fo, "link_state=%s\n", link_state);
    fprintf(fo, "ip_addr=%s\n", ip_addr);

    fprintf(fo, "ether_plug=%s\n", ether_plug);
    fprintf(fo, "wisp_plug=0\n");
    fprintf(fo, "usb_plug=%s\n", usb_plug);
    fprintf(fo, "ether_connect=%d\n", ether_connect);
    fprintf(fo, "wisp_connect=%d\n", wisp_connect);
    fprintf(fo, "usb_connect=%d\n", usb_connect);
    fprintf(fo, "connection_type=%s\n", swap_proto);
}
static void
get_qcloud_status(FILE *fo)
{
    char qid_status[TMP_LEN] = {0};
    ezplib_get_attr_val("myqnapcloud_id_rule", 0, "status", qid_status, sizeof(qid_status), EZPLIB_USE_CLI);
    fprintf(fo, "qcloud_status=%s\n", qid_status);

}

static void
get_cpu_idle(FILE *fo)
{
    static int cpu_pre = -1;
    FILE *f;
    char buf[LONG_BUF_LEN];

    if (!(f = popen("top -n 1 | awk '/^CPU/ {print $8}' | sed 's/%/\\ /g'", "r"))) {
        return;
    }
    fgets(buf, sizeof(buf), f);
    int cpu_post = 100 - atoi(buf);
    int cpu = (cpu_pre + cpu_post) / 2 ;
    cpu_pre = cpu_post;    

    fprintf(fo, "CPU_idle=%d\n", cpu);
    pclose(f);
}

static void
get_battery_status(FILE *fo)
{
    FILE *f;
    char buf[LONG_BUF_LEN];
    int percentage = 0;

    if (!(f = popen("ezp-i2c gauge percentage", "r"))) {
        return;
    }
    fgets(buf, sizeof(buf), f);
    sscanf(buf, "Battery: %d %%", &percentage);
    fprintf(fo, "Battery=%d\n", percentage);
    pclose(f);
}

static void
get_vpn_status(FILE *fo)
{
    char get_vpn_enable[TMP_LEN] = {0}, vpn_server[TMP_LEN] = {0}, vpn_name[TMP_LEN] = {0};
    char *vpn_ip_addr, *vpn_status;

    int vpn_enable = 0;
    int vpn_num = 6;
    int i;
    int vpn_enable_state[TMP_LEN]={0};

    ezplib_get_attr_val("wan_pptp_l2tp_global_rule", 0, "enable", get_vpn_enable, sizeof(get_vpn_enable), EZPLIB_USE_CLI);
    vpn_enable = atoi(get_vpn_enable);
    for(i = 0; i < vpn_num; i++){
        ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "enable", get_vpn_enable, sizeof(get_vpn_enable), EZPLIB_USE_CLI);
        vpn_enable_state[i] = atoi(get_vpn_enable);
        if(vpn_enable_state[i] == 1) {
            // vpn_enable = 1;
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "host", vpn_server, sizeof(vpn_server), EZPLIB_USE_CLI);
            ezplib_get_attr_val("wan_pptp_l2tp_rule", i, "username", vpn_name, sizeof(vpn_name), EZPLIB_USE_CLI);
        }
    }
    
    vpn_ip_addr = nvram_safe_get("vpn0_ipaddr");
    vpn_status = nvram_safe_get("wan_pptp_l2tp_status");
    if(strcmp(vpn_status, "Connected")) {
        vpn_ip_addr = vpn_status;
    }

    fprintf(fo, "vpn_enable=%d\n", vpn_enable);
    fprintf(fo, "vpn_server=%s\n", vpn_server);
    fprintf(fo, "vpn_ip_addr=%s\n", vpn_ip_addr);
    fprintf(fo, "vpn_user_name=%s\n", vpn_name);
}

static void
get_charge_status(FILE *fo)
{
    FILE *f;
    char buf[LONG_BUF_LEN];

    if (!(f = popen("ezp-i2c charger", "r"))) {
        return;
    }
    fgets(buf, sizeof(buf), f);

    if(strncmp(buf, "No charger",10) == 0)
        fprintf(fo, "charge_status=Not Charging\n");
    else
        fprintf(fo, "charge_status=Charging\n");
    pclose(f);
}

static void
get_device_info(FILE *fo)
{
    char *model, *fw_version;
    char time[2048], up_time[TMP_LEN];

    model = nvram_get("model");
    fw_version = nvram_get("fw_version");
    fprintf(fo, "model=%s\n", model);
    fprintf(fo, "fw_version=%s\n", fw_version);

    if (!show_localtime(time, 2048)) {
        fprintf(fo, "cur_time=%s\n", time);
    } else {
        fprintf(fo, "cur_time=%s\n", "Unknown");
    }

    if (!show_uptime(up_time, TMP_LEN)) {
        fprintf(fo, "up_time=%s\n", up_time);
    } else {
        fprintf(fo, "up_time=%s\n", "Unknown");
    }
}


static void
get_storage_status(FILE *fo, char *device_name)
{
    FILE *f;
    uint64_t free_size, size, use_size;
    char buf[TMP_LEN], tmp2[TMP_LEN];
    char storage_size[SHORT_BUF_LEN],storage_usize[SHORT_BUF_LEN],
         storage_upercentage[SHORT_BUF_LEN],storage_fsize[SHORT_BUF_LEN];

    char path[SHORT_BUF_LEN],f_size[SHORT_BUF_LEN],dir[SHORT_BUF_LEN]={0};
    char storage_state[SHORT_BUF_LEN];
    int device_id = 0; //default USB=0

    if(strcmp("SD",device_name)==0)
        device_id=1;
    else if(strcmp("USB",device_name)==0)
        device_id=0;
    else if(strcmp("Qsync",device_name)==0)
        device_id=2;

    get_storage_space_info(device_name, 4, tmp2, sizeof(tmp2), 0);
    fprintf(fo, "%s=%s\n", device_name, tmp2);

    /* Storage infomation */
    ezplib_get_attr_val("storage_state_rule", device_id, "mount_state",
                        storage_state, sizeof(storage_state), EZPLIB_USE_CLI);
    if (strcmp(storage_state, "mounted") == 0) {
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf),
                "/sbin/getsysinfo disk_info %s", device_name);
        if ((f = popen(buf, "r"))) {
            fscanf(f, "%s %s %s %s %s %s", path, storage_size, storage_usize,
                   f_size, storage_upercentage, dir);
            if ((strlen(dir)) > 0) {
                size = atoi(storage_size);
                use_size = atoi(storage_usize);
            } else {
                size = atoi(path);
                use_size = atoi(storage_size);
                strcpy(storage_upercentage, f_size);
            }
            adj_size((float)size, storage_size, sizeof(storage_size));
            free_size = size - use_size;
            adj_size((float)free_size, storage_fsize, sizeof(storage_fsize));
            fprintf(fo, "%s_remaining=%s \n", device_name, 
                    storage_fsize);
            use_size*= 100;
            if(size == 0) {
                fprintf(fo, "%s_usage=N/A\n", device_name);
            }else {
                fprintf(fo, "%s_usage=%llu\n", device_name, use_size/size);
            }
            pclose(f);
        }
    } else if (strcmp(storage_state, "unknown") == 0) {
        fprintf(fo, "%s_usage=N/A\n", device_name);
        fprintf(fo, "%s_remaining=&nbsp;\n", device_name);
    } else if(strcmp(storage_state, "format") == 0) {
        fprintf(fo, "%s_usage=N/A\n", device_name);
        fprintf(fo, "%s_remaining=&nbsp;\n", device_name);
    } else if(strcmp(storage_state, "con_err") == 0) {
        fprintf(fo, "%s_usage=N/A\n", device_name);
        fprintf(fo, "%s_remaining=&nbsp;\n", device_name);
    } else {
        fprintf(fo, "%s_usage=N/A\n", device_name);
        fprintf(fo, "%s_remaining=&nbsp;\n", device_name);
    }
}

static void
get_dhcp_lease_num(FILE *fo)
{
    FILE *f;
    int counter = 0;
    char buf[LONG_BUF_LEN];

    if (!(f = fopen("/tmp/dhcp.leases", "r+"))) {
        return;
    }
    while(fgets(buf, sizeof(buf), f)) {
        counter++;
    }
    fprintf(fo, "dhcp_lease_num=%d\n", counter);
    fclose(f);
}

static void
get_meminfo(FILE *fo)
{
    FILE *f;
    char *p;
    char buf[LONG_BUF_LEN];
    unsigned long total = 0, used = 0, free=0;

    if (!(f = fopen("/proc/meminfo", "r+"))) {
        return;
    }
    fgets(buf, sizeof(buf), f); // header
    p = strchr(buf, ':');
    *p = '\0';
    sscanf(p + 1, "%lu", &total);
    fgets(buf, sizeof(buf), f);

    p = strchr(buf, ':');
    *p = '\0';
    sscanf(p + 1, "%lu", &free);
    used=total-free;
    fprintf(fo, "Memory=%.0f\n", ((double)used/(total+5000)*100)/3);
    fprintf(fo, "Memory_free=%d\n", free);
    fclose(f);
}

static void
update_wan_ifname()
{
    int i;
    char buf[SHORT_BUF_LEN];
    char *proto, *ifname;

    for (i = 0; i < nwan; i++) {
        snprintf(buf, SHORT_BUF_LEN, "wan%d_proto", i);
        proto = nvram_safe_get(buf);
        if (!strcmp(proto, "wwan") || !strcmp(proto, "directip") || 
                !strcmp(proto, "htc")) {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_ifname", i);
        } else {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_device", i);
        }
        ifname = nvram_safe_get(buf);
        if (!strcmp(wan_ifname[i], ifname)) {
            /* the same ifname */
            continue;
        }

        strcpy(wan_ifname[i], ifname);
        memset(cnt_now[i], 0, MAX_COUNTER);
    }
}

static void
init()
{
    int i;

    nwan = atoi(nvram_safe_get("wan_num"));    
    
    cnt_rx_max = (unsigned long *)calloc(nwan, sizeof(unsigned long));
    cnt_tx_max = (unsigned long *)calloc(nwan, sizeof(unsigned long));
    cnt_now = (unsigned long **)calloc(nwan, sizeof(unsigned long *));
    wan_ifname = (char **)calloc(nwan, sizeof(char *));
    for (i = 0; i < nwan; i++) {
        cnt_now[i] = (unsigned long *)calloc(MAX_COUNTER, sizeof(unsigned long));
        wan_ifname[i] = calloc(MIDDLE_BUF_LEN, sizeof(char));
    }

    /* TODO: For the complicated situation, multiple WAN + 3.5G, we cannot
     * simply use br0 to monitor.  Therefore, we have to considerate the WAN
     * changing issue, and periodically check the WAN interface.
     */
    update_wan_ifname();
}

static void 
calc_ct_generic(long uptime, mrtg_t *mrtg, int nth_proto, int nth_intv,
        unsigned long *counter, long *utime, unsigned long *last, int *tail,
        unsigned long *array, int round)
{
	unsigned long diff;
	uint32_t c;
	uint32_t sc;
    uint32_t last0, last1;
    int i;

    /* Duration. */
    if (uptime - *utime == 0) {
        return;
    }

    /* Update utime. */
    *utime = uptime;
    /* Update tail. */
    *tail = (*tail + 1) % round;

    /* Keep the values because they will be overwritten later. */
    last0 = last[0];
    last1 = last[1];

    /**
     * Calculate the data:
     * counter[0]: opened session by now
     * last[0]: opened session on the past sampling point
     * counter[1]: closed session by now
     * last[1]: closed session on the past sampling point
     *
     * array[0]: the number of sessions occuring in the nth_intv
     *           = counter[0] - last[0]
     * array[1]: the number of sessions still opened now
     *           = counter[0] - counter[1]
     */ 
    for (i = 0; i < MAX_COUNTER; ++i) {
        /* always use counter[0] */
        c = counter[0];
        sc = (i) ? counter[1] : last[i];
        diff = diff_complement(c, sc);
        last[i] = counter[i];

        *(array + (*tail * MAX_COUNTER) + i) = diff;
    }
}

static void 
calc_generic(long uptime, unsigned long *counter, long *utime, 
        unsigned long *last, int *tail, unsigned long *array, int round)
{
	long tick;
	unsigned long diff;
	uint32_t c;
	uint32_t sc;
    int i;

    /* Duration. */
    tick = uptime - *utime;
    if (!tick)
        return;

    /* Update utime. */
    *utime = uptime;
    /* Update tail. */
    *tail = (*tail + 1) % round;

    for (i = 0; i < MAX_COUNTER; ++i) {
        c = counter[i];
        sc = last[i];
        diff = diff_complement(c, sc);
        last[i] = c;

        *(array + (*tail * MAX_COUNTER) + i) = diff / tick;

        _dprintf("%s:[%d][%d][%lu bytes][diff %lu][tick %lu]"
                "[%lu bytes/sec][sz:%d]\n", __FUNCTION__, *tail, i, 
                counter[i], diff, tick, 
                *(array + (*tail * MAX_COUNTER) + i), round);
    }
}

static void
consist_direction(char *ifname, unsigned long *rx, unsigned long *tx)
{
    char buf[SHORT_BUF_LEN];
    char *val;
    int nwan = atoi(nvram_safe_get("wan_num"));
    int i, revert = 1;

    nwan = (nwan == 0) ? 1 : nwan;

    for (i = 0; i < nwan; i++) {
        snprintf(buf, sizeof(buf), "wan%d_proto", i);
        val = nvram_safe_get(buf);
        if (!strcmp(val, "wwan") || !strcmp(val, "directip") ||
                !strcmp(val, "htc")) {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_ifname", i);
        } else {
            snprintf(buf, SHORT_BUF_LEN, "wan%d_device", i);
        }
        val = nvram_safe_get(buf);
        if (!strcmp(ifname, val)) {
            revert = 0;
        }
    }

    if (revert) {
        double tmp;
        tmp = *rx;
        *rx = *tx;
        *tx = tmp;
    }
}

static void calc(long z, long uptime)
{
	FILE *f, *f0, *f1, *f2;
	char buf[LONG_BUF_LEN];
	char *ifname;
	char *p;
	unsigned long counter[MAX_COUNTER];
	mrtg_t *sp;
    mrtg_t *ctp;
	time_t now;
	time_t mon;
	struct tm *tms;
	int i, n;
	char include[LONG_BUF_LEN];
	char tmp[MIDDLE_BUF_LEN];
    unsigned long ct_counter[MAX_COUNTER];

	now = time(0);
	ezplib_get_attr_val("stats_rule", 0, "ifnames", include, 
            sizeof(include), EZPLIB_USE_CLI);

    /* For traffic */
	if ((f = fopen("/proc/net/dev", "r")) == NULL) 
        return;
    /* For restoring data for widget query. */
	if ((f2 = fopen(widget_system_fn, "w+")) == NULL) 
        return;
    /* For retrieving memory usage. */
    get_meminfo(f2);

    /* For cpu usage. */
    get_cpu_idle(f2);

    /* For dhcp lease num. */
    get_dhcp_lease_num(f2);

#if defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
    /* For throughput graph */
    get_throughput_last(f2, z, uptime);

    /* For retriving USB info. */
    get_usbinfo(f2);

    /* For charge status */
    get_charge_status(f2);

    /* For battery status */
    get_battery_status(f2);

    /* For vpn status */
    get_vpn_status(f2);

    /* For storage USB status */
    get_storage_status(f2, "USB");
    
    /* For storage SD status */
    get_storage_status(f2, "SD");

#if defined(PLATFORM_M2E)
    /* For storage Qsync status */
    get_storage_status(f2, "Qsync");
#endif
#endif

    /* For link status */
    get_link_status(f2);

    /*For qcloud status*/
    get_qcloud_status(f2);

    /* For device information */
    get_device_info(f2);

    /* Check whether the WAN ifname changed. */
    update_wan_ifname();

	fgets(buf, sizeof(buf), f);	// header
	fgets(buf, sizeof(buf), f);	// "
	while (fgets(buf, sizeof(buf), f)) {
		if ((p = strchr(buf, ':')) == NULL) 
            continue;
		*p = 0;

		if ((ifname = strrchr(buf, ' ')) == NULL) 
            ifname = buf;
        else 
            ++ifname;

		// <rx bytes, packets, errors, dropped, fifo errors, frame errors, compressed, multicast><tx ...>
		if (sscanf(p + 1, "%lu%*u%*u%*u%*u%*u%*u%*u%lu", &counter[0],
                    &counter[1]) != 2)
            continue;

        /* Rx/Tx of internal interfaces are converse to download/upload
         * direction. We need to correct it.
         */
        consist_direction(ifname, &counter[0], &counter[1]);

        for (i = 0; i < nwan; i++) {
            if (!strcmp(wan_ifname[i], ifname)) {
                double rx, tx;
                /* If the interface has been changed, update_wan_ifname() will
                 * clean up the values of cnt_now[] as initialization.
                 */
                /* At initial, synchronize cnt_now[] with count[]. */
                cnt_now[i][0] = 
                    ((cnt_now[i][0] == 0) ? counter[0] : cnt_now[i][0]);
                cnt_now[i][1] = 
                    ((cnt_now[i][1] == 0) ? counter[1] : cnt_now[i][1]);
                
                rx = (double)diff_complement(counter[0], cnt_now[i][0]) / 
                                WIDGET_INTERVAL;
                tx = (double)diff_complement(counter[1], cnt_now[i][1]) /
                                WIDGET_INTERVAL;
                cnt_rx_max[i] = (cnt_rx_max[i] > rx) ? cnt_rx_max[i] : rx;
                cnt_tx_max[i] = (cnt_tx_max[i] > tx) ? cnt_tx_max[i] : tx;
                if (cnt_rx_max[i] >= 800000000) cnt_rx_max[i]=0;
                if (cnt_tx_max[i] >= 800000000) cnt_tx_max[i]=0;

                fprintf(f2, 
                        "Rx%d=%.0f\nTx%d=%.0f\nRx_Max%d=%lu\nTx_Max%d=%lu\n", 
                        i, rx, i, tx, i, cnt_rx_max[i], i, cnt_tx_max[i]);
                cnt_now[i][0] = counter[0];
                cnt_now[i][1] = counter[1];
            }
        }
        
        if (z > uptime) {
            /* Not ready to prepare the MRTG data. */
            continue;
        }

		if ((!find_word(include, ifname))) 
            continue;

		sp = thp;
		for (i = thp_count; i > 0; --i) {
			if (strcmp(sp->name, ifname) == 0) 
                break;
			++sp;
		}

		if (i == 0) {
			if (thp_count >= MAX_SPEED_IF) 
                continue;
			
            /* New statistics. */
			_dprintf("%s: add %s as #%d\n", __FUNCTION__, ifname, thp_count);
			sp = &thp[thp_count++];
			memset(sp, 0, sizeof(*sp));
			strcpy(sp->name, ifname);
			sp->sync = 1;
		} else {
			if (sp->hour_utime == uptime) 
                continue;

			if (sp->hour_utime > uptime) 
                sp->sync = 1;
		}

		if (sp->sync) {
            /* Reset. */
			_dprintf("%s: %s sync\n", __FUNCTION__, ifname);

			sp->sync = 0;

            sp->hour_utime = sp->day_utime = sp->week_utime = 
                sp->month_utime = uptime;

			memcpy(sp->hour_last, counter, sizeof(sp->hour_last));
			memcpy(sp->day_last, counter, sizeof(sp->day_last));
			memcpy(sp->week_last, counter, sizeof(sp->week_last));
			memcpy(sp->month_last, counter, sizeof(sp->month_last));

			memset(sp->hour[sp->hour_tail], 0, sizeof(sp->hour[0]));
			memset(sp->day[sp->day_tail], 0, sizeof(sp->day[0]));
			memset(sp->week[sp->week_tail], 0, sizeof(sp->week[0]));
			memset(sp->month[sp->month_tail], 0, sizeof(sp->month[0]));

			memset(counter, 0, sizeof(counter));
		} 

        _dprintf("%s:[%s]uptime=%lu hour_utime=%lu day_utime=%lu " 
                "week_utime=%lu month_utime=%lu \n", __FUNCTION__, 
                sp->name, uptime, sp->hour_utime, sp->day_utime, 
                sp->week_utime, sp->month_utime);
        /* Hour */
        if (uptime - sp->hour_utime + TIMER_DEVIATION >= UNIT_NHOUR) {
            calc_generic(uptime, counter, &sp->hour_utime, sp->hour_last, 
                &sp->hour_tail, &sp->hour[0][0], MAX_NHOUR);
        }
        /* Day */
        if (uptime - sp->day_utime + TIMER_DEVIATION >= UNIT_NDAY) {
            calc_generic(uptime, counter, &sp->day_utime, sp->day_last, 
                &sp->day_tail, &sp->day[0][0], MAX_NDAY);
        }
        /* Week */
        if (uptime - sp->week_utime + TIMER_DEVIATION >= UNIT_NWEEK) {
            calc_generic(uptime, counter, &sp->week_utime, sp->week_last, 
                &sp->week_tail, &sp->week[0][0], MAX_NWEEK);
        }
        /* Month */
        if (uptime - sp->month_utime + TIMER_DEVIATION >= UNIT_NMONTH) {
                calc_generic(uptime, counter, &sp->month_utime, sp->month_last, 
                    &sp->month_tail, &sp->month[0][0], MAX_NMONTH);
        }

        // todo: split, delay
        // EZPACKET: TODO: XXX:
        if (now > Y2K) {
            tms = localtime(&now);
            bump(hist.daily, &hist.dailyp, MAX_NDAILY,
                (tms->tm_year << 16) | ((uint32_t)tms->tm_mon << 8) | 
                tms->tm_mday, counter);
            
            /* XXX:
            n = atoi(nvram_safe_get("stats_offset"));
            if ((n < 1) || (n > 31)) 
             */
            n = 1;
            mon = now + ((1 - n) * (60 * 60 * 24));
            tms = localtime(&mon);
            bump(hist.monthly, &hist.monthlyp, MAX_NMONTHLY,
                (tms->tm_year << 16) | ((uint32_t)tms->tm_mon << 8), counter);
        }
    }
    fclose(f);
    fclose(f2);

    /* For conntrack count */
    for (i = 0; i < MAX_CT_PROTO; i++) {
        snprintf(tmp, MIDDLE_BUF_LEN, "%s%s",
                 proto_count_dir, ct_proto_open_fn[i]);
        if ((f0 = fopen(tmp, "r")) == NULL) {
            return;
        }
        snprintf(tmp, MIDDLE_BUF_LEN, "%s%s",
                proto_count_dir, ct_proto_close_fn[i]);
        if ((f1 = fopen(tmp, "r")) == NULL) {
            return;
        }
        
        ctp = &ct[i];
        if (!ctp->name[0]) {
            ctp->sync = 1;
            strcpy(ctp->name, ct_proto_name[i]);
        }

        memset(ct_counter, 0, sizeof(ct_counter));
        fscanf(f0, "%lu", &ct_counter[0]);
        fscanf(f1, "%lu", &ct_counter[1]);

        if (ctp->sync) {
            ctp->sync = 0;
            ctp->hour_utime = ctp->day_utime = ctp->week_utime =
                ctp->month_utime = uptime; 
            memcpy(ctp->hour_last, ct_counter, sizeof(ctp->hour_last));
            memcpy(ctp->day_last, ct_counter, sizeof(ctp->day_last));
            memcpy(ctp->week_last, ct_counter, sizeof(ctp->week_last));
            memcpy(ctp->month_last, ct_counter, sizeof(ctp->month_last));

            memset(ctp->hour[ctp->hour_tail], 0, sizeof(ctp->hour[0]));
            memset(ctp->day[ctp->day_tail], 0, sizeof(ctp->day[0]));
            memset(ctp->week[ctp->week_tail], 0, sizeof(ctp->week[0]));
            memset(ctp->month[ctp->month_tail], 0, sizeof(ctp->month[0]));

            memset(ct_counter, 0, sizeof(ct_counter));
        }

        /* Max seen sessions in the lifetime */
        n = ct_counter[0] - ct_counter[1];
        if (n > ctp->lifetime_max) {
            ctp->lifetime_max = n;
        }

        /* Hour */
        if (uptime - ctp->hour_utime + TIMER_DEVIATION >= UNIT_NHOUR) {
            calc_ct_generic(uptime, &ct[0], i, TWOHOURLY, ct_counter,
                    &ctp->hour_utime,
                    ctp->hour_last, &ctp->hour_tail, &ctp->hour[0][0],
                    MAX_NHOUR);
        }
        /* Day */
        if (uptime - ctp->day_utime + TIMER_DEVIATION >= UNIT_NDAY) {
            calc_ct_generic(uptime, &ct[0], i, DAILY, ct_counter,
                    &ctp->day_utime, ctp->day_last, &ctp->day_tail,
                    &ctp->day[0][0], MAX_NDAY);
        }
        /* Week */
        if (uptime - ctp->week_utime + TIMER_DEVIATION >= UNIT_NWEEK) {
            calc_ct_generic(uptime, &ct[0], i, WEEKLY, ct_counter,
                    &ctp->week_utime, ctp->week_last, &ctp->week_tail,
                    &ctp->week[0][0], MAX_NWEEK);
        }
        /* Month */
        if (uptime - ctp->month_utime + TIMER_DEVIATION >= UNIT_NMONTH) {
            calc_ct_generic(uptime, &ct[0], i, MONTHLY, ct_counter,
                    &ctp->month_utime, ctp->month_last, &ctp->month_tail,
                    &ctp->month[0][0], MAX_NMONTH);
        }

        fclose(f0);
        fclose(f1);
    }

#if 0
        // missing IFs get sync=1, really old ones get removed
        for (i = 0; i < thp_count; ++i) {
            sp = &thp[i];
            if (sp->hour_utime == uptime) 
                continue;
            if (((uptime - sp->utime) > SDAY) || (!find_word(include, sp->ifname)))
            {
                --thp_count;
                memcpy(sp, sp + 1, (thp_count - i) * sizeof(thp[0]));
            } else {
                sp->sync = 1;
            }
        }
#endif

    // todo: total > user
    if (uptime >= save_utime) {
        save(0);
        save_utime = uptime + get_stime();
        _dprintf("%s: uptime = %lu, save_utime = %lu\n", __FUNCTION__, uptime, 
                save_utime);
    }
}

static void sig_handler(int sig)
{
        switch (sig) {
        case SIGTERM:
        case SIGINT:
            gotterm = 1;
            break;
        case SIGHUP:
            gothup = 1;
            break;
        case SIGUSR1:
            gotuser = 1;
            break;
	case SIGUSR2:
		gotuser = 2;
		break;
	}
}

int
gen_system_info(int new)
{
	struct sysinfo si;
	struct sigaction sa;
	struct stat st;
	long z;

	clear_hist();
	unlink(load_fn);

    /* Create a directory "/var/stats" if necessary. */
    if (stat(dir_fn, &st)) {
        /* non-exist */ 
            mkdir(dir_fn, 0777);
    } else { 
        /* exist */ 
        if (!S_ISDIR(st.st_mode)) {
            unlink(dir_fn);
            mkdir(dir_fn, 0777);
        }
    }

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

    init();

	sysinfo(&si);
	load(si.uptime, new);

	sysinfo(&si);
	start_utime = z = si.uptime;
	while (1) {
		while (si.uptime < z) {
			sleep(WIDGET_INTERVAL);
			if (gothup) {
				if (unlink(load_fn) == 0) 
                    load_new();
				else save(0);
				gothup = 0;
			}
			if (gotterm) {
				save(1);
                unlink(pid_fn);
				exit(0);
			}
			if (gotuser == 1) {
                char *type = nvram_safe_get("stats_type");
                if (!strcmp(type, "thp")) {
                    char *ifname = nvram_safe_get("stats_ifname");
				    save_mrtgjs(z + si.uptime, mrtgjs_fn, thp_count, thp, THP,
                            ifname);
                } else if(!strcmp(type, "ct")) {
                    char *proto = nvram_safe_get("stats_proto");
				    save_mrtgjs(z + si.uptime, mrtgjs_fn, MAX_CT_PROTO, ct, CT,
                            proto);
                } else {
                    return 1;
                }
				gotuser = 0;
			}
			else if (gotuser == 2) {
				save_histjs();
				gotuser = 0;
			}
			sysinfo(&si);
		    calc(z, si.uptime);
		}
		z += INTERVAL;
	}

	return 0;
}


int
gen_sig_strength_info()
{
    FILE *f1;
    char tmp[MIDDLE_BUF_LEN], cmd[MIDDLE_BUF_LEN];
    char manufacturer[SHORT_BUF_LEN], model[SHORT_BUF_LEN],
         imei[SHORT_BUF_LEN], network[SHORT_BUF_LEN];
    int ctrltty, datatty, numtty;
    int k;
#ifdef EZPacket
    int fd, i, nwan, f_wwan;
    struct ifconf ifc;
    struct ifreq ifbuf[16], *ifp;
#endif
    nwan = atoi(nvram_safe_get("wan_num"));

    while (1) {
        /* Iterate WAN. */
        for (k = 0, numtty = 0; k < nwan; k++) {
            char buf[SHORT_BUF_LEN];
            char *proto;
            snprintf(buf, sizeof(buf), "wan%d_proto", k);
            proto = nvram_safe_get(buf);
            if (strcmp(proto, "wwan") && strcmp(proto, "directip")) {
                continue;
            }

            if ((f1 = fopen(widget_sig_strength_tmp_fn, "w+")) == NULL) {
                goto cont2;
            }

#ifdef EZPacket
            /* flag to indicate connection of wwan is up. */
            f_wwan = 0;

            if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                goto cont2;
            }
            memset(ifbuf, 0, sizeof(ifbuf));
            ifc.ifc_len = sizeof(ifbuf);
            ifc.ifc_buf = (caddr_t)ifbuf;
            /* gets interfaces list */
            if (ioctl(fd, SIOCGIFCONF, (char*)&ifc) == -1 ||
                    ifc.ifc_len < sizeof(struct ifreq)) {
                goto cont1;
            }

            /* Travel through all ifname */
            nwan = atoi(nvram_safe_get("wan_num"));
            for (i = 0; i < nwan; i++) {
                char buf[SHORT_BUF_LEN];
                snprintf(buf, sizeof(buf), "wan%d_proto", i);
                if (strcmp(nvram_safe_get(buf), "wwan")) {
                    continue;
                }
                /* Check whether interface ppp exists. */
                for (ifp = ifbuf; ifp < (char *)ifbuf + ifc.ifc_len; ifp++) {
                    snprintf(buf, sizeof(buf), "ppp%d", i);
                    if (!strcmp(buf, ifp->ifr_name)) {
                        f_wwan = 1;
                        break;
                    }        
                }
            }

            if (!f_wwan) {
                /* Not match */
                goto cont1;
            }
#endif

            manufacturer[0] = '\0';
            model[0] = '\0';
            imei[0] = '\0';
            network[0] = '\0';

            ezplib_get_attr_val("wan_wwan_probe_rule", k, "ctrltty", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            ctrltty = atoi(tmp);

            ezplib_get_attr_val("wan_wwan_probe_rule", k, "datatty", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            datatty = atoi(tmp);

            if (ctrltty == datatty) {
                /* We cannot probe anymore if the tty is used for pppd. */
                goto cont1;
            }

            ezplib_get_attr_val("wan_wwan_probe_rule", k, "devname", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            snprintf(cmd, sizeof(cmd), "/dev/%s%d", tmp, numtty + ctrltty);
            if (!f_exists(cmd)) {
                fprintf(f1, "wwan_on=0\n");
                goto cont1;
            }

            fprintf(f1, "wwan_on=1\n");
#if 0

            /* Info */
            ezplib_get_attr_val("wan_wwan_probe_rule", k, "devname", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            snprintf(cmd, sizeof(cmd), "ezpcom -d /dev/%s%d info", tmp, 
                    numtty + ctrltty);
            if ((f2 = popen(cmd, "r")) == NULL) {
                goto cont1;
            }

            while (fgets(tmp, sizeof(tmp), f2)) {

                if (manufacturer[0] == '\0' && !strncmp(tmp, "Manufacturer", 12)) {
                    sscanf(tmp, "%*[^:]: %s", manufacturer);
                }
                if (model[0] == '\0' && !strncmp(tmp, "Model", 5)) {
                    sscanf(tmp, "%*[^:]: %s", model);

                }
                if (imei[0] == '\0' && !strncmp(tmp, "IMEI", 4)) {
                    sscanf(tmp, "%*[^:]: %s", imei);
                }
            }
#ifdef EZPacket
            fprintf(f1, "Manufacturer=%s\n", manufacturer);
            fprintf(f1, "Model=%s\n", model);
#endif
            fprintf(f1, "IMEI=%s\n", imei);

            pclose(f2);

            /* Reg */
            ezplib_get_attr_val("wan_wwan_probe_rule", k, "devname", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            snprintf(cmd, sizeof(cmd), "ezpcom -d /dev/%s%d reg", tmp, 
                    numtty + ctrltty);
            if ((f2 = popen(cmd, "r")) == NULL) {
                goto cont1;
            }
            fgets(tmp, sizeof(tmp), f2);
            fgets(tmp, sizeof(tmp), f2);
            sscanf(tmp, "%*[^\"]\"%[^\"]\",%*s", network);

            fprintf(f1, "ISP=%s\n", network);

            pclose(f2);

            /* signal */

            ezplib_get_attr_val("wan_wwan_probe_rule", k, "devname", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            snprintf(cmd, sizeof(cmd), "ezpcom -d /dev/%s%d sig", tmp, 
                    numtty + ctrltty);
            if ((f2 = popen(cmd, "r")) == NULL) {
                goto cont1;
            }
            if (fscanf(f2, "%*[^:]: %d,%*d", &sig) != 1 || sig == 0) {
                /* Restore signal because sometimes we cannot fetch the right
                 * value.
                 */
                sig = old_sig;
            } else {
                /* Update signal */
                old_sig = sig;
            }
            /* sig 1: 1 ~ 6
             * sig 2: 7 ~ 12
             * sig 3: 13 ~ 18
             * sig 4: 19 ~ 24
             * sig 5: 25 ~ 31
             */
            sig --;
            sig /= 6;
            if (sig < 5) {
                sig ++;
            }

            pclose(f2);

            fprintf(f1, "Signal=%d\n", sig);
#endif /*#if 0*/
cont1:
#ifdef EZPacket
            close(fd);
#endif
cont2:
            if (f1) {
                fclose(f1);
                snprintf(tmp, sizeof(tmp), "mv %s %s",
                        widget_sig_strength_tmp_fn, widget_sig_strength_fn);
                system(tmp);
            }

            ezplib_get_attr_val("wan_wwan_probe_rule", k, "numtty", tmp,
                    sizeof(tmp), EZPLIB_USE_CLI);
            numtty += atoi(tmp);
        } /* WAN iteration. */
        sleep(30);
    }
    return 0;
}


int
main(int argc, char *argv[])
{
    int new = 0;
    FILE *f;
    pid_t parent, child[2] = {0};
    if (argc > 1) {
        if (strcmp(argv[1], "--new") == 0) {
            new = 1;
            _dprintf("new=1\n");
        }
    }
    inital_prod();

    parent = getpid();
    if ((child[0] = fork()) == 0) {
        gen_system_info(new);
    }


    if (strstr(WWAN_MODEL, ezp_prod_cat)) {
        if ((child[1] = fork()) == 0) {
            gen_sig_strength_info();
        } 
    }

    if (child[0] != 0 && child[1] != 0) {
        /* Parent: write a PID file. */
        if ((f = fopen(pid_fn, "w")) == NULL) {
            return 1;
        }
        fprintf(f, "%d\n", parent);
        fclose(f);

        return 0;
    }
    return 0;
}
