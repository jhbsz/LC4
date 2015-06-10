#ACTION is set {
#   printdebug
#}

# For some reasons, 
ACTION == remove, DRIVER ~~ (option*|sierra*|rndis_host*|sierra_net*|cdc_ether*|ezp_iphone*|cdc_acm*|usbbcm*) {
    exec /usr/sbin/nvram fset wan_upnp_status_rule=1^down^;
    exec /usr/sbin/nvram default wan_wwan_probe_rule;
    exec /usr/sbin/nvram replace attr plug_rule 0 usb 0;
}

$include /etc/hotplug2-common.rules

SUBSYSTEM ~~ (net|button|usb|block|scsi_generic|sound) {
        exec /sbin/hotplug-call %SUBSYSTEM%;
}

ACTION == add, DRIVER ~~ (usb-storage) {
    exec /etc/led_control.sh turn_down_mounting_led 35 &;
        next
}

ACTION == add, DRIVER ~~ (option|sierra|cdc_acm) {
    exec /usr/sbin/nvram replace attr plug_rule 0 usb 1;
	next
}

ACTION == add, DRIVER ~~ (rndis_host) {
    exec /usr/sbin/nvram replace attr plug_rule 0 usb 1;
	next
}

ACTION == add, DRIVER ~~ (sierra_net|cdc_ether|Gobi*) {
    exec /usr/sbin/nvram replace attr plug_rule 0 usb 1;
	next
}

ACTION == add, DRIVER ~~ (usbbcm) {
    exec /usr/sbin/nvram fset wan_upnp_status_rule=1^up^beceem;
    next
}

ACTION == add, DRIVER ~~ (ezp_iphone) {
    exec /usr/sbin/nvram replace attr plug_rule 0 usb 1;
    next
}

DEVICENAME ~~ (watchdog) {
	exec /sbin/watchdog -t 5 /dev/watchdog;
	next
}
