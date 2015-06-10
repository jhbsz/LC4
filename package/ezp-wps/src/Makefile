LDFLAGS += -L../ezp-nvram -lnvram -lshared


all: ezp-wps


ezp-wps: wps.o
	$(CC) -o $@ $^

clean:
	rm -f *.o ezp-wps
