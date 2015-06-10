#!/bin/sh

local PLATFORM="$(nvram get platform)"

[ $PLATFORM != "ALS" ] && exit

ret="$(/bin/ps -ef |grep UART|grep port0)"
[ -z "$ret" ] && {
    /usr/sbin/UART /etc/serial.conf.port0 &
}
ret="$(/bin/ps -ef |grep UART|grep port1)"
[ -z "$ret" ] && {
    /usr/sbin/UART /etc/serial.conf.port1 &
}
ret="$(/bin/ps -ef |grep UART|grep port2)"
[ -z "$ret" ] && {
    /usr/sbin/UART /etc/serial.conf.port2 &
}
sync
echo 3 > /proc/sys/vm/drop_caches
