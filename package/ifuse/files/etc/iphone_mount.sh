#!/bin/sh

. /etc/arch.sh

restart_usbmuxd() {
  sleep 3
  kill -9 $(pidof usbmuxd)
  usbmuxd -u
  sleep 3
}

[ -e "/tmp/iphone_mount.pid" ] && exit
echo $$ > /tmp/iphone_mount.pid
arch_led_start wwan
local iphone_line="$(cat /proc/bus/usb/devices | grep -n  iPhone | cut -f1 -d:)"
serial_num_line=$(($iphone_line + 1))
serial_num="$(sed -n "$serial_num_line"p /proc/bus/usb/devices | cut -f2 -d=)"

count=0
usbmuxd -u
# restart_usbmuxd
sleep 5
arch_led_stop wwan

while [ true ]; do
  rootca_status=1
  [ -z "$(cat /var/lib/lockdown/$serial_num.plist | grep RootCertificate)" ] && rootca_status=0
  wifimac_status=1
  [ -z "$(cat /var/lib/lockdown/$serial_num.plist | grep WiFiMACAddress)" ] && wifimac_status=0
  eth_status=1
  [ -z "$(ifconfig eth0 2>&-)" ] && eth_status=0
  ifuse_status=1
  [ -z "$(mount | grep ifuse)" ] && ifuse_status=0
  iph_status=0
  # Assume status : rootca -> wifimac -> ifuse -> eth
  case "$eth_status$wifimac_status$ifuse_status$rootca_status" in
  "0000"|"1000")
  # Wait rootCA
    [ "$count" = "150" ] && {
      return
    }
    sleep 1
    count=$(($count + 1))
  ;;
  "0001"|"0010"|"0011"|"0100"|"0101"|"0110"|"0111"|"1001")
  # Restart usbmuxd
    restart_usbmuxd
  ;;
  "1100"|"1101")
  # mount ifuse
    ifuse /tmp/netdevdrv/
    sleep 3
  ;;
  "1111"|"1110"|"1011"|"1010")
  # mount success
      WAN_NUM=$(nvram get wan_num)
      for i in $(seq 0 $(($WAN_NUM - 1)))
      do
          [ "$(nvram get wan${i}_proto)" = "iphone" ] && {
              [ "$(nvram show wan_status_rule $i state)" -lt "2" ] && {
                ifdown wan${i}
                ifup wan${i}
              } || {
                iph_status=1
              }
              break
          }
      done
      sleep 2
  ;;
  esac
  # We leave until we get ip
  [ "$eth_status" = "1" -a "$iph_status" = "1" ] && {
    arch_led_stop wwan
    break
  }
done

