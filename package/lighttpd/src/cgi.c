/* 
   some simple CGI helper routines
   Copyright (C) Andrew Tridgell 1997-2001
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fnmatch.h>
#include <strings.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "cgi.h"
#include "common.h"
#include "memdbg.h"

#define MMAP_FAILED ((void *)-1)

#define CONTENT_DISPOSITION "Content-Disposition:"
#define CONTENT_TYPE "Content-Type:"
#define MULTIPART_FORM_DATA "multipart/form-data"
#define CRLF "\r\n"

int write_one_part(struct cgi_state *cgi, FILE *f, int *len, char *boundary, char *filename, char *name, char *tmp_path, FILE *tmpfile);

const char *get(struct cgi_state *, const char *);

/*******************************************************************
mmap a file
********************************************************************/
void *map_file(const char *fname, size_t *size)
{
        struct stat st;
        void *p;
        int fd;

        fd = open(fname, O_RDONLY, 0);
        if (fd == -1) {
                fprintf(stderr, "Failed to load %s - %s\n", fname, strerror(errno));
                return NULL;
        }
        fstat(fd, &st);
        p = mmap(NULL, st.st_size+1, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FILE, fd, 0);
        close(fd);
        if (p == MMAP_FAILED) {
                fprintf(stderr, "Failed to mmap %s - %s\n", fname, strerror(errno));
                return NULL;
        }
        *size = st.st_size;

        /* make sure its terminated */
        *((char *)p+st.st_size) = 0;
        return p;
}


void unmap_file(void *p, size_t size)
{
        munmap(p, size+1);
}

void *x_malloc(size_t size)
{
        void *ret;
        ret = __ax_malloc(size);
        if (!ret) {
                fprintf(stderr, "Out of memory on size %d\n", (int)size);
                _exit(1);
        }
        return ret;
}


/* 
   trim the tail of a string
*/
void trim_tail(char *s, char *trim_chars)
{
        int len = strlen(s);
        while (len > 0 && strchr(trim_chars, s[len-1])) len--;
        s[len] = 0;
}


/* 
   inplace handling of + and % escapes in http variables 
*/
void unescape(char *p)
{
	unsigned v;

	while (*p) {
//		if (*p == '+') *p = ' ';
		if (*p == '%' && sscanf(p+1, "%02x", &v) == 1) {
			*p = (char)v;
			memcpy(p+1, p+3, strlen(p+3)+1);
		}
		p++;
	}
}

/*
  read one line from a file, allocating space as need be
  adjust length on return
*/
char *grab_line(FILE *f, const char *terminator, int *length)
{
	int len = 1024;
	char *ret = __ax_malloc(len);
	int i = 0;
	int tlen = strlen(terminator);

	while (*length) {
		int c;
	
		if (i == len) {
			len *= 2;
			ret = __ax_realloc(ret, len);
		}
	
		c = fgetc(f);
		(*length)--;

		if (c == EOF) {
			(*length) = 0;
			break;
		}
		
		ret[i++] = c;

		if (memcmp(terminator, &ret[i-tlen], tlen) == 0) {
			i -= tlen;
			break;
		}
	}

	ret[i] = 0;
	return ret;
}


/*
  add a name/value pair to the list of cgi variables 
*/
void put(struct cgi_state *cgi, const char *name, const char *value)
{
	struct cgi_var *var;
	int len;
	char *cgi_name, *p;

	if (!name || !value) return;

	var = __ax_malloc(sizeof(*var));
	memset(var, 0, sizeof(*var));
	var->next = cgi->variables;

	/* trim leading spaces */
	while (*name && (*name == '+' || *name == ' ')) name++;

	var->name = __ax_strdup(name);
	var->value = __ax_strdup(value);

	unescape(var->name);
	unescape(var->value);

	/* trim trailing spaces */
	len = strlen(var->value);
	while (len && isspace(var->value[len-1])) {
		var->value[len-1] = 0;
		len--;
	}

	for (p=var->name; *p; p++) {
		if (!isalnum(*p) && !strchr("_-", *p)) {
			*p = '_';
		}
	}

	cgi->variables = var;
	__ax_asprintf(&cgi_name, "CGI_%s", var->name);
	__ax_free(cgi_name);
}


/*
  parse a url encoded form
*/
void load_urlencoded(struct cgi_state *cgi)
{
	int len = cgi->content_length;
	char *line;
	char *p;
	FILE *f = stdin;

	while (len && (line=grab_line(f, "&", &len))) {
		p = strchr(line,'=');
		if (p) {
			*p = 0;
			put(cgi, line, p+1);
		}
		__ax_free(line);
	}
}

/*
  parse a single element of a multipart encoded form
  It's rather more complex than I would like :(
*/
int load_one_part(struct cgi_state *cgi, FILE *f, int *len, char *boundary)
{
	char *line;
	char *name=NULL;
	char *content;
	char *filename=NULL;
	unsigned content_len=0, content_alloc=1024;
	unsigned boundary_len = strlen(boundary);
	int c;
    int raw_data = 0;
    int towrite = 0;

    const char *chunked_upload = get(cgi, "func");
    const char *upload_root_dir = get(cgi, "upload_root_dir");
    const char *upload_id = get(cgi, "upload_id");
    char tmp_path[2048] = "";
    FILE *file = NULL;
    int is_chunked_upload = 0;
    if (chunked_upload){
        if (!strcmp(chunked_upload, "chunked_upload")) {
            is_chunked_upload = 1;
            // Write file with upload_id : Combine the path to upload_id temp file
            strcpy(tmp_path, "/tmp/mnt");
            strcat(tmp_path, upload_root_dir);
            strcat(tmp_path, "/");
            strcat(tmp_path, ".upload_cache");
            strcat(tmp_path, "/");
            strcat(tmp_path, upload_id);
            // Open file
            file = fopen(tmp_path, "a");
        }
    }
	while (*len && (line=grab_line(f, CRLF, len))) {
		if (*line == 0) break;
		if (strcmp(line,"--") == 0) return 1;
		if (strncasecmp(line, CONTENT_TYPE, 
				strlen(CONTENT_TYPE)) == 0) {
			raw_data = 1;
		}
		if (strncasecmp(line, CONTENT_DISPOSITION, 
				strlen(CONTENT_DISPOSITION)) == 0) {
			char *p = strstr(line,"; name=");
			if (!p) continue;
			p += 7;
			if (*p == '"') p++;
			name = strndup(p, strcspn(p, "\";"));
			p = strstr(line,"; filename=\"");
			if (p) {
				p += 12;
				filename = strndup(p, strcspn(p, "\";"));
			}
		}
	}
    if (raw_data && name && (tmp_path[0] != '\0') && file) {
        // If you want to write file
        towrite = write_one_part(cgi, f, len, boundary, filename, name, tmp_path, file);
    } else {
        // If you don't want to write file 
        content = __ax_malloc(content_alloc);
        while (*len && (c = fgetc(f)) != EOF) {
            (*len)--;
            if (content_len >= (content_alloc-1)) {
                content_alloc *= 2;
                content = __ax_realloc(content, content_alloc);
            }
            content[content_len++] = c;
            /* we keep grabbing content until we hit a boundary */
            if (memcmp(boundary, &content[content_len-boundary_len], 
                        boundary_len) == 0 &&
                    memcmp("--", &content[content_len-boundary_len-2], 2) == 0) {
                content_len -= boundary_len+4;
                if (name) {
                    if (raw_data || filename) {
                        put(cgi, name, filename?filename:"");
                        cgi->variables->content = content;
                        cgi->variables->content_len = content_len;
                    } else {
                        content[content_len] = 0;
                        put(cgi, name, content);
                        __ax_free(name);
                        __ax_free(content);
                    }
                } else {
                    __ax_free(content);
                }
                fgetc(f); fgetc(f);
                (*len) -= 2;
                return 0;
            }
        }
    }

	if (filename) __ax_free(filename);
    if (file) fclose(file);

	return 1;
}

/* 
 * Write your current content to file 
 * 
 */
int write_one_part(struct cgi_state *cgi, FILE *f, int *len, char *boundary, char *filename, char *name, char *tmp_path, FILE *tmpfile)
{
    int ret = 1;
    int towrite = 0;
    unsigned content_len=0;
    char *content, *buf_content;
    unsigned boundary_len = strlen(boundary);
    int c;
    int current_index = 0, write_index = 0, buf_index = 0;
    int bytes = 0;

    content = __ax_malloc(boundary_len + 5); // "boundary" + "--" + "\r\n" + "\0"
    buf_content = __ax_malloc(BLK_SIZE + 1);

    while (*len && (c = fgetc(f)) != EOF) {
        (*len)--;
        /*
         *      reference_boundry[boundary_len + 3];
         *      write_index = 0;
         *      if(write_index < boundary_len) {
         *          reference_boundry[write_index] = c;
         *          write_index ++;
         *      } else {
         *          memcpy(reference_boundry, reference_boundry + 1, boundary_len + 1);
         *          reference_boundry[write_index] = c;
         *      }
         *
         * */
        current_index = content_len++; // increase content length
        if (write_index == (boundary_len + 4)) {
            // if content is full, try to compare boundary
            // If grab boundary ....
            if (memcmp(boundary, &content[4], boundary_len) == 0 &&
                    memcmp("--", &content[2], 2) == 0) {
                content_len -= boundary_len+4;
                if (name) {
                    put(cgi, name, filename?filename:"");
                    cgi->variables->content_len = content_len;
                } else {
                    content[boundary_len + 2] = 0;
                    put(cgi, name, content);
                }
                fgetc(f); fgetc(f);
                (*len) -= 2;
                towrite = 1;
            } else {
                // Cannot grab boundary, just pop out current character to buffer
                buf_content[buf_index] = content[0];
                buf_content[buf_index + 1] = 0;
                memcpy(content, content+1, boundary_len + 3);
                content[write_index - 1] = c;
                buf_index++;
            }
        } else {
            // if content is not full
            content[write_index] = c;
            write_index++;
        }

        if ( ((buf_index == BLK_SIZE) || feof(f) || (towrite)) && (tmpfile != NULL))
        {
            // Write file
            if (buf_index && towrite){
                bytes = fwrite(buf_content, 1, buf_index, tmpfile);
            }
            buf_index = 0;
            __ax_free(buf_content);
            if(towrite) {
                return ret;
            } 
            buf_content = __ax_malloc(BLK_SIZE + 1);
        }
    }

    if (buf_content)  __ax_free(buf_content); 

    return ret;
}

/*
  parse a multipart encoded form (for file upload)
  see rfc1867
*/
void load_multipart(struct cgi_state *cgi)
{
	char *boundary;
	FILE *f = stdin;
	int len = cgi->content_length;
	char *line;

	if (!cgi->content_type) return;
	boundary = strstr(cgi->content_type, "boundary=");
	if (!boundary) return;
	boundary += 9;
	trim_tail(boundary, CRLF);
	line = grab_line(f, CRLF, &len);
	if (strncmp(line,"--", 2) != 0 || 
	    strncmp(line+2,boundary,strlen(boundary)) != 0) {
		fprintf(stderr,"Malformed multipart?\n");
		__ax_free(line);
		return;
	}

	if (strcmp(line+2+strlen(boundary), "--") == 0) {
		/* the end is only the beginning ... */
		__ax_free(line);
		return;
	}

	__ax_free(line);
    while (load_one_part(cgi, f, &len, boundary) == 0) ;
}

/*
  load all the variables passed to the CGI program. May have multiple variables
  with the same name and the same or different values. 
*/
void load_variables(struct cgi_state *cgi)
{
	char *p, *s, *tok;

    if ((s=cgi->query_string)) {
        char *pp;
        for (tok=strtok_r(s,"&;", &pp);tok;tok=strtok_r(NULL,"&;", &pp)) {
            p = strchr(tok,'=');
            if (p) {
                *p = 0;
                put(cgi, tok, p+1);
            }
        }
    }

	if (cgi->content_length > 0 && cgi->request_post) {
		if (strncmp(cgi->content_type, MULTIPART_FORM_DATA, 
			    strlen(MULTIPART_FORM_DATA)) == 0) {
			load_multipart(cgi);
		} else {
			load_urlencoded(cgi);
		}
	}

}


/*
  find a variable passed via CGI
  Doesn't quite do what you think in the case of POST text variables, because
  if they exist they might have a value of "" or even " ", depending on the 
  browser. Also doesn't allow for variables[] containing multiple variables
  with the same name and the same or different values.
*/
const char *get(struct cgi_state *cgi, const char *name)
{
	struct cgi_var *var;
	for (var = cgi->variables; var; var = var->next) {
        if (strcmp(var->name, name) == 0) {
			return var->value;
		}
	}
	return NULL;
}

/*
   return the content of a binary cgi variable (for file upload)
*/
const char *get_content(struct cgi_state *cgi, const char *name, unsigned *size)
{
	struct cgi_var *var;

	for (var = cgi->variables; var; var = var->next) {
		if (strcmp(var->name, name) == 0) {
			*size = var->content_len;
			return var->content;
		}
	}
	return NULL;
}

/*
  tell a browser about a fatal error in the http processing
*/
void http_error(struct cgi_state *cgi, 
		       const char *err, const char *header, const char *info)
{
	if (!cgi->got_request) {
		/* damn browsers don't like getting cut off before they give a request */
		char line[1024];
		while (fgets(line, sizeof(line)-1, stdin)) {
			if (strncasecmp(line,"GET ", 4)==0 || 
			    strncasecmp(line,"POST ", 5)==0 ||
			    strncasecmp(line,"PUT ", 4)==0) {
				break;
			}
		}
	}

	printf("HTTP/1.0 %s\r\n%sConnection: close\r\nContent-Type: text/html\r\n\r\n<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY><H1>%s</H1>%s<p></BODY></HTML>\r\n\r\n", err, header, err, err, info);
}

/*
  send a http header based on file extension
*/
enum MIME_TYPE http_header(struct cgi_state *cgi, const char *filename)
{
	int i;
	static struct {
		char *pattern;
		char *mime_type;
		enum MIME_TYPE type;
	} mime_types[] = {
		{"*.gif",  "image/gif",  MIME_TYPE_IMAGE_GIF},
		{"*.jpg",  "image/jpeg", MIME_TYPE_IMAGE_JPEG},
		{"*.txt",  "text/plain", MIME_TYPE_TEXT_PLAIN},
		{"*.html", "text/html",  MIME_TYPE_TEXT_HTML},
		{NULL,     "data",       MIME_TYPE_UNKNOWN},
	};

	printf("HTTP/1.0 200 OK\r\nConnection: close\r\n");

	for (i=0; mime_types[i].pattern; i++) {
		if (fnmatch(mime_types[i].pattern, filename, 0) == 0) break;
	}
	printf("Content-Type: %s\r\n\r\n", mime_types[i].mime_type);
	return mime_types[i].type;
}


/* we're running under a web server as cgi */
int setup_cgi(struct cgi_state *cgi)
{
	char *p;

	if ((p = getenv("CONTENT_LENGTH"))) {
		cgi->content_length = atoi(p);
	}
	if ((p = getenv("REQUEST_METHOD"))) {
		cgi->got_request = 1;
		if (strcasecmp(p, "POST") == 0) {
			cgi->request_post = 1;
		}
	}
	if ((p = getenv("QUERY_STRING"))) {
		cgi->query_string = __ax_strdup(p);
	}
	if ((p = getenv("SCRIPT_NAME"))) {
		cgi->url = __ax_strdup(p);
		cgi->pathinfo = cgi->url;
	}
	if ((p = getenv("CONTENT_TYPE"))) {
		cgi->content_type = __ax_strdup(p);
	}
	return 0;
}

void add_header(struct cgi_state *cgi, const char *line)
{
	if (cgi->num_headers == MAX_HEADERS) return;
	cgi->extra_headers[cgi->num_headers++] = __ax_strdup(line);
}

/* we are a mini-web server. We need to read the request from stdin */
int setup_standalone(struct cgi_state *cgi)
{
	char line[1024];
	char *url=NULL;
	char *p;

	while (fgets(line, sizeof(line)-1, stdin)) {
		trim_tail(line, CRLF);
		if (line[0] == 0) break;
		if (strncasecmp(line,"GET ", 4)==0) {
			cgi->got_request = 1;
			url = __ax_strdup(&line[4]);
		} else if (strncasecmp(line,"POST ", 5)==0) {
			cgi->got_request = 1;
			cgi->request_post = 1;
			url = __ax_strdup(&line[5]);
		} else if (strncasecmp(line,"PUT ", 4)==0) {
			cgi->got_request = 1;
			cgi->http_error(cgi, "400 Bad Request", "",
					"This server does not accept PUT requests");
			return -1;
		} else if (strncasecmp(line,"Content-Length: ", 16)==0) {
			add_header(cgi, line);
			cgi->content_length = atoi(&line[16]);
		} else if (strncasecmp(line,"Content-Type: ", 14)==0) {
			add_header(cgi, line);
			cgi->content_type = __ax_strdup(&line[14]);
		} else {
			add_header(cgi, line);
		}
		/* ignore all other requests! */
	}

	if (!url) {
		cgi->http_error(cgi, "400 Bad Request", "",
				"You must specify a GET or POST request");
		exit(1);
	}

	/* trim the URL */
	if ((p = strchr(url,' ')) || (p=strchr(url,'\t'))) {
		*p = 0;
	}

	/* anything following a ? in the URL is part of the query string */
	if ((p=strchr(url,'?'))) {
		cgi->query_string = p+1;
		*p = 0;
	}

	cgi->url = url;
	cgi->pathinfo = url;
	return 0;
}

/*
  read and parse the http request
 */
int setup(struct cgi_state *cgi)
{
	int ret;

	if (getenv("GATEWAY_INTERFACE")) {
		ret = setup_cgi(cgi);
	} else {
		ret = setup_standalone(cgi);
	}

	while (*cgi->pathinfo && *cgi->pathinfo == '/') cgi->pathinfo++;

	return ret;
}

/*
  destroy a open cgi state
*/
void destroy(struct cgi_state *cgi)
{
	while (cgi->variables) {
		struct cgi_var *var = cgi->variables;
		__ax_free(var->name);
		__ax_free(var->value);
		cgi->variables = cgi->variables->next;
		__ax_free(var);
	}
    DBG("free cgi->url:%p", cgi->url);
	if(cgi->url)
        __ax_free(cgi->url);
    DBG("free cgi->query_string:%p", cgi->query_string);
	if(cgi->query_string)
        __ax_free(cgi->query_string);
    DBG("free cgi->content_type:%p", cgi->content_type);
	if(cgi->content_type)
        __ax_free(cgi->content_type);
	memset(cgi, 0, sizeof(cgi));
	__ax_free(cgi);
}

struct cgi_state cgi_base = {
	/* methods */
	setup,
	destroy,
	http_header,
	load_variables,
	get,
	get_content,
	http_error,
	
	/* rest are zero */
};

struct cgi_state *cgi_init(void)
{
	struct cgi_state *cgi;

	cgi = __ax_malloc(sizeof(*cgi));
	memcpy(cgi, &cgi_base, sizeof(*cgi));

	return cgi;
}
