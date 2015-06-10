
TOPDIR = ../../..
TOP = ..
SRCBASE = ../..

#CFLAGS	+= -I. -I$(TOPDIR)/target/linux/package/openwrt/include/ -I$(TOP)/shared -I$(SRCBASE)/include -Wall -I$(SRCBASE)/

LDFLAGS	+= -L../ezp-nvram -lnvram -lshared -lpthread

vpath %.c $(TOP)/shared
vpath %.o $(SRCBASE)/router/httpd/prebuilt

all: httpd

clean:
	rm -f *.o *~ httpd

install:
	install -D httpd $(INSTALLDIR)/usr/sbin/httpd
	$(STRIP) $(INSTALLDIR)/usr/sbin/httpd
	install -d $(INSTALLDIR)/etc
	#install *.pem $(INSTALLDIR)/etc
	install openssl.cnf $(INSTALLDIR)/etc
	install -m 755 gencert.sh $(INSTALLDIR)/usr/sbin

cert:
	# create the key and certificate request
	openssl req -new -out cert.csr -config openssl.cnf -keyout privkey.pem -days 3650 -newkey rsa:512
	# remove the passphrase from the key:
	openssl rsa -in privkey.pem -out key.pem
	# convert the certificate request into a signed certificate
	openssl x509 -in cert.csr -out cert.pem -req -signkey key.pem 
	rm -f cert.csr privkey.pem
	# Show human-readable format
	openssl x509 -in cert.pem -text -noout

httpd: wf.o sched.o widget.o lang.o common.o cgi.o ej.o httpd.o ezp.o wan.o dhcp.o \
	   log.o upgrade.o fl.o fr.o rt.o bw.o tos.o pptpd.o ipsec.o \
	   wl.o opendns.o ddns.o md5c.o lb.o nat_pass.o upnp.o vnc.o fw.o l2tpd.o \
	   clone.o lan.o ntp.o st.o ad.o stats.o cron.o turbonat.o ftp.o \
       bt.o storage.o iscsi.o smb.o sm.o vlan.o wan_detect.o itunes.o \
       vs.o  wps.o log.o drive.o camera.o music.o snmp.o wizard.o vpn_wizard.o \
       power.o mac_sense.o relay.o file_mode.o als.o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

build_date.o: build_date.c
build_date:
	echo "#define BUILD_DATE \"`date \"+%b %d %Y\"`\"" > build_date.c
	echo "#define BUILD_TIME \"`date \"+%H:%M:%S\"`\"" >> build_date.c


*.o: $(CY_DEPS)
