#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include "ezp-lib.h"
#include "shared.h"
#include <bcmnvram.h>
#include <bcmutils.h>

#include	<signal.h>
#include	<sys/wait.h>

//#include	"sdk_version.h" //Perish added to get F/W version\
/* Remove this definition, make sure we don't use it again. */
/* #define RT288X_SDK_VERSION "3.3.0.0" */


#include "linux/autoconf.h"//Steve
#ifdef CONFIG_RT2880_GPIO_PCI_WIXM
#define RT2880GPIO_WPS_PB_PIN		47
#define RT2880GPIO_RST_PIN			50
/*Steve*/
#define RT2880GPIO_CHAL_PB_PIN      40
#define RT2880GPIO_FW_UPGRADE_PIN   45 //Add for FW upgrade
#define RT2880GPIO_FW_OK_PIN        46 //Add for FW upgrade
#define RT2880GPIO_LED_FP_IN_PIN    8  //Add for FW protect
#define RT2880GPIO_LED_FP_OUT_PIN   13 //Add for FW protect
#define RT2880GPIO_LED_WPS_PIN      49
#else
#define RT2880GPIO_WPS_PB_PIN		0
#define RT2880GPIO_RST_PIN			11
#define RT2880GPIO_LED_WPS_PIN      8
#endif

#ifdef DEBUG
#define DBGPRINT(fmt, args...)		printf(fmt, ## args)
#else
#define DBGPRINT(fmt, args...)
#endif

#define FW_VERSION	"test_v1.0"

#define DPORT 			8888
#define MAXLEN			240
#define PACKET_SIZE		100
#define MAGIC_NUM		"ABOCOM"
#define PASSWORD		"1234567890"
#define ERROR_RETURN		"ERROR FORMAT"
#define DIRECTION		0x00

//perish added for WPS LED pin 2009/1/20
#define WPS_LED 14

/* TEST HEADLE	FOR DIAP	********************************************/
#define ABOCOM_PACKET_CMD_DISCOVER	               	0x00
#define ABOCOM_PACKET_CMD_SET				0x01
#define ABOCOM_PACKET_CMD_GET				0x02

#define ABOCOM_PACKET_MAC				0x01
#define ABOCOM_PACKET_WIRELESS_CARD0_ENABLE		0x02	//SET ONLY
#define ABOCOM_PACKET_WIRELESS_CARD0_SSID		0x03
#define ABOCOM_PACKET_WIRELESS_CARD0_CHANNEL		0x04
#define ABOCOM_PACKET_WIRELESS_CARD1_ENABLE		0x05	//SET ONLY
#define ABOCOM_PACKET_WIRELESS_CARD1_SSID		0x06
#define ABOCOM_PACKET_WIRELESS_CARD1_CHANNEL		0x07
#define ABOCOM_PACKET_REBOOT				0x08	//SET ONLY
#define ABOCOM_PACKET_FACTORY_DEFAULT			0x09	//SET ONLY
#define ABOCOM_PACKET_WIRELESS_CARD0_DISABLE            0x0A
#define ABOCOM_PACKET_WIRELESS_CARD1_DISABLE            0x0B
#define ABOCOM_PACKET_COUNTRY                           0x0C
#define ABOCOM_PACKET_WIRELESS_CARD0_COUNTRY           	0x0D
#define ABOCOM_PACKET_WIRELESS_CARD1_COUNTRY           	0x0E
#define ABOCOM_PACKET_WIRELESS_CARD0_MAC                0x0F
#define ABOCOM_PACKET_WIRELESS_CARD1_MAC                0x10
#define ABOCOM_PACKET_WIRELESS_CARD0_STATUS             0x11
#define ABOCOM_PACKET_WIRELESS_CARD1_STATUS             0x12
#define ABOCOM_PACKET_WIRELESS_CARD0_RATEMODE        	0x13
#define ABOCOM_PACKET_WIRELESS_CARD1_RATEMODE        	0x14
#define ABOCOM_PACKET_NAME                              0x15
#define ABOCOM_PACKET_VERSION                           0x16
#define ABOCOM_USB_INFORMATION                          0x17
#define ABOCOM_PACKET_WIRELESS_CARD0_NAME               0x18
#define ABOCOM_PACKET_WIRELESS_CARD1_NAME               0x19
#define ABOCOM_PACKET_FTP_ENABLE                        0x1A
#define ABOCOM_PACKET_FTP_ANONYMOUS                     0x1B
#define ABOCOM_PACKET_FTP_ROOTDIR                       0x1C
#define ABOCOM_PACKET_FTP_SERVERPORT                    0x1D
#define ABOCOM_PACKET_FTP_RESTART                       0x1E
#define ABOCOM_PACKET_CLOSE_UDPS						0x1F
#define ABOCOM_PACKET_LED_TEST_GREEN                   	0x20
#define ABOCOM_PACKET_LED_TEST_AMBER                   	0x21
#define ABOCOM_PACKET_WIRELESS_CARD0_AC         		0x22    /* 8651B Only */
#define ABOCOM_PACKET_WIRELESS_CARD1_AC         		0x23    /* 8651B Only */
#define ABOCOM_PACKET_WIRELESS_CARD0_HIDESSID   		0x24    /* 8651B Only */
#define ABOCOM_PACKET_WIRELESS_CARD1_HIDESSID   		0x25    /* 8651B Only */
#define ABOCOM_PACKET_WDS_MAC			                0x26    /*RT2880*/
#define ABOCOM_PACKET_MODE			                    0x27    /*RT2880*/
#define EZP_USB_FIRST_EXTDEV_NUM    3

/*********************************************************************/
int             sockfd;
struct 		timeval        timeout;
struct          sockaddr_in    client_addr;

unsigned char DIAP_MAGIC_NUM[10]	= MAGIC_NUM;
unsigned char DIAP_PASSWORD[11]		= PASSWORD;
unsigned char DIAP_DIRECTION		= DIRECTION;
unsigned char DIAP_TYPE			= 0x00;
unsigned char DIAP_OPT			= 0x00;
unsigned char DIAP_CONTEXT[76];
unsigned char DIAP_PACKET[100];


/* Function define here */ //======Amendment by Jacken 2004/12/15 ==========//
int OpenSock();
int DIAP_PACKER_ANALYZE();
int DIAP_PACKET_GEN();
int DIAP_DISCOVER();


int DIAP_SET_MAC(unsigned char *mac);
int DIAP_SET_COUNTRY(unsigned char *region);
int DIAP_SET_WLAN0_ENABLE();
int DIAP_SET_WLAN0_DISABLE();
int DIAP_SET_WLAN0_SSID(unsigned char *wlanssid);
int DIAP_SET_WLAN0_CHANNEL(unsigned char *wlanchannel);
int DIAP_SET_WLAN0_RATEMODE(unsigned char *wlanrate);
int DIAP_SET_WLAN0_STATUS(unsigned char *wlanmode);
int DIAP_SET_WLAN0_COUNTRY(unsigned char *region);
int DIAP_SET_WLAN0_MAC(unsigned char *mac);
int DIAP_SET_WLAN1_ENABLE();
int DIAP_SET_WLAN1_DISABLE();
int DIAP_SET_WLAN1_SSID(unsigned char *wlanssid);
int DIAP_SET_WLAN1_CHANNEL(unsigned char *wlanchannel);
int DIAP_SET_WLAN1_RATEMODE(unsigned char *wlanrate);
int DIAP_SET_WLAN1_STATUS(unsigned char *wlanmode);
int DIAP_SET_WLAN1_COUNTRY(unsigned char *region);
int DIAP_SET_WLAN1_MAC(unsigned char *mac);
int DIAP_SET_FTP_ENABLE(unsigned char *status);
int DIAP_SET_FTP_ANONYMOUS(unsigned char *status);
int DIAP_SET_FTP_ROOTDIR(unsigned char *rootdir);
int DIAP_SET_FTP_SERVERPORT(unsigned char *port);
int DIAP_SET_FTP_RESTART();
int DIAP_SET_FACTORYDEFAULT();
int DIAP_SET_REBOOT();
int DIAP_SET_CARD0_WDS_MAC(unsigned char *mac); //Steve add


int DIAP_GET_NAME();
int DIAP_GET_VERSION();
int DIAP_GET_MAC();
int DIAP_GET_WLAN0_STATUS();
int DIAP_GET_WLAN0_SSID();
int DIAP_GET_WLAN0_NAME();
int DIAP_GET_WLAN0_RATEMODE();
int DIAP_GET_WLAN0_CHANNEL();
int DIAP_GET_WLAN0_COUNTRY();
int DIAP_GET_WLAN0_MAC();
int DIAP_GET_WLAN1_STATUS();
int DIAP_GET_WLAN1_SSID();
int DIAP_GET_WLAN1_NAME();
int DIAP_GET_WLAN1_RATEMODE();
int DIAP_GET_WLAN1_CHANNEL();
int DIAP_GET_WLAN1_COUNTRY();
int DIAP_GET_WLAN1_MAC();
int DIAP_GET_FTP_ENABLE();
int DIAP_GET_FTP_ANONYMOUS();
int DIAP_GET_FTP_ROOTDIR();
int DIAP_GET_FTP_SERVERPORT();
int DIAP_GET_USB_INFORMATION();
int DIAP_GET_LEDTEST_GREEN();
int DIAP_GET_LEDTEST_AMBER();
int DIAP_GET_CARD0_WDS_MAC(); //Steve add

unsigned char HEXTODEC(unsigned char ch);


/*main program of udps*/
void main(){
	int i;
	int iRet;
	int errCode;
	int s1,len;
	int iSize;
	int iTimeout=1000;
	unsigned char *p;
	unsigned char magic_num[10];
	unsigned char password[11];
	unsigned char direction;
	unsigned char DAT[78];

	unsigned char szsenBuffer[100];
	unsigned char szRecBuffer[MAXLEN];
	iSize=sizeof(client_addr);

	timeout.tv_sec=2;  // 2 seconds
  	timeout.tv_usec=0;

	FILE *fp;
	char line[20];
	pid_t pid;
	char *pdfile = "/var/run/udps.pid";

    printf("Enter udps \n");

	if ((fp = fopen(pdfile, "r")) != NULL) {
		fgets(line, sizeof(line), fp);
		if ( sscanf(line, "%d", &pid) ) {
			if (pid > 1)
				kill(pid, SIGTERM);
		}
		fclose(fp);
	}
	sprintf(line, "%d\n", getpid());
	if ((fp = fopen(pdfile, "w")) == NULL) {
		printf("Can't create PID file!");
		return -1;
	}
	fwrite(line, strlen(line), 1, fp);
	fclose(fp);

	while(1)
	{
		errCode=OpenSock();

		printf(" \n\n");
		iRet=recvfrom(sockfd, szRecBuffer, sizeof(szRecBuffer), 0, (struct sockaddr *)&client_addr, (socklen_t *)&iSize );

		/* Clear buffer */
		memset(DIAP_PACKET,0,sizeof(DIAP_PACKET));
		memcpy(DIAP_PACKET,szRecBuffer,sizeof(DIAP_PACKET));

		#if 0
		/* DUMP DATA */
		for(i = 0 ;i < PACKET_SIZE ; i ++)
		{
			if( (i+1) < 16)
			printf(" %02x",szRecBuffer[i]);
			if((i+1) == 16)
			printf(" %02x\n",szRecBuffer[i]);
		  	if( (i+1) > 16)
		  	{
		  		if( ((i+1) % 16) == 0 )
		  		{
		  			printf(" %02x\n",szRecBuffer[i]);
		  		}
		  		else
		  		printf(" %02x",szRecBuffer[i]);
		  	}
		}
		/* End of dump data */
		#endif

		printf("\n");
		/* CHECK PACKET INFO CORRECT */
		if(DIAP_PACKER_ANALYZE() == 0)
			continue;

		/* DIAP OPERATION AND SELECT COMMAND */
		#ifdef DEBUG
			printf("DIAP_TYPE = %02x\n",DIAP_TYPE);
		#endif

		switch(DIAP_TYPE)
		{   //Jacken: revise by Jacken 2004/12/15
			/* Return system info */
			case ABOCOM_PACKET_CMD_DISCOVER:
				printf("DIAP DISCOVER PACKET FIND!!\n");
				DIAP_DISCOVER();
				break;

			case ABOCOM_PACKET_CMD_SET:
				 /* DIAP SET DATA */
				 printf("DIAP SET COMMAND FIND!!\n");
				 switch(DIAP_OPT)
				 {
				 	case ABOCOM_PACKET_MAC:
				 	iRet = DIAP_SET_MAC(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_ENABLE:
				 	iRet = DIAP_SET_WLAN0_ENABLE();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_SSID:
				 	iRet = DIAP_SET_WLAN0_SSID(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_CHANNEL:
				 	iRet = DIAP_SET_WLAN0_CHANNEL(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_ENABLE:
				 	iRet = DIAP_SET_WLAN1_ENABLE();
				 	break;

			 	       	case ABOCOM_PACKET_WIRELESS_CARD1_SSID:
				 	iRet = DIAP_SET_WLAN1_SSID(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_CHANNEL:
				 	iRet = DIAP_SET_WLAN1_CHANNEL(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_REBOOT:
				 	iRet = DIAP_SET_REBOOT();
				  	break;

				 	case ABOCOM_PACKET_FACTORY_DEFAULT:
				 	iRet = DIAP_SET_FACTORYDEFAULT();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_DISABLE:
                                        //printf("JACKEN DEBUG");
				 	iRet = DIAP_SET_WLAN0_DISABLE();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_DISABLE:
				 	iRet = DIAP_SET_WLAN1_DISABLE();
				  	break;

				 	case ABOCOM_PACKET_COUNTRY:
				 	iRet = DIAP_SET_COUNTRY(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_COUNTRY:
				 	iRet = DIAP_SET_WLAN0_COUNTRY(DIAP_CONTEXT);
				  	break;

			 	       	case ABOCOM_PACKET_WIRELESS_CARD1_COUNTRY:
				 	iRet = DIAP_SET_WLAN1_COUNTRY(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_MAC:
				 	iRet = DIAP_SET_WLAN0_MAC(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_MAC:
				 	iRet = DIAP_SET_WLAN1_MAC(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_RATEMODE:
				 	iRet = DIAP_SET_WLAN0_RATEMODE(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_RATEMODE:
				 	iRet = DIAP_SET_WLAN1_RATEMODE(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_STATUS:
				 	iRet = DIAP_SET_WLAN0_STATUS(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_STATUS:
				 	iRet = DIAP_SET_WLAN1_STATUS(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_FTP_ENABLE:
				 	iRet = DIAP_SET_FTP_ENABLE(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_FTP_ANONYMOUS:
				 	iRet = DIAP_SET_FTP_ANONYMOUS(DIAP_CONTEXT);
				 	break;

				 	case ABOCOM_PACKET_FTP_ROOTDIR:
				 	iRet = DIAP_SET_FTP_ROOTDIR(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_FTP_SERVERPORT:
				 	iRet = DIAP_SET_FTP_SERVERPORT(DIAP_CONTEXT);
				  	break;

				 	case ABOCOM_PACKET_FTP_RESTART:
				 	iRet = DIAP_SET_FTP_RESTART();
				  	break;

				  	case ABOCOM_PACKET_WDS_MAC:
				 	iRet = DIAP_SET_CARD0_WDS_MAC(DIAP_CONTEXT);
				 	break;
				}
				break;

			case ABOCOM_PACKET_CMD_GET:
				/* DIAP GET DATA */
		                printf("DIAP GET COMMAND FIND!!\n");
				switch(DIAP_OPT)
				{
				 	case ABOCOM_PACKET_MAC:
				 	iRet = DIAP_GET_MAC();
				  	break;

				 	case ABOCOM_PACKET_NAME:
				 	iRet = DIAP_GET_NAME();
				  	break;

				 	case ABOCOM_PACKET_VERSION:
				 	iRet = DIAP_GET_VERSION();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_SSID:
				 	iRet = DIAP_GET_WLAN0_SSID();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_CHANNEL:
				 	iRet = DIAP_GET_WLAN0_CHANNEL();
				  	break;

			 		case ABOCOM_PACKET_WIRELESS_CARD1_SSID:
				 	iRet = DIAP_GET_WLAN1_SSID();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_CHANNEL:
				 	iRet = DIAP_GET_WLAN1_CHANNEL();
				  	break;

				 	case ABOCOM_PACKET_COUNTRY:
				 	iRet = DIAP_GET_COUNTRY();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_COUNTRY:
				 	iRet = DIAP_GET_WLAN0_COUNTRY();
				  	break;

					case ABOCOM_PACKET_WIRELESS_CARD1_COUNTRY:
				 	iRet = DIAP_GET_WLAN1_COUNTRY();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_MAC:
				 	iRet = DIAP_GET_WLAN0_MAC();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_STATUS:
				 	iRet = DIAP_GET_WLAN0_STATUS();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_STATUS:
				 	iRet = DIAP_GET_WLAN1_STATUS();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_NAME:
				 	iRet = DIAP_GET_WLAN1_NAME();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_RATEMODE:
				 	iRet = DIAP_GET_WLAN1_RATEMODE();
				  	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_NAME:
				 	iRet = DIAP_GET_WLAN0_NAME();
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD0_RATEMODE:
				 	iRet = DIAP_GET_WLAN0_RATEMODE();
				 	break;

				 	case ABOCOM_PACKET_WIRELESS_CARD1_MAC:
				 	iRet = DIAP_GET_WLAN1_MAC();
				  	break;

				 	case ABOCOM_USB_INFORMATION:
				 	iRet = DIAP_GET_USB_INFORMATION();
				  	break;

				 	case ABOCOM_PACKET_FTP_ENABLE:
				 	iRet = DIAP_GET_FTP_ENABLE();
				  	break;

				 	case ABOCOM_PACKET_FTP_ANONYMOUS:
				 	iRet = DIAP_GET_FTP_ANONYMOUS();
				  	break;

				 	case ABOCOM_PACKET_FTP_ROOTDIR:
				 	iRet = DIAP_GET_FTP_ROOTDIR();
				  	break;

				 	case ABOCOM_PACKET_FTP_SERVERPORT:
				 	iRet = DIAP_GET_FTP_SERVERPORT();
				 	break;

				 	case ABOCOM_PACKET_LED_TEST_GREEN:
				 	iRet = DIAP_GET_LEDTEST_GREEN();
				 	break;

				 	case ABOCOM_PACKET_LED_TEST_AMBER:
				 	iRet = DIAP_GET_LEDTEST_AMBER();
				 	break;

				 	case ABOCOM_PACKET_WDS_MAC:
				 	iRet = DIAP_GET_CARD0_WDS_MAC();
				  	break;

				  	case ABOCOM_PACKET_MODE:
				 	iRet = DIAP_GET_MODE();
				  	break;

				}
				break;

				/* no command find */
				default:
				memset(szsenBuffer,0,sizeof(szsenBuffer));
				strcpy((char*)szsenBuffer,ERROR_RETURN);
				iRet=sendto(sockfd, szsenBuffer, sizeof(szsenBuffer), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
				printf("\n Send Packet iRet=%08x \n",iRet);
				break;
		}
		close(sockfd);
	}

}

/* Function: OpenSock()
	 Purpose:	init socket config and setting UDP protol
	 Return: 0 or stop program.
*/
int OpenSock()
{
	int	IL=1;
	int	so_broadcast;
     // set up client address
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(DPORT);
	client_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	// create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		printf("Error: socket( )\n");
        	exit(1);
	}
	// set socket option - enable broadcasting
	if (setsockopt(sockfd, SOL_SOCKET,
                   SO_BROADCAST, &so_broadcast,
				   sizeof(so_broadcast)))
	{
        	printf("Error: setsockopt(..SO_BROADCAST..)\n");
		exit(1);
	}

	// set socket receive timeout
	/*
	if (setsockopt(sockfd, SOL_SOCKET,
                   SO_RCVTIMEO, &timeout,
				   sizeof(timeout)))
	{
        	printf("Error: setsockopt(..SO_RCVTIMEO..)\n");
		exit(1);
	}
	*/

	// set socket send timeout
     	if (setsockopt(sockfd, SOL_SOCKET,
                   SO_SNDTIMEO, &timeout,
				   sizeof(timeout)))
	{
        	printf("Error: setsockopt(..SO_SNDTIMEO..)\n");
		exit(1);
	}

	// set socket REUSEADDR
	if (setsockopt(sockfd, SOL_SOCKET,
                   SO_REUSEADDR, &IL, sizeof(IL)))
	{
        	printf("Error: setsockopt(..SO_REUSEADDR..)\n");
		exit(1);
	}
	if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)))
	{
        	printf("Error: bind( )\n");
		exit(1);
	}
     	return 0;
}
/**************************************************************/
/* Function: DIAP_PACKET_GEN()
	 Purpose:	SEND DISCOVER INFO PACKET TO CLIENT
	 Return: 0:success  1:fail
*/
/**************************************************************/
int DIAP_PACKET_GEN()
{
	unsigned char *p;
	unsigned char DAT[100];
	memset(DAT,0,sizeof(DAT));
	p = DAT;
	/*1. set magic number 10 bytes*/
	memcpy(p,DIAP_MAGIC_NUM,10);
	/*2. set password 11 bytes */
	memcpy((p+9),DIAP_PASSWORD,11);
	/*3. set direction 1 byte */
	//*(p+20) = DIAP_DIRECTION;
	*(p+20) = 0x00;
	/*4. set diap type 1 byte */
	*(p+21) = DIAP_TYPE;
	/*5. set diap opt 1 byte */
	*(p+22) = DIAP_OPT;
	/*6. set diap context 76 byte */
	memcpy((p+23),DIAP_CONTEXT,76);
	memcpy(DIAP_PACKET,DAT,sizeof(DAT));
	return 0;
}
/**************************************************************/
/* Function: DIAP_PACKER_ANALYZE()
	 Purpose:	SEND DISCOVER INFO PACKET TO CLIENT
	 Return: 0:success  1:fail
*/
/**************************************************************/
int DIAP_PACKER_ANALYZE()
{
	unsigned char *p;
	p = DIAP_PACKET;

	unsigned char ANALYZE_DIAP_MAGIC_NUM[10];
	unsigned char ANALYZE_DIAP_PASSWORD[11];
	unsigned char ANALYZE_DIAP_DIRECTION;
	unsigned char ANALYZE_DIAP_TYPE;
	unsigned char ANALYZE_DIAP_OPT;
	unsigned char ANALYZE_DIAP_CONTEXT[76];

	/*COPY DATA FOR ANALYZE */
	memcpy(ANALYZE_DIAP_MAGIC_NUM,p,10);
	memcpy(ANALYZE_DIAP_PASSWORD,(p+9),11);
	ANALYZE_DIAP_DIRECTION = *(p+21);
	ANALYZE_DIAP_TYPE = *(p+22);
	ANALYZE_DIAP_OPT = *(p+23);
	memcpy(ANALYZE_DIAP_CONTEXT,(p+24),76);
	/*CHECK PACKET FORMAT CORRECT */
	#ifdef DEBUG
		printf("ANALYZE_DIAP_MAGIC_NUM:%s\n",ANALYZE_DIAP_MAGIC_NUM);
		printf("ANALYZE_DIAP_PASSWORD:%s\n",ANALYZE_DIAP_PASSWORD);
		printf("ANALYZE_DIAP_DIRECTION:%02x\n",ANALYZE_DIAP_DIRECTION);
		printf("ANALYZE_DIAP_TYPE:%02x\n",ANALYZE_DIAP_TYPE);
		printf("ANALYZE_DIAP_OPT:%02x\n",ANALYZE_DIAP_OPT);
	#endif

	/* Check MAGIC_NUM */
	if(strcmp(ANALYZE_DIAP_MAGIC_NUM,"ABOCOM") != 0)
	{
		close(sockfd);
		printf("MAGICNUM NOT MATCH!!\n");
		return 0;
	}

	/* Check Password */
	if(strncmp(ANALYZE_DIAP_PASSWORD,PASSWORD,10) != 0)
	{
		printf("PASSWORD NOT MATCH!!\n");
		close(sockfd);
		return 0;
	}

	/* Check Direction */
	if(ANALYZE_DIAP_DIRECTION != 0xff)
	{
		printf("DIRECTION NOT MATCH!!\n");
		close(sockfd);
		return 0;
	}

	/* Update date */
	memcpy(DIAP_MAGIC_NUM,ANALYZE_DIAP_MAGIC_NUM,10);
	memcpy(DIAP_PASSWORD,ANALYZE_DIAP_PASSWORD,11);
	DIAP_DIRECTION = ANALYZE_DIAP_DIRECTION;
	DIAP_TYPE = ANALYZE_DIAP_TYPE;
	DIAP_OPT = ANALYZE_DIAP_OPT;
	memcpy(DIAP_CONTEXT,ANALYZE_DIAP_CONTEXT,76);
	return 1;
}
/**************************************************************/
/* Function: DIAP_DISCOVER()
	 Purpose:	SEND DISCOVER INFO PACKET TO CLIENT
	 Return: 0:success  1:fail
*/
/**************************************************************/

int DIAP_DISCOVER()
{

   	FILE *fp;
 	unsigned char buf[200]="";
 	unsigned char tempStr[32]="";
    unsigned char verstr[32]="";
 	int iRet;

    printf("=============================================================\n");
    printf("UDPS =>  Get DEVICE INFORMATION\n");
	memset(buf,0,sizeof(buf));
    /* EZP: */    
    system("nvram get lan0_ipaddr > /var/lanip");

	fp = fopen("/var/lanip","a+");
	if(fp==NULL){
        printf("Error open /var/lanip\n");
	}else{
		fgets(tempStr,32,fp);
		printf("%s\n",tempStr);
	}
	fclose(fp);

    memset(buf,0,sizeof(buf));
    system("nvram get custom_fw_version > /var/sw_version");
    fp = fopen("/var/sw_version","a+");                                             if(fp==NULL){
        printf("Error open /var/sw_version\n");
    }else{
        fgets(verstr, 32, fp);
        printf("%s\n", verstr);
        fclose(fp);
        /* If custom version is empty */
        if(strlen(verstr) < 2) {
            system("nvram get fw_version > /var/sw_version");
            fp = fopen("/var/sw_version","a+");
            if(fp==NULL){
                printf("Error open /var/sw_version\n");
            }else{
                fgets(verstr, 32, fp);
                printf("%s\n", verstr);
                fclose(fp);
            }
        }
    }

	sprintf(buf, "Firmware version: %s \r\n Lan IP: %s", verstr, tempStr);
	system("rm /var/lanip");
    printf("%s\n", buf);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
    printf("=============================================================\n");
	return 0;
}


/* Function: DIAP_SET_MAC()
	 Purpose:	init socket config and setting UDP protol
	 Return: 0:success  1:fail
*/
int DIAP_SET_MAC(unsigned char *mac)
{
	unsigned char buf[76]="";
	unsigned char cmd_ra0[80]="";

	unsigned char cmd_set[80]="";

	int iRet;
	int i;
	printf("=============================================================\n");
	printf("UDPS => SET MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));

	//Perish modified to set WLAN mac, WAN mac, LAN mac in oder at one time 2009/2/26
	//========== set WLAN mac ======================================================
	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 4=%02X%02X ",*(mac+1),*(mac+0));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 6=%02X%02X ",*(mac+3),*(mac+2));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 8=%02X%02X ",*(mac+5),*(mac+4));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);
	//========== set WAN mac ======================================================
	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 2e=%02X%02X ",*(mac+1),*(mac+0));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 30=%02X%02X ",*(mac+3),*(mac+2));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 32=%02X%02X ",(*(mac+5)+5),*(mac+4));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);
	//========== set LAN mac ======================================================
	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 28=%02X%02X ",*(mac+1),*(mac+0));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 2a=%02X%02X ",*(mac+3),*(mac+2));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);

	memset(cmd_ra0,0,sizeof(cmd_ra0));
	sprintf(cmd_ra0,"iwpriv ra0 e2p 2c=%02X%02X ",(*(mac+5)+4),*(mac+4));
	system(cmd_ra0);
	printf("%s\n",cmd_ra0);
	
	//==========================================================================================
	sprintf(buf,"SET MAC SUCCESS %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_SET_COUNTRY(unsigned char *region)
{
	FILE *fp;
 	unsigned char buf[100];
 	int iRet;
	unsigned char cmd[100]="";

    printf("Set COUNTRY CODE\n");
    /* EZP:Set country region into nvram default */
    if (region[0] == '0') {
        switch (region[1])
        {
            case '0':
                /* Region code '00',  US */
                system("nvram fset wl_region_default=\"0\"");
                system("nvram fset wl_region=\"0\"");
                system("nvram fset wl_regionaband_default=\"0\"");
                system("nvram fset wl_regionaband=\"0\"");
                system("nvram fset wl_countrycode_default=\"US\"");
                system("nvram fset wl_countrycode=\"US\"");
                break;
            case '1':
                /* Region code '01', FR */
                system("nvram fset wl_region_default=\"1\"");
                system("nvram fset wl_region=\"1\"");
                system("nvram fset wl_regionaband_default=\"2\"");
                system("nvram fset wl_regionaband=\"2\"");
                system("nvram fset wl_countrycode_default=\"FR\"");
                system("nvram fset wl_countrycode=\"FR\"");
                break;
            case '5':
                /* Region code '05', JP */
                system("nvram fset wl_region_default=\"5\"");
                system("nvram fset wl_region=\"5\"");
                system("nvram fset wl_regionaband_default=\"6\"");
                system("nvram fset wl_regionaband=\"6\"");
                system("nvram fset wl_countrycode_default=\"JP\"");
                system("nvram fset wl_countrycode=\"JP\"");
                break;
            default:
                break;
        }
    }
    else {
        system("nvram fset wl_region_default=\"1\"");
        system("nvram fset wl_region=\"1\"");
        system("nvram fset wl_regionaband_default=\"2\"");
        system("nvram fset wl_regionaband=\"2\"");
        system("nvram fset wl_countrycode_default=\"FR\"");
        system("nvram fset wl_countrycode=\"FR\"");
    }
    system("nvram commit");
    sprintf(buf,"Set COUNTRY CODE SUCCESS %s\n",region);
    memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
    memcpy(DIAP_CONTEXT,buf,sizeof(buf));
    DIAP_PACKET_GEN();
    iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)  &client_addr, sizeof(client_addr));
    printf("\nSend Packet iRet=%08x \n",iRet);
    return 0;
}

int DIAP_SET_WLAN0_ENABLE()
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN0 Enable\n");
/* EZP */
	sprintf(cmd,"ifconfig ra0 up");
       //printf(cmd);
	system(cmd);

	sprintf(buf,"SET WLAN0 ENABLE SUCCESS\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN0_DISABLE()
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN0 Disable\n");
    /* EZP */
	sprintf(cmd,"ifconfig ra0 down");
        //printf(cmd);
	system(cmd);

	sprintf(buf,"SET WLAN0 DISABLE SUCCESS\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN0_SSID(unsigned char *wlanssid)
{
	unsigned char buf[76];
	unsigned char cmd[100]={0};
	int iRet;

    printf("=============================================================\n");
	printf("UDPS =>  SET WLAN0 SSID: %s\n",wlanssid);

	sprintf(cmd,"iwpriv ra0 set SSID=%s",wlanssid);
    printf("%s\n",cmd);
	system(cmd);
    /* EZP:disable hardware nat for factory wireless testing. */
    sprintf(cmd,"rmmod hw_nat");
	system(cmd);

	memset(cmd,0,sizeof(cmd));
    /* EZP */
	sprintf(cmd,"nvram replace attr wl0_ssid_rule 0 ssid %s",wlanssid);
    printf("%s\n",cmd);
	system(cmd);

	sprintf(buf,"SET WLAN0 SSID SUCCESS %s\n",wlanssid);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_SET_WLAN0_CHANNEL(unsigned char *wlanchannel)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;

    printf("=============================================================\n");
	printf("UDPS =>  SET WLAN0 CHANNEL\n");
  	sprintf(cmd,"iwpriv ra0 set Channel=%s",wlanchannel);
    printf(cmd);
	system(cmd);
    /* EZP */
  	sprintf(cmd,"nvram replace attr wl_basic_rule 0 channel %s",wlanchannel);
    printf(cmd);
	system(cmd);
	sprintf(buf,"SET WLAN0 CHANNEL SUCCESS %s\n",wlanchannel);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_SET_WLAN0_RATEMODE(unsigned char *wlanrate)
{
 	unsigned char buf[76]={0};
	unsigned char cmd[100]={0};
	int iRet;

    printf("=============================================================\n");
    printf("UDPS =>  SET WLAN0 RATEMODE: %s\n",wlanrate);

    /* Rate Mode */
	if(strncmp(wlanrate,"802.11G",7) == 0){
		printf("Set 802.11G !!\n");
		sprintf(cmd,"iwpriv ra0 set WirelessMode=0");
        printf("%s\n",cmd);
	    system(cmd);
	}else if(strncmp(wlanrate,"802.11N",7) == 0){
		printf("Set 802.11N !!\n");
		sprintf(cmd,"iwpriv ra0 set WirelessMode=9");
        printf("%s\n",cmd);
	    system(cmd);
	}

	//sprintf(cmd,"flash set WLAN0_BASIC_RATES %s",wlanrate);
	//system(cmd);
	//printf("Steve Test:RATEMODE: %s\n",cmd);

	sprintf(buf,"SET WLAN0 RATEMODE SUCCESS %s\n",wlanrate);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_SET_WLAN0_STATUS(unsigned char *wlanmode)
{
 	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;

	printf("DIAP SET WLAN0 STATUS: %s\n",wlanmode);
	sprintf(cmd,"nvram replace attr wl_basic_rule 0 net_mode %s",wlanmode);
	system(cmd);

	sprintf(buf,"SET WLAN0 STATUS SUCCESS %s\n",wlanmode);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN0_COUNTRY(unsigned char *region)
{
	FILE *fp;
 	unsigned char buf[100]="";
 	int iRet;
	unsigned char cmd[100]="";

    printf("Set WLAN0 COUNTRY CODE\n");
	memset(buf,0,sizeof(buf));

	sprintf(cmd,"flash set DEF_WLAN0_REG_DOMAIN %s",region);
       //printf(cmd);
	system(cmd);

	memset(buf,0,sizeof(buf));

	sprintf(cmd,"flash set WLAN0_REG_DOMAIN %s",region);
       //printf(cmd);
	system(cmd);

	sprintf(buf,"Set WLAN0 COUNTRY CODE SUCCESS %s\n",region);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN0_MAC(unsigned char *mac)
{

	FILE *fp;
 	unsigned char buf[100]="";
 	unsigned char cmd_wlan0[100]="";
 	int iRet;

	printf("SET WLAN0 MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
	sprintf(buf,"WLAN0 MAC: %02x%02x%02x%02x%02x%02x",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
    /* EZP */
	sprintf(cmd_wlan0,"ifconfig ra0 hw ether %02x%02x%02x%02x%02x%02x",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
//	printf("%s\n",cmd_wlan0);
	system(cmd_wlan0);
	memset(cmd_wlan0,0,sizeof(cmd_wlan0));
    /* EZP */
	sprintf(cmd_wlan0,"flash set HW_WLAN0_MAC %02x%02x%02x%02x%02x%02x",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
//	printf("%s\n",cmd_wlan0);
	system(cmd_wlan0);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_ENABLE()
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN1 Enable\n");

	sprintf(buf,"There is no WLAN1 exist !\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_DISABLE()
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN1 DISABLE\n");

	sprintf(buf,"There is no WLAN1 to setup\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_SSID(unsigned char *wlanssid)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN1 SSID: %s\n",wlanssid);
    /* EZP */
  	sprintf(cmd,"flash set WLAN1_SSID %s",wlanssid);
       //printf(cmd);
	system(cmd);

	sprintf(buf,"SET WLAN1 SSID SUCCESS %s\n",wlanssid);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_CHANNEL(unsigned char *wlanchannel)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP SET WLAN1 CHANNEL: %s\n",wlanchannel);
    /* EZP */
  	sprintf(cmd,"flash set WLAN1_CHANNEL %s",wlanchannel);
       //printf(cmd);
	system(cmd);

	sprintf(buf,"SET WLAN1 CHANNEL SUCCESS %s\n",wlanchannel);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_RATEMODE(unsigned char *wlanrate)
{
 	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;

	printf("DIAP SET WLAN1 RATEMODE: %s\n",wlanrate);
    /* EZP */
	sprintf(cmd,"flash set WLAN1_BASIC_RATES %s",wlanrate);
       //printf(cmd);
	system(cmd);

	sprintf(buf,"SET WLAN1 RATEMODE SUCCESS %s\n",wlanrate);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_STATUS(unsigned char *wlanmode)
{
 	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;

	printf("DIAP SET WLAN1 STATUS: %s\n",wlanmode);
    /* EZP */
	sprintf(cmd,"flash set WLAN1_MODE %s",wlanmode);
	system(cmd);

	sprintf(buf,"SET WLAN1 STATUS SUCCESS %s\n",wlanmode);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_COUNTRY(unsigned char *region)
{
	FILE *fp;
 	unsigned char buf[100]="";
 	int iRet;
	unsigned char cmd[100]="";

	printf("Set WLAN1 COUNTRY CODE\n");
	memset(buf,0,sizeof(buf));
	sprintf(cmd,"flash set DEF_WLAN1_REG_DOMAIN %s",region);
       //printf(cmd);
	system(cmd);

	memset(buf,0,sizeof(buf));
	sprintf(cmd,"flash set WLAN1_REG_DOMAIN %s",region);
       //printf(cmd);
	system(cmd);

	sprintf(buf,"Set WLAN1 COUNTRY CODE SUCCESS %s\n",region);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_WLAN1_MAC(unsigned char *mac)
{
	FILE *fp;
 	unsigned char buf[100];
	unsigned char cmd_wlan1[100]="";
 	int iRet;
	memset(buf,0,sizeof(buf));
	printf("SET WLAN1 MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
    /* EZP */
	sprintf(buf,"WLAN1 MAC: %02x%02x%02x%02x%02x%02x",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
    /* EZP */
	sprintf(cmd_wlan1,"flash set HW_WLAN1_MAC %02x%02x%02x%02x%02x%02x",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
//	printf("%s\n",cmd_wlan1);
	system(cmd_wlan1);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_FTP_ENABLE(unsigned char *status)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP Set FTP Enable: %s\n",status);

	sprintf(buf,"This function is not supported ! %s\n",status);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_FTP_ANONYMOUS(unsigned char *status)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP Set FTP Anonymous: %s\n",status);

	sprintf(buf,"Function FTP Anonymous not support %s\n",status);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_FTP_ROOTDIR(unsigned char *rootdir)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP Set FTP Root Dir: %s\n",rootdir);
	sprintf(buf,"Not support FTP Root Dir %s\n",rootdir);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
 }

int DIAP_SET_FTP_SERVERPORT(unsigned char *port)
{
	unsigned char buf[76];
	unsigned char cmd[100];
	int iRet;
	printf("DIAP Set FTP Server Port: %s\n",port);
	sprintf(buf,"FTP Port not support %s\n",port);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_SET_FTP_RESTART()
{
	unsigned char buf[76];
       	unsigned char cmd[80];
     	int iRet;

	printf("Start To Restart FTP\n");
	sprintf(buf,"Restart FTP not support !\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("\nSend Packet iRet=%08x \n",iRet);
//       	sprintf(cmd,"webs DIAP 0044");
//       	system(cmd);
}

int DIAP_SET_FACTORYDEFAULT()
{
	unsigned char buf[76];
       	unsigned char cmd[80];
     	int iRet;

    printf("=============================================================\n");
    printf("UDPS =>  Start To Factorydefault System\n");
	sprintf(buf,"Start To Factorydefault System OK\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");

	DBGPRINT("ralink_init clear\n");
    /* EZP */
	system("nvram factory");
}
int DIAP_SET_REBOOT()
{
	unsigned char buf[76];
       	unsigned char cmd[80];
     	int iRet;

    printf("=============================================================\n");
	printf("UDPS =>  Start To Reboot System\n");
	sprintf(buf,"Start To Reboot System OK\n");
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");

//     	sprintf(cmd,"reboot");
//     	system(cmd);
	system("reboot");
}

//--------------------------------------------//
/* Function: DIAP_GET_MAC()
	 Purpose:	init socket config and setting UDP protol
	 Return: 0:success 1:fail
*/
//--------------------------------------------//

int DIAP_GET_NAME()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;


       	printf("Get DEVICE NAME\n");
    /* EZP */
	system("echo \"ra0\" > /var/device");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/device","a+");
	if(fp==NULL)
	{
		printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
	printf("%s\n",p);
	for(i=1;i<13;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/device");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_VERSION()
{
	FILE *fp=NULL;
 	unsigned char buf[200];
 	int iRet;
 	char ver1[32];
	char lang[5];
	char cc[5];

 	printf("=============================================================\n");
	printf("UDPS =>  Get DEVICE FIRMWARE VERSION\n");
    /*
     * Get firmware version from nvram
     */
    memset(buf, 0, sizeof(buf));
    memset(ver1, 0, sizeof(ver1));
    system("nvram get custom_fw_version > /var/sw_version");
    fp = fopen("/var/sw_version","a+");                                             if(fp==NULL){
        printf("Error open /var/sw_version\n");
    }else{
        fgets(ver1, 32, fp);
        printf("%s\n", ver1);
        fclose(fp);
        /* If custom version is empty */
        if(strlen(ver1) < 2) {
            system("nvram get fw_version > /var/sw_version");
            fp = fopen("/var/sw_version","a+");
            if(fp==NULL){
                printf("Error open /var/sw_version\n");
            }else{
                fgets(ver1, 32, fp);
                printf("%s\n", ver1);
                fclose(fp);
            }
        }
    }

	sprintf(buf, "%s", ver1);
	printf("Firmware Version : %s\n",buf);


	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
    printf("=============================================================\n");
	return 0;
}

int DIAP_GET_MAC()
{
	int iRet;
	unsigned char buf[20]="";
	unsigned char mac[20]="";
	unsigned char mac1[4]="";
	unsigned char mac2[4]="";
	unsigned char mac3[4]="";
	FILE *fp;
	char tempStr[200]="";
	char *p;
	int i;

	printf("=============================================================\n");
    printf("UDPS =>  Get MAC\n");

	system("iwpriv ra0 e2p 4 | grep 0x > /var/nic0_1");
	system("iwpriv ra0 e2p 6 | grep 0x > /var/nic0_2");
	system("iwpriv ra0 e2p 8 | grep 0x > /var/nic0_3");

	//mac1
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/nic0_1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(buf,20,fp);
	}
	fclose(fp);
	memcpy(mac1,buf+11,4);
	//printf("\n\n Steve Test:%s\n",mac1);
	mac[0]=mac1[2];
	mac[1]=mac1[3];
	mac[2]=mac1[0];
	mac[3]=mac1[1];

	//mac2
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/nic0_2","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(buf,20,fp);
	}
	fclose(fp);
	memcpy(mac2,buf+11,4);
	//printf("\n\n Steve Test:%s\n",mac2);
	mac[4]=mac2[2];
	mac[5]=mac2[3];
	mac[6]=mac2[0];
	mac[7]=mac2[1];

	//mac3
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/nic0_3","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(buf,20,fp);
	}
	fclose(fp);
	memcpy(mac3,buf+11,4);
	//printf("\n\n Steve Test:%s\n",mac3);
	mac[8]=mac3[2];
	mac[9]=mac3[3];
	mac[10]=mac3[0];
	mac[11]=mac3[1];

	memset(buf,0,sizeof(buf));
	memcpy(buf,mac,12);
	printf("Read MAC is : %s\n",buf);

	system("rm -rf  /var/nic0_1");
	system("rm -rf  /var/nic0_2");
	system("rm -rf  /var/nic0_3");

//=============================================================================
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
    printf("=============================================================\n");
	return 0;
}

int DIAP_GET_COUNTRY()
{
	FILE *fp;
 	unsigned char buf[100];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;
    char *countrycode, *countryregion, *regionaband;

    printf("=============================================================\n");
    printf("UDPS =>  Get COUNTRY CODE\n");
    /* EZP */
    countrycode = nvram_safe_get("wl_countrycode");
    countryregion = nvram_safe_get("wl_region");
    regionaband = nvram_safe_get("wl_regionaband");
    if ((countrycode[0] == 'U') && (countrycode[1] == 'S')) {
        if((countryregion[0] == '0') && (regionaband[0] == '0')) {
            printf("\n US's Configuration is correct !!! \n");
            strcpy((char*)buf, "00");
        }
        else {
            printf("\n US's Configuration is wrong !!! \n");
            strcpy((char*)buf,"ff");
        }
    } else if ((countrycode[0] == 'F') && (countrycode[1] == 'R')) {
        if((countryregion[0] == '1') && (regionaband[0] == '2')) {
            printf("\n FR's Configuration is correct !!! \n");
            strcpy((char*)buf, "01");
        }
        else {
            printf("\n FR's Configuration is wrong !!! \n");
            strcpy((char*)buf,"ff");
        }
    } else if ((countrycode[0] == 'J') && (countrycode[1] == 'P')) {
        if((countryregion[0] == '5') && (regionaband[0] == '6')) {
            printf("\n FR's Configuration is correct !!! \n");
            strcpy((char*)buf, "05");
        }
        else {
            printf("\n FR's Configuration is wrong !!! \n");
            strcpy((char*)buf,"ff");
        }
    }else{	
        printf("\n NO Country info. Match !!!! Configuration is wrong !!! \n");
        strcpy((char*)buf,"ff");
    }

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
    printf("=============================================================\n");
	return 0;
}

int DIAP_GET_WLAN0_STATUS()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

       	printf("Get WLAN0 STATUS\n");
    /* EZP */
	system("nvram show wl_basic_rule 0 net_mode > /var/mode0");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/mode0","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);
	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<5;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/mode0");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN0_SSID()
{
	FILE *fp;
 	int iRet;
	char tempStr[50]="";
	char *p;

    printf("=============================================================\n");
    printf("UDPS =>  Get WLAN0 SSID\n");
    /* EZP */
	system("nvram show wl0_ssid_rule 0 ssid > /var/ssid0");

	fp = fopen("/var/ssid0","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,50,fp);
		printf("%s\n",tempStr);
	}
	fclose(fp);

	printf("SSID=%s\n",tempStr);
	system("rm /var/ssid0");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,tempStr,sizeof(tempStr));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
    printf("=============================================================\n");
	return 0;
}

int DIAP_GET_WLAN0_NAME()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;

       	printf("Get WLAN0 NAME\n");
	memset(buf,0,sizeof(buf));
	sprintf(buf,"There is no WLAN0 NAME");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN0_RATEMODE()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

       	printf("Get WLAN0 RateMode\n");
    /* EZP */
	system("flash get WLAN0_BASIC_RATES > /var/rate0");
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/rate0","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);
	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<4;i++)
		buf[i-1]=p[i];
//	printf("buf=%s\n",buf);
	system("rm /var/rate0");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN0_CHANNEL()
{
	FILE *fp;
 	int iRet;
	char tempStr[10]={0};
	char *p;

    printf("=============================================================\n");
    printf("UDPS =>  Get WLAN0 CHANNEL\n");

    /* EZP */
	system("nvram show wl_basic_rule 0 channel > /var/channel0");

	fp = fopen("/var/channel0","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,10,fp);
	}
	fclose(fp);

	printf("Channel=%s\n",tempStr);
	system("rm /var/channel0");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,tempStr,sizeof(tempStr));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_GET_WLAN0_COUNTRY()
{
       	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

       	printf("Get WLAN0 Country\n");
    /* EZP */
	system("echo \"0\" > /var/reg0");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/reg0","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<5;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/reg0");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN0_MAC()
{
	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

       	printf("Get WLAN0 MAC\n");
    /* EZP */
	system("flash get HW_WLAN0_MAC > /var/wlan0_addr");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/wlan0_addr","a+");
	if(fp==NULL)
	{
		printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<13;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/wlan0_addr");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
//     	printf("DIAP GET WLAN0 MAC:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_STATUS()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

    printf("Get WLAN1 STATUS\n");
    /* EZP */
	system("flash get WLAN1_MODE > /var/mode1");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/mode1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);
	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<5;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/mode1");

       	printf(buf);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_SSID()
{
	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

    printf("Get WLAN1 SSID\n");
    /* EZP */
	system("flash get WLAN1_SSID > /var/ssid1");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/ssid1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<13;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/ssid1");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_NAME()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;

       	printf("Get WLAN1 NAME\n");
	memset(buf,0,sizeof(buf));
	sprintf(buf,"There is no WLAN1 for this module !");


	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_RATEMODE()
{
	FILE *fp;
 	unsigned char buf[200];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

    printf("Get WLAN1 RateMode\n");
    /* EZP */
    system("flash get WLAN1_BASIC_RATES > /var/rate1");
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/rate1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);
	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<4;i++)
		buf[i-1]=p[i];
//	printf("buf=%s\n",buf);
	system("rm /var/rate1");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_CHANNEL()
{
	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

    printf("Get WLAN1 CHANNEL\n");
    /* EZP */
    system("flash get WLAN1_CHANNEL > /var/channel1");
	memset(buf,0,sizeof(buf));
	fp = fopen("/var/channel1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);
	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<4;i++)
		buf[i-1]=p[i];
//	printf("buf=%s\n",buf);
	system("rm /var/channel1");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_COUNTRY()
{
	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

   	printf("Get WLAN1 Country\n");
    /* EZP */
	system("flash get DEF_WLAN1_REG_DOMAIN > /var/reg1");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/reg1","a+");
	if(fp==NULL)
	{	printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<5;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/reg1");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_WLAN1_MAC()
{
	FILE *fp;
 	unsigned char buf[20];
 	int iRet;
	char tempStr[200]="";
	char *p;
	int i;

    printf("Get WLAN1 MAC\n");
    /* EZP */
	system("flash get HW_WLAN1_MAC > /var/wlan1_addr");
	memset(buf,0,sizeof(buf));

	fp = fopen("/var/wlan1_addr","a+");
	if(fp==NULL)
	{
		printf("Error open /var/stringlist\n");
	}
	else
	{
		fgets(tempStr,200,fp);
//		printf("%s\n",tempStr);
	}
	fclose(fp);

	p = strpbrk(tempStr,"=");
//	printf("%s\n",p);
	for(i=1;i<13;i++)
		buf[i-1]=p[i];

//	printf("buf=%s\n",buf);
	system("rm /var/wlan1_addr");

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
       	printf("DIAP GET WLAN1 MAC:%c%c:%c%c:%c%c:%c%c:%c%c:%c%c\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_FTP_ENABLE()
{

 	unsigned char buf[200];
 	int iRet;

       	printf("Get FTP Enable Status\n");
	memset(buf,0,sizeof(buf));
	sprintf(buf,"not support");

       	printf(buf);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_FTP_ANONYMOUS()
{
    DBGPRINT("UDPS =>  Get FTP Anonymous Status |\n");
	return 0;
}

int DIAP_GET_FTP_ROOTDIR()
{
 	DBGPRINT("UDPS =>  Get FTP Enable Root Dir |\n");

	return 0;
}

int DIAP_GET_FTP_SERVERPORT()
{
	DBGPRINT("UDPS =>  Get FTP Server Port |\n");

	return 0;
}



int Detect_USB() {

	FILE *fp;
  int cnt=0;
  unsigned char strBuf[256]={0};
  unsigned char *ptr;
  unsigned char str_vendor[5]={"12d1"};
  unsigned char str_device[5]={"1001"};
  
	fp = fopen("/proc/bus/usb/devices","r");
	if(fp == NULL) return -1;
  /*Search exist */
  /*P:  Vendor=12d1 ProdID=1001 Rev= 0.00*/
  while(fgets(strBuf,256,fp) != NULL) {

		if(strstr(strBuf,"Vendor")) cnt += 1;
    if(cnt == EZP_USB_FIRST_EXTDEV_NUM){
       ptr = strstr(strBuf,"Vendor");
       ptr += 7;
       strncpy(str_vendor,ptr,4);
       ptr = strstr(strBuf,"ProdID");
       ptr += 7;
       strncpy(str_device,ptr,4); 
       break;
    }
  }
  fclose(fp);
  return cnt; 
}


int DIAP_GET_USB_INFORMATION()
{
 	int iRet;
 	unsigned char buf[200];
  DBGPRINT("UDPS =>  Get USB Information |\n");
	memset(buf,0,sizeof(buf));
	
	if(Detect_USB() == EZP_USB_FIRST_EXTDEV_NUM) {
		sprintf(buf,"support");
	}
	else { 
	    sprintf(buf,"not support");
  }
  printf(buf);
    
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
  printf("\nSend Packet iRet=%08x \n",iRet);	

	return 0;
}

#ifdef CONFIG_RT2880_GPIO_PCI_WIXM
#include <fcntl.h>
#define	RT2880GPIO_WRITE_BIT2		0x15

int gpio_write_bit2(int idx, int value)
{
	int fd, req;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return -1;
	}
	value &= 1;
	if (0L <= idx && idx < 32)
		req = RT2880GPIO_WRITE_BIT2 | (idx << 24);
	else {
		close(fd);
		printf("gpio_write_bit2: index %d out of range\n", idx);
		return -1;
	}
	if (ioctl(fd, req, value) < 0) {
		perror("ioctl");
		close(fd);
		return -1;
	}
	close(fd);
}

#else

#include <fcntl.h>
#define RT2880GPIO_DATA_LEN		24
#define	RT2880GPIO_WRITE_BIT		0x05
int gpio_write_bit(int idx, int value)
{
	int fd, req;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return -1;
	}
	value &= 1;
	if (0L <= idx && idx < RT2880GPIO_DATA_LEN)
		req = RT2880GPIO_WRITE_BIT | (idx << RT2880GPIO_DATA_LEN);
	else {
		close(fd);
		printf("gpio_write_bit: index %d out of range\n", idx);
		return -1;
	}
	if (ioctl(fd, req, value) < 0) {
		perror("ioctl");
		close(fd);
		return -1;
	}
	close(fd);
}


#endif


int DIAP_GET_LEDTEST_GREEN()
{
	unsigned char buf[100];
	unsigned char cmd[100];
 	int iRet;

  //Perish added for test WPS LED for WR5204 2009/1/20
  printf("=============================================================\n");
  printf("UDPS => DIAP GET LEDTEST\n");
  
  /* EZP */
  sprintf(cmd, "gpioctl led %d 4000 0 1 0 4000", WPS_LED);
  system(cmd);
  printf("%s\n", cmd);

	sprintf(buf,"LED TEST START");
    printf("%s\n", buf);
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

int DIAP_GET_LEDTEST_AMBER()
{
	unsigned char buf[100];
 	int iRet;

	sprintf(buf,"DIAP_GET_LEDTEST_AMBER not support now");
      	printf(buf);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
     	printf("\nSend Packet iRet=%08x \n",iRet);
	return 0;
}

unsigned char HEXTODEC(unsigned char ch)
{
	/* 判斷是否為數字0-9 [ascii-> 30~39]     */
	if(ch >= 0x30 && ch <= 0x39)
	{
		return (ch-0x30);
	}

	/* 判斷是否為十六進制的 a ~ f */
	if(ch >= 'A' && ch <= 'F' )
	{
		return (ch-0x41+10);
	}
	/* 判斷是否為十六進制的 a ~ f */
	if(ch >= 'a' && ch <= 'f' )
	{
		return (ch-0x61+10);
	}
	return 0;

}



/*Steve add*/
int DIAP_GET_CARD0_WDS_MAC()
{
	int iRet;
	unsigned char buf[20]="";
	unsigned char mac[20]="";
	unsigned char mac1[4]="";
	unsigned char mac2[4]="";
	unsigned char mac3[4]="";
	FILE *fp;
	char tempStr[200]="";
	char *p;
	int i;

	DBGPRINT("UDPS =>  Get WDS MAC |\n");
  
	return 0;
}

/* Function: DIAP_SET_MAC()
	 Purpose:	init socket config and setting UDP protol
	 Return: 0:success  1:fail
*/
int DIAP_SET_CARD0_WDS_MAC(unsigned char *mac)
{
	unsigned char buf[76]="";
	unsigned char cmd_ra0[80]="";

	unsigned char cmd_set[80]="";

	int iRet;
	int i;
	printf("=============================================================\n");
	printf("UDPS => SET WDS MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));

	sprintf(buf,"SET WDS MAC not support: %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac+0),*(mac+1),*(mac+2),*(mac+3),*(mac+4),*(mac+5));
	
	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	strcpy((char*)DIAP_CONTEXT,buf);
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	printf("Send Packet iRet=%08x \n",iRet);
	printf("=============================================================\n");
	return 0;
}

int DIAP_GET_MODE()
{
	FILE *fp=NULL;
 	unsigned char buf[20];
 	int iRet;
 	char c;

 	printf("=============================================================\n");
	printf("UDPS =>  Get Mode\n");
	memset(buf,0,sizeof(buf));

	//Perish added for WR5204 2009/1/22
  fp = fopen("/proc/LSW", "r");
  if(fp == NULL)
  	sprintf(buf, "%s", "NO_SUPPORT");
  else{
  	c = fgetc(fp);
  	fclose(fp);
  	
  	if(c == '1')
  		sprintf(buf, "%s", "AP_MODE");
  	else if(c == '2')
  		sprintf(buf, "%s", "GW_MODE");
  	else if(c == '3')
  		sprintf(buf, "%s", "WISP_MODE");
  	else 
  		sprintf(buf, "%s", "NO_SUPPORT");
	}
  
	printf("Mode : %s\n",buf);

	memset(DIAP_CONTEXT,0,sizeof(DIAP_CONTEXT));
	memcpy(DIAP_CONTEXT,buf,sizeof(buf));
	DIAP_PACKET_GEN();
	iRet=sendto(sockfd, DIAP_PACKET, sizeof(DIAP_PACKET), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
  printf("Send Packet iRet=%08x \n",iRet);
  printf("=============================================================\n");
	return 0;
}
