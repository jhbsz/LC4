#!/bin/sh
SYSTIME=/etc/tampSYSTIME
date +%y%m%d%H%M%S > $SYSTIME
time=`cat $SYSTIME` 
#output HTTP header
echo "Pragma: no-cache\n"
echo "Cache-control: no-cache\n"
echo "Content-type: application/octet-stream"
echo "Content-Transfer-Encoding: binary"			#  "\n" make Un*x happy
######echo "Content-Disposition: attachment; filename=\"rom-$time.dat\""
echo "Content-Disposition: attachment; filename=\"rom-$time.cfg\""
echo ""

######echo "#The following line must not be removed."
######echo "Default"
######ralink_init show 2860 2>/dev/null
nvram export /tmp/curr.cfg
cat /tmp/curr.cfg 2>/dev/null
rm -rf /tmp/curr.cfg
