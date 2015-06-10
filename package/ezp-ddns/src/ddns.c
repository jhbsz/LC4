#include <stdio.h>
#include <string.h>
#include "ddns.h"
#include "nas_hash.h"
#include "ezp-lib.h"

#define BUF_SIZE 256
#define	QNAPDDNS_MD5_STR	"QNAP"
#define QNAPDDNS_VERSION        "1.1"
#define NAS_HASH_MD5 1

char tmpfile_aximddns[]={"/tmp/temp.XXXXXX"};
char server_url[BUF_SIZE]={"https://www.mycloudnas.com/cgi-bin/dnsRequest.cgi"};	
char wan_ip[64]={0};

static char *register_key_word[] = {
	"www", "ftp", "mail", "shop", "file", "files", "qnap", 
	"wiki", "community", "forum", "download", "firmware", 
	"RD", "DQV", "Sales", "hr", "finance", "accounting",
	"warehouse", "NVR", "NMP", "NAS", "demo", "tsd", "support", 
	"doc", "marketing", "surveillance", "ODM", "OEM", "buffalo", 
	"synology", "thecus", "iomaga", "wd", "seagate", "dlink", 
	"netgear", "readynas", "\0"
};

static char *domain_name_server[] = {
	"mycloudwifi.com",
	"mycloudnas.com",
	"myqnapnas.com",
	"qcloudnas.com",
	"\0"
};


int matches(const char *cmd, const char *pattern)
{
    int len = strlen(cmd);
    if (len > strlen(pattern))
        return -1;
    return memcmp(pattern, cmd, len);
}

static const struct cmd {
    const char *cmd;
    int (*func)(int argc, char **argv);
} cmds[] = {
    { "check_name", do_check_mycloudwifi_name },
    { "set_name",  	do_set_mycloudwifi_ddns },
    { "disable_name", do_disable_mycloudwifi_ddns },
    { "update_name", do_update_mycloudwifi_ddns },
    { 0 }
};

static int do_cmd(const char *argv0, int argc, char **argv)
{
    const struct cmd *c;

    for (c = cmds; c->cmd; ++c) {
        if (matches(argv0, c->cmd) == 0)
            return c->func(argc-1, argv+1);
    }

    printf("Object %s is unknown.\n", argv0);
    return -1;
}


char* skip_white_space(char* pstr)
{
		return pstr;
}
//0:success 1:is not valid 2:is used
int is_valid_mycloudwifi_name(char *name)
{
	int i = 0, name_len = 0;
	
	if(name == NULL){
		return 1;
	}
	
	name_len = strlen(name);
	if((name_len < 3) || (name_len > 30)){
		return 1;
	}

	for(i=0; i< name_len; i++){
		if(!isalnum(name[i])){	//is not 0~9 a~z A~Z
			if(name[i] != '.'){
				return 1;
			}
		}
	}
	
	i = 0;
	while(strlen(register_key_word[i]) > 0){
		if(!strcasecmp(register_key_word[i], name)){
			return 2;
		}
		i++;
	}
	
	return 0;
}
static int get_mac_address(char *mac, int mac_len)
{
    char tmp[64]={0}, *tmp_str;
    char target[] = ":";
    FILE *file;

    if ((file = fopen("/proc/ezp_machine_id", "r"))) {
        fgets(tmp, sizeof(tmp), file);
        tmp[17]='\0';
        tmp_str = strtok(tmp,target);
        sprintf(mac,"%s",tmp_str);
        while(tmp_str != NULL)
        {
            tmp_str = strtok( NULL,target);
            if(tmp_str != NULL)
                sprintf(mac,"%s%s",mac,tmp_str);
        }
        return AXIMDDNS_SUCCESS; 
    } else {
        return AXIMDDNS_ERR_NO_WAN_MAC;
    }
}


static int get_serial_number(char *serial_number, int serial_number_len)
{
    char *tmp_str;

    tmp_str=nvram_get("model");
    sprintf(serial_number,"%s",tmp_str);
    if(serial_number[0] != NULL)
    {
        return AXIMDDNS_SUCCESS;
    }else{
        return AXIMDDNS_ERR_MODELNAME;
    }

}
static int _encrypt(char *data, char *e_data, int e_data_len)
{
	char md_str[33]={'\0'};

	NAS_Hash_Hex_Ex(data, md_str, sizeof(md_str), NAS_HASH_MD5);
	strncpy(e_data, md_str, e_data_len);
	return 0;
}

static int gen_encrypt_key(const char *serial_number, const char *mac, char *key, int key_size)
{
	char  data[256];
    snprintf(data, sizeof(data), "%s%s%s%s", serial_number, QNAPDDNS_MD5_STR, wan_ip, mac);
	return _encrypt(data, key, key_size);
}
void log_aximddns_message(int status)
{
    if(status == AXIMDDNS_SUCCESS)
        printf("Message:Setting success\n");
    else if(status == QNAPDDNS_ERR_NAME_USED)
        printf("Error Message:The user name has been used\n");
    else if(status == QNAPDDNS_ERR_NAME_FAIL)           
        printf("Error Message:The user name is net allow to use\n");
    else if(status == QNAPDDNS_ERR_NETWORK_OUT)        
        printf("Error Message:Network timeout\n");
    else if(status == QNAPDDNS_ERR_SERVER_BUZY)        
        printf("Error Message:AXIMDDNS is busy\n");
    else if(status == QNAPDDNS_ERR_SN_MAC_FAIL)      
        printf("Error Message:Your serial number or mac address is error\n");
    else if(status == QNAPDDNS_ERR_OPEN_FILE ||
            status == AXIMDDNS_ERR_NO_LAN_MAC ||       
            status == AXIMDDNS_ERR_NO_WAN_MAC ||       
            status == AXIMDDNS_ERR_ARGV ||          
            status == AXIMDDNS_ERR_ARGC )          
        printf("Error Message:Setting failure\n");
#if 0    
    else if(status == QNAPDDNS_ERR_INPUT)                
    else if(status == QNAPDDNS_ERR_NOUSER)                
    else if(status == QNAPDDNS_ERR_OUT_OF_MEMORY)  
    else if(status == QNAPDDNS_ERR_NO_USERNAME)       
    else if(status == QNAPDDNS_ERR_USER_IS_EXIST)       
    else if(status == QNAPDDNS_ERR_NO_EMAIL)        
    else if(status == QNAPDDNS_ERR_NO_SMTP)      
    else if(status == QNAPDDNS_ERR_NO_MAILADDR)
#endif      


}
int do_check_mycloudwifi_name(int argc, char **argv)
{
	char dname[64]={0};
    int ret=0;

	if (argc < 2)
	{
		printf("do_check_mycloudwifi_name ARG ERROR USE");
		return AXIMDDNS_ERR_ARGC;
	}
	if(*argv != NULL && *(argv+1) != NULL)
	{	
		strncpy(wan_ip,*argv,sizeof(wan_ip));
		strncpy(dname,*(argv+1),sizeof(dname));
	}
	else
	{
		printf("do_check_mycloudwifi_name INPUT ERROR\n");
		return AXIMDDNS_ERR_ARGV;
	}
    
	ret = check_mycloudwifi_name_ex(dname,domain_name_server[0]);
	if(ret == QNAPDDNS_ERR_SN_MAC_FAIL){
		ret = check_mycloudwifi_name_ex(dname,domain_name_server[0]);
		if(ret == QNAPDDNS_ERR_SN_MAC_FAIL){
			ret = QNAPDDNS_ERR_NETWORK_OUT;
		}
	}
    log_aximddns_message(ret);
	return ret;
}
int do_disable_mycloudwifi_ddns(int argc, char **argv)
{
    int ret=0;
    
    ret=disable_mycloudwifi_ddns();
    log_aximddns_message(ret);

    return ret;
}

int do_set_mycloudwifi_ddns(int argc, char **argv)
{
    char dname[64]={0};
    int ret = 0;
    if (argc < 2)
    {
        printf("do_set_mycloudwifi_ddns wrong use");
		return AXIMDDNS_ERR_ARGC;
    }
    if(*argv != NULL && *(argv+1) != NULL)
    {
        strncpy(wan_ip,*argv,sizeof(wan_ip));
        strncpy(dname,*(argv+1),sizeof(dname));
    }
    else
    {
        printf("do_set_mycloudwifi_ddns input error\n");
		return AXIMDDNS_ERR_ARGV;
    }
    ret=set_mycloudwifi_ddns(dname,domain_name_server[0]);
    log_aximddns_message(ret);
    return ret;

}
int do_update_mycloudwifi_ddns(int argc, char **argv)
{
    char dname[64]={0};
    int ret = 0;
    if (argc < 3)
    {
        printf("do_update_mycloudwifi_ddns wrong use");
		return AXIMDDNS_ERR_ARGC;
    }
    if(*argv != NULL && *(argv+1) != NULL && *(argv+2) != NULL)
    {
        strncpy(wan_ip,*argv,sizeof(wan_ip));
        strncpy(dname,*(argv+1),sizeof(dname));
    }
    else
    {
        printf("do_update_mycloudwifi_ddns input error\n");
		return AXIMDDNS_ERR_ARGV;
    }
    if(strcmp("init",*(argv+2)))
    {
        create_update_name_wanip_postfile(tmpfile_aximddns,dname,domain_name_server[0],wan_ip);
        ret = update_mycloudwifi_ddns(tmpfile_aximddns);
        unlink(tmpfile_aximddns);
    } 
    else 
    {
        create_update_wanip_postfile(tmpfile_aximddns,wan_ip);
        ret = update_mycloudwifi_ddns(tmpfile_aximddns);
        unlink(tmpfile_aximddns);
    }
    log_aximddns_message(ret);

    return ret;
}

int check_mycloudwifi_name_ex(char *dname, char *qname)
{
	int ret = QNAPDDNS_ERR_NAME_FAIL, ret_val = QNAPDDNS_ERR_NETWORK_OUT;
	char cmd[BUF_SIZE*2],tmp_buffer[512]; 
	char *p, *q, serial_number[64], key[64] ,mac[64];
	FILE *pipe = NULL, *fpt = NULL;
	char * retp = NULL;

	ret = is_valid_mycloudwifi_name(dname);
	if(ret == 1){	
		return QNAPDDNS_ERR_NAME_FAIL;
	}
	if(ret == 2){	
		printf("QNAPDDNS_ERR_NAME_USED\r\n");
		return QNAPDDNS_ERR_NAME_USED;
	}
	if ((fpt = fopen(tmpfile_aximddns, "w+"))==NULL){
		return QNAPDDNS_ERR_OPEN_FILE;
	}
	fprintf(fpt, "subfunc=check\n");
	get_serial_number(serial_number, sizeof(serial_number));
	fprintf(fpt, "&sn=%s\n", serial_number);
	get_mac_address(mac, sizeof(mac));
	fprintf(fpt, "&mac=%s\n", mac);
	fprintf(fpt, "&dname=%s\n", dname);
	fprintf(fpt, "&qname=%s\n", qname);
	gen_encrypt_key(serial_number, mac, key, sizeof(key));
	fprintf(fpt, "&key=%s\n", key);
	fprintf(fpt, "&ver=%s\n", QNAPDDNS_VERSION);
	fclose(fpt);
		
	ret = QNAPDDNS_ERR_NETWORK_OUT;
	sprintf(cmd, "/usr/bin/curl -k -4 --connect-timeout 30 --data \"@%s\" --url %s 2>/dev/null", tmpfile_aximddns, server_url);
	pipe = popen(cmd, "r");
	if(pipe){
        do{
            retp = fgets(tmp_buffer,sizeof(tmp_buffer), pipe);
            if(retp != NULL) {
                if((p = strstr(tmp_buffer, "<returnCode>"))!= 0){
                    q = strstr(tmp_buffer,"</returnCode>");
                    *q = '\0';
                    p = p + strlen("<returnCode>");
                    ret_val = atoi(p);
                    break;
                }

			}
		} while (retp);
		
        pclose(pipe);
		switch(ret_val){
			case 0:
				ret = 0;
				break;
			case CGIERROR_USED:
				ret = QNAPDDNS_ERR_NAME_USED;
				break;
			case CGIERROR_KEY_NOT_MATCH:
				ret = QNAPDDNS_ERR_SN_MAC_FAIL;
				break;
			case CGIERROR_BASE:
			case CGIERROR_UNKNOWN_FUNC:
			case CGIERROR_PARAMETER_NOT_FOUND:
			case CGIERROR_NO_DATA:
			case CGIERROR_DB_NOT_FOUND:
			case CGIERROR_UNKNOWN_ERROR:
			default:
				ret = QNAPDDNS_ERR_NETWORK_OUT;
		}
	}
	unlink(tmpfile_aximddns);
    //printf("ret is %d\n",ret);
	return ret;
}

// 0:success other:fail
int set_mycloudwifi_ddns(char *dname, char *qname)
{
	int ret = QNAPDDNS_ERR_NAME_FAIL, ret_val = QNAPDDNS_ERR_NETWORK_OUT;
	char cmd[BUF_SIZE*2];
	char *p, *q, serial_number[64], key[64], mac[64], tmp_buffer[512];
    char * retp = NULL;
	FILE *pipe = NULL, *fpt = NULL;
	
    ret = is_valid_mycloudwifi_name(dname);
	if(ret == 1){	
		return QNAPDDNS_ERR_NAME_FAIL;
	}
	if(ret == 2){	
		return QNAPDDNS_ERR_NAME_USED;
	}
	if ((fpt = fopen(tmpfile_aximddns, "w+"))==NULL){
		return QNAPDDNS_ERR_OPEN_FILE;
	}
	fprintf(fpt, "subfunc=set\n");
	get_serial_number(serial_number, sizeof(serial_number));
	fprintf(fpt, "&sn=%s\n", serial_number);
	get_mac_address(mac, sizeof(mac));
	fprintf(fpt, "&mac=%s\n", mac);
	fprintf(fpt, "&dname=%s\n", dname);
	fprintf(fpt, "&qname=%s\n", qname);
	gen_encrypt_key(serial_number, mac, key, sizeof(key));
	fprintf(fpt, "&key=%s\n", key);
	fprintf(fpt, "&ver=%s\n", QNAPDDNS_VERSION);
	fclose(fpt);
	
	ret = QNAPDDNS_ERR_NETWORK_OUT;
	sprintf(cmd, "/usr/bin/curl -k -4 --connect-timeout 30 --data \"@%s\" --url %s 2>>/dev/null", tmpfile_aximddns, server_url);
	pipe = popen(cmd, "r");
    if(pipe)
    {
        do{
            retp = fgets(tmp_buffer,sizeof(tmp_buffer), pipe);
            if(retp != NULL) 
            {
                if((p = strstr(tmp_buffer, "<returnCode>"))!= 0)
                {
                    q = strstr(tmp_buffer,"</returnCode>");
                    *q = '\0';
                    p = p + strlen("<returnCode>");
                    ret_val = atoi(p);
                    break;
                }
            }   
        } while (retp);

		pclose(pipe);
		switch(ret_val){
			case 0:
				ret = 0;
				break;
			case CGIERROR_USED:
				ret = QNAPDDNS_ERR_NAME_USED;
				break;
			case CGIERROR_BASE:
				ret = QNAPDDNS_ERR_NETWORK_OUT;
				break;
			case CGIERROR_UNKNOWN_FUNC:
			case CGIERROR_PARAMETER_NOT_FOUND:
			case CGIERROR_NO_DATA:
			case CGIERROR_DB_NOT_FOUND:
			case CGIERROR_UNKNOWN_ERROR:
				ret = QNAPDDNS_ERR_SERVER_BUZY;
				break;
			case CGIERROR_KEY_NOT_MATCH:
				ret = QNAPDDNS_ERR_SN_MAC_FAIL;
				break;
			default:
				ret = QNAPDDNS_ERR_NETWORK_OUT;
		}
    }
	unlink(tmpfile_aximddns);
	return ret;
}

// 0:success other:fail
int disable_mycloudwifi_ddns(void)
{
	int ret = QNAPDDNS_ERR_NETWORK_OUT, ret_val = QNAPDDNS_ERR_NETWORK_OUT;
	char cmd[BUF_SIZE*2],tmp_buffer[512];
	char *p, *q, serial_number[64], mac[32], key[64],*retp = NULL;
	FILE *pipe = NULL, *fpt = NULL;
	
	if ((fpt = fopen(tmpfile_aximddns, "w+"))==NULL){
		return QNAPDDNS_ERR_OPEN_FILE;
	}
	fprintf(fpt, "subfunc=disable\n");
	get_serial_number(serial_number, sizeof(serial_number));
	fprintf(fpt, "&sn=%s\n", serial_number);
	get_mac_address(mac, sizeof(mac));
	fprintf(fpt, "&mac=%s\n", mac);
	gen_encrypt_key(serial_number, mac, key, sizeof(key));
	fprintf(fpt, "&key=%s\n", key);
	fprintf(fpt, "&ver=%s\n", QNAPDDNS_VERSION);
	fclose(fpt);
	
	sprintf(cmd, "/usr/bin/curl -k -4 --connect-timeout 30 --data \"@%s\" --url %s 2>>/dev/null", tmpfile_aximddns, server_url);
	pipe = popen(cmd, "r");
    if(pipe)
    {
        do
        {
            retp = fgets(tmp_buffer,sizeof(tmp_buffer), pipe);
            if(retp != NULL) 
            {
                if((p = strstr(tmp_buffer, "<returnCode>"))!= 0)
                {
                    q = strstr(tmp_buffer,"</returnCode>");
                    *q = '\0';
                    p = p + strlen("<returnCode>");
                    ret_val = atoi(p);
                    break;
                }
            }
        } while (retp);
    pclose(pipe);
		switch(ret_val){
			case 0:
				ret = 0;
				break;
			case CGIERROR_USED:
				ret = QNAPDDNS_ERR_NAME_USED;
				break;
			case CGIERROR_BASE:
				ret = QNAPDDNS_ERR_NETWORK_OUT;
				break;
			case CGIERROR_UNKNOWN_FUNC:
			case CGIERROR_PARAMETER_NOT_FOUND:
			case CGIERROR_NO_DATA:
			case CGIERROR_DB_NOT_FOUND:
			case CGIERROR_UNKNOWN_ERROR:
				ret = QNAPDDNS_ERR_SERVER_BUZY;
				break;
			case CGIERROR_KEY_NOT_MATCH:
				ret = QNAPDDNS_ERR_SN_MAC_FAIL;
				break;
			default:
				ret = QNAPDDNS_ERR_NETWORK_OUT;
        }
	}
	unlink(tmpfile_aximddns);
	return ret;
}

// 0:success other:fail
int create_update_name_wanip_postfile(char *postfile, char *dname, char *qname,char *wan_ip)
{
    char serial_number[64], mac[32], key[64];
    FILE *fpt = NULL;
    
    if ((fpt = fopen(postfile, "w+"))==NULL){
        return QNAPDDNS_ERR_OPEN_FILE;
    }
    fprintf(fpt, "subfunc=set\n");
    fprintf(fpt, "&dname=%s\n", dname);
    fprintf(fpt, "&qname=%s\n", qname);
    get_serial_number(serial_number, sizeof(serial_number));
    fprintf(fpt, "&sn=%s\n", serial_number);
    get_mac_address(mac, sizeof(mac));
    fprintf(fpt, "&mac=%s\n", mac);
    gen_encrypt_key(serial_number, mac, key, sizeof(key));
    fprintf(fpt, "&key=%s\n", key);
    fprintf(fpt, "&wanIP=%s\n", wan_ip);
    fprintf(fpt, "&ver=%s\n", QNAPDDNS_VERSION);
    fclose(fpt);

    return 0;
}

// 0:success other:fail
int create_update_wanip_postfile(char *postfile, char *wan_ip)
{
	char serial_number[64], mac[32], key[64];
	FILE *fpt = NULL;
	
    if ((fpt = fopen(postfile, "w+")) == NULL)
    {
		return QNAPDDNS_ERR_OPEN_FILE;
	}
    fprintf(fpt, "subfunc=update\n");
	get_serial_number(serial_number, sizeof(serial_number));
	fprintf(fpt, "&sn=%s\n", serial_number);
	get_mac_address(mac, sizeof(mac));
	fprintf(fpt, "&mac=%s\n", mac);
	gen_encrypt_key(serial_number, mac, key, sizeof(key));
	fprintf(fpt, "&key=%s\n", key);
	fprintf(fpt, "&wanIP=%s\n", wan_ip);
	fprintf(fpt, "&ver=%s\n", QNAPDDNS_VERSION);
	fclose(fpt);
	
	return 0;
}

// 0:success other:fail
int update_mycloudwifi_ddns(char *postfile)
{
	int ret = QNAPDDNS_ERR_NETWORK_OUT, ret_val = QNAPDDNS_ERR_NETWORK_OUT;
	char cmd[BUF_SIZE*2], *p, *q, *retp,tmp_buffer[512];
	FILE *pipe = NULL, *fpr = NULL;
	
	fpr = fopen(postfile, "r+");
	if(fpr){
		fclose(fpr);
		ret = QNAPDDNS_ERR_NETWORK_OUT;
		sprintf(cmd, "/usr/bin/curl -k -4 --connect-timeout 30 --data \"@%s\" --url %s 2>>/dev/null", postfile, server_url);
		pipe = popen(cmd, "r");

    if(pipe)
    {			
        do
        {
            retp = fgets(tmp_buffer,sizeof(tmp_buffer), pipe);
            if(retp != NULL)
            {
                if((p = strstr(tmp_buffer, "<returnCode>"))!= 0)
                {
                    q = strstr(tmp_buffer,"</returnCode>");
                    *q = '\0';
                    p = p + strlen("<returnCode>");
                    ret_val = atoi(p);
                    break;
                }
            }
        } while (retp);
        pclose(pipe);
			switch(ret_val){
				case 0:
					ret = 0;
					break;
				case CGIERROR_USED:
					ret = QNAPDDNS_ERR_NAME_USED;
					break;
				case CGIERROR_BASE:
					ret = QNAPDDNS_ERR_NETWORK_OUT;
					break;
				case CGIERROR_UNKNOWN_FUNC:
				case CGIERROR_PARAMETER_NOT_FOUND:
				case CGIERROR_NO_DATA:
				case CGIERROR_DB_NOT_FOUND:
				case CGIERROR_UNKNOWN_ERROR:
					ret = QNAPDDNS_ERR_SERVER_BUZY;
					break;
				case CGIERROR_KEY_NOT_MATCH:
					ret = QNAPDDNS_ERR_SN_MAC_FAIL;
					break;
				default:
					ret = QNAPDDNS_ERR_NETWORK_OUT;
			}
		}
	}
	return ret;
}

int main(int argc, char **argv)
{
		/* Skip program name */
    int ret;
    if(argc<3)
    {
        printf("too few arguments");
    		return 0;
    }
    if (argc > 2)
    {
        ret = do_cmd(argv[1], argc-1, argv+1);
        return ret;
    }
}
