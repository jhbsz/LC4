#include "upload.cgi.h"

#define REFRESH_TIMEOUT		"40000"		/* 40000 = 40 secs*/

/*
 *  Uboot image header format
 *  (ripped from mkimage.c/image.h)
 */
#define IH_MAGIC	0x27051956
#define IH_NMLEN	32
typedef struct image_header {
    uint32_t    ih_magic;   /* Image Header Magic Number    */
    uint32_t    ih_hcrc;    /* Image Header CRC Checksum    */
    uint32_t    ih_time;    /* Image Creation Timestamp */
    uint32_t    ih_size;    /* Image Data Size      */
    uint32_t    ih_load;    /* Data  Load  Address      */
    uint32_t    ih_ep;      /* Entry Point Address      */
    uint32_t    ih_dcrc;    /* Image Data CRC Checksum  */
    uint8_t     ih_os;      /* Operating System     */
    uint8_t     ih_arch;    /* CPU architecture     */
    uint8_t     ih_type;    /* Image Type           */
    uint8_t     ih_comp;    /* Compression Type     */
    uint8_t     ih_name[IH_NMLEN];  /* Image Name       */
} image_header_t;

/*
 *  taken from "mkimage -l" with few modified....
 */
int check(char *imagefile, int offset, int len)
{
	struct stat sbuf;

	int  data_len;
	char *data;
	unsigned char *ptr;
	unsigned long checksum;

	image_header_t header;
	image_header_t *hdr = &header;

	int ifd;

	if ((unsigned)len < sizeof(image_header_t)) {
		fprintf (stderr, "Bad size: \"%s\" is no valid image\n", imagefile);
		return 0;
	}

	ifd = open(imagefile, O_RDONLY);
	if(!ifd){
		fprintf (stderr, "Can't open %s: %s\n", imagefile, strerror(errno));
		return 0;
	}

	if (fstat(ifd, &sbuf) < 0) {
		close(ifd);
		fprintf (stderr, "Can't stat %s: %s\n", imagefile, strerror(errno));
		return 0;
	}

	ptr = (unsigned char *) mmap(0, sbuf.st_size, PROT_READ, MAP_SHARED, ifd, 0);
	if ((caddr_t)ptr == (caddr_t)-1) {
		close(ifd);
		fprintf (stderr, "Can't mmap %s: %s\n", imagefile, strerror(errno));
		return 0;
    }
	ptr += offset;

	/*
	 *  handle Header CRC32
	 */
    memcpy (hdr, ptr, sizeof(image_header_t));

    if (ntohl(hdr->ih_magic) != IH_MAGIC) {
		munmap(ptr, len);
		close(ifd);
		fprintf (stderr, "Bad Magic Number: \"%s\" is no valid image\n", imagefile);
		return 0;
	}

	data = (char *)hdr;

    checksum = ntohl(hdr->ih_hcrc);
    hdr->ih_hcrc = htonl(0);	/* clear for re-calculation */

    if (crc32 (0, data, sizeof(image_header_t)) != checksum) {
		munmap(ptr, len);
		close(ifd);
        fprintf (stderr, "*** Warning: \"%s\" has bad header checksum!\n", imagefile);
		return 0;
    }

	/*
	 *  handle Data CRC32
	 */
    data = (char *)(ptr + sizeof(image_header_t));
    data_len  = len - sizeof(image_header_t) ;

    if (crc32 (0, data, data_len) != ntohl(hdr->ih_dcrc)) {
		munmap(ptr, len);
		close(ifd);
        fprintf (stderr, "*** Warning: \"%s\" has corrupted data!\n", imagefile);
		return 0;
    }

	munmap(ptr, len);
	close(ifd);

	return 1;
}

/*
 * I'm too lazy to use popen() instead of system()....
 * ( note:  static buffer used)
 */
#define DEFAULT_LAN_IP "10.10.10.254"
char *getLanIP(void)
{
	static char buf[64];
	char *nl;
	FILE *fp;

	memset(buf, 0, sizeof(buf));
	//if( (fp = popen("nvram_get 2860 lan_ipaddr", "r")) == NULL )
	if( (fp = popen("nvram get lan0_ipaddr", "r")) == NULL )
		goto error;

	if(!fgets(buf, sizeof(buf), fp)){
		pclose(fp);
		goto error;
	}

	if(!strlen(buf)){
		pclose(fp);
		goto error;
	}
	pclose(fp);

	if(nl = strchr(buf, '\n'))
		*nl = '\0';

	return buf;

error:
	fprintf(stderr, "warning, cant find lan ip\n");
	return DEFAULT_LAN_IP;
}

void javascriptUpdate(int success)
{
	printf("<script language=\"JavaScript\" type=\"text/javascript\">");
	if(success){
		printf(" \
function refresh_all(){	\
  top.location.href = \"http://%s\"; \
} \
function update(){ \
  self.setTimeout(\"refresh_all()\", %s);\
}", getLanIP(), REFRESH_TIMEOUT);
	}else{
		printf("function update(){ parent.menu.setUnderFirmwareUpload(0);}");
	}
	printf("</script>");
}

inline void webFoot(void)
{
	printf("</body></html>\n");
}

void websDecodeUrl(char *decoded, char *token, int len)
{
    char  *ip,  *op;
    int     num, i, c;

    op = decoded;
    for (ip = token; *ip && len > 0; ip++, op++) {
        if (*ip == '+') {
            *op = ' ';
        } else if (*ip == '%' && isxdigit(ip[1]) && isxdigit(ip[2])) {
            ip++;
            for (i = 0, num = 0; i < 2; i++, ip++) {
                c = tolower(*ip);
                if (c >= 'a' && c <= 'f') {
                    num = (num * 16) + 10 + c - 'a';
                } else {
                    num = (num * 16) + c - '0';
                }
            }
            *op = (char) num;
            ip--;

        } else {
            *op = *ip;
        }
        len--;
    }
    *op = '\0';
}

int main (int argc, char *argv[])
{
	struct stat stat_buf;
	char *filename;
	char *and, *equal;
	char *query_string_raw = getenv("QUERY_STRING");
	char query_string[1024];

	if(!query_string_raw)
		goto err;

	websDecodeUrl(query_string, query_string_raw, sizeof(query_string));

	and = strchr(query_string, '&');
	equal = strchr(query_string, '=');
	if(!equal){
		fprintf(stderr, "html format error\n");
		goto err;
	}
	if(and)
		*and = '\0';
	filename = equal + 1;

	if(stat(filename, &stat_buf) == -1){
		perror("stat");
		goto err;
	}

	printf(
"\
Server: %s\n\
Pragma: no-cache\n\
Cache-Control: no-cache\n\
Expires: Fri, 30 Oct 1998 14:19:41 GMT\n\
Content-type: text/html\n",
getenv("SERVER_SOFTWARE"));

	printf("\n\
<html>\n\
<head>\n\
<TITLE>Upload Firmware</TITLE>\n\
<link rel=stylesheet href=/style/normal_ws.css type=text/css>\n\
<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">\n\
<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">\n\
<META HTTP-EQUIV=\"Expires\" CONTENT=\"-1\">\n\
</head>\n\
<body onload=\"update()\"> <h1> Upload Firmware</h1>");

	// examination
	if(!check(filename, 0, stat_buf.st_size) ){
		printf("Not a valid firmware.");
		javascriptUpdate(0);
		goto err;
	}

	// flash write
	mtd_write_firmware(filename, 0, stat_buf.st_size);

	printf("Done...rebooting");
	system("sleep 3 && reboot &");
	javascriptUpdate(1);
err:
	webFoot();
	exit(0);
}

