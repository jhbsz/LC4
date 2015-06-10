#!/bin/sh /etc/rc.common
# hotplug event: $NUM and $TYPE are required. 
. /etc/network.sh
START=60
TRIGGER=90
log_msg="UPnP-server"

stop()
{
    killall upnpd
}

start()
{
[ "$(nvram show upnp_rule 0 enable)" = "1" ] || exit

if [ "$(nvram show wl_mode_rule 0 mode)" = "normal" -o \
     "$(nvram show wl_mode_rule 0 mode)" = "wisp"]; then
    stop
    # if igmpproxy is enable, client may register upnp service via
    # multicasting
    [ "$(nvram show igmp_proxy_rule 0 enable)" = "1" ] && {
        route add -net 239.0.0.0 netmask 255.0.0.0 dev "$(nvram get lan0_ifname)"
    } || {
        route del -net 239.0.0.0 netmask 255.0.0.0 dev "$(nvram get lan0_ifname)"
    }
    upnp_xml.sh "$(nvram get lan0_ipaddr)"
    upnpd -f "$(nvram get wan0_ifname)" "$(nvram get lan0_ifname)" &
fi
}

