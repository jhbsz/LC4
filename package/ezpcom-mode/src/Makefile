# $Id: Makefile 4726 2006-09-02 18:37:11Z nbd $
#	$(CC) $(CFLAGS) -o $@ huaweiAktBbo.c 


all: ezpcom-mode

ezpcom-mode: 
	$(CC) $(CFLAGS) -o $@ usb_modeswitch.c -D EZP_SEC_NO_AUTH -D DEBUG
#	$(CC) $(CFLAGS) -o abc usb_modeswitch.orig.c -D EZP_SEC_NO_AUTH

clean:
	rm -f *.o ezpcom-mode

