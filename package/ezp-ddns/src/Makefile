LDFLAGS +=  -L../ezp-nvram -lnvram -lshared
all: ezp-ddns
ezp-ddns:md5c.o nas_hash.o ddns.o
	$(CC) -o $@ $^ $(LDFLAGS)

md5c.o:
	$(CC) md5c.c -c
nas_hash.o:
	$(CC) nas_hash.c -c
ddns.o:
	$(CC) $(CFLAGS) ddns.c -c 
clean:
	rm -f *.o ezp-ddns
