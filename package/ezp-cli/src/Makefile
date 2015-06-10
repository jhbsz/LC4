LDFLAGS +=  -L../ezp-nvram -lnvram -lshared


all: cli

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

stats: cli.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o cli 
