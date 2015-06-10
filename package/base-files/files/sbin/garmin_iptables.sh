#!/bin/sh
#   Program:
#       This program used to modified the features as following
#           1. Setup multicast group via 'smcroute'
#           2. Create SNAT & DNAT rules for GARMIN
#           3. Create FORWARD rules for filter
#

CLAN_INTF="br0"
CLAN_SEG="$(nvram show lan_static_rule 0 ipaddr)/$(nvram show lan_static_rule 0 mask)"

GMN_INTF="vlan3"
GMN_SEG="172.16.0.0/16"

MCAST_SEG="224.0.0.0/4"

GMN_MCAST_FILTER_NAME="GMN_MCAST_FILTER"
GMN_TCP_FILTER_NAME="GMN_TCP_FILTER"
GMN_UDP_FILTER_NAME="GMN_UDP_FILTER"
GMN_SNAT_NAME="GMN_SNAT"
GMN_DNAT_NAME="GMN_DNAT"

GMN_DROP_FILTER_NAME="GMN_DROP"

GMN_IPT_LOCK="/tmp/.garmin_iptables.lock"

SMCRouteRuleEdit()
{
    if [ $# -eq 5 ]; then
        if [ $1 = "add" ]; then
            echo "Adding a multicast routing rule..."
                smcroute -a $2 $3 $5 $4
        elif [ $1 = "rem" ]; then
            echo "Removing a multicast routing rule..."
                smcroute -r $2 $3 $5
        else
            echo "Invalid parameters. EX: SMCRouteRuleEdit <add/rem> <orig_intf> <orig_ip> <mcast_intf> <mcast_group>"
        fi
    else
        echo "Invalid parameters. EX: SMCRouteRuleEdit <add/rem> <orig_intf> <orig_ip> <mcast_intf> <mcast_group>"
    fi
}

SMCRouteRuleCreate()
{
    smcroute -k
    sleep 1
    if [ -n "$(pidof smcroute)" ]; then
        echo "Killing the smcroute process..."
        kill -9 $(pidof smcroute)
    fi
    smcroute -d
    if [ -n "$(pidof smcroute)" ]; then
        echo "smcroute has running..."

        local MCAST_TOTAL=$(nvram show gmn_mcast_rule_num 0)
        if [ $MCAST_TOTAL -gt 0 ]; then
            local loop=0
            while [ $loop -lt $MCAST_TOTAL ]
            do
                local ORIG_INTF=$(nvram show gmn_mcast_rule $loop orig_intf)
                local ORIG_IP=$(nvram show gmn_mcast_rule $loop orig_ip)
                local MCAST_INTF=$(nvram show gmn_mcast_rule $loop mcast_intf)
                local MCAST_GROUP=$(nvram show gmn_mcast_rule $loop mcast_group)
                
                SMCRouteRuleEdit add $ORIG_INTF $ORIG_IP $MCAST_INTF $MCAST_GROUP
                loop=$(($loop+1))
            done
        else
            echo "There is not any rule for multicast..."
        fi
    else
        echo "smcroute has not running..."
    fi
}

NatRulesEdit()
{
    if [ $# -eq 5 ]; then
        if [ $1 == "add" ]; then
            echo "Adding NAT rule..."
            iptables -t nat -I $GMN_SNAT_NAME -o $4 -s $3 -j SNAT --to $5
            iptables -t nat -I $GMN_DNAT_NAME -i $4 -d $5 -j DNAT --to $3
        elif [ $1 == "rem" ]; then
            echo "Removing NAT rule..."
            iptables -t nat -D $GMN_SNAT_NAME -o $4 -s $3 -j SNAT --to $5
            iptables -t nat -D $GMN_DNAT_NAME -i $4 -d $5 -j DNAT --to $3
        else
            echo "Invalid parameters. EX: NatRulesEdit <add/rem> <gmn_ip> <clan_ip>"
        fi
    else
        echo "Invalid parameters. EX: NatRulesEdit <add/rem> <gmn_ip> <clan_ip>"
    fi
}

NatRulesCreate()
{
    ##  Check chain and add to POSTROUTING for SNAT  ##
    iptables -t nat -nvL | grep "$GMN_SNAT_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_SNAT_NAME has created, now flushing it..."
        iptables -t nat -F $GMN_SNAT_NAME
    else
        echo "$GMN_SNAT_NAME has not be create!"
        iptables -t nat -N $GMN_SNAT_NAME
    fi

    iptables -t nat -nvL POSTROUTING | grep "$GMN_SNAT_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_SNAT_NAME in POSTROUTING has added."
    else
        echo "Adding $GMN_SNAT_NAME to POSTROUTING"
        iptables -t nat -I POSTROUTING -j $GMN_SNAT_NAME
    fi

    ##  Check chain and add to PREROUTING for DNAT  ##
    iptables -t nat -nvL | grep "$GMN_DNAT_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_DNAT_NAME has created, now flushing it..."
        iptables -t nat -F $GMN_DNAT_NAME
    else
        echo "$GMN_DNAT_NAME has not be create!"
        iptables -t nat -N $GMN_DNAT_NAME
    fi

    iptables -t nat -nvL PREROUTING | grep "$GMN_DNAT_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_DNAT_NAME in PREROUTING has added."
    else
        echo "Adding $GMN_DNAT_NAME to PREROUTING"
        iptables -t nat -I PREROUTING -j $GMN_DNAT_NAME
    fi

    local NAT_TOTAL=$(nvram show gmn_nat_rule_num 0)
    local loop=0
    while [ $loop -lt $NAT_TOTAL ]
    do
        local ORIG_INTF=$(nvram show gmn_nat_rule $loop orig_intf)
        local ORIG_IP=$(nvram show gmn_nat_rule $loop orig_ip)
        local TARG_INTF=$(nvram show gmn_nat_rule $loop targ_intf)
        local TARG_IP=$(nvram show gmn_nat_rule $loop targ_ip)
        NatRulesEdit add $ORIG_INTF $ORIG_IP $TARG_INTF $TARG_IP
        loop=$(($loop+1))
    done
}

FilterRulesEdit()
{
    if [ $# -eq 6 ]; then
        if [ $1 = "tcp" ]; then
            if [ $2 = "add" ]; then
                echo "Adding filter rule for tcp..."
                iptables -t filter -I $GMN_TCP_FILTER_NAME -i $3 -o $4 -p tcp --dport $5:$6 -j ACCEPT
            elif [ $2 = "rem" ]; then
                echo "Removing filter rule for tcp..."
                iptables -t filter -D $GMN_TCP_FILTER_NAME -i $3 -o $4 -p tcp --dport $5:$6 -j ACCEPT
            else
                echo "Invalid parameters. EX: FilterRulesEdit <multicast/tcp/udp> <add/rem> <port_from> <port_to>"
            fi
        elif [ $1 = "udp" ]; then
            if [ $2 = "add" ]; then
                echo "Adding filter rule for udp..."
                iptables -t filter -I $GMN_UDP_FILTER_NAME -i $3 -o $4 -p udp --dport $5:$6 -j ACCEPT
            elif [ $2 = "rem" ]; then
                echo "Removing filter rule for udp..."
                iptables -t filter -D $GMN_UDP_FILTER_NAME -i $3 -o $4 -p udp --dport $5:$6 -j ACCEPT
            else
                echo "Invalid parameters. EX: FilterRulesEdit <multicast/tcp/udp> <add/rem> <port_from> <port_to>"
            fi
        else
            echo "Invalid parameters. EX: FilterRulesEdit <multicast/tcp/udp> <add/rem> <port_from> <port_to>"
        fi
    else
        echo "Invalid parameters. EX: FilterRulesEdit <add/rem> <port_from> <port_to>"
    fi
}

FilterMCastCreate()
{
    iptables -t filter -nvL | grep "$GMN_MCAST_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_MCAST_FILTER_NAME has been created, now flushing it..."
        iptables -t filter -F $GMN_MCAST_FILTER_NAME
    else
        echo "$GMN_MCAST_FILTER_NAME has not be create!"
        iptables -t filter -N $GMN_MCAST_FILTER_NAME
    fi

    iptables -t filter -nvL FORWARD | grep "$GMN_MCAST_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_MCAST_FILTER_NAME in FORWARD has been added"
    else
        echo "Adding $GMN_MCAST_FILTER_NAME to FORWARD"
        iptables -t filter -I FORWARD -d $MCAST_SEG -p udp -j $GMN_MCAST_FILTER_NAME
    fi

    local FILTER_TOTAL=$(nvram show gmn_filter_rule_num 0)
    local loop=0
    while [ $loop -lt $FILTER_TOTAL ]
    do
        local SRC_INTF=$(nvram show gmn_filter_rule $loop src_intf)
        local DST_INTF=$(nvram show gmn_filter_rule $loop dst_intf)
        local PORT_FROM=$(nvram show gmn_filter_rule $loop port_from)
        local PORT_TO=$(nvram show gmn_filter_rule $loop port_to)
        FilterRulesEdit multicast add $SRC_INTF $DST_INTF $PORT_FROM $PORT_TO
        loop=$(($loop+1))
    done
}

FilterTCPCreate()
{
    iptables -t filter -nvL | grep "$GMN_TCP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_TCP_FILTER_NAME has been created, now flushing it..."
        iptables -t filter -F $GMN_TCP_FILTER_NAME
    else
        echo "$GMN_TCP_FILTER_NAME has not be create!"
        iptables -t filter -N $GMN_TCP_FILTER_NAME
    fi

    iptables -t filter -nvL FORWARD | grep "$GMN_TCP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_TCP_FILTER_NAME in FORWARD has been added"
    else
        echo "Adding $GMN_TCP_FILTER_NAME to FORWARD"
        iptables -t filter -I FORWARD -p tcp -j $GMN_TCP_FILTER_NAME
    fi

    local FILTER_TOTAL=$(nvram show gmn_filter_rule_num 0)
    local loop=0
    while [ $loop -lt $FILTER_TOTAL ]
    do
        local SRC_INTF=$(nvram show gmn_filter_rule $loop src_intf)
        local DST_INTF=$(nvram show gmn_filter_rule $loop dst_intf)
        local PORT_FROM=$(nvram show gmn_filter_rule $loop port_from)
        local PORT_TO=$(nvram show gmn_filter_rule $loop port_to)
        FilterRulesEdit tcp add $SRC_INTF $DST_INTF $PORT_FROM $PORT_TO
        loop=$(($loop+1))
    done
}

FilterUDPCreate()
{
    iptables -t filter -nvL | grep "$GMN_UDP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_UDP_FILTER_NAME has been created, now flushing it..."
        iptables -t filter -F $GMN_UDP_FILTER_NAME
    else
        echo "$GMN_UDP_FILTER_NAME has not be create!"
        iptables -t filter -N $GMN_UDP_FILTER_NAME
    fi

    iptables -t filter -nvL FORWARD | grep "$GMN_UDP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_UDP_FILTER_NAME in FORWARD has been added"
    else
        echo "Adding $GMN_UDP_FILTER_NAME to FORWARD"
        iptables -t filter -I FORWARD -p udp -j $GMN_UDP_FILTER_NAME
    fi

    local FILTER_TOTAL=$(nvram show gmn_filter_rule_num 0)
    local loop=0
    while [ $loop -lt $FILTER_TOTAL ]
    do
        local SRC_INTF=$(nvram show gmn_filter_rule $loop src_intf)
        local DST_INTF=$(nvram show gmn_filter_rule $loop dst_intf)
        local PORT_FROM=$(nvram show gmn_filter_rule $loop port_from)
        local PORT_TO=$(nvram show gmn_filter_rule $loop port_to)
        FilterRulesEdit udp add $SRC_INTF $DST_INTF $PORT_FROM $PORT_TO
        loop=$(($loop+1))
    done
}

FilterDrop()
{
    iptables -t filter -nvL | grep "$GMN_DROP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_DROP_FILTER_NAME has been created, now flushing it..."
        iptables -t filter -F $GMN_DROP_FILTER_NAME
    else
        echo "$GMN_DROP_FILTER_NAME has not be create!"
        iptables -t filter -N $GMN_DROP_FILTER_NAME
    fi
    iptables -t filter -nvL FORWARD | grep "$GMN_DROP_FILTER_NAME"
    if [ $? -eq 0 ]; then
        echo "$GMN_DROP_FILTER_NAME in FORWARD has been added"
    else
        echo "Adding $GMN_DROP_FILTER_NAME to FORWARD"
        iptables -t filter -I FORWARD -j $GMN_DROP_FILTER_NAME
    fi

    iptables -t filter -A $GMN_DROP_FILTER_NAME -i vlan2 -o vlan3 -j DROP
    iptables -t filter -A $GMN_DROP_FILTER_NAME -i vlan3 -o vlan2 -j DROP
    iptables -t filter -A $GMN_DROP_FILTER_NAME -p icmp -j ACCEPT
    iptables -t filter -A $GMN_DROP_FILTER_NAME -i br0 -o vlan3 -j DROP
    iptables -t filter -A $GMN_DROP_FILTER_NAME -i vlan3 -o br0 -j DROP
}

#-------  Shell start from here  -------#
lock $GMN_IPT_LOCK
if [ $# -eq 0 ]; then   #If no parameter, then create all
    NatRulesCreate
    FilterDrop
    FilterUDPCreate
    FilterTCPCreate
    SMCRouteRuleCreate
else
    if [ $# -eq 1 ] && [ $1 = "nat" ]; then
        NatRulesCreate
    elif [ $# -eq 6 ] && [ $1 = "nat" ]; then
        NatRulesEdit $2 $3 $4 $5 $6
    fi

    if [ $# -eq 1 ] && [ $1 = "filter" ]; then
        FilterDrop
        FilterUDPCreate
        FilterTCPCreate
    elif [ $# -eq 2 ] && [ $1 = "filter" ]; then
        if [ $2 = "tcp" ]; then
            FilterTCPCreate
        elif [ $2 = "udp" ]; then
            FilterUDPCreate
        elif [ $2 = "drop" ]; then
            FilterDrop
        fi
    elif [ $# -eq 7 ] && [ $1 = "filter" ]; then
        FilterRulesEdit $2 $3 $4 $5 $6 $7
    fi

    if [ $# -eq 1 ] && [ $1 = "smcroute" ]; then
        SMCRouteRuleCreate
    elif [ $# -eq 6 ] && [ $1 = "smcroute" ]; then
        SMCRouteRuleEdit $2 $3 $4 $5 $6
    fi
fi
lock -u $GMN_IPT_LOCK

