#!/bin/sh
echo "Run wifi_unload.sh"
killall wscd
killall syslog-ng
cat /proc/meminfo | grep MemFree
