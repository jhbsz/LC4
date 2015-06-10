#ifndef _HAIRTUNES_H_
#define _HAIRTUNES_H_
int hairtunes_init(char *pAeskey, char *pAesiv, char *fmtpstr, char *mimetypestr, int pCtrlPort, int pTimingPort,
         int pDataPort, char *pRtpHost, char*pPipeName, char *pLibaoDriver, char *pLibaoDeviceName, char *pLibaoDeviceId,
         int bufStartFill);

#define TIMES   1
// default buffer size
// needs to be a power of 2 because of the way BUFIDX(seqno) works
#define BUFFER_FRAMES  (1024*TIMES)
// and how full it needs to be to begin (must be <BUFFER_FRAMES)
#define START_FILL    (250*TIMES)
#endif 
