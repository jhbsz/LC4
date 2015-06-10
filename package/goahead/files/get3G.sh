#!/bin/sh
#
#
# usage: get3G.sh
#

echo "Run get3G.sh script"

dev_name="$(nvram show wan_wwan_probe_rule 0 devname)"

ctrltty="$(nvram show wan_wwan_probe_rule 0 ctrltty)"

apn="$(nvram show wan_wwan_rule 0 apn)"

env -i EZPCOMAPN=$apn /usr/sbin/ezpcom PIN -d /dev/${dev_name}${ctrltty} > /tmp/SIM.txt

/usr/sbin/ezpcom -d /dev/${dev_name}${ctrltty} info > /tmp/info.txt
sleep 1

/usr/sbin/ezpcom -d /dev/${dev_name}${ctrltty} reg > /tmp/reg.txt

echo "End get3G.sh script"
