#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "nas_cgiutil.h"
#include "common.h"
#include "bcmnvram.h"
#include "ezp-lib.h"
#include "shared.h"

// USE LIBXML
#define AUTH_SIDSIZE   512
#define DEFAULT_USER_NOBODY "nobody"
int is_booting_xml(void)
{
    // int is_file_exist = 1;
    PrintXML("is_booting", 0, "1");
    PrintXML("mediaReady", 0, "1");
    return 0;
}

int GenParametersXML()
{
   PrintXML("HTTPHost", 0, "10.1.1.100");

   PrintXML("webAccessPort", 0, "8080");
   PrintXML("QWebPort", 0, "80");
   PrintXML("webFSEnabled", 0, "1");

   GenMultimedia();  
 
   PrintXML("MSV2Supported", 0, "1");
   PrintXML("MSV2WebEnabled", 0, "1");
   PrintXML("MSV2URL", 0, "/MSV2/");

   GenDownloadStation();

   PrintXML("DSV2Supported", 0, "0");
   PrintXML("DSV3Supported", 0, "1");

   PrintXML("QWebEnabled", 0, "1");
   PrintXML("QWebSSLEnabled", 0, "0");
   PrintXML("QWebSSLPort", 0, "8081");
   PrintXML("NVREnabled", 0, "0");
   PrintXML("NVRURL", 0, "/surveilance");
   PrintXML("NVRVER", 0, "2");  

   PrintXML("WFM2", 0, "1");
   PrintXML("wfmPortEnabled", 0, "0");
   
   PrintXML("webFSEnabled", 0, "1");
   PrintXML("wfmPort", 0, "8081");
   PrintXML("wfmSSLEnabled", 0, "0");
   PrintXML("wfmSSLPort", 0, "443");
   PrintXML("wfmURL", 0, "/cgi-bin/filemanager.html");

   GenMusic();
   GenVideo();   
   GenPhotoStation();

   GenTunnel();
   PrintXML("forceSSL", 0, "0");
   
   PrintXML("HDAROOT_ALMOST_FULL", 0, "0");
    
   GenPasswordConstraints();
   // add for QGenie setting
   GenAximSetting();

   return 0;
}

int main(void)
{
    char *user = DEFAULT_USER_NOBODY;
    char *pwd;
    //char sid[AUTH_SIDSIZE]; 
    //char decoded_pass[512] = "";
    //int result;
    int by_service, auth_type, i;
    char ezp_username[254], ezp_passwd[254], decode_pwd[254]={0}, user_tolower[254]={0};
    unsigned long n = 254;

    /* we get server's username and password */
    ezplib_get_attr_val("http_rule", 0, "username", ezp_username, sizeof(ezp_username), 0);
    ezplib_get_attr_val("http_rule", 0, "passwd", ezp_passwd, sizeof(ezp_passwd), 0);

    INPUT *input = CGI_INIT();
    INPUT *tmp;

    /* Get Input String */
    if ((tmp = CGI_Find_Parameter(input, "user"))) {
       user = tmp->val;
       fprintf(stderr,"<P>user: %s </P>", user);
    }

    /* tolower username to compare guest name */
    for(i=0;i<strlen(user);i++) {
        user_tolower[i] = tolower(user[i]);
    }
    
    if ((tmp = CGI_Find_Parameter(input, "pwd"))) {
       pwd = tmp->val;
       base64_decode((unsigned char *)pwd, strlen(pwd), (unsigned char *)decode_pwd, &n);
       fprintf(stderr,"<P>pwd: %s </P>", decode_pwd);
    }
    if((tmp = CGI_Find_Parameter(input, "service"))){
       by_service = 1;
       auth_type = atoi(tmp->val);
    }
    
    // Test XML Response     
    QDoc_Begin();

    /* we compare username & password */
    if(!strcmp(ezp_username, user) && !strcmp(ezp_passwd, decode_pwd)) {
        PrintXML("isAdmin", 0, "1");
        GenAuthPassedXML(1);
        GenAuthSidXML(ADMIN_SID);
    }
    else if(!strcmp(user_tolower, "guest")) {
        PrintXML("isAdmin", 0, "0");
        GenAuthPassedXML(1);
        GenAuthSidXML(GUEST_SID);
    }
    else {
        GenAuthPassedXML(0);
        PrintXML("isAdmin", 0, "0");
    }

    PrintXML("doQuick", 0, "");
    PrintXML("is_booting", 0, "0");
    PrintXML("username", 0, user);
    PrintXML("groupname", 0, "administrators");
    GenGlobal();
   
    GenParametersXML();    
    PrintXML("serviceURL", 0, "/cgi-bin/filemanager.html");
    PrintXML("ts", 0, "1378959754");
    PrintXML("DeviceModelName", 0, "TGB-103N");

    QDoc_End();
    CGI_RELEASE(input);

    return 0;
}

