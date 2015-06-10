#!/bin/sh
nvram fset upgrade_port="80"
nvram fset upgrade_fw_version="3.1.0-A16"
nvram fset upgrade_fw_version_fn="310-a16"
nvram fset upgrade_brand="axim"
nvram fset upgrade_model="m2"
nvram fset upgrade_mac_filter="54:d0:ed:"
nvram fset upgrade_stop="0"
nvram fset lan_static_rule="192.168.120.1^24^^192.168.120.1^"
nvram fset lan_static_rule_default="192.168.120.1^24^^192.168.120.1^"
nvram fset port_config="lan"
nvram commit
reboot
