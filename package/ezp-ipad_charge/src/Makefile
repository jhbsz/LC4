LDFLAGS += -lshared


all: ipad_charge

ipad_charge: ipad_charge.o
	$(CC) -o $@ $^ -lusb-1.0 -L$(STAGING_DIR)/usr/lib

clean:
	rm -f *.o ipad_charge 
