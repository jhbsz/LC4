CFLAGS += -O2 -D_FILE_OFFSET_BITS=64 -I./libexfat -DFUSE_USE_VERSION=26
LIBS=-lfuse -lpthread

all: mount.exfat

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

mount.exfat: libexfat/cluster.o libexfat/io.o libexfat/log.o libexfat/lookup.o libexfat/mount.o libexfat/node.o libexfat/utf.o libexfat/utils.o fuse/main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
