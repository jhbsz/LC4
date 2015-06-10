#include "fcgi_stdio.h" /* fcgi library; put it first*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fcgi_nas_cgiutil.h"
#include "fcgi_common.h"
#include "fcgi_qbox_op.h"
#include "fcgi_memdbg.h"

#include <bcmnvram.h>
#include "shared.h"
#include "ezp-lib.h"

int qbox_name_to_uid(char *remote_user) {
    if(!strncmp(remote_user, "admin", sizeof("admin"))) {
        return 0;
    }
    if(!strncmp(remote_user, "guest", sizeof("guest"))) {
        return 1001;
    }
    return -1;
}

int Get_Specify_ID(char *uid, char *buf, size_t buflen) {
    if(!uid || !uid[0]) {
        return -1;
    }
    switch(atoi(uid)) {
        case 0:
            strncpy(buf, "Administrator", buflen);
            break;
        case 1001:
            strncpy(buf, "Guest", buflen);
            break;
        default:
            return -1;
    }
    return 0;
}

void
qbox_get_software_version(char *vbuf, size_t vbuf_len) {
    strncpy(vbuf, nvram_safe_get("fw_version"), vbuf_len);
}

void
Get_Device_SUID(char *suid_buf, size_t suid_buf_len) {
    strncpy(suid_buf, "QGenie", suid_buf_len);
}

void
Get_Mac_Address(char *mac_buf, size_t mac_buf_len) {
    strncpy(mac_buf, nvram_safe_get("lan_hwaddr_rule_default"), mac_buf_len);
    mac_buf[0] = tolower(mac_buf[0]);
    mac_buf[1] = tolower(mac_buf[1]);
    mac_buf[2] = tolower(mac_buf[3]);
    mac_buf[3] = tolower(mac_buf[4]);
    mac_buf[4] = tolower(mac_buf[6]);
    mac_buf[5] = tolower(mac_buf[7]);
    mac_buf[6] = tolower(mac_buf[9]);
    mac_buf[7] = tolower(mac_buf[10]);
    mac_buf[8] = tolower(mac_buf[12]);
    mac_buf[9] = tolower(mac_buf[13]);
    mac_buf[10] = tolower(mac_buf[15]);
    mac_buf[11] = tolower(mac_buf[16]);
    mac_buf[12] = 0;
}

int op_qbox_get_nas_uid(INPUT *input)
{
    INPUT *tmp;
    char string[128], output[64], version[64] = "---";
    char suid[32] = {'\0'}, mac[32] = "---";
    char remote_user[64] = "";
    int uid = 0;
    int is_booting = 1;

    sprintf(output, "---");

    if ((tmp = CGI_Find_Parameter(input, "user")) != NULL){
        snprintf(remote_user, sizeof(remote_user), "%s", tmp->val);
    }

    uid = qbox_name_to_uid(remote_user);
    if(uid < 0)
        goto end;

    snprintf(string, sizeof(string), "%d", uid);

    if (Get_Specify_ID(string, output, sizeof(output)) != 0){
        snprintf(output, sizeof(output), "---");
    }

end:
    qbox_get_software_version(version, sizeof(version));
    Get_Device_SUID(suid, sizeof(suid));
    Get_Mac_Address(mac, sizeof(mac));
    printf("{");
    //_printJsonString(1, "UID", output);
    _printJsonString(1, "UID", "d176c9d770b0942379bc3dc7e6863f33");
    _printJsonString(0, "Qsync_version", version);
#ifdef LIBRSYNC
    _printJsonString(0, "versioning_version", Get_Versioning_Version());
#endif
    //if (file_is_exist("/tmp/.boot_done"))
    if (!strncmp("ready", nvram_safe_get("sysstat"),sizeof("ready")))
        is_booting = 0;
    _printJsonInt(0, "is_booting", is_booting);

    // _printJsonString(0, "SUID", suid);
    _printJsonString(0, "SUID", "fc174cb804e458b621eded56fe5526c9");
    _printJsonString(0, "MAC0", mac);
    printf("}");

    return 0;
}

int qsync_prepare(INPUT *input)
{
    INPUT *tmp;
    if ((tmp=CGI_Find_Parameter(input, "func"))!=NULL){
        /* Here we do about other feature
        if( _session_auth(input) == -1)
            return -1;
        if (!strcmp(tmp->val, "qbox_get_max_log")){
            Start_JSON();
            op_qbox_get_max_log(input);
        }
        */
    }else if((tmp=CGI_Find_Parameter(input, "user")) != NULL){
        // Start_JSON();
        op_qbox_get_nas_uid(input);
    }

    CGI_RELEASE(input);
    return 0;
}
