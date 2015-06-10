/* 
 *  Copyright (c) 2008, 2009    Acrospeed Inc.    All rights reserved. 
 *  
 *  Serial driver for Camelot
 *
 */

/* force to turn on CONFIG_SERIAL_CONSOLE, fix this later */
#ifndef CONFIG_SERIAL_CONSOLE
#define CONFIG_SERIAL_CONSOLE
#endif

#if defined(CONFIG_SERIAL_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
    #define SUPPORT_SYSRQ

/* use Ctrl-Break-h to invoke the SYSRQ help menu */
    #include <linux/sysrq.h>
#endif

#include <linux/delay.h>


#include <linux/module.h>

#include <linux/types.h>
#include <linux/serial.h>
#include <linux/serialP.h>
#include <linux/serial_reg.h>

#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/console.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/bitops.h>

#include <asm/setup.h>

#include <asm/system.h>

#include <asm/irq.h>

#include <asm/camelot/camelot.h>
#include "serial_camelot.h"

#if defined(CONFIG_SERIAL_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
static unsigned long break_pressed = 0;
#endif

static struct tty_driver *serial_driver;

/* number of characters left in xmit buffer before we ask for more */
#define WAKEUP_CHARS 256

//static DEFINE_SPINLOCK(mips_irq_lock);

static void rs_disable_tx_interrupts (struct async_struct *info) 
{
    unsigned long flags;
    volatile int *p = (int *) UR_BASE;

    //spin_lock_irqsave(&mips_irq_lock, flags);
    local_irq_save(flags);

    p[URCS>>2] &= ~URCS_TIE;

    //spin_unlock_irqrestore(&mips_irq_lock, flags);
    local_irq_restore(flags);
}

static void transmit_chars(struct async_struct *info);

static void rs_enable_tx_interrupts (struct async_struct *info)
{
    unsigned long flags;
    volatile int *p = (int *) UR_BASE;

    //spin_lock_irqsave(&mips_irq_lock, flags);
    local_irq_save(flags);

    p[URCS>>2] |= URCS_TIE;

	transmit_chars(info);

    //spin_unlock_irqrestore(&mips_irq_lock, flags);
    local_irq_restore(flags);
}

static void rs_disable_rx_interrupts (struct async_struct *info) 
{
    unsigned long flags;
    volatile int *p = (int *) UR_BASE;

    //spin_lock_irqsave(&mips_irq_lock, flags);
    local_irq_save(flags);

    p[URCS>>2] &= ~URCS_RIE;

    //spin_unlock_irqrestore(&mips_irq_lock, flags);
    local_irq_restore(flags);
}

static void rs_enable_rx_interrupts (struct async_struct *info)
{
    unsigned long flags;
    volatile int *p = (int *) UR_BASE;

    //spin_lock_irqsave(&mips_irq_lock, flags);
    local_irq_save(flags);

    p[URCS>>2] |= URCS_RIE;

    //spin_unlock_irqrestore(&mips_irq_lock, flags);
    local_irq_restore(flags);
}

static struct async_struct *IRQ_ports;

static void change_speed(struct async_struct *info,
                         struct ktermios *old_termios)
{
    /* not implemented */
}
static void rs_wait_until_sent(struct tty_struct *tty, int timeout);


static struct serial_state rs_table[1];

#define NR_PORTS ARRAY_SIZE(rs_table)

#include <asm/uaccess.h>

#define serial_isroot()	(capable(CAP_SYS_ADMIN))


static inline int serial_paranoia_check(struct async_struct *info,
                                        char *name, const char *routine)
{
#ifdef SERIAL_PARANOIA_CHECK
    static const char *badmagic =
    "Warning: bad magic number for serial struct (%s) in %s\n";
    static const char *badinfo =
    "Warning: null async_struct for (%s) in %s\n";

    if (!info)
    {
        printk(badinfo, name, routine);
        return 1;
    }
    if (info->magic != SERIAL_MAGIC)
    {
        printk(badmagic, name, routine);
        return 1;
    }
#endif
    return 0;
}

/*
 * ------------------------------------------------------------
 * rs_stop() and rs_start()
 *
 * This routines are called before setting or resetting tty->stopped.
 * They enable or disable transmitter interrupts, as necessary.
 * ------------------------------------------------------------
 */
static void rs_stop(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_stop"))
        return;

    rs_disable_tx_interrupts(info);
}

static void rs_start(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_start"))
        return;

    local_irq_save(flags);
    if (info->xmit.head != info->xmit.tail
        && info->xmit.buf
	)
    {
        rs_enable_tx_interrupts(info);
    }
    local_irq_restore(flags);
}

static void rs_sched_event(struct async_struct *info,
                           int event)
{
    info->event |= 1 << event;
    tasklet_schedule(&info->tlet);
}

static void receive_chars(struct async_struct *info)
{
    struct tty_struct *tty = info->tty;
    unsigned char ch, flag;
    volatile int *p = (int *) UR_BASE;
    struct  async_icount *icount;
	int rx_loop = 0;
	int ubr;

    flag = TTY_NORMAL;
    icount = &info->state->icount;

	do
    {
        if (!((1<<23)&(ubr = p[URBR]) )) //if no input, skip
			break;
		ch = ubr >>24;
        icount->rx++;

#if defined(CONFIG_SERIAL_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
        if ( break_pressed )
        {
            if (time_before(jiffies, break_pressed + HZ*5))
            {
                if (ch == 0)
                    continue;
                handle_sysrq(ch, tty);
                break_pressed = 0;
                continue;
            }
            break_pressed = 0;
        }
#endif
        tty_insert_flip_char(tty, ch, flag);
    } while (rx_loop++ < 256) ;

	// if rx some data, push it
	if (rx_loop)
    	tty_flip_buffer_push(tty);

    return;
}

static void transmit_chars(struct async_struct *info)
{
    volatile int *p = (int *) UR_BASE;
	int empty=0;

    for (;;)
    {
        /* XXX: check fifo full */
        if ((p[URCS>>2]&URCS_TF))
            break;
		else
    	if (info->x_char)
		{
        	p[URBR>>2] = ((unsigned int)info->x_char)<<24;
        	info->state->icount.tx++;
        	info->x_char = 0;
    	}
		else
    	if (info->xmit.head == info->xmit.tail
        	|| info->tty->stopped || info->tty->hw_stopped)
    	{
			empty=1;
			break;
		}
		else
		{
        	p[URBR>>2] = ((unsigned int)info->xmit.buf[info->xmit.tail++]) <<24;
        	info->xmit.tail = info->xmit.tail & (SERIAL_XMIT_SIZE-1);
        	info->state->icount.tx++;
       		if (info->xmit.head == info->xmit.tail)
			{
				empty=1;
            	break;
			}
		}
    }

    if (empty)
    {
		rs_disable_tx_interrupts(info);
    }

    if (CIRC_CNT(info->xmit.head,
                 info->xmit.tail,
                 SERIAL_XMIT_SIZE) < WAKEUP_CHARS)
        rs_sched_event(info, RS_EVENT_WRITE_WAKEUP);
    return;
}


static irqreturn_t camelot_uart_interrupt(int irq, void *dev_id)
{
    unsigned long status;
    volatile int *p = (int *) UR_BASE;
    struct async_struct * info;
	int pass_counter = 256;

    /* XXX : Clear any interrupts we might be about to handle */

    info = IRQ_ports;
    if (!info || !info->tty)
    {
        // printk("camelot_uart_interrupt: ignored\n");
        return IRQ_HANDLED;
    }

	do
	{
    		status = p[URCS>>2];

			// only handle these event
			if (!(status & (URCS_FE|URCS_PER|URCS_RF|URCS_TE)))
				break;
			/* RX Frame Error */
			if (status & URCS_FE)
			{
				info->state->icount.frame++;
			}

			/* RX Parity Error */
			if (status & URCS_PER)
			{
				info->state->icount.parity++;
			}

			/* TX holding register empty - transmit a byte */
			if (status & URCS_TE)
			{
				transmit_chars(info);
				info->last_active = jiffies;
			}

			/* Byte received , plus RX timeout ? (which bit for RX timeout?) */
			if (status & URCS_RF)
			{
				receive_chars(info);
				info->last_active = jiffies;
			}

	} while (--pass_counter  > 0) ;

	return (pass_counter!=256) ? IRQ_HANDLED : IRQ_NONE;
}

/*
 * -------------------------------------------------------------------
 * Here ends the serial interrupt routines.
 * -------------------------------------------------------------------
 */

/*
 * This routine is used to handle the "bottom half" processing for the
 * serial driver, known also the "software interrupt" processing.
 * This processing is done at the kernel interrupt level, after the
 * rs_interrupt() has returned, BUT WITH INTERRUPTS TURNED ON.  This
 * is where time-consuming activities which can not be done in the
 * interrupt driver proper are done; the interrupt driver schedules
 * them using rs_sched_event(), and they get done here.
 */

static void do_softint(unsigned long private_)
{
    struct async_struct *info = (struct async_struct *) private_;
    struct tty_struct   *tty;

    tty = info->tty;
    if (!tty)
        return;

    if (test_and_clear_bit(RS_EVENT_WRITE_WAKEUP, &info->event))
        tty_wakeup(tty);
}

/*
 * ---------------------------------------------------------------
 * Low level utility subroutines for the serial driver:  routines to
 * figure out the appropriate timeout for an interrupt chain, routines
 * to initialize and startup a serial port, and routines to shutdown a
 * serial port.  Useful stuff like that.
 * ---------------------------------------------------------------
 */

static int startup(struct async_struct * info)
{
    unsigned long flags;
    int retval=0;
    unsigned long page;

    if (info->flags & ASYNC_INITIALIZED)
    {
		return retval;
    }


    local_irq_save(flags);
    if ( !info->xmit.buf)
	{
    	page = get_zeroed_page(GFP_KERNEL);
    	if (!page)
		{
        	retval= -ENOMEM;
			goto errout;
		}
    	info->xmit.buf = (unsigned char *) page;
	}

#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("starting up ttys%d ...", info->line);
#endif

    /* Clear anything in the input buffer */
    if (serial_isroot())
    {
        if (info->tty)
            set_bit(TTY_IO_ERROR,
                    &info->tty->flags);
        retval = 0;
    }

    rs_enable_tx_interrupts(info);
    rs_enable_rx_interrupts(info);

    IRQ_ports = info;

    if (info->tty)
        clear_bit(TTY_IO_ERROR, &info->tty->flags);
    info->xmit.head = info->xmit.tail = 0;

    /*
     * Set up the tty->alt_speed kludge
     */
    if (info->tty)
    {
        if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_HI)
            info->tty->alt_speed = 57600;
        if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_VHI)
            info->tty->alt_speed = 115200;
        if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_SHI)
            info->tty->alt_speed = 230400;
        if ((info->flags & ASYNC_SPD_MASK) == ASYNC_SPD_WARP)
            info->tty->alt_speed = 460800;
    }

    /*
     * and set the speed of the serial port
     */
    change_speed(info, NULL);

    info->flags |= ASYNC_INITIALIZED;
    local_irq_restore(flags);
    return 0;

    errout:
    local_irq_restore(flags);
    return retval;
}

/*
 * This routine will shutdown a serial port; interrupts are disabled, and
 * DTR is dropped if the hangup on close termio flag is on.
 */
static void shutdown(struct async_struct * info)
{
    unsigned long   flags;
    struct serial_state *state;

    if (!(info->flags & ASYNC_INITIALIZED))
        return;

    state = info->state;

#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("Shutting down serial port %d ....\n", info->line);
#endif

    local_irq_save(flags); /* Disable interrupts */

    /*
     * clear delta_msr_wait queue to avoid mem leaks: we may free the irq
     * here so the queue might never be waken up
     */
    wake_up_interruptible(&info->delta_msr_wait);

    rs_disable_tx_interrupts(info);
    rs_disable_rx_interrupts(info);

    if (info->xmit.buf)
    {
        free_page((unsigned long) info->xmit.buf);
        info->xmit.buf = NULL;
    }

    IRQ_ports = NULL;

    if (info->tty)
        set_bit(TTY_IO_ERROR, &info->tty->flags);

    info->flags &= ~ASYNC_INITIALIZED;
    local_irq_restore(flags);
}


static int rs_put_char(struct tty_struct *tty, unsigned char ch)
{
    struct async_struct *info;
    unsigned long flags;

    if (!tty)
        return 0;

    info = tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_put_char"))
        return 0;

    if (!info->xmit.buf)
        return 0;

    local_irq_save(flags);
    if (CIRC_SPACE(info->xmit.head,
                   info->xmit.tail,
                   SERIAL_XMIT_SIZE) == 0)
    {
        local_irq_restore(flags);
        return 0;
    }

    info->xmit.buf[info->xmit.head++] = ch;
    info->xmit.head &= SERIAL_XMIT_SIZE-1;
    local_irq_restore(flags);
    return 1;
}

static void rs_flush_chars(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_flush_chars"))
        return;

    if (info->xmit.head == info->xmit.tail
        || tty->stopped
        || tty->hw_stopped
        || !info->xmit.buf)
        return;

    rs_enable_tx_interrupts(info);
}

static int rs_write(struct tty_struct * tty, const unsigned char *buf, int count)
{
    int c, ret = 0;
    struct async_struct *info;
    unsigned long flags;

    if (!tty)
        return 0;

    info = tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_write"))
        return 0;

    if (!info->xmit.buf)
        return 0;

    local_irq_save(flags);
    while (1)
    {
        c = CIRC_SPACE_TO_END(info->xmit.head,
                              info->xmit.tail,
                              SERIAL_XMIT_SIZE);
        if (count < c)
            c = count;
        if (c <= 0)
        {
            break;
        }
        memcpy(info->xmit.buf + info->xmit.head, buf, c);
        info->xmit.head = ((info->xmit.head + c) &
                           (SERIAL_XMIT_SIZE-1));
        buf += c;
        count -= c;
        ret += c;
    }
    local_irq_restore(flags);

    if (info->xmit.head != info->xmit.tail
        && !tty->stopped
        && !tty->hw_stopped
		)
    {
        rs_enable_tx_interrupts(info);
    }

    return ret;
}

static int rs_write_room(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_write_room"))
        return 0;

    return CIRC_SPACE(info->xmit.head, info->xmit.tail, SERIAL_XMIT_SIZE);
}

static int rs_chars_in_buffer(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_chars_in_buffer"))
        return 0;

    return CIRC_CNT(info->xmit.head, info->xmit.tail, SERIAL_XMIT_SIZE);
}

static void rs_flush_buffer(struct tty_struct *tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_flush_buffer"))
        return;

    local_irq_save(flags);
    info->xmit.head = info->xmit.tail = 0;
    local_irq_restore(flags);
    tty_wakeup(tty);
}

/*
 * This function is used to send a high-priority XON/XOFF character to
 * the device
 */
static void rs_send_xchar(struct tty_struct *tty, char ch)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;

    if (serial_paranoia_check(info, tty->name, "rs_send_char"))
        return;

    info->x_char = ch;
    if (ch)
    {
        rs_enable_tx_interrupts(info);
    }
}

#if 0
/*
 * ------------------------------------------------------------
 * rs_throttle()
 * 
 * This routine is called by the upper-layer tty layer to signal that
 * incoming characters should be throttled.
 * ------------------------------------------------------------
 */
static void rs_throttle(struct tty_struct * tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;
    unsigned long flags;
#ifdef CAMELOT_UART_DEBUG_THROTTLE
    char    buf[64];

    printk("throttle %s: %d....\n", tty_name(tty, buf),
           tty->ldisc.chars_in_buffer(tty));
#endif

    if (serial_paranoia_check(info, tty->name, "rs_throttle"))
        return;

    if (I_IXOFF(tty))
        rs_send_xchar(tty, STOP_CHAR(tty));
}

static void rs_unthrottle(struct tty_struct * tty)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;
    unsigned long flags;
#ifdef CAMELOT_UART_DEBUG_THROTTLE
    char    buf[64];

    printk("unthrottle %s: %d....\n", tty_name(tty, buf),
           tty->ldisc.chars_in_buffer(tty));
#endif

    if (serial_paranoia_check(info, tty->name, "rs_unthrottle"))
        return;

    if (I_IXOFF(tty))
    {
        if (info->x_char)
            info->x_char = 0;
        else
            rs_send_xchar(tty, START_CHAR(tty));
    }
}
#endif

/*
 * ------------------------------------------------------------
 * rs_ioctl() and friends
 * ------------------------------------------------------------
 */

static int get_serial_info(struct async_struct * info,
                           struct serial_struct __user * retinfo)
{
    struct serial_struct tmp;
    struct serial_state *state = info->state;

    if (!retinfo)
        return -EFAULT;
    memset(&tmp, 0, sizeof(tmp));
    lock_kernel();
    tmp.type = state->type;
    tmp.line = state->line;
    tmp.port = state->port;
    tmp.irq = state->irq;
    tmp.flags = state->flags;
    tmp.xmit_fifo_size = state->xmit_fifo_size;
    tmp.baud_base = state->baud_base;
    tmp.close_delay = state->close_delay;
    tmp.closing_wait = state->closing_wait;
    tmp.custom_divisor = state->custom_divisor;
    unlock_kernel();
    if (copy_to_user(retinfo,&tmp,sizeof(*retinfo)))
        return -EFAULT;
    return 0;
}

static int set_serial_info(struct async_struct * info,
                           struct serial_struct __user * new_info)
{
    struct serial_struct new_serial;
    struct serial_state old_state, *state;
    unsigned int        change_irq,change_port;
    int             retval = 0;

    if (copy_from_user(&new_serial,new_info,sizeof(new_serial)))
        return -EFAULT;

    lock_kernel();
    state = info->state;
    old_state = *state;

    change_irq = new_serial.irq != state->irq;
    change_port = (new_serial.port != state->port);
    if (change_irq || change_port || (new_serial.xmit_fifo_size != state->xmit_fifo_size))
    {
        unlock_kernel();
        return -EINVAL;
    }

    if (!serial_isroot())
    {
        if ((new_serial.baud_base != state->baud_base) ||
            (new_serial.close_delay != state->close_delay) ||
            (new_serial.xmit_fifo_size != state->xmit_fifo_size) ||
            ((new_serial.flags & ~ASYNC_USR_MASK) !=
             (state->flags & ~ASYNC_USR_MASK)))
            return -EPERM;
        state->flags = ((state->flags & ~ASYNC_USR_MASK) |
                        (new_serial.flags & ASYNC_USR_MASK));
        info->flags = ((info->flags & ~ASYNC_USR_MASK) |
                       (new_serial.flags & ASYNC_USR_MASK));
        state->custom_divisor = new_serial.custom_divisor;
        goto check_and_exit;
    }

    if (new_serial.baud_base < 9600)
    {
        unlock_kernel();
        return -EINVAL;
    }

    /*
     * OK, past this point, all the error checking has been done.
     * At this point, we start making changes.....
     */

    state->baud_base = new_serial.baud_base;
    state->flags = ((state->flags & ~ASYNC_FLAGS) |
                    (new_serial.flags & ASYNC_FLAGS));
    info->flags = ((state->flags & ~ASYNC_INTERNAL_FLAGS) |
                   (info->flags & ASYNC_INTERNAL_FLAGS));
    state->custom_divisor = new_serial.custom_divisor;
    state->close_delay = new_serial.close_delay * HZ/100;
    state->closing_wait = new_serial.closing_wait * HZ/100;
    info->tty->low_latency = (info->flags & ASYNC_LOW_LATENCY) ? 1 : 0;

    check_and_exit:
    if (info->flags & ASYNC_INITIALIZED)
    {
        if (((old_state.flags & ASYNC_SPD_MASK) !=
             (state->flags & ASYNC_SPD_MASK)) ||
            (old_state.custom_divisor != state->custom_divisor))
        {
            if ((state->flags & ASYNC_SPD_MASK) == ASYNC_SPD_HI)
                info->tty->alt_speed = 57600;
            if ((state->flags & ASYNC_SPD_MASK) == ASYNC_SPD_VHI)
                info->tty->alt_speed = 115200;
            if ((state->flags & ASYNC_SPD_MASK) == ASYNC_SPD_SHI)
                info->tty->alt_speed = 230400;
            if ((state->flags & ASYNC_SPD_MASK) == ASYNC_SPD_WARP)
                info->tty->alt_speed = 460800;
            change_speed(info, NULL);
        }
    } else
        retval = startup(info);
    unlock_kernel();
    return retval;
}

#if 0
/*
 * get_lsr_info - get line status register info
 *
 * Purpose: Let user call ioctl() to get info when the UART physically
 * 	    is emptied.  On bus types like RS485, the transmitter must
 * 	    release the bus after transmitting. This must be done when
 * 	    the transmit shift register is empty, not be done when the
 * 	    transmit holding register is empty.  This functionality
 * 	    allows an RS485 driver to be written in user space. 
 */
static int get_lsr_info(struct async_struct * info, unsigned int __user *value)
{
    unsigned char status;
    unsigned int result;
    unsigned long flags;

#if 0
    local_irq_save(flags);
    status = custom.serdatr;
    mb();
    local_irq_restore(flags);
    result = ((status & SDR_TSRE) ? TIOCSER_TEMT : 0);
#endif
    if (copy_to_user(value, &result, sizeof(int)))
        return -EFAULT;
    return 0;
}


static int rs_tiocmget(struct tty_struct *tty, struct file *file)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    unsigned char control, status;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_ioctl"))
        return -ENODEV;
    if (tty->flags & (1 << TTY_IO_ERROR))
        return -EIO;
#if 0
    control = info->MCR;
    local_irq_save(flags);
    status = ciab.pra;
    local_irq_restore(flags);
    return((control & SER_RTS) ? TIOCM_RTS : 0)
    | ((control & SER_DTR) ? TIOCM_DTR : 0)
    | (!(status  & SER_DCD) ? TIOCM_CAR : 0)
    | (!(status  & SER_DSR) ? TIOCM_DSR : 0)
    | (!(status  & SER_CTS) ? TIOCM_CTS : 0);
#endif
}

static int rs_tiocmset(struct tty_struct *tty, struct file *file,
                       unsigned int set, unsigned int clear)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_ioctl"))
        return -ENODEV;
    if (tty->flags & (1 << TTY_IO_ERROR))
        return -EIO;

    local_irq_save(flags);
#if 0
    if (set & TIOCM_RTS)
        info->MCR |= SER_RTS;
    if (set & TIOCM_DTR)
        info->MCR |= SER_DTR;
    if (clear & TIOCM_RTS)
        info->MCR &= ~SER_RTS;
    if (clear & TIOCM_DTR)
        info->MCR &= ~SER_DTR;
    rtsdtr_ctrl(info->MCR);
#endif
    local_irq_restore(flags);
    return 0;
}
#endif

/*
 * rs_break() --- routine which turns the break handling on or off
 */
static void rs_break(struct tty_struct *tty, int break_state)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_break"))
        return;

    local_irq_save(flags);
#if 0
    if (break_state == -1)
        custom.adkcon = AC_SETCLR | AC_UARTBRK;
    else
        custom.adkcon = AC_UARTBRK;
#endif
    mb();
    local_irq_restore(flags);
}


static int rs_ioctl(struct tty_struct *tty, struct file * file,
                    unsigned int cmd, unsigned long arg)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    struct async_icount cprev, cnow;    /* kernel counter temps */
    struct serial_icounter_struct icount;
    void __user *argp = (void __user *)arg;
    unsigned long flags;

    if (serial_paranoia_check(info, tty->name, "rs_ioctl"))
        return -ENODEV;

    if ((cmd != TIOCGSERIAL) && (cmd != TIOCSSERIAL) &&
        (cmd != TIOCSERCONFIG) && (cmd != TIOCSERGSTRUCT) &&
        (cmd != TIOCMIWAIT) && (cmd != TIOCGICOUNT))
    {
        if (tty->flags & (1 << TTY_IO_ERROR))
            return -EIO;
    }

    switch (cmd)
    {
    case TIOCGSERIAL:
        return get_serial_info(info, argp);
    case TIOCSSERIAL:
        return set_serial_info(info, argp);
    case TIOCSERCONFIG:
        return 0;
#if 0
    case TIOCSERGETLSR: /* Get line status register */
        return get_lsr_info(info, argp);
#endif
    case TIOCSERGSTRUCT:
        if (copy_to_user(argp,
                         info, sizeof(struct async_struct)))
            return -EFAULT;
        return 0;

        /*
         * Wait for any of the 4 modem inputs (DCD,RI,DSR,CTS) to change
         * - mask passed in arg for lines of interest
         *   (use |'ed TIOCM_RNG/DSR/CD/CTS for masking)
         * Caller should use TIOCGICOUNT to see which one it was
         */
    case TIOCMIWAIT:
        local_irq_save(flags);
        /* note the counters on entry */
        cprev = info->state->icount;
        local_irq_restore(flags);
        while (1)
        {
            interruptible_sleep_on(&info->delta_msr_wait);
            /* see if a signal did it */
            if (signal_pending(current))
                return -ERESTARTSYS;
            local_irq_save(flags);
            cnow = info->state->icount; /* atomic copy */
            local_irq_restore(flags);
            if (cnow.rng == cprev.rng && cnow.dsr == cprev.dsr && 
                cnow.dcd == cprev.dcd && cnow.cts == cprev.cts)
                return -EIO; /* no change => error */
            if ( ((arg & TIOCM_RNG) && (cnow.rng != cprev.rng)) ||
                 ((arg & TIOCM_DSR) && (cnow.dsr != cprev.dsr)) ||
                 ((arg & TIOCM_CD)  && (cnow.dcd != cprev.dcd)) ||
                 ((arg & TIOCM_CTS) && (cnow.cts != cprev.cts)) )
            {
                return 0;
            }
            cprev = cnow;
        }
        /* NOTREACHED */

        /* 
         * Get counter of input serial line interrupts (DCD,RI,DSR,CTS)
         * Return: write counters to the user passed counter struct
         * NB: both 1->0 and 0->1 transitions are counted except for
         *     RI where only 0->1 is counted.
         */
    case TIOCGICOUNT:
        local_irq_save(flags);
        cnow = info->state->icount;
        local_irq_restore(flags);
        icount.cts = cnow.cts;
        icount.dsr = cnow.dsr;
        icount.rng = cnow.rng;
        icount.dcd = cnow.dcd;
        icount.rx = cnow.rx;
        icount.tx = cnow.tx;
        icount.frame = cnow.frame;
        icount.overrun = cnow.overrun;
        icount.parity = cnow.parity;
        icount.brk = cnow.brk;
        icount.buf_overrun = cnow.buf_overrun;

        if (copy_to_user(argp, &icount, sizeof(icount)))
            return -EFAULT;
        return 0;
    case TIOCSERGWILD:
    case TIOCSERSWILD:
        /* "setserial -W" is called in Debian boot */
        printk ("TIOCSER?WILD ioctl obsolete, ignored.\n");
        return 0;

    default:
        return -ENOIOCTLCMD;
    }
    return 0;
}

static void rs_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
{
    struct async_struct *info = (struct async_struct *)tty->driver_data;
    unsigned int cflag = tty->termios->c_cflag;

    change_speed(info, old_termios);

    /* Handle transition to B0 status */
    if ((old_termios->c_cflag & CBAUD) &&
        !(cflag & CBAUD))
    {
#if 1

#else
        info->MCR &= ~(SER_DTR|SER_RTS);
        local_irq_save(flags);
        rtsdtr_ctrl(info->MCR);
        local_irq_restore(flags);
#endif
    }

    /* Handle transition away from B0 status */
    if (!(old_termios->c_cflag & CBAUD) &&
        (cflag & CBAUD))
    {
#if 1

#else
        info->MCR |= SER_DTR;
        if (!(tty->termios->c_cflag & CRTSCTS) || 
            !test_bit(TTY_THROTTLED, &tty->flags))
        {
            info->MCR |= SER_RTS;
        }
        local_irq_save(flags);
        rtsdtr_ctrl(info->MCR);
        local_irq_restore(flags);
#endif
    }
    /* Handle turning off CRTSCTS */
    if ((old_termios->c_cflag & CRTSCTS) &&
        !(tty->termios->c_cflag & CRTSCTS))
    {
        tty->hw_stopped = 0;
        rs_start(tty);
    }

#if 1
    /*
     * No need to wake up processes in open wait, since they
     * sample the CLOCAL flag once, and don't recheck it.
     * XXX  It's not clear whether the current behavior is correct
     * or not.  Hence, this may change.....
     */
    if (!(old_termios->c_cflag & CLOCAL) &&
        (tty->termios->c_cflag & CLOCAL))
        wake_up_interruptible(&info->open_wait);
#endif
}

/*
* ------------------------------------------------------------
* rs_close()
* 
* This routine is called when the serial port gets closed.  First, we
* wait for the last remaining data to be sent.  Then, we unlink its
* async structure from the interrupt chain if necessary, and we free
* that IRQ if nothing is left in the chain.
* ------------------------------------------------------------
*/
static void rs_close(struct tty_struct *tty, struct file * filp)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    struct serial_state *state;
    unsigned long flags;

    if (!info || serial_paranoia_check(info, tty->name, "rs_close"))
        return;

    state = info->state;

    local_irq_save(flags);

    if (tty_hung_up_p(filp))
    {
        local_irq_restore(flags);
        return;
    }

#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("rs_close ttys%d, count = %d\n", info->line, state->count);
#endif
    if ((tty->count == 1) && (state->count != 1))
    {
        /*
         * Uh, oh.  tty->count is 1, which means that the tty
         * structure will be freed.  state->count should always
         * be one in these conditions.  If it's greater than
         * one, we've got real problems, since it means the
         * serial port won't be shutdown.
         */
        printk("rs_close: bad serial port count; tty->count is 1, "
               "state->count is %d\n", state->count);
        state->count = 1;
    }
    if (--state->count < 0)
    {
        printk("rs_close: bad serial port count for ttys%d: %d\n",
               info->line, state->count);
        state->count = 0;
    }
    if (state->count)
    {
        local_irq_restore(flags);
        return;
    }
    info->flags |= ASYNC_CLOSING;
    /*
     * Now we wait for the transmit buffer to clear; and we notify 
     * the line discipline to only process XON/XOFF characters.
     */
    tty->closing = 1;
    if (info->closing_wait != ASYNC_CLOSING_WAIT_NONE)
        tty_wait_until_sent(tty, info->closing_wait);
    /*
     * At this point we stop accepting input.  To do this, we
     * disable the receive line status interrupts, and tell the
     * interrupt driver to stop checking the data ready bit in the
     * line status register.
     */
    info->read_status_mask &= ~UART_LSR_DR;
    if (info->flags & ASYNC_INITIALIZED)
    {
        /*
         * Before we drop DTR, make sure the UART transmitter
         * has completely drained; this is especially
         * important if there is a transmit FIFO!
         */
        rs_wait_until_sent(tty, info->timeout);
    }
    shutdown(info);
    rs_flush_buffer(tty);

    tty_ldisc_flush(tty);
    tty->closing = 0;
    info->event = 0;
    info->tty = NULL;
    if (info->blocked_open)
    {
        if (info->close_delay)
        {
            msleep_interruptible(jiffies_to_msecs(info->close_delay));
        }
        wake_up_interruptible(&info->open_wait);
    }
    info->flags &= ~(ASYNC_NORMAL_ACTIVE|ASYNC_CLOSING);
    wake_up_interruptible(&info->close_wait);
    local_irq_restore(flags);
}

/*
 * rs_wait_until_sent() --- wait until the transmitter is empty
 */
static void rs_wait_until_sent(struct tty_struct *tty, int timeout)
{
#if 1
    return;
#else
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    unsigned long orig_jiffies, char_time;
    int lsr;

    if (serial_paranoia_check(info, tty->name, "rs_wait_until_sent"))
        return;

    if (info->xmit_fifo_size == 0)
        return; /* Just in case.... */

    orig_jiffies = jiffies;

    lock_kernel();
    /*
     * Set the check interval to be 1/5 of the estimated time to
     * send a single character, and make it at least 1.  The check
     * interval should also be less than the timeout.
     * 
     * Note: we have to use pretty tight timings here to satisfy
     * the NIST-PCTS.
     */
    char_time = (info->timeout - HZ/50) / info->xmit_fifo_size;
    char_time = char_time / 5;
    if (char_time == 0)
        char_time = 1;
    if (timeout)
        char_time = min_t(unsigned long, char_time, timeout);
    /*
     * If the transmitter hasn't cleared in twice the approximate
     * amount of time to send the entire FIFO, it probably won't
     * ever clear.  This assumes the UART isn't doing flow
     * control, which is currently the case.  Hence, if it ever
     * takes longer than info->timeout, this is probably due to a
     * UART bug of some kind.  So, we clamp the timeout parameter at
     * 2*info->timeout.
     */
    if (!timeout || timeout > 2*info->timeout)
        timeout = 2*info->timeout;
#ifdef CAMELOT_UART_DEBUG_RS_WAIT_UNTIL_SENT
    printk("In rs_wait_until_sent(%d) check=%lu...", timeout, char_time);
    printk("jiff=%lu...", jiffies);
#endif
#if 0
    while (!((lsr = custom.serdatr) & SDR_TSRE))
    {
#ifdef CAMELOT_UART_DEBUG_RS_WAIT_UNTIL_SENT
        printk("serdatr = %d (jiff=%lu)...", lsr, jiffies);
#endif
        msleep_interruptible(jiffies_to_msecs(char_time));
        if (signal_pending(current))
            break;
        if (timeout && time_after(jiffies, orig_jiffies + timeout))
            break;
    }
#endif
    __set_current_state(TASK_RUNNING);
    unlock_kernel();
#ifdef CAMELOT_UART_DEBUG_RS_WAIT_UNTIL_SENT
    printk("lsr = %d (jiff=%lu)...done\n", lsr, jiffies);
#endif
#endif
}

/*
 * rs_hangup() --- called by tty_hangup() when a hangup is signaled.
 */
static void rs_hangup(struct tty_struct *tty)
{
    struct async_struct * info = (struct async_struct *)tty->driver_data;
    struct serial_state *state = info->state;

    if (serial_paranoia_check(info, tty->name, "rs_hangup"))
        return;

    state = info->state;

    rs_flush_buffer(tty);
    shutdown(info);
    info->event = 0;
    state->count = 0;
    info->flags &= ~ASYNC_NORMAL_ACTIVE;
    info->tty = NULL;
    wake_up_interruptible(&info->open_wait);
}

/*
 * ------------------------------------------------------------
 * rs_open() and friends
 * ------------------------------------------------------------
 */
static int block_til_ready(struct tty_struct *tty, struct file * filp,
                           struct async_struct *info)
{
#ifdef DECLARE_WAITQUEUE
    DECLARE_WAITQUEUE(wait, current);
#else
    struct wait_queue wait = { current, NULL};
#endif
    struct serial_state *state = info->state;
    int     retval;
    int     do_clocal = 0, extra_count = 0;
    unsigned long   flags;

    /*
     * If the device is in the middle of being closed, then block
     * until it's done, and then try again.
     */
    if (tty_hung_up_p(filp) ||
        (info->flags & ASYNC_CLOSING))
    {
        if (info->flags & ASYNC_CLOSING)
            interruptible_sleep_on(&info->close_wait);
#ifdef SERIAL_DO_RESTART
        return((info->flags & ASYNC_HUP_NOTIFY) ?
               -EAGAIN : -ERESTARTSYS);
#else
        return -EAGAIN;
#endif
    }

    /*
     * If non-blocking mode is set, or the port is not enabled,
     * then make the check up front and then exit.
     */
    if ((filp->f_flags & O_NONBLOCK) ||
        (tty->flags & (1 << TTY_IO_ERROR)))
    {
        info->flags |= ASYNC_NORMAL_ACTIVE;
        return 0;
    }

    if (tty->termios->c_cflag & CLOCAL)
        do_clocal = 1;

    /*
     * Block waiting for the carrier detect and the line to become
     * free (i.e., not in use by the callout).  While we are in
     * this loop, state->count is dropped by one, so that
     * rs_close() knows when to free things.  We restore it upon
     * exit, either normal or abnormal.
     */
    retval = 0;
    add_wait_queue(&info->open_wait, &wait);
#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("block_til_ready before block: ttys%d, count = %d\n",
           state->line, state->count);
#endif
    local_irq_save(flags);
    if (!tty_hung_up_p(filp))
    {
        extra_count = 1;
        state->count--;
    }
    local_irq_restore(flags);
    info->blocked_open++;
    while (1)
    {
if (1)
break;
#if 0
        local_irq_save(flags);
        if (tty->termios->c_cflag & CBAUD)
            rtsdtr_ctrl(SER_DTR|SER_RTS);
        local_irq_restore(flags);
#endif
        set_current_state(TASK_INTERRUPTIBLE);
        if (tty_hung_up_p(filp) ||
            !(info->flags & ASYNC_INITIALIZED))
        {
#ifdef SERIAL_DO_RESTART
            if (info->flags & ASYNC_HUP_NOTIFY)
                retval = -EAGAIN;
            else
                retval = -ERESTARTSYS;
#else
            retval = -EAGAIN;
#endif
            break;
        }
#if 0
        if (!(info->flags & ASYNC_CLOSING) &&
            (do_clocal || (!(ciab.pra & SER_DCD)) ))
            break;
#endif
        if (signal_pending(current))
        {
            retval = -ERESTARTSYS;
            break;
        }
#ifdef CAMELOT_UART_DEBUG_OPEN
        printk("block_til_ready blocking: ttys%d, count = %d\n",
               info->line, state->count);
#endif
        schedule();
    }
    __set_current_state(TASK_RUNNING);
    remove_wait_queue(&info->open_wait, &wait);
    if (extra_count)
        state->count++;
    info->blocked_open--;
#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("block_til_ready after blocking: ttys%d, count = %d\n",
           info->line, state->count);
#endif
    if (retval)
        return retval;
    info->flags |= ASYNC_NORMAL_ACTIVE;
    return 0;
}

static int get_async_struct(int line, struct async_struct **ret_info)
{
    struct async_struct *info;
    struct serial_state *sstate;

    sstate = rs_table + line;
    sstate->count++;
    if (sstate->info)
    {
        *ret_info = sstate->info;
        return 0;
    }
    info = kzalloc(sizeof(struct async_struct), GFP_KERNEL);
    if (!info)
    {
        sstate->count--;
        return -ENOMEM;
    }
#ifdef DECLARE_WAITQUEUE
    init_waitqueue_head(&info->open_wait);
    init_waitqueue_head(&info->close_wait);
    init_waitqueue_head(&info->delta_msr_wait);
#endif
    info->magic = SERIAL_MAGIC;
    info->port = sstate->port;
    info->flags = sstate->flags;
    info->xmit_fifo_size = sstate->xmit_fifo_size;
    info->line = line;
    tasklet_init(&info->tlet, do_softint, (unsigned long)info);
    info->state = sstate;
    if (sstate->info)
    {
        kfree(info);
        *ret_info = sstate->info;
        return 0;
    }
    *ret_info = sstate->info = info;
    return 0;
}

/*
 * This routine is called whenever a serial port is opened.  It
 * enables interrupts for a serial port, linking in its async structure into
 * the IRQ chain.   It also performs the serial-specific
 * initialization for the tty structure.
 */
static int rs_open(struct tty_struct *tty, struct file * filp)
{
    struct async_struct *info;
    int             retval, line;

    line = tty->index;
    if ((line < 0) || (line >= NR_PORTS))
    {
        return -ENODEV;
    }
    retval = get_async_struct(line, &info);
    if (retval)
    {
        return retval;
    }
    tty->driver_data = info;
    info->tty = tty;
    if (serial_paranoia_check(info, tty->name, "rs_open"))
        return -ENODEV;

#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("rs_open %s, count = %d\n", tty->name, info->state->count);
#endif
    info->tty->low_latency = (info->flags & ASYNC_LOW_LATENCY) ? 1 : 0;

    /*
     * If the port is the middle of closing, bail out now
     */
    if (tty_hung_up_p(filp) ||
        (info->flags & ASYNC_CLOSING))
    {
        if (info->flags & ASYNC_CLOSING)
            interruptible_sleep_on(&info->close_wait);
#ifdef SERIAL_DO_RESTART
        return((info->flags & ASYNC_HUP_NOTIFY) ?
               -EAGAIN : -ERESTARTSYS);
#else
        return -EAGAIN;
#endif
    }

    /*
     * Start up serial port
     */
    retval = startup(info);
    if (retval)
    {
        return retval;
    }

    retval = block_til_ready(tty, filp, info);
    if (retval)
    {
#ifdef CAMELOT_UART_DEBUG_OPEN
        printk("rs_open returning after block_til_ready with %d\n",
               retval);
#endif
        return retval;
    }

#ifdef CAMELOT_UART_DEBUG_OPEN
    printk("rs_open %s successful...", tty->name);
#endif
    return 0;
}

#if 0
/*
 * /proc fs routines....
 */

static inline int line_info(char *buf, struct serial_state *state)
{
    struct async_struct *info = state->info, scr_info;
    char    stat_buf[30], control, status;
    int ret;
    unsigned long flags;

    ret = sprintf(buf, "%d: uart:amiga_builtin",state->line);

    /*
     * Figure out the current RS-232 lines
     */
    if (!info)
    {
        info = &scr_info;   /* This is just for serial_{in,out} */

        info->magic = SERIAL_MAGIC;
        info->flags = state->flags;
        info->quot = 0;
        info->tty = NULL;
    }
#if 1

#else
    local_irq_save(flags);
    status = ciab.pra;
    control = info ? info->MCR : status;
    local_irq_restore(flags);
#endif
    stat_buf[0] = 0;
    stat_buf[1] = 0;
#if 1

#else
    if (!(control & SER_RTS))
        strcat(stat_buf, "|RTS");
    if (!(status & SER_CTS))
        strcat(stat_buf, "|CTS");
    if (!(control & SER_DTR))
        strcat(stat_buf, "|DTR");
    if (!(status & SER_DSR))
        strcat(stat_buf, "|DSR");
    if (!(status & SER_DCD))
        strcat(stat_buf, "|CD");
#endif
    if (info->quot)
    {
        ret += sprintf(buf+ret, " baud:%d",
                       state->baud_base / info->quot);
    }

    ret += sprintf(buf+ret, " tx:%d rx:%d",
                   state->icount.tx, state->icount.rx);

    if (state->icount.frame)
        ret += sprintf(buf+ret, " fe:%d", state->icount.frame);

    if (state->icount.parity)
        ret += sprintf(buf+ret, " pe:%d", state->icount.parity);

    if (state->icount.brk)
        ret += sprintf(buf+ret, " brk:%d", state->icount.brk);

    if (state->icount.overrun)
        ret += sprintf(buf+ret, " oe:%d", state->icount.overrun);

    /*
     * Last thing is the RS-232 status lines
     */
    ret += sprintf(buf+ret, " %s\n", stat_buf+1);
    return ret;
}

static int rs_read_proc(char *page, char **start, off_t off, int count,
                        int *eof, void *data)
{
    int len = 0, l;
    off_t   begin = 0;

    len += sprintf(page, "serinfo:1.0 driver:%s\n", serial_version);
    l = line_info(page + len, &rs_table[0]);
    len += l;
    if (len+begin > off+count)
        goto done;
    if (len+begin < off)
    {
        begin += len;
        len = 0;
    }
    *eof = 1;
    done:
    if (off >= len+begin)
        return 0;
    *start = page + (off-begin);
    return((count < begin+len-off) ? count : begin+len-off);
}
#endif

#ifdef CONFIG_CONSOLE_POLL
static int camelot_uart_poll_init(struct tty_driver *driver, int line, char *options)
{
    return 0;
}

static int camelot_uart_poll_get_char(struct tty_driver *driver, int line)
{
    volatile int *p = (int *) UR_BASE;
    int ch;

	while(1)
	{
    	if ((1<<23)& (ch = p[URBR]) ) // rx flag on, break
			break;
	} 
    return ch>>24;
}

static void camelot_uart_poll_put_char(struct tty_driver *driver, int line, char ch)
{
    int i;
    volatile int *p = (int *) UR_BASE;

    /* Wait for UARTA_TX register to empty */
    i = 1000000;
    while ((p[URCS>>2] & URCS_TF) && i--);
    /* Send the character */
    p[URBR>>2] = (int)ch<<24;
}
#endif

static const struct tty_operations serial_ops = {
    .open = rs_open,
    .close = rs_close,
    .write = rs_write,
    .put_char = rs_put_char,
    .flush_chars = rs_flush_chars,
    .write_room = rs_write_room,
    .chars_in_buffer = rs_chars_in_buffer,
    .flush_buffer = rs_flush_buffer,
    .ioctl = rs_ioctl,
    //.throttle = rs_throttle,
    //.unthrottle = rs_unthrottle,
    .set_termios = rs_set_termios,
    .stop = rs_stop,
    .start = rs_start,
    .hangup = rs_hangup,
    .break_ctl = rs_break,
    .send_xchar = rs_send_xchar,
    .wait_until_sent = rs_wait_until_sent,
    //.read_proc = rs_read_proc,
    //.tiocmget = rs_tiocmget,
    //.tiocmset = rs_tiocmset,
#ifdef CONFIG_CONSOLE_POLL
    .poll_init  = camelot_uart_poll_init,
    .poll_get_char  = camelot_uart_poll_get_char,
    .poll_put_char  = camelot_uart_poll_put_char,
#endif 
};

static int __init camelot_rs_init(void)
{
    unsigned long flags;
    struct serial_state * state;

    serial_driver = alloc_tty_driver(1);
    if (!serial_driver)
        return -ENOMEM;

    IRQ_ports = NULL;

    /* Initialize the tty_driver structure */

    serial_driver->owner = THIS_MODULE;
    serial_driver->driver_name = "camelot_uart";
    serial_driver->name = "ttyS";
    serial_driver->major = TTY_MAJOR;
    serial_driver->minor_start = 64;
    serial_driver->type = TTY_DRIVER_TYPE_SERIAL;
    serial_driver->subtype = SERIAL_TYPE_NORMAL;
    serial_driver->init_termios = tty_std_termios;
    serial_driver->init_termios.c_cflag =
    B1152000 | CS8 | CREAD | HUPCL | CLOCAL;
    serial_driver->flags = TTY_DRIVER_REAL_RAW;
    /* EZP:
    serial_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
    */ 
    tty_set_operations(serial_driver, &serial_ops);

    if (tty_register_driver(serial_driver))
        panic("Couldn't register serial driver\n");

    state = rs_table;
    state->magic = SSTATE_MAGIC;
#if 0
    state->port = (int)&custom.serdatr; /* Just to give it a value */
#endif
    state->line = 0;
    state->custom_divisor = 0;
    state->close_delay = 5*HZ/10;
    state->closing_wait = 30*HZ;
    state->icount.cts = state->icount.dsr = 
                        state->icount.rng = state->icount.dcd = 0;
    state->icount.rx = state->icount.tx = 0;
    state->icount.frame = state->icount.parity = 0;
    state->icount.overrun = state->icount.brk = 0;

    printk(KERN_INFO "ttyS%d is enabled\n",
           state->line);

    state->xmit_fifo_size = 16;
    local_irq_save(flags);

    if (0 > request_irq( IRQ_UR, camelot_uart_interrupt, IRQF_DISABLED, "camelot uart0", state))
        panic("Couldn't request IRQ for UART device\n");

    local_irq_restore(flags);

    return 0;
}

static __exit void camelot_rs_exit(void) 
{
    int error;
    struct async_struct *info = rs_table[0].info;

    /* printk("Unloading %s: version %s\n", serial_name, serial_version); */
    tasklet_kill(&info->tlet);
    if ((error = tty_unregister_driver(serial_driver)))
        printk("SERIAL: failed to unregister serial driver (%d)\n",
               error);
    put_tty_driver(serial_driver);

    if (info)
    {
        rs_table[0].info = NULL;
        kfree(info);
    }
}

module_init(camelot_rs_init)
module_exit(camelot_rs_exit)


#ifdef CONFIG_SERIAL_CONSOLE

void camelot_serial_outc(unsigned char c)
{
    int i;
    volatile int *p = (int *) UR_BASE;
    unsigned int res;

    /* Disable UARTA_TX interrupts */
    res = p[URCS>>2] ;
    p[URCS>>2] &= ~URCS_TIE;

    /* Wait for UARTA_TX register to empty */
    i = 10000;
    while ((p[URCS>>2] & URCS_TF) && i--);

    /* Send the character */
    p[URBR>>2] = (int)c<<24;

    /* Enable UARTA_TX interrupts */
    p[URCS>>2] = res;
}

    #if 0
static int serial_console_wait_key(struct console *co)
{
    int c;
    volatile int *p = (int *) UR_BASE;
    unsigned int res;

    /* Disable RX interrupts */
    res = p[URCS>>2] ;
    p[URCS>>2] &= ~URCS_RIE;

    /* Wait fo rx */
    while (!(p[URCS>>2] & URCS_RF));

    c = p[URBR>>2]>>24;

    /* Enable RX interrupts */
    p[URCS>>2] = res;
    return c;
}
    #endif

static int serial_console_setup(struct console *co, char *options)
{
    volatile int *p = (int *) UR_BASE;
    int baud = 115200;
    int bits = 8;
    int parity = 'n';
    char *s;
    int brsr = 0;

    if (options)
    {
        baud = simple_strtoul(options, NULL, 10);
        s = options;
        while (*s >= '0' && *s <= '9')
            s++;
        if (*s) parity = *s++;
        if (*s) bits   = *s++ - '0';
    }

    if (baud >0)
        brsr = UART_CLK/baud;
#if 0
    switch (bits)
    {
    case 7:
        break;
    default:
        break;
    }
    switch (parity)
    {
    case 'o':
    case 'O':
        ctrl1 |= CAMELOT_UART_CTRL1_ENPARITY;
        break;
    case 'e':
    case 'E':
        ctrl1 |= (CAMELOT_UART_CTRL1_ENPARITY |
                  CAMELOT_UART_CTRL1_EVENPARITY);
        break;
    default:
        break;
    }
#endif

    /* Write the control registers */
    p[URCS>>2] = (brsr<<16) ; //| (7<<URCS_RTH_S);

    return 0;
}

static void serial_console_write(struct console *co, const char *s,
                                 unsigned count)
{

    while (count--)
    {
        if (*s == '\n')
            camelot_serial_outc('\r');
        camelot_serial_outc(*s++);
    }
}

static struct tty_driver *serial_console_device(struct console *c, int *index)
{
    *index = 0;
    return serial_driver;
}

static struct console sercons = {
    .name =     "ttyS",
    .write =    serial_console_write,
    .device =   serial_console_device,
    .setup =    serial_console_setup,
    .flags =    CON_PRINTBUFFER,
    .index =    -1,
};

/*
 *	Register console.
 */
static int __init camelot_serial_console_init(void)
{
    register_console(&sercons);
    return 0;
}
console_initcall(camelot_serial_console_init);

#endif

