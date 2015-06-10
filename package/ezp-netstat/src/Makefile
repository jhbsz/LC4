
all: ezp-netstat

LDFLAGS += -L../ezp-nvram -lnvram -lshared

ezp-netstat: netstat.o misc.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *~ ezp-netstat
