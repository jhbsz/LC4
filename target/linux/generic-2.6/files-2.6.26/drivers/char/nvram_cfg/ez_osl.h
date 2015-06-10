#ifndef _ez_osl_h_
#define _ez_osl_h_

#include "typedefs.h"
#include "linuxver.h"


#define	MALLOC(osh, size)	osl_malloc((osh), (size))
#define	MFREE(osh, addr, size)	osl_mfree((osh), (addr), (size))
#define MALLOCED(osh)		osl_malloced((osh))
#define	MALLOC_FAILED(osh)	osl_malloc_failed((osh))

/* bcopy, bcmp, and bzero */
#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))
#define printf(fmt, args...)    printk(fmt, ## args)

#define ASSERT(exp)             do {} while (0)



extern void *osl_malloc(void *osh, uint size);
extern void osl_mfree(void *osh, void *addr, uint size);
extern uint osl_malloced(void *osh);
extern uint osl_malloc_failed(void *osh);
extern void *osl_attach(void *pdev);
extern void osl_detach(void *osh);

#endif /* #ifndef _ez_osl_h_ */

