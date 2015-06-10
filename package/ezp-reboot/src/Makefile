all: ezp-reboot

ezp-reboot: ezp-reboot.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o ezp-reboot
