//test.c: demonstration program using the BQ gauge and i2c interface

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ezp-i2c.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#include <linux/ezp-gpio.h>
#include <linux/ioctl.h>

#include "ezp-lib.h"

#define TMP_LEN 256
#define LONG_TMP_LEN 2048
#define MCU_COMMUNICATION_GPIO  42
#define SUB_CMD_ON      "on"
#define SUB_CMD_OFF     "off"
#define SUB_CMD_START   "start"
#define SUB_CMD_FINISH  "finish"
#define CMD_LEN(_CMD_)  (sizeof(_CMD_) - 1)

// #define USE_GPIO_ACQUIRE_SESSION
// #define I2C_COMMAND_DEBUG
typedef unsigned char   u8;
typedef signed char     s8;

int result=0;
int w_len=0;
int r_len=0;
unsigned char i2c_addr=0x00;
int check_and_acquire_mcu(long wait_msecs);
int release_mcu(int mcu_gpio_fd);
char convert_char(char c);

void log_msg_file(char *filename, char *msg) {
    FILE *logf = fopen(filename, "a+");
    if(logf) {
        fprintf(logf, "%s", msg);
        fclose(logf);
    }
}

int seti2caddr(unsigned char addr)
{
	int retval;
	
	//ioctl to set slave address
	retval = ioctl(i2cfile,RT2880_I2C_SET_ADDR,addr);

	//if we fail, tell the user and return the error code
	if(retval  < 0) {
		printf("Could not set slave address\n");
		return retval;
	}
	
	//set our current address variable to the new address
	//this is necessary because the IOCTL_RDWR ioctl requires you to specify the address for each message
	//since this address might change (if we enter ROM mode on the BQ device), we need to keep track of
	//which one we are using.
	return retval;
}
int init_i2c()
{
	char filename[20];
	int retval;
	
	snprintf(filename, 19, "/dev/i2c");
	
	//open the file and store it to our variable
	i2cfile = open(filename, O_RDWR);
	
	//if we failed, inform the reader and return the handle value
	if( i2cfile < 0) {
		printf("Couldread not open i2c device file\n");
		retval = i2cfile;
		return retval;
	}
	return retval;
}

void release_i2c(void) {
    close(i2cfile);
    i2cfile = -1;
}

int read_i2c(unsigned char *buf)
{
	int retval;
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    int mcu_gpio = -1;
#endif
    seti2caddr(i2c_addr);
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    mcu_gpio = check_and_acquire_mcu(0);
    if(mcu_gpio <= 0) {
        printf("Can not acquire mcu gpio\n");
        return mcu_gpio;
    }
#endif
	retval = ioctl(i2cfile,rt2880_i2c_read,buf);
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    release_mcu(mcu_gpio);
#endif
	
	//if we fail, tell the user, close the file, and return the error code
	if( retval < 0)
	{
		printf("Could not read from I2C\n");
	}
#ifdef I2C_COMMAND_DEBUG
    int chk_buf_index;
    printf("Cmd:%d Count:%x\n", rt2880_i2c_read, buf[0]);
    for(chk_buf_index = 0; chk_buf_index < buf[0]; chk_buf_index++) {
        printf("[%d]:%x\t", chk_buf_index + 1 ,buf[chk_buf_index + 1]);
    }
    printf("\n");
#endif
	
	return retval;
	
}
int write_i2c(unsigned char *buf)
{
	int retval;
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    int mcu_gpio = -1;
#endif
#ifdef I2C_COMMAND_DEBUG
    int chk_buf_index;
    printf("Cmd:%d Count:%x\n", rt2880_i2c_write, buf[0]);
    for(chk_buf_index = 0; chk_buf_index < buf[0]; chk_buf_index++) {
        printf("[%d]:%x\t", chk_buf_index + 1 ,buf[chk_buf_index + 1]);
    }
    printf("\n");
#endif
    seti2caddr(i2c_addr);
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    mcu_gpio = check_and_acquire_mcu(0);
    if(mcu_gpio <= 0) {
        printf("Can not acquire mcu gpio\n");
        return mcu_gpio;
    }
#endif
	retval = ioctl(i2cfile,rt2880_i2c_write,buf);
#if defined(PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
    release_mcu(mcu_gpio);
#endif
	
	//if we fail, tell the user, close the file, and return the error code
	if( retval < 0)
	{
		printf("Could not write to I2C\n");
	}
	
	return retval;
}
int normal_read_i2c(unsigned char *buf, int mode)
{
	int retval;
    seti2caddr(i2c_addr);
    retval = ioctl(i2cfile, RT2880_I2C_NORMAL_READ + mode - 1 , buf);
	//if we fail, tell the user, close the file, and return the error code
	if( retval < 0)
	{
		printf("Could not read from I2C, ret:%d\n", retval);
	}
	return retval;
}
int normal_write_i2c(unsigned char *buf, int mode)
{
	int retval;
    seti2caddr(i2c_addr);
    printf("send ioctl %d RT2880_I2C_NORMAL_WRITE:%d RT2880_I2C_NORMAL_WRITE2:%d\n",
            RT2880_I2C_NORMAL_WRITE + mode - 1, RT2880_I2C_NORMAL_WRITE, RT2880_I2C_NORMAL_WRITE2);
    retval = ioctl(i2cfile, RT2880_I2C_NORMAL_WRITE + mode - 1 , buf);
	//if we fail, tell the user, close the file, and return the error code
	if( retval < 0)
	{
		printf("Could not read from I2C\n");
	}
	return retval;
}
void read_and_return(unsigned char *read) {
    read[0]=1;
    read_i2c(read);
    printf("Data Read: 0x%02X \n",read[1]);
    result=read[1];
}
void print_usage() {
    printf("ezp-i2c [gauge|security|write|charger|fw] ...\n");
    printf("ezp-i2c gauge [voltage|host|client|state|fw|read|percentage|current|green_led|red_led] ...\n");
    printf("ezp-i2c gauge host [sleep|booting|ready|wifioff|clear|reboot|recovery]\n");
    printf("ezp-i2c gauge read [address] [number]\n");
    printf("ezp-i2c gauge green_led [on|off]\n");
    printf("ezp-i2c gauge red_led [on|off]\n");
    printf("ezp-i2c gauge temp\n");
    printf("ezp-i2c gauge temp_offset XX, XX is a number between -12.8 to 12.7\n");
#ifdef PLATFORM_M2E
    printf("ezp-i2c gauge oled [on|off]\n");
    printf("ezp-i2c gauge upgrade [start|finish]\n");
    printf("ezp-i2c wifi [user|usb_1|usb_2|qsync|sd|storage|ssid]\n");
    printf("ezp-i2c internet [on|off]\n");
    printf("ezp-i2c wan ip\n");
    printf("ezp-i2c lan ip\n");
#endif
    printf("ex: ezp-i2c gauge read 0x04 4\n");
    printf("ezp-i2c write [I2C addr] [number] [data0] [data1]....\n");
    printf("ex: ezp-i2c write 0x58 2 0x02 0x01\n");
    close(i2cfile);
    exit(1);
}
/*
Input :
    u8 i2c_dev_addr     : i2c device address, 0x58 for guage
    int wlen, u8* wbuf  : write buffer and bytes to write, 
                          *Must be allocated because we will change the content
    int rlen, u8* rbuf  : Read buffer and bytes to read,                    
                          *Must be allocated because we will change the content
Output :
    wbuf will contain "0xff" on byte i if i-th byte write failed
    rbuf will contain "0xff" in all read bytes if any byte read failed
Return :
    -1  : write failed
    -2  : read failed
    0   : if only write no read, then return 0 on write success
    other : first read byte
 */
int i2c_send_and_read(u8 i2c_dev_addr, u8* wbuf, u8* rbuf) {
    int i2cres = 0, i;
    i2c_addr = i2c_dev_addr;
    if(wbuf && wbuf[0]) {
        i2cres = write_i2c(wbuf);
        if(i2cres < 0) {
            fprintf(stderr, "write request failed, write len:%d first byte:0x%02x i2cres:%d\n", wbuf[0], wbuf[1], i2cres);
            return -1;
        }
    }
    if(rbuf && rbuf[0]) {
        usleep(500000);
        i2cres = read_i2c(rbuf);
        if(i2cres < 0) {
            fprintf(stderr, "read request failed, read len:%d first byte:0x%02x i2cres:%d\n", rbuf[0], rbuf[1], i2cres);
            return -2;
        }
    }
    return 0;
}

typedef int (*i2c_cmd_handler_func)(int, char *[]);
typedef struct _i2c_cmd_handler {
    char *command;
    i2c_cmd_handler_func func;
    bool do_retry;
} i2c_cmd_handler;

int gauge_temp(int argc, char *argv[]) {
#define INIT_TEMPERATURE_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x14;__BUF__[2] = 0;}
#define INIT_TEMPERATURE_RESP(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0xFF;__BUF__[2] = 0;__BUF__[3] = 0;}
    float temperature = 0;
    FILE *pFile = NULL;
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char output_string[30];

    i2c_addr = 0x58;
    pFile =  fopen("/tmp/gauge_temp", "r");
    if(pFile) {
        if(0 >= fscanf(pFile, "%f", &temperature)) {
            temperature = 30;
        }
        fclose(pFile);
        pFile = NULL;
    }
    INIT_TEMPERATURE_REQU(wbuf);
    INIT_TEMPERATURE_RESP(rbuf);
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    if(result >= 0) {
        float temp_flo;
        /* Write to percentage file */
        temperature = rbuf[2];
        temperature = (temperature * 256) + rbuf[1];
        temperature = temperature - 2731.5;
        temperature = temperature / 10;


        pFile =  fopen("/tmp/gauge_temp", "w");
        snprintf(output_string,sizeof(output_string),"%.2f", temperature);
        fwrite(output_string, 1 , strlen(output_string), pFile);
        fclose(pFile);
        printf("Temperature: %.2f 'C\n", 
                temperature);
    } else {
        printf("Temperature: Not Ready\n", 
                temperature);
    }
    return result;
}

int gauge_temp_offset(int argc, char *argv[]) {
#define INIT_TEMPOFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x15;__BUF__[2] = 0;__BUF__[3] = 0;}
#define INIT_TEMPOFF_RESP(__BUF__) {__BUF__[0] = 0;}
    float k_val;
    char real_k_val = 0;
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    if(!(argv[0]) || !(argv[0][0])) {
        printf("Insufficient parameters : ezp-i2c gauge temp_offset XX, XX is a number between -12.8 to 12.7.\n");
        return -1;
    }
    sscanf(argv[0], "%f", &k_val);
    if((k_val > 12.7) || (k_val < -12.80001)) {
        printf("Invalid calibration value:%s\n", argv[0]);
        return -1;
    }
    INIT_TEMPOFF_REQU(wbuf);
    INIT_TEMPOFF_RESP(rbuf);
    real_k_val = (k_val * 10);
    i2c_addr = 0x58;
    wbuf[2] = real_k_val & 0xff;
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

int gauge_oled(int argc, char *argv[]) {
#define INIT_TEMPERATURE_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x14;__BUF__[2] = 0;}
#define INIT_TEMPERATURE_RESP(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0xFF;__BUF__[2] = 0;__BUF__[3] = 0;}
    unsigned char wbuf[20];
    unsigned char rbuf[20];

    i2c_addr = 0x58;
    if (!strcmp(argv[0], "on")) {
#define INIT_OLED_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x12; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_OLED_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_OLED_ON_REQU(wbuf);
        INIT_OLED_ON_RESP(rbuf);
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else if (!strcmp(argv[0], "off")) {
#define INIT_OLED_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x12; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_OLED_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_OLED_OFF_REQU(wbuf);
        INIT_OLED_OFF_RESP(rbuf);
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else {
        print_usage();
    }
    return result;
}
#define UPGRADE_START_CMD   "start"
#define UPGRADE_FINISH_CMD  "finish"
int gauge_upgrade(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];

    i2c_addr = 0x58;
    if(argv[0] && *argv[0]) {
        /* ezp-i2c gauge upgrade start */
        if(!strncmp(argv[0], SUB_CMD_START, CMD_LEN(SUB_CMD_START))) {
#define INIT_UPGRADE_START_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x02; __BUF__[2] = 0x03;__BUF__[3] = 0;}
#define INIT_UPGRADE_START_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
            INIT_UPGRADE_START_REQU(wbuf);
            INIT_UPGRADE_START_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        } else if(!strncmp(argv[0], SUB_CMD_FINISH, CMD_LEN(SUB_CMD_FINISH))) {
#define INIT_UPGRADE_FINISH_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x02; __BUF__[2] = 0x04;__BUF__[3] = 0;}
#define INIT_UPGRADE_FINISH_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
            INIT_UPGRADE_FINISH_REQU(wbuf);
            INIT_UPGRADE_FINISH_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        } else {
            print_usage();
        }
    }
    return result;
}


i2c_cmd_handler i2c_cmd_handlers[] = {
    {"temp", gauge_temp, true},
    {"temp_offset", gauge_temp_offset, true},
#ifdef PLATFORM_M2E
    {"oled", gauge_oled, true},
    {"upgrade", gauge_upgrade, true},
#endif
    {NULL, NULL}
};

int handler_dummy(int argc, char *argv[]) {
    printf("Handler dummy\n");
    return 0;
}

int wifi_user_count(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    FILE *f;
    int counter = 0;
    char buf[LONG_TMP_LEN];

    if (argc != 0) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

    if ((f = fopen("/tmp/dhcp.leases", "r"))) {
        while(fgets(buf, sizeof(buf), f)) {
            counter++;
        }
        fclose(f);
        f = NULL;
    }
#define INIT_USER_REQU(__BUF__, __USER__) {__BUF__[0] = 2; __BUF__[1] = 0x0B; __BUF__[2] = __USER__;}
#define INIT_USER_RESP(__BUF__) {__BUF__[0] = 0;}
    INIT_USER_REQU(wbuf, counter);
    INIT_USER_RESP(rbuf);
    printf("Users %d\n", counter);
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

/*
 * Parameters :
 * Input
 * mount_string : USB/SD/Qsync
 * 
 * Output
 * insert_state : 0:not insert, 1:inserted, 2: inserted but readonly
 * free_space : return device free space in size_t
 * total_space : return device total space in size_t
 * */

void
get_storage_space_info(char *mount_string, 
        int *insert_state,
        unsigned long long *free_space, 
        unsigned long long *total_space) {
    char tmp_cmd[TMP_LEN]={0};
    char tmp_res[TMP_LEN]={0};
    char tmp_mount_point[TMP_LEN] = {0};
    unsigned long long freesp = 0, usedsp = 0, totalsp = 0;
    FILE *file;
    snprintf(tmp_cmd, sizeof(tmp_cmd), "mount | grep %s", mount_string);
    if ((file = popen(tmp_cmd, "r"))) {
        fgets(tmp_res, sizeof(tmp_res), file);
    }
    pclose(file);
    /* The df format is 
     * /dev/sdb1              3944740   3182596    762144 81% /tmp/mnt/USB 
     * $1                     $2:total  $3:used    $4:free
     * */
    if (strcmp(tmp_res, "")) { 
        if (strstr(tmp_res, "rw")) { 
            *insert_state = 2;
        } else {
            *insert_state = 1;
        }
        *free_space = 0;
        *total_space = 0;
        snprintf(tmp_cmd, sizeof(tmp_cmd), "df | grep %s", mount_string);
        if ((file = popen(tmp_cmd, "r"))) {
            fgets(tmp_res, sizeof(tmp_res), file);
            pclose(file);
        }
        sscanf(tmp_res, "%s %llu %llu %llu", tmp_mount_point, &totalsp, &usedsp, &freesp);
    } else {
        *insert_state = 0;
    }
    *free_space = freesp;
    *total_space = totalsp;
}
#define INIT_NOT_INSERT_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x0C; __BUF__[2] = 0;}
#define INIT_NOT_INSERT_RESP(__BUF__) {__BUF__[0] = 0;}
#define INIT_SPACE_REQU(__BUF__) {__BUF__[0] = 7; __BUF__[1] = 0x0C; __BUF__[2] = 1; __BUF__[3] = 0; __BUF__[4] = 0; __BUF__[5] = 0; __BUF__[6] = 0; __BUF__[7] = 0;}
#define INIT_SPACE_RESP(__BUF__) {__BUF__[0] = 0;}
#define SIZE_MB (unsigned long long)(1024)
#define SIZE_GB (unsigned long long)(SIZE_MB * 1024)
#define SIZE_TB (unsigned long long)(SIZE_GB * 1024)
#define SIZE_GB_OD (unsigned long long)(SIZE_MB * 1000)
#define SIZE_TB_OD (unsigned long long)(SIZE_GB * 1000)
#define USB_RULE_IDX 0
#define SD_RULE_IDX 1
#define INTER_RULE_IDX 2
#define FORMAT_SPACE_OUTPUT_STRING(__SPVAL__, __UNIT__, __OUTPUT_BUF__, __UNIT_TYPE__) { __OUTPUT_BUF__[3] = __SPVAL__ / 1000; \
    __OUTPUT_BUF__[4] = (__SPVAL__ % 1000) / 100; \
    __OUTPUT_BUF__[5] = (__SPVAL__ % 100) / 10; \
    __OUTPUT_BUF__[6] = (__SPVAL__ % 10); \
    __OUTPUT_BUF__[7] = __UNIT_TYPE__; \
  } \


int usb_storage_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char buf[TMP_LEN];
    FILE *f;
    unsigned long long free_space = 0, total_space = 0;
    int insert_state;

    if (argc == 1) {
    } else if(argc > 1) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

    ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        /* Card inserted */
        get_storage_space_info("USB", &insert_state, &free_space, &total_space);

    } else {
        /* Card not inserted */
        insert_state = 0;
    }
    printf("Get %s pluged:%d free:%llu total:%llu\n", "USB", insert_state, free_space, total_space) ;
    if(insert_state == 0) {
        INIT_NOT_INSERT_REQU(wbuf);
        INIT_NOT_INSERT_RESP(rbuf);
        /* USB:0x0C */
        wbuf[1] = 0x0C;
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else {
        INIT_SPACE_REQU(wbuf);
        INIT_SPACE_RESP(rbuf);
        /* USB:0x0C */
        wbuf[1] = 0x0C;
        /* For free space : 1 */
        wbuf[2] = 1;
        
        if(free_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_TB, wbuf, 2);

        } else if(free_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_GB / 10)/ SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        if(result) 
            return result;
        /* For total space : 1 */
        wbuf[2] = 2;
        if(total_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_TB, wbuf, 2);
        } else if(total_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    }
    return result;
}

int sd_storage_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char buf[TMP_LEN];
    FILE *f;
    unsigned long long free_space = 0, total_space = 0;
    int insert_state;

    if (argc == 1) {
    } else if(argc > 1) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

    ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        /* Card inserted */
        get_storage_space_info("SD", &insert_state, &free_space, &total_space);

    } else {
        /* Card not inserted */
        insert_state = 0;
    }

    printf("Get %s pluged:%d free:%llu total:%llu\n", "SD", insert_state , free_space, total_space) ;
    if(insert_state == 0) {
        INIT_NOT_INSERT_REQU(wbuf);
        INIT_NOT_INSERT_RESP(rbuf);
        /* SD:0x0E */
        wbuf[1] = 0x0E;
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else {
        int sp_quant;
        INIT_SPACE_REQU(wbuf);
        INIT_SPACE_RESP(rbuf);
        /* SD:0x0E */
        wbuf[1] = 0x0E;
        /* For free space : 1 */
        wbuf[2] = 1;
        if(free_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_TB, wbuf, 2);
        } else if(free_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        if(result) 
            return result;
        /* For total space : 1 */
        wbuf[2] = 2;
        if(total_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_TB, wbuf, 2);
        } else if(total_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    }
    return result;
}

int qsync_storage_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char buf[TMP_LEN];
    FILE *f;
    unsigned long long free_space = 0, total_space = 0;
    int insert_state;

    if (argc == 1) {
    } else if(argc > 1) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

    ezplib_get_attr_val("storage_state_rule", INTER_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        /* Card inserted */
        get_storage_space_info("Qsync", &insert_state, &free_space, &total_space);

    } else {
        /* Card not inserted */
        insert_state = 0;
    }

    printf("Get %s pluged:%d free:%llu total:%llu\n", "Qsync", insert_state, free_space, total_space) ;
    if(insert_state == 0) {
        INIT_NOT_INSERT_REQU(wbuf);
        INIT_NOT_INSERT_RESP(rbuf);
        /* Internal:0x0D */
        wbuf[1] = 0x0D;
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else {
        int sp_quant;
        INIT_SPACE_REQU(wbuf);
        INIT_SPACE_RESP(rbuf);
        /* Internal:0x0D */
        wbuf[1] = 0x0D;
        /* For free space : 1 */
        wbuf[2] = 1;
        if(free_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_TB, wbuf, 2);
        } else if(free_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_space = (free_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(free_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        if(result) 
            return result;
        /* For total space : 1 */
        wbuf[2] = 2;
        if(total_space > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_TB, wbuf, 2);
        } else if(total_space > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_space = (total_space * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(total_space, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    }
    return result;
}

int total_storage_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char buf[TMP_LEN];
    FILE *f;
    unsigned long long free_space[3] = {0,0,0}, total_space[3] = {0,0,0}, free_total = 0, total_total = 0;
    int insert_state, idx, storage_inserted = 0;

    if (argc == 1) {
    } else if(argc > 1) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

    ezplib_get_attr_val("storage_state_rule", USB_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        get_storage_space_info("USB", &insert_state, &free_space[USB_RULE_IDX], &total_space[USB_RULE_IDX]);
    }
    ezplib_get_attr_val("storage_state_rule", SD_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        get_storage_space_info("SD", &insert_state, &free_space[SD_RULE_IDX], &total_space[SD_RULE_IDX]);
    }
    ezplib_get_attr_val("storage_state_rule", INTER_RULE_IDX, 
            "insert_state", buf, sizeof(buf), EZPLIB_USE_CLI);
    if(buf[0] == '1') {
        get_storage_space_info("Qsync", &insert_state, &free_space[INTER_RULE_IDX], &total_space[INTER_RULE_IDX]);
    }

    free_total = free_space[0] + free_space[1] + free_space[2];
    total_total = total_space[0] + total_space[1] + total_space[2];

    printf("Get %s pluged:%d free:%llu total:%llu\n", "All Storage", total_total, free_total, total_total) ;
    if(total_total == 0) {
        INIT_NOT_INSERT_REQU(wbuf);
        INIT_NOT_INSERT_RESP(rbuf);
        /* All storage:0x0F */
        wbuf[1] = 0x0F;
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    } else {
        int sp_quant;
        INIT_SPACE_REQU(wbuf);
        INIT_SPACE_RESP(rbuf);
        /* All storage:0x0F */
        wbuf[1] = 0x0F;
        /* For free space : 1 */
        wbuf[2] = 1;
        if(free_total > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_total = (free_total * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(free_total, SIZE_TB, wbuf, 2);
        } else if(free_total > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_total = (free_total * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(free_total, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            free_total = (free_total * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(free_total, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        if(result) 
            return result;
        /* For total space : 1 */
        wbuf[2] = 2;
        if(total_total > SIZE_TB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_total = (total_total * 10 + 5 * SIZE_TB / 10) / SIZE_TB;
            FORMAT_SPACE_OUTPUT_STRING(total_total, SIZE_TB, wbuf, 2);
        } else if(total_total > SIZE_GB_OD) {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_total = (total_total * 10 + 5 * SIZE_GB / 10) / SIZE_GB;
            FORMAT_SPACE_OUTPUT_STRING(total_total, SIZE_GB, wbuf, 1);
        } else {
            // We reserve n-digits plus 1 points, round to the nearest
            // hundredth
            total_total = (total_total * 10 + 5 * SIZE_MB / 10) / SIZE_MB;
            FORMAT_SPACE_OUTPUT_STRING(total_total, SIZE_MB, wbuf, 0);
        }
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    }
    return result;
}

char convert_char(char c) {
    const char convert_special_char_src[] = {'-',   '_',  '(',  ')', 0x0};
    const char convert_special_char_dst[] = {0x3e, 0x3f, 0x40, 0x41, 0xff};

    if((c >= 'A') && (c <= 'Z')) {
        c = c - 'A';
    } else if((c >= 'a') && (c <= 'z')) {
        c = c - 'a' + 0x1a;
    } else if((c >= '0') && (c <= '9')) {
        c = c - '0' + 0x34;
    } else {
        int idx;
        for(idx = 0;idx < sizeof(convert_special_char_src);idx++) {
            if(convert_special_char_src[idx] == c) {
                c = convert_special_char_dst[idx];
                break;
            }
        }
        if(idx >= sizeof(convert_special_char_src)) {
            c = 0xff;
        }
    }
    return c;
}

int wifi_ssid(int argc, char *argv[]) {
    unsigned char wbuf[40];
    unsigned char rbuf[40];
    char buf[40];
    int idx;
    int ssid_finished = 0;

    if (argc != 0) {
        print_usage();
    }
    i2c_addr = 0x58;
    rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
    rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

#define INIT_SSID_REQU(__BUF__) {__BUF__[0] = 34; __BUF__[1] = 0x16;}
#define INIT_SSID_RESP(__BUF__) {__BUF__[0] = 0;}
    INIT_SSID_REQU(wbuf);
    INIT_SSID_RESP(rbuf);
    ezplib_get_attr_val("wl0_ssid_rule", 0,
            "ssid", buf, sizeof(buf), EZPLIB_USE_CLI);
#ifdef I2C_COMMAND_DEBUG
    printf("%s(%d) ssid:%s len:%d\n", __func__, __LINE__, buf, strlen(buf));
#endif
    wbuf[2] = (strlen(buf) < 32 ? strlen(buf) : 32);
    for(idx = 0;idx < wbuf[2] ; idx++) {
        if((buf[idx] == 0x0) || (ssid_finished == 1)) {
            ssid_finished = 1;
            wbuf[idx + 3] = convert_char(0x00);
        } else {
            wbuf[idx + 3] = convert_char(buf[idx]);
        }
    } 
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

i2c_cmd_handler i2c_wifi_cmd_handlers[] = {
    {"user", wifi_user_count, true},
    {"usb_1", usb_storage_info, true},
    {"usb_2", qsync_storage_info, true},
    {"qsync", qsync_storage_info, true},
    {"sd", sd_storage_info, true},
    {"storage", total_storage_info, true},
    {"ssid", wifi_ssid, true},
    {NULL, NULL}
};

int internet_on_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];

    i2c_addr = 0x58;
#define INIT_INTERNET_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x17; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_INTERNET_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
    INIT_INTERNET_ON_REQU(wbuf);
    INIT_INTERNET_ON_RESP(rbuf);
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

int internet_off_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];

    i2c_addr = 0x58;
#define INIT_INTERNET_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x17; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_INTERNET_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
    INIT_INTERNET_OFF_REQU(wbuf);
    INIT_INTERNET_OFF_RESP(rbuf);
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return 0;
}

i2c_cmd_handler i2c_internet_cmd_handlers[] = {
    {SUB_CMD_ON, internet_on_info, true},
    {SUB_CMD_OFF, internet_off_info, true},
    {NULL, NULL}
};

#define GET_IP_CHARMAP(__IPV__, __IPBUF__) {*__IPBUF__ = (__IPV__ > 99 ? (__IPV__ / 100): 0xFF); __IPBUF__ ++ ;\
                                            *__IPBUF__ = (__IPV__ > 9 ? ((__IPV__ % 100) / 10): 0xFF); __IPBUF__ ++; \
                                            *__IPBUF__ = __IPV__ % 10; __IPBUF__ ++ ; }

extern char *nvram_get(char *);
int wan_ip_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char *ip_addr;
    char *bufp = &wbuf[2];
    unsigned char ipval, bpos;

    i2c_addr = 0x58;
    ip_addr = nvram_get("wan0_ipaddr");
    if(ip_addr && *ip_addr) {
#define INIT_WAN_IP_REQU(__BUF__) {__BUF__[0] = 13; __BUF__[1] = 0x18;}
#define INIT_WAN_IP_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_WAN_IP_REQU(wbuf);
        INIT_WAN_IP_RESP(rbuf);
        ipval = strtol(ip_addr, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
    } else {
#define INIT_WAN_IP_0_0_0_0_REQU(__BUF__) {__BUF__[0] = 13; __BUF__[1] = 0x18; __BUF__[2] = 0xFF;__BUF__[3] = 0xFF;__BUF__[4] = 0xFF;\
                                                                               __BUF__[5] = 0xFF;__BUF__[6] = 0xFF;__BUF__[7] = 0xFF;\
                                                                               __BUF__[8] = 0xFF;__BUF__[9] = 0xFF;__BUF__[10] = 0xFF;\
                                                                               __BUF__[11] = 0xFF;__BUF__[12] = 0xFF;__BUF__[13] = 0xFF; }
#define INIT_WAN_IP_0_0_0_0_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_WAN_IP_0_0_0_0_REQU(wbuf);
        INIT_WAN_IP_0_0_0_0_RESP(rbuf);
    }
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

i2c_cmd_handler i2c_wan_cmd_handlers[] = {
    {"ip", wan_ip_info, true},
    {NULL, NULL}
};

int lan_ip_info(int argc, char *argv[]) {
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char *ip_addr;
    char *bufp = &wbuf[2];
    unsigned char ipval, bpos;

    i2c_addr = 0x58;
    ip_addr = nvram_get("lan0_ipaddr");
    if(ip_addr) {
#define INIT_LAN_IP_REQU(__BUF__) {__BUF__[0] = 13; __BUF__[1] = 0x19;}
#define INIT_LAN_IP_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_LAN_IP_REQU(wbuf);
        INIT_LAN_IP_RESP(rbuf);
        ipval = strtol(ip_addr, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
        ipval = strtol(ip_addr + 1, &ip_addr, 10);
        GET_IP_CHARMAP(ipval, bufp);
    } else {
#define INIT_LAN_IP_0_0_0_0_REQU(__BUF__) {__BUF__[0] = 13; __BUF__[1] = 0x19; __BUF__[2] = 0xFF;__BUF__[3] = 0xFF;__BUF__[4] = 0xFF;\
                                                                               __BUF__[5] = 0xFF;__BUF__[6] = 0xFF;__BUF__[7] = 0xFF;\
                                                                               __BUF__[8] = 0xFF;__BUF__[9] = 0xFF;__BUF__[10] = 0xFF;\
                                                                               __BUF__[11] = 0xFF;__BUF__[12] = 0xFF;__BUF__[13] = 0xFF; }
#define INIT_LAN_IP_0_0_0_0_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_LAN_IP_0_0_0_0_REQU(wbuf);
        INIT_LAN_IP_0_0_0_0_RESP(rbuf);
    }
    result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
    return result;
}

i2c_cmd_handler i2c_lan_cmd_handlers[] = {
    {"ip", lan_ip_info, true},
    {NULL, NULL}
};

int handle_i2c_input_commands(int argc, char *argv[], i2c_cmd_handler *cmd_handlers) {
    int retval = -1;
    int retry_count = 3;
    i2c_cmd_handler *search_handle = cmd_handlers;
    while(search_handle->command) {
        if(!strcmp(search_handle->command, argv[0])) {
            argv ++;
            argc --;
            do {
                retval = search_handle->func(argc, argv);
                retry_count --;
            }
            while(search_handle->do_retry && (retval != 0) && (retry_count >= 0)); 
            break;
        }
        search_handle ++;
    }
    return retval;
}

#if defined (PLATFORM_M2E) && !defined(USE_GPIO_ACQUIRE_SESSION)
/*
  0 : gpio acquired normally
 -1 : can not set gpio direction input
 -2 : can not read gpio value
 -3 : mcu busy timeout 20 times
 */
static int check_gpio_and_set_low(int fd_gpio) {
    int result_gpio = -1;
    unsigned int cmd_gpio;
    unsigned int gpio_pin = MCU_COMMUNICATION_GPIO;
    int mcu_busy_count = 20;
    /* set gpio dirin */
    result_gpio = ioctl(fd_gpio, EZP_GPIO_SET_DIR_IN, gpio_pin);

    if(result_gpio < 0) {
        return -1;
    }
    do {
        /* read gpio pin MCU_COMMUNICATION_GPIO */
        cmd_gpio = (gpio_pin << 24) | EZP_GPIO_READ_BIT;
        if ((ioctl(fd_gpio, cmd_gpio, &result_gpio)) != -1)
        {
            if(result_gpio == 1)
            {
                /* we got gpio high, set gpio pin MCU_COMMUNICATION_GPIO as output */
                ioctl(fd_gpio, EZP_GPIO_SET_DIR_OUT, gpio_pin);
                /* set pin MCU_COMMUNICATION_GPIO output low */
                ioctl(fd_gpio, EZP_GPIO_CLEAR, gpio_pin);
                return 0;
            }
            else
            {
                usleep(100000);
                mcu_busy_count --;
                result_gpio = -3;
            }
        } else {
            result_gpio = -2;
        }
    } while(mcu_busy_count > 0);
    return result_gpio;
}

#define COLLISION_DETECTION_TIMES       3
#define COLLISION_DETECTION_INTERVAL    4000 /* 4 ms = 4000 us, and other commands offer 3 ms overhead  */

static int acquired_mcu_gpio_fd = -1;
int check_and_acquire_mcu(long wait_msecs) {
    int fd_gpio;
    int failcount = 0;
    int chk_times = COLLISION_DETECTION_TIMES;
    int gpio_result = -1;
    /* set gpio pin MCU_COMMUNICATION_GPIO dirin */
    do {
        if ((fd_gpio = open("/dev/gpio", O_RDWR)) < 0)
        {
            printf("Error whilst opening /dev/gpio, wait a while and do again\n");
            usleep(100000);
            if(failcount == 10) {
                return -1;
            }
            failcount ++;
            continue;
        }
        for(chk_times = COLLISION_DETECTION_TIMES; chk_times > 0;chk_times --) {
            gpio_result = check_gpio_and_set_low(fd_gpio);
            if(gpio_result < 0) {
                printf("Collision detected, retreve, failtimes:%d\n", failcount);
                failcount ++;
                break;
            } else if(COLLISION_DETECTION_TIMES > 1) {
                usleep(COLLISION_DETECTION_INTERVAL);
            }
        }

#if 0
        cmd_gpio = (1L << gpio_pin);
        result_gpio = ioctl(fd_gpio, EZP_GPIO_SET_DIR_IN, gpio_pin);

        /* read gpio pin MCU_COMMUNICATION_GPIO */
        cmd_gpio = (gpio_pin << 24) | EZP_GPIO_READ_BIT;
        if ((ioctl(fd_gpio, cmd_gpio, &result_gpio)) != -1)
        {
            if(result_gpio == 1)
            {
                /* dirin high, set gpio pin MCU_COMMUNICATION_GPIO output low */
                ioctl(fd_gpio, EZP_GPIO_SET_DIR_OUT, gpio_pin);
                /* set pin MCU_COMMUNICATION_GPIO dirout low */
                ioctl(fd_gpio, EZP_GPIO_CLEAR, gpio_pin);
            }
            else
            {
                usleep(100000);
            }
        }
        else
        {
            printf("Gpio pin 42 error\n");
            close(fd_gpio);
            return -2;
        }
#endif
    } while(gpio_result < 0 && failcount < 10);
    acquired_mcu_gpio_fd = fd_gpio;
#ifdef I2C_COMMAND_DEBUG
    printf("Mcu gpio %d acquired, fd:%d\n", gpio_pin, acquired_mcu_gpio_fd);
#endif
    return fd_gpio;
}

int release_mcu(int mcu_gpio_fd) {
    unsigned int gpio_pin = MCU_COMMUNICATION_GPIO;
    if(mcu_gpio_fd <= 0) mcu_gpio_fd = acquired_mcu_gpio_fd;
    if(mcu_gpio_fd <= 0) return -1;
    /* set gpio MCU_COMMUNICATION_GPIO dirin */
    ioctl(mcu_gpio_fd, EZP_GPIO_SET_DIR_IN, gpio_pin);
    close(mcu_gpio_fd);
    acquired_mcu_gpio_fd = -1;
#ifdef I2C_COMMAND_DEBUG
    printf("Mcu gpio %d released and set to input, fd:%d\n", gpio_pin, acquired_mcu_gpio_fd);
#endif
    return 1;
}

#endif

#define DETECT_IN_RANGE(__A,__B,R) (((__A - __B) <= R) && ((__A - __B) >= -R))
#define INIT_PERCENTAGE_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x06;__BUF__[2] = 0;}
#define INIT_PERCENTAGE_RESP(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0xFF;__BUF__[2] = 0;}
#define INIT_VOLTAGE_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x01; __BUF__[2] = 0;}
#define INIT_VOLTAGE_RESP(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
#define INIT_CURRENT_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x0A; __BUF__[2] = 0;}
#define INIT_CURRENT_RESP(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
int main(int argc, char **argv) {
    system("touch /tmp/ezp-i2c.lock");
    int fd = open("/tmp/ezp-i2c.lock", O_WRONLY);
    static struct flock lock;
    unsigned char wbuf[20];
    unsigned char rbuf[20];
    char output_string[30];
    int rwres = 0;
    FILE *pFile = NULL;
    int countdown;
#ifdef I2C_COMMAND_DEBUG
    unsigned char logbuf[256];
#endif

    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);

#ifdef I2C_COMMAND_DEBUG
    sprintf(logbuf, "argv 0:%s 1:%s 2:%s 3:%s\n", argv[0], (argc > 1) ? argv[1] : "" , (argc > 2) ? argv[2] : "", (argc > 3) ? argv[3] : "");
    printf("%s", logbuf);
    log_msg_file("/tmp/123", logbuf);
#endif
#if defined (PLATFORM_M2E) && defined (USE_GPIO_ACQUIRE_SESSION)
    int fd_gpio;
    unsigned int cmd_gpio;
    int result_gpio = 0;
    unsigned int gpio_pin = MCU_COMMUNICATION_GPIO;
    if ((fd_gpio = open("/dev/gpio", O_RDWR)) < 0)
    {
        printf("Error whilst opening /dev/gpio\n");
        return -1;
    }
    while(result_gpio == 0) {
        /* set gpio pin MCU_COMMUNICATION_GPIO dirin */
        cmd_gpio = (1L << gpio_pin);
        result_gpio = ioctl(fd_gpio, EZP_GPIO_SET_DIR_IN, gpio_pin);

        /* read gpio pin MCU_COMMUNICATION_GPIO */
        cmd_gpio = (gpio_pin << 24) | EZP_GPIO_READ_BIT;
        if ((ioctl(fd_gpio, cmd_gpio, &result_gpio)) != -1)
        {
            if(result_gpio == 1)
            {
                /* dirin high, set gpio pin MCU_COMMUNICATION_GPIO output low */
                ioctl(fd_gpio, EZP_GPIO_SET_DIR_OUT, gpio_pin);
                /* set pin MCU_COMMUNICATION_GPIO dirout low */
                ioctl(fd_gpio, EZP_GPIO_CLEAR, gpio_pin);
            }
            else
            {
                usleep(100000);
                //close(fd_gpio);
                //goto unlock_and_leave;
                // return 0;
            }
        }
        else
        {
            printf("Gpio pin 42 error\n");
            close(fd_gpio);
            goto unlock_and_leave;
            // return 0;
        }
    }
#endif
	//Initialize the I2C interface
	init_i2c();
    
    if (argc < 2) {
        print_usage();
    }
	
    if (!strcmp(argv[1], "gauge")) {
        if (argc < 3) {
            print_usage();
        }
        i2c_addr=0x58;
        rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
        rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

        if (!strcmp(argv[2], "voltage")) {
            unsigned int voltage = 0;
            pFile =  fopen("/tmp/gauge_voltage", "r");
            if(pFile) {
                if(0 >= fscanf(pFile, "%d", &voltage)) {
                    voltage = 0;
                }
                fclose(pFile);
                pFile = NULL;
            }
            INIT_VOLTAGE_REQU(wbuf);
            INIT_VOLTAGE_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result >= 0) {
                /* Write to voltage file */
                voltage = rbuf[1] + rbuf[2]*256;
                pFile =  fopen("/tmp/gauge_voltage", "w");
                snprintf(output_string,sizeof(output_string),"%d",voltage);
                fwrite(output_string, 1 , strlen(output_string), pFile);
                fclose(pFile);
                pFile = NULL;
            }
            printf("Voltage: %d mV\n",voltage);
        } else if (!strcmp(argv[2], "host")) {
            // Retry three times
            countdown = 3;
#define INIT_HOST_REQU(__BUF__, __MODE__) {__BUF__[0] = 2; __BUF__[1] = 0x02; __BUF__[2] = __MODE__; __BUF__[3] = 0;}
#define INIT_HOST_RESP(__BUF__) {__BUF__[0] = 0;}
            if (argc < 4) {
                print_usage();
            }
            do {
                if (!strcmp(argv[3], "sleep")) {
                    INIT_HOST_REQU(wbuf, 0x00);
                } else if (!strcmp(argv[3], "booting")) {
                    INIT_HOST_REQU(wbuf, 0x01);
                } else if (!strcmp(argv[3], "ready")) {
                    INIT_HOST_REQU(wbuf, 0x02);
                } else if (!strcmp(argv[3], "wifioff")) {
                    INIT_HOST_REQU(wbuf, 0x03);
                } else if (!strcmp(argv[3], "clear")) {
                    INIT_HOST_REQU(wbuf, 0x80);
                } else if (!strcmp(argv[3], "recovery")) {
                    INIT_HOST_REQU(wbuf, 0x05);
                } else if (!strcmp(argv[3], "reboot")) {
                    INIT_HOST_REQU(wbuf, 0x06);
                } else {
                    print_usage();
                }
                INIT_HOST_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
                if(result < 0) {
                    printf("issue host command %s failed, retry %d\n", argv[3], countdown);
                    usleep(1000000);
                }
                countdown --;
            }
            while((0 > result) && (0 < countdown));
            //i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result < 0)
                result = 0;
            else
                result = rbuf[0];
        } else if (!strcmp(argv[2], "client")) {
            countdown = 3;
#define INIT_CLIENT_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x03; __BUF__[2] = 0;}
#define INIT_CLIENT_RESP(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0xFF; __BUF__[2] = 0;}
            do {
                INIT_CLIENT_REQU(wbuf);
                INIT_CLIENT_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
                if(result < 0) {
                    printf("issue client command failed, retry %d\n", countdown);
                    usleep(1000000);
                }
                countdown --;
            }
            while((0 > result) && (0 < countdown));
            // i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result < 0)
                result = 0;
            else
                result = rbuf[1];
        } else if (!strcmp(argv[2], "state")) {
#define INIT_STATE_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x04; __BUF__[2] = 0;}
#define INIT_STATE_RESP(__BUF__) {__BUF__[0] = 5; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0xFF; __BUF__[4] = 0xFF; __BUF__[5] = 0xFF; __BUF__[6] = 0;}
            INIT_STATE_REQU(wbuf);
            INIT_STATE_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            printf("Data Read: 0x%02X 0x%02X 0x%02X 0x%02X\n",rbuf[1],rbuf[2],rbuf[3],rbuf[4]);
            if(result >= 0) {
                pFile =  fopen("/tmp/gauge_state", "w");
                snprintf(output_string,255,"0x%02X 0x%02X 0x%02X 0x%02X",rbuf[1], rbuf[2],rbuf[3],rbuf[4]);
                fwrite(output_string, 1 , strlen(output_string), pFile);
                fclose(pFile);
            }
        } else if (!strcmp(argv[2], "fw")) {
#define INIT_GAUGE_FW_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x11; __BUF__[2] = 0;}
#define INIT_GAUGE_FW_RESP(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;  __BUF__[4] = 0;}
            INIT_GAUGE_FW_REQU(wbuf);
            INIT_GAUGE_FW_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            printf("Data Read: 0x%02X\n",rbuf[1]);
            if(result >= 0) {
                pFile =  fopen("/tmp/gauge_fw", "w");
                snprintf(output_string,10,"%d",rbuf[1]);
                fwrite(output_string, 1 , strlen(output_string), pFile);
                fclose(pFile);
            }
        } else if (!strcmp(argv[2], "read")) {
            unsigned int cmd;
            int i=0;
            if (argc < 5) {
                print_usage();
            }
            sscanf(argv[3],"0x%02X", &cmd);
            r_len=atoi(argv[4]);
            wbuf[0] = 1;
            wbuf[1] = cmd;
            wbuf[2] = 0;
            rbuf[0] = r_len;
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result >= 0) {
                for (i=1 ; i<r_len+1; i++) {
                    printf("Data Read[%d]: 0x%02X\n", i, rbuf[i]);
                }
            } else {
                printf("Data read failed, result:%d\n", result);
            }
        } else if (!strcmp(argv[2], "write")) {
            int i=0;
            sscanf(argv[3],"0x%02X", &i2c_addr);
            rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
            rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
            w_len=atoi(argv[4]);
            wbuf[0] = w_len;
            for (i=1 ; i< w_len + 1; i++) {
                sscanf(argv[4+i],"0x%02X", &wbuf[i]);
                printf("Data write[%d]: 0x%02x\n", i, wbuf[i]);
            }
            rbuf[0] = 0;
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result < 0) {
                printf("Data write failed, result:%d\n", result);
            }
        } else if (!strcmp(argv[2], "percentage")) {
            unsigned int percentage = 0;
            pFile =  fopen("/tmp/gauge_percentage", "r");
            if(pFile) {
                if(0 >= fscanf(pFile, "%d", &percentage)) {
                    percentage = 70;
                }
                fclose(pFile);
                pFile = NULL;
            }
            INIT_PERCENTAGE_REQU(wbuf);
            INIT_PERCENTAGE_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result >= 0) {
                /* Write to percentage file */
                percentage = rbuf[1];
                pFile =  fopen("/tmp/gauge_percentage", "w");
                snprintf(output_string,sizeof(output_string),"%d", percentage);
                fwrite(output_string, 1 , strlen(output_string), pFile);
                fclose(pFile);
            }
            printf("Battery: %d %%\n", percentage);
        } else if (!strcmp(argv[2], "current")) {
            int current=0;
            FILE *pFile =  fopen("/tmp/gauge_current", "r");
            if(pFile) {
                if(0 >= fscanf(pFile, "%d", &current)) {
                    current = 0;
                }
                fclose(pFile);
            }
            INIT_CURRENT_REQU(wbuf);
            INIT_CURRENT_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            if(result >= 0) {
                /* Write to current file */
                current= *(signed char *)(&rbuf[2]);
                current *= 1 << 8;
                current |= rbuf[1];
                pFile =  fopen("/tmp/gauge_current", "w");
                snprintf(output_string,sizeof(output_string),"%d", current);
                fwrite(output_string, 1 , strlen(output_string), pFile);
                fclose(pFile);
            }
            printf("Current: %d mA\n",current);
        } else if (!strcmp(argv[2], "led")) {
            if (!strcmp(argv[3], "resume")) {
#define INIT_LED_RESUME_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x10; __BUF__[2] = 0;}
#define INIT_LED_RESUME_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_LED_RESUME_REQU(wbuf);
                INIT_LED_RESUME_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else {
                print_usage();
            }
        } else if (!strcmp(argv[2], "green_led")) {
            if (!strcmp(argv[3], "on")) {
#define INIT_GREEN_LED_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x08; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_GREEN_LED_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_GREEN_LED_ON_REQU(wbuf);
                INIT_GREEN_LED_ON_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else if (!strcmp(argv[3], "off")) {
#define INIT_GREEN_LED_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x08; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_GREEN_LED_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_GREEN_LED_OFF_REQU(wbuf);
                INIT_GREEN_LED_OFF_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else {
                print_usage();
            }
        } else if (!strcmp(argv[2], "red_led")) {
            if (!strcmp(argv[3], "on")) {
#define INIT_RED_LED_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x09; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_RED_LED_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_RED_LED_ON_REQU(wbuf);
                INIT_RED_LED_ON_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else if (!strcmp(argv[3], "off")) {
#define INIT_RED_LED_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x09; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_RED_LED_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_RED_LED_OFF_REQU(wbuf);
                INIT_RED_LED_OFF_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else {
                print_usage();
            }
        } else if (!strcmp(argv[2], "wm_led")) {
            if (!strcmp(argv[3], "on")) {
#define INIT_WM_LED_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x12; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_WM_LED_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_WM_LED_ON_REQU(wbuf);
                INIT_WM_LED_ON_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else if (!strcmp(argv[3], "off")) {
#define INIT_WM_LED_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x12; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_WM_LED_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_WM_LED_OFF_REQU(wbuf);
                INIT_WM_LED_OFF_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else {
                print_usage();
            }
        } else if (!strcmp(argv[2], "pm_led")) {
            if (!strcmp(argv[3], "on")) {
#define INIT_PM_LED_ON_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x13; __BUF__[2] = 0x01;__BUF__[3] = 0;}
#define INIT_PM_LED_ON_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_PM_LED_ON_REQU(wbuf);
                INIT_PM_LED_ON_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else if (!strcmp(argv[3], "off")) {
#define INIT_PM_LED_OFF_REQU(__BUF__) {__BUF__[0] = 2; __BUF__[1] = 0x13; __BUF__[2] = 0x00;__BUF__[3] = 0;}
#define INIT_PM_LED_OFF_RESP(__BUF__) {__BUF__[0] = 0; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
                INIT_PM_LED_OFF_REQU(wbuf);
                INIT_PM_LED_OFF_RESP(rbuf);
                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            } else {
                print_usage();
            }
        } else {
            /* ignore 2 words : ezp-i2c gauge */
            argc -= 2;
            argv += 2;
            if(-1 == handle_i2c_input_commands(argc, argv, i2c_cmd_handlers)) {
                print_usage();
            }
        }
    } else if (!strcmp(argv[1], "normal")) {
        i2c_addr = 0x58;
        rt2880_i2c_write = RT2880_I2C_NORMAL_WRITE;
        rt2880_i2c_read = RT2880_I2C_NORMAL_READ;
        if (!strcmp(argv[2], "read")) {
            int i;
            unsigned char i2cbuf[20];
            unsigned long len = strtol(argv[4], NULL, 10); // data length, 1,2 or 4 bytes
            sscanf(argv[3],"0x%02x", &i2c_addr);
            memset(i2cbuf, 0 , sizeof(i2cbuf));
            i2cbuf[0] = len;
            result = read_i2c(i2cbuf);
            printf("read addr:0x%02x length %d return:%d\n", i2c_addr, i2cbuf[0], result);
            for(i=0;i<len;i++) {
                printf("read data [%d]:0x%02x\n", i + 1, i2cbuf[i+1]);
            }
        } else if (!strcmp(argv[2], "write")) {
            int i;
            unsigned char i2cbuf[20];
            unsigned long len = strtol(argv[4], NULL, 10); // data length, 1,2 or 4 bytes
            unsigned long sendcode = strtol(argv[5], NULL, 16); // Specify data to send , ex : 0x0A
            sscanf(argv[3],"0x%02x", &i2c_addr);
            memset(i2cbuf, 0 , sizeof(i2cbuf));
            printf("write addr:0x%02x length %d sendcode:0x%04x\n", i2c_addr, i2cbuf[0], sendcode);
            i2cbuf[0] = len;
            for(i = 0; i < len; i++) {
                i2cbuf[i + 1] = sendcode & 0x000000ff;
                sendcode = sendcode >> 8;
            }
            result = write_i2c(i2cbuf);
            printf("write addr:0x%02x length %d result:%d\n", i2c_addr, i2cbuf[0], result);
            for(i=0;i<len;i++) {
                printf("write data [%d]:0x%02x\n", i + 1, i2cbuf[i+1]);
            }
        }
    } else if (!strcmp(argv[1], "fw")) {
        i2c_addr=0x58;
        rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
        rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
#define INIT_FW_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x05; __BUF__[2] = 0;}
#define INIT_FW_RESP(__BUF__) {__BUF__[0] = 3; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0;}
        INIT_FW_REQU(wbuf);
        INIT_FW_RESP(rbuf);
        result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
        printf("Data Read: 0x%02X 0x%02X 0x%02X\n",rbuf[1],rbuf[2],rbuf[3]);
        if(result >= 0) {
            char release[10];
            pFile =  fopen("/tmp/mcu_fw", "w");
            if ( rbuf[1] == 0x01) snprintf(release,10,"A"); 
            else if ( rbuf[1] == 0x02) snprintf(release,10,"B"); 
            else if ( rbuf[1] == 0x03) snprintf(release,10,"C"); 
            else snprintf(release,10,"Unknown"); 
            snprintf(output_string,10,"%s%02d",release,rbuf[2]);
            fwrite(output_string, 1 , strlen(output_string), pFile);
            fclose(pFile);
        }
    } else if (!strcmp(argv[1], "security")) {
        FILE *pFile;
        char output[256]={0};
#if defined(PLATFORM_LIS) || defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
        system("nvram replace attr license_status_rule 0 state 1");
        system("nvram replace attr license_status_rule 0 trycount 0");
        snprintf(output,sizeof(output),"Result: Pass\n");
#else
        unsigned char buf_rand[4]={0};
        unsigned char buf[4]={0};
        unsigned char buf_return[4]={0};
        int cnt=0;
        unsigned char flag=1;
        countdown = 10;
        i2c_addr=0x50;
        rt2880_i2c_write=RT2880_I2C_SECURITY_WRITE;
        rt2880_i2c_read=RT2880_I2C_SECURITY_READ;
        rwres = -1;
        while((rwres < 0) && countdown) {
            countdown --;
            rwres = write_i2c(buf_rand);
            if(rwres < 0) continue;
            sleep(1);
            rwres = read_i2c(buf_return);
            if(rwres < 0) continue;
            printf("Data rand: %02x%02x%02x%02x\n",buf_rand[0],buf_rand[1],buf_rand[2],buf_rand[3]);
            buf[0] = ((~buf_rand[0])^buf_rand[1]^(~buf_rand[2])^buf_rand[3])+key1;
            buf[1] = (buf_rand[0]^(~buf_rand[1])^buf_rand[2]^(~buf_rand[3]))+key2;
            buf[2] = ((~buf_rand[0])^buf_rand[1]^buf_rand[2]^buf_rand[3])+key3;
            buf[3] = (buf_rand[0]^buf_rand[1]^(~buf_rand[2])^buf_rand[3])+key4;
            printf("Data: %02x%02x%02x%02x\n",buf[0],buf[1],buf[2],buf[3]);
        }
        for (cnt=0;cnt<4;cnt++) {
            if (buf[cnt] != buf_return[cnt]) {
                flag=0;
                break;
            }
        }
        printf("Data Read: %02x%02x%02x%02x\n",buf_return[0],buf_return[1],buf_return[2],buf_return[3]);
        if (flag == 0) {
            flag=1;
            sleep(1);
            read_i2c(buf_return);
            for (cnt=0;cnt<4;cnt++) {
                if (buf[cnt] != buf_return[cnt]) {
                    flag=0;
                    break;
                }
            }
            printf("Data Read: %02x%02x%02x%02x\n",buf_return[0],buf_return[1],buf_return[2],buf_return[3]);
        }
        if (flag) {
            system("nvram replace attr license_status_rule 0 state 1");
            system("nvram replace attr license_status_rule 0 trycount 0");
            snprintf(output,sizeof(output),"Result: Pass\n");
        } else {
            system("nvram replace attr license_status_rule 0 state 0");
            snprintf(output,sizeof(output),"Result: Fail\n");
        }
#endif
        pFile =  fopen("/tmp/ezp-i2c_security", "w");
        fwrite(output, 1 , strlen(output), pFile);
        fclose(pFile);
    } else if (!strcmp(argv[1], "charger")) {
        if ((argc == 3) && (!strcmp(argv[2], "status"))) {
            i2c_addr=0x58;
            rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
            rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
#define INIT_CHARGER_STATUS_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x04; __BUF__[2] = 0;}
#define INIT_CHARGER_STATUS_RESP(__BUF__) {__BUF__[0] = 11; __BUF__[1] = 0xFF; __BUF__[2] = 0xFF; __BUF__[3] = 0xFF;__BUF__[4] = 0xFF;__BUF__[5] = 0xFF;__BUF__[6] = 0xFF;__BUF__[7] = 0xFF;__BUF__[8] = 0xFF;__BUF__[9] = 0xFF;__BUF__[10] = 0xFF;__BUF__[11] = 0;}
            INIT_CHARGER_STATUS_REQU(wbuf);
            INIT_CHARGER_STATUS_RESP(rbuf);
            result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
            printf("Data Read: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",rbuf[1],rbuf[2],rbuf[3],rbuf[4],rbuf[5],rbuf[6],rbuf[7],rbuf[8],rbuf[9],rbuf[10],rbuf[11]);
        } else {
            countdown = 3;
#define INIT_CHARGER_REQU(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0x07; __BUF__[2] = 0;}
#define INIT_CHARGER_RESP(__BUF__) {__BUF__[0] = 1; __BUF__[1] = 0xFF; __BUF__[2] = 0;}
            do {
                INIT_CHARGER_REQU(wbuf);
                INIT_CHARGER_RESP(rbuf);
                i2c_addr=0x58;
                rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
                rt2880_i2c_read=RT2880_I2C_GAUGE_READ;

                result = i2c_send_and_read(i2c_addr, wbuf, rbuf);
                if(result < 0) {
                    printf("issue charger command failed, retry %d\n", countdown);
                    usleep(1000000);
                }
                countdown --;
            }
            while((0 > result) && (0 < countdown));
            if(0 > result) {
                printf("No charger\n");
            } else {
                switch (rbuf[1]) {
                    case 0x00:
                        printf("No charger\n");
                        break;
                    case 0x01:
                        printf("Charger type: Other\n");
                        break;
                    case 0x02:
                        printf("Charger type: SDP\n");
                        break;
                    case 0x04:
                        printf("Charger type: DCP\n");
                        break;
                    case 0x08:
                        printf("Charger type: CDP\n");
                        break;
                    default:
                        printf("Not define\n");
                        break;
                }
            }
        }
#ifdef PLATFORM_M2E
    } else if (!strcmp(argv[1], "wifi")) {
        /* ignore 2 words : ezp-i2c wifi */
        argc -= 2;
        argv += 2;
        if(-1 == handle_i2c_input_commands(argc, argv, i2c_wifi_cmd_handlers)) {
            print_usage();
        }
    } else if (!strcmp(argv[1], "internet")) {
        /* ignore 2 words : ezp-i2c gauge */
        argc -= 2;
        argv += 2;
        rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
        rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
        i2c_addr = 0x58;
        if(-1 == handle_i2c_input_commands(argc, argv, i2c_internet_cmd_handlers)) {
            print_usage();
        }
    } else if (!strcmp(argv[1], "wan")) {
        /* ignore 2 words : ezp-i2c gauge */
        argc -= 2;
        argv += 2;
        rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
        rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
        i2c_addr = 0x58;
        if(-1 == handle_i2c_input_commands(argc, argv, i2c_wan_cmd_handlers)) {
            print_usage();
        }
    } else if (!strcmp(argv[1], "lan")) {
        /* ignore 2 words : ezp-i2c gauge */
        argc -= 2;
        argv += 2;
        rt2880_i2c_write=RT2880_I2C_GAUGE_WRITE;
        rt2880_i2c_read=RT2880_I2C_GAUGE_READ;
        i2c_addr = 0x58;
        if(-1 == handle_i2c_input_commands(argc, argv, i2c_lan_cmd_handlers)) {
            print_usage();
        }
#endif
    } else {
        print_usage();
    }
#if defined (PLATFORM_M2E) && defined (USE_GPIO_ACQUIRE_SESSION)
    /* set gpio MCU_COMMUNICATION_GPIO dirin */
    ioctl(fd_gpio, EZP_GPIO_SET_DIR_IN, gpio_pin);
    close(fd_gpio);
unlock_and_leave:
#endif
    release_i2c();
	return result;
}
