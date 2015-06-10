#!/bin/sh

. /etc/log.sh

# mainly for vlan
if_valid () (
  ifconfig "$1" >&- 2>&- ||
  [ "${1%%[0-9]*}" = "br" ] || [ "${1%%[0-9]*}" = "ppp" ] ||
  {
    [ "${1%%[0-9]*}" = "vlan" ] && ( 
      id=${1#vlan}
      vif=$(nvram show vlan_rule $2 ifname)
      [ -z "$vif" ] && {
        # give another try
        hwaddr=$(nvram get ${hwname}macaddr)
        vif=$(ifconfig -a | awk '/^eth.*'$hwaddr'/ {print $1; exit}' IGNORECASE=1)
      }

      [ -z "$vif" ] && return 1
      echo "# vlan$id => $vif"

      $DEBUG ifconfig $vif promisc
      $DEBUG ifconfig $vif up
      $DEBUG vconfig add $vif $id 2>&-

# For change default MAC Address.
      [ "$id" = "1" ] && \
        HWADDR=$(nvram show lan_hwaddr_rule_default 0 hwaddr) || \
        HWADDR=$(nvram show wan_hwaddr_rule_default $(($i-1)) hwaddr)
      ip link set dev vlan${id} address $HWADDR

    )
  } ||
  { debug "# missing interface '$1' ignored"; false; }
)

# mainly for br $1: brX $2: stp
ifbr_valid () (
  ifconfig "$1" >&- 2>&- ||
  [ "${1%%[0-9]*}" = "vlan" ] || [ "${1%%[0-9]*}" = "ppp" ] ||
  {
    [ "${1%%[0-9]*}" = "br" ] && ( 
      id=${1#br}
      echo "# br$id"

      $DEBUG brctl delbr $1 2>&-
      $DEBUG brctl addbr $1
      $DEBUG brctl setfd $1 0
      $DEBUG brctl stp $1 ${2:-0}

      for sif in $(nvram show br_rule $id ifnames); do
        if_valid $sif || continue
        $DEBUG ip link set dev $sif up allmulticast on
        $DEBUG brctl addif $1 $sif
      done

    )
  } ||
  { debug "# missing br interface '$1' ignored"; false; }
)

# $1:proto, $2:iface, $3:type, $4:num, $5:ifname

do_ifup() {
    lock /tmp/ifupdown.$2.lock
    proto=$1
    [ "$(nvram show vpn_status_rule $4 state)" = "1" ] && proto=vpnc
    
    local ip_type="$(nvram show wl0_apcli_rule 0 ip_type)"
    [ "$proto" = "wisp" -a -n "$ip_type" ] && proto="ip_type"
	case "$proto" in
	static)
		ip=$(nvram show ${3}_static_rule $4 ipaddr)
		mask=$(nvram show ${3}_static_rule $4 mask)
		mtu=$(nvram show ${3}_main_rule $4 mtu)

		$DEBUG ip -4 addr flush dev $5
		$DEBUG ip -4 addr add $ip/$mask broadcast + dev $5
		$DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up

        gateway=$(nvram show ${3}_static_rule $4 gateway)
        dns=$(nvram show ${3}_static_rule $4 dns1)
        iface_up "static" "$iface" "$ip" "$mask" "$gateway" "$dns"
        lock -u /tmp/ifupdown.$2.lock
	;;
    directip)
        # config the directip card first
		[ -x "/sbin/ifup.$1" ] && { $DEBUG /sbin/ifup.$1 ${2} ${3} ${4} ${5}; }
        [ "$?" = "1" ] && lock -u /tmp/ifupdown.$2.lock && exit

        # same as dhcp TODO: remove the same dhcp code
		dhcp_ip=$(nvram get ${2}_ipaddr)
		mtu=$(nvram show ${3}_main_rule $4 mtu)

		$DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up

        pidfile=/var/run/$2.pid
        [ -f $pidfile ] && $DEBUG kill_and_wait $(cat $pidfile)

		dhcp_args="-i $5 ${dhcp_ip:+-r $dhcp_ip} -b -p $pidfile"
		dhcp_hostname=$(nvram show ${3}_dhcp_rule $4 hostname)
		dhcp_hostname=${dhcp_hostname%%.*}

		[ -z $dhcp_hostname ] || dhcp_args="$dhcp_args -H $dhcp_hostname"

        # FIXME!
		[ "$1" = "pptp" ] && dhcp_args="$dhcp_args -n -q" || dhcp_args="$dhcp_args -R &"

		# XXX: [ -r $pidfile ] && oldpid=$(cat $pidfile 2>&-)
		# XXX: [ -n "$oldpid" ] && pidof udhcpc | grep "$oldpid" >&- 2>&- && {
		# XXX: 	kill_and_wait udhcpc $oldpid
		# XXX: }
                local UML295="$(cat /proc/bus/usb/devices | grep "Vendor=10a9 ProdID=6064")"

                [ -n "$UML295" ] || {
		    ${DEBUG:-eval} "udhcpc $dhcp_args"
                }
		# hotplug events are handled by /usr/share/udhcpc/default.script
        lock -u /tmp/ifupdown.$2.lock
        ;;
	dhcp*|htc*|iphone*|wimax*|beceem*)
        [ -x "/sbin/ifup.$1" ] && { $DEBUG /sbin/ifup.$1 $4; }
        dhcp_ip=$(nvram get ${2}_ipaddr)
        [ "$proto" = "htc" -o "$proto" = "iphone" -o "$proto" = "wimax" ] && mtu=$(nvram show wan_${proto}_rule $4 mtu)
        [ "$proto" = "wisp" -a "$(nvram show wl0_apcli_rule 0 enable)" = "0" ] && {
            # We rescan wifi sites and, if found, restart wifi.
            # after this script, wl0_apcli_rule enable entry will be set 1
            /sbin/ezp-wl-ctrl apcli_swap_scan
        }
        $DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up

        pidfile=/var/run/$2.pid
        [ -f $pidfile ] && $DEBUG kill_and_wait $(cat $pidfile)

        dhcp_args="-i $5 ${dhcp_ip:+-r $dhcp_ip} -b -p $pidfile"
        dhcp_hostname=$(nvram show ${3}_dhcp_rule $4 hostname)
        dhcp_hostname=${dhcp_hostname%%.*}

        [ -z $dhcp_hostname ] || dhcp_args="$dhcp_args -H $dhcp_hostname"

# FIXME!
        [ "$1" = "pptp" ] && dhcp_args="$dhcp_args -n -q" || dhcp_args="$dhcp_args -R &"

# XXX: [ -r $pidfile ] && oldpid=$(cat $pidfile 2>&-)
# XXX: [ -n "$oldpid" ] && pidof udhcpc | grep "$oldpid" >&- 2>&- && {
# XXX: 		kill_and_wait udhcpc $oldpid
# XXX: }
        ${DEBUG:-eval} "udhcpc $dhcp_args"
# hotplug events are handled by /usr/share/udhcpc/default.script
        lock -u /tmp/ifupdown.$2.lock
	;;
	wisp*)
        [ -x "/sbin/ifup.$1" ] && { $DEBUG /sbin/ifup.$1 $4; }
        dhcp_ip=$(nvram get ${2}_ipaddr)
        [ "$proto" = "wisp" -a "$(nvram show wl0_apcli_rule 0 enable)" = "0" ] && {
            # In M1, if wisp connect 10times fail, we do nothing
            if [  "$(nvram get platform)" = "AXA" -o "$(nvram get platform)" = "M2E" ]; then
                local wisp_trycount="$(nvram show wan_status_rule 0 wisp_trycount)"
                if [ $wisp_trycount -lt 10 ]; then
                    /sbin/ezp-wl-ctrl apcli_swap_scan
                    wisp_trycount=$(($wisp_trycount + 1))
                    nvram replace attr wan_status_rule 0 wisp_trycount $wisp_trycount
                fi
            else
                /sbin/ezp-wl-ctrl apcli_swap_scan
            fi
        }

        pidfile=/var/run/$2.pid
        [ -f $pidfile ] && $DEBUG kill_and_wait $(cat $pidfile)

        dhcp_args="-i $5 ${dhcp_ip:+-r $dhcp_ip} -b -p $pidfile"
        dhcp_hostname=$(nvram show ${3}_dhcp_rule $4 hostname)
        dhcp_hostname=${dhcp_hostname%%.*}

        [ -z $dhcp_hostname ] || dhcp_args="$dhcp_args -H $dhcp_hostname"
        [ "$1" = "pptp" ] && dhcp_args="$dhcp_args -n -q" || dhcp_args="$dhcp_args -R &"
        # If we check there is no ssid/bssid in apcli setting, just leave
        [ -n "$(nvram show wl0_apcli_rule 0 ssid)" -o -n "$(nvram show wl0_apcli_rule 0 bssid)" ] && {
            $DEBUG ip -4 link set dev $5 ${mtu:+mtu $(($mtu))} up
            ${DEBUG:-eval} "udhcpc $dhcp_args"
        }
        lock -u /tmp/ifupdown.$2.lock
	;;
    vpnc)
        vpn_type=$(nvram show wan_pptp_l2tp_rule $4 vpn_type)
        if [ "$vpn_type" = "pptp" ]; then
            ifup.pptp ${2} ${3} ${4} ${5}
        elif [ "$vpn_type" = "l2tp" ]; then 
            ifup.l2tp ${2} ${3} ${4} ${5}
        fi
        lock -u /tmp/ifupdown.$2.lock
    ;;
	*)
        lock -u /tmp/ifupdown.$2.lock
		[ -x "/sbin/ifup.$1" ] && { 
            $DEBUG /sbin/ifup.$1 ${2} ${3} ${4} ${5}
        } || {
		    echo "### ifup ${2}: ignored ${2}_proto=\"$1\" (not supported)"
        }
	;;
	esac
}

iface_update_default_route_and_dns () {
local route_args; route_args=""
echo -n > /tmp/resolv.conf
local i=0
limit=$(nvram get wan_num)

# "dnsassigned" check is used if wan is up but no valid DNS assigned, 
# ie : user selects ISP dns but ISP fails to give us one.
dnsassigned="0"

while [ $i -lt $limit ];
do
    # If current wan action is up
    local action=$(nvram show wan_status_rule $i action)
    local type="wan"
    [ "$(nvram show vpn_status_rule $i action)" = "up" ] && type="vpn"
    local state=$(nvram show ${type}_status_rule $i state)
    [ "$action" = "up" -a "$state" -ge "3" ] && {
        # get ${type} info from nvram
        local gateway=$(nvram get ${type}${i}_gateway)
        local proto=$(nvram get wan${i}_proto)
        local ifname=$(nvram get ${type}${i}_ifname)
        
        [ -n "$gateway" ] && {
            local dns=$(nvram get wan${i}_dns)
            local weight=$(nvram show wan_lb_rule ${i} weight)
            local src=$(nvram get ${type}${i}_ipaddr)

            # this is really a hack for directip
            ip route replace $gateway proto static dev $ifname src $src
            route_args="$route_args nexthop via $gateway dev $ifname weight ${weight:-1}"
            [ -n "$dns" ] && {
                echo "nameserver $dns" >> /tmp/resolv.conf
                logger -t "ifacedns[222]" "WAN$(($i + 1)) Using DNS $dns"
                dnsassigned="1"
                ip route append $dns via $gateway dev $ifname
            }
        }
        [ -z "$gateway" -a -n "$ifname" ] && {
            route_args="$route_args nexthop dev $ifname weight ${weight:-1}"
        }
        [ "$(nvram get wan${i}_proto)" = "wisp" ] && {
            logger EZP_USR "WIFI Connect:WISP Connected"
        }
    }
    i=$(expr $i + 1)
done

# If no dns assigned, we set opendns as default DNS
wan_state=$(nvram show wan_status_rule $cur state)
[ "$dnsassigned" = "0" ] && {
    echo "nameserver 208.67.220.220" >> /tmp/resolv.conf
    echo "nameserver 208.67.222.222" >> /tmp/resolv.conf
    logger -t "ifacedns[222]" "Using Default DNS 208.67.220.220 208.67.222.222" &
}
ip route del default
[ -n "$route_args" ] && ip route replace default proto static $route_args
ip route flush cache
}

# Add/delete the routing tables for a individual interface.
# $1:num  $2:ipaddr $3:mask $4:gateway $5:ifname
iface_add_route () {
    proto=$(nvram get wan${1}_proto)
    eval $(ipcalc $2 $3)
    ip route add table 1${num} $4 dev $5 src $2 proto static
    ip route add table 1${num} $NETWORK/$PREFIX dev $5 src $2 proto static
    ip route add table 1${num} default via $4 dev $5 proto static
    ip rule add prio 1${num} from $2 lookup 1$num
    eval $(ipcalc $(nvram get lan0_ipaddr) $(nvram get lan0_mask))
    ip route add table 1${num} $NETWORK/$PREFIX dev $(nvram get lan0_ifname) proto static
    ip rule add from $NETWORK/$PREFIX fwmark $(nvram get wan${1}_rtmark)/$(nvram get rt_mask) lookup 1${num}
    ip route flush cache
}

iface_del_route () {
    proto=$(nvram get wan${i}_proto)
    eval $(ipcalc $(nvram get lan0_ipaddr) $(nvram get lan0_mask))
    ip rule del from $NETWORK/$PREFIX fwmark $(nvram get wan${1}_rtmark)/$(nvram get rt_mask) lookup 1${num}
    ip route del table 1${num} $NETWORK/$PREFIX dev $(nvram get lan0_ifname) proto static
    eval $(ipcalc $2 $3)
    ip rule del prio 1${num} from $2 lookup 1$num
    ip route del table 1${num} default via $4 dev $5 proto static
    ip route del table 1${num} $NETWORK/$PREFIX dev $5 src $2 proto static
    ip route del table 1${num} $4 dev $5 src $2 proto static
    ip route flush cache
}

iface_up () {
    lock /tmp/.iface
    type="${2%%[0-9]*}"
    num="${2#$type}"
    proto=$1
    state=$(nvram show ${type}_status_rule $num state)
    [ "$(nvram show vpn_status_rule $num action)" = "up" ] && {
        state=$(nvram show vpn_status_rule $num state)
        type="vpn"
    }
    to_log "iface up $type $num $proto $state"
    case $state in
        3)
            ipaddr=$(nvram get ${type}${num}_ipaddr)
            mask=$(nvram get ${type}${num}_mask)
            gateway=$(nvram get ${type}${num}_gateway)
            ifname="$(nvram get ${type}${num}_ifname)"
            if [ "$type" = "wan" -o "$type" = "vpn" ]; then
                # remove routing rule for wan detect 
                host=$(nvram show wan_detect_rule $num host)
                [ -n "$host" ] && ip route del $host/32 via $gateway 
                iface_add_route $num $ipaddr $mask $gateway $ifname
                iface_update_default_route_and_dns
                msg="WAN$(($num+1))"
            else
                msg="LAN"
            fi
            log_usr "$msg" up OK [$ipaddr]
            to_log "iface up set $2 state to 4"
            ezp-i2c internet on
            # set wan state to 4
            nvram replace attr ${type}_status_rule $num state "4"
            /etc/rc.common /etc/init.d/mDNSResponder  restart
            /etc/rc.common /etc/init.d/als-ddns start
            nvram replace attr ${type}_status_rule $num uptime "$(get_system_time)"
            [ "$type" = "wan" ] && {
                # hotplug event
                env -i ACTION="ifup" INTERFACE="$2" PROTO=$proto TYPE="$type" \
                    NUM="$num" IFNAME="$ifname" /sbin/hotplug-call "iface" &
            }
            [ "$(nvram get platform)" = "ALS" ] && {
                /etc/rc.common /etc/init.d/als-ddns start
                [ "$(nvram get wan0_proto)" = "wwan" ] && {
                    gpioctl clear 67
                    /etc/rc.common /etc/init.d/pptpd stop
                    /etc/rc.common /etc/init.d/pptpd start
                }
                [ "$(nvram get wan0_proto)" = "pppoe" ] && {
                    /etc/rc.common /etc/init.d/pptpd stop
                    /etc/rc.common /etc/init.d/pptpd start
                }
            }
        ;;
        1)
            nvram set ${type}${num}_ipaddr=$3
            nvram set ${type}${num}_mask=$4
            nvram set ${type}${num}_gateway=$5
            nvram set ${type}${num}_dns=$6
            nvram set ${type}${num}_domain=$7
            [ "$type" = "wan" ] && {
                ezp-i2c wan ip
            }
            [ "$type" = "lan" ] && {
                ezp-i2c lan ip
            }
            to_log "iface up set ${type}${num} state to 2"
            # set wan state to 2 to indicate the wan get ip
            nvram replace attr ${type}_status_rule $num state "2"
            # reset plug_rule 
            nvram replace attr plug_rule 0 wisp "0"

            # add routing rule for wan detect 
            host=$(nvram show wan_detect_rule $num host)
            [ -n "$host" ] && ip route add $host/32 via $5 &
        ;;
    esac
    lock -u /tmp/.iface
}

down_service_and_remove_route() {
    type="${2%%[0-9]*}"
    num="${2#$type}"
    proto=$1
    ipaddr="$(nvram get ${2}_ipaddr)"

    [ "$type" = "wan" ] && [ -n "$ipaddr" ] && {
        gateway="$(nvram get ${2}_gateway)"
        ifname="$(nvram get ${2}_ifname)"
        mask="$(nvram get ${2}_mask)"
        [ "$(nvram show vpn_status_rule $num action)" = "up" ] && {
            ifname="$(nvram get ${2}_ifnames)"
        }
        
        iface_del_route $num $ipaddr $mask $gateway $ifname
    }
    if [ "$type" = "wan" ]; then
        iface_update_default_route_and_dns 
        msg="WAN$(($num+1))"
    else
        msg="LAN"
    fi
    log_usr "$msg" down OK [${ipaddr}]
    [ "$type" = "wan" ] && {
        env -i ACTION="ifdown" INTERFACE="$2" PROTO=$proto \
        TYPE="$type" NUM="$num" IFNAME="$ifname" /sbin/hotplug-call "iface" &
    }
}

iface_down () {
    ifacename=${2}
    type="${2%%[0-9]*}"
    num="${2#$type}"
    proto=$1
    action=$(nvram show wan_status_rule $num action)
    to_log "iface down $type $num $proto $action"
    case $action in
        *)
            down_service_and_remove_route $proto $2
            ipaddr="$(nvram get ${2}_ipaddr)"
            [ -n "$ipaddr" ] && {
                nvram unset ${2}_ipaddr
                nvram unset ${2}_mask
                nvram unset ${2}_gateway
                nvram unset ${2}_dns
                nvram unset ${2}_domain
                [ "$type" = "wan" ] && {
                    ezp-i2c wan ip
                    ezp-i2c internet off
		    #nvram replace attr wan_status_rule $num action down
                }
                [ "$type" = "lan" ] && {
                    ezp-i2c lan ip
                }
            }
        ;;
        standby)
            down_service_and_remove_route $proto $2
            # add routing rule for wan detect 
            host=$(nvram show wan_detect_rule $num host)
            gateway="$(nvram get ${2}_gateway)"
            [ -n "$host" ] && ip route add $host/32 via $gateway &
        ;;
    esac
    lock -u /tmp/.iface.changestate.${ifacename}.lock
    [ -f /tmp/.iface.changestate.${ifacename}.lock.pid ] && rm /tmp/.iface.changestate.${ifacename}.lock.pid
    
}

# 1:$iface
iface_detect() {
    local iface=$1
    local type="${1%%[0-9]*}"
    local num="${1#$type}"
    local err=0
    local proto=$(nvram get ${iface}_proto)
    local ifname=$(nvram get ${iface}_ifname)
    local failcount=$(nvram get ${iface}_failcount)
    local host_type=$(nvram show wan_detect_rule $num host_type)
    local detect_type=$(nvram show wan_detect_rule $num type)
    local timeout=$(nvram show wan_detect_rule $num timeout)
    local threshold=$(nvram show wan_detect_rule $num threshold)
    local enable=$(nvram show wan_detect_rule $num enable)
    local ipaddr=$(nvram get ${iface}_ipaddr)

    
    # if need detect remote host
    if [ "$enable" = "1" -a "$proto" != "static" ]; then 
        # PPP only uses ping.
        [ "${ifname%%[0-9]*}" = "ppp" ] && detect_type="ping"

        case $detect_type in
            arping)
                #FIXME EZP: for arping parameter and wan proto.
                arping -q -c 1 -w 1 -I $ifname -s $(nvram get ${iface}_ipaddr) \
                    $(nvram get ${iface}_gateway) || err=1
            ;;
            ping)
                case $host_type in
                    gateway)
                       pinghost="$(nvram get ${iface}_gateway)"
                       [ $pinghost = "10.64.64.64" -a "${ifname%%[0-9]*}" = "ppp" ] && {
                           local gateway_valid=$(nvram show ppp_control_rule 0 gateway_valid)
                           [ $gateway_valid = "0" ] && {
                               pinghost="8.8.8.8"
                           }
                       }
                    ;;
                    custom)
                       pinghost="$(nvram show wan_detect_rule $num host)"
                    ;;
                esac
                [ -z $pinghost ] && pinghost="8.8.8.8"
                ping -q -c 1 -w $timeout -I $ifname $pinghost || err=1
            ;;
        esac
        [ "$err" = "0" ] &&  {
            failcount="0"
        }

        [ "$err" = "1" ] && {
            failcount=$(($failcount + 1))
            if [ "$failcount" -ge "$threshold" ]; then
                err=2
                failcount=$threshold
                logger EZP_USR "WAN$(($num+1)): Connectivity Check to $pinghost Failed".
            fi
        }
    else
        [ -z "$ipaddr" ] && {
            err=2
        }
        failcount="0"
    fi

    nvram set ${iface}_failcount=$failcount

    return $err
}
# Usage set_wan_vpn wanidx:[0|1] vpndev:[ppp0|ipsec0|...] updown:[up|down]
# Example : set_wan_vpn 0 ipsec0 up
# Desciprtion : This interface set 1. vpn status 2. vpn devices and gateway
# While using this interface, we assume the vpn connection is established
# externally, and we would like to switch the vpn state, so it only support
# state 0: not connected and state 4: already connected
set_wan_vpn() {
    local wanidx="$1"
    local vpndev="$2"
    local updown="$3"
    local updownstate=0
    [ -n "$(echo $wanidx | grep wan)" ] && wanidx=${1#wan}
    [ -z "$wanidx" -o -z "$vpndev" -o -z "$updown" ] && return -1
    [ "$updown" = "up" ] && updownstate=4
    nvram fset vpn${wanidx}_ifname=$vpndev
    nvram replace attr vpn_status_rule $wanidx state $updownstate
    nvram replace attr vpn_status_rule $wanidx action $updown
}

# This interface is to set vpn# detail.
# Usage : set_wan_vpn_conn_detail wan_index ip gateway netmask dns
set_wan_vpn_conn_detail() {
    local wanidx="$1"
    local vpnipaddr="$2"
    local vpngw="$3"
    local vpnmask="$4"
    local vpndns="$5"
    [ -n "$(echo $wanidx | grep wan)" ] && wanidx=${1#wan}
    [ -z "$vpngw" -o -z "$vpnipaddr" ] && return -1
    nvram fset vpn${wanidx}_ipaddr=$vpnipaddr
    nvram fset vpn${wanidx}_gateway=$vpngw
    nvram fset vpn${wanidx}_netmask=$vpnmask
    nvram fset vpn${wanidx}_mask=$vpnmask
    nvram fset vpn${wanidx}_dns=$vpndns
}

to_log() {
    [ "$(nvram get net_dbg)" = "1" ] || return
    msg=$1
    echo "$1" > /dev/ttyS1
    echo "$1" >> /tmp/net.log
}

