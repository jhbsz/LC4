#!/bin/sh
while :; do
i="0"
while [ -z "$(pidof goahead)" ]; do
    # start goahead every 10 second if starting failed
    [ "$i" = "0" ] && /etc/rc.common /etc/init.d/goahead start
    echo "$(sleep 1)"
    i="$($i + 1)"
    [ "$i" = "10" ] && i="0"
done
i="0"
while [ -z "$(pidof wscd)" ]; do
    # start wps every 10 second if starting failed
    [ "$i" = "0" ] && /etc/rc.common /etc/init.d/wps start
    echo "$(sleep 1)"
    i="$($i + 1)"
    [ "$i" = "10" ] && i="0"
done
echo "$(sleep 1)"
done
