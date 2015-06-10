#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include "fcgi_nas_cgiutil.h"
#include "fcgi_common.h"
#include <bcmnvram.h>
#include "shared.h"
#include "ezp-lib.h"
#include <stdbool.h>
#include <ctype.h>

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
#if 0
   if(check_request_from_myqnapcloud()) {
      ezplib_get_attr_val("upnpc_status_rule", 0, "activated", buf, sizeof(buf), 0);
      if(buf[0] == '1') {
         /* Activated */
         ezplib_get_attr_val("upnpc_status_rule", 0, "outport", buf, sizeof(buf), 0);
         /* We got valid external port settings */
         if(buf[0]) {
            PrintXML("QWebPort", 0, buf);
         } else {
            PrintXML("QWebPort", 0, "80");
         }
      } else {
         /* Not activated */
         PrintXML("QWebPort", 0, "80");
      }
   } else {
      PrintXML("QWebPort", 0, "80");
   }
#endif
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
   PrintIntXML("Battery", 0, get_bettery_percentage());
   PrintIntXML("Charging", 0, get_charging_status());
   // add for QGenie setting
   GenAximSetting();

   return 0;
}

static inline bool check_guest_allow(void) {
    char *guest_allow = nvram_get("allow_guest");
    if(guest_allow && (guest_allow[0] == '0')) {
        return false;
    }
    return true;
}

int fcgi_handle_auth_login(INPUT *input)
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

    INPUT *tmp;

    /* Get Input String */
    if ((tmp = CGI_Find_Parameter(input, "user"))) {
       user = tmp->val;
       FCGI_LOG("user:%s", user);
    }

    /* tolower username to compare guest name */
    for(i=0;i<strlen(user);i++) {
        user_tolower[i] = tolower(user[i]);
    }
    
    if ((tmp = CGI_Find_Parameter(input, "pwd"))) {
       pwd = tmp->val;
       base64_decode((unsigned char *)pwd, strlen(pwd), (unsigned char *)decode_pwd, &n);
       FCGI_LOG("pwd:%s", pwd);
    }
    if((tmp = CGI_Find_Parameter(input, "service"))){
       by_service = 1;
       auth_type = atoi(tmp->val);
    }
    
    // Test XML Response     
    QDoc_Begin();
    PrintXML("DeviceModelName", 0, "TGB-103N");

    /* we compare username & password */
    if(!strcmp(ezp_username, user) && !strcmp(ezp_passwd, decode_pwd)) {
        GenAuthPassedXML(1);
        GenAuthSidXML(ADMIN_SID);
    }
    else if(!strcmp(user_tolower, "guest") && check_guest_allow()) {
        GenAuthPassedXML(1);
        GenAuthSidXML(GUEST_SID);
    }
    else {
        GenAuthPassedXML(0);
        QDoc_End();
        CGI_RELEASE(input);
        return 0;
    }

    PrintXML("doQuick", 0, "");
    PrintXML("is_booting", 0, "0");
    PrintXML("isAdmin", 0, "1");
    PrintXML("username", 0, user);
    PrintXML("groupname", 0, "administrators");
    GenGlobal();
   
    GenParametersXML();    
    PrintXML("serviceURL", 0, "/cgi-bin/filemanager.html");
    PrintXML("ts", 0, "1378959754");

    QDoc_End();
    CGI_RELEASE(input);

    return 0;
}

