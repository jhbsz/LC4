LDFLAGS += -L../ezp-nvram -lnvram -lshared


all: power-detect


power-detect: power-detect.o
	$(CC) -o $@ $^

clean:
	rm -f *.o power-detect
