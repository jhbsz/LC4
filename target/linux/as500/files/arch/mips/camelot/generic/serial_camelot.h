/*
 *
 *  Serial driver for Camelot
 *
 */
#include <linux/serialP.h>
#include <linux/generic_serial.h>

//#undef CAMELOT_UART_DEBUG
#define CAMELOT_UART_DEBUG

#ifdef CAMELOT_UART_DEBUG
#define CAMELOT_UART_DEBUG_OPEN		0x00000001
#define CAMELOT_UART_DEBUG_SETTING	0x00000002
#define CAMELOT_UART_DEBUG_FLOW		0x00000004
#define CAMELOT_UART_DEBUG_MODEMSIGNALS	0x00000008
#define CAMELOT_UART_DEBUG_TERMIOS	0x00000010
#define CAMELOT_UART_DEBUG_TRANSMIT	0x00000020
#define CAMELOT_UART_DEBUG_RECEIVE	0x00000040
#define CAMELOT_UART_DEBUG_INTERRUPTS	0x00000080
#define CAMELOT_UART_DEBUG_PROBE		0x00000100
#define CAMELOT_UART_DEBUG_INIT		0x00000200
#define CAMELOT_UART_DEBUG_CLEANUP	0x00000400
#define CAMELOT_UART_DEBUG_CLOSE		0x00000800
#define CAMELOT_UART_DEBUG_FIRMWARE	0x00001000
#define CAMELOT_UART_DEBUG_MEMTEST	0x00002000
#define CAMELOT_UART_DEBUG_THROTTLE	0x00004000
#define CAMELOT_UART_DEBUG_NO_TX		0xffffffdf
#define CAMELOT_UART_DEBUG_ALL		0xffffffff

#define rs_dprintk(f, str...) if(CAMELOT_UART_DEBUG_NO_TX & f) printk(str)
#define func_enter() rs_dprintk(CAMELOT_UART_DEBUG_FLOW,		\
				"rs: enter " __func__ "\n")
#define func_exit() rs_dprintk(CAMELOT_UART_DEBUG_FLOW,		\
				"rs: exit " __func__ "\n")
#else
#define rs_dprintk(f, str...)
#define func_enter()
#define func_exit()
#endif

/*
 * Hardware specific serial port structure
 */
struct rs_port { 	
	struct gs_port		gs;		/* Must be first field! */
	struct wait_queue	*shutdown_wait; 
	int			stat_flags;
	struct async_icount	icount;		/* Counters for 4 input IRQs */
	int			read_status_mask;
	int			ignore_status_mask;
	int			x_char;		/* XON/XOFF character */
}; 
