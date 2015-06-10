
all: usb_read_write_sleep

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

usb_read_write_sleep: usb_read_write_sleep.c
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

clean:
	rm -f *.o commain
