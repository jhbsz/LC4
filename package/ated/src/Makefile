EXEC = ated
SRCS = ated.c

LDFLAGS += -L$(STAGING_DIR)/usr/lib -lnvram  -lshared

#EXTRA_CFLAGS += -DDBG

all: $(EXEC)

$(EXEC): $(SRCS) 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ $@.c

clean:
	-rm -f $(EXEC) *.o

