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
    /sbin/insmod rt2860v2_ap 
}

arch_wifi_control() {
    local idx=0
    local vidx=0
    local wl_num=$(nvram get wl_rule_num)
    local wlv_num=$(nvram get wlv_rule_num)
    local secmode=$(nvram show wl0_sec_rule 0 secmode)
    local enable=0
    if [ "$(nvram show wl_basic_rule 0 enable)" = "0" -o "$wl_num" = "0" ]; then
        iwpriv ra0 set RadioOn=0
        return
    else
        while [ $idx -lt $wl_num ];
        do
            enable=0
            while [ $vidx -lt $wlv_num ];
            do
                enable="$(nvram show wl${idx}_basic_rule $vidx enable)"
                [ "$enable" = "1" ] && break;
                vidx=$(($vidx + 1))
            done
            [ "$enable" = "1" ] && break;
            idx=$(($idx + 1))
        done
        [ "$enable" = "0" ] && iwpriv ra0 set RadioOn=0
    fi
    if [ "$secmode" = "wpa" -o "$secmode" = "wpa2" ]; then
        rt2860apd || err=1
    fi
    [ "$(nvram get brand)" = "AXIM" ] && {
        # check region value at EEPROM 38h
        # this value must be FF, then rt2860 kernel module will read region
        # setting from config file.
        result=$(iwpriv ra0 e2p 38 | awk -F":" '/0038/{printf $2 }')
        [ "$result" != "0xFFFF  " ] && {
            iwpriv ra0 e2p 38=ffff
            nvram fset region_err=1
            nvram commit
            reboot
        }
    }
}

arch_usb_start() {
    insmod dwc_otg
}

arch_usb_stop() {
    echo ""
    # rmmod dwc_otg
}

arch_led_config() {
    local platform=$(nvram get platform)
    [ -f "/dev/gpio" ] || mknod /dev/gpio c 252 0
    case $platform in
        AMA)
            # Reset
            gpioctl dirin 13
            # WPS
            gpioctl dirin 18
        ;;
        AMB)
            gpioctl dirin 13
            gpioctl dirin 10
        ;;
        AMD|AMC|GMA)
            gpioctl dirin 12
            gpioctl dirin 10
            gpioctl dirout 0
        ;;
        AME)
            # USB
            gpioctl dirout 18
            # Reset
            gpioctl dirin 2
            # WPS
            gpioctl dirin 7
        ;;
        BAB)
            gpioctl dirin 0
            gpioctl dirin 10
            gpioctl dirin 17
            gpioctl dirin 18
            gpioctl dirin 19
        ;;
        BAB|BAD|BAD_mac_sense)
            gpioctl dirin 0
            gpioctl dirin 10
            gpioctl dirin 13
        ;;
        ABE|ABE16)
            gpioctl dirin 10
        ;;
        ABA|ABB|ABC)
            gpioctl dirin 0
            gpioctl dirin 10
        ;;
        LIS)
            # Reset
            gpioctl dirin 10
            # MS, SD detect
            gpioctl dirin 11
            gpioctl dirin 12
        ;;
        AXA)
            # Reset
            gpioctl dirin 10
            # WPS
            gpioctl dirin 0
            # SD Card
            gpioctl dirin 14
            # Power switch
            gpioctl dirin 17
            # USB
            gpioctl led 7 0 1 0 0 0
        ;;
        AXB)
            # Reset
            gpioctl dirin 20
            # Power LED
            gpioctl dirin 0
        ;;
        AXM)
            # Reset
            gpioctl dirin 20
            # Power LED
            gpioctl dirout 0
        ;;
        *)
            gpioctl dirin 0
            gpioctl dirin 10
        ;;
    esac
}

arch_led_start() {
    local platform=$(nvram get platform)
    local licinv=$(nvram get license_invalid)
    local wl_enable=$(nvram show wl_basic_rule 0 enable)
    local action=$1
    case "$action" in
        boot)
            case $platform in
                AMA)
                    gpioctl led 8 1 1 1 0 1 > /dev/null
                ;; 
                AMB)
                    gpioctl led 8 1 1 1 0 1 > /dev/null
                ;;
                AMD|AMC)
                    gpioctl led 8 1 1 1 0 1 > /dev/null
                    # enable usb
                    gpioctl led 0 1 0 0 0 0 > /dev/null
				;;
				GMA)
                    gpioctl led 8 1 1 1 0 1 > /dev/null
                    # disable usb
                    gpioctl led 0 0 1 0 0 0 > /dev/null
                ;;
                AME)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
                BAB|BAD|BAD_mac_sense)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;           
                ABA|ABB|ABC|ABE|ABE16)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
                PAB)
                    gpioctl led 7 1 1 1 0 1 > /dev/null
                    # enable usb
                    gpioctl led 9 1 0 0 0 0 > /dev/null
                ;;
                AXA)
                    ezp-i2c gauge host booting 
                ;;
                AXB|AXM)
                    gpioctl led 0 1 1 1 0 1 > /dev/null
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
            esac
        ;;
        wwan)
            case $platform in
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 8 1 1 1 0 1 > /dev/null
                    }
                ;;
                AMB)
                    [ "$licinv" = "0" ] && {
                        gpioctl led 8 1 1 1 0 1 > /dev/null
                    }
                ;;
                AMD|AMC|GMA)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                    [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                ;;           
                ABA|ABB|ABC|ABE|ABE16)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host booting
                ;;
                LIS)
                    echo "none" > /dev/null 
                ;;
                *)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
            esac
        ;;
        storage)
            case $platform in
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 8 1 1 1 0 1 > /dev/null
                    }
                ;;
                AMD|GMA)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                    [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                ;;           
                ABA|ABB|ABC|ABE|ABE16)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host booting 
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
            esac
        ;;
        usbremove)
            case $platform in
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 8 1 1 1 0 1 > /dev/null
                    }
                ;;
                AMB)
                    [ "$licinv" = "0" ] && {
                        gpioctl led 8 1 1 1 0 1 > /dev/null
                    }
                ;;
                AMD|AMC|GMA)
                    gpioctl led 9 1 1 1 0 1 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                    [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                        gpioctl led 14 1 1 1 0 1 > /dev/null
                    }
                ;;           
                ABA|ABB|ABC|ABE|ABE16)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 2
                        } || gpioctl led 9 1 1 1 0 1 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host booting 
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    gpioctl led 14 1 1 1 0 1 > /dev/null
                ;;
            esac
        ;;
        usbunsupport)
            case $platform in
                LIS)
                    iwpriv ra0 set WlanLed=0
                    gpioctl led 13 1 1 1 0 1 > /dev/null
                ;;
            esac
        ;;
        usbhubplug)
            case $platform in
                LIS)
                    iwpriv ra0 set WlanLed=0
                    gpioctl led 13 1 15 1 0 1 > /dev/null
                ;;
            esac
        ;;
        wifi)
            case $platform in
                AMA)
                    [ "$wl_enable" = "1" ] && {
                        gpioctl led 11 1 0 0 0 0 > /dev/null
                    } || {
                        gpioctl led 11 0 1 0 0 0 > /dev/null
                    }
                ;;
                AMB)
                    [ "$wl_enable" = "1" ] && {
                        gpioctl led 9 1 0 0 0 0 > /dev/null
                    } || {
                        gpioctl led 9 0 1 0 0 0 > /dev/null
                    }
                ;;
                AMD|GMA)
                    [ "$wl_enable" = "1" ] && {
                        gpioctl led 7 1 0 0 0 0 > /dev/null
                    } || {
                        gpioctl led 7 0 1 0 0 0 > /dev/null
                    }
                ;;
            esac
        ;;
        factory)
            case $platform in
                AMD|GMA)
                    gpioctl led 8 1 1 0 0 1 > /dev/null
                ;;
                AXA|LIS)
                    gpioctl led 10 1 1 0 0 1 > /dev/null
                ;;
                AXB|AXM)
                    gpioctl led 20 1 1 0 0 1 > /dev/null
                ;;
            esac
        ;;
    esac
}

arch_led_stop() {
    local platform=$(nvram get platform)
    local licinv=$(nvram get license_invalid)
    local wl_enable=$(nvram show wl_basic_rule 0 enable)
    local action=$1
    case "$action" in
        preboot)
            # AXA requires a early boot ready status
            case $platform in
                AXA)
                    ezp-i2c gauge host ready
                ;;
            esac
        ;;
        boot)
            case $platform in
                ABA)
                    if [ "$licinv" -gt "0" ]; then 
                        gpioctl led 9 20 10 1 0 1 > /dev/null
                    else 
                        gpioctl led 9 1 0 0 0 0 > /dev/null
                    fi
                ;;
                AMA)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 8 20 10 1 0 1 > /dev/null
                    else
                        nvram replace attr led_rule 0 boot 0
                        local usb_action=$(nvram show led_rule 0 usb)
                        case "$usb_action" in
                            0)
                                gpioctl led 8 0 1 0 0 0 > /dev/null
                            ;;
                            1)
                                gpioctl led 8 1 0 0 0 0 > /dev/null 
                            ;;
                            2)
                                gpioctl led 8 1 1 1 0 1 > /dev/null
                            ;;
                        esac
                        nvram replace attr led_rule 0 usb 0
                    fi
                ;;
                AMB)
                    if [ "$licinv" = 0 ]; then
                        gpioctl led 8 0 1 0 0 0 > /dev/null
                    else
                        gpioctl led 8 20 10 1 0 1 > /dev/null
                    fi
                ;;  
                AMD|AMC|GMA)
                    if [ "$licinv" = 0 ]; then
                        gpioctl led 8 1 0 0 0 0 > /dev/null
                    else
                        gpioctl led 8 20 10 1 0 1 > /dev/null
                    fi
                ;;
                AME)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 9 20 10 1 0 1 > /dev/null
                    else
                        nvram replace attr led_rule 0 boot 0
                        local usb_action=$(nvram show led_rule 0 usb)
                        case "$usb_action" in
                            0)
                                gpioctl led 9 0 1 0 0 0 > /dev/null
                            ;;
                            1)
                                gpioctl led 9 1 0 0 0 0 > /dev/null 
                            ;;
                            2)
                                gpioctl led 9 1 1 1 0 1 > /dev/null
                            ;;
                        esac
                        nvram replace attr led_rule 0 usb 0
                    fi
                ;;
                BAB|BAD|BAD_mac_sense)
                    if [ "$licinv" = 0 ]; then
                        gpioctl led 14 0 1 0 0 0 > /dev/null
                    else
                        gpioctl led 14 20 10 1 0 1 > /dev/null
                    fi
                ;;
                ABA|ABB|ABC|ABE|ABE16)
                    if [ "$licinv" -gt "0" ]; then 
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    fi
                        gpioctl led 9 1 0 0 0 0 > /dev/null
                ;;
                PAB)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 7 20 10 1 0 1 > /dev/null
                    else
                        nvram replace attr led_rule 0 boot 0
                        local usb_action=$(nvram show led_rule 0 usb)
                        case "$usb_action" in
                            0)
                                gpioctl led 7 0 1 0 0 0 > /dev/null
                                # enable usb
                                gpioctl led 9 1 0 0 0 0 > /dev/null
                            ;;
                            1)
                                gpioctl led 9 0 1 0 0 0 > /dev/null 
                            ;;
                            2)
                                gpioctl led 9 1 1 1 0 1 > /dev/null
                            ;;
                        esac
                    fi
                ;;
                AXA)
                    ezp-i2c gauge host ready
                ;;
                AXB|AXM)
                    gpioctl led 0 1 0 0 0 0 > /dev/null
                    # For Music
                    gpioctl led 18 1 0 0 0 0 > /dev/null
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    if [ "$licinv" -gt "0" ]; then 
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    fi
                        gpioctl led 9 1 0 0 0 0 > /dev/null
                ;;
            esac
        ;;
        wwan)
            case $platform in
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 8 1 0 0 0 0 > /dev/null
                    }
                ;;
                AMB)
                    [ "$licinv" = "0" ] && {
                        gpioctl led 8 1 0 0 0 0 > /dev/null
                    }
                ;;
                AMD|AMC|GMA)
                    gpioctl led 9 1 0 0 0 0 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 9 1 0 0 0 0 > /dev/null
                    }
                ;;
                ABA)
                    gpioctl led 14 1 0 0 0 0 > /dev/null
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    } || {
                        [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                            gpioctl led 14 1 0 0 0 0 > /dev/null
                        } || {
                            gpioctl led 14 20 10 1 0 1 > /dev/null
                        }
                    }
                ;;
                ABA|ABB|ABC|ABE|ABE16)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 1 1 1 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 0 > /dev/null
                    fi
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 9 0 1 0 0 0 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host ready
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 1 1 1 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 0 > /dev/null
                    fi
                ;;
            esac
        ;;
        storage)
            case $platform in
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 8 1 0 0 0 0 > /dev/null
                    }
                ;;
                ABA)
                    gpioctl led 14 1 0 0 1 1 > /dev/null
                ;;
                AMD|GMA)
                    gpioctl led 9 1 0 0 0 0 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 9 1 0 0 0 0 > /dev/null
                    }
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    } || {
                        [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                            gpioctl led 14 1 0 0 0 0 > /dev/null
                        } || {
                            gpioctl led 14 20 10 1 0 1 > /dev/null
                        }
                    }
                ;;
                ABA|ABB|ABC|ABE|ABE16)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 1 1 1 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 20 > /dev/null
                    fi
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 1
                        } || gpioctl led 9 0 1 0 0 0 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host ready 
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 1 1 1 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 20 > /dev/null
                    fi
                ;;
            esac
        ;;
        usbremove)
            case $platform in
                ABA)
                    gpioctl led 14 0 1 0 0 0 > /dev/null
                ;;
                AMA)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 0
                        } || gpioctl led 8 0 1 0 0 0 > /dev/null
                    }
                ;;
                AMB)
                    [ "$licinv" = "0" ] && {
                        gpioctl led 8 0 1 0 0 0 > /dev/null
                    }
                ;;
                AMD|AMC|GMA)
                    gpioctl led 9 0 1 0 0 0 > /dev/null
                ;;
                AME)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 0
                        } || gpioctl led 9 0 1 0 0 0 > /dev/null
                    }
                ;;
                BAB|BAD|BAD_mac_sense)
                    local usbledbev="$(nvram show appmgr_rule 0 usbled)"
                    # 0: usbled is disabled when unauth
                    # 1: usbled is enabled when unauth
                    [ "$usbledbev" = "0" -a "$licinv" = "0" ] && {
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    }
                    [ -z "$usbledbev" -o "$usbledbev" = "1" ] && {
                        [ "$licinv" = "0" ] && {
                            gpioctl led 14 1 0 0 0 0 > /dev/null
                        } || {
                            gpioctl led 14 20 10 1 0 1 > /dev/null
                        }
                    }
                ;;
                ABA|ABB|ABC|ABE|ABE16)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 20 > /dev/null
                    fi
                ;;
                PAB)
                    [ "$licinv" = "0" ] && {
                        [ "$(nvram show led_rule 0 boot)" = "1" ] && {
                            nvram replace attr led_rule 0 usb 0
                        } || gpioctl led 9 1 0 0 0 0 > /dev/null
                        gpioctl led 7 0 1 0 0 0 > /dev/null
                    }
                ;;
                AXA)
                    ezp-i2c gauge host ready 
                ;;
                LIS)
                    echo "none" > /dev/null
                ;;
                *)
                    if [ "$licinv" -gt "0" ]; then  
                        gpioctl led 14 1 0 0 0 0 > /dev/null
                    else
                        gpioctl led 14 1 1 1 1 20 > /dev/null
                    fi
                ;;
            esac
        ;;
        usbunsupport)
            case $platform in
                LIS)
                    gpioctl led 13 0 1 0 0 1 > /dev/null
                    iwpriv ra0 set WlanLed=1 > /dev/null
                ;;
            esac
        ;;
        usbhubplug)
            case $platform in
                LIS)
                    gpioctl led 13 0 1 0 0 1 > /dev/null
                    iwpriv ra0 set WlanLed=1 > /dev/null
                ;;
            esac
        ;;
    esac
}

arch_uboot_update(){
    UBOOT_NEW="/etc/uboot/uboot"
    correct_version=$1
    version=$(grep -r U-Boot /dev/mtd1 | grep -r \(|sed 's/ //g' |sed -n '1p')

    [ -f "${UBOOT_NEW}" -a "$version" != "$correct_version" ] && {
        # Update uboot
        mtd write ${UBOOT_NEW} Bootloader
        reboot
    }
}

arch_initd_AMA_phy(){

    # Fixed Phy 100M mode.
    switch mii w 0 0 1000 3000

    # Power down the PHY if we don't need it!
    # Power down Phy1~Phy5
    switch mii w 1 0 800 800
    switch mii w 2 0 800 800
    switch mii w 3 0 800 800
    switch mii w 4 0 800 800
    switch mii w 5 0 800 800
}

arch_initd_BAB_phy(){

    # Fixed Phy 100M mode.
    switch mii w 0 0 1000 3000
    switch mii w 1 0 1000 3000

    # Power down the PHY if we don't need it!
    # Power down Phy2~Phy5
    switch mii w 2 0 800 800
    switch mii w 3 0 800 800
    switch mii w 4 0 800 800
    switch mii w 5 0 800 800
}

arch_initd_AMD_phy(){
    # Power down the PHY if we don't need it!
    # Power down Phy0~Phy4
    switch mii w 0 0 800 800
    switch mii w 1 0 800 800
    switch mii w 2 0 800 800
    switch mii w 3 0 800 800
    switch mii w 4 0 800 800
}

arch_initd_AMD_port_learning(){
# stop port learning
    local i=0
    while [ $i -lt 5 ]; do
        [ "$(nvram show vlanport_rule $i pvid)" != "1" ] && switch greg w 0x$(($i+2))04 0x2104 
        i=$(($i+1))       
    done
    switch greg w 0x54 0x0
    switch greg w 0x58 0x0
    switch greg w 0x50 0x9 #flush all dynamic entries in the Address Table
}

arch_initd_AXA_phy(){

    # Fixed Phy 100M mode.
    switch mii w 4 0 1000 3000

    # Power down the PHY if we don't need it!
    # Power down Phy0~Phy3 and Phy5
    switch mii w 0 0 800 800
    switch mii w 1 0 800 800
    switch mii w 2 0 800 800
    switch mii w 3 0 800 800
    switch mii w 5 0 800 800
}

arch_initd_LIS_phy(){

    # Power down the PHY if we don't need it!
    # Power down Phy0~Phy5
    switch mii w 0 0 800 800
    switch mii w 1 0 800 800
    switch mii w 2 0 800 800
    switch mii w 3 0 800 800
    switch mii w 4 0 800 800
    switch mii w 5 0 800 800
}

arch_initd_default_countdown(){
    nvram replace attr countdown_rule 0 w_count $(nvram show countdown_rule 0 w_countdown)    
    nvram replace attr countdown_rule 0 p_count $(nvram show countdown_rule 0 p_countdown)    
}

arch_initd_boot_start(){
    platform=$(nvram get platform)
    case $platform in
        AMA)
            correct_version="U-Boot1.1.3(Dec102010-19:00:24)"
            arch_uboot_update "$correct_version"
        ;;
        AMD|GMA)
            correct_version="U-Boot1.1.3(Mar32011-17:38:17)"
            arch_uboot_update "$correct_version"
        ;;
        ABE16)
            correct_version="U-Boot1.1.3(Jun242011-18:20:39)"
            arch_uboot_update "$correct_version"
        ;;
        BAD|BAD_mac_sense)
            correct_version="U-Boot1.1.3(Oct132011-16:24:45)"
            arch_uboot_update "$correct_version"
        ;;
        AXM)
            correct_version="U-Boot3.5.2.0.A000MUSIC(Apr102013-18:40:56)"
            arch_uboot_update "$correct_version"
        ;;

    esac
}

arch_initd_done_start(){
    platform=$(nvram get platform)
    case $platform in
        AMA|AME|PAB)
            arch_initd_AMA_phy
        ;;
        BAB|AXB)
            arch_initd_BAB_phy
        ;;
        AMD|GMA)
            arch_initd_AMD_phy
        ;;
        AXA)
            arch_initd_AXA_phy
            arch_initd_default_countdown
            echo "0" > /sys/devices/platform/rt3xxx-ehci/usb1/1-1/power/autosuspend
            echo "auto" > /sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/power/level
            echo "auto" > /sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.2/power/level
        ;;
        AXM)
            arch_initd_LIS_phy
            arch_initd_default_countdown
        ;;
        LIS)
            arch_initd_LIS_phy
            arch_initd_default_countdown
            echo "auto" > /sys/devices/platform/rt3xxx-ehci/usb1/1-1/1-1.1/power/level
        ;;
    esac
    [ "$(nvram get switch_type)" = "ar8316" ] && {
        [ "$platform" = "AMD" -o "$platform" = "GMA" ] && {
            arch_initd_AMD_port_learning
        }
    }
}

arch_storage_insert_state() {
    local readernum="$(nvram get card_reader_hardware_rule_num)"
    local idx=0
    local gpionum
    local cardinserted="0"

    while [ "$idx" -lt "$readernum" ]; do
        gpionum="$(nvram show card_reader_hardware_rule $idx gpio)"
        [ -n "$gpionum" -a -n "$(gpioctl get $gpionum | grep 'is 0')" ] && {
            cardinserted="1"
            break
        }
        idx="$(($idx + 1))"
    done
    nvram replace attr storage_state_rule 1 insert_state "$cardinserted"
}
