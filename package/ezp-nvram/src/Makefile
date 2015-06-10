# $Id: Makefile 4726 2006-09-02 18:37:11Z nbd $

# EXTRA_CFLAGS := -c -I. -I../include -g -DNVRAM_MMAP=1
# EXTRA_CFLAGS := -c -I. -I../include -g
EXTRA_CFLAGS := -c -I. -I../include
LIBSHARED_OBJS := shutils.o wl.o wl_linux.o linux_timer.o base64.o strings.o files.o process.o crc32.o semaphore.o
LIBNVRAM_OBJS := nvram_linux.o ezp-lib.o

all: libshared.so libnvram.so nvram

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

libshared.so: $(LIBSHARED_OBJS)
	$(CC) -shared -o $@ $^

libnvram.so: $(LIBNVRAM_OBJS)
	$(CC) -shared -o $@ $^

nvram: main.o
	$(CC) -o $@ $^ -L. -lnvram -lshared

clean:
	rm -f *.o *.so nvram
