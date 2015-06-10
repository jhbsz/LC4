
/*
 * Codes at here are heavily taken from upload.cgi.c which is for large file uploading , but 
 * in fact "upload_settings" only need few memory(~16k) so it is not necessary to follow 
 * upload.cgi.c at all.
 * 
 * YYHuang@Ralink TODO: code size.
 *  
 */

#include <unistd.h>	//for unlink
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define RFC_ERROR "RFC1867 ...."

void *memmem(const void *buf, size_t buf_len, const void *byte_line, size_t byte_line_len)
{
    unsigned char *bl = (unsigned char *)byte_line;
    unsigned char *bf = (unsigned char *)buf;
    unsigned char *p  = bf;

    while (byte_line_len <= (buf_len - (p - bf))){
        unsigned int b = *bl & 0xff;
        if ((p = (unsigned char *) memchr(p, b, buf_len - (p - bf))) != NULL){
            if ( (memcmp(p, byte_line, byte_line_len)) == 0)
                return p;
            else
                p++;
        }else{
            break;
        }
    }
    return NULL;
}

#define MEM_SIZE	1024
#define MEM_HALT	512
int findStrInFile(char *filename, int offset, unsigned char *str, int str_len)
{
	int pos = 0, rc;
	FILE *fp;
	unsigned char mem[MEM_SIZE];

	if(str_len > MEM_HALT)
		return -1;
	if(offset <0)
		return -1;

	fp = fopen(filename, "rb");
	if(!fp)
		return -1;

	rewind(fp);
	fseek(fp, offset + pos, SEEK_SET);
	rc = fread(mem, 1, MEM_SIZE, fp);
	while(rc){
		unsigned char *mem_offset;
		mem_offset = (unsigned char*)memmem(mem, rc, str, str_len);
		if(mem_offset){
			fclose(fp);	//found it
			return (mem_offset - mem) + pos + offset;
		}

		if(rc == MEM_SIZE){
			pos += MEM_HALT;	// 8
		}else
			break;
		
		rewind(fp);
		fseek(fp, offset+pos, SEEK_SET);
		rc = fread(mem, 1, MEM_SIZE, fp);
	}

	fclose(fp);
	return -1;
}

/*
 *  ps. callee must free memory...
 */
void *getMemInFile(char *filename, int offset, int len)
{
    void *result;
    FILE *fp;
    if( (fp = fopen(filename, "r")) == NULL ){
        return NULL;
    }
	fseek(fp, offset, SEEK_SET);
    result = malloc(sizeof(unsigned char) * len );
	if(!result)
		return NULL;
    if( fread(result, 1, len, fp) != len){
        free(result);
        return NULL;
    }
    return result;
}

int import(char *filename, int offset, int len)
//void import(char *filename, int offset, int len)
{
    char data;
    FILE *fp, *src;
    char buf[4096];//aron add
    char *p; //aron add
    char q[2]; //aron add
    FILE *fprod; //aron add
    
    
    if(!( fp = fopen("/var/uploadCfg", "w+"))){
      return 1;
    }

    if(!( src = fopen(filename, "r"))){
      fclose(fp);
      return 1;
    }

    if( fseek(src, offset, SEEK_SET) == -1){
        printf("fseek error\n");
    }

    while( len > 0){
        if(! fread(&data, 1, 1, src))
            break;
        fwrite(&data, 1, 1, fp);
        len--;
    }

    fclose(fp);    
    fclose(src);

    if((fp=fopen("/var/uploadCfg","r"))==NULL)
        return 1;
    else
    {
        fgets(buf, sizeof(buf), fp);
        fclose(fp);
        if ((p = strstr(buf, "EZP_LOG")) == NULL) {
            system("rm -rf /var/uploadCfg");
            return 1;
        }
        if ((p = strstr(p + strlen("EZP_LOG"), "EZP_")) == NULL) {
            system("rm -rf /var/uploadCfg");
            return 1;
        }
        p += strlen("EZP_");
    
        /* prod_cat */
        system("nvram get prod_cat > /var/prod_cat");
        if((fprod=fopen("/var/prod_cat","r"))==NULL)
	    return 1;
        else
	    fgets(q,2,fprod);
        fclose(fprod);
        system("rm -rf /var/prod_cat");
        if (p[0] != q[0]) {
            system("rm -rf /var/uploadCfg");   
            return 1;
        }
    
        /* prod_subcat */
        system("nvram get prod_subcat > /var/prod_subcat");
        if((fprod=fopen("/var/prod_subcat","r"))==NULL)
	    return 1;
        else
	    fgets(q,2,fprod);
        fclose(fprod);
        system("rm -rf /var/prod_subcat");
        if (p[1] != q[0]) {
            system("rm -rf /var/uploadCfg");
            return 1;
        }
    }
    
    system("nvram import /var/uploadCfg");
    system("nvram commit");
    system("rm -rf /var/uploadCfg");
    return 0;
}


int main (int argc, char *argv[])
{
    int file_begin, file_end;
    int line_begin, line_end;
    char *boundary; int boundary_len;
    char *filename = getenv("UPLOAD_FILENAME");
    //char temp1[32]; //aron add
    //char temp2[32]; //aron add
    int file_size, result; //aron add

    printf(
"\
Server: %s\n\
Pragma: no-cache\n\
Content-type: text/html\n",
getenv("SERVER_SOFTWARE"));

    printf("\n\
<html>\n\
<head>\n\
<TITLE>Import Settings</TITLE>\n\
<link rel=stylesheet href=/style/normal_ws.css type=text/css>\n\
<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">\n\
</head>\n\
<body> ");


    if(!filename){
        printf("failed, can't get env var.\n");
        return -1;
    }

    line_begin = 0;
    if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
        //printf("%s", RFC_ERROR);
        //return -1;
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
        
    }
	boundary_len = line_end - line_begin;
    boundary = getMemInFile(filename, line_begin, boundary_len);

    // sth like this..
    // Content-Disposition: form-data; name="filename"; filename="\\192.168.3.171\tftpboot\a.out"
    //
    char *line, *semicolon, *user_filename;
    line_begin = line_end + 2;
    if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }
    line = getMemInFile(filename, line_begin, line_end - line_begin);
    if(strncasecmp(line, "content-disposition: form-data;", strlen("content-disposition: form-data;"))){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }
    semicolon = line + strlen("content-disposition: form-data;") + 1;
    if(! (semicolon = strchr(semicolon, ';'))  ){
        printf("dont support multi-field upload.\n");
        goto err;
    }
    user_filename = semicolon + 2;
    if( strncasecmp(user_filename, "filename=", strlen("filename="))  ){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }
    user_filename += strlen("filename=");
    //until now we dont care about what  true filename is.
    free(line);

    // We may check a string  "Content-Type: application/octet-stream" here
    // but if our firmware extension name is the same with other known name, 
    // the browser will define other content-type itself instead,
    line_begin = line_end + 2;
    if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }

    line_begin = line_end + 2;
    if((line_end = findStrInFile(filename, line_begin, "\r\n", 2)) == -1){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }

    file_begin = line_end + 2;

    if( (file_end = findStrInFile(filename, file_begin, boundary, boundary_len)) == -1){
        //printf("%s", RFC_ERROR);
        printf("<script>parent.upload_message='Not a valid configuration.';</script>");
        goto err;
    }
    file_end -= 2;		// back 2 chars.(\r\n);

    
    file_size = file_end - file_begin;
    //sprintf(temp1, "%d", file_size); 
    //sprintf(temp2, "echo %s > /var/size", temp1);
    //system(temp2);
    if(file_size>1000000) //if upload fize is more than 1M, it may be a firmware image and we reject it.
    {
    	printf("<script>parent.upload_message='Not a valid configuration.';</script>");
    	goto err;
    }
    
    result = import(filename, file_begin, file_end - file_begin);
    if(result)
    {
       printf("<script>parent.upload_message='Not a valid configuration.';</script>");
       goto err;
    }
    else
    {
      system("sleep 3 && reboot &");
      printf("</body> <script>function waitingpage(){top.location.href = '/local/advance/proceeding.asp';}waitingpage();</script></html>");
    }
err:
    printf("<script>parent.reload_configuration_backup_page();</script>");
    free(boundary);
    exit(0);
}

