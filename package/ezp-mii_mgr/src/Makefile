
all: mii_mgr

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

mii_mgr: mii_mgr.c
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o mii_mgr
