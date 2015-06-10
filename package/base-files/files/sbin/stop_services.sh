#!/bin/sh
. /etc/arch.sh
service_list_file=/usr/share/service_list.txt
cron_new_file=/usr/share/cron_upgrade

# Stop block related services
# ACTION=remove /sbin/hotplug-call block

# Stop watchdog to prevent system hang
kill -9 $(pidof watchdog)
/usr/sbin/watchdog -c /etc/watchdog.upgrade
# Stop services to reserve memory
for servicename in $(cat $service_list_file); do
  [ -n "$(pidof $servicename)" ] && {
    echo "stop $servicename with pid $(pidof $servicename)"
    kill -9 $(pidof $servicename)
  }
done
service_list="pptpd xl2tpd ipsec vncrepeater pptp"
for stopserv in $service_list; do
    echo "/etc/rc.common /etc/init.d/$stopserv stop"
    /etc/rc.common /etc/init.d/$stopserv stop
done

# Prevent periodic task from running
remove_file_list="/sbin/server-detect /etc/init.d/ddns /etc/init.d/aximddns /sbin/ddns-detect /etc/init.d/ntpclient"
for remove_file in $remove_file_list; do
    echo "rm $remove_file"
    rm $remove_file
done

# Remove unnecessary modules
for wtim in 1 2 3; do
for modfile in $(lsmod | awk '{print $1}'); do
    echo "rmmod $modfile"
    rmmod $modfile
done
done

# Reserve memories
echo 1 > /proc/sys/vm/drop_caches
echo 1000 > /proc/sys/net/ipv4/netfilter/ip_conntrack_max
echo 0 > /proc/sys/net/ipv4/ip_forward

arch_led_start boot
# umount_drive_list="/tmp/mnt/SD /tmp/mnt/USB"
# for umount_drive in $umount_drive_list; do
#     echo "umount -l $umount_drive"
#     umount -l $umount_drive
# done

