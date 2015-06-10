#all: authLogin.cgi utilRequest.cgi sysinfoReq.cgi stdenv.cgi sendfile.cgi
all: authLogin.cgi utilRequest.cgi sysinfoReq.cgi stdenv.cgi fcgi_http.exe

LDFLAGS += -L../ezp-nvram -lnvram

sysinfoReq.cgi: sysinfoReq.c nas_cgiutil.o common.o cgi.o
	$(CC) $(CFLAGS) $(LDFLAGS) cgi.o common.o nas_cgiutil.o sysinfoReq.c -o sysinfoReq.cgi
authLogin.cgi: authLogin.c nas_cgiutil.o common.o cgi.o
	$(CC) $(CFLAGS) $(LDFLAGS) cgi.o common.o nas_cgiutil.o authLogin.c -o authLogin.cgi
utilRequest.cgi: utilRequest.c qbox_op.o common.o nas_cgiutil.o cgi.o upload.o download.o file_op.o cgi_search.o
	$(CC) $(CFLAGS) $(LDFLAGS) nas_cgiutil.o common.o file_op.o cgi_search.o upload.o download.o qbox_op.o cgi.o utilRequest.c -o utilRequest.cgi
nas_cgiutil.o: nas_cgiutil.c nas_cgiutil.h cgi.o
	$(CC) $(CFLAGS) -c nas_cgiutil.c -o nas_cgiutil.o	
common.o: common.c common.h tag.h
	$(CC) $(CFLAGS) -c common.c -o common.o 
cgi.o: cgi.c cgi.h 
	$(CC) $(CFLAGS) -c cgi.c -o cgi.o
qbox_op.o: qbox_op.c qbox_op.h common.o
	$(CC) $(CFLAGS) -c qbox_op.c -o qbox_op.o
upload.o: upload.c upload.h common.o
	$(CC) $(CFLAGS) -c upload.c -o upload.o
download.o: download.c download.h common.o
	$(CC) $(CFLAGS) -c download.c -o download.o
file_op.o: file_op.c file_op.h common.o
	$(CC) $(CFLAGS) -c file_op.c -o file_op.o
cgi_search.o: cgi_search.c cgi_search.h common.o
	$(CC) $(CFLAGS) -c cgi_search.c -o cgi_search.o
#tinyfcgi: tinyfcgi.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi tinyfcgi.c nas_cgiutil.o common.o file_op.o search.o upload.o download.o qbox_op.o cgi.o -o tinyfcgi
#sendfile.cgi: sendfile.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi fcgi_cgi.o fcgi_common.o fcgi_authLogin.o fcgi_nas_cgiutil.o fcgi_download.o fcgi_file_op.o fcgi_utilRequest.o fcgi_upload.o fcgi_search.o fcgi_qbox_op.o fcgi_sysinfoReq.o sendfile.c -o sendfile.cgi
#printenv.cgi: printenv.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi printenv.c nas_cgiutil.o common.o file_op.o search.o upload.o download.o qbox_op.o cgi.o -o printenv.cgi
#qfile_service.cgi: qfile_service.c
#	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi qfile_service.c nas_cgiutil.o common.o file_op.o search.o upload.o download.o qbox_op.o cgi.o -o qfile_service.cgi
fcgi_authLogin.o: fcgi_authLogin.c
	$(CC) $(CFLAGS) -c fcgi_authLogin.c -o fcgi_authLogin.o
fcgi_cgi.o: fcgi_cgi.c
	$(CC) $(CFLAGS) -c fcgi_cgi.c -o fcgi_cgi.o
fcgi_common.o: fcgi_common.c
	$(CC) $(CFLAGS) -c fcgi_common.c -o fcgi_common.o
fcgi_download.o: fcgi_download.c
	$(CC) $(CFLAGS) -c fcgi_download.c -o fcgi_download.o
fcgi_file_op.o: fcgi_file_op.c
	$(CC) $(CFLAGS) -c fcgi_file_op.c -o fcgi_file_op.o
fcgi_nas_cgiutil.o: fcgi_nas_cgiutil.c
	$(CC) $(CFLAGS) -c fcgi_nas_cgiutil.c -o fcgi_nas_cgiutil.o
fcgi_qbox_op.o: fcgi_qbox_op.c
	$(CC) $(CFLAGS) -c fcgi_qbox_op.c -o fcgi_qbox_op.o
fcgi_search.o: fcgi_search.c
	$(CC) $(CFLAGS) -c fcgi_search.c -o fcgi_search.o
fcgi_sysinfoReq.o: fcgi_sysinfoReq.c
	$(CC) $(CFLAGS) -c fcgi_sysinfoReq.c -o fcgi_sysinfoReq.o
fcgi_upload.o: fcgi_upload.c
	$(CC) $(CFLAGS) -c fcgi_upload.c -o fcgi_upload.o
fcgi_sysRequest.o: fcgi_sysRequest.c
	$(CC) $(CFLAGS) -c fcgi_sysRequest.c -o fcgi_sysRequest.o
fcgi_qsyncPrepare.o: fcgi_qsyncPrepare.c
	$(CC) $(CFLAGS) -c fcgi_qsyncPrepare.c -o fcgi_qsyncPrepare.o
fcgi_utilRequest.o: fcgi_utilRequest.c
	$(CC) $(CFLAGS) -c fcgi_utilRequest.c -o fcgi_utilRequest.o
http_common.o: http_common.h http_common.c
	$(CC) $(CFLAGS) -c http_common.c -o http_common.o
lang.o: lang.h lang.c fcgi_common.o
	$(CC) $(CFLAGS) -c lang.c -o lang.o
ej.o: ej.c
	$(CC) $(CFLAGS) -c ej.c -o ej.o
stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c -o stats.o
wl.o: wl.c
	$(CC) $(CFLAGS) -c wl.c -o wl.o
wan.o: wan.c fcgi_common.o
	$(CC) $(CFLAGS) -c wan.c -o wan.o
st.o: st.c
	$(CC) $(CFLAGS) -c st.c -o st.o
lan.o: lan.c
	$(CC) $(CFLAGS) -c lan.c -o lan.o
widget.o: widget.c
	$(CC) $(CFLAGS) -c widget.c -o widget.o
ezp.o: ezp.h ezp.c fcgi_common.o 
	$(CC) $(CFLAGS) -c ezp.c -o ezp.o
httpd.o: httpd.h httpd.c 
	$(CC) $(CFLAGS) -c httpd.c -o httpd.o
upgrade.o: upgrade.c 
	$(CC) $(CFLAGS) -c upgrade.c -o upgrade.o
my_qnap_cloud.o: my_qnap_cloud.c 
	$(CC) $(CFLAGS) -c my_qnap_cloud.c -o my_qnap_cloud.o
stdenv.cgi: stdenv.c fcgi_common.o fcgi_cgi.o fcgi_nas_cgiutil.o fcgi_authLogin.o fcgi_download.o fcgi_file_op.o fcgi_utilRequest.o fcgi_upload.o fcgi_search.o fcgi_qbox_op.o fcgi_sysinfoReq.o fcgi_qsyncPrepare.o fcgi_sysRequest.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi fcgi_cgi.o fcgi_common.o fcgi_authLogin.o fcgi_nas_cgiutil.o fcgi_download.o fcgi_file_op.o fcgi_utilRequest.o fcgi_upload.o fcgi_search.o fcgi_qbox_op.o fcgi_sysinfoReq.o fcgi_sysRequest.o fcgi_qsyncPrepare.o stdenv.c -o stdenv.cgi
fcgi_http.exe: fcgi_http.c httpd.o fcgi_common.o fcgi_cgi.o lang.o ej.o ezp.o st.o fcgi_nas_cgiutil.o wl.o lan.o widget.o stats.o http_common.o wan.o upgrade.o my_qnap_cloud.o
	$(CC) $(CFLAGS) $(LDFLAGS) -lfcgi fcgi_nas_cgiutil.o httpd.o st.o ej.o ezp.o lang.o fcgi_cgi.o fcgi_common.o wl.o lan.o widget.o stats.o http_common.o wan.o upgrade.o my_qnap_cloud.o fcgi_http.c -o fcgi_http.exe

clean:
	rm -f *.cgi *.o
