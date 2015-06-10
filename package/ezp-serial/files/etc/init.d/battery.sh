#!/bin/sh

local P1="$(gpioctl get 25| sed -n '2p'|awk '{print $4}')"
local P2="$(gpioctl get 26| sed -n '2p'|awk '{print $4}')"
local P3="$(gpioctl get 68| sed -n '2p'|awk '{print $4}')"
local PLATFORM="$(nvram get platform)"
local BATTERY="$(nvram get als_battery_rule)"
local ALERT="$(nvram show alert_rule 0 battery)"
local POINT=0

[ $PLATFORM != "ALS" ] && exit

[ $P1 = "1" -a $P2 = "1" -a $P3 = "1" ] && { 
    POINT=1
    nvram fset als_battery_rule="1"
    [ $ALERT != "1" ] && {
        nvram replace attr alert_rule 0 battery 1
    }
}
[ $P2 = "1" -a $P3 = "1" -a $P1 = "0" ] && { 
    POINT=2
    nvram fset als_battery_rule="2"
    [ $ALERT != "2" ] && {
        nvram replace attr alert_rule 0 battery 2
    }
} 

[ $POINT = 0 ] && { 
    nvram fset als_battery_rule="0"
    [ $ALERT != "0" ] && {
        nvram replace attr alert_rule 0 battery 0
    }
}
[ "$(nvram show mail_rule 0 battery)" = "1" -a $ALERT != "$(nvram show alert_rule 0 battery)" ] && {
    /bin/sh /etc/init.d/send_mail battery $POINT $ALERT
}
nvram commit
