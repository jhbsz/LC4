#!/bin/sh
#   Program:
#

LOCK_FILE="/tmp/.auto_fixedip.lock"
FIXED_HWADDR="00:05:4F:40:01:00"

lock $LOCK_FILE

FL_HWA_TOTAL=$(nvram show fl_hwaddr_rule_num 0)
if [ -n "$FL_HWA_TOTAL" ]; then
    loop=0
    while [ $loop -lt $FL_HWA_TOTAL ]
    do
        FL_HWA_RULE=$(nvram show fl_hwaddr_rule $loop hwaddr)
        if [ $FL_HWA_RULE == $FIXED_HWADDR ]; then
            FL_HWA_NUM=$loop
        fi
        loop=$(($loop+1))
    done
fi

if [ -n "$FL_HWA_NUM" ]; then
    FL_HWA_IP=$(nvram show fl_hwaddr_rule $FL_HWA_NUM ipaddr)
    LAN_STATIC_IP=$(nvram show lan_static_rule 0 ipaddr)
    if [ $(echo $FL_HWA_IP | awk '{FS="."}{print $1 $2 $3}') != $(echo $LAN_STATIC_IP | awk '{FS="."}{print $1 $2 $3}') ]; then
        FL_HWA_IP_CHANGE=$(echo $LAN_STATIC_IP | awk '{FS="."}{print $1 "." $2 "." $3 ".253"}')
        nvram replace attr fl_hwaddr_rule $FL_HWA_NUM ipaddr "$FL_HWA_IP_CHANGE"
        echo "The fixed ip has been changed, please reconnect..."
    else
        echo "The segment of fixed ip and lan_static_rule ip is the same..."
    fi
else
    echo "No rule with hwadd=$FIXED_HWADDR"
fi

lock -u $LOCK_FILE
