
all: mac-sense

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

mac-sense: mac_sense.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o mac-sense
