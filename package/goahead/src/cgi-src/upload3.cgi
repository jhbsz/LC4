#!/bin/sh

   PWD="/web/cgi-bin"
   TMP="/var/upload"
SERVER="ftp://ftp2.zyxel.com/NBG4615/firmware"
#SERVER="ftp://zyus2504:zyus2504@ftp2.zyxel.com.tw/NBG4615/firmware"
#SERVER="ftp://public:public@172.10.5.30/NBG-419N/firmware"

#echo Server:
#echo Pragma: no-cache
#echo Content-type: text/html

#echo
#echo \<html\>
#echo \<head\>
#echo \<TITLE\>Upload Firmware\</TITLE\>
#echo \<link rel=stylesheet href=/style/normal_ws.css type=text/css\>
#echo \<meta http-equiv=\"content-type\" content=\"text/html\; charset=iso-8859-1\"\>
#echo \</head\>
#echo \<body\>
#echo \<script\>

#nvram_set zyfw_upload_message "Firmware download in progress. Please wait..."

#$PWD/upload1.cgi

echo "1" > /var/upload_start
### Tommy
#wget $SERVER/"`nvram_get zyfw_fw_file`" -O $TMP/zyfw_fw_file
wget $SERVER/"`nvram get zyfw_fw_file`" -O $TMP/zyfw_fw_file
#wget $SERVER/xxx -O $TMP/zyfw_fw_file
#wget -c ftp://zyus2504:zyus2504@ftp2.zyxel.com.tw/NBG-419N/firmware/NBG-419N_V1.00_BFQ.2_B1_20091218.bin -O /var/NBG-419N_V1.00_BFQ.2_B1_20091218.bin

echo "0" > /var/upload_start

ls -l /var/upload/zyfw_fw_file | sed 's/ \/var.*$//g' | sed 's/^.* //g' > /var/upload/zyfw_fw_file_size
zyfw_fw_file_size=`cat /var/upload/zyfw_fw_file_size`
### Tommy
#fw_file_size=`nvram_get zyfw_size`
fw_file_size=`nvram get zyfw_size`
if [ $fw_file_size -gt $zyfw_fw_file_size ]; then
 echo Server: && echo Pragma: no-cache && echo Content-type: text/html && echo && echo "<html><script>parent.upload_message='Firmware download has stopped. Please check your Internet connection or try again later.'; parent.reload_upload_firmware_page();</script></html>" && rm /var/upload/zyfw_fw_file && exit
fi
if [ $zyfw_fw_file_size -gt $fw_file_size ]; then
 echo Server: && echo Pragma: no-cache && echo Content-type: text/html && echo && echo "<html><script>parent.upload_message='Not a valid firmware.'; parent.reload_upload_firmware_page();</script></html>" && rm /var/upload/zyfw_fw_file && exit
fi

! test -s $TMP/zyfw_fw_file && echo Server: && echo Pragma: no-cache && echo Content-type: text/html && echo && echo "<html><script>parent.upload_message='Firmware download has stopped. Please check your Internet connection or try again later.'; parent.reload_upload_firmware_page();</script></html>" && rm /var/upload/zyfw_fw_file && exit

#echo \</script\>
#echo \</body\>\</html\>

#nvram_set zyfw_upload_message "Firmware upgrade is proceeding. Please wait and do NOT power off your device."

#aron add for more free memory
killall wscd
killall syslog-ng

$PWD/upload.cgi $TMP/zyfw_fw_file

#nvram_set zyfw_fw_file
#nvram_set zyfw_fw_version
#nvram_set zyfw_release_date
#nvram_set zyfw_release_note
#nvram_set zyfw_size
#nvram_set zyfw_upload_message
