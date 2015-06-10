
all: commain

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

commain: commain.o comlib.o termlib.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o commain
