LDFLAGS += -L../ezp-nvram -lnvram -lshared


all: ezp-eth-led 


power-detect: ezp-eth-led.o
	$(CC) -o $@ $^

clean:
	rm -f *.o ezp-eth-led 
