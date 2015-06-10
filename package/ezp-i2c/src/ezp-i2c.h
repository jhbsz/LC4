#ifndef SECURITY_H_
#define SECURITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>

#define RT2880_I2C_READ         3
#define RT2880_I2C_WRITE        5
#define RT2880_I2C_SET_ADDR     7
#define RT2880_I2C_SECURITY_WRITE   8
#define RT2880_I2C_SECURITY_READ    9
#define RT2880_I2C_GAUGE_WRITE      10
#define RT2880_I2C_GAUGE_READ       11
#define RT2880_I2C_NORMAL_READ	    21
#define RT2880_I2C_NORMAL_READ2	    22
#define RT2880_I2C_NORMAL_READ3	    23
#define RT2880_I2C_NORMAL_READ4	    24
#define RT2880_I2C_NORMAL_WRITE	    31
#define RT2880_I2C_NORMAL_WRITE2	32
#define RT2880_I2C_NORMAL_WRITE3	33
#define RT2880_I2C_NORMAL_WRITE4	34

#ifdef EZP_I2C_KEY_RFLINK
#define key1 0xAA
#define key2 0x19
#define key3 0x01
#define key4 0xBC
#elif defined(EZP_I2C_KEY_3S)
#define key1 0xAA
#define key2 0x19
#define key3 0x03
#define key4 0xBC
#elif defined(EZP_I2C_KEY_AXIM)
#define key1 0xAA
#define key2 0x29
#define key3 0x09
#define key4 0xBC
#else
#define key1 0xAA
#define key2 0x19
#define key3 0x09
#define key4 0xBC
#endif


static unsigned char rt2880_i2c_write;
static unsigned char rt2880_i2c_read;
// i2cfile: contains current handle for i2c file
static int i2cfile;

int init_i2c();

/*
 * seti2caddr: Sets the I2C address to a custom value
 * Should NOT be neccessary in normal operation.  Only needed if you enter ROM mode to reflash device.
 * Arguments: unsigned char addr: 8 bit address value (lower 7 bits are the address)
 * Returns: ioctl failure code on failure: 0 otherwise
 */
int seti2caddr(unsigned char addr);

int write_i2c(unsigned char *buf);
int read_i2c(unsigned char *buf);

int close_i2c();

#endif /*SECURITY_H_*/
