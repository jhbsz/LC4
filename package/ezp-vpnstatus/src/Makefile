LDFLAGS +=  -L../ezp-nvram -lnvram -lshared


all: vpnstatus

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

vpnstatus: vpnstatus.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o cli 
