/*
   Mode switching tool for controlling flip flop (multiple device) USB gear
   Version 0.9.6, 2009/01/08

   Copyright (C) 2007, 2008, 2009 Josua Dietze (mail to "usb_admin" at the
   domain from the README; please do not post the complete address to The Net!
   Or write a personal message through the forum to "Josh")

   Command line parsing, decent usage/config output/handling, bugfixes and advanced
   options added by:
   Joakim Wennergren (jokedst) (gmail.com)

   TargetClass parameter implementation to support new Option devices/firmware:
   Paul Hardwick (http://www.pharscape.org)

   Created with initial help from:
   "usbsnoop2libusb.pl" by Timo Lindfors (http://iki.fi/lindi/usb/usbsnoop2libusb.pl)

   Config file parsing stuff borrowed from:
   Guillaume Dargaud (http://www.gdargaud.net/Hack/SourceCode.html)

   Hexstr2bin function borrowed from:
   Jouni Malinen (http://hostap.epitest.fi/wpa_supplicant, from "common.c")

   Code, fixes and ideas from:
   Aki Makkonen
   Denis Sutter 
   Lucas Benedicic 
   Roman Laube 
   Vincent Teoh
   Tommy Cheng

   More contributors are listed in the config file.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details:

http://www.gnu.org/licenses/gpl.txt

*/

/* Recommended tab size: 4 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>
#include <usb.h>
#include <usb.h>
#include <unistd.h>
#include <sys/wait.h>
#include "usb_modeswitch.h"
#include <sys/types.h>
#include <sys/stat.h>
#define EZPACKET
#ifdef EZPACKET
#include "bcmnvram.h"
#include "ezp-lib.h"
#include "ezpcom-lib.h"
#endif

#define LINE_DIM 1024

int write_bulk(int endpoint, char *message, int length);
int read_bulk(int endpoint, char *buffer, int length);

char *TempPP=NULL;

struct usb_dev_handle *devh;

int DefaultVendor=0, DefaultProduct=0, TargetVendor=0, TargetProduct=0, TargetClass=0;
int MessageEndpoint=0, ResponseEndpoint=-1;
char DetachStorageOnly=0, HuaweiMode=0, SierraMode=0, SonyMode=0, SonyMode1 = 0,
     BandluxeMode=0, QisdaMode=0, QuantaMode=0, TouchMode=0, QMIMode=0;
char verbose=0, show_progress=1, ResetUSB=0;
char MessageContent[LINE_DIM];
// Settable Interface and Configuration (for debugging mostly) (jmw)
int Interface = 0, Configuration = -1, AltSetting = -1;
char Hotplug = 0;

#ifdef EZPACKET
/* EZP Security IC. */
#define EZP_SEC_READ_NUM_REGS 4
#define EZP_SEC_KEY         { 0xAA, 0x19, 0x09, 0xBC }
unsigned char ezp_sec_key[4] = EZP_SEC_KEY;

void ezp_sec_expect(char *src, char *expect)
{
    int i;
    char data[EZP_SEC_READ_NUM_REGS];
    for (i = 0; i < 16; i++) {
        switch (src[i]%4) {
            case 1:
                data[0] = *(src+i);
                break;
            case 2:
                data[1] = *(src+i);
                break;
            case 3:
                data[2] = *(src+i);
                break;
            case 0:
                data[3] = *(src+i);
                break;
        }
    }

    expect[0] = ((~data[0])^data[1]^(~data[2])^data[3])+ezp_sec_key[0];
    expect[1] = (data[0]^(~data[1])^data[2]^(~data[3]))+ezp_sec_key[1];
    expect[2] = ((~data[0])^data[1]^data[2]^data[3])+ezp_sec_key[2];
    expect[3] = (data[0]^data[1]^(~data[2])^data[3])+ezp_sec_key[3];
}

void ezp_sec_shuffle(char *src)
{
    int i, j;
    int pos[EZP_SEC_READ_NUM_REGS];
    pos[0] = pos[1] = pos[2] = pos[3] = -1;
    for (i = 0; i < 16; i++) {
        switch (src[i] % 4) {
            case 1:
                pos[0] = i;
                break;
            case 2:
                pos[1] = i;
                break;
            case 3:
                pos[2] = i;
                break;
            case 0:
                pos[3] = i;
                break;
        }
    }
    for (i = 0; i < 4; i++) {
        /* Find out the missing position and fill up the value. */
        if (pos[i] == -1) {
            for (j = 0; j < 15; j++) {
                if ((j != pos[(i + 1) % 4]) && (j != pos[(i + 2) % 4]) &&
                        (j != pos[(i + 3) % 4])) {
                    pos[i] = j;
                    switch (i) {
                        case 0:
                            src[j] = ((src[j] >> 2) << 2) | 0x01;
                            break;
                        case 1:
                            src[j] = ((src[j] >> 2) << 2) | 0x02;
                            break;
                        case 2:
                            src[j] = ((src[j] >> 2) << 2) | 0x03;
                            break;
                        case 3:
                            src[j] = ((src[j] >> 2) << 2);
                            break;
                    }
                    break;
                }
            }
        }
    }
#ifdef DEBUG
    for (i = 0; i < 16; i++) {
        switch (src[i] % 4) {
            case 1:
                pos[0] = i;
                break;
            case 2:
                pos[1] = i;
                break;
            case 3:
                pos[2] = i;
                break;
            case 0:
                pos[3] = i;
                break;
        }
    }
    printf("P2[%d,%d,%d,%d]\n", pos[0], pos[1], pos[2], pos[3]) ;
#endif
}
int ezp_sec_auth_app(void)
{
    FILE *fp;
    char write[16];
    char read[4], expect[4];
    long int rn;
    int i;

    /* ezp-sec */
    for (i = 0; i < 4; i++) {
        rn = rand();
#ifdef DEBUG
        printf("%X\n", rn);
#endif
        memcpy(write + 4*i, &rn, sizeof(rn));
    }

    ezp_sec_shuffle(write);
    fp = fopen ("/proc/ezp_sec", "w") ;
    if( fp == NULL )
    {
        printf("cannot write\n" ) ;
        exit(1) ;
    }
    fwrite(write, 1, 16, fp);
    fclose(fp);

    fp = fopen ("/proc/ezp_sec", "r") ;
    if( fp == NULL )
    {
        printf("cannot read\n" ) ;
        exit(1) ;
    }
    fread(read, 1, 4, fp);
    fclose(fp);

#ifdef DEBUG
    printf("R[%02X %02X %02X %02X]%d\n", read[0], read[1], read[2], read[3], 
            sizeof(rn));
#endif
    ezp_sec_expect(write, expect);

#ifdef DEBUG
    printf("R2[%02X %02X %02X %02X]\n", expect[0], expect[1], expect[2], expect[3]);
#endif

    /* Compare. */
    if ( expect[0] != read[0] || expect[1] != read[1] || 
            expect[2] != read[2] || expect[3] != read[3] ) {
        return -1;
    } else {
        return 0;
    }
}
#endif

static struct option long_options[] =
{
    {"help",				no_argument, 0, 'h'},
    {"DefaultVendor",		required_argument, 0, 'v'},
    {"DefaultProduct",		required_argument, 0, 'p'},
    {"TargetVendor",		required_argument, 0, 'V'},
    {"TargetProduct",		required_argument, 0, 'P'},
    {"TargetClass",			required_argument, 0, 'C'},
    {"MessageEndpoint",		required_argument, 0, 'm'},
    {"MessageContent",		required_argument, 0, 'M'},
    {"ResponseEndpoint",	required_argument, 0, 'r'},
    {"DetachStorageOnly",	required_argument, 0, 'd'},
    {"HuaweiMode",			required_argument, 0, 'H'},
    {"SierraMode",			required_argument, 0, 'H'},
    {"SonyMode",			required_argument, 0, 'H'},
    {"SonyMode1",			required_argument, 0, 'H'},
    {"BandluxeMode",		required_argument, 0, 'H'},
    {"QisdaMode",	    	required_argument, 0, 'H'},
    {"QuantaMode",	    	required_argument, 0, 'H'},
    {"TouchMode",	    	required_argument, 0, 'H'},
    {"QMIMode",			    required_argument, 0, 'x'},
    {"NeedResponse",		required_argument, 0, 'n'},
    {"ResetUSB",			required_argument, 0, 'R'},
    {"config",				required_argument, 0, 'c'},
    {"verbose",				no_argument, 0, 'W'},
    {"quiet",				no_argument, 0, 'Q'},
    {"Interface",			required_argument, 0, 'i'},
    {"Configuration",		required_argument, 0, 'u'},
    {"AltSetting",			required_argument, 0, 'a'},
    {0, 0, 0, 0}
};

void readConfigFile(const char *configFilename)
{
    ParseParamHex(configFilename, TargetVendor);
    ParseParamHex(configFilename, TargetProduct);
    ParseParamHex(configFilename, TargetClass);
    ParseParamHex(configFilename, DefaultVendor);
    ParseParamHex(configFilename, DefaultProduct);
    ParseParamBool(configFilename, DetachStorageOnly);
    ParseParamBool(configFilename, HuaweiMode);
    ParseParamBool(configFilename, SierraMode);
    ParseParamBool(configFilename, SonyMode);
    ParseParamBool(configFilename, SonyMode1);
    ParseParamBool(configFilename, BandluxeMode);
    ParseParamBool(configFilename, QisdaMode);
    ParseParamBool(configFilename, QuantaMode);
    ParseParamBool(configFilename, TouchMode);
    ParseParamBool(configFilename, QMIMode);
    ParseParamHex(configFilename, MessageEndpoint);
    ParseParamString(configFilename, MessageContent);
    //    ParseParamHex(configFilename, NeedResponse);
    ParseParamHex(configFilename, ResponseEndpoint);
    ParseParamHex(configFilename, ResetUSB);
    ParseParamHex(configFilename, Interface);
    ParseParamHex(configFilename, Configuration);
    ParseParamHex(configFilename, AltSetting);
}

void printConfig()
{
    printf ("DefaultVendor=0x%x\n",		DefaultVendor);
    printf ("DefaultProduct=0x%x\n",	DefaultProduct);
    printf ("TargetVendor=0x%x\n",		TargetVendor);
    printf ("TargetProduct=0x%x\n",		TargetProduct);
    printf ("TargetClass=0x%x\n",		TargetClass);
    printf ("DetachStorageOnly=%i\n",	(int)DetachStorageOnly);
    printf ("HuaweiMode=%i\n",			(int)HuaweiMode);
    printf ("SierraMode=%i\n",			(int)SierraMode);
    printf ("SonyMode=%i\n",			(int)SonyMode);
    printf ("SonyMode1=%i\n",			(int)SonyMode1);
    printf ("BandluxeMode=%i\n",		(int)BandluxeMode);
    printf ("QisdaMode=%i\n",		    (int)QisdaMode);
    printf ("QuantaMode=%i\n",		    (int)QuantaMode);
    printf ("TouchMode=%i\n",		    (int)TouchMode);
    printf ("QMIMode=%i\n",		        (int)QMIMode);
    printf ("MessageEndpoint=0x%x\n",	MessageEndpoint);
    printf ("MessageContent=\"%s\"\n",	MessageContent);
    //	printf ("NeedResponse=%i\n",		(int)NeedResponse);
    if ( ResponseEndpoint > -1 )
        printf ("ResponseEndpoint=0x%x\n",	ResponseEndpoint);
    printf ("Interface=0x%x\n",	Interface);
    if ( Configuration > -1 )
        printf ("Configuration=0x%x\n",	Configuration);
    if ( AltSetting > -1 )
        printf ("AltSetting=0x%x\n",	AltSetting);
    printf ("\n");
}

int readArguments(int argc, char **argv)
{
    int c, option_index = 0, count=0;
    if(argc==1) return 0;

    while (1)
    {
        c = getopt_long (argc, argv, "hWQv:p:V:P:C:m:M:r:d:H:S:O:n:c:R:i:u:a:",
                long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;
        count++;
        switch (c)
        {
            case 'R': ResetUSB = strtol(optarg, NULL, 16); break;
            case 'v': DefaultVendor = strtol(optarg, NULL, 16); break;
            case 'p': DefaultProduct = strtol(optarg, NULL, 16); break;
            case 'V': TargetVendor = strtol(optarg, NULL, 16); break;
            case 'P': TargetProduct = strtol(optarg, NULL, 16); break;
            case 'C': TargetClass = strtol(optarg, NULL, 16); break;
            case 'm': MessageEndpoint = strtol(optarg, NULL, 16); break;
            case 'M': strcpy(MessageContent, optarg); break;
            case 'r': ResponseEndpoint = strtol(optarg, NULL, 16); break;
            case 'd': DetachStorageOnly = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'H': HuaweiMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'S': SierraMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'O': SonyMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'T': SonyMode1 = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'B': BandluxeMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'q': QisdaMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 't': QuantaMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'J': TouchMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'x': QMIMode = (toupper(optarg[0])=='Y' || toupper(optarg[0])=='T'|| optarg[0]=='1'); break;
            case 'n': break;
            case 'c': readConfigFile(optarg); break;
            case 'W': verbose=1; count--; break;
            case 'Q': show_progress=1; count--; break;

            case 'i': Interface = strtol(optarg, NULL, 16); break;
            case 'u': Configuration = strtol(optarg, NULL, 16); break;
            case 'a': AltSetting = strtol(optarg, NULL, 16); break;

            case 'h':
                      printf ("Usage: usb_modeswitch [-hvpVPmMrdHn] [-c filename]\n\n");
                      printf (" -h, --help                    this help\n");
                      printf (" -v, --DefaultVendor [nr]      set vendor number to look for\n");
                      printf (" -p, --DefaultProduct [nr]     set model number to look for\n");
                      printf (" -V, --TargetVendor [nr]       target vendor; if found, quit\n");
                      printf (" -P, --TargetProduct [nr]      target model, if found quit\n");
                      printf (" -C, --TargetClass [nr]        target device class, if found quit\n");
                      printf (" -m, --MessageEndpoint [nr]    where to write message\n");
                      printf (" -M, --MessageContent [nr]     message to send\n");
                      //				printf (" -n, --NeedResponse [1|0]      whether to try to read a response - OBSOLETE\n");
                      printf (" -r, --ResponseEndpoint [nr]   if given, read response from there\n");
                      printf (" -d, --DetachStorageOnly [1|0] whether to just detach the storage driver\n");
                      printf (" -H, --HuaweiMode [1|0]        whether to just apply a special sequence\n");
                      printf (" -S, --SierraMode [1|0]        whether to just apply a special sequence\n");
                      printf (" -O, --SonyMode [1|0]          whether to just apply a special sequence\n");
                      printf (" -T, --SonyMode [1|0]          whether to just apply a special sequence\n");
                      printf (" -B, --BandluxeMode [1|0]      whether to just apply a special sequence\n");
                      printf (" -q, --QisdaMode [1|0]         whether to just apply a special sequence\n");
                      printf (" -q, --QuantaMode [1|0]         whether to just apply a special sequence\n");
                      printf (" -J, --TouchMode [1|0]         whether to just apply a special sequence\n");
                      printf (" -x, --QMIMode [1|0]           whether to just apply a special sequence\n");
                      printf (" -R, --ResetUSB [1|0]          whether to reset the device in the end\n");
                      printf (" -c, --config [filename]       load different config file\n");
                      printf (" -Q, --quiet                   don't show progress or error messages\n");
                      printf (" -W, --verbose                 print all settings before running\n\n");
                      printf (" -i, --Interface               select initial USB interface (default 0)\n");
                      printf (" -u, --Configuration           select USB configuration\n");
                      printf (" -a, --AltSetting              select alternative USB interface setting\n\n");
                      exit(0);
                      break;

            default: //Unsupported - error message has already been printed
                      printf ("\n");
                      exit(1);
        }
    }

    return count;
}


int main(int argc, char **argv) {
    int ret, message_length;
    int numTargets = 0;
    int numDefaults = 0;
    char buf[65535], tmp_proto[16], tmp_ip[32], tmp[128];
    int i, j, found, wan_num, brand, model;
    char Mode;

    struct usb_device *dev;
    char ByteString[LINE_DIM/2];

#ifndef EZPACKET
    printf("\n * usb_modeswitch: tool for controlling \"flip flop\" mode USB devices\n");
    printf(" * Version 0.9.6 (C) Josua Dietze 2009\n");
    printf(" * Works with libusb 0.1.12 and probably other versions\n\n");
#endif

#ifdef EZPACKET
    /* ezp-sec auth */
#ifndef EZP_SEC_NO_AUTH
    if (ezp_sec_auth_app()) {
        printf("ezp-sec auth app failed\n");
        return 0;
    }
#endif
#elif
    // Check command arguments, use them instead of config file if possible
    switch (readArguments(argc, argv)) {
        case 1: 					// only one argument
            if (!verbose && show_progress) // the only arguments usable with the default config file
                break; 				// just one argument and none of the flags above -> MUST be -c
        case 0:						// no argument or -W or -q
            readConfigFile("/etc/usb_modeswitch.conf");
            break;
        default:					// all params must come as arguments
            ;
    }
#endif

    //	if (!show_progress) printf(" Quiet mode, no progress shown\n\n");

    if(verbose)
        printConfig();

    usb_init();

    if (verbose)
        usb_set_debug(15);

    usb_find_busses();
    usb_find_devices();

    if (verbose)
        printf("\n");

#ifdef EZPACKET
    i = 0;
    j = 0;
    found = 0;

    /* EZP: 
     * (1) If the auto mode is set, we keep going.
     * (2) If the manual mode is set, we verified the nvram.
     */
    wan_num = atoi(nvram_safe_get("wan_num")) - 1;

    /* EZP: WAN iteration. */
        printf("ezpcom-mode: wan%d iteration\n", wan_num);

#ifdef EZP_DEBUG
        /* WAN proto */
        snprintf(tmp_proto, sizeof(tmp_proto), "wan%d_proto", wan_num);
        if (strcmp(nvram_safe_get(tmp_proto), "wwan")) {
            printf("ezpcom-mode: wan%d_proto != wwan\n", wan_num);
            continue;
        }

        /* This WAN iface already gets ipaddr. */
        snprintf(tmp_ip, sizeof(tmp_ip), "wan%d_ipaddr", wan_num);
        if (strcmp(nvram_safe_get(tmp_ip), "")) {
            printf("ezpcom-mode: wan%d_ipaddr != null\n", wan_num);
            continue;
        }
#endif

        /* auto or manual. */
        ezplib_get_attr_val("wan_wwan_rule", wan_num, "brand", 
                tmp, sizeof(tmp), 0);
        brand = atoi(tmp);
        ezplib_get_attr_val("wan_wwan_rule", wan_num, "model", 
                tmp, sizeof(tmp), 0);
        model = atoi(tmp);

        
    /* EZP: We start to iterate all of the models here. */
    for (; strlen(ezp_com_vendor_map[i].desc); i++) {
        struct ezp_com_map_entry_type *map_ptr;
        for (j = 0, map_ptr = ezp_com_pointer_map[i]; 
                strlen(map_ptr[j].desc); j++)  {

#ifdef DEBUG
            printf("Begin: [%s] [%s] %d %d %d\n", 
                    ezp_com_vendor_map[i].desc, map_ptr[j].desc,
                    map_ptr[j].DeviceNum, map_ptr[j].DataTTY,
                    map_ptr[j].CtrlTTY);
#endif

        if (brand == 0 && model == 0) {
            /* auto */
            printf("ezpcom-mode: auto mode [wan%d]([%s(%d)][%s(%d)]\n", wan_num, ezp_com_vendor_map[i].desc, i, map_ptr[j].desc, j);
        } else {
            /* manual */
            if (brand != i || (!strstr(map_ptr[j].desc, "HIDDEN") && model != j)) {
                continue;
            }
            printf("ezpcom-mode: manual mode [wan%d][%s(%d)][%s(%d)]\n", wan_num, 
                    ezp_com_vendor_map[i].desc, i, map_ptr[j].desc, j);
        }

        /* XXX: We come in here in the case of the auto mode or 
         * the selected brand/model in manual mode. */

        DefaultVendor = ezp_com_vendor_map[i].DefaultVendor;
        DefaultProduct = map_ptr[j].DefaultProduct;

        TargetVendor = 
            ezp_com_vendor_map[map_ptr[j].TargetVendorType].DefaultVendor;
        TargetProduct = map_ptr[j].TargetProduct;
        TargetClass = map_ptr[j].TargetClass;

        MessageEndpoint = map_ptr[j].MessageEndpoint;
        ResponseEndpoint = map_ptr[j].ResponseEndpoint;
        DetachStorageOnly = map_ptr[j].DetachStorageOnly;
        Interface = map_ptr[j].Interface;
        Hotplug = map_ptr[j].Hotplug;

        if (map_ptr[j].MessageContent) {
            strcpy(MessageContent, map_ptr[j].MessageContent);
        } else {
            MessageContent[0]='\0';
        }

        /* We use the 7 bits for mode. */
        Mode = map_ptr[j].Mode & 0x7F;
        HuaweiMode = 0;
        SierraMode = 0;
        SonyMode = 0;
        SonyMode1 = 0;
        BandluxeMode = 0;
        QisdaMode = 0;
        QuantaMode = 0;
        TouchMode = 0;
        QMIMode = 0;
        if (Mode == 1) {
            /* Huawei */
            HuaweiMode = 1;
        } else if (Mode == 2) {
            /* Sierra */
            SierraMode = 1;
        } else if (Mode == 3) {
            /* Sony */
            if (!strcmp(map_ptr[j].desc, "MD300")) {
                /* MD 300 */
                SonyMode1 = 1;
            } else {
                /* others */
                SonyMode = 1;
            }
        } else if (Mode == 4) {
            /* Bandluxe */
            BandluxeMode = 1;
        } else if (Mode == 5) {
            /* Qisda */
            QisdaMode = 1;
        } else if (Mode == 6) {
            /* QuantaMode */
            QuantaMode = 1;
        } else if (Mode == 7) {
            /* TouchMode */
            TouchMode = 1;
        } else if (QMIMode == 1) {
            /* QMIMode */ 
           goto found_but_no_switching;
        }

        /* We use the leftmost bit for reset. */
        ResetUSB = 0;
        if (map_ptr[j].Mode & 0x80) {
            ResetUSB = 1;
        } 

        numTargets = 0;
        numDefaults = 0;
#endif

        if (show_progress) printf("Mode:%d HuaweiMode:%d SierraMode:%d SonyMode1:%d SonyMode:%d BandluxeMode:%d QisdaMode:%d QuantaMode:%d TouchMode:%d\n",
                Mode,HuaweiMode, SierraMode, SonyMode1, SonyMode, BandluxeMode, QisdaMode, QuantaMode, TouchMode);
        if (!(DefaultVendor && DefaultProduct)) {
            if (show_progress) printf("No default vendor/product ID given. Aborting\n\n");
            exit(1);
        }

        if (TargetVendor && TargetProduct) {
            if (show_progress) printf("Looking for target devices %08X, %08X\n", TargetVendor, TargetProduct);
        dev = search_devices(&numTargets, TargetVendor, TargetProduct, TargetClass);
            if (numTargets) {
                if (show_progress) printf(" Found target devices (%d)\n", numTargets);
            } else {
                if (show_progress) printf(" No target device found\n");
            }
            if (dev) {
                goto found_but_no_switching;
            }
        }

        /* Check the hotplug point! */
        char *invoke;
        invoke = getenv("EZPHOTPLUG");
        if (invoke && *invoke && !strcmp(invoke, "usb")) {
            if (!(Hotplug & HOTPLUG_USB)) {
                /* Don't try this device. */
                continue;
            }
        } else if (invoke && *invoke && !strcmp(invoke, "scsi_generic")) {
            if (!(Hotplug & HOTPLUG_SCSI_GENERIC)) {
                /* Don't try this device. */
                continue;
            }
        } else {
                /* If not specified, then always execute! */
        }

        if (show_progress) printf("Looking for default devices %08X, %08X\n", DefaultVendor, DefaultProduct);
        dev = search_devices(&numDefaults, DefaultVendor, DefaultProduct, TargetClass);

        if (numDefaults) {
            if (show_progress) printf(" Found default devices (%d)\n", numDefaults);
            if (TargetClass > 0) {
                if ( dev != NULL ) {
                    if (show_progress) printf(" OK, found default device not in target class mode\n");
                } else {
                    if (show_progress) printf(" All devices in target class mode\n");
                }
            }
        }
        if (dev != NULL) {
            if (show_progress) printf("Prepare switching, accessing latest device\n");
        } else {
            if (show_progress) printf(" No default device found. Is it connected? Bye\n\n");
#ifdef EZPACKET
            continue;
#else
            exit(0);
#endif
        }

        devh = usb_open(dev);
        if (devh == NULL) {
            if (show_progress) fprintf(stderr, "Error: could not access device. Aborting\n\n");
            exit(1);
        }

        signal(SIGTERM, release_usb_device);

#ifdef LIBUSB_HAS_GET_DRIVER_NP
        if ( !HuaweiMode && !SierraMode && !SonyMode && !BandluxeMode && 
                !QisdaMode && !QuantaMode && !TouchMode) {
            if (show_progress) printf("Looking for active default driver to detach it\n");
            ret = usb_get_driver_np(devh, Interface, buf, sizeof(buf));
            if (ret == 0) {
                if (show_progress) printf(" OK, driver found (\"%s\")\n", buf);
                if ( (DetachStorageOnly && !strcmp(buf,"usb-storage")) || !DetachStorageOnly ) {
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
                    ret = usb_detach_kernel_driver_np(devh, Interface);
                    if (ret == 0) {
                        if (show_progress) printf(" OK, Driver \"%s\" successfully detached\n", buf);
                    } else {
                        if (show_progress) printf(" Driver \"%s\" detach failed with error %d. Trying to continue ...\n", buf, ret);
                    }
#else
                    if (show_progress) printf(" Driver detaching not possible on this platform. ");
                    if (DetachStorageOnly) {
                        if (show_progress) printf("Please remove USB storage driver manually! Bye\n\n");
                        exit(1);
                    }
                    else
                        if (show_progress) printf("Trying to continue ...\n");
#endif
                } else {
                    if (show_progress) printf(" No usb-storage driver found. Switching not necessary. Bye\n\n");
                    exit(0);
                }
            } else {
                if (DetachStorageOnly) {
                    if (show_progress) printf(" No driver found. Driver probably detached already. Bye\n\n");
                    exit(0);
                } else
                    if (show_progress) printf(" No driver found. Device probably not initialized. Trying to continue ...\n");
            }
        }
#else
        printf(" Driver detection not possible on this platform. Trying to continue ...\n");
#endif

        if (!DetachStorageOnly && !HuaweiMode && !SierraMode && !SonyMode && !SonyMode1 &&
                !BandluxeMode && !QisdaMode && !QuantaMode && !TouchMode) {
            if (show_progress) printf("Setting up communication with device\n");

            if (Configuration != -1) {
                if (show_progress) printf("Setting configuration %i\n", Configuration);
                ret = usb_set_configuration(devh, Configuration);
                if (ret == 0 ) {
                    if (show_progress) printf(" OK, configuration was successfully set\n");
                } else {
                    if (show_progress) printf(" Setting the configuration returned error %d, trying to continue ...\n", ret);
                }
            }
            ret = usb_claim_interface(devh, Interface);

            if (ret != 0) {
                if (show_progress) fprintf(stderr, "Error: couldn't claim interface (error %d). Can't communicate. Aborting\n\n", ret);
                exit(1);
            }
            if (AltSetting != -1) {
                if (show_progress) printf("Setting alt setting %i\n", AltSetting);
                ret = usb_set_altinterface(devh, AltSetting);
                if (ret != 0) {
                    if (show_progress) fprintf(stderr, "Error: could not set alt setting (error %d). Aborting\n\n", ret);
                    exit(1);
                }
            }
            ret = usb_clear_halt(devh, MessageEndpoint);

            if (strlen(MessageContent) && !MessageEndpoint) {
                if (show_progress) fprintf(stderr, "Error: no MessageEndpoint given. Can't send message. Aborting\n\n");
                exit(1);
            }

            if (strlen(MessageContent) % 2 != 0) {
                if (show_progress) fprintf(stderr, "Error: MessageContent hex string has uneven length. Aborting\n\n");
                exit(1);
            }

            message_length = strlen(MessageContent) / 2;
            if (!message_length) {
                if (show_progress) printf("No MessageContent given. Sending nothing\n\n");
                exit(1);
            } else {
                if (hexstr2bin(MessageContent,ByteString,message_length) == -1) {
                    if (show_progress) fprintf(stderr, "Error: MessageContent: %s\n is not a hex string. Aborting\n\n", MessageContent);
                    exit(1);
                }

                if (show_progress) printf("Trying to send the message\n");
                write_bulk(MessageEndpoint, ByteString, message_length);

                if (ResponseEndpoint > -1) {
                    if (show_progress) printf("Reading the response\n");
                    read_bulk(ResponseEndpoint, ByteString, LINE_DIM/2);
                }
            }

            /* you might try this again if device still does not switch */

            		usb_clear_halt(devh, MessageEndpoint);

        }	

        if ( HuaweiMode ) {
            /* Huawei E169/E169U is only switched in scsi_generic event! */
            char *invoke;
            invoke = getenv("EZPHOTPLUG");
            if (invoke && *invoke && !strcmp(invoke, "scsi_generic")) {
                ret = usb_control_msg(devh, USB_TYPE_STANDARD + USB_RECIP_DEVICE, USB_REQ_SET_FEATURE, 00000001, 0, buf, 0, 1000);
                if (ret != 0) {
                    if (show_progress) fprintf(stderr, "Error: sending Huawei control message failed (error %d). Aborting\n\n", ret);
                    exit(1);
                } else
                    if (show_progress) printf(" OK, Huawei control message successfully sent.\n");
            }
        }

        if ( SierraMode ) {
            ret = usb_control_msg(devh, 0x40, 0x0b, 00000001, 0, buf, 0, 1000);
            if (ret != 0) {
                if (show_progress) fprintf(stderr, "Error: sending Sierra control message failed (error %d). Aborting\n\n", ret);
                exit(1);
            } else
                if (show_progress) printf(" OK, Sierra control message successfully sent.\n");
        }

        if ( SonyMode ) {
            buf[0] = 0x5a;
            buf[1] = 0x11;
            buf[2] = 0x02;
            ret = usb_control_msg(devh, 0xc0, 0x11, 00000002, 0, buf, 3, 1000);
        }
        
        if ( SonyMode1 ) {
            FILE *fp;
            int bconfval;
            if ((fp = popen("cat /sys/devices/lm0/usb1/1-1/bConfigurationValue", "r"))) {
                fscanf(fp, "%d", &bconfval);
                if (bconfval != 3) {
                    ret = system("echo \"3\" > /sys/devices/lm0/usb1/1-1/bConfigurationValue");
                    if (ret < 0) {
                        if (show_progress) fprintf(stderr, "Error: sending Sony control message failed (error %d). Aborting\n\n", ret);
                        exit(1);
                    } else
                        if (show_progress) printf(" OK, Sony control message successfully sent.\n");
                }
                pclose(fp);
            }
        }

        if ( BandluxeMode ) {
            /* EZP: XXX */
            ret = system("/usr/bin/sdparm --command=eject /dev/sg0");
            if (ret < 0) {
                if (show_progress) fprintf(stderr, "Error: sending Bandluxe control message failed (error %d). Aborting\n\n", ret);
                exit(1);
            } else
                if (show_progress) printf(" OK, Bandluxe control message successfully sent.\n");
            /*
            ret = usb_control_msg(devh, 0xc0, 0x11, 00000002, 0, buf, 3, 1000);
            if (ret < 0) {
                if (show_progress) fprintf(stderr, "Error: sending Sony control message failed (error %d). Aborting\n\n", ret);
                exit(1);
            } else
                if (show_progress) printf(" OK, Sony control message successfully sent.\n");
             */
        }

        if ( QisdaMode ) {
            typedef struct _h21SwitchModeReq
            {
                unsigned char header[4];
                unsigned char data[12];
            }h21SwitchModeReq;

            h21SwitchModeReq h21_usb_cm_req;
            /*
             * set 1 to single com port,set 2 to CDCACM mode, set 3 to 
             * dual com port
             */
            h21_usb_cm_req.data[5]=0x01;  
            ret=usb_control_msg(devh, 0 | (0x02 << 5) | 0x00, 0x04, 0x00, 0x00,
                    (char *)&h21_usb_cm_req, sizeof(h21SwitchModeReq), 1000);
            if (ret != 0) {
                if (show_progress) fprintf(stderr, "Error: sending Qisda control message failed (error %d). Aborting\n\n", ret);
                exit(1);
            } else
                if (show_progress) printf(" OK, Qisda control message successfully sent.\n");
        }

        if ( QuantaMode ) {
            /* Q101 is only switched in scsi_generic event! */
            char *invoke;
            invoke = getenv("EZPHOTPLUG");
            if (invoke && *invoke && !strcmp(invoke, "scsi_generic")) {

                ret = system("echo \"suspend\" > /sys/devices/lm0/usb1/1-1/power/level");
                if (ret < 0) {
                    if (show_progress) fprintf(stderr, "Error: sending Quanta control message failed (error %d). Aborting\n\n", ret);
                    exit(1);
                } else
                    if (show_progress) printf(" OK, Quanta control message successfully sent.\n");
            }
        }

        if ( TouchMode ) {
            /* Q110 and DWM156 are switched in scsi_generic event! */
            char *invoke;
            invoke = getenv("EZPHOTPLUG");
            if (invoke && *invoke && !strcmp(invoke, "scsi_generic")) {

                ret = system("mount -t vfat /dev/sda /mnt");
                if (ret < 0) {
                    if (show_progress) fprintf(stderr, "Error %d: mount\n", ret);
                    exit(1);
                } else {
                    if (show_progress) printf(" OK, mount successfully.\n");
                }

                ret = system("touch /mnt/WCDMA.cfg");
                if (ret < 0) {
                    if (show_progress) fprintf(stderr, "Error %d: touch\n", ret);
                    exit(1);
                } else {
                    if (show_progress) printf(" OK, touch successfully.\n");
                }

                ret = system("umount /mnt");
                if (ret < 0) {
                    if (show_progress) fprintf(stderr, "Error %d:unmount\n", ret);
                    exit(1);
                } else {
                    if (show_progress) printf(" OK, unmount successfully.\n");
                }
            }
        }

        if ( ResetUSB ) {
            if (show_progress) printf("Resetting usb device ");
            sleep( 1 );
            int success;
            int bpoint;
            bpoint = 0;
            do {
                success = usb_reset(devh);
                if (show_progress) printf(".");
                bpoint++;
                if (bpoint > 100)
                    success = 1;
            } while (success < 0);

            if ( success ) {
                if (show_progress) printf("\n Reset failed. Can be ignored if device switched OK.\n");
            } else {
                if (show_progress) printf("\n OK, device was reset\n");
            }
        }

        if (show_progress) printf("-> See /proc/bus/usb/devices (or call lsusb) for changes. Bye\n\n");

        if (!DetachStorageOnly && !HuaweiMode && !ResetUSB) {
            ret = usb_release_interface(devh, Interface);
            if (ret != 0)
                if (show_progress) fprintf(stderr, "Error: releasing interface failed (error %d). Trying to continue\n", ret);
        }

        if (!ResetUSB)
            ret = usb_close(devh);

found_but_no_switching: 
        /* EZP: Shall we iterate more? */
        if (map_ptr[j].Mode != (char) 0xF0) {
            struct stat st;
            int num;
            char tmp_ctrltty[32], tmp_datatty[32];
            printf("ezpcom-mode: found [%s][%s]\n", ezp_com_vendor_map[i].desc,
                    map_ptr[j].desc);
#ifdef DEBUG
        printf("[%s] [%s] %d %d %d\n", 
                ezp_com_vendor_map[i].desc, map_ptr[j].desc,
                map_ptr[j].DeviceNum, map_ptr[j].DataTTY,
                map_ptr[j].CtrlTTY);
#endif
            snprintf(tmp, sizeof(tmp), "%d", i);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "brand", 
                    tmp);
            snprintf(tmp, sizeof(tmp), "%d", j);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "model", 
                    tmp);
            /* ttynum */
            snprintf(tmp, sizeof(tmp), "%d", map_ptr[j].DeviceNum);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "ttynum", 
                    tmp);
            /* datatty */
            snprintf(tmp, sizeof(tmp), "%d", map_ptr[j].DataTTY);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "datatty", 
                    tmp);
            /* ctrltty */
            snprintf(tmp, sizeof(tmp), "%d", map_ptr[j].CtrlTTY);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "ctrltty", 
                    tmp);
            /* devname */
            snprintf(tmp, sizeof(tmp), "%s", map_ptr[j].DevName);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "devname", 
                    tmp);
            /* flag */
            snprintf(tmp, sizeof(tmp), "%d", map_ptr[j].Flag);
            ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "flag", 
                    tmp);
            printf("total:%d data:%d control:%d\n", map_ptr[j].DeviceNum,  map_ptr[j].DataTTY, map_ptr[j].CtrlTTY);
            if(ezp_com_vendor_map[i].desc && ezp_com_vendor_map[i].desc[0]) {
                ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "vendor", ezp_com_vendor_map[i].desc);
            }

            if(map_ptr[j].desc && map_ptr[j].desc[0]) {
                ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "product", map_ptr[j].desc);
                if(strstr(map_ptr[j].desc,"340")) {
                    ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "stayfg", "fg");
                } else {
                    ezplib_replace_attr("wan_wwan_probe_rule", wan_num, "stayfg", "bg");
                }
            }

            snprintf(tmp_ctrltty, sizeof(tmp_ctrltty), "/sys/class/tty/%s%d", map_ptr[j].DevName, map_ptr[j].CtrlTTY);
            snprintf(tmp_datatty, sizeof(tmp_datatty), "/sys/class/tty/%s%d", map_ptr[j].DevName, map_ptr[j].DataTTY);
            for (num=0;num<15;num++){
                if((stat(tmp_ctrltty ,&st) == 0) &&
                   (stat(tmp_datatty ,&st) == 0)) {
                    if (show_progress) printf("Use %d sec\n",num);
                    found = 1;
                    break;
                }
                if (show_progress) printf("Wait 1 sec..\n");
                sleep(1);
            }
            if (found) {
                break;
            }
        } 
        /* EZP: Model iteration ends. */
        }
        /* EZP: Vendor iteration ends. */
        if (found) {
            break;
        }
    }
    /* EZP: WAN iteration ends. */
    found = 0;

    return 0;
}

int write_bulk(int endpoint, char *message, int length) {
    int ret;
    ret = usb_bulk_write(devh, endpoint, message, length, 1000);
    if (ret >= 0 ) {
        if (show_progress) printf(" OK, message successfully sent.\n");
    } else {
        if (show_progress) printf(" Sending the message returned error %d, trying to continue ...\n", ret);
    }
    return ret;
}

int read_bulk(int endpoint, char *buffer, int length) {
    int ret;
    ret = usb_bulk_read(devh, endpoint, buffer, length, 1000);
    if (ret >= 0 ) {
        if (show_progress) printf(" OK, response successfully read (%d bytes).\n", ret);
    } else {
        if (show_progress) printf(" Reading the response returned error %d, trying to continue ...\n", ret);
    }
    return ret;
}

void release_usb_device(int dummy) {
    int ret;
    if (show_progress) printf("Program cancelled by system. Bye\n\n");
    ret = usb_release_interface(devh, Interface);
    if (!ret)
        if (show_progress) fprintf(stderr, "Error: failed to release interface: %d\n", ret);
    usb_close(devh);
    if (!ret)
        if (show_progress) fprintf(stderr, "Error: failed to close interface: %d\n", ret);
    if (show_progress) printf("\n");
    exit(1);
}


struct usb_device* search_devices( int *numFound, int vendor, int product, int targetClass) {
    struct usb_bus *bus;
    int devClass;
    struct usb_device* right_dev = NULL;

    *numFound = 0;
    for (bus = usb_get_busses(); bus; bus = bus->next) {
        struct usb_device *dev;
        for (dev = bus->devices; dev; dev = dev->next) {
            if (dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product) {
                (*numFound)++;
                devClass = dev->descriptor.bDeviceClass;
#ifdef EZPACKET
                if (devClass == 0)
                    devClass = dev->config[0].interface[0].altsetting[0].bInterfaceClass;
                if (devClass != targetClass || targetClass == 0)
                    right_dev = dev;
#endif
                right_dev = dev;
            }
        }
    }
    return right_dev;
}


// the parameter parsing stuff

char* ReadParseParam(const char* FileName, char *VariableName) {
    static char Str[LINE_DIM];
    char *VarName, *Comment=NULL, *Equal=NULL;
    char *FirstQuote, *LastQuote, *P1, *P2;
    int Line=0, Len=0, Pos=0;
    FILE *file=fopen(FileName, "r");

    if (file==NULL) {
        if (show_progress) fprintf(stderr, "Error: Could not find file %s\n\n", FileName);
        exit(1);
    }

    while (fgets(Str, LINE_DIM-1, file) != NULL) {
        Line++;
        Len=strlen(Str);
        if (Len==0) goto Next;
        if (Str[Len-1]=='\n' or Str[Len-1]=='\r') Str[--Len]='\0';
        Equal = strchr (Str, '=');			// search for equal sign
        Pos = strcspn (Str, ";#!");			// search for comment
        Comment = (Pos==Len) ? NULL : Str+Pos;
        if (Equal==NULL or ( Comment!=NULL and Comment<=Equal)) goto Next;	// Only comment
        *Equal++ = '\0';
        if (Comment!=NULL) *Comment='\0';

        // String
        FirstQuote=strchr (Equal, '"');		// search for double quote char
        LastQuote=strrchr (Equal, '"');
        if (FirstQuote!=NULL) {
            if (LastQuote==NULL) {
                if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing end quote.\n", FileName, Line);
                goto Next;
            }
            *FirstQuote=*LastQuote='\0';
            Equal=FirstQuote+1;
        }

        // removes leading/trailing spaces
        Pos=strspn (Str, " \t");
        if (Pos==strlen(Str)) {
            if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing variable name.\n", FileName, Line);
            goto Next;		// No function name
        }
        while ((P1=strrchr(Str, ' '))!=NULL or (P2=strrchr(Str, '\t'))!=NULL)
            if (P1!=NULL) *P1='\0';
            else if (P2!=NULL) *P2='\0';
            VarName=Str+Pos;
            //while (strspn(VarName, " \t")==strlen(VarName)) VarName++;

            Pos=strspn (Equal, " \t");
            if (Pos==strlen(Equal)) {
                if (show_progress) fprintf(stderr, "Error reading parameter file %s line %d - Missing value.\n", FileName, Line);
                goto Next;		// No function name
            }
            Equal+=Pos;

            //		printf("%s=%s\n", VarName, Equal);
            if (strcmp(VarName, VariableName)==0) {		// Found it
                fclose(file);
                return Equal;
            }
Next:;
    }

    // not found
    //	fprintf(stderr, "Error reading parameter file %s - Variable %s not found.", 
    //				FileName, VariableName);
    fclose(file);
    return NULL;
}

int hex2num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}


int hex2byte(const char *hex)
{
    int a, b;
    a = hex2num(*hex++);
    if (a < 0)
        return -1;
    b = hex2num(*hex++);
    if (b < 0)
        return -1;
    return (a << 4) | b;
}

int hexstr2bin(const char *hex, char *buf, int len)
{
    int i;
    int a;
    const char *ipos = hex;
    char *opos = buf;
    //    printf("Debug: hexstr2bin bytestring is ");

    for (i = 0; i < len; i++) {
        a = hex2byte(ipos);
        //        printf("%02X", a);
        if (a < 0)
            return -1;
        *opos++ = a;
        ipos += 2;
    }
    //    printf(" \n");
    return 0;
}

