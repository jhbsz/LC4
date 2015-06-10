#!/bin/sh

do_verify_upgrade() {
  local fixed_version=$1
  local device_ip=$2
  local device_port=$3
  local fw_version=""
  local up_result=1
  local trycount=0
  while [ "$trycount" -lt 10 ]; do
    rm test_upgrade
    [ "$device_port" = "80" ] && {
      curl -m 10 -s -u admin:admin -w "%{http_code}" -b "auth=YWRtaW46YWRtaW4%3D" http://$device_ip/st_info.asp -o /tmp/test_upgrade
    } || {
      curl -m 10 -s -u admin:admin -w "%{http_code}" -b "auth=YWRtaW46YWRtaW4%3D" http://$device_ip:$device_port/st_info.asp -o /tmp/test_upgrade
    }
    fw_version="$(cat /tmp/test_upgrade | grep "<td>Version</td>")"
    echo "$fw_version" >> /tmp/123
    [ -n "$fw_version" ] && break
    trycount=$(($trycount + 1))
    sleep 10
  done
  [ -n "$(echo $fw_version | grep $fixed_version)" ] && up_result=0
  [ "$up_result" != "0" ] && echo "My version $fw_version, upgrade machine firmware version $fixed_version" >> /tmp/mnt/SD/upgrade_log.txt
  return $up_result
}

show_word_and_check() {
  local showstr="$1"
  local checkpolicy="$2"
  local timeout=""
  local checkpingip=""
  local checknvram=""
  local loopcount="0"
  case "$checkpolicy" in
  "-w")
    timeout="$3"
  ;;
  "-p")
    checkpingip="$3"
  ;;
  "-n")
    checknvram="$3"
    nvram fset "$checknvram=running"
  ;;
  esac
  
  while [ true ]; do
    nvram replace attr wl0_ssid_rule 0 ssid "$showstr"
    ezp-i2c wifi ssid
    # break when we can not get device, ie:unpluged
    [ -z "$checkpingip" -a -z "$timeout" -a -z "$checknvram" ] && break
    [ -n "$checkpingip" -a "$(ping -w 1 -c 1 $checkpingip 2>/dev/null 1>/dev/null; echo $?)" != "0"  ] && break
    [ -n "$timeout" -a "$loopcount" -gt "$timeout" ] && break
    [ -n "$checknvram" -a -z "$(nvram get $checknvram)" ] && break
    loopcount=$(($loopcount + 6))
    sleep 6
  done
}

[ -z "$(nvram get upgrade_fw_version)" ] && {
  nvram fset upgrade_port="80"
  nvram fset upgrade_fw_version="3.1.0-A16"
  nvram fset upgrade_fw_version_fn="310-a16"
  nvram fset upgrade_brand="axim"
  nvram fset upgrade_model="m2"
  nvram fset upgrade_mac_filter="54:d0:ed:"
  nvram fset upgrade_stop="0"
}
nvram fset lan_static_rule="192.168.120.1^24^^192.168.120.1^"
nvram fset lan_static_rule_default="192.168.120.1^24^^192.168.120.1^"
nvram fset port_config="lan"

while [ "$(nvram get upgrade_stop)" = "0" ]; do
devmac="$(nvram get upgrade_mac)"
devip="$(nvram get upgrade_ip)"
devport="$(nvram get upgrade_port)"
fwversion="$(nvram get upgrade_fw_version)"
fwvername="$(nvram get upgrade_fw_version_fn)"
macfilter="$(nvram get upgrade_mac_filter)"
brand="$(nvram get upgrade_brand)"
product="$(nvram get upgrade_model)"
fwfn="/tmp/mnt/SD/$brand-$product-$fwvername.bin"
[ -z "$devmac" -o -z "$(echo $devmac | grep "$macfilter")" ] && {
  echo "Not a QGenie device"
  show_word_and_check "Waiting..."
  sleep 2
  continue
}

[ -z "$devip" -o "$(ping -w 1 -c 1 $devip 2>/dev/null 1>/dev/null; echo $?)" != "0" ] && {
  echo "Can not get $devip, leave"
  show_word_and_check "Waiting..."
  sleep 2
  continue
}

do_verify_upgrade "$fwversion" "$devip" "$devport"
verifyresult=$?
if [ "$verifyresult" = "0" ]; then
  echo "Firmware version $fwversion is correct, leave"
  show_word_and_check "$fwversion OK" 
  sleep 2
  continue
else
  echo "Doing upgrade"
  show_word_and_check "Upgrading..." -n "upgrade_display" &
  echo "Will upgrade device mac:$devmac ip:$devip" >> /tmp/mnt/SD/upgrade_log.txt
  http_code="$(curl -s -m 150 -w "%{http_code}" -u admin:admin -b "auth=YWRtaW46YWRtaW4%3D" -F binfile=@$fwfn http://$devip/upgrade.cgi)"
  [ -n "$(echo $http_code | grep "upgraded successfully")" ] && {
    echo "Upgrade success, reboot" >> /tmp/mnt/SD/upgrade_log.txt
    nvram unset upgrade_display
    sleep 2
    continue
  } || {
    nvram unset upgrade_display
    sleep 2
    echo "Upgrade failed, reboot" >> /tmp/mnt/SD/upgrade_log.txt
    show_word_and_check "Failed..."
    sleep 2
    show_word_and_check "Failed..."
    sleep 2
    show_word_and_check "Failed..."
    sleep 2
    continue
  }
fi
done
