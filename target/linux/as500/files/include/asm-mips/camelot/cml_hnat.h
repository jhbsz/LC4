#ifndef	CML_HNAT_H
#define	CML_HNAT_H

#ifdef	__ECOS
#define HNAT_LOCK_INIT
#define HNAT_LOCK			cyg_scheduler_lock()
#define HNAT_UNLOCK			cyg_scheduler_unlock()
#endif

#define CML_HNAT_NOHNAT		0
#define CML_HNAT_FULLHNAT	1
#define CML_HNAT_SEMIHNAT	2

#define CML_HNAT_PRIVATE_IF	0
#define CML_HNAT_PUBLIC_IF	1
	

#define	HNRC_LEARNMAC		2
#define HNRC_TX				1
#define HNRC_OK				0
#define HNRC_ERR			-1
#define HNRC_SAME			-2

#define	HNAT_DBG_NATDS		(1<<1)
#define	HNAT_DBG_HASHO		(1<<2)
#define	HNAT_DBG_HASHI		(1<<3)

#define SIP2LSB(v)			((unsigned short)((v << 16) >>16))
#define SIP2MSB(v)			((unsigned short)(v >> 16))
#define LSB2SIP(v, m)		((unsigned int)(v | (m << 16)))

#define	CML_HNAT_PUBLIC_IP_MAX_NUM	16

typedef struct 
{
	/* half-word 0 */
	unsigned short	mipidx:4;	/* global IP address index */
	unsigned short	counter:12;	/* HW will update this counter */
#define ml_next		counter
	/* half-word 1 */
	unsigned short 	mp;		/* global port */
	/* half-word 2 */
	unsigned char 	prot;		/* protocol */
	unsigned char 	hw_resv:1; 
	unsigned char 	full:1;
	unsigned char 	tocpu:1;
	unsigned char 	drop:1;
	unsigned char 	alg:4;
	/* half-word 3 */
	unsigned short 	siplsb;		/* Least significant two bytes of local IP address */
	/* half-word 4,5 */
	unsigned int 	dip;		/* remote IP address */
	/* half-word 6 */
	unsigned short 	sp;		/* local port */
	/* half-word 7 */
	unsigned short 	dp;		/* remote port */
	/* half-word 8 */
	unsigned short 	used:1;		/* indicate the descriptor has used */
	unsigned short	sw_resv1:3;
	unsigned short 	hnat_acc:1;	/* HW accelerate the seesion */
	unsigned short	has_dmac:1;
	unsigned short 	dmac:10;
	/* half-word 9 */
	unsigned short	sw_resv2:5;
	unsigned short	has_smac:1;
	unsigned short 	smac:10;
	/* half-word 10-15 */
	unsigned short 	sipmsb;		/* Most significant two bytes of local IP address */
	unsigned short	sw_resv3:3;
	unsigned short  ihnext:13;
#ifndef CML_HNAT_BUCKET_SIZE_24
	unsigned short 	sw_resv_word[4];
#endif
}natdes;

int cml_hnat_init(void);
void cml_hnat_reset(void);
void cml_hnat_cfgmode(int mode);
int cml_hnat_cfgip(int which, int ip, int is_pppoe, int index);

natdes *cml_hnat_alloc(void);
void cml_hnat_free(natdes *);

int cml_hnat_attach(natdes *);
void cml_hnat_detach(natdes *);

void cml_hnat_learn_mac(char *hwadrp, char dir, char vid);
unsigned int cml_hnat_get_counter(short s);

unsigned long hmu_keygen(unsigned short ip, unsigned short port, unsigned short prot);
short sess_to_idx(natdes *);
natdes *idx_to_sess(short s);
unsigned int idx_to_mip(int mipidx);
int cml_hnat_get_statistic(void);
int cml_hnat_rx_check(char *buf, char *pkt, unsigned int w0, unsigned int w1);
void cml_hnat_manual_cfgmac(natdes *ds, char *mac, int dir);
void cml_hnat_fast_send(char *iphdr, int len, unsigned long key, natdes *ds, int dir);
void cml_hnat_dump(short cmd, void *parm);

extern unsigned short hnat_max_num; 
extern int hnat_mode_g;
#endif /* CML_HNAT_H */
