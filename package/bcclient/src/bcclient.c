/*******************************************************************************************************************************
 *
 *	Copyright (c) 2000  ICP Electronics Inc.  All Rights Reserved.
 *
 *	FILE:
 *		main.c
 *
 *	Abstract:
 *		main program for Quick Setup Server Side.
 *
 *	FUNCTIONS:	TBD.
 *
 *	COMMENTS: 	N/A
 *
 *	HISTORY:
 *		12/16/00	kw lee
 *
 ********************************************************************************************************************************/
#define TYPEDEF_BOOL

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h> 
#include <sys/wait.h> 
#include "router.h"

#include "dbg.h"
#include "finder.h"

/* ezp-nvram */
#include "bcmnvram.h"
#include "ezp-lib.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

const ON_COMMAND_FUNC DO_SERVER_CHANGE_FUNC_TABLE[] = 
{
    {   FINDER_CMD_ACQUIRE_SERVER, OnCmdAcquireServer   },
    {   FINDER_CMD_ACQUIRE_CONFIG, OnCmdAcquireConfig   },
    {   FINDER_CMD_AUTHENTICATE, OnCmdAuthenticate      },
    {   FINDER_CMD_MODIFY_CONFIG, OnCmdModifyConfig     },
    {   FINDER_CMD_ACQUIRE_DETAILS, OnCmdAcquireDetails },
    {   FINDER_CMD_SET_SERVER_NAME, OnCmdSetServerName  },
    {   FINDER_CMD_SET_USER_PASSWORD, OnCmdSetUserPassword  },
    {   FINDER_CMD_SET_NETWORK, OnCmdSetNetwork         },
    {   FINDER_CMD_REBOOT, OnCmdReboot                  },	
    {   0, NULL 	}
};

	
int g_debug_mode = 0;

static unsigned int g_sequence_number = 0;		// current sequence number, normally not in use...
struct in_addr g_last_recv_sock_addr = { 0 };		// last received valid packet address ( PC finder )...
//static char MACAddr[10], LanMACAddr[10];
unsigned char MACAddr[10], LanMACAddr[10];


// Meiji 2007-12-10: since different model may use different way to write system log, add one function to ease the maintain efforts to write system log, 
void write_event_log(const char *log_str, int log_type)
{
    if (log_str && strlen(log_str)) {
#ifndef	USE_NEW_LOG_BY_WILLIAM
        Write_Log((char *) log_str, log_type);
#else
        // Meiji 2007: New TS-409 firmware use new NAS LOG Library by William, add include & link path for this...
        struct naslog_event a_event;
        a_event.type = log_type;
        strcpy(a_event.event_user, "");
        strcpy(a_event.event_comp, "");
        strncpy(a_event.event_desc, log_str, DESC_LENGTH);
        naslog_event_add(&a_event);
#endif		
    }
}

#if _VIOSTOR_
void write_event_log_nvr(const char *log_str, int log_type, const char *user_name)
{
    if (log_str && strlen(log_str)) {
#ifndef	USE_NEW_LOG_BY_WILLIAM
        Write_Log((char *) log_str, log_type);
#else		
        struct naslog_event a_event;
		
        a_event.type = log_type;
        strncpy(a_event.event_user, user_name, USER_LENGTH);
        strncpy(a_event.event_ip, inet_ntoa(g_last_recv_sock_addr), IP_LENGTH);
        strncpy(a_event.event_comp, "---", COMP_LENGTH);
        strncpy(a_event.event_desc, log_str, DESC_LENGTH);
        naslog_event_add(&a_event);
#endif		
    }
}
#endif

static void error_abort(const char *on_what)
{
    fputs(strerror(errno), stderr);
    fputs(": ", stderr);
    fputs(on_what, stderr);
    fputc('\n', stderr);
    exit(1);
}


// NOTE: pb should be at least 4 bytes buffer
void ip_str_to_bytes(char *str, unsigned char *pb)
{
    int n = IREV(inet_addr(str));		// need to do IREV for endian issues...
        pb[0] = n & 0xff;
        pb[1] = (n >> 8) & 0xff;
        pb[2] = (n >> 16) & 0xff;
        pb[3] = (n >> 24) & 0xff;
}

// NOTE: pb should be at least 6 bytes buffer, str must be in format "xx:xx:xx:xx:xx:xx"...
void mac_str_to_bytes(char *str, unsigned char *pb)
{
    int n;
    char buf[3];
    memset(buf, 0, sizeof(buf));

    strncpy(buf, &(str[0]), 2);	sscanf(buf, "%x", &n);		pb[0] = (unsigned char) n;
    strncpy(buf, &(str[3]), 2);	sscanf(buf, "%x", &n);		pb[1] = (unsigned char) n;
    strncpy(buf, &(str[6]), 2);	sscanf(buf, "%x", &n);		pb[2] = (unsigned char) n;
    strncpy(buf, &(str[9]), 2);	sscanf(buf, "%x", &n);		pb[3] = (unsigned char) n;
    strncpy(buf, &(str[12]), 2);	sscanf(buf, "%x", &n);		pb[4] = (unsigned char) n;
    strncpy(buf, &(str[15]), 2);	sscanf(buf, "%x", &n);		pb[5] = (unsigned char) n;
}


// Meiji 2007-05-14 ported from NASLib, original used to fix '$$' password issues.
int Get_Command_Line_Parameter_String(const char *in_str, char *out_str, int buf_size)
{
    int sq = 0;	// flag of single quoted
    int i = 0, j = 0;	

    for (i = 0; in_str[i] != '\0'; i++) {
        if (in_str[i] == '\'') {	// special handling of source single quote
            if (sq) {	// end current output single quote string first...
                if ( (j + 1) < buf_size ) {
                    out_str[j++] = '\'';		// end single quote...
                    sq = 0;
                }
                else break;		// out buffer not enough...
            }
            // then use doule quote to embrace single quote...
            if ( (j + 3) < buf_size ) {
                out_str[j++] = '\"';		// start double quote...
                out_str[j++] = in_str[i];	// '
                out_str[j++] = '\"';		// end double quote...
            }
            else break;		// out buffer not enough...
        }
        else {
            if (!sq) {
                if ( (j + 3) < buf_size ) {
                    out_str[j++] = '\'';	// start single quote...
                    out_str[j++] = in_str[i];
                    sq = 1;
                }
                else break;		// out buffer not enough...
            }
            else {
                if ( (j + 2) < buf_size ) out_str[j++] = in_str[i];
                else break;		// out buffer not enough...
            }
        }	
    }

    // end single quote if any...
    if (sq) {
        if ( (j + 1) < buf_size ) {
            out_str[j++] = '\'';		// end single quote...
            sq = 0;
        }
        else {
            // failed to write right single quote... SHOULD NOT HAPPEN...
            out_str[0] = '\0';
            return -1;
        }
    }
	
    // if in_str is EMPTY string (""), we need to put "\"\"" (2characters) instead of empty string in out_str...
    if (j == 0) {
        if ( (j + 2) < buf_size ) {
            out_str[j++] = '\"';		// start double quote...
            out_str[j++] = '\"';		// end double quote...
        }
        else {
            // SHOULD NOT HAPPEN...
            out_str[0] = '\0';
            return -3;
        }
    }
	
    // write end of string character...
    if ( j < buf_size ) out_str[j++] = '\0';		// end of string
    else {	// no space to write end of string... SHOULD NOT HAPPEN...
        out_str[0] = '\0';
        return -2;
    }

    return j;
}

// check packet source, format & sequence number.  If valid, return 1, O/W 0.
int is_valid_packet(FINDER_CMD_HEADER *pCH)
{
    if(SREV(pCH->Cmd) == FINDER_CMD_ACQUIRE_SERVER)  	// We will always respond to AcquireServer
        return 1;

	/* Check preamble via MAC address */
	if(memcmp(pCH->Preamble, MACAddr, 6) == 0) 
	{
		return 1;
	}
	return 0;
}


// easy function to create UDP socket...
int create_udp_socket(int so_broadcast, int so_reuseaddr, char *addr, int port, struct sockaddr_in *lpsa, char *bind_ifn)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if ( sock == -1 ) return -1;

	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr)) == -1) ||
		(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast)) == -1)) {
		close(sock);
		return -2;
	}

	// Meiji 2008-10-08: To avoid multi-LAN routing problem, specify sending interface here...
	if ( bind_ifn ) {	
		struct ifreq ifr;
		strncpy(ifr.ifr_name, bind_ifn, IFNAMSIZ);
		if ( setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0 ) {
			DBG("SO_BINDTODEVICE to %s failed...\n", bind_ifn);
		}
		// else	DBG("SO_BINDTODEVICE to %s OK..\n", bind_ifn);
	}
	
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IREV(inet_addr(addr));
	sa.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
		close(sock);
		return -3;
	}
	
	if (lpsa) memcpy(lpsa, &sa, sizeof(sa));
	
	return sock;
}


// independent receiver structure
typedef struct RECEIVER_PARAM
{
	char	*bind_addr;
	int	listen_port;

	char	*recv_buf;
	int	recv_buf_size;
	
	int	data_ready_flag;	// notify caller that data is ready. / notify receiver that data has been consumed.
	int	quit_flag;		// notify receiver to quit receiving...
	int	recv_data_count;
	
	struct sockaddr_in 	recv_sockaddr;
	
}	RECEIVER_PARAM;

// receiver thread to receive network packets...
void *receiver(void *recv_param)
{
	RECEIVER_PARAM *pRP = (RECEIVER_PARAM *) recv_param;
	int sock = create_udp_socket(1, 1, pRP->bind_addr/*"255.255.255.255"*/, pRP->listen_port, NULL, NULL);
	if (sock < 0) {
		// error...
		return NULL;
	}
	
	while (!pRP->quit_flag) {
		int recv_cnt = pRP->recv_buf_size;
		int ret = recvfrom(sock, pRP->recv_buf, recv_cnt, 0, (struct sockaddr*) &(pRP->recv_sockaddr), (socklen_t *)&recv_cnt);
		if (ret <= 0) continue;
		
		pRP->recv_data_count = ret;
		pRP->data_ready_flag = true;
		while (pRP->data_ready_flag) {	// buffer still not consumed, wait for a while...
			usleep(10000);	// wait 10 ms...
		}
		pRP->recv_data_count = 0;
	}
	
	close(sock);

	pRP->quit_flag = false;		// to notify that we have quitted successfully...
	
	return NULL;
}



int g_forever=1;

void* main_flow(void *port)
{
	// load system configuration first...
    printf("main_flow\n");
    query_config_file();
    unsigned char preamble[8];
    memcpy(preamble, MACAddr, 6);
    memcpy(&(preamble[6]), "Se", 2);

    // create sockets...
    int nic_socket_array[MAX_NIC_DEVICE_NUM];
    struct sockaddr_in nic_sa_array[MAX_NIC_DEVICE_NUM];
    int sock_index = 0;
    // Wayne 2012-03-08: create sockets for bonding NICs
    char nic_device_name[MAX_NIC_DEVICE_NAME_LEN];

    memset(nic_socket_array, 0, sizeof(nic_socket_array));
    for (sock_index = 0; sock_index < r_lan_num + r_wan_num; sock_index++)
    {
        char wan_ifname[MAX_NIC_DEVICE_NAME_LEN];
        int bcclient_wan = atoi(nvram_get("bcclient_wan"));

        memset(nic_device_name, 0, sizeof(nic_device_name));
        if(sock_index == 0)
            snprintf(nic_device_name, sizeof(nic_device_name), "br0");
        else if(bcclient_wan == 1){
            snprintf(wan_ifname, sizeof(wan_ifname), "wan%d_ifname", sock_index-1);
            snprintf(nic_device_name, sizeof(nic_device_name), nvram_get(wan_ifname));
        }
        printf("create_udp:idx:%d device:%s\n", sock_index, nic_device_name);
        nic_socket_array[sock_index] = create_udp_socket(1, 1, (char *)"255.255.255.255", (*(int*)port), &nic_sa_array[sock_index], nic_device_name );
		if (nic_socket_array[sock_index] < 0)
		{
			printf("%d error connection\n",nic_socket_array[sock_index]);
			char error_message[50];

			memset(error_message, 0, sizeof(error_message));
			snprintf(error_message, sizeof(error_message), "[Finder] Failed to create broadcasting socket %d.", sock_index);
			error_abort(error_message);
			if (sock_index == 0)
			{
				return NULL;
			}
		}
	}

	// create receiver thread to receive data...
	// Meiji: use non-blocking socket or use another thread,,,
	char recv_buf[MAX_PACKET_SIZE];
	RECEIVER_PARAM rp;
	memset(&rp, 0, sizeof(rp));
	rp.bind_addr = "255.255.255.255";
	rp.listen_port = *((int*)port);
	rp.recv_buf = recv_buf;
	rp.recv_buf_size = sizeof(recv_buf);
	rp.data_ready_flag = false;
	rp.quit_flag = false;				// used to tell receiver to quit...
	rp.recv_data_count = 0;
	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, &receiver, &rp);

	
	// when started, we send FINDER_CMD_ACQUIRE_SERVER automatically...
	TAG_LIST tag_list;
	tag_list.tag_num = 0;
	int ret = OnCmdAcquireServer(NULL, 0, &tag_list);
		
	char send_buf[MAX_PACKET_SIZE];
	char tag_buf[MAX_PACKET_SIZE];
	int len = TagListToBuffer(&tag_list, tag_buf, sizeof(tag_buf));
	len = MakeFinderCommandPacket(send_buf, sizeof(send_buf), preamble, FINDER_CMD_ACQUIRE_SERVER, 0, 0, tag_buf, len);

	for (sock_index = 0; sock_index < r_lan_num + r_wan_num; sock_index++)
	{
		if (nic_socket_array[sock_index] > 0) 
			ret = sendto(nic_socket_array[sock_index], send_buf, len, 0, (struct sockaddr*) &nic_sa_array[sock_index], sizeof(nic_sa_array[sock_index]));
	}
	if (g_forever)
	{
		g_forever = 0;

		// receive & respond loop
	for ( ; ; ) {
			rp.data_ready_flag = false;	// reset data_ready_flag will cause receiver to start another packet receiving...
			while (!rp.data_ready_flag) {
				// check if any worker threads finished its job...
				usleep(100000);		// sleep 0.1 second
			}

			// data received...
			int n = rp.recv_data_count;
			if (n < (int) sizeof(FINDER_CMD_HEADER))	// data length too short...
				continue;
				
			FINDER_CMD_PACKET *packet = (FINDER_CMD_PACKET *)recv_buf;
			FINDER_CMD_HEADER *header = (FINDER_CMD_HEADER *) recv_buf;

			// Check if a packet broadcast by NAS Server
			if (memcmp(&(header->Preamble[6]), "Se", 2) == 0)	// filter server packets...
				continue;
		

			//add our MAC to Finder packet , QFinder must need this MAC
 			mac_str_to_bytes((char*)nvram_get("lan_hwaddr_rule_default"), MACAddr );
			memcpy( preamble , MACAddr , 6);		// just in case if MacA address changed from time to time...
	
			printf("recv_buf->cmd %d start\n",header->Cmd);

			printf("Pre:%x:%x:%x:%x:%x:%x:%x:%x\n",header->Preamble[0], header->Preamble[1], header->Preamble[2], header->Preamble[3], header->Preamble[4], header->Preamble[5],header->Preamble[6],header->Preamble[7]);
			printf("MAC:%x:%x:%x:%x:%x:%x:%x:%x\n",MACAddr[0], MACAddr[1], MACAddr[2], MACAddr[3], MACAddr[4], MACAddr[5], MACAddr[6], MACAddr[7]);
			if (!is_valid_packet(header)) {
				DBG("Invalid packet received (%d bytes).\n", n);
				continue;
			}
			
			printf("recv_buf->cmd %d end\n",header->Cmd);

			g_last_recv_sock_addr = rp.recv_sockaddr.sin_addr;
		    DBG("receiver get packets from %s.\n", inet_ntoa(g_last_recv_sock_addr));
			
			int command = SREV(header->Cmd);
			DBG("Valid packet receive (%d bytes) - command %d.\n", n, command);
			
			// finding the command handling routine...
			ON_COMMAND_PROC pOnCommandFunc = NULL;
			for (int i = 0; DO_SERVER_CHANGE_FUNC_TABLE[i].pOnCommandFunc; i++) {
				if (DO_SERVER_CHANGE_FUNC_TABLE[i].nCommandID == command) {	// found...
					pOnCommandFunc = DO_SERVER_CHANGE_FUNC_TABLE[i].pOnCommandFunc;
					break;
				}
			}
			tag_list.tag_num = 0;		// clean the tag list...
			if (pOnCommandFunc) {	// do the command...
				DBG("Doing command[%d]. ", command);
				ret = pOnCommandFunc(packet, n, &tag_list);
			}
			else {		// unsupported operation?
				DBG("Unsupported function- %d. ", command);
				ret = ERROR_UNSUPPORTED;
			}
			AddTagIntegerToTagList(&tag_list, TAG_OPERATION_RESULT, ret);		// put return code in TAG_OPERATION_RESULT tag...
			DBG("Return Code: %d.\n", ret);
			
			len = TagListToBuffer(&tag_list, tag_buf, sizeof(tag_buf));
			len = MakeFinderCommandPacket(send_buf, sizeof(send_buf), preamble, 
				command, NEXT_SEND_SEQ_NUM(g_sequence_number), ret, tag_buf, len);

			for (sock_index = 0; sock_index < r_lan_num + r_wan_num; sock_index++)
			{
				if (nic_socket_array[sock_index] > 0) {
                    ret = sendto(nic_socket_array[sock_index], send_buf, len, 0, (struct sockaddr*) &nic_sa_array[sock_index], sizeof(nic_sa_array[sock_index]));
                    if(ret <= 0) {
                        DBG("Error send broadcast message\n");
                    }
                    ret = sendto(nic_socket_array[sock_index], send_buf, len, 0, (struct sockaddr*) &(rp.recv_sockaddr), sizeof(rp.recv_sockaddr));
                    if(ret <= 0) {
                        DBG("Error send unicast message\n");
                    }
                }
			}
		}
	}
	else
	{
		for (sock_index = 0; sock_index < MAX_NIC_DEVICE_NUM; sock_index++)
		{
			if (nic_socket_array[sock_index] > 0) close(nic_socket_array[sock_index]);
		}
		return NULL;
	}

	// notify receiver thread to quit...
	rp.quit_flag = true;				// used to tell receiver to quit...
	while (rp.quit_flag);				// wait for receiver to quit...
	
	for (sock_index = 0; sock_index < MAX_NIC_DEVICE_NUM; sock_index++)
	{
		if (nic_socket_array[sock_index] > 0) close(nic_socket_array[sock_index]);
	}
	return NULL;
}

static void update_info(int signo)
{
	int port = FINDER_SERVICE_PORT;
	if (signo==SIGUSR1)
	{
		main_flow(&port);
	}
}

void show_usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s [OPTIONS]\n", prog_name); 
	fprintf(stderr, "\t-D\trunning in debug mode.\n");
	fprintf(stderr, "\t-p port_no\tsepcify listening UDP port.\n");
}

int main(int argc, char **argv)
{
	int port = FINDER_SERVICE_PORT;
	
	int oc = 0;
	while ((oc = getopt(argc, argv, "Dp:")) != -1) {
		// optarg
		switch(oc) {
			case 'D':
				g_debug_mode = 1;
				break;
			case 'p':
				{
					int p = atoi(optarg);
					if (p == 0) {
						show_usage(argv[0]);
						exit(1);
					}
					port = p;
				}
				break;
			default:
				show_usage(argv[0]);
				exit(1);
				break;
		}
	}

	if (signal(SIGUSR1, update_info)==SIG_ERR)
		printf("can't catch SIGUSR1 !!\n");

    KeyTableInit();
	
	if (g_debug_mode) {
		main_flow(&port);
		exit(0);
	}

	// Run like daemon, quit right away...
	pid_t pid;
    pid_t sid;
	if((pid = fork()) < 0) {
		error_abort("[bcclient] Failed to fork process.");
		return pid;
	}
	else if(pid > 0) {
		exit(EXIT_SUCCESS);		// quit current instance, child will continue to run...
		return 0;
	}
	
	// child process continues...
    sid = setsid();
    if (sid < 0)
       exit(EXIT_FAILURE);

    pthread_t th;
    pthread_create(&th, NULL, &main_flow, &port);

    // loop forever...
    while (1) {

        int ret;
        if (waitpid(-1, &ret, WNOHANG) > 0) {	// some child died...
            // BUGBUG: check return code...
        }

        fflush(stdin);
        sleep(30);		// sleep 30 seconds...
    }

    return 0;
}

