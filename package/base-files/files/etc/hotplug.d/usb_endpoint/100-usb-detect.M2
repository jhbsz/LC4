#!/bin/sh
. /etc/arch.sh

local platform="$(nvram get platform)"
LOCKFILE="/tmp/usb.caution.lock"

[ "$platform" != "LIS" ] && {
    exit
}

case "$ACTION" in
    "add")
        if [ "$DRIVER" = "usb-storage" ]; then
            echo "get usb storage" >> /dev/null
        elif [ "$DRIVER" = "usb" ]; then
            echo "get usb" >> /dev/null
        elif [ "$DRIVER" = "hub" ]; then
            [ "$(echo "$(cat /proc/bus/usb/devices)" | grep -c "Driver=hub")" -gt "3" ] && {
                 arch_led_start usbhubplug
            }

        else
            USB_PATH="$(echo $DEVPATH | awk 'FS="/" {print $8}')"
            if (set -o noclobber; echo "$USB_PATH" > "$LOCKFILE") 2> /dev/null; then
                echo "unsupported device" >> /dev/null
                #arch_led_start usbunsupport
            else
                echo "been locked" >> /dev/null
            fi

        fi
    ;;
    "remove")
        if [ "$DRIVER" = "usb-storage" ]; then
            echo "remove usb storage" >> /dev/null
        elif [ "$DRIVER" = "usb" ]; then
            echo "remove usb" >> /dev/null
        else
            [ "$(echo "$(cat /proc/bus/usb/devices)" | grep -c "Driver=hub")" -gt "3" ] || {
                arch_led_stop usbhubplug
            }
        fi
    ;;
esac
