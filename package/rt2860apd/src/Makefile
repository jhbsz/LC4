
LDFLAGS += -L../../ezp-nvram -lnvram -lshared

all: encode_ssid b64 site_survey

encode_ssid: encode_ssid.c
	$(CC) -o encode_ssid encode_ssid.c

site_survey: site_survey.c
	$(CC) -o site_survey site_survey.c $(LDFLAGS)

b64: b64.c
	$(CC) -o b64 b64.c

clean:
	rm -f *.o encode_ssid.c site_survey.c
