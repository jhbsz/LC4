#!/bin/sh
{
link_check 0
TMPF=/var/uuu

while [ true ]; do
    
    sleep 300
    VSZ=0
    num=1
    ps -A | grep upnpd | cut -f2 -d "a" > $TMPF
    line=`cat $TMPF | wc -l`
    
    while [ $num -le $line ];
	do
  		VSZ=`head -n $num $TMPF | tail -n 1 | cut -f6 -d " "`
		if [ $VSZ -gt 2000 ]; then
			echo "VSZ=$VSZ"
			killall -9 upnpd
			sleep 2
			/sbin/restart_upnp.sh
			echo "Upnp is not normal" >> /var/ulog
			break;
		fi		
  		num=`expr $num + 1`
	done
		
	link_check 1
done
} &
