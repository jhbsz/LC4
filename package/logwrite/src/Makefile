
all: logwrite

LDFLAGS += -L../ezp-nvram -lnvram -lshared

logwrite:
	$(CC) $(CFLAGS) $(LDFLAGS) -o logwrite logwrite.c

clean:
	rm -f logwrite
