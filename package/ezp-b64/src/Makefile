LDFLAGS += -lshared

all: b64

b64: base64.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o b64
