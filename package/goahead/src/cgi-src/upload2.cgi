#!/bin/sh

   PWD="/web/cgi-bin"
   TMP="/var/upload"
SERVER="ftp://ftp2.zyxel.com/NBG4615/firmware"
#SERVER="ftp://zyus2504:zyus2504@ftp2.zyxel.com.tw/NBG4615/firmware"
#SERVER="ftp://public:public@172.10.5.30/NBG-419N/firmware"

#nvram_set zyfw_fw_file
#nvram_set zyfw_fw_version
#nvram_set zyfw_release_date
#nvram_set zyfw_release_note
#nvram_set zyfw_size
#nvram_set zyfw_upload_message "Please wait..."

#$PWD/upload1.cgi

rm -fr $TMP

mkdir $TMP

###wget $SERVER/ZYFW_INFO.tgz -O $TMP/ZYFW_INFO.tgz  
wget $SERVER/zyfw_info -O $TMP/zyfw_info

###tar xfC $TMP/ZYFW_INFO.tgz $TMP

###rm $TMP/ZYFW_INFO.tgz

#test -z `nvram_get current_release_date` && nvram_set current_release_date 0

#! test -s $TMP/zyfw_info && nvram_set zyfw_upload_message "Retrieve Failed. Please check your Internet connection or try again later." && exit
! test -s $TMP/zyfw_info && echo Server: && echo Pragma: no-cache && echo Content-type: text/html && echo && echo "<html><script>parent.upload_message='Retrieve Failed. Please check your Internet connection or try again later.'; parent.reload_upload_firmware_page();</script></html>" && exit

#test `nvram_get current_release_date` -ge `cat $TMP/zyfw_info | sed -n 's/-//g;s/\///g;s/Release date:[ ]*\(.*\)/\1/p'` && nvram_set zyfw_upload_message "No newer firmware exists. The firmware on your device is already the latest version." && exit

#ReleaseNote=`cat $TMP/zyfw_info | sed -n 's/Release note:[ ]*\(.*\)/\1/p'`
#wget $SERVER/$ReleaseNote -O $TMP/$ReleaseNote          

echo Server:
echo Pragma: no-cache
echo Content-type: text/html

echo
echo \<html\>
echo \<head\>
echo \<TITLE\>Upload Firmware\</TITLE\>
echo \<link rel=stylesheet href=/style/normal_ws.css type=text/css\>
echo \<meta http-equiv=\"content-type\" content=\"text/html\; charset=iso-8859-1\"\>
echo \</head\>
echo \<body\>
echo \<script\>
echo "parent.upload_message='Ready';"
echo "parent.zyfw_fw_file='';"
echo "parent.zyfw_fw_version='';"
echo "parent.zyfw_release_date='';"
echo "parent.zyfw_release_note='';"
echo "parent.zyfw_size='';"

echo "/*"
### Tommy
#nvram_set zyfw_fw_file        "`cat $TMP/zyfw_info | sed -n 's/FW file:[      ]*\(.*\)/\1/p'`"
nvram set zyfw_fw_file="`cat $TMP/zyfw_info | sed -n 's/FW file:[      ]*\(.*\)/\1/p'`"
echo "*/"
echo "parent.zyfw_fw_file='`cat $TMP/zyfw_info | sed -n 's/FW file:[      ]*\(.*\)/\1/p'`';"
#nvram_set zyfw_fw_version     "`cat $TMP/zyfw_info | sed -n 's/FW version:[   ]*\(.*\)/\1/p'`"
echo "parent.zyfw_fw_version='`cat $TMP/zyfw_info | sed -n 's/FW version:[   ]*\(.*\)/\1/p'`';"
#nvram_set zyfw_release_date   "`cat $TMP/zyfw_info | sed -n 's/Release date:[ ]*\(.*\)/\1/p'`"
echo "parent.zyfw_release_date='`cat $TMP/zyfw_info | sed -n 's/Release date:[ ]*\(.*\)/\1/p'`';"
#nvram_set zyfw_release_note   "`cat $TMP/zyfw_info | sed -n 's/Release note:[ ]*\(.*\)/\1/p'`"
echo "parent.zyfw_release_note='`cat $TMP/zyfw_info | sed -n 's/Release note:[ ]*\(.*\)/\1/p'`';"
echo "/*"
### Tommy
#nvram_set zyfw_size           "`cat $TMP/zyfw_info | sed -n 's/Size:[         ]*\(.*\)/\1/p'`"
nvram set zyfw_size="`cat $TMP/zyfw_info | sed -n 's/Size:[         ]*\(.*\)/\1/p'`"
echo "*/"
echo "parent.zyfw_size='`cat $TMP/zyfw_info | sed -n 's/Size:[         ]*\(.*\)/\1/p'`';"
#nvram_set zyfw_upload_message "Ready"

echo "parent.reload_upload_firmware_page();"
echo \</script\>
echo \</body\>\</html\>

#cp -s $TMP/"`nvram_get zyfw_release_note`" /etc_ro/web/local/advance/zyfw_release_note.pdf
#cp -s $TMP/"`cat $TMP/zyfw_info | sed -n 's/Release note:[ ]*\(.*\)/\1/p'`" $TMP/release_note.pdf
