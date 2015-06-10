#!/bin/sh

[ -z "$NUM" ] && NUM="all"
wan_state="$(nvram show wan_status_rule ${NUM} state)"
# We need wan to be connected
[ "$wan_state" -lt "2" ] && exit
ip_switch_num="$(nvram get ip_switch_rule_num)"
# No ip switch rule or rule num = 0 disable auto ip function.
[ -z "$ip_switch_num" -o "$ip_switch_num" = "0"  ] && exit

wan_num=$(nvram get wan_num)
# Haneling single wan case
[ "$wan_num" = "1" ] && NUM="0"
[ "$NUM" != "all" ] && {
    wan_ip="$(nvram get wan${NUM}_ipaddr)"
    wan_mask="$(nvram get wan${NUM}_mask)"
    # We need wan ip and network exist
    [ -z "$wan_ip" -o -z "$wan_mask" ] && exit
    eval "$(ipcalc $wan_ip $wan_mask)"
    wan_net="${NETWORK}:"
}
# Haneling multiple wan case
[ "$NUM" = "all" ] && {
    idx=0
    while [ "$idx" -lt "$wan_num" ]; do
        wan_ip="$(nvram get wan${idx}_ipaddr)"
        wan_mask="$(nvram get wan${idx}_mask)"
        [ -n "$wan_ip" -a -n "$wan_mask" ] && {
            eval "$(ipcalc $wan_ip $wan_mask)"
            wan_net="${wan_net}${NETWORK}:"
        }
        idx=$(($idx + 1))
    done
}
# After this step, wan_net looks like 
# single wan case : "10.1.1.0"
# miltiple wan case : "10.1.1.0:192.168.10.0:"
# We will use the net token to find a match with lan net


lan_ip="$(nvram get lan0_ipaddr)"
lan_mask="$(nvram get lan0_mask)"
ip_switch_num="$(nvram get ip_switch_rule_num)"
last_ip_switch_index=$(($ip_switch_num - 1))
idx="0"
ip_switch_found="0"

# First, one of the switch ips should equal to lan ip, if no, we set
# the lan ip to first switch ip
# This step is for maintainance, if we can be sure the first switch ip always
# equal to lan ip, then this stem can be ignored. Otherwise, perform it at wan
# started.
while [ "$idx" -lt "$ip_switch_num" ]; do
    switch_ip="$(nvram show ip_switch_rule $idx)"
    [ "$switch_ip" = "$lan_ip" ] && {
        ip_switch_found="$idx"
        break
    }
    idx="$(($idx + 1))"
done
# Make sure the last switch ip equal to lan_ip : xxx|yyy|zzz|lan_ipaddr
#[ "$ip_switch_found" = "0" ] && {
#    nvram replace rule ip_switch_rule "$last_ip_switch_index" $lan_ip
#}


eval "$(ipcalc $lan_ip $lan_mask)"
lan_net=$NETWORK
to_restart_lan="0"
idx="0"
# if lan doesn't needed to change
[ -z "$(echo $wan_net | grep "$lan_net:")" ] && exit

# Then we go over all switch ips and find if a switch ip network equal to wan network
# If there is a match, it means there is a "collision" of lan and wan and needs change lan ip.
# We try to find next and replace it with current lan net.
# 
# The ip_switch_rule looks like
# 1st_switch_ip|2nd_switch_ip|...
# The number of switch ip is wan num
while [ "$idx" -lt "$ip_switch_num" ]; do
    switch_ip="$(nvram show ip_switch_rule $idx)"
    eval "$(ipcalc $switch_ip $lan_mask)"
    switch_net="$NETWORK"
    # We try to find a switch_net "xxx.yyy.zzz.0:" in 
    # wan net list "aaa.bbb.ccc.0:ooo.ppp.qqq.0:iii.jjj.kkk:" string
    # If not exist, this switch net can be used
    [ -z "$(echo $wan_net | grep "$switch_net:")" ] && {
        [ "$switch_net" != "$lan_net" ] && {
            nvram replace attr lan_static_rule 0 ipaddr "$switch_ip"
            nvram replace attr lan_static_rule 0 dns "$switch_ip"
            to_restart_lan="1"
        }
        break
    }
    idx="$(($idx + 1))"
done

[ "$to_restart_lan" = "1" ] && {
    /etc/rc.common /etc/init.d/httpd stop
    /etc/rc.common /etc/init.d/dnsmasq stop
    /etc/rc.common /etc/init.d/network-lan stop
    ezp-wl-ctrl wifi_off
    sleep 20
    ezp-wl-ctrl wifi_on
    /etc/rc.common /etc/init.d/network-lan start
    /etc/rc.common /etc/init.d/dnsmasq start
    /etc/rc.common /etc/init.d/httpd start
}
