
# CROSS_COMPILE=/opt/toolchain_mips_nofpu/bin/mips-linux-uclibc-
# KEY: default key
ifndef KEY
KEY=EZPackethahahehe
endif

ifndef KEY
$(error KEY not defined)
endif

KEYVAL=$(strip $(KEY))

LIBCRYPTPATH=./libtomcrypt-1.00
CFLAGS := -I$(LIBCRYPTPATH)/src/headers
LIBTOMCRYPT :=$(LIBCRYPTPATH)/libtomcrypt.a
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
HOSTCC=gcc

all: encrypt ezpkey.h  $(LIBTOMCRYPT) ezpup

clean:
	rm -f *.o ezpup ezpkey.h encrypt tmp.out
	make -C $(LIBCRYPTPATH) clean

ezpkey.h:
	/bin/echo -en '\n /*$(KEYVAL)*/\n' > $@
	/bin/echo -en '\n#define EZPUPGKEY "' >> $@
	/bin/echo -n "$(KEYVAL)" | md5sum | cut -d' ' -f1 | sed -n -e 's/\(..\)/\\x\1/g' -e 's/$$/"/p' >> $@

ezpup.o: ezpkey.h

ezpup: ezpup.o ezpkey.h $(LIBTOMCRYPT)
	$(CC) -s $(LDFLAGS) -o $@ $< $(LIBTOMCRYPT)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(LIBTOMCRYPT):
	sed -i -e 's/\<errno\>/my_errno/' $(LIBCRYPTPATH)/demos/encrypt.c
	make -C $(LIBCRYPTPATH) "CC=$(CC)" "LD=$(LD)" "AR=$(AR)"

encrypt:
	make -C $(LIBCRYPTPATH) crypt
	ln -sf $(LIBCRYPTPATH)/encrypt .

showkey:
	@echo "key='$(KEYVAL)'"

pack:
	@[ -n "$(FIRMWARE)" ] || (echo "FIRMWARE not assigned"; false)
	@[ -n "$(KEYVAL)" ] || (echo "KEYVAL not assigned"; false)
	./ezpenc.sh "$(KEYVAL)" "$(FIRMWARE)" "$(FIRMWARE).rom"

