# lighttpd configuration file
# 
## modules to load
# all other module should only be loaded if really neccesary
# - saves some time
# - saves memory
server.modules = ( 
	"mod_fastcgi", 
	"mod_auth", 
	"mod_alias", 
	"mod_status", 
	"mod_setenv", 
	"mod_webdav"
)
## Add the cgi module
# cgi.assign = (".cgi" => "")

setenv.add-response-header = ( "Access-Control-Allow-Origin" => "*" )
setenv.add-response-header += ( "Access-Control-Allow-Credentials" => "true " )
setenv.add-response-header += ( "Access-Control-Allow-Methods" => "ACL, CANCELUPLOAD, CHECKIN, CHECKOUT, COPY, DELETE, GET, HEAD, LOCK, MKCALENDAR, MKCOL, MOVE, OPTIONS, POST, PROPFIND, PROPPATCH, PUT, REPORT, SEARCH, UNCHECKOUT, UNLOCK, UPDATE, VERSION-CONTROL" )
setenv.add-response-header += ( "Access-Control-Allow-Headers" => "Overwrite, Destination, Content-Type, Depth, User-Agent, Translate, Range, Content-Range, Timeout, X-File-Size, X-Requested-With, If-Modified-Since, X-File-Name, Cache-Control, Location, Lock-Token, If" )
setenv.add-response-header += ( "Access-Control-Expose-Headers" => "DAV, content-length, Allow" )

## mimetype mapping
mimetype.assign = ( 
	".pdf"   => "application/pdf",
	".class" => "application/octet-stream",
	".pac"   => "application/x-ns-proxy-autoconfig",
	".swf"   => "application/x-shockwave-flash",
	".wav"   => "audio/x-wav",
	".gif"   => "image/gif",
	".jpg"   => "image/jpeg",
	".jpeg"  => "image/jpeg",
	".png"   => "image/png",
	".mpeg"  => "video/mpeg",
	".mpg"   => "video/mpeg",
	".mp4"   => "video/mpeg",
	".mov"   => "video/quicktime",
	".qt"    => "video/quicktime",
	".avi"   => "video/x-msvideo",
	".asf"   => "video/x-ms-asf",
	".asx"   => "video/x-ms-asf",
	".wmv"   => "video/x-ms-wmv",
	".mp3"   => "audio/mpeg",
	".css"   => "text/css",
	".html"  => "text/html",
	".htm"   => "text/html",
	".js"    => "text/javascript",
	".txt"   => "text/plain",
	".dtd"   => "text/xml",
	".xml"   => "text/xml"
)

# force use of the "write" backend (closes: #2401)
server.network-backend = "linux-sendfile"
server.event-handler = "linux-sysepoll"
index-file.names = ("login.asp")
fastcgi.debug = 1

#"bin-copy-environment" => ( "PATH", "SHELL", "USER" ),
## a static document-root, for virtual-hosting take look at the 
## server.virtual-* options
dir-listing.encoding = "utf-8"

## to help the rc.scripts
server.pid-file = "/var/run/lighttpd.pid"

#### webdav
$HTTP["url"] =~ "^($|/)" {
 server.dir-listing = "enable"
 webdav.activate = "enable"
 alias.url = ( "/cgi-bin" => "/etc/lighttpd/cgi-bin" )
}
server.upload-dirs = (
 "/tmp/mnt/SD/.upload",
 "/tmp/mnt/USB/.upload",
)

server.document-root = "/tmp/mnt"
fastcgi.server = (
  ".cgi" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http_cgi",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".asp" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".html" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".htm" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  "common.js" =>
  (( "socket" => "/tmp/tinyfcgi.socket_common_js",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
)

$HTTP["url"] =~ "/cgi-bin" {
  fastcgi.server = (
  ".cgi" =>
  (( "socket" => "/tmp/tinyfcgi.socket",
    "bin-path" => "/etc/lighttpd/cgi-bin/stdenv.cgi",
    "bin-environment" => (
      "PHP_FCGI_CHILDREN" => "16",
      "PHP_FCGI_MAX_REQUESTS" => "10000"
  ),
  "broken-scriptfilename" => "enable" ,
  "allow-x-send-file" => "enable"
  )),
)
}

$SERVER["socket"] == ":80" {
server.document-root = "/tmp/mnt"
fastcgi.server = (
  ".cgi" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http_cgi",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".asp" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".html" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  ".htm" =>
  (( "socket" => "/tmp/tinyfcgi.socket_http",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
  "common.js" =>
  (( "socket" => "/tmp/tinyfcgi.socket_common_js",
     "bin-path" => "/etc/lighttpd/cgi-bin/fcgi_http.exe",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000"
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
)

}
