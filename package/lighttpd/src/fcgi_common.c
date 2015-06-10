#define _GNU_SOURCE
#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/time.h>
#include <time.h>
#include <utime.h>
#include <fcntl.h>
#include <ctype.h>
#include <libgen.h>
#include <unistd.h>
#include "fcgi_file_op.h"
#include "fcgi_common.h"
#include "fcgi_memdbg.h"
#include <bcmnvram.h>
#include "shared.h"
#include "ezp-lib.h"

//TODO: Remove this in the future
#define ERROR_FAIL  -1
#define SUCCESS     0

int _copy_split_data(WFM2_SPLIT_TYPE_T *aSplitType, char *aSrcBuf, int aLength);
int _str2lower(char *aStr);

xmlDocPtr document;
xmlAttrPtr doc_attr;
xmlNodePtr curPtr;
xmlNodePtr rootPtr;

static char __session_full_username[BUF256] = "";
static char __session_username[BUF256] = "";
static char __session_domain[BUF256] = "";

WFM2_MIMES wfm2_mimes[] = {
    {0, NULL, NULL},
    // image
    {1, "jpg", "image/jpeg"},
    {2, "jpeg", "image/jpeg"},
    {3, "gif", "image/gif"},
    {4, "png", "image/png"},
    {5, "tif", "image/tiff"},
    {6, "tiff", "image/tiff"},
    {7, "bmp", "image/bmp"},
    // video
    {8, "m2ts", "video/mpeg"},
    {9, "avi", "video/x-msvideo"},
    {10, "mpg", "video/mpeg"},
    {11, "mp4", "video/mp4"},
    {12, "wmv", "video/x-ms-wmv"},
    {13, "ts", "video/mpeg"},
    {14, "tp", "video/mpeg"},
    {15, "asf", "video/x-ms-asf"},
    {16, "m2t", "video/mpeg"},
    {17, "mov", "video/quicktime"},
    {18, "m2v", "video/mpeg"},
    {19, "mpeg", "video/mpeg"},
    {20, "3gp", "video/3gp"},
    {21, "mkv", "video/x-matroska"},
    {22, "mts", "video/mpeg"},
    {23, "tod", "video/mpeg"},
    {24, "mod", "video/mpeg"},
    {25, "trp", "video/mpeg"},
    {26, "m1v", "video/mpeg"},
    {27, "m4v", "video/mp4"},
    {28, "rmp4", "video/mp4"},
    {29, "divx", "video/divx"},
    {30, "flv", "video/x-flv"},
    {31, "rmvb", "video/vnd.rn-realvideo"},
    {32, "rm", "video/vnd.rn-realvideo"},
    // audio
    {33, "mp3", "audio/mpeg"},
    {34, "aac", "audio/x-aac"},
    {35, "wav", "audio/x-wav"},
    {36, "wma", "audio/x-ms-wma"},
    {37, "flac", "audio/x-flac"},
    {38, "ape", "audio/x-monkeys-audio"},
    {39, "dts", "application/octet-stream"},
    {40, "m4a", "audio/x-m4a"},
    {41, "ogg", "audio/ogg"},
    {42, "ac3", "audio/ac3"},
    {43, "aiff", "audio/x-aiff"},
    {44, "au", "audio/basic"},
    // others
    {45, "txt", "text/plain"},
    {46, "pdf", "application/pdf"},
    {47, "ppt", "application/x-ppt"},
    {48, "xml", "text/xml"},
    {49, "dtd", "text/xml"},
    {50, "htm", "text/html"},
    {51, "html", "text/html"},
    {52, "xhtml", "text/html"},
    {53, "img", "application/x-img"},
    {54, "zip", "application/zip"},
    {55, "gzip", "application/zip"},
    {56, "tar", "application/octet-stream"},
    {57, "gz", "application/octet-stream"},
    {58, "tgz", "application/octet-stream"},
    {59, "doc", "application/msword"},
    {60, "pps", "application/vnd.ms-powerpoint"},
    {61, "xls", "application/x-xls"},
    {0, NULL, NULL}
};

WFM_VIDEO_TRANSCODE_QUALITY_STRUCT wfm_video_quality[] = {
    {WFM_TQ_LOW, 12, 24, 15, "1000000"},
    {WFM_TQ_MEDIUM, 6, 18, 15, "2000000"},
    {WFM_TQ_HIGH, 1, 6, 24, "3000000"},
    {0, 0, 0}
};

/**********************************************
    Function: 
        formatString
    Description:
        Format input string
    Input: 
        input format with characters
    Output:
        new character from formating charaters
 **********************************************/
int formatString(char *strbuf, char *fmt, ...)
{
    va_list ap;
    int d;
    char c, *s;
    int ret = 0;

    va_start(ap, fmt);

    while (*fmt) {
        switch(*fmt++) {
            case 's':                       /* string */
                s = va_arg(ap, char *);
                ret = sprintf(strbuf, "%s", s);
                break;
            case 'd':                       /* int */
                d = va_arg(ap, int);
                ret = sprintf(strbuf, "%d", d);
                break;
            case 'c':                       /* char */
                /* Note: char is promoted to int. */
                c = va_arg(ap, int);
                ret = sprintf(strbuf, "%c", c);
                break;
        }
    }
    va_end(ap);
    return ret;
}
/*************************************************
Function:
    transfer_special_char
Description:
    Add slash to front of characters in string.
Parameter:
    original        IN: the original string.
    new_char        OUT: the new string which is added slash.
Return values:
    None
*************************************************/
void transfer_special_char(char *original, char *new_char)
{
    int i = 0, j = 0;
    char *ptr = NULL, *start = NULL;

    start = original;
    ptr = new_char;
    while (start[i]) {
        ptr[j] = '\\';
        ptr[j+1] = start[i];
        j = j + 2;
        i++;
    }
    new_char[j] = 0x0;
}

/*************************************************
	Function:
        	check_exist
	Description:
        	check the path exist or not, and tranfer path to full path.
	Parameter:
        	inode           IN: the orignal path. (/Public)
        	real_path       OUT: full path. (/share/xxx_DATA/Public)
	Return values:
        	exist   0:      success
                	-1:     fail
**************************************************/
int check_exist(char *inode, char *real_path)
{
    char tmp[MAX_FILE_PATH], *part_path = NULL;
    struct stat buf;
    int exist, len = 0;
    // char *delim = "/", *ptr = NULL, *inode_tmp = NULL;
    char *ptr = NULL, *inode_tmp = NULL;

    ptr = inode;
    do {
	 ptr = ptr + 1;
         len++;
         //for share in the root path. exp: /PUblic.
         if (len > strlen(inode)) {
		ptr = NULL;
		break;
         }
    } while (ptr[0] != '/');
    if (ptr != NULL)
    	    part_path = __ax_strdup(ptr);
    inode_tmp = __ax_strdup(inode);
    bzero(tmp, sizeof(tmp));
    
    // Tranfer path with UTF8
    // urldecode2(tmp, inode);
    strcpy(tmp, inode);
    
    strcpy(real_path, ROOT_PATH);
    strcat(real_path, tmp);
    
    exist = stat(real_path, &buf);
    if(exist == 0) {
   	 switch(buf.st_mode & S_IFMT) {
         	case S_IFDIR:   exist=1; break;
         	case S_IFLNK:   exist=2; break;
         	case S_IFREG:   exist=3; break;
                default :       exist=0; break;
         }
    }
 
    if(part_path)
    	__ax_free(part_path);
    if(inode_tmp)
    	__ax_free(inode_tmp);

    return exist;
}

void replace_share_path(char *path, size_t size)
{
        char *ptr = NULL;
        char copy[256];
        struct passwd   *passwd;
        ptr = strstr(path, "%D");
        if(ptr){
                if(strcmp(__session_domain,"") == 0)
                        sprintf(copy, "%s", ptr+3);
                else
                        sprintf(copy, "%s", ptr+2);
                snprintf(ptr, size - (path - ptr),"%s%s", __session_domain, copy);
        }
        ptr = strstr(path, "%U");
        if(ptr){
                sprintf(copy, "%s", ptr+2);
                snprintf(ptr, size - (path-ptr), "%s%s", __session_username, copy);
        }
        ptr = strstr(path, "%H");
        if(ptr){
                if((passwd = getpwnam(__session_full_username)))
                        snprintf(ptr, size, "%s", passwd->pw_dir);
        }

}

// 1: legal, 0: illegal
int Check_Illegal_Path(char *aPath)
{
    char *pSep="/";
    char *p=NULL, *tmpPath=NULL;


    tmpPath = __ax_strdup(aPath);

    p = strtok(tmpPath, pSep);
    if(p == NULL) {
        if(tmpPath) __ax_free(tmpPath);
        return 1;
    }

    while(p != NULL) {
        if(!strcmp(p, "..")) {
            if(tmpPath) __ax_free(tmpPath);
            return 0;
        }
        p = strtok(NULL, pSep);
    }

    if(tmpPath)
        __ax_free(tmpPath);

    return 1;
}

/*************************************************
Function:
        check_user_read_write_privilege         
Description:
        check the user has read/write privilege or not in some share folder
Parameter:
        user:   user name
        path:   share path
Return values:
                exist   0:        success
                       -1:      fail
NasMgmt/HTTP/WebPage/WFM2/common.h
**************************************************/
int check_user_read_write_privilege(char *user, char *path)
{
 	// For AXIMCom, always has read/write privilege in share folder
        return 0;
}

/*************************************************
 * Function:
 *     Cal_Size
 * Description:
 *     calculate total size
 * Parameter:
 *     sours:      IN:the folder or file path
 *     file_count: OUT:file total number
 *     dir_count:  OUT:folder total number
 *     total_size: OUT:total size 
 * Return values:
 *     none
 ***************************************************/
static int get_dir_size(char *fn, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size)
{
    DIR *d;
    struct stat buf;
    int exists, total, i;
    char *s = NULL;
    struct dirent **de;

    d = opendir(fn);
    if (d == NULL) {
        perror("get_dir_size.");
        return ERROR_FAIL;
    }

    if( ! *file_count || ! *dir_count || ! *total_size){
        *file_count = *dir_count = *total_size = 0;
    }

    total = scandir(fn, &de, 0, alphasort);

    for(i = 0; i < total; i++) {
        s = __ax_calloc(1, strlen(de[i]->d_name)+ strlen(fn)+ 64);
        sprintf(s, "%s/%s", fn, de[i]->d_name);
        exists = stat(s, &buf);
        if ( (exists>=0) && strcmp(".",de[i]->d_name) && strcmp("..",de[i]->d_name) && S_ISDIR(buf.st_mode)){ //check if the dir is real
            *dir_count+=(unsigned long)1;;
            get_dir_size(s, file_count, dir_count, total_size);
        }
        else if(exists>=0 && (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode))){
            *file_count += (unsigned long)1;
            *total_size += (unsigned long long)(buf.st_size);
        }
        if (exists < 0) {
            FCGI_LOG("Couldn't stat %s\n", s);
        }
     
        if(s) __ax_free(s);
    }
    closedir(d);

    return SUCCESS;
}

static int get_dir_size_ex(char *fn, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size, char *copy_pid)
{
    DIR *d;
    struct stat buf;
    int exists;
    unsigned long total=0, i=0, count=(*file_count);
    char *s = NULL;
    struct dirent **de;
    int percent=0;

    d = opendir(fn);
    if (d == NULL) {
        perror("get_dir_size.");
        return ERROR_FAIL;
    }

    /*
    if( ! *file_count || ! *dir_count || ! *total_size){
        *file_count = *dir_count = *total_size = 0;
    }*/

    total = scandir(fn, &de, 0, alphasort);

    for(i = 0; i < total; i++) {
        s = __ax_calloc(1, strlen(de[i]->d_name)+ strlen(fn)+ 64);
        sprintf(s, "%s/%s", fn, de[i]->d_name);
        exists = stat(s, &buf);
        if ( (exists>=0) && strcmp(".",de[i]->d_name) && strcmp("..",de[i]->d_name) && S_ISDIR(buf.st_mode)){ //check if the dir is real
            *dir_count+=(unsigned long)1;;
            get_dir_size_ex(s, file_count, dir_count, total_size, copy_pid);
        }
        else if(exists>=0 && (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode))){
            *file_count += (unsigned long)1;
            *total_size += (unsigned long long)(buf.st_size);
        }
        if (exists < 0) {
            FCGI_LOG("Couldn't stat %s\n", s);
        }

        if(((*file_count) != 0) && (((*file_count)%3) == 0)) {
            percent = (int)(100. * (count+i) / ((*file_count) * 10));
            //wfm_daemon_write_report_file(de[i]->d_name, (i+1), (*file_count), (unsigned long long)(*total_size), (int)percent, 0, copy_pid);
        }

        if(s) __ax_free(s);
    }
    closedir(d);
    return SUCCESS;
}

/*************************************************
 *  Function:
 *       Cal_Size
 *  Description:
 *       calculate total size
 *  Parameter:
 *       sours:      IN:the folder or file path
 *       file_count: OUT:file total number
 *       dir_count:  OUT:folder total number
 *       total_size: OUT:total size 
 *  Return values:
 *        none
 ****************************************************/
void Cal_Size(char *sours, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size)
{
    DIR *dp;
    struct stat statbuf;

    if((dp=opendir(sours))==NULL){
        if(stat(sours, &statbuf) == 0){
            *total_size += (unsigned long long)(statbuf.st_size);
            *file_count += (unsigned long)1;
        }
    }else{
        get_dir_size(sours, file_count, dir_count, total_size);
    }
    closedir(dp);
}

void Cal_Size_Ex(char *sours, unsigned long *file_count, unsigned long *dir_count, unsigned long long *total_size, char *copy_pid)
{
    DIR *dp;
    struct stat statbuf;

    if((dp=opendir(sours))==NULL){
        if(stat(sours, &statbuf) == 0){
            *total_size += (unsigned long long)(statbuf.st_size);
            *file_count += (unsigned long)1;
        }
    }else{
        get_dir_size_ex(sours, file_count, dir_count, total_size, copy_pid);
        closedir(dp);
    }
}


/***************************************************
    Function: 
        PrintXMLContent
    Description:
        Print the children content under a assigned node on web
    Input: 
        XMLDocument pointer, the node pointer to this document, and the node name
    Output:
        None
 ****************************************************/ 
void
PrintXMLContent(xmlDocPtr doc, xmlNodePtr cur, char *nodename)
{
    printf("<%s>", nodename);
    FCGI_LOG_D("<%s>", nodename);
    xmlChar *key;
    cur = cur->xmlChildrenNode;

    while (cur != NULL) {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if (key == NULL) {
            printf("<%s><![CDATA[]]></%s>", cur->name, cur->name);
    FCGI_LOG_D("<%s><![CDATA[]]></%s>", cur->name, cur->name);
        } else {
            printf("<%s><![CDATA[%s]]></%s>", cur->name, key, cur->name);
    FCGI_LOG_D("<%s><![CDATA[]]></%s>", cur->name, cur->name);
        }
        xmlFree(key);
        cur = cur->next;
    }
    printf("</%s>", nodename);
    FCGI_LOG_D("</%s>", nodename)
}

/*********************************************** 
    Function: 
        PrintXML
    Description:
        Append a node for current XML Document
    Input:
        new node name, depth, and node content for fmt
    Out: 
        none
 ************************************************/

void
PrintXML(char *nodeName, int depth, char *fmt)
{
    if (document == NULL) {
        printf("<P>Failed</P>");
        FCGI_LOG_D("<P>Failed</P>");
        return;
    }

    char str[BUF_SIZE];
    if (fmt == NULL) {
        sprintf(str, "<![CDATA[]]>");
    } else {
        sprintf(str, "<![CDATA[%s]]>", fmt);
    }

    if (depth == 0) {
        xmlNewTextChild(curPtr, NULL, (xmlChar *)nodeName, (xmlChar *)str);
        printf("<%s>%s</%s>", nodeName, str, nodeName);
        FCGI_LOG_D("<%s>%s</%s>", nodeName, str, nodeName);
    }

}

/**********************************
    Function: 
        PrintIntXML
    Description:
        Append a node for current XML Document and set to the given integer value
    Input:
        new node name, depth, and an integer
    Out: 
        none
 ***********************************/
void
PrintIntXML(char *nodeName, int depth, int val) {
    char *intstr = NULL;
    __ax_asprintf(&intstr, "%d", val);
    if(intstr) {
        PrintXML(nodeName, depth,intstr);
        free(intstr);
    }
}

/**********************************
    Function: 
        GenModel
    Description:
        Generate model information for device
    Input:
    Output:
        None
 ***********************************/
void GenModel()
{
    xmlNodePtr model_node = xmlNewNode(NULL, BAD_CAST"model");
    xmlAddChild(rootPtr, model_node);
    
    //model name under model node
    xmlNewTextChild(model_node, NULL, BAD_CAST"modelName", BAD_CAST nvram_get("model"));

    //internalModelName mode node
    xmlNewTextChild(model_node, NULL, BAD_CAST"internalModelName", BAD_CAST nvram_get("model"));

    //platform
    xmlNewTextChild(model_node, NULL, BAD_CAST"platform", BAD_CAST nvram_get("platform"));

    //customModelName
    xmlNewTextChild(model_node, NULL, BAD_CAST"customModelName", BAD_CAST"0");

    //displayModelName 
    xmlNewTextChild(model_node, NULL, BAD_CAST"displayModelName", BAD_CAST nvram_get("model"));

    //storage_v2
    xmlNewTextChild(model_node, NULL, BAD_CAST"storage_v2", BAD_CAST"0");

    PrintXMLContent(document, model_node, "model");
}

/**********************************
  Function:
     GenFirmware
  Description:
     Append Firmware node for current XML document
  Input:
  Output:
***********************************/
void GenFirmware()
{
    xmlNodePtr firmware_node = xmlNewNode(NULL, BAD_CAST"firmware");
    xmlAddChild(rootPtr, firmware_node);
   
    //version
    xmlNewTextChild(firmware_node, NULL, BAD_CAST"version", BAD_CAST"4.0.1");

    //build
    xmlNewTextChild(firmware_node, NULL, BAD_CAST"build", BAD_CAST"20130912");

    //buildtime
    xmlNewTextChild(firmware_node, NULL, BAD_CAST"buildTime", BAD_CAST"2013/09/12");

    PrintXMLContent(document, firmware_node, "firmware");
}

/*************************************************
    Function: 
        GenSpecVerionAndHostname
    Description:
        Append hostname and spec version under current XML document
    Input:
    Output:
        none
 **************************************************/
void GenSpecVerionAndHostname()
{
    PrintXML("specVersion", 0, "1.0");
    PrintXML("hostname", 0, nvram_get("wl0_ssid_rule"));
}

/**********************************************
    Function: 
        GenDemoSiteSupport
    Description:
        Append DemoSiteSuppurt node for current XML document
    Input:
    Output:  
        None
 **********************************************/
void GenDemoSiteSupport()
{
    PrintXML("DemoSiteSuppurt", 0, "no");
}

/***************************************************
    Function:
        GenCustomLogo
    Description:
        Append customlogo content under current XML document  
    Input:
    Output:
        None
 ***************************************************/
void GenCustomLogo()
{
    xmlNodePtr customlogo_node = xmlNewNode(NULL, BAD_CAST"customLogo");
    xmlAddChild(rootPtr, customlogo_node);

    //customFrontLogo
    xmlNewTextChild(customlogo_node, NULL, BAD_CAST"customFrontLogo", NULL);

    //customLoginLogo
    xmlNewTextChild(customlogo_node, NULL, BAD_CAST"customLoginLogo", NULL);

    PrintXMLContent(document, customlogo_node, "customLogo");
}

/*************************************************
    Function: 
        GenGlobal
    Description:
        Append Basic XML node under current XML document: model, firmware, spec and so on
    Input:
    Output:
        None  
 *************************************************/
void GenGlobal()
{
    GenModel();
    GenFirmware();
    GenSpecVerionAndHostname();
    GenDemoSiteSupport();
    GenCustomLogo();
}

/**************************************************
    Function:
        GenAuthPassedXML
    Description:
        Append XML node under current XML document if is auth passed
    Input:
        1 : auth passed 
        0 : auth not passed
    Output:
         None
 **************************************************/
void GenAuthPassedXML(int authPassed)
{
    char authPass[8];
    sprintf(authPass, "%d", authPassed);
    PrintXML("authPassed", 0, authPass);
}

/**************************************************
    Function:
        GenAuthSidXML
    Description:
        Append XML node under current XML document if is Sid
    Input:
    Output:
        None
 **************************************************/
void GenAuthSidXML(char *sid)
{
    PrintXML("authSid", 0, sid);
}

// Generate Service
void GenMultimedia()
{
    PrintXML("QMultimediaEnabled", 0, "1");
}

void GenDownloadStation()
{
    PrintXML("QDownloadEnabled", 0, "1");
}

void GenMusic()
{
    PrintXML("QMusicEnabled", 0, "0");
    PrintXML("QMusicURL", 0, "/musicstation/");
}

void GenVideo()
{
    PrintXML("QVideosEnabled", 0, "0");
    PrintXML("QVideosURL", 0, "/videostation/");
}

void GenPhotoStation()
{
    PrintXML("QPhotosEnabled", 0, "0");
    PrintXML("QPhotosURL", 0, "/photo/");
}

void GenTunnel()
{
    PrintXML("stunnelEnabled", 0, "1");
    PrintXML("stunnelPort", 0, "443"); 
}

void GenPasswordConstraints()
{
    xmlNodePtr passwd_node = xmlNewNode(NULL, BAD_CAST"passwdConstraints");
    xmlAddChild(rootPtr, passwd_node);

    //passwdConstraint01
    xmlNewTextChild(passwd_node, NULL, BAD_CAST"passwdConstraint01", BAD_CAST "0");

    //passwdConstraint02
    xmlNewTextChild(passwd_node, NULL, BAD_CAST"passwdConstraint02", BAD_CAST "0");

    //passwdConstraint03
    xmlNewTextChild(passwd_node, NULL, BAD_CAST"passwdConstraint03", BAD_CAST "0");

    //passwdConstraint04
    xmlNewTextChild(passwd_node, NULL, BAD_CAST"passwdConstraint04", BAD_CAST "0");
   
    PrintXMLContent(document, passwd_node, "passwdConstraints");
}

void
QDoc_Begin()
{
    document = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST"QDocRoot");
    doc_attr = xmlNewProp(root_node, BAD_CAST"version", BAD_CAST"1.0");

    // Setup root node
    xmlDocSetRootElement(document, root_node);
    curPtr = xmlDocGetRootElement(document);
    rootPtr = xmlDocGetRootElement(document);

     if (curPtr == NULL) {
        printf("null");
        FCGI_LOG_D("null");
    } else {
        printf("<?xml version='1.0' encoding='UTF-8' ?>");
        FCGI_LOG_D("<?xml version='1.0' encoding='UTF-8' ?>");
        printf("<%s version='%s'>", curPtr->name, xmlGetProp(curPtr, BAD_CAST"version"));
        FCGI_LOG_D("<%s version='%s'>", curPtr->name, xmlGetProp(curPtr, BAD_CAST"version"));
    }
}

void
QDoc_End()
{
    xmlNodePtr cur = xmlDocGetRootElement(document);
    printf("</%s>", cur->name);
    FCGI_LOG_D("</%s>", cur->name);
    xmlFreeDoc(document);
    xmlMemoryDump();
}

void
OutputRootXML()
{
    xmlNodePtr cur = xmlDocGetRootElement(document);
    PrintXMLContent(document, cur, "QDocRoot");
}

void
GenAximSetting()
{
    xmlNodePtr QGenieSetting = xmlNewNode(NULL, BAD_CAST"QGenieSetting");

    xmlAddChild(rootPtr, QGenieSetting);
    char port[256]={0}, lan_mac[256]={0}, buf[128];
    int i;

    ezplib_get_attr_val("http_rule", 0, "port", port, sizeof(port), 0);

    FCGI_LOG("check_request_from_myqnapcloud():%d\n", check_request_from_myqnapcloud());
    if(check_request_from_myqnapcloud()) {
        /* FIXME !!!
         * We get the first rule set from upnpc_status_rule, which should get
         * the rule set with name "web_mgmt". Maybe fix later. */
        ezplib_get_attr_val("upnpc_status_rule", 0, "activated", buf, sizeof(buf), 0);
        if(buf[0] == '1') {
            /* Activated */
            ezplib_get_attr_val("upnpc_status_rule", 0, "outport", buf, sizeof(buf), 0);
            /* We got valid external port settings */
            if(buf[0]) {

                xmlNewTextChild(QGenieSetting, NULL, BAD_CAST"AWebPort", (xmlChar *)buf);
            } else {
                xmlNewTextChild(QGenieSetting, NULL, BAD_CAST"AWebPort", (xmlChar *)port);
            }
        } else {
            /* Not activated */
            xmlNewTextChild(QGenieSetting, NULL, BAD_CAST"AWebPort", (xmlChar *)port);
        }
    } else {
        xmlNewTextChild(QGenieSetting, NULL, BAD_CAST"AWebPort", (xmlChar *)port);
    }

    ezplib_get_attr_val("lan_hwaddr_rule_default", 0, "hwaddr", lan_mac, sizeof(lan_mac), 0);
    for(i = 0;i < strlen(lan_mac);i++) {
        lan_mac[i] = tolower(lan_mac[i]);
    }
    xmlNewTextChild(QGenieSetting, NULL, BAD_CAST"AMac", (xmlChar *)lan_mac);

    PrintXMLContent(document, QGenieSetting, "QGenieSetting");
}
/************************
    Function: 
        Start_JSON
    Description:
        output header
    Parameter: 
        none

    Return values:
        none
 *************************/
int Start_JSON()
{
    printf("Content-type: text/html; charset=\"UTF-8\"%c%c%c%c", 13, 10, 13, 10);
    FCGI_LOG_D("Content-type: text/html; charset=\"UTF-8\"%c%c%c%c", 13, 10, 13, 10);
    return 0;
}

void update_qsync_device_status(char *user, char *ip, char *sid, char *uid,
                                int action, char *comp, char *desc, int type, int channel, int type2)
{
     
     return;
}

int DOMAIN_IP_LIST_To_JSON(int type, DOMAIN_IP_INFO *dn_ip_info)
{
    json_object *status_object = NULL;

    status_object = json_object_new_object();
    json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
    json_object_object_add(status_object, TAGMYCLOUDNASHN, json_object_new_string(dn_ip_info->mycloudnas_dn));
    json_object_object_add(status_object, TAGDDNSHN, json_object_new_string(dn_ip_info->ddns_dn));
    json_object_object_add(status_object, TAGEXTIP, json_object_new_string(dn_ip_info->ext_ip));
    json_object_object_add(status_object, "local_ip", json_object_new_string(dn_ip_info->local_ip));
    json_object_object_add(status_object, "host_ip", json_object_new_string(dn_ip_info->host_ip));
    json_object_object_add(status_object, "local_ip_list", json_object_new_string(dn_ip_info->local_ip_list));
    json_object_object_add(status_object, "port", json_object_new_int(dn_ip_info->port));
    json_object_object_add(status_object, "sslPort", json_object_new_int(dn_ip_info->sslPort));
    json_object_object_add(status_object, "extPort", json_object_new_int(dn_ip_info->extPort));
    json_object_object_add(status_object, "extSslPort", json_object_new_int(dn_ip_info->extSslPort));
    printf("%s\n", json_object_to_json_string(status_object));
    json_object_put(status_object);

    return 0;
}
/*************************************************
Function:
    File_List_To_JSON
Description:
    list files to JSON format
Parameter:
    file:   file info structure
    count:  the total file number which will be listed.
    total_file: total file number
    permisson:  the user permission of the file's parent folder
Return values:
    none
**************************************************/
int File_List_To_JSON(JSON_FILE_OBJECT *file, int count, int total_file, int permission, int verbose)
{
    json_object *file_object = NULL, *file_array = NULL, *file_list_object = NULL;
    // int     i, medialib=0;
    int     i;
    char    temp[32];
#ifdef FOLDER_ENCRYPTION
    int encrypted=0;
#endif

    file_array = json_object_new_array();
    for (i=0; i<count; i++) {
        file_object = json_object_new_object();
        json_object_object_add(file_object, TAGFILENAME, json_object_new_string((file+i)->filename));
        json_object_object_add(file_object, TAGISFOLDER, json_object_new_int((file+i)->isfolder));
        sprintf(temp, "%llu", (file+i)->filesize);
        json_object_object_add(file_object, TAGFILESIZE, json_object_new_string(temp));
        json_object_object_add(file_object, TAGGROUP, json_object_new_string((file+i)->group));
        json_object_object_add(file_object, TAGOWNER, json_object_new_string((file+i)->owner));
        json_object_object_add(file_object, TAGISCOMMPRESSED, json_object_new_int((file+i)->iscompressed));
        json_object_object_add(file_object, TAGPRIVILEGE, json_object_new_string((file+i)->privilege));
//        json_object_object_add(file_object, TAGPRIVILEGE, json_object_new_string("777"));
        json_object_object_add(file_object, TAGFILETYPE, json_object_new_int((file+i)->filetype));
        json_object_object_add(file_object, TAGMT, json_object_new_string((file+i)->mt_str));
        json_object_object_add(file_object, TAGMTEPOCH, json_object_new_int((file+i)->mt));
#ifdef QBOX_SUPPORT
        json_object_object_add(file_object, TAGQBOXTYPE, json_object_new_int((file+i)->qbox_type));
        json_object_object_add(file_object, TAGQBOXSHAREIDSTATUS, json_object_new_int((file+i)->qbox_share_id_status));
        json_object_object_add(file_object, TAGQBOXSHAREID, json_object_new_string((file+i)->qbox_share_id));
#endif
#ifdef LIBRSYNC
        json_object_object_add(file_object, TAGVERSIONINGSUPPORT, json_object_new_int((file+i)->support_versioning));
#endif
        json_object_object_add(file_object, TAGEXIST, json_object_new_int(((file+i)->exist||(file+i)->mt)?1:0));
        json_object_object_add(file_object, "mp4_240", json_object_new_int((file+i)->mp4_240));
        json_object_object_add(file_object, "mp4_360", json_object_new_int((file+i)->mp4_360));
        json_object_object_add(file_object, "mp4_720", json_object_new_int((file+i)->mp4_720));
        json_object_object_add(file_object, "mp4_480", json_object_new_int((file+i)->mp4_480));
        json_object_object_add(file_object, "mp4_1080", json_object_new_int((file+i)->mp4_1080));
        json_object_object_add(file_object, "trans", json_object_new_int((file+i)->transcoded));
        json_object_object_add(file_object, "play", json_object_new_int((file+i)->play));
        json_object_array_add(file_array, file_object);
    }
/*
        if((file+i)->av_info.filename && strlen((file+i)->av_info.filename)>0){ //media info
                char sType[WFM2_MIME_LENGTH]={0};
                if(wfm2_getMIME((file+i)->av_info.filename, sType)>0){
                    json_object_object_add(file_object, "width", json_object_new_int((file+i)->av_info.width));
                    json_object_object_add(file_object, "height", json_object_new_int((file+i)->av_info.height));
                    json_object_object_add(file_object, "duration", json_object_new_int((file+i)->av_info.duration));
                    json_object_object_add(file_object, "bitrate", json_object_new_int((file+i)->av_info.bitrate));
                    json_object_object_add(file_object, "vcodec", json_object_new_string((file+i)->av_info.vcodec));
                    json_object_object_add(file_object, "acodec", json_object_new_string((file+i)->av_info.acodec));
                    json_object_object_add(file_object, "rotate", json_object_new_int((file+i)->av_info.rotate));
                }
        }

    file_list_object = json_object_new_object();
    medialib = Get_Private_Profile_Integer("DISABLE", "mediaLibrary", 0, APP_RUNTIME_CONF);
    json_object_object_add(file_list_object, "medialib", json_object_new_int(medialib == 1 ? 0 : 1));
*/
    file_list_object = json_object_new_object();
    json_object_object_add(file_list_object, TAGTOTAL, json_object_new_int(total_file));
#ifdef ACL
    json_object_object_add(file_list_object, "acl", json_object_new_int(permission));
    int is_acl_enable = 0;
    if (Is_ACL_Enabled())
        is_acl_enable = 1;
    json_object_object_add(file_list_object, "is_acl_enable", json_object_new_int(is_acl_enable));
    json_object_object_add(file_list_object, "is_winacl_support", json_object_new_int(Is_Win_ACL_Supported() ? 1 : 0));
    json_object_object_add(file_list_object, "is_winacl_enable", json_object_new_int(Is_Win_ACL_Enabled() ? 1 : 0));
#endif
    json_object_object_add(file_list_object, "rtt_support", json_object_new_int(0));
/*
    if(verbose == 1) {
        json_object_object_add(file_list_object, "medialib_enable", json_object_new_int(Is_MediaLib_Enabled() ? 1 : 0));
    }
*/
    json_object_object_add(file_list_object, TAGDATA, file_array);
    //int dlen = write(STDOUT_FILENO, json_object_to_json_string(file_list_object), strlen(json_object_to_json_string(file_list_object)));
    //fsync(STDOUT_FILENO);
    // FCGI_LOG_D("%s\n", json_object_to_json_string(file_list_object));
    //sleep(2);
    printf("%s\n", json_object_to_json_string(file_list_object));
    json_object_put(file_list_object);

    return 0;
}

// Added By Alex to cheat NAS sysinfoReq.cgi
int Sysinfo_To_JSON(int total)
{
     json_object *info_object = NULL, *info_array = NULL, *info_list_object = NULL;
     int i = 0;
     char *id, *name, *html, *surl, *url;

     // Print out sysinfo
     info_array = json_object_new_array();
     for (i = 0; i < total; i++) {
         info_object = json_object_new_object();
         json_object_object_add(info_object, TAGAXIMDISPLAY, json_object_new_int(1) );
         json_object_object_add(info_object, TAGAXIMICON, json_object_new_string("") );
                 
         if (i==0) {
            id = "home";
            html = "/cgi-bin/html/login-m.html";
            name = "MT7620";
            surl = ":443/cgi-bin/html/login-m.html";
            url  = ":8081/cgi-bin/html/login-m.html";
         } else {
            id = "webFileManager";
            html = "cgi-bin/html/filemanager.html";
            name = "File Station";
            surl = ":443/cgi-bin/filemanager.html";
            url  = ":8081/cgi-bin/filemanager.html";
         }

         json_object_object_add(info_object, TAGAXIMID, json_object_new_string(id));
         json_object_object_add(info_object, TAGAXIMHTML, json_object_new_string(html));
         json_object_object_add(info_object, TAGAXIMNAME, json_object_new_string(name));
         json_object_object_add(info_object, TAGAXIMSURL, json_object_new_string(surl));
         json_object_object_add(info_object, TAGAXIMURL, json_object_new_string(url));
         json_object_array_add(info_array, info_object);
     }
     // Start add info list
     info_list_object = json_object_new_object();
     json_object_object_add(info_list_object, TAGAXIMDATA, info_array);
     json_object_object_add(info_list_object, TAGAXIMHOSTNAME, json_object_new_string("MT7620"));
     json_object_object_add(info_list_object, TAGAXIMLANG, json_object_new_string("EN"));
     json_object_object_add(info_list_object, TAGAXIMPROXY, json_object_new_int(1));
     json_object_object_add(info_list_object, TAGAXIMSTATUS, json_object_new_int(1));
     printf("%s\n", json_object_to_json_string(info_list_object));
     json_object_put(info_list_object);
     return 0;
}

/*************************************************
 * Function:
 *     Status_With_Pid_To_JSON
 * Description:
 *     output status and pid to JSON
 * Parameter:
 *     type:   status type
 *     pid:    pid number
 * Return values:
 *     none
 ***************************************************/
int Status_With_Pid_To_JSON(int type, int pid)
{
    json_object *status_object = NULL;

    status_object = json_object_new_object();
    json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
    json_object_object_add(status_object, TAGPID, json_object_new_int(pid));
    printf("%s\n", json_object_to_json_string(status_object));
    json_object_put(status_object);

    return 0;
}

void urldecode2(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
                (isxdigit(a) && isxdigit(b))) {
                    if (a >= 'a')
                        a -= 'a'-'A';
                    if (a >= 'A')
                        a -= ('A' - 10);
                    else
                        a -= '0';
                    if (b >= 'a')
                        b -= 'a'-'A';
                    if (b >= 'A')
                        b -= ('A' - 10);
                    else
                        b -= '0';
                    *dst++ = 16*a+b;
                    src+=3;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

int Status_Progress_To_JSON(int number)
{
        json_object *status_object = NULL;

        status_object = json_object_new_object();
        json_object_object_add(status_object, "number", json_object_new_int(number));
        printf("%s\n", json_object_to_json_string(status_object));
        json_object_put(status_object);

        return 0;
}

int Status_To_JSON(int type)
{
    json_object *status_object = NULL;

    status_object = json_object_new_object();
#ifdef FILE_STATION_VER
    json_object_object_add(status_object, TAGVERSION, json_object_new_string(FILE_STATION_VER));
#endif
#ifdef FILE_STATION_VER
    json_object_object_add(status_object, TAGBUILDNUM, json_object_new_string(BUILD_NUM));
#endif
    json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
    json_object_object_add(status_object, TAGSUCCESS, json_object_new_string("true"));
    printf("%s\n", json_object_to_json_string(status_object));
    json_object_put(status_object);

    return 0;
}

int _check_sort_filter(char aFilter)
{
    int nRet = 0;

    if(aFilter == '@') { nRet = 1; return nRet; }
    else if(aFilter == '-') { nRet = 2; return nRet; }
    else if(aFilter == '!') { nRet = 3; return nRet; }
    else if(aFilter == '#') { nRet = 4; return nRet; }
    else if(aFilter == '$') { nRet = 5; return nRet; }
    else if(aFilter == '%') { nRet = 6; return nRet; }
    else if(aFilter == '&') { nRet = 7; return nRet; }
    else if(aFilter == '(') { nRet = 8; return nRet; }
    else if(aFilter == ')') { nRet = 9; return nRet; }
    else if(aFilter == ';') { nRet = 10; return nRet; }
    else if(aFilter == '.') { nRet = 11; return nRet; }
    else if(aFilter == '[') { nRet = 12; return nRet; }
    else if(aFilter == ']') { nRet = 13; return nRet; }
    else if(aFilter == '`') { nRet = 14; return nRet; }
    else if(aFilter == '^') { nRet = 15; return nRet; }
    else if(aFilter == '_') { nRet = 16; return nRet; }
    else if(aFilter == '{') { nRet = 17; return nRet; }
    else if(aFilter == '}') { nRet = 18; return nRet; }
    else if(aFilter == '~') { nRet = 19; return nRet; }
    else if(aFilter == '+') { nRet = 20; return nRet; }
    else if(aFilter == '=') { nRet = 21; return nRet; }

    return nRet;
}

int wfm2_compare_filter(int aA, int aB)
{

    if(aA == 0 && aB == 0) return 0;
    else if(aA > 0 && aB == 0) return -1;
    else if(aA == 0 && aB > 0) return 1;
    else if(aA > 0 && aB > 0) {
        if(aA > aB) return 1;
        else if(aA < aB) return -1;
        else return 0;
    }
    else return 0;

}

int split_sort_filename(char *aFileName, WFM2_SPLIT_TYPE_T *aSplitType)
{
    char    *pSrcTmp=NULL;
    int     nMod=-1, nIndex=0, nLen=0, i=0, ret=0;
    char    sBuf[MAX_FILE_LENGTH]={0};

    nLen = strlen(aFileName); // ex: txt.txt
    pSrcTmp = __ax_strdup(aFileName);

    for(i=0; i<nLen; i++) {
        if(i == 0 && ((ret = _check_sort_filter(pSrcTmp[0])) > 0)) {
            if(_copy_split_data(aSplitType, pSrcTmp, 1) < 0) {
                if(pSrcTmp) __ax_free(pSrcTmp);
                return -1;
            }

            aSplitType->isFilter = ret;
            continue;
        }
        else if(isdigit(pSrcTmp[i]) || pSrcTmp[i] == '-' || pSrcTmp[i] == '.' || pSrcTmp[i] == '(') {
            if(pSrcTmp[i] == '-') {
                if(nMod != 1) {
                    if(strlen(sBuf) > 0) {
                        if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
                            if(pSrcTmp) __ax_free(pSrcTmp);
                            return -1;
                        }

                        memset(sBuf, '\0', sizeof(sBuf));
                        nIndex=0;
                    }
                }
                nMod=2;
            }
            else if(pSrcTmp[i] == '(') {
                if(nMod != 7) {
                    if(strlen(sBuf) > 0) {
                        if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
                            if(pSrcTmp) __ax_free(pSrcTmp);
                            return -1;
                        }

                        memset(sBuf, '\0', sizeof(sBuf));
                        nIndex=0;
                    }
                }
                nMod=8;
            }
            else if(isdigit(pSrcTmp[i])) {
                if(nMod == 0 || nMod != 22) { // ex: a12(1)
                    if(strlen(sBuf) > 0) {
                        if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
                            if(pSrcTmp) __ax_free(pSrcTmp);
                            return -1;
                        }

                        memset(sBuf, '\0', sizeof(sBuf));
                        nIndex=0;
                    }
                }
                nMod=22;
            }
            else {
                if(strlen(sBuf) > 0) {
                    if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
                        if(pSrcTmp) __ax_free(pSrcTmp);
                        return -1;
                    }

                    memset(sBuf, '\0', sizeof(sBuf));
                    nIndex=0;
                }
                nMod=23;
            }
        }
        else {
            if(nMod != 0) {
                if(strlen(sBuf) > 0) {
                    if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
                        if(pSrcTmp) __ax_free(pSrcTmp);
                        return -1;
                    }

                    memset(sBuf, '\0', sizeof(sBuf));
                    nIndex=0;
                }
            }
            nMod=0;
        }

        sBuf[nIndex] = pSrcTmp[i];
        nIndex++;
    }

    if(nIndex > 0) { // last word
        if(_copy_split_data(aSplitType, sBuf, nIndex) < 0) {
            if(pSrcTmp) __ax_free(pSrcTmp);
            return -1;
        }
    }

    if(pSrcTmp) __ax_free(pSrcTmp);

    return 0;
}

int _copy_split_data(WFM2_SPLIT_TYPE_T *aSplitType, char *aSrcBuf, int aLength)
{
    WFM2_SPLIT_NODE_T   *tNode=NULL;
    WFM2_SPLIT_NODE_T   *pNode=NULL;


    if(aLength <= 0 || strlen(aSrcBuf) <= 0) return -1;

    if((tNode = (WFM2_SPLIT_NODE_T *)__ax_calloc(1, sizeof(WFM2_SPLIT_NODE_T))) == NULL) {
        return -1;
    }

    if(aLength == 1) {
        if((tNode->s = (char *)__ax_calloc(1, 2)) == NULL) {
            return -1;
        }

        if(aSrcBuf[0] == '-') { // "-" cannot compare
            strncpy(tNode->s, " ", 1);
        }
        else strncpy(tNode->s, aSrcBuf, 1);
    }
    else if(aSrcBuf[0] == '-') {
        if(isdigit(aSrcBuf[1])) {
            if((tNode->s = (char *)__ax_calloc(1, aLength)) == NULL) {
                return -1;
            }
            strncpy(tNode->s, &aSrcBuf[1], aLength);
        }
        else {
            if((tNode->s = (char *)__ax_calloc(1, (aLength+1))) == NULL) {
                return -1;
            }
            strncpy(tNode->s, aSrcBuf, aLength);
        }
    }
    else if(aSrcBuf[(aLength-1)] == ' ') {
        if((tNode->s = (char *)__ax_calloc(1, aLength)) == NULL) {
            return -1;
        }
        strncpy(tNode->s, aSrcBuf, (aLength-1));
    }
    else if(aSrcBuf[0] == '.') {
        if((tNode->s = (char *)__ax_calloc(1, (aLength+1))) == NULL) {
            return -1;
        }
        strncpy(tNode->s, aSrcBuf, aLength);
    }
    else if(aSrcBuf[(aLength-1)] == '.') {
        if((tNode->s = (char *)__ax_calloc(1, (aLength+1))) == NULL) {
            return -1;
        }
        strncpy(tNode->s, aSrcBuf, (aLength-1));
    }
    else {
        if((tNode->s = (char *)__ax_calloc(1, (aLength+1))) == NULL) {
            return -1;
        }
        strncpy(tNode->s, aSrcBuf, aLength);
    }
    tNode->next = NULL;

    if(_str2lower(tNode->s) < 0)
        return -1;

    aSplitType->count++;

    if(aSplitType->data == NULL) {
        aSplitType->data = tNode;
    }
    else {
        pNode = aSplitType->data;
        while(pNode->next != NULL) {
            pNode = pNode->next;
        }
        pNode->next = tNode;
    }

    return 0;
}

int _str2lower(char *aStr)
{
    int i=0, nLen=0, c=0;

    if(aStr == NULL || (nLen=strlen(aStr)) == 0) return -1;

    for(i=0; i<nLen; i++) {
        c=tolower(aStr[i]);
        aStr[i] = c;
    }

    return 0;
}

void free_wfm2_split_data(WFM2_SPLIT_NODE_T *aNode)
{
    WFM2_SPLIT_NODE_T *tNode=aNode;
    WFM2_SPLIT_NODE_T *tNodeTmp=NULL;

    while(tNode != NULL) {
        tNodeTmp = tNode->next;
        if(tNode->s) __ax_free(tNode->s);
        __ax_free(tNode);

        tNode = tNodeTmp;
    }

    return;
}

/*************************************************
 * Function:
 *         get_veto_files
 * Description:
 *         Get veto files from wfm2.conf
 * Parameter:
 *         veto_files  OUT: veto files list
 * Return values:
 *         The total number of veto files
 ***************************************************/

int get_veto_files(VETO_INFO **veto_files)
{
    char veto_list[BUF512], *delim = "/", *p = NULL;
    int file_count = 0;
    VETO_INFO *veto_files_tmp = NULL;

    //Get_Private_Profile_String(GLOBAL, VETO_FILES, " ", veto_list, sizeof(veto_list), WFM2_CONFIG_PATH);
    if (!strcmp(veto_list, ""))
        return file_count;

    file_count++;
    veto_files_tmp = (VETO_INFO *)__ax_calloc(1, sizeof(VETO_INFO));
    p = strtok(veto_list, delim);
    strcpy(veto_files_tmp->file_name, p);
    while ((p = strtok(NULL, delim))) {
        veto_files_tmp = (VETO_INFO *)__ax_realloc((void *)veto_files_tmp, sizeof(VETO_INFO)*(file_count+1));
        strcpy((veto_files_tmp + file_count)->file_name, p);
        file_count++;
    }

    *veto_files = veto_files_tmp;

    return file_count;
}


// hidden_file = 1 : list sys hidden files
// hidden_file = 0 : donnot list sys hidden files
int get_veto_files_Ex(VETO_INFO **veto_files, int hidden_file, char *user)
{
    char veto_list[BUF512], *delim = "/", *p = NULL;
    // int file_count = 0, admin = 0;
    int file_count = 0;
    VETO_INFO *veto_files_tmp = NULL;

    //Get_Private_Profile_String(GLOBAL, VETO_FILES, " ", veto_list, sizeof(veto_list), WFM2_CONFIG_PATH);
    if (!strcmp(veto_list, ""))
        return file_count;

#ifdef QBOX_SUPPORT
    if (User_Belongs_To_Group(user, GROUP_ADMINISTRATORS))
        admin = 1;
#endif
    file_count++;
    veto_files_tmp = (VETO_INFO *)__ax_calloc(1, sizeof(VETO_INFO));
    p = strtok(veto_list, delim);
    strcpy(veto_files_tmp->file_name, p);
    while ((p = strtok(NULL, delim))) {
#ifdef QBOX_SUPPORT /* admin can browse Qbox folder */
        if(admin && strcmp(p, QBOX_NAME) == 0){
            continue;
        }
#endif
        veto_files_tmp = (VETO_INFO *)__ax_realloc((void *)veto_files_tmp, sizeof(VETO_INFO)*(file_count+1));
        strcpy((veto_files_tmp + file_count)->file_name, p);
        file_count++;
    }

    if(hidden_file == 0) {
        veto_files_tmp = (VETO_INFO *)__ax_realloc((void *)veto_files_tmp, sizeof(VETO_INFO)*(file_count+1));
        strcpy((veto_files_tmp + file_count)->file_name, ".*");
        file_count++;
    }
    *veto_files = veto_files_tmp;

    return file_count;
}

// return value 1: find veto file
// return value 0: not find veto file
int wfm2_check_veto_files(int veto_file_count, VETO_INFO *veto_files, char *name)
{
    char *pTmp=NULL;
    int i=0, veto_hidden=0;


    if(veto_file_count == 0 || veto_files == NULL || name == NULL)
        return 0;

    pTmp = __ax_strdup(name);

    for (i=0; i<veto_file_count; i++) {
        if (strstr((veto_files + i)->file_name, "*")) {
            char *p=NULL, *delim="*", str[BUF512]={0}, *pstr=NULL;
            strcpy(str, (veto_files + i)->file_name);
            p = strtok(str, delim);
            if (((veto_files + i)->file_name)[0] == '*') {
                pstr = pTmp + strlen(pTmp);
                pstr = pstr - strlen(p);
                if (!strcmp(pstr, p)) {
                    veto_hidden = 1;
                    break;
                }
            }
            else if (((veto_files + i)->file_name)[strlen((veto_files + i)->file_name) - 1] == '*'){
                if (!strncmp(pTmp, p, strlen(p))) {
                    veto_hidden = 1;
                    break;
                }
            }
            else {
                if (!strncmp(pTmp, p, strlen(p))) {
                    p = strtok(NULL, delim);
                    pstr = pTmp + strlen(pTmp);
                    pstr = pstr - strlen(p);
                    if (!strcmp(pstr, p)) {
                        veto_hidden = 1;
                        break;
                    }
                }
            }
        }
        else {
            if (!strcmp(pTmp, (veto_files + i)->file_name)) {
                veto_hidden = 1;
                break;
            }
        }
    }

    if(pTmp) __ax_free(pTmp);

    return veto_hidden;
}

/*************************************************
 * Function:
 *     Share_List_To_JSON
 * Description:
 *     list share to JSON format
 * Parameter:
 *     path:       share path (/Public)
 *     share:      share info structure
 *     count:      the share total number
 *     isdraggable:    is draggable or not
 *     property:   share property (x or r or w)
 *     type:       is normal share or iso
 *     share(folder or iso)
 *  Return values:      
 *     none
 ***************************************************/
int Share_List_To_JSON(char *path, char (*share)[MAX_FILE_LENGTH], int count, int isdraggable, char (*property)[8], char (*type)[16])
{
    json_object *share_object = NULL, *share_array = NULL;
    char *full_path = NULL;
    int i;

    /*
    char *q_dir_1="Photos!";
    char *q_dir_2="Videos";
    char *q_dir_3="Documents";
    char *q_dir_4="Others";
    char *q_dir_5="Music";
    */
    // char *q_dirs = "Photos!Videos!Documents!Others!Music";
    char q_dirs[128];
#ifdef  ACL
    int is_acl_enable = 0;
    is_acl_enable = Is_ACL_Enabled();
#endif
    /*
    if(path)
        full_path = (char *)__ax_malloc(strlen(path) + MAX_FILE_LENGTH + 1);
    else
        full_path = (char *)__ax_malloc(MAX_FILE_LENGTH + 1);
        */
    full_path = nvram_get("st_defpath_rule_num");
    if(full_path && *full_path) {
        int rulecnt = atoi(full_path);
        char *datap = q_dirs;
        int currlen = 0;
        for(i = 0;i < rulecnt; i++) {
            ezplib_get_attr_val("st_defpath_rule", i, "defpath", datap, sizeof(q_dirs), 0);
            if(*datap) {
                currlen = strlen(datap);
                datap[currlen] = '!';
                datap += currlen + 1;
            }
        }

    }
    if(path)
        full_path = (char *)__ax_malloc(strlen(path) + MAX_FILE_LENGTH + 1);
    else
        full_path = (char *)__ax_malloc(MAX_FILE_LENGTH + 1);
    share_array = json_object_new_array();
    for (i=0; i<count; i++) {
        bzero(full_path, sizeof(full_path));
        share_object = json_object_new_object();
        json_object_object_add(share_object, TAGTEXT, json_object_new_string(share[i]));
        if(path == NULL)
            sprintf(full_path, "/%s", share[i]);
        else if (!strcmp(path, "/Qsync") && strstr(q_dirs, share[i])){
            continue;
        }
        else{
            sprintf(full_path, "%s/%s", path, share[i]);
        }
        json_object_object_add(share_object, TAGID, json_object_new_string(full_path));
        if (property == NULL)
            json_object_object_add(share_object, TAGCLS, json_object_new_string("folder"));
        else
            json_object_object_add(share_object, TAGCLS, json_object_new_string(property[i]));
        json_object_object_add(share_object, TAGISDRAGGABLE, json_object_new_int(isdraggable));
        json_object_object_add(share_object, TAGICONCLS, json_object_new_string(type[i]));
#ifdef  ACL
        if (is_acl_enable) {
            int no_support = 0;
            if (path == NULL) {
                no_support = No_Support_ACL(share[i]);
            }
            else {
                no_support = No_Support_ACL(path);
            }
            json_object_object_add(share_object, "noSupportACL", json_object_new_int(no_support));
        }
#endif
        json_object_array_add(share_array, share_object);
    }
    printf("%s\n", json_object_to_json_string(share_array));
    json_object_put(share_array);
    /*
    if(full_path)
        __ax_free(full_path);
        */
    if(full_path)
        __ax_free(full_path);
    return 0;
}


void WFM_HTTP_Not_Modified()
{
    printf("Status: 304\n");
    printf("HTTP/1.1 304 Not Modified\n");
    printf("Content-Length: 0%c%c%c%c",13, 10, 13, 10);
}

void WFM_HTTP_Cache_Control(time_t last_modified)
{
    char _s[128]={0};
    struct tm *_tm=NULL;
    time_t sevenday, tt_now = time(NULL);
    printf("Cache-Control: private, max-age=604800, pre-check=604800\n");
    printf("Pragma: private\n");
    sevenday = tt_now + 604800;
    _tm=gmtime(&sevenday);
    strftime(_s, sizeof(_s), "%a, %d %b %Y %T %Z", _tm);
    printf("Expires: %s\n", _s);
    if(last_modified > 0){
        _tm=gmtime(&last_modified);
        strftime(_s, sizeof(_s), "%a, %d %b %Y %T %Z", _tm);
        printf("Last-Modified: %s\n", _s);
    }
}

int wfm2_getMIME(char *aFileName, char *aType)
{
    int i=0;
    char *ptr=NULL;

    if(!aFileName)
        return 0;

    ptr = strrchr(aFileName, '.');
    if(ptr == NULL) return 0;

    for(i=1; wfm2_mimes[i].index; i++){
        if(!strcasecmp(&ptr[1], wfm2_mimes[i].name)) {
            strncpy(aType, wfm2_mimes[i].type, WFM2_MIME_LENGTH);
            return wfm2_mimes[i].index;
        }
    }

    strncpy(aType, "application/octet-stream", WFM2_MIME_LENGTH);

    return 0;
}

int wfm2_range_download_with_url(char *aFullPath, char *aUrlPath, int flag)
{
    //FILE    *fp = NULL;
    unsigned long long size=0, length=0, start=0, end=0;
    unsigned long long c_start=0, c_end=0;
    //int nBufSize=(1024 * 8);
    //char    s1[nBufSize+1];
    char    msg_buf[512];
    char    sType[WFM2_MIME_LENGTH]={0};
    char    *pFileName=NULL;
    char    *ptr=NULL, *p=NULL;
    time_t  tt_time;
    struct stat _fstat;
    int     nRet=0;
    int     nCheck=1;
    if(stat(aFullPath, &_fstat)) return -1;
    if(!aUrlPath) aUrlPath = aFullPath;
    pFileName = __ax_strdup(basename(aFullPath));
    nCheck = flag;
    if(nCheck == WFM2_DL_FLAG_CHECK_MIMETYPE) {
        if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4_240)) != NULL) {
            strcpy(sType, "video/mp4");
        }
        else if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4_360)) != NULL) {
            strcpy(sType, "video/mp4");
        }
        else if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4_480)) != NULL) {
            strcpy(sType, "video/mp4");
        }
        else if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4_720)) != NULL) {
            strcpy(sType, "video/mp4");
        }
        else if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4P_720)) != NULL) {
            strcpy(sType, "video/mp4");
        }
        else if((p=strstr(aFullPath, WFM2_TRANSCODED_FILE_POSTFIX_MP4_1080)) != NULL) {
            strcpy(sType, "video/mp4");
        }
    }
    else {
        // get mime type
        nRet=wfm2_getMIME(pFileName, sType);
    }

    // init size length
    size = _fstat.st_size;
    length = size;
    end = size-1;

    tt_time=_fstat.st_mtime;
    ptr=(char*)getenv("IF_MODIFIED_SINCE");
    if (ptr!=NULL){
        if(tt_time == strtoul(ptr, NULL, 10)){
            WFM_HTTP_Not_Modified();
            if(pFileName) __ax_free(pFileName);
            return 0;
        }
    }
    else{
        WFM_HTTP_Cache_Control(tt_time);
    }

    // get HTTP_RANGE env
    ptr = (char*)getenv("HTTP_RANGE"); // HTTP_RANGE=Range: bytes=0-11885129
    if(ptr != NULL) {
        if(strstr(ptr, ",")) {
            printf("HTTP/1.1 416 Requested Range Not Satisfiable\n");
            //printf("Content-Range: bytes %llu-%llu/%llu%c%c%c%c", start, end, size, 13, 10, 13, 10);
            snprintf(msg_buf, sizeof(msg_buf), "Content-Range: bytes %llu-%llu/%llu%c%c%c%c", start, end, size, 13, 10, 13, 10);
            printf(msg_buf);
            if(pFileName) __ax_free(pFileName);
            return -1;
        }

        if(ptr[0] == '-') {
            c_start = size - strtoull(&ptr[1], NULL, 10);
        }
        else { // byte range request
            p=strchr(ptr, '=');
            if(p) {
                sscanf(p, "=%llu-%llu", &c_start, &c_end);
            } else {
                // New format looks like 2345678-
                sscanf(ptr, "%llu-%llu", &c_start, &c_end);
                if(c_end == 0) {
                    c_end = end;
                }
            }
        }

        /* Here we handles 1: c_end > end, the requested c_end is larger then
         * file size, and 2: c_start >= c_end: we specify wrong c_start/c_end,
         * including the case that c_start == 0 and no c_end, the "0-". */
        c_end = (c_end > end || (c_start >= c_end)) ? end : c_end;

        if(c_start > c_end || c_start > size - 1 || c_end >= size) {
            printf("HTTP/1.1 416 Requested Range Not Satisfiable\n");
            //printf("Content-Range: bytes %llu-%llu/%llu%c%c%c%c", start, end, size, 13, 10, 13, 10);
            snprintf(msg_buf, sizeof(msg_buf), "Content-Range: bytes %llu-%llu/%llu%c%c%c%c", start, end, size, 13, 10, 13, 10);
            printf(msg_buf);
            // (?) Echo some info to the client?
            if(pFileName) __ax_free(pFileName);
            return -1;
        }

        start = c_start;
        end = c_end;
        length = end - start + 1; // Calculate new content length
        printf("HTTP/1.1 206 Partial Content\n");
        //printf("Content-Range: bytes %llu-%llu/%llu\n", start, end, size);
        snprintf(msg_buf, sizeof(msg_buf), "Content-Range: bytes %llu-%llu/%llu\n", start, end, size);
        printf(msg_buf);
        snprintf(msg_buf, sizeof(msg_buf), "X-Sendfile2: %s %llu-%llu\n", aUrlPath, start, end);
        printf("%s", msg_buf);
        printf("Content-Type: %s\n", sType);
        printf("Content-Disposition: inline; filename=\"%s\"\n", pFileName);
        // printf("Content-Length: %llu%c%c%c%c", length, 13, 10, 13, 10);
        snprintf(msg_buf, sizeof(msg_buf), "Content-Length: %llu%c%c%c%c", length, 13, 10, 13, 10);
        printf(msg_buf);
    }
    else{ // info byte range
        printf("Accept-Ranges: bytes\n");
        // printf("Content-Length: %llu\n", length);
        snprintf(msg_buf, sizeof(msg_buf), "Content-Length: %llu\n", length);
        printf(msg_buf);
        snprintf(msg_buf, sizeof(msg_buf), "X-Sendfile2: %s %llu-%llu\n", aUrlPath, start, end);
        printf("%s", msg_buf);
        //printf("Content-Disposition: attachment; filename=\"%s\"\n", pFileName);
        printf("Content-Disposition: inline; filename=\"%s\"\n", pFileName);
        printf("Content-Transfer-Encoding: binary\n");
        printf("Content-Type: %s%c%c%c%c", sType, 13, 10, 13, 10);
    }
    /*
    fp = fopen(aFullPath,"r");
    if(fp == NULL) {
        if(pFileName) __ax_free(pFileName);
        return -1;
    }

    if(start > 0)
        fseek(fp, start ,SEEK_SET);
        //fseeko(fp, start ,SEEK_SET);

    //while(!feof(fp) && ((nTell = ftello(fp)) <= end)) {
    while(!feof(fp) && ((nTell = ftell(fp)) <= end)) {
        if (nbyte == 0) {
            if(nTell + nBufSize > end) {
                nBufSize = end - nTell + 1;
            }
            nbyte = fread(s1, 1, nBufSize, fp);
        }
        nRemain = fwrite(s1, 1, nbyte, stdout);
        nbyte -= nRemain;
    }

    fclose(fp);
    */
    if(pFileName) __ax_free(pFileName);

    return 0;
}

int CopyAndMove_Status_To_JSON(int type, char *th_num, char *file_name, char *file_size, char *files_cnt, int percent, int copying)
{
    json_object *status_object = NULL;

    status_object = json_object_new_object();
    json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
    json_object_object_add(status_object, "the_num", json_object_new_string(th_num));
    json_object_object_add(status_object, TAGFILENAME, json_object_new_string(file_name));
    json_object_object_add(status_object, TAGFILESIZE, json_object_new_string(file_size));
    json_object_object_add(status_object, "files", json_object_new_string(files_cnt));
    json_object_object_add(status_object, TAGPERCENT, json_object_new_int(percent));
    json_object_object_add(status_object, "copying", json_object_new_int(copying));
    printf("%s\n", json_object_to_json_string(status_object));
    json_object_put(status_object);

    return 0;
}

void Set_Private_Profile_String(char *section, char *param, char *var, char *file)
{
    char buf[256];
    int i;
    int cgi_num = atoi(nvram_get("cgi_progress_num"));

    memset(buf, 0, sizeof(buf));

    for(i=0; i<cgi_num; i++)
    {
        ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(!strcmp(buf, file)) {
            ezplib_replace_attr("cgi_progress", i, param, var);
            break;
        }
    }
    return;
}

void Get_Private_Profile_String(char *section, char *param, char *tmp, char *var, int var_size, char *file)
{
    char buf[256];
    int i;
    int cgi_num = atoi(nvram_get("cgi_progress_num"));

    memset(buf, 0, sizeof(buf));
    for(i=0; i<cgi_num; i++)
    {
        ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(!strcmp(buf, file)) {
            ezplib_get_attr_val("cgi_progress", i, param, var, var_size, 0);
            break;
        }
    }        
    return;
}

void Set_Private_Profile_Integer(char *section, char *param, int var, char *file)
{
    char var_str[256], buf[256];
    int i;
    int cgi_num = atoi(nvram_get("cgi_progress_num"));

    memset(buf, 0, sizeof(buf));
    for(i=0; i<cgi_num; i++)
    {
        ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(!strcmp(buf, file)) {
            sprintf(var_str, "%d", var);
            ezplib_replace_attr("cgi_progress", i, param, var_str);
            break;
        }
    }
    return;
}

int Get_Private_Profile_Integer(char *section, char *param, int var, char *file)
{
    char buf[256];
    int i;
    int cgi_num = atoi(nvram_get("cgi_progress_num"));

    memset(buf, 0, sizeof(buf));
    for(i=0; i<cgi_num; i++)
    {
        ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(!strcmp(buf, file)) {
            ezplib_get_attr_val("cgi_progress", i, param, buf, sizeof(buf), 0);
            break;
        }
    }        
    return atoi(buf);
}

void wfm_daemon_create_report_file(char *aRemote_user, char *aStatus_file)
{
    pid_t   pid=0;
    char buf[256];
    int cgi_num = atoi(nvram_get("cgi_progress_num"));
    int i, ret=0;

    memset(buf, 0, sizeof(buf));

    pid = getpid();
    sprintf(aStatus_file, "%d", pid);

    for(i=0; i<cgi_num; i++)
    {
        ret = ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(ret == 0)
            break;
    }
    ezplib_replace_attr("cgi_progress", i, "pid", aStatus_file);

    return;
}

int wfm_daemon_write_report_file(char *name, unsigned long num, unsigned long files, unsigned long long size, int percent, int copying, char *copypid)
{
    char string_size[32]={0};
    int nPercent = percent;


    Set_Private_Profile_String("", COPY_FILE, name, copypid);

    sprintf(string_size, "%ld", num);
    Set_Private_Profile_String("", COPY_FILE_NUM, string_size, copypid);

    sprintf(string_size, "%ld", files);
    Set_Private_Profile_String("", COPY_FILE_CNT, string_size, copypid);

    sprintf(string_size, "%llu", size);
    Set_Private_Profile_String("", COPY_SIZE, string_size, copypid);

    Set_Private_Profile_Integer("", COPY_PERCENT, nPercent, copypid);
    Set_Private_Profile_Integer("", COPY_COPYING_STATUS, copying, copypid);

    return 0;
}

void wfm_daemon_delete_report_file(char *pid)
{
    char buf[256];
    int cgi_num = atoi(nvram_get("cgi_progress_num"));
    int i;

    memset(buf, 0, sizeof(buf));
    for(i=0; i<cgi_num; i++)
    {
        ezplib_get_attr_val("cgi_progress", i, "pid", buf, sizeof(buf), 0);
        if(!strcmp(buf, pid)) {
            /* clean cgi_progress */
            ezplib_replace_attr("cgi_progress", i, "pid", "");
            ezplib_replace_attr("cgi_progress", i, "copy_file", "");
            ezplib_replace_attr("cgi_progress", i, "copy_number", "");
            ezplib_replace_attr("cgi_progress", i, "files_count", "");
            ezplib_replace_attr("cgi_progress", i, "copy_size", "");
            ezplib_replace_attr("cgi_progress", i, "percent", "");
            ezplib_replace_attr("cgi_progress", i, "copying", "");
            ezplib_replace_attr("cgi_progress", i, "copy_status", "");
            break;
        }
    }

    return;
}

int Status_With_Size_And_Counter_To_JSON(int type, char *size, char *folder, char *file)
{
    json_object *status_object = NULL;

    status_object = json_object_new_object();
    json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
    json_object_object_add(status_object, TAGSIZE, json_object_new_string(size));
    json_object_object_add(status_object, "folderCnt", json_object_new_string(folder));
    json_object_object_add(status_object, "fileCnt", json_object_new_string(file));
    printf("%s\n", json_object_to_json_string(status_object));
    json_object_put(status_object);

    return 0;
}
int Status_With_Size_And_MD5_To_JSON(int type, char *size, char *md5, char *size2)
{
        json_object *status_object = NULL;

        status_object = json_object_new_object();
        json_object_object_add(status_object, TAGSTATUS, json_object_new_int(type));
        json_object_object_add(status_object, TAGSIZE, json_object_new_string(size));
        json_object_object_add(status_object, "md5", json_object_new_string(md5));
        json_object_object_add(status_object, "md5Size", json_object_new_string(size2));
        printf("%s\n", json_object_to_json_string(status_object));
        // DBG("json : %s", json_object_to_json_string(status_object));
        json_object_put(status_object);

        return 0;
}
#define BATT_RECHECK_TIME_INTERVAL_SECS 10*60
#define BATT_PERCENTAGE_FILE            "/tmp/gauge_percentage"
#define BATT_CHECK_COMMAND              "/usr/sbin/ezp-i2c gauge percentage"
#define CHAG_RECHECK_TIME_INTERVAL_SECS 10
#define CHAG_CHECK_COMMAND              "/usr/sbin/ezp-i2c gauge current"

static time_t batt_time = 0;
static int batt_percent = 0;
int get_bettery_percentage(void) {
    time_t curr_time = time(NULL);
    FILE *batcmd_p = NULL;
    char batflag[32], batbuf[32];
    if((curr_time - batt_time) > BATT_RECHECK_TIME_INTERVAL_SECS) {
        batt_time = curr_time;
        batcmd_p = popen(BATT_CHECK_COMMAND, "r");
        if(batcmd_p) {
            fgets(batbuf, sizeof(batbuf), batcmd_p);
            FCGI_LOG("Get battery buf %s", batbuf);
            sscanf(batbuf, "%s %d", batflag, &batt_percent);
            FCGI_LOG("Get battery %s %d", batflag, batt_percent);
            pclose(batcmd_p);
            batcmd_p = NULL;
        }
    } 
#ifdef __DEBUG__
    else {
        FCGI_LOG("Get prev battery %d", batt_percent);
    }
#endif
    return batt_percent;
}

static time_t chag_time = 0;
static int chag_state = 0;
int get_charging_status(void) {
    time_t curr_time = time(NULL);
    FILE *cmd_p = NULL;
    char sflag[32], sbuf[32];
    if((curr_time - chag_time) > CHAG_RECHECK_TIME_INTERVAL_SECS) {
        chag_time = curr_time;
        cmd_p = popen(CHAG_CHECK_COMMAND, "r");
        if(cmd_p) {
            fgets(sbuf, sizeof(sbuf), cmd_p);
            FCGI_LOG("Get charging current %s", sbuf);
            sscanf(sbuf, "%s %d", sflag, &chag_state);
            FCGI_LOG("Get charging current %s %d", sflag, chag_state);
            pclose(cmd_p);
            cmd_p = NULL;
        }
    } 
#ifdef __DEBUG__
    else {
        FCGI_LOG("Get prev charging current %d", chag_state);
    }
#endif
    /*
     Current > 0 means charging
     */
    return ((chag_state > 0) ? 1:0);
}

int set_filetime(const char *src, int mtime, int actime)
{
    struct utimbuf tbuf;
    time_t now = time(NULL);

    if(mtime)
        tbuf.modtime = (time_t)mtime;
    else
        tbuf.modtime = now;

    if(actime)
        tbuf.actime = (time_t)actime;
    else
        tbuf.actime = now;

    if(tbuf.actime < tbuf.modtime)
        tbuf.actime = tbuf.modtime;

    utime(src, &tbuf);
    return 0;
}

static int url_from_myqnapcloud = 0;

int check_request_from_myqnapcloud(void) {
    return url_from_myqnapcloud;

}

void set_from_myqnapcloud(int frommyq){
    url_from_myqnapcloud = frommyq;
}

