options { 
	chain_hostnames(off); 
	sync(0); 

	# The default action of syslog-ng 1.6.0 is to log a STATS line
	# to the file every 10 minutes.  That's pretty ugly after a while.
	# Change it to every 12 hours so you get a nice daily update of
	# how many messages syslog-ng missed (0).
	stats(43200); 
};

source src { 
	unix-stream("/dev/log"); 
	internal(); 
	pipe("/proc/kmsg" log_prefix("kernel :")); 
};

filter f_cron_msgs { not match("cron*"); };

destination d_all { program("logwrite -K -k $kid_default$ -s \"&-EZP_IPT&-EZP_USR\" -f -k $kid_iptlog$ -s \"EZP_IPT\"  -f -k $kid_usr$ -c SYS -s \"EZP_USR\"  -b -k $kid_dhcp_serv$ -c DHCPServer -s \"dnsmasq\" -b -k $kid_dhcp_cli$ -c DHCPClient -s \"udhcpc\" -b -k $kid_dns$ -c DNS -s \"ifacedns\" -b -k $kid_upnp$ -c UPnP -s \"upnpd\" -b -k $kid_ntp$ -c NTPClient -s \"ntpclient\" -b -k $kid_ddns$ -c DDNS -s \"ddnsassign\" -f -k $kid_usr$ -c WEB -s \"Web management\" -f -k $kid_sys_warning$ -c SystemWarning -s \"panic\" -f -k $kid_wireless$ -c Wireless -s \"rt2860ap\" -b -k USR -c IPSEC -s \"pluto\" -b -k USR -c IPSEC -s \"ipsec_setup\" -b -k USR -c IPSEC -s \"ipsec__plutorun\" -b -k USR -c IPSEC -s \"klips_info\" -b -k USR -c IPSEC -s \"klips_debug\" -b -k USR -c IPSEC -s \"KLIPS\" -b -k USR -c L2TP -s \"l2tpd\" -b -k USR -c PPTP -s \"pptpd\" -U -k $kid_usr$ -s \"New USB device found,\" -w -k $kid_usr$ -s \"WIFIConnect\" -u -k $kid_usr$ -s \"Product:\" -b -k USR -c \"SYS\" -s \"chat\" -b -k $kid_usr$ -c \"SYS\" -s \"storage[\" "); };
$remote_enable$$tcp_proto$destination d_usr_tcp {  tcp("$remote_ip$" port($remote_port$) );};
$remote_enable$$udp_proto$destination d_usr_udp {  udp("$remote_ip$" port($remote_port$) );};

log { source(src); destination(d_all);};
$remote_enable$$tcp_proto$log { source(src); filter(f_cron_msgs); destination(d_usr_tcp);}; 
$remote_enable$$udp_proto$log { source(src); filter(f_cron_msgs); destination(d_usr_udp);}; 
