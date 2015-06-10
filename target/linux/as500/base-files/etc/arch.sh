#!/bin/sh

arch_switch_start() {

    # Turn on the port-based VLAN (?)
    switch reg w 14 405555

    # VLAN group => VID (tag identifier)
    switch reg w 50 2001
    switch reg w 54 4003

    # Enabled remove tag from port 0 to port 6 (port 7 is for host)
    switch reg w 98 7f3f
    
    [ "$(nvram get wan_num)" = "1" ] && {
        # LLLLW (single WAN)
        switch reg w 40 1001
        switch reg w 44 1001
        switch reg w 48 1002
        switch reg w 70 ffff506f

        # elif [ "$1" = "WLLLL" ]; then
        #    switch reg w 40 1002
        #    switch reg w 44 1001
        #    switch reg w 48 1001
        #    switch reg w 70 ffff417e
        # elif [ "$1" = "GW" ]; then
        #    switch reg w 40 1001
        #    switch reg w 44 1001
        #    switch reg w 48 2001
        #    switch reg w 70 ffff605f
    }

    [ "$(nvram get wan_num)" = "2" ] && {
        # LLLWW (two WANs)
        # Port-based VID
        switch reg w 40 1001
        switch reg w 44 3001
        switch reg w 48 1002
  
        # VLAN 2 group (VID 3)   100 1000
        # VLAN 1 group (VID 2)   101 0000
        # VLAN 0 group (VID 1)   100 0111
        switch reg w 70 ff485047
    }
}

arch_wifi_start() {
    # Get rt2860 ready. 
    /etc/wl/rt2860ap-init || err=1
    /sbin/insmod rt2860v2_ap && {
    if ( nvram show wl0_sec_rule 0 secmode | grep -q '\<\(wpa\|psk\)2\?\>' ); then {
        rt2860apd || err=1
        } fi
    }
}

arch_usb_start() {
    insmod dwc_otg
}

arch_usb_stop() {
    echo ""
    # rmmod dwc_otg
}

arch_led_boot_start() {
    echo ""
#    [ -f /dev/gpio ] || mknod /dev/gpio c 252 0
#    gpioctl dirin 0
#    gpioctl dirin 10
#    gpioctl led 9 1 1 1 0 1
}

arch_led_boot_stop() {
    echo ""
#    gpioctl led 9 1 0 1 1 1
}

arch_led_usb_start() {
    echo ""
#    gpioctl led 14 1 1 1 0 1
}

arch_led_usb_stop() {
    echo ""
#    gpioctl led 14 1 1 1 1 20
}

arch_led_licinv_start() {
    echo ""
#    gpioctl led 14 1 0 1 1 1
}

