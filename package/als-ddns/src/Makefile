
all: als-ddns

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

als-ddns: ddns.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o als_ddns $(all)
