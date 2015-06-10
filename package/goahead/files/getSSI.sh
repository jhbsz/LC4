#!/bin/sh
#
#
# usage: getSSI.sh
#

echo "Run getSSI.sh script"

dev_name="$(nvram show wan_wwan_probe_rule 0 devname)"

ctrltty="$(nvram show wan_wwan_probe_rule 0 ctrltty)"

/usr/sbin/ezpcom -d /dev/${dev_name}${ctrltty} sig > /tmp/sig.txt

echo "End getSSI.sh script"
