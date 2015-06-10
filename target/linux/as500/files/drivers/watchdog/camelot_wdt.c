/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 *
 *  Watchdog driver for Camelot SoC
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>

#include <asm/camelot/camelot.h>

#define	WDT_WDMODE		0x00000008	/* 1: Watchdog mode  0: Timer mode */
#define WDT_WDTIMERGO	0x00000004	/* 1: WDTimer enable  0: disable   */
#define WDT_WDTIMER		0x00000002	/* 1: Watchdog timer interrupt enable  0: disable */
#define WDT_TWDALM		0x00000001


static int nowayout = WATCHDOG_NOWAYOUT;
static int heartbeat = 60;	/* (secs) Default is 1 minute */
static unsigned long wdt_status;
static unsigned long boot_status;
static DEFINE_SPINLOCK(wdt_lock);

#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1

#define WDT_PRESCALAR_VAL	0xfffff

void camelot_wdt_enable(void)
{
	volatile unsigned int *TimerReg = (unsigned int *) TM_BASE;

	spin_lock(&wdt_lock);

    TimerReg[T2PR] = WDT_PRESCALAR_VAL;
    TimerReg[T2LR] = SYS_CLK / (WDT_PRESCALAR_VAL + 1) * heartbeat;
    TimerReg[T2CR] = 0;
    TimerReg[T2CN] = WDT_WDMODE | WDT_WDTIMERGO;

	spin_unlock(&wdt_lock);
}

void camelot_wdt_disable(void)
{
	volatile unsigned int *TimerReg = (unsigned int *) TM_BASE;

	spin_lock(&wdt_lock);

    TimerReg[T2CN] = ~(WDT_WDMODE | WDT_WDTIMERGO);

	spin_unlock(&wdt_lock);
}

static int camelot_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
		return -EBUSY;

	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
	camelot_wdt_enable();
	return nonseekable_open(inode, file);
}

static ssize_t
camelot_wdt_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt_status);
			}
		}
		camelot_wdt_enable();
	}
	return len;
}

static struct watchdog_info ident = {
	.options	= WDIOF_CARDRESET | WDIOF_MAGICCLOSE |
			  WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
	.identity	= "Camelot Watchdog",
};


static long camelot_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int ret = -ENOTTY;
	int time;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				   sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_GETBOOTSTATUS:
		ret = put_user(boot_status, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		camelot_wdt_enable();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, (int *)arg);
		if (ret)
			break;

		if (time <= 0 || time > 60) {
			ret = -EINVAL;
			break;
		}

		heartbeat = time;
		camelot_wdt_enable();
		/* Fall through */

	case WDIOC_GETTIMEOUT:
		ret = put_user(heartbeat, (int *)arg);
		break;
	}
	return ret;
}

static int camelot_wdt_release(struct inode *inode, struct file *file)
{
	if (test_bit(WDT_OK_TO_CLOSE, &wdt_status))
		camelot_wdt_disable();
	else
		printk(KERN_CRIT "WATCHDOG: Device closed unexpectedly - "
					"timer will not stop\n");
	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

	return 0;
}


static const struct file_operations camelot_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= camelot_wdt_write,
	.unlocked_ioctl	= camelot_wdt_ioctl,
	.open		= camelot_wdt_open,
	.release	= camelot_wdt_release,
};

static struct miscdevice camelot_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &camelot_wdt_fops,
};

static int __init camelot_wdt_init(void)
{
	int ret;

	spin_lock_init(&wdt_lock);

	/* boot status support in HW? */
	boot_status = 0;

	ret = misc_register(&camelot_wdt_miscdev);
	if (ret == 0)
		printk(KERN_INFO "Camelot Watchdog Timer: heartbeat %d sec\n",
			heartbeat);
	return ret;
}

static void __exit camelot_wdt_exit(void)
{
	misc_deregister(&camelot_wdt_miscdev);
}


module_init(camelot_wdt_init);
module_exit(camelot_wdt_exit);

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds (default 60s)");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

