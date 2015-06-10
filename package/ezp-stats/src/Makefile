# EXTRA_CFLAGS := -c -I. -I../include -g -DNVRAM_MMAP=1
# EXTRA_CFLAGS := -c -I. -I../include -g
#
LDFLAGS += -L../ezp-nvram -lnvram -lshared


all: stats

#%.o: %.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

stats: stats.o
	$(CC) -o $@ $^ -lpthread $(LDFLAGS)

clean:
	rm -f *.o stats
