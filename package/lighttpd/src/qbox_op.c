#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "nas_cgiutil.h"
#include "qbox_op.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "memdbg.h"
#include "bcmnvram.h"

#define AUTH_SIDSIZE 512

extern char remote_user[MAX_USER_LEN];
static char *tmp_path = NULL;
static char gSort_type[8]={0};
int g_hidden_file=0;

JSON_FILE_OBJECT *all_file=NULL, *temp_file=NULL;

static int sorting_by_filename(char *sort_mode, int total);
static int sorting_by_natural(char *sort_type, int total);
static int sorting_by_filesize(char *sort_mode, int total);
static int sorting_by_filetype(char *sort_mode, int total);
static int sorting_by_modifiedtime(char *sort_mode, int total);
static int sorting_by_privilege(char *sort_mode, int total);
static int sorting_by_owner(char *sort_mode, int total);
static int sorting_by_group(char *sort_mode, int total);
int handle_file_list(WFM2_FILE_LIST_PARAM *aListParam, WFM2_FILE_OBJECT *aFileObj);
int _get_tree(char *inode, int is_iso);
int get_root_share(int path_bool);

static SORT_FUNC sort_func[] = { {"filename", sorting_by_filename},
    {"natural", sorting_by_natural},
    {"filesize", sorting_by_filesize},
    {"filetype", sorting_by_filetype},
    {"mt", sorting_by_modifiedtime},
    {"privilege", sorting_by_privilege},
    {"owner", sorting_by_owner},
    {"group", sorting_by_group} };

static int _print_string(char *s)
{
    int c, bs;
    if (!s) return 0;
    while ((c = *s++)) {
    	bs = 1;
    	switch (c) {
        	case '/':
       		case '\\':
                      if(*s == 'u') bs = 0; // copy \uHHHH verbatim
                      break;
                case '"':                break;
                case '\b':    c = 'b';   break;
                case '\f':    c = 'f';   break;
                case '\n':    c = 'n';   break;
                case '\r':    c = 'r';   break;
                case '\t':    c = 't';   break;
                default:      bs = 0;    break;
    	}
    	if (bs) putchar('\\');
    	putchar(c);
    }
    return 0;
}

void _printJsonString(int isfirst, char *label, char *string)
{
    if(isfirst) { 
    	printf("\"%s\": \"", label);
    } else {
    	printf(", \"%s\": \"", label);
    }
    _print_string(string);
    printf("\"");
}

void _printJsonInt(int isfirst, char *label, int value)
{
    if(isfirst) {
          printf("\"%s\":%d", label, value);
    } else {
          printf(", \"%s\": %d", label, value);
    }
}

int op_qbox_get_sid(INPUT *input)
{
    INPUT *tmp;
    char *sid = NULL;
    char comp[COMP_LENGTH];
    int  status = 0, device_type;
    
    if ((tmp = CGI_Find_Parameter(input, "sid"))) {
        sid = tmp->val;    
    	if ((tmp = CGI_Find_Parameter(input, "qbox_computer"))) {
        	snprintf(comp, COMP_LENGTH, "Qfile-%s", tmp->val);
            
        	if((tmp = CGI_Find_Parameter(input, "qbox_device_type")))
                	device_type = atoi(tmp->val);
            	///TODO: update_qsync_device_status
        } else {
        }      
    }
    
    printf("{");   
    _printJsonInt(1, "status", status);
    ///TODO: Print correct JSON format sid
    _printJsonString(0, "sid", sid);
    //_printJsonString(0, "sid", qbox_sid);
    printf("}");
    return 0;
}

int op_get_domain_ip_list(DOMAIN_IP_INFO *aDomain_ip_info)
{
    DOMAIN_IP_INFO _domain_ip_info;

    memset(&_domain_ip_info, 0, sizeof(DOMAIN_IP_INFO));
    if(nvram_get("wan0_ipaddr") == NULL)
        strcpy(_domain_ip_info.ext_ip, "");
    else
        strcpy(_domain_ip_info.ext_ip, nvram_get("wan0_ipaddr"));
    /* disable ddns */
    strcpy(_domain_ip_info.ddns_dn, "");
    strcpy(_domain_ip_info.local_ip, nvram_get("lan0_ipaddr"));

    memset(_domain_ip_info.host_ip, '\0', sizeof(_domain_ip_info.host_ip));
    strcpy(_domain_ip_info.host_ip, nvram_get("lan0_ipaddr"));
    strcpy(_domain_ip_info.local_ip_list, nvram_get("lan0_ipaddr"));

    _domain_ip_info.port = TAGAXIMLANPORT;
    _domain_ip_info.sslPort = TAGAXIMSSLPORT;
    _domain_ip_info.extPort = TAGAXIMEXTPORT;
    _domain_ip_info.extSslPort = TAGAXIMSSLWANPORT;

    DOMAIN_IP_LIST_To_JSON(1, &_domain_ip_info);

    return 0;
}

int op_qbox_get_qbox_info(INPUT *input)
{
//    int policy = 0;
//    policy = qbox_get_qbox_info(remote_user);
    printf("{");
//    _printJsonInt(1, "qbox_function_enable", policy > 1? 1 :0);
//    _printJsonInt(0, "qbox_user_enable", policy & 1);
    _printJsonInt(1, "qbox_function_enable", 0);
    _printJsonInt(0, "qbox_user_enable", 0);
    printf("}");

    return 0;
}

//type: 4 = folder, 8 = file, 0 = unknow
int fselect(const struct dirent *dir)
{
//    VETO_INFO *veto_files;
    int file_count = 0, veto_hidden = 0;
    char    tmp[16];

//    Get_Private_Profile_String(GLOBAL, VETO_HIDDEN, " ", tmp, sizeof(tmp), WFM2_CONFIG_PATH);
    if (!strcmp(tmp, "yes"))
        file_count = 0;
        //file_count = get_veto_files_Ex(&veto_files, g_hidden_file, remote_user);
    /*
    for (i=0; i<file_count; i++) {
        //if veto file from list include *, hidden
        if (strstr((veto_files + i)->file_name, "*")) {
            char *p = NULL, *delim = "*", str[512], *pstr = NULL;
            strcpy(str, (veto_files + i)->file_name);
            p = strtok(str, delim);
            //if veto file = *XXXX, hidden
            if (((veto_files + i)->file_name)[0] == '*') {
                pstr = ((char *)dir->d_name) + strlen(dir->d_name);
                pstr = pstr - strlen(p);
                if (!strcmp(pstr, p)) {
                    veto_hidden = 1;
                    break;
                }
            }
            //if veto file = XXXX*, hidden
            else if (((veto_files + i)->file_name)[strlen((veto_files + i)->file_name) - 1] == '*'){
                if (!strncmp(dir->d_name, p, strlen(p))) {
                    veto_hidden = 1;
                    break;
                }
            }
            //if veto file = XXXX*XXXX, hiddden
            else {
                if (!strncmp(dir->d_name, p, strlen(p))) {
                    p = strtok(NULL, delim);
                    pstr = ((char *)dir->d_name) + strlen(dir->d_name);
                    pstr = pstr - strlen(p);
                    if (!strcmp(pstr, p)) {
                        veto_hidden = 1;
                        break;
                    }

                }
            }
        }
        else {
            if (!strcmp(dir->d_name, (veto_files + i)->file_name)) {
                veto_hidden = 1;
                break;
            }
        }
    }
    */

    if (veto_hidden) return 0;
    else if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        return 0;
    else if(dir->d_type == 4)
        return 1;
    else if(dir->d_type == 0 || dir->d_type == 10 ) {
        char *full_path = NULL;
        struct  stat buf;
        full_path = (char *)__ax_malloc(strlen(tmp_path) + strlen(dir->d_name) + 2);
        sprintf(full_path, "%s/%s", tmp_path, dir->d_name);
        stat(full_path, &buf);
        if (full_path) __ax_free(full_path);

        if (S_ISDIR(buf.st_mode))
            return 1;
        else
            return 0;
    }
    else
        return 0;
}


int op_get_tree(INPUT *input)
{
    INPUT *tmp;
    char *inode = NULL, *sid = NULL;
    int is_iso = 0;
    int ret = 0;

    if((tmp = CGI_Find_Parameter(input, "sid")))
        sid = tmp->val;
    if((tmp = CGI_Find_Parameter(input, "node")))
        inode = tmp->val;
    if((tmp = CGI_Find_Parameter(input, "is_iso")))
        is_iso = atoi(tmp->val);
    if((tmp = CGI_Find_Parameter(input, "hidden_file")))
        g_hidden_file = atoi(tmp->val);
    if((tmp = CGI_Find_Parameter(input, "dir")))
        strcpy(gSort_type, tmp->val);

    if(inode && !strcmp(inode, "share_root_path"))
        get_root_share(1);
    else if(inode && (!strcmp(inode, "share_root") || !strcmp(inode, "tree_root"))) {
        if(strcmp(sid, GUEST_SID))
            get_root_share(0);
        else
            get_root_share(9);
    }
    else if(inode && (!strcmp(inode, "recycle_root")))
        ret = get_root_share(2);
    else if(inode)
        _get_tree(inode, is_iso);
    else
        Status_To_JSON(WFM2_PERMISSION_DENY);

    if(ret == -1)
        Status_To_JSON(WFM2_RECYCLE_BIN_NOT_ENABLE);

    //get_root_share(0);
    return 0;

}
int get_root_share(int path_bool)
{
    DIR * dir;
    struct dirent * ptr;
    dir = opendir("/tmp/mnt/");
    int i, j, is_init=0; 
    char *qfile_dir[] = {
        "Qsync",
        "SD",
        "USB",
        "Videos",
        "Photos",
        "Music",
        "Documents",
        "Others"};
    struct dirent qfile_readdir[sizeof(qfile_dir)/sizeof(char *)];
    memset(qfile_readdir, 0, sizeof(qfile_readdir));
    printf("[");

    while((ptr = readdir(dir))!=NULL)
    {
        if(!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..") )
            continue;
        if((path_bool == 9) && strcmp(ptr->d_name, "SD") && strcmp(ptr->d_name, "USB"))
            continue;
            for(j = 0; j < sizeof(qfile_dir)/sizeof(char *); j++)
            {
                if(!strcmp(ptr->d_name, qfile_dir[j]))
                {
                    qfile_readdir[j].d_type = ptr->d_type;
                    memcpy(qfile_readdir[j].d_name, ptr->d_name, strlen(ptr->d_name)+1);
                    break;
                }
            }
    }

    for(i = 0; i < sizeof(qfile_dir)/sizeof(char *); i++){
        if (qfile_readdir[i].d_name[0] == 0) {
            continue;
        }
        if(is_init != 0)
           printf(",");
        is_init = 1;
        printf("{");
        _printJsonString(1, "cls", "w");
        _printJsonInt(0, "draggable", 0);

        if (qfile_readdir[i].d_type == 4) {
            _printJsonString(0, "iconCls", "folder");
            char id[BUF_SIZE];
            strcpy(id, "/");
            strcat(id, qfile_readdir[i].d_name);
            _printJsonString(0, "id", id);
        } else if (qfile_readdir[i].d_type == 8) {
            _printJsonString(0, "iconCls", "");
           _printJsonString(0, "id", qfile_readdir[i].d_name);
        } else if (qfile_readdir[i].d_type == 0) {
            _printJsonString(0, "iconCls", "");
            _printJsonString(0, "id", qfile_readdir[i].d_name);
        } else if (qfile_readdir[i].d_type == 10) {
            _printJsonString(0, "iconCls", "folder");
            char id[BUF_SIZE];
            strcpy(id, "/");
            strcat(id, qfile_readdir[i].d_name);
            _printJsonString(0, "id", id);
        }
        _printJsonString(0, "text", qfile_readdir[i].d_name);
        printf("}");
    }

    printf("]");
   
    closedir(dir);
    return 0;
}

// For get_list
static int natural_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    WFM2_SPLIT_TYPE_T *ptFileNameA=(WFM2_SPLIT_TYPE_T *)&a->split_name;
    WFM2_SPLIT_TYPE_T *ptFileNameB=(WFM2_SPLIT_TYPE_T *)&b->split_name;
    WFM2_SPLIT_NODE_T *pA=(WFM2_SPLIT_NODE_T *)ptFileNameA->data;
    WFM2_SPLIT_NODE_T *pB=(WFM2_SPLIT_NODE_T *)ptFileNameB->data;
    int                 nNum=0;
    int                 nRet=0, ret=0;
    int                 i=0;
    unsigned long long  nA=0, nB=0;
    int                 filterA=ptFileNameA->isFilter, filterB=ptFileNameB->isFilter;

    if(ptFileNameA->count == ptFileNameB->count) {
        nNum = ptFileNameA->count;
    }
    else {
        nNum = (ptFileNameA->count > ptFileNameB->count) ? ptFileNameB->count : ptFileNameA->count;
    }

    for(i=0; i<nNum; i++) {
        if(i==0 && (filterA > 0 || filterB > 0)) {
            ret = wfm2_compare_filter(filterA, filterB);
            //if(ret != 0) {
            if(ret > 0) { nRet = 1; break; }
            else if(ret < 0) { nRet = -1; break; }
            else { nRet = 0;}
            //}
        }
        else {
            nA = strtoul(pA->s, NULL, 10);
            nB = strtoul(pB->s, NULL, 10);
            if(nA > 0 && nB > 0) { // pA->s=111, pB->s=222
                if(nA > nB) {
                    nRet=1;
                    break;
                }
                else if(nA < nB) {
                    nRet=-1;
                    break;
                }
                else {
                    if (strcmp(pA->s, pB->s) > 0) return 1;
                    if (strcmp(pA->s, pB->s) < 0) return -1;
                }
            }
            else if(nA == 0 && nB == 0){ // pA->s=test, pB->s=b
                if (strcmp(pA->s, pB->s) > 0) return 1;
                if (strcmp(pA->s, pB->s) < 0) return -1;
            }
            else if(nA > 0 || nB > 0) { // pA->s=test => nA=0, pB->s=123 => nB=123
                if(pA->s[0] == '.') {
                    nRet = -1;
                    break;
                }
                else if(pB->s[0] == '.') {
                    nRet = 1;
                    break;
                }
                else if(nA == 0 && isdigit(pA->s[0])) { // pA->s=0 => nA=0, pB->s=123 => nB=123
                    nRet=-1;
                    break;
                }
                else if(nB == 0 && isdigit(pB->s[0])) { // pB->s=0 => nB=0, pA->s=123 => nA=123
                    nRet=1;
                    break;
                }
                else if(nA > 0) { // pA->s=test => nA=0, pB->=123 => nB=123
                    nRet=-1;
                    break;
                }
                else {
                    nRet=1;
                    break;
                }
            }
        }

        pA = pA->next;
        pB = pB->next;
    }

    if(nRet == 0) //default value
        nRet = (ptFileNameA->count >= ptFileNameB->count) ? 1 : -1;

    return nRet;
}


// For get_tree
static int natural_compare_asc_tree(const void *f1, const void *f2)
{
    WFM_SECTION_OBJECT *a = (WFM_SECTION_OBJECT *)f1;
    WFM_SECTION_OBJECT *b = (WFM_SECTION_OBJECT *)f2;


    WFM2_SPLIT_TYPE_T *ptFileNameA=(WFM2_SPLIT_TYPE_T *)&a->split_name;
    WFM2_SPLIT_TYPE_T *ptFileNameB=(WFM2_SPLIT_TYPE_T *)&b->split_name;
    WFM2_SPLIT_NODE_T *pA=(WFM2_SPLIT_NODE_T *)ptFileNameA->data;
    WFM2_SPLIT_NODE_T *pB=(WFM2_SPLIT_NODE_T *)ptFileNameB->data;
    int                 nNum=0;
    int                 nRet=0, ret=0;
    int                 i=0;
    unsigned long long  nA=0, nB=0;
    int                 filterA=ptFileNameA->isFilter, filterB=ptFileNameB->isFilter;


    if(ptFileNameA->count == ptFileNameB->count) {
        nNum = ptFileNameA->count;
    }
    else {
        nNum = (ptFileNameA->count > ptFileNameB->count) ? ptFileNameB->count : ptFileNameA->count;
    }

    for(i=0; i<nNum; i++) {
        if(i==0 && (filterA > 0 || filterB > 0)) {
            ret = wfm2_compare_filter(filterA, filterB);
            //if(ret != 0) {
            if(ret > 0) { nRet = 1; break; }
            else if(ret < 0) { nRet = -1; break; }
            else { nRet = 0;}
            // }
        }
        else {
            nA = strtoul(pA->s, NULL, 10);
            nB = strtoul(pB->s, NULL, 10);
            if(nA > 0 && nB > 0) { // pA->s=111, pB->s=222
                if(nA > nB) {
                    nRet=1;
                    break;
                }
                else if(nA < nB) {
                    nRet=-1;
                    break;
                }
                else {
                    if (strcmp(pA->s, pB->s) > 0) return 1;
                    if (strcmp(pA->s, pB->s) < 0) return -1;
                }
            }
            else if(nA == 0 && nB == 0){ // pA->s=test, pB->s=b
                if (strcmp(pA->s, pB->s) > 0) return 1;
                if (strcmp(pA->s, pB->s) < 0) return -1;
            }
            else if(nA > 0 || nB > 0) { // pA->s=test => nA=0, pB->s=123 => nB=123
                if(pA->s[0] == '.') {
                    nRet = -1;
                    break;
                }
                else if(pB->s[0] == '.') {
                    nRet = 1;
                    break;
                }
                else if(nA == 0 && isdigit(pA->s[0])) { // pA->s=0 => nA=0, pB->s=123 => nB=123
                    nRet=-1;
                    break;
                }
                else if(nB == 0 && isdigit(pB->s[0])) { // pB->s=0 => nB=0, pA->s=123 => nA=123
                    nRet=1;
                    break;
                }
                else if(nA > 0) { // pA->s=test => nA=0, pB->=123 => nB=123
                    nRet=-1;
                    break;
                }
                else {
                    nRet=1;
                    break;
                }
            }
        }
        pA = pA->next;
        pB = pB->next;
    }

    if(nRet == 0) //default value
        nRet = (ptFileNameA->count >= ptFileNameB->count) ? 1 : -1;

    return nRet;
}

// For get_list
static int natural_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;


    WFM2_SPLIT_TYPE_T *ptFileNameA=(WFM2_SPLIT_TYPE_T *)&a->split_name;
    WFM2_SPLIT_TYPE_T *ptFileNameB=(WFM2_SPLIT_TYPE_T *)&b->split_name;
    WFM2_SPLIT_NODE_T *pA=(WFM2_SPLIT_NODE_T *)ptFileNameA->data;
    WFM2_SPLIT_NODE_T *pB=(WFM2_SPLIT_NODE_T *)ptFileNameB->data;
    int                 nNum=0;
    int                 nRet=0, ret=0;
    int                 i=0;
    unsigned long long  nA=0, nB=0;
    int                 filterA=ptFileNameA->isFilter, filterB=ptFileNameB->isFilter;

    if(ptFileNameA->count == ptFileNameB->count) {
        nNum = ptFileNameA->count;
    }
    else {
        nNum = (ptFileNameA->count > ptFileNameB->count) ? ptFileNameB->count : ptFileNameA->count;
    }
    for(i=0; i<nNum; i++) {
        if(i==0 && (filterA > 0 || filterB > 0)) {
            ret = wfm2_compare_filter(filterA, filterB);
            if(ret != 0) {
                if(ret > 0) { nRet = -1; break; }
                else if(ret < 0) { nRet = 1; break; }
                else { nRet = 0;}
            }
        }
        else {
            nA = strtoul(pA->s, NULL, 10);
            nB = strtoul(pB->s, NULL, 10);
            if(nA > 0 && nB > 0) { // pA->s=111, pB->s=222
                if(nA > nB) {
                    nRet=-1;
                    break;
                }
                else if(nA < nB) {
                    nRet=1;
                    break;
                }
                else {
                    if (strcmp(pA->s, pB->s) > 0) return -1;
                    if (strcmp(pA->s, pB->s) < 0) return 1;
                }
            }
            else if(nA == 0 && nB == 0){ // pA->s=test, pB->s=b
                if (strcmp(pA->s, pB->s) > 0) return -1;
                if (strcmp(pA->s, pB->s) < 0) return 1;
            }
            else if(nA > 0 || nB > 0) { // pA->s=test => nA=0, pB->s=123 => nB=123
                if(pA->s[0] == '.') {
                    nRet = 1;
                    break;
                }
                else if(pB->s[0] == '.') {
                    nRet = -1;
                    break;
                }
                else if(nA == 0 && isdigit(pA->s[0])) { // pA->s=0 => nA=0, pB->s=123 => nB=123
                    nRet=1;
                    break;
                }
                else if(nB == 0 && isdigit(pB->s[0])) { // pB->s=0 => nB=0, pA->s=123 => nA=123
                    nRet=-1;
                    break;
                }
                else if(nA > 0) { // pA->s=test => nA=0, pB->=123 => nB=123
                    nRet=1;
                    break;
                }
                else {
                    nRet=-1;
                    break;
                }
            }
        }

        pA = pA->next;
        pB = pB->next;
    }

    if(nRet == 0) //default value
        nRet = (ptFileNameA->count >= ptFileNameB->count) ? -1 : 1;

    return nRet;
}

// For get_tree
static int natural_compare_desc_tree(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    WFM2_SPLIT_TYPE_T *ptFileNameA=(WFM2_SPLIT_TYPE_T *)&a->split_name;
    WFM2_SPLIT_TYPE_T *ptFileNameB=(WFM2_SPLIT_TYPE_T *)&b->split_name;
    WFM2_SPLIT_NODE_T *pA=(WFM2_SPLIT_NODE_T *)ptFileNameA->data;
    WFM2_SPLIT_NODE_T *pB=(WFM2_SPLIT_NODE_T *)ptFileNameB->data;
    int                 nNum=0;
    int                 nRet=0, ret=0;
    int                 i=0;
    unsigned long long  nA=0, nB=0;
    int                 filterA=ptFileNameA->isFilter, filterB=ptFileNameB->isFilter;

    if(ptFileNameA->count == ptFileNameB->count) {
        nNum = ptFileNameA->count;
    }
    else {
        nNum = (ptFileNameA->count > ptFileNameB->count) ? ptFileNameB->count : ptFileNameA->count;
    }
    for(i=0; i<nNum; i++) {
        if(i==0 && (filterA > 0 || filterB > 0)) {
            ret = wfm2_compare_filter(filterA, filterB);
            if(ret != 0) {
                if(ret > 0) { nRet = -1; break; }
                else if(ret < 0) { nRet = 1; break; }
                else { nRet = 0;}
            }
        }
        else {
            nA = strtoul(pA->s, NULL, 10);
            nB = strtoul(pB->s, NULL, 10);
            if(nA > 0 && nB > 0) { // pA->s=111, pB->s=222
                if(nA > nB) {
                    nRet=-1;
                    break;
                }
                else if(nA < nB) {
                    nRet=1;
                    break;
                }
                else {
                    if (strcmp(pA->s, pB->s) > 0) return -1;
                    if (strcmp(pA->s, pB->s) < 0) return 1;
                }
            }
            else if(nA == 0 && nB == 0){ // pA->s=test, pB->s=b
                    if (strcmp(pA->s, pB->s) > 0) return -1;
                                if (strcmp(pA->s, pB->s) < 0) return 1;
                                        }
                    else if(nA > 0 || nB > 0) { // pA->s=test => nA=0, pB->s=123 => nB=123
                        if(pA->s[0] == '.') {
                            nRet = 1;
                            break;
                        }
                        else if(pB->s[0] == '.') {
                            nRet = -1;
                            break;
                        }
                        else if(nA == 0 && isdigit(pA->s[0])) { // pA->s=0 => nA=0, pB->s=123 => nB=123
                            nRet=1;
                            break;
                        }
                        else if(nB == 0 && isdigit(pB->s[0])) { // pB->s=0 => nB=0, pA->s=123 => nA=123
                            nRet=-1;
                            break;
                        }
                        else if(nA > 0) { // pA->s=test => nA=0, pB->=123 => nB=123
                            nRet=1;
                            break;
                        }
                        else {
                            nRet=-1;
                            break;
                        }
                    }
        }

        pA = pA->next;
        pB = pB->next;
    }

    if(nRet == 0) //default value
        nRet = (ptFileNameA->count >= ptFileNameB->count) ? -1 : 1;

    return nRet;
}

// For get_tree sorting
static int sorting_by_natural_tree(char *sort_type, int total, WFM_SECTION_OBJECT *secList)
{
    if (!strcmp(sort_type, "ASC")) {
        qsort(secList, total, sizeof(WFM_SECTION_OBJECT), natural_compare_asc_tree);
    }
    else if (!strcmp(sort_type, "DESC")) {
        qsort(secList, total, sizeof(WFM_SECTION_OBJECT), natural_compare_desc_tree);
    }

    return 0;
}

int _get_tree(char *inode, int is_iso)
{
    struct dirent **namelist;
    int i, total = 0;
    char *path = NULL;
    int exist;
    char (*share)[MAX_FILE_LENGTH];
    char (*property)[8] = NULL, (*type)[16] = NULL;
    WFM_SECTION_OBJECT  *secList=NULL;

    path = (char *)__ax_malloc(strlen(inode) + SHARE_PATH_LENGTH + 15);
    exist = check_exist(inode, path);
    if(exist < 0){
        Status_To_JSON(WFM2_FILE_NO_EXIST);
        if (path) __ax_free(path);
        return -1;
    }

    //for fselect to tell which one is folder or file
    tmp_path = (char *)__ax_malloc(strlen(path) + 1);
    strncpy(tmp_path, path, strlen(path));
    tmp_path[strlen(path)] = 0x0;

    total = scandir(path, &namelist, fselect, alphasort);
    if(total < 0){
        perror("scandir");
        if (path) __ax_free(path);
        if (tmp_path) __ax_free(tmp_path);
        return -1;
    }
    else {
        share = (char (*)[MAX_FILE_LENGTH]) __ax_malloc(total * MAX_FILE_LENGTH * sizeof(char));
        type = (char (*)[16])__ax_malloc(total * 16 * sizeof(char));
#ifdef  ACL
        property = (char (*)[8])__ax_malloc(total * 8 * sizeof(char));
#endif
        for(i=0; i<total; i++)
        {
            strcpy(share[i], namelist[i]->d_name);
            strcpy(type[i], "folder");
#ifdef  ACL
            full_path = (char *)__ax_malloc(strlen(path) + strlen(namelist[i]->d_name) + 2);
            sprintf(full_path, "%s/%s", path, namelist[i]->d_name);
            if (Is_ACL_Enabled()) {
                mode = Get_ACL_User_Group_Security_For_Share(remote_user, full_path);
                sprintf(property[i], "%d", mode);
            }
            else {
                sprintf(property[i], "%d", 7);
            }
            if (full_path) __ax_free(full_path);
#endif
        }

        // for natural sorting
        secList = __ax_calloc(total, sizeof(WFM_SECTION_OBJECT));
        for(i=0; i<total; i++) {
            strncpy((secList+i)->share, share[i], strlen(share[i]));
            if(split_sort_filename(share[i], &(secList+i)->split_name) < 0) {
                int j;
                for(j=0; j<(i+1); j++) {
                    free_wfm2_split_data((secList+j)->split_name.data);
                }
                if(secList != NULL) __ax_free(secList);

                if(share) __ax_free(share);

                return -1;
            }
        }

        if(strcmp(gSort_type, "DESC"))
            sorting_by_natural_tree("ASC", total, secList);
        else
            sorting_by_natural_tree("DESC", total, secList);

        for(i=0; i<total; i++) {
            memset(share[i], 0, sizeof(share[i]));
            strncpy(share[i], (secList+i)->share, strlen((secList+i)->share));
        }
        Share_List_To_JSON(inode, share, total, 1, property, type);
    }
    // __ax_free 
    for(i=0; i<total; i++) {
        free_wfm2_split_data((secList+i)->split_name.data);
    }
    if(secList != NULL) __ax_free(secList);
    // end natural sorting
    if (path) __ax_free(path);
    if (share) __ax_free(share);
    if (tmp_path) __ax_free(tmp_path);
    if (type) __ax_free(type);

    return -1;
}

// For get_file_list: sorting by filename
static int filename_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->filename, b->filename) > 0) return 1;
    if (strcmp(a->filename, b->filename) < 0) return -1;

    return 0;
}

static int filename_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->filename, b->filename) < 0) return 1;
    if (strcmp(a->filename, b->filename) > 0) return -1;

    return 0;
}

// Sorting by file size
static int filesize_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->filesize > b->filesize) return 1;
    if (a->filesize < b->filesize) return -1;

    return 0;
}

static int filesize_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->filesize < b->filesize) return 1;
    if (a->filesize > b->filesize) return -1;

    return 0;
}

// Sorting by file type
static int filetype_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->type_str, b->type_str) > 0) return 1;
    if (strcmp(a->type_str, b->type_str) < 0) return -1;

    return 0;
}

static int filetype_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->type_str, b->type_str) < 0) return 1;
    if (strcmp(a->type_str, b->type_str) > 0) return -1;

    return 0;
}

// Sorting by modification time
static int modifiedtime_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->mt > b->mt) return 1;
    if (a->mt < b->mt) return -1;

    return 0;
}

static int modifiedtime_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (a->mt < b->mt) return 1;
    if (a->mt > b->mt) return -1;

    return 0;
}

// Sorting by privilege
static int privilege_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->privilege, b->privilege) > 0) return 1;
    if (strcmp(a->privilege, b->privilege) < 0) return -1;

    return 0;
}

static int privilege_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->privilege, b->privilege) < 0) return 1;
    if (strcmp(a->privilege, b->privilege) > 0) return -1;

    return 0;
}

// Sorting by owner
static int owner_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->owner, b->owner) > 0) return 1;
    if (strcmp(a->owner, b->owner) < 0) return -1;

    return 0;
}

static int owner_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->owner, b->owner) < 0) return 1;
    if (strcmp(a->owner, b->owner) > 0) return -1;

    return 0;
}

// Sorting by group
static int group_compare_asc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->group, b->group) > 0) return 1;
    if (strcmp(a->group, b->group) < 0) return -1;

    return 0;
}

static int group_compare_desc(const void *f1, const void *f2)
{
    JSON_FILE_OBJECT *a = (JSON_FILE_OBJECT *)f1;
    JSON_FILE_OBJECT *b = (JSON_FILE_OBJECT *)f2;

    if (strcmp(a->group, b->group) < 0) return 1;
    if (strcmp(a->group, b->group) > 0) return -1;

    return 0;
}

/* For get_file_list sorting  */
static int sorting_by_natural(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), natural_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), natural_compare_desc);

    return 0;
}

static int sorting_by_filename(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filename_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filename_compare_desc);
    return 0;
}

static int sorting_by_filesize(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filesize_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filesize_compare_desc);
    return 0;
}

static int sorting_by_filetype(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filetype_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), filetype_compare_desc);

    return 0;
}

static int sorting_by_modifiedtime(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), modifiedtime_compare_desc);
    return 0;
}

static int sorting_by_privilege(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), privilege_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), privilege_compare_desc);
    return 0;
}

static int sorting_by_owner(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), owner_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), owner_compare_desc);
    return 0;
}

static int sorting_by_group(char *sort_type, int total)
{
    DBG("sort_type %s", sort_type);
    if (!strcmp(sort_type, "ASC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), group_compare_asc);
    else if (!strcmp(sort_type, "DESC"))
        qsort(all_file, total, sizeof(JSON_FILE_OBJECT), group_compare_desc);

    return 0;
}

int op_get_file_list(INPUT *input)
{
    INPUT *tmp;
    WFM2_FILE_LIST_PARAM sFileListParam;
    char *full_path = NULL;
    int nRet=0;

    memset(&sFileListParam, 0, sizeof(WFM2_FILE_LIST_PARAM));
    if((tmp = CGI_Find_Parameter(input, "path"))) {
        full_path = (char *)__ax_malloc(strlen(tmp->val) + MAX_FILE_LENGTH + PATH_PREFIX_LENGTH + 256);
        int exist = check_exist(tmp->val, full_path);
        if (exist < 0) {
            Status_To_JSON(WFM2_FILE_NO_EXIST);
            if(full_path) __ax_free(full_path);
                return -1;
        }
        sFileListParam.node = __ax_strdup(tmp->val);
    }

    if((tmp = CGI_Find_Parameter(input, "list_mode"))) {
        strncpy(sFileListParam.listMode, tmp->val, WFM2_LIST_MODE_LEN);
    }

    if((tmp = CGI_Find_Parameter(input, "start"))) {
        sFileListParam.start = atoi(tmp->val);
    }
    else {
        sFileListParam.start = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "limit"))) {
        sFileListParam.limit = atoi(tmp->val);
    }
    else {
        sFileListParam.limit = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "is_iso"))) {
        sFileListParam.is_iso = atoi(tmp->val);
    }
    else {
        sFileListParam.is_iso = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "type"))) { // 1:music, 2:video, 3:photo, 4: folder
        sFileListParam.filetype = atoi(tmp->val);
        if(sFileListParam.filetype < MUSIC || sFileListParam.filetype > MEDIA)
            sFileListParam.filetype = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "mp4_360"))) {
        sFileListParam.mp4_360 = atoi(tmp->val);
    }
    else {
        sFileListParam.mp4_360 = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "mp4_720"))) {
        sFileListParam.mp4_720 = atoi(tmp->val);
    }
    else {
        sFileListParam.mp4_720 = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "mp4_240"))) {
        sFileListParam.mp4_240 = atoi(tmp->val);
    }
    else {
        sFileListParam.mp4_240 = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "mp4_480"))) {
        sFileListParam.mp4_480 = atoi(tmp->val);
    }
    else {
        sFileListParam.mp4_480 = 0;
    }

    if((tmp = CGI_Find_Parameter(input, "mp4_1080"))) {
        sFileListParam.mp4_1080 = atoi(tmp->val);
    }
    else {
        sFileListParam.mp4_1080 = 0;
    }

    // for special video file
    if((tmp = CGI_Find_Parameter(input, "filename"))) {
        strncpy(sFileListParam.filename, tmp->val, (MAX_FILE_LENGTH-1));
    }

    if((tmp = CGI_Find_Parameter(input, "sort"))) {
        strncpy(sFileListParam.sort, tmp->val, WFM2_SORT_MODE_LEN);
    }

    if((tmp = CGI_Find_Parameter(input, "dir"))) {
        strncpy(sFileListParam.dir, tmp->val, WFM2_SORT_TYPE_LEN);
    }

    if((tmp = CGI_Find_Parameter(input, "hidden_file"))) {
        sFileListParam.hidden_file = atoi(tmp->val);
    }
    else {
        sFileListParam.hidden_file = 0;
    }

    nRet = handle_file_list(&sFileListParam, NULL);

    //MARK: original handle list function
    //sFileListParam.end = sFileListParam.start + sFileListParam.limit - 1;
    //_get_file_list(full_path, &sFileListParam, NULL);

    if(full_path) __ax_free(full_path);
    if(sFileListParam.node != NULL) __ax_free(sFileListParam.node);
    return nRet;
}

int _get_file_list(char *path, WFM2_FILE_LIST_PARAM *aListParam, WFM2_FILE_OBJECT *aFileObj)
{
    char tmp[MAX_FILE_PATH];
    DIR *dir, *_dir;
    struct dirent * ptr;
    struct  stat buf;
    int total=0, count=0, i, j, nFindPageNum=0;;
    int start=aListParam->start, end=aListParam->end, sort_mode=aListParam->sortmode, permission=aListParam->permission;
    char q_dir_1[MAX_FILE_PATH]="Photos";
    char q_dir_2[MAX_FILE_PATH]="Videos";
    char q_dir_3[MAX_FILE_PATH]="Documents";
    char q_dir_4[MAX_FILE_PATH]="Others";
    char q_dir_5[MAX_FILE_PATH]="Music";

    dir = opendir(path);
    while ((ptr = readdir(dir))!=NULL) {
        //do not include "." and ".."
        if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
            continue; 
       //do not include q_dir
       if(!strcmp(path, "/tmp/mnt/Qsync")){
            if (!strcmp(ptr->d_name,q_dir_1) || !strcmp(ptr->d_name,q_dir_2) || !strcmp(ptr->d_name,q_dir_3) || !strcmp(ptr->d_name,q_dir_4) || !strcmp(ptr->d_name,q_dir_5))
            continue;
       } 
        total++;
    }
    closedir(dir);

    _dir = opendir(path);
    all_file = (JSON_FILE_OBJECT *)__ax_malloc(total * sizeof(JSON_FILE_OBJECT));
    memset(all_file, 0, (total * sizeof(JSON_FILE_OBJECT)));

    while ((ptr = readdir(_dir))!=NULL) {
        //do not include "." and ".."
        if (!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..")) 
            continue;
        //do not include q_dir
        if (!strcmp(path, "/tmp/mnt/Qsync")){
            if (!strcmp(ptr->d_name, q_dir_1) || !strcmp(ptr->d_name,q_dir_2) || !strcmp(ptr->d_name,q_dir_3) || !strcmp(ptr->d_name,q_dir_4) || !strcmp(ptr->d_name,q_dir_5)) 
            continue;
        }

        struct passwd *user;
        struct group  *data;

        user = getpwuid(buf.st_uid);
        if (user == NULL)
            sprintf((all_file+count)->owner, "%d", buf.st_uid);
        else
            strcpy((all_file+count)->owner, user->pw_name);
        data = getgrgid(buf.st_gid);
        if (data == NULL)
            sprintf((all_file+count)->group, "%d", buf.st_gid);
        else
            strcpy((all_file+count)->group, data->gr_name);
        sprintf(tmp, "%s/%s", path, ptr->d_name);
        stat(tmp, &buf);
        (all_file+count)->filesize = (unsigned long long)(buf.st_size);
        (all_file+count)->mt = buf.st_mtime;
        strcpy((all_file+count)->filename, ptr->d_name);
        sprintf((all_file+count)->privilege, "%03o", (buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)));
        if (S_ISDIR(buf.st_mode)) {
            (all_file+count)->isfolder = 1;
            strcpy((all_file+count)->type_str, " ");
        }
        count++;

        if(count >= total) {
            break;
        }
    }
    closedir(_dir);

    // split sort filename for natural sort
    if(sort_mode == 1) { // sort by natural
        for (i=0; i<total; i++) {
            if(split_sort_filename((all_file+i)->filename, &(all_file+i)->split_name) < 0) {
                if(all_file) {
                    for(j=0; j<(i+1); j++)
                        free_wfm2_split_data((all_file+j)->split_name.data);

                    __ax_free(all_file);
                }

                return -1;
            }
        }
    }

    //sorting by type
    (sort_func[sort_mode].function)(aListParam->dir, total);

    temp_file = (JSON_FILE_OBJECT *)__ax_malloc(total * sizeof(JSON_FILE_OBJECT));
    memset(temp_file, 0, (total * sizeof(JSON_FILE_OBJECT)));

    j=0;

    //display folder first
    for (i=0; i<total; i++) {
        if ((all_file+i)->isfolder == 1) {
            *(temp_file+j) = *(all_file+i);
            j++;
        }
    }

    for (i=0; i<total; i++) {
        if ((all_file+i)->isfolder == 0) {
            *(temp_file+j) = *(all_file+i);

            if(aListParam->filename != NULL && strlen(aListParam->filename) > 0) {
                if(!strcmp((temp_file+j)->filename, aListParam->filename)) {
                    nFindPageNum = (int)((i+1) / (end-start+1));

                    if(((i+1)%(end-start+1)) > 0) nFindPageNum += 1;

                    if(nFindPageNum > 1) {
                        int nLimit=0;
                        nLimit = end-start+1;
                        start = (nFindPageNum-1) * nLimit;
                        end = start + (nLimit-1);
                    }
                    else start = 0;
                }
            }

            j++;
        }
    }
    if (all_file) __ax_free(all_file);    

    if(total < 0){
        perror("scandir");
        //if (all_file) __ax_free(all_file);
        if (temp_file) __ax_free(temp_file);
            return -1;
    }
    else{
        int total_obj = 0;
        JSON_FILE_OBJECT *file=NULL;
        if(total > end)
            total_obj = end - start + 1;
        else if(total <= end && total >= start)
            total_obj = total - start;
        else {
            //if (all_file) __ax_free(all_file);
            if (temp_file) __ax_free(temp_file);
            return -1;
        }

        file = (JSON_FILE_OBJECT *)__ax_malloc(total_obj * sizeof(JSON_FILE_OBJECT));
        memset(file, 0, (total_obj * sizeof(JSON_FILE_OBJECT)));
        for (i=0; i<total_obj; i++) {
            strcpy((file+i)->filename, (temp_file+start)->filename);
            (file+i)->isfolder = (temp_file+start)->isfolder;
            (file+i)->filesize = (temp_file+start)->filesize;
            strcpy((file+i)->owner, (temp_file+start)->owner);
            strcpy((file+i)->group, (temp_file+start)->group);
            (file+i)->mt = (temp_file+start)->mt;
            strcpy((file+i)->mt_str, (temp_file+start)->mt_str);
            strcpy((file+i)->privilege, (temp_file+start)->privilege);
            start++;
        }
        if(temp_file) __ax_free(temp_file);
        File_List_To_JSON(file, total_obj, total, permission, 0);
        if(file) __ax_free(file);
    }

    return 0;
}

int handle_file_list(WFM2_FILE_LIST_PARAM *aListParam, WFM2_FILE_OBJECT *aFileObj)
{
    char    *full_path=NULL;
    int     exist, i, is_iso=0;
    //SECTION_INFO *shareList=NULL;
    char    *node1=NULL, *iso=NULL;
    int     nLen=0, nLen1=0;

    // check iso section
    node1 = strstr(&aListParam->node[1], "/");
    nLen = strlen(aListParam->node);
    if(node1 != NULL) {
        nLen1 = strlen(node1);
        iso = __ax_calloc(1, (nLen - nLen1 + 1));
        strncpy(iso, &aListParam->node[1], (nLen - nLen1));
    }
    else {
        iso = __ax_calloc(1, (nLen + 1));
        strncpy(iso, &aListParam->node[1], nLen);
    }

    is_iso = 0;
    /*
    nShareCount = Get_NAS_Share_List_V2_ISO(&shareList, 0, 0, NULL, ISO_FILTER_ONLY, &nTotalShare);
    for(i=0; i<nTotalShare; i++){
        if (!strcasecmp(iso, shareList[i].section_name)) {
            is_iso = 1;
            break;
        }
    }
    if(shareList != NULL)   Release_List(shareList);
    */
    if(iso) __ax_free(iso);

    /*The first request node is "/", but we do not display folder in "/", so
     * give null share json*/
    if (!strcmp(aListParam->node, "/")) {
        File_List_To_JSON(NULL, 0, 0, 0, 0);
        return -1;
    }
/*
    node_tmp = __ax_strdup(aListParam->node);
    g_access = Get_NAS_User_Security_For_Share_Ex(aListParam->remoteUser, strtok(node_tmp, delim));
#ifdef STORAGE_V2
    if(g_access == SHARE_NOACCESS && aListParam->no_check == 0)
#else
        if(g_access == SHARE_NOACCESS)
#endif
        {
            Status_To_JSON(WFM2_PERMISSION_DENY);
            if(node_tmp)
                __ax_free(node_tmp);
            return -1;
        }
    if(node_tmp)
        __ax_free(node_tmp);
*/

    //nEnd = nStart + nLimit - 1;
    aListParam->end = aListParam->start + aListParam->limit - 1;

    full_path = (char *)__ax_malloc(strlen(aListParam->node) + MAX_FILE_LENGTH + PATH_PREFIX_LENGTH + 256);
    exist = check_exist(aListParam->node, full_path);
    if(exist < 0)
    {
#ifdef STORAGE_V2
        if(aListParam->no_check == 0)
#endif
#ifdef LIBRSYNC 
            if(aListParam->view_versioning == 0)
#endif
            {
                Status_To_JSON(WFM2_FILE_NO_EXIST);
                if(full_path) __ax_free(full_path);
                return -1;
            }
    }

#ifdef  ACL
    //if not iso folder, check permission
    //the external disk and non-ext3 or non-ext4 vdd are not
    //supported for ACL
    //so do not check the acl permission
    if(Is_ACL_Enabled() && No_Support_ACL(aListParam->node) == 0) {
#ifdef LIBRSYNC 
        if(aListParam->view_versioning == 0)
#endif
            if(!is_iso) {
                aListParam->permission = Get_ACL_User_Group_Security_For_Share(aListParam->remoteUser, full_path);
                if(aListParam->permission < 5) {
                    Status_To_JSON(WFM2_PERMISSION_DENY);
                    if (full_path) __ax_free(full_path);
                    return -1;
                }
            }
    }
    else
        nPermission = g_access;
#endif // ACL

    for (i=0; i<sizeof(sort_func)/sizeof(SORT_FUNC); i++) {
        if (!strcmp(sort_func[i].sort_mode, aListParam->sort)) {
            aListParam->sortmode = i;
            break;
        }
    }

#ifdef STORAGE_V2
    if (aListParam->no_check) {
        if(access(aListParam->node, 0) < 0) {
            Status_To_JSON(WFM2_FILE_NO_EXIST);
            if (full_path)
                __ax_free(full_path);

            return -1;
        } else {
            _get_file_list(aListParam->node, aListParam, aFileObj);
        }
    } else
        _get_file_list(full_path, aListParam, aFileObj);
#else
    _get_file_list(full_path, aListParam, aFileObj);
#endif

    if(full_path)
        __ax_free(full_path);
    return 0;
}

