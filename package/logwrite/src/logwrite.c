#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <getopt.h>
#include <errno.h>
#include <ctype.h>

/*"GENA" */
#define DEFAULT_KEY_ID 0x414e4547 
#define DEFAULT_KEY_CAT "UNKNOWN"

struct shbuf_ds {
    int size;           // size of data written
    int head;           // start of message list
    int tail;           // end of message list
    char data[1];       // data/messages
} ;          // shared memory pointer

static unsigned long KEY_ID = DEFAULT_KEY_ID ;  
char *KEY_CAT = DEFAULT_KEY_CAT ;  
char *KEY_STR = DEFAULT_KEY_CAT ;  

static struct sembuf SMwup[1] = { {1, -1, IPC_NOWAIT} };    // set SMwup
static struct sembuf SMwdn[3] = { {0, 0}, {1, 0}, {1, +1} };    // set SMwdn
//static struct shbuf_ds *logbuf = NULL ;

#ifndef DEFAULT_SYSLOG_BUFFER_SIZE
#define DEFAULT_SYSLOG_BUFFER_SIZE 16
#endif
#define MSGBUF_SIZE 512

//static int shmid = -1;  // ipc shared memory id
//static int s_semid = -1;    // ipc semaphore id
static int shm_size = ((DEFAULT_SYSLOG_BUFFER_SIZE)*1024);   // default shm size

typedef enum{
    EZP_SYSTEM_DBG = 0,
    EZP_XXX=1,
    USB_ID,
    USB_NAME,
    PPP_READY,
    PPP_ID_ERROR,
    PPP_PW_ERROR,
    PPP_TIMEOUT,
    PIN_ERROR,
    WISP_ID,
    FILTER_BRACLET, /* Filter for [  ]: message*/
    LOGWRITE_FILTER_NUM
} LOGWRITE_FILTER;

struct __logwrite_msg {
    unsigned long keyid;
    char *keycat;
    char *keystr;
    LOGWRITE_FILTER filter;
    /* share mamory and semiphare wrt to each filter */
    int shmid;
    int semid;
    /* share memory log buffer */
    struct shbuf_ds *logbuf;
    /* filter function */
    void (*func)(char *msg, char *keycat);
} *logwrite_msg_cat = NULL;
int logwrite_msg_cat_num = 0;

static void ipcsyslog_cleanup(int _shmid, int _semid)
{
    if (_shmid != -1) {
	shmctl(_shmid, IPC_RMID, NULL);
    }
    if (_semid != -1) {
	semctl(_semid, 0, IPC_RMID, 0);
    }
}

static void ipcsyslog_init(unsigned long shmkey, int shmsz, int logidx)
{
    int newbuf = 0 ;

    if ((logwrite_msg_cat[logidx].shmid = shmget(shmkey, 0, 0)) == -1) {
        /* the log buffer seems not present, try create one */
        if ((logwrite_msg_cat[logidx].shmid = shmget(shmkey, shmsz, IPC_CREAT | 1023)) == -1) {
            perror("shmget");
            exit(-1) ;
        }

        newbuf = 1 ;
    }

    if ((logwrite_msg_cat[logidx].logbuf = shmat(logwrite_msg_cat[logidx].shmid, NULL, 0)) == NULL) {
        perror("shmat");
        exit(-1) ;
    }

    if(newbuf) {
        logwrite_msg_cat[logidx].logbuf->size = shm_size - sizeof(struct __logwrite_msg);
        logwrite_msg_cat[logidx].logbuf->head = logwrite_msg_cat[logidx].logbuf->tail = 0;
    }

    // we'll trust the OS to set initial semval to 0 (let's hope)
    if ((logwrite_msg_cat[logidx].semid = semget(shmkey, 2, IPC_CREAT | IPC_EXCL | 1023)) == -1) {
        if (errno == EEXIST) {
            if ((logwrite_msg_cat[logidx].semid = semget(shmkey, 2, 0)) == -1) {
                perror("semget");
                exit(-1);
            }
        } else {
            perror("semget");
            exit(-1) ;
        }
    }
}

/*
 * sem_up - up()'s a semaphore.
 */
static inline void sem_up(int semid)
{
    if (semop(semid, SMwup, 1) == -1) {
	perror("semop[SMwup]");
	exit(-1) ;
    }
}

/*
 * sem_down - down()'s a semaphore
 */
static inline void sem_down(int semid)
{
    if (semop(semid, SMwdn, 3) == -1) {
	perror("semop[SMwdn]");
	exit(-1) ;
    }
}


/*
 *
 */
unsigned int str2u32(unsigned char *u32str)
{
    unsigned int ret = 0 ;

    if(u32str[0] == '0') {
	if(toupper(u32str[1]) == 'X') /* Hex-decimal */

	    sscanf(u32str, "%x", &ret) ;
	else /* Octal */
	    sscanf(u32str, "%o", &ret) ;
    }
    else if(isdigit(u32str[0])) {/* Decimal */
	ret = atoi(u32str) ;
    }
    else { /* string, assumed to be little-endian */
	int i ;

	ret = 0 ;
	for(i = 0; i < 4 && u32str[i]; i++)
	    ret |= ((unsigned int) u32str[i]) << (8*i) ;
    }

    return ret ;
}

/* write EZP message to buffer */
void filt_f_message(char *msg, char *keycat)
{
    char *p1, *p2, *p3;
    char dst[MSGBUF_SIZE];
    char *cur = &dst[0];
    int n;
    int check_EZP = 0;
    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(msg, '>');
    p2 = strchr(msg, '<');
    p3 = strstr(msg, "EZP_");
    if (!p3) {
        p3 = strstr(msg, "(");
        check_EZP = 1;
    }
    if (!p3) {
        p3 = strstr(msg, "]: ");
        if (!p3) {
            p3 = strstr(msg, ": ");
        }
        if (p3) {
            p3 = strrchr(p3, ' ');
            check_EZP = 2;
        }
    }
    if (!p3) {
        p3 = msg + strlen("<13>Jul 27 11:27:46");
        check_EZP = 3;
    }

    if (!p1 || !p3) 
        return;

    /* level and category */
    snprintf(cur, strlen(keycat) + 4, "<%s>\t", keycat);
    cur = cur + (strlen(keycat) + 3);

    /* time */
    p1 += 1;
    strncpy(cur, p1, p3 - p1);
    cur = cur + (p3 - p1) - 1;

    switch (check_EZP) {
        case 0:
            /* case EZP_XXX */
            p3 += strlen("EZP_XXX");
            break;
        case 1:
        case 2:
        default:
            /* case (none) */
            /* case [1234]: */
            p3 = strchr(p3, ':');
            break;
    }

    /* message body */
    n = snprintf(cur, MSGBUF_SIZE - (cur - &dst[0]), "%s", p3);
    strncpy(msg, dst, MSGBUF_SIZE);
}

/* Convert:
 * <30>Dec  8 11:12:58 NBG4615 dnsmasq[1427]: reading /etc/resolv.conf 
 * <DHCP>  Dec  8 11:12:58 reading /etc/resolv.conf*/
void filt_b_message(char *msg, char *keycat)
{
    char *p1, *p2, *p3;
    char dst[MSGBUF_SIZE];
    char *cur = &dst[0], *pcur;
    int n, offset3;
    int check_EZP = 0;
    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */

    p1 = strchr(msg, '>') + 1;
    p2 = strchr(msg, '<');
    p3 = strstr(msg, "]: ");
    offset3 = strlen("]: ");
    if(!p3) {
        /* Sometimes we got string like "pppd: unrecognized option 'require-mppe-128'" */
        p3= strstr(msg, ": ");
        offset3 = strlen(": ");
    }
    p3 += offset3;

    /* level and category */
    n = snprintf(cur, MSGBUF_SIZE, "<%s>\t", keycat);
    cur = cur + n;

    /* time */
    memcpy(cur, p1, sizeof("Dec  8 11:12:58 ") - 1);
    cur = cur + (sizeof("Dec  8 11:12:58 ") - 1);

    /* message body */
    n = snprintf(cur, MSGBUF_SIZE - (cur - &dst[0]), "%s", p3);
    strncpy(msg, dst, MSGBUF_SIZE);
    printf("msg:%s\n", msg);
}

/* write USB idVendor idProduct message to buffer */
void filt_U_message(char *msg, char *keycat)
{
    /* char *cur = msg; */
    char dst[MSGBUF_SIZE];
    char *cur = &dst[0];
    char *p1, *p3;
    int n;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(msg, '>');
    p3 = strstr(msg, "New USB device found,");
    if (!p1 || !p3) return;
    p1 += 1;
    p3 += strlen("New USB device found,");

    /* USB devices are categoried as SYS messages */
    n = snprintf(cur, MSGBUF_SIZE, "<SYS>\t");
    cur = cur + n;

    /* time */
    memcpy(cur, p1, sizeof("Dec  8 11:12:58 ") - 1);
    cur = cur + (sizeof("Dec  8 11:12:58 ") - 1);

    /* Message Body */
    snprintf(cur , (&dst[0]) + MSGBUF_SIZE - cur, " Find USB device: %s", p3);
    strncpy(msg, dst, MSGBUF_SIZE);
    printf("msg:%s\n", msg);
}
/* write USB device message to buffer */
void filt_u_message(char *msg, char *keycat)
{
    /* char *cur = msg; */
    char dst[MSGBUF_SIZE];
    char *cur = &dst[0];
    char *p1, *p3;
    int n;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(msg, '>');
    p3 = strstr(msg, "Product:");
    if (!p1 || !p3) return;
    p1 += 1;
    p3 += strlen("Product:");

    /* USB devices are categoried as SYS messages */
    n = snprintf(cur, MSGBUF_SIZE, "<SYS>\t");
    cur = cur + n;

    /* time */
    memcpy(cur, p1, sizeof("Dec  8 11:12:58 ") - 1);
    cur = cur + (sizeof("Dec  8 11:12:58 ") - 1);

    /* Message Body */
    snprintf(cur , (&dst[0]) + MSGBUF_SIZE - cur, " Find USB device: %s", p3);
    strncpy(msg, dst, MSGBUF_SIZE);
    printf("msg:%s\n", msg);
}

/* write PPPoE access success message to buffer */
void filt_r_message(char *msg, char *keycat)
{
    char *cur = msg;
    char *p1, *p2, *p3;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(cur, '>');

    if (!p1) return;

    p1 += 1;
    p2 = p1 + strlen("Jul 27 14:50:37 ");
    snprintf(cur, p2 - p1, "%s", p1);
    cur += p2 - p1;

    snprintf(cur - 1, msg + MSGBUF_SIZE - cur, " PPPoE access success\n");
}
/* write PPPoE User Name error message to buffer */
void filt_i_message(char *msg, char *keycat)
{
    char *cur = msg;
    char *p1, *p2, *p3;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(cur, '>');

    if (!p1) return;

    p1 += 1;
    p2 = p1 + strlen("Jul 27 14:50:37 ");
    snprintf(cur, p2 - p1, "%s", p1);
    cur += p2 - p1;

    snprintf(cur - 1, msg + MSGBUF_SIZE - cur, " PPPoE User Name Error\n");
}
/* write PPPoE Password error message to buffer */
void filt_p_message(char *msg, char *keycat)
{
    char *cur = msg;
    char *p1, *p2, *p3;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(cur, '>');

    if (!p1) return;

    p1 += 1;
    p2 = p1 + strlen("Jul 27 14:50:37 ");
    snprintf(cur, p2 - p1, "%s", p1);
    cur += p2 - p1;

    snprintf(cur - 1, msg + MSGBUF_SIZE - cur, " PPPoE Password Error\n");
}
/* write PPPoE Timeout error message to buffer */
void filt_t_message(char *msg, char *keycat)
{
    char *cur = msg;
    char *p1, *p2, *p3;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(cur, '>');

    if (!p1) return;

    p1 += 1;
    p2 = p1 + strlen("Jul 27 14:50:37 ");
    snprintf(cur, p2 - p1, "%s", p1);
    cur += p2 - p1;

    snprintf(cur - 1, msg + MSGBUF_SIZE - cur, " PPPoE Timeout Error\n");
}
/* write PIN error message to buffer */
void filt_P_message(char *msg, char *keycat)
{
    char *cur = msg;
    char *p1, *p2, *p3;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(cur, '>');

    if (!p1) return;

    p1 += 1;
    p2 = p1 + strlen("Jul 27 14:50:37 ");
    snprintf(cur, p2 - p1, "%s", p1);
    cur += p2 - p1;

    snprintf(cur - 1, msg + MSGBUF_SIZE - cur, " PIN Error\n");
}

/* write message to buffer */
/* write WISP message to buffer */
void filt_w_message(char *msg, char *keycat)
{
    /* char *cur = msg; */
    char dst[MSGBUF_SIZE], wan_status[MSGBUF_SIZE];
    char *cur = &dst[0];
    char *p1, *p3;
    int n;

    /* <13>Jul 27 11:27:46 (none) root: EZP_USR ACL Database rt: start [OK] */
    p1 = strchr(msg, '>');
    p3 = strstr(msg, "WIFIConnect");
    if (!p1 || !p3) return;
    p1 += 1;
    p3 += strlen("WIFIConnect");

    /* USB devices are categoried as SYS messages */
    n = snprintf(cur, MSGBUF_SIZE, "<SYS>\t");
    cur = cur + n;

    /* time */
    memcpy(cur, p1, sizeof("Dec  8 11:12:58 ") - 1);
    cur = cur + (sizeof("Dec  8 11:12:58 ") - 1);

    /* Message Body */
    snprintf(cur , (&dst[0]) + MSGBUF_SIZE - cur, "WIFI Connect: %s", p3);
    strncpy(msg, dst, MSGBUF_SIZE);
    /* error passwd */ 
    if(strstr(p3, "wrong") || strstr(p3, "encryption")) {
        ezplib_get_attr_val("wan_status_rule", 0, "state", wan_status, sizeof(wan_status), 0);
        if(!strcmp(wan_status, "0") || !strcmp(wan_status, "1"))
            ezplib_replace_attr("plug_rule", 0, "wisp", "1");
    }/* disconnected */
    printf("msg:%s\n", msg);
}
/* write USB device message to buffer */

void circ_message(const char *msg, int logidx)
{
    int l = strlen(msg) + 1;	/* count the whole message w/ '\0' included */
    struct shbuf_ds *logbuf = logwrite_msg_cat[logidx].logbuf;
    int s_semid = logwrite_msg_cat[logidx].semid;
    sem_down(s_semid);

    /*
     * Circular Buffer Algorithm:
     * --------------------------
     *
     * Start-off w/ empty buffer of specific size SHM_SIZ
     * Start filling it up w/ messages. I use '\0' as separator to break up messages.
     * This is also very handy since we can do printf on message.
     *
     * Once the buffer is full we need to get rid of the first message in buffer and
     * insert the new message. (Note: if the message being added is >1 message then
     * we will need to "remove" >1 old message from the buffer). The way this is done
     * is the following:
     * When we reach the end of the buffer we set a mark and start from the beginning.
     * Now what about the beginning and end of the buffer? Well we have the "head"
     * index/pointer which is the starting point for the messages and we have "tail"
     * index/pointer which is the ending point for the messages. When we "display" the
     * messages we start from the beginning and continue until we reach "tail". If we
     * reach end of buffer, then we just start from the beginning (offset 0). "head" and
     * "tail" are actually offsets from the beginning of the buffer.
     *
     * Note: This algorithm uses Linux IPC mechanism w/ shared memory and semaphores to provide
     *       a threasafe way of handling shared memory operations.
     */
    if ((logbuf->tail + l) < logbuf->size) {
	/* before we append the message we need to check the HEAD so that we won't
	   overwrite any of the message that we still need and adjust HEAD to point
	   to the next message! */
	if (logbuf->tail < logbuf->head) {
	    if ((logbuf->tail + l) >= logbuf->head) {
		/* we need to move the HEAD to point to the next message
		 * Theoretically we have enough room to add the whole message to the
		 * buffer, because of the first outer IF statement, so we don't have
		 * to worry about overflows here!
		 */
		int k = logbuf->tail + l - logbuf->head;	/* we need to know how many bytes
							   we are overwriting to make
							   enough room */
		char *c =
		    memchr(logbuf->data + logbuf->head + k, '\0',
			    logbuf->size - (logbuf->head + k));
		if (c != NULL) {	/* do a sanity check just in case! */
		    logbuf->head = c - logbuf->data + 1;	/* we need to convert pointer to
							   offset + skip the '\0' since
							   we need to point to the beginning
							   of the next message */
		    /* Note: HEAD is only used to "retrieve" messages, it's not used
		       when writing messages into our buffer */
		} else {	/* show an error message to know we messed up? */
		    printf("Weird! Can't find the terminator token??? \n");
		    logbuf->head = 0;
		}
	    }
	}

	/* in other cases no overflows have been done yet, so we don't care! */
	/* we should be ok to append the message now */
	strncpy(logbuf->data + logbuf->tail, msg, l);	/* append our message */
	logbuf->tail += l;	/* count full message w/ '\0' terminating char */
    } else {
	/* we need to break up the message and "circle" it around */
	char *c;
	int k = logbuf->tail + l - logbuf->size;	/* count # of bytes we don't fit */

	/* We need to move HEAD! This is always the case since we are going
	 * to "circle" the message.
	 */
	c = memchr(logbuf->data + k, '\0', logbuf->size - k);

	if (c != NULL) {	/* if we don't have '\0'??? weird!!! */
	    /* move head pointer */
	    logbuf->head = c - logbuf->data + 1;

	    /* now write the first part of the message */
	    strncpy(logbuf->data + logbuf->tail, msg, l - k - 1);

	    /* ALWAYS terminate end of buffer w/ '\0' */
	    logbuf->data[logbuf->size - 1] = '\0';

	    /* now write out the rest of the string to the beginning of the buffer */
	    strcpy(logbuf->data, &msg[l - k - 1]);

	    /* we need to place the TAIL at the end of the message */
	    logbuf->tail = k + 1;
	} else {
	    printf
		("Weird! Can't find the terminator token from the beginning??? \n");
	    logbuf->head = logbuf->tail = 0;	/* reset buffer, since it's probably corrupted */
	}

    }
    sem_up(s_semid);
}

static void assemble_msg(char *msgbuf, int bufsz, int argc, char *argv[], int ind)
{
    int n = 0 ;

    while(ind < argc) { 
	int sl = strlen( argv[ind] ) ;
	int wl = bufsz - n - 2 ;

	if(wl == 0)
	    break ;
	else if(wl > sl)
	    wl = sl ;

	strncpy(&(msgbuf[n]), argv[ind], wl) ;
	msgbuf[n + wl] = ' ' ; /* append a white space */
	n += wl + 1;
	ind++ ;
    }

    msgbuf[n - 1] = '\n' ;
    msgbuf[n] = '\0' ;
}

static void usage(char *progname)
{
    printf(
	    "Usage: %s [-k KEYID] [-l bufsize ] [-d] [MESSAGE]\n"
	    "    Log your messages.  If no message is given in the \n"
	    "    argument list, log messages will be read from stdin.\n"
	    , progname) ;
    printf(
	    " -k KEYID  : User-defined KEYID. Default: 0x%08X\n"
	    " -l BUFSIZE: Buffer size. Default: %d K bytes\n"
	    " -d        : clean up log buffer after finished logging\n"
	    , DEFAULT_KEY_ID, DEFAULT_SYSLOG_BUFFER_SIZE) ;
}

void log_write_msg_init(void) {
    logwrite_msg_cat = NULL;
    logwrite_msg_cat_num = 0;
}

void log_write_msg_fini(void) {
    if (logwrite_msg_cat) {
        free(logwrite_msg_cat);
    }
    log_write_msg_init();
}

void log_write_msg_shm_cleanup(void) {
    int i;
    for (i = 0;i < logwrite_msg_cat_num; i++) {
	    ipcsyslog_cleanup(logwrite_msg_cat[i].shmid, logwrite_msg_cat[i].semid) ;
    }
}

void log_write_msg_clear(void) {
    int i;
    for (i = 0;i < logwrite_msg_cat_num; i++) {
        logwrite_msg_cat[i].logbuf->head = 0;
        logwrite_msg_cat[i].logbuf->tail = 1;
        logwrite_msg_cat[i].logbuf->data[0] = '\0';
    }
}

void log_write_msg_add(char *keycat, char *keystr, unsigned long keyid, LOGWRITE_FILTER filter) {
    printf("log for KEY_CAT:%s KEY_STR:%s KEY_ID:%x filter:%d\n", keycat, keystr, keyid, filter);
    logwrite_msg_cat = (struct __logwrite_msg *) realloc(logwrite_msg_cat, sizeof(struct __logwrite_msg) * (logwrite_msg_cat_num + 1));
    logwrite_msg_cat[logwrite_msg_cat_num].keyid = keyid;
    logwrite_msg_cat[logwrite_msg_cat_num].keycat = keycat;
    if (!strcmp(keystr, DEFAULT_KEY_CAT)) {
        logwrite_msg_cat[logwrite_msg_cat_num].keystr = NULL;
    }
    else {
        logwrite_msg_cat[logwrite_msg_cat_num].keystr = keystr;
    }
    logwrite_msg_cat[logwrite_msg_cat_num].filter = filter;
    switch (filter){
        case EZP_XXX:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_f_message;
            break;
        case USB_ID:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_U_message;
            break;
        case USB_NAME:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_u_message;
            break;
        case PPP_READY:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_r_message;
            break;
        case PPP_ID_ERROR:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_i_message;
            break;
        case PPP_PW_ERROR:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_p_message;
            break;
        case PPP_TIMEOUT:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_t_message;
            break;
        case PIN_ERROR:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_P_message;
            break;
	case WISP_ID:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_w_message;
            break;
        case FILTER_BRACLET:
            logwrite_msg_cat[logwrite_msg_cat_num].func = filt_b_message;
            break;
        default:
            logwrite_msg_cat[logwrite_msg_cat_num].func = NULL;
            break;               
    }
    /* Initial share memory and semiphare for each filter category. */
    ipcsyslog_init(logwrite_msg_cat[logwrite_msg_cat_num].keyid, 
            shm_size, logwrite_msg_cat_num);
    logwrite_msg_cat_num ++;
}
/*
 * We have combinations like "str1&+str2&-str3..." or "&-str1&-str2..."
 */
#define KEYSTR_GET_OP(OPCHR) ((OPCHR == '-') ? 0 : 1)
static inline int logxor(int log1, int log2) {
    return (((log1) || (log2)) && (!(log1) || !(log2)));
}
int keystr_match(char *msg, char *key) {
    char *andk = NULL, *actkey, *cur, 
         op = 1 /*op = 0 for "not" 1 for "required" */;
    int ret = 1;
    if (!key || !msg) return 1;
    cur = actkey = strdup(key);
    if (*cur == '&') {
        cur ++;
        op = KEYSTR_GET_OP(*cur);
        cur ++;
    }
    while (cur) {
        if (andk = strchr(cur, '&')) {
            *andk = 0;
            andk ++;
        }
        printf("Process %s with %s strcasestr(msg,cur):%d op:%d res:%d... ", msg,cur, strcasestr(msg,cur),op, strcasestr(msg,cur)^op);
        if (logxor(strcasestr(msg,cur) , op)) {
            ret = 0;
            printf("Not match\n");
            break;
        }
        printf("ok\n");
        cur = andk;
        if (cur) {
            op = KEYSTR_GET_OP(*cur);
            cur ++;
        }
    }
    free(actkey);
    return ret;
}
int logwrite_main(int argc, char **argv)
{
    char msgbuf[MSGBUF_SIZE], msglogbuf[MSGBUF_SIZE];
    char filter = 0 , old_filter = LOGWRITE_FILTER_NUM, trigger = 0;
    int cleanup = 0 , clear = 0;
    int nolog = 0 , i;

    while(1) {
        int c = getopt(argc, argv, "k:l:c:s:fUwuriptPdDhKb") ;
        if(c == -1)
            break ;
        /* With these options, we will add an additional filter into log */
        if (strchr("fUuriptPKb",c)) {
            if (old_filter != LOGWRITE_FILTER_NUM) {
                trigger = 1;
            }
            old_filter = filter;
        }
        switch(c) {
            case 'k':
                if(optarg)
                    KEY_ID = str2u32(optarg) ;
                break ;
            case 'l':
                if(optarg)
                    shm_size = str2u32(optarg) ;
                break ;
            case 'c':
                if(optarg)
                    KEY_CAT = optarg ;
                break ;
            case 's':
                if(optarg)
                    KEY_STR = optarg ;
                break ;
            case 'f':
                filter = EZP_XXX ;
		        break ;
	        case 'w':
                filter = WISP_ID ;
                break ;
            case 'U':
                filter = USB_ID ;
                break ;
            case 'u':
                filter = USB_NAME ;
                break ;
            case 'r':
                filter = PPP_READY ;
                break ;
            case 'i':
                filter = PPP_ID_ERROR ;
                break ;
            case 'p':
                filter = PPP_PW_ERROR ;
                break ;
            case 't':
                filter = PPP_TIMEOUT ;
                break ;
            case 'P':
                filter = PIN_ERROR ;
                break ;
            case 'K':
                filter = EZP_SYSTEM_DBG ;
                break ;
            case 'b':
                filter = FILTER_BRACLET ;
                break ;
            case 'd':
                clear = 1 ;
                nolog = 1 ;
                break ;
            case 'D':
                cleanup = 1 ;
                nolog = 1 ;
                break ;
            case 'h':
                usage("logwrite") ;
                goto end ;
        }
        if (trigger) {
            log_write_msg_add(KEY_CAT, KEY_STR, KEY_ID, old_filter);
            trigger = 0;
            KEY_ID = DEFAULT_KEY_ID;
            KEY_CAT = DEFAULT_KEY_CAT;
            KEY_STR = DEFAULT_KEY_CAT;
        }
    }
    log_write_msg_add(KEY_CAT, KEY_STR, KEY_ID, filter);
    /* create log buffer if it's not present */
    if(optind < argc) { /* treat the remaining args as the message*/
        for (i = 0;i < logwrite_msg_cat_num; i++) {
            assemble_msg(msgbuf, MSGBUF_SIZE, argc, argv, optind) ;
            if (!logwrite_msg_cat[i].keystr || keystr_match(msgbuf, logwrite_msg_cat[i].keystr)) {
                if (logwrite_msg_cat[i].func) {
                    logwrite_msg_cat[i].func(msgbuf, logwrite_msg_cat[i].keycat);
                }
                circ_message(msgbuf, i) ;
            }
        }
    }
    else if(!nolog) {
        while( fgets(msglogbuf, MSGBUF_SIZE, stdin) != NULL ) {
            for (i = 0;i < logwrite_msg_cat_num; i++) {
                strncpy(msgbuf, msglogbuf, sizeof(msglogbuf));
                if (!logwrite_msg_cat[i].keystr || keystr_match(msgbuf, logwrite_msg_cat[i].keystr)) {
                    if (logwrite_msg_cat[i].func) {
                        logwrite_msg_cat[i].func(msgbuf, logwrite_msg_cat[i].keycat);
                    }
                    circ_message(msgbuf, i) ;
                }
            }
        }
    }
#if 0
    ipcsyslog_init(KEY_ID, shm_size) ;

    if(optind < argc) { /* treat the remaining args as the message*/
	assemble_msg(msgbuf, MSGBUF_SIZE, argc, argv, optind) ;
    switch (filter){
        case EZP_XXX:{
                   filt_f_message(msgbuf) ;
                   break;
               }
        case USB_ID:{
                   filt_U_message(msgbuf) ;
                   break;
               }
        case USB_NAME:{
                   filt_u_message(msgbuf) ;
                   break;
               }
        case PPP_READY:{
                   filt_r_message(msgbuf) ;
                   break;
               }
        case PPP_ID_ERROR:{
                   filt_i_message(msgbuf) ;
                   break;
               }
        case PPP_PW_ERROR:{
                   filt_p_message(msgbuf) ;
                   break;
               }
        case PPP_TIMEOUT:{
                   filt_t_message(msgbuf) ;
                   break;
               }
        case PIN_ERROR:{
                   filt_P_message(msgbuf) ;
                   break;
               }
    }
	circ_message(msgbuf) ;
    }
    else if(!nolog) {
	/* read messages from STDIN until EOF is encountered */
	while( fgets(msgbuf, MSGBUF_SIZE, stdin) != NULL ) {
    switch (filter){
        case EZP_XXX:{
                   filt_f_message(msgbuf) ;
                   break;
               }
        case USB_ID:{
                   filt_U_message(msgbuf) ;
                   break;
               }
        case USB_NAME:{
                   filt_u_message(msgbuf) ;
                   break;
               }
        case PPP_READY:{
                   filt_r_message(msgbuf) ;
                   break;
               }
        case PPP_ID_ERROR:{
                   filt_i_message(msgbuf) ;
                   break;
               }
        case PPP_PW_ERROR:{
                   filt_p_message(msgbuf) ;
                   break;
               }
        case PPP_TIMEOUT:{
                   filt_t_message(msgbuf) ;
                   break;
               }
        case PIN_ERROR:{
                   filt_P_message(msgbuf) ;
                   break;
               }
    }
	circ_message(msgbuf) ;
    }
    }
#endif
    if(clear) {
        log_write_msg_clear();
#if 0
        logbuf->head = 0;
        logbuf->tail = 1;
        logbuf->data[0] = '\0';
#endif
    }
    if(cleanup) {
        log_write_msg_shm_cleanup();
#if 0
	shmid = shmget(KEY_ID, 0, 0) ;
	s_semid = semget(KEY_ID, 0, 0) ;

	ipcsyslog_cleanup(shmid, s_semid) ;
#endif
    }


end:
    return 0 ;
}



int main(int argc, char **argv)
{
    int ret ;

    log_write_msg_init();
    ret = logwrite_main(argc, argv) ;
    log_write_msg_fini();

    return ret ;
}

