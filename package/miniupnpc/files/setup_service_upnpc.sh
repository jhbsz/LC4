#!/bin/sh

setup_rule() {
    local ruleidx="$1"
    local wanst="$2"
    local wanpr="$3"
    local deactivate_rule="$4"
    local loopout=1
    local retval=0
    [ -z "$NUM" ] && NUM=0
    local wipaddr="$(nvram get wan${NUM}_ipaddr)"
    local localnet=0
    local outport="$(nvram show upnpc_status_rule $ruleidx outport)"
    local defoutport="$(nvram show upnpc_setting_rule $ruleidx outport)"
    # Get random number
    local incn="$(($(cat /proc/uptime | awk -F'[. ]' '{print $1$2}') % 2000))"
    incn=$(($incn % 2000))
    [ -z "$outport" -o "$outport" -lt "$defoutport" ] && outport="$defoutport"
    eval "$(ipcalc $wipaddr 8)"
    [ "$NETWORK" = "10.0.0.0" ] && localnet=1
    [ "$localnet" = "0" ] && {
        eval "$(ipcalc $wipaddr 12)"
        [ "$NETWORK" = "172.16.0.0" ] && localnet=1
    }
    [ "$localnet" = "0" ] && {
        eval "$(ipcalc $wipaddr 16)"
        [ "$NETWORK" = "192.168.0.0" ] && localnet=1
    }
    # We do upnp triggle only for private ip address
    [ "$localnet" = "1" ] && {
        [ "$wanst" = "4" -a "$deactivate_rule" = "0" ] && {
            upnpc -l > /tmp/igd_upnpc_list.tmp
            local inport="$(nvram show upnpc_setting_rule $ruleidx inport)"
            local proto="$(nvram show upnpc_setting_rule $ruleidx proto)"
            local myip="$(nvram get wan0_ipaddr)"
            while [ "$loopout" -gt "0" ]; do
            	[ -n "$(cat /tmp/igd_upnpc_list.tmp | grep "$outport->" | grep TCP | sed "/$myip/d")" ] && {
                    [ "$outport" -lt "8000" ] && outport=8000 || outport=$(($outport + 2))
                    continue
                }
                upnpc -a $myip $inport $outport $proto 2>&1 | tee /tmp/upnpc_log_$ruleidx
                [ -n "$(cat /tmp/upnpc_log_$ruleidx | grep "No IGD UPnP Device found")" ] && {
                    retval=1
                    loopout=0
                    break
                }
                [ -z "$(cat /tmp/upnpc_log_$ruleidx | grep "failed with code")" ] && break
                # if outport is inside 1~5000, we first move the port outside 5000
                [ "$outport" -lt "8000" ] && outport=8000 || outport=$(($outport + $incn))
                [ "$outport" -ge "65536" ] && outport=8000
                loopout=$(($loopout - 1))
            done
        } || {
            loopout=0
        }
    } 
    [ "$loopout" -gt "0" ] && {
        nvram replace attr upnpc_status_rule $ruleidx activated 1
        nvram replace attr upnpc_status_rule $ruleidx outport $outport
    } || {
        nvram replace attr upnpc_status_rule $ruleidx activated 0
        nvram replace attr upnpc_status_rule $ruleidx outport $outport
    }
    return $retval
}
wanstate="$(nvram show wan_status_rule 0 state)"
proto="$(nvram get wan0_proto)"
servicenum="$(nvram get upnpc_setting_rule_num)"
idx=0
deactrule=0
while [ "$idx" -lt "$servicenum" ]; do
setup_rule "$idx" "$wanstate" "$wanproto" "$deactrule"
deactrule="$?"
idx=$(($idx + 1))
done

