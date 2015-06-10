#!/bin/sh
. /etc/arch.sh

local platform="$(nvram get platform)"
[ "$platform" = "AXA" ] && {
    local upaction=0
    local downaction=1
    local USB_PATH="1-1[.]1"
    [ "$platform" = "LIS" ] && {
        USB_PATH="1-1.2"
    }
    [ "$1" = "turn_up_mounting_led" ] && upaction=1
    # USB case
    [ -n "$(echo $PHYSDEVPATH | grep $USB_PATH)" ] && {
        arch_led_start storage
    } || {
    # SD card case
        local devnode="$(nvram show card_reader_hardware_rule 0 dev)"
        [ "$upaction" = "1" ] && arch_led_start storage || {
            [ -f "/dev/$devnode" -a -z "$(sdparm --command=ready /dev/$devnode | grep Not)" ] && arch_led_start storage || {
                [ -f "/dev/$devnode" -a -z "$(sdparm --command=ready /dev/$devnode | grep Not)" ] && arch_led_start storage
            }
        }
    }
    # [ "$upaction" = "1" ] && arch_led_start storage
    sleep $2
    arch_led_stop storage
}

