#!/bin/sh
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2006 Fokus Fraunhofer <carsten.tittel@fokus.fraunhofer.de>

alias debug=${DEBUG:-:}
alias mount='busybox mount'

# newline
N="
"

_C=0
NO_EXPORT=1
LOAD_STATE=1
LIST_SEP=" "

hotplug_dev() {
	env -i ACTION=$1 INTERFACE=$2 /sbin/hotplug-call net
}

append() {
	local var="$1"
	local value="$2"
	local sep="${3:- }"
	
	eval "export ${NO_EXPORT:+-n} -- \"$var=\${$var:+\${$var}\${value:+\$sep}}\$value\""
}

list_contains() {
	local var="$1"
	local str="$2"
	local val

	eval "val=\" \${$var} \""
	[ "${val%% $str *}" != "$val" ]
}

list_remove() {
	local var="$1"
	local remove="$2"
	local val

	eval "val=\" \${$var} \""
	val1="${val%% $remove *}"
	[ "$val1" = "$val" ] && return
	val2="${val##* $remove }"
	[ "$val2" = "$val" ] && return
	val="${val1## } ${val2%% }"
	val="${val%% }"
	eval "export ${NO_EXPORT:+-n} -- \"$var=\$val\""
}

config_load() {
	[ -n "$IPKG_INSTROOT" ] && return 0
#	uci_load "$@"
}

reset_cb() {
	config_cb() { return 0; }
	option_cb() { return 0; }
	list_cb() { return 0; }
}
reset_cb

package() {
	return 0
}

config () {
	local cfgtype="$1"
	local name="$2"
	
	export ${NO_EXPORT:+-n} CONFIG_NUM_SECTIONS=$(($CONFIG_NUM_SECTIONS + 1))
	name="${name:-cfg$CONFIG_NUM_SECTIONS}"
	append CONFIG_SECTIONS "$name"
	[ -n "$NO_CALLBACK" ] || config_cb "$cfgtype" "$name"
	export ${NO_EXPORT:+-n} CONFIG_SECTION="$name"
	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_TYPE=$cfgtype"
}

option () {
	local varname="$1"; shift
	local value="$*"

	export ${NO_EXPORT:+-n} "CONFIG_${CONFIG_SECTION}_${varname}=$value"
	[ -n "$NO_CALLBACK" ] || option_cb "$varname" "$*"
}

list() {
	local varname="$1"; shift
	local value="$*"
	local len

	config_get len "$CONFIG_SECTION" "${varname}_LENGTH" 
	len="$((${len:-0} + 1))"
	config_set "$CONFIG_SECTION" "${varname}_ITEM$len" "$value"
	config_set "$CONFIG_SECTION" "${varname}_LENGTH" "$len"
	append "CONFIG_${CONFIG_SECTION}_${varname}" "$value" "$LIST_SEP"
	list_cb "$varname" "$*"
}

config_rename() {
	local OLD="$1"
	local NEW="$2"
	local oldvar
	local newvar
	
	[ -n "$OLD" -a -n "$NEW" ] || return
	for oldvar in `set | grep ^CONFIG_${OLD}_ | \
		sed -e 's/\(.*\)=.*$/\1/'` ; do
		newvar="CONFIG_${NEW}_${oldvar##CONFIG_${OLD}_}"
		eval "export ${NO_EXPORT:+-n} \"$newvar=\${$oldvar}\""
		unset "$oldvar"
	done
	export ${NO_EXPORT:+-n} CONFIG_SECTIONS="$(echo " $CONFIG_SECTIONS " | sed -e "s, $OLD , $NEW ,")"
	
	[ "$CONFIG_SECTION" = "$OLD" ] && export ${NO_EXPORT:+-n} CONFIG_SECTION="$NEW"
}

config_unset() {
	config_set "$1" "$2" ""
}

config_clear() {
	local SECTION="$1"
	local oldvar

	list_remove CONFIG_SECTIONS "$SECTION"
	export ${NO_EXPORT:+-n} CONFIG_SECTIONS="${SECTION:+$CONFIG_SECTIONS}"

	for oldvar in `set | grep ^CONFIG_${SECTION:+${SECTION}_} | \
		sed -e 's/\(.*\)=.*$/\1/'` ; do 
		unset $oldvar 
	done
}

config_get() {
	case "$3" in
		"") eval "echo \"\${CONFIG_${1}_${2}}\"";;
		*)  eval "export ${NO_EXPORT:+-n} -- \"$1=\${CONFIG_${2}_${3}}\"";;
	esac
}

# config_get_bool <variable> <section> <option> [<default>]
config_get_bool() {
	local _tmp
	config_get "_tmp" "$2" "$3"
	case "$_tmp" in
		1|on|true|enabled) export ${NO_EXPORT:+-n} "$1=1";;
		0|off|false|disabled) export ${NO_EXPORT:+-n} "$1=0";;
		*) eval "$1=$4";;
	esac
}

config_set() {
	local section="$1"
	local option="$2"
	local value="$3"
	local old_section="$CONFIG_SECTION"

	CONFIG_SECTION="$section"
	option "$option" "$value"
	CONFIG_SECTION="$old_section"
}

config_foreach() {
	local function="$1"
	[ "$#" -ge 1 ] && shift
	local type="$1"
	[ "$#" -ge 1 ] && shift
	local section cfgtype
	
	[ -z "$CONFIG_SECTIONS" ] && return 0
	for section in ${CONFIG_SECTIONS}; do
		config_get cfgtype "$section" TYPE
		[ -n "$type" -a "x$cfgtype" != "x$type" ] && continue
		eval "$function \"\$section\" \"\$@\""
	done
}

config_list_foreach() {
	[ "$#" -ge 3 ] || return 0
	local section="$1"; shift
	local option="$1"; shift
	local function="$1"; shift
	local val
	local len
	local c=1

	config_get len "${section}" "${option}_LENGTH"
	[ -z "$len" ] && return 0
	while [ $c -le "$len" ]; do
		config_get val "${section}" "${option}_ITEM$c"
		eval "$function \"\$val\" \"$@\""
		c="$(($c + 1))"
	done
}

load_modules() {
	[ -d /etc/modules.d ] && {
		cd /etc/modules.d
		sed 's/^[^#]/insmod &/' $* | ash 2>&- || :
	}
}

include() {
	local file
	
	for file in $(ls $1/*.sh 2>/dev/null); do
		. $file
	done
}

find_mtd_part() {
	local PART="$(grep "\"$1\"" /proc/mtd | awk -F: '{print $1}')"
	local PREFIX=/dev/mtdblock
	
	PART="${PART##mtd}"
	[ -d /dev/mtdblock ] && PREFIX=/dev/mtdblock/
	echo "${PART:+$PREFIX$PART}"
}

strtok() { # <string> { <variable> [<separator>] ... }
	local tmp
	local val="$1"
	local count=0

	shift

	while [ $# -gt 1 ]; do
		tmp="${val%%$2*}"

		[ "$tmp" = "$val" ] && break

		val="${val#$tmp$2}"

		export ${NO_EXPORT:+-n} "$1=$tmp"; count=$((count+1))
		shift 2
	done

	if [ $# -gt 0 -a -n "$val" ]; then
		export ${NO_EXPORT:+-n} "$1=$val"; count=$((count+1))
	fi

	return $count
}


jffs2_mark_erase() {
	local part="$(find_mtd_part "$1")"
	[ -z "$part" ] && {
		echo Partition not found.
		return 1
	}
	echo -e "\xde\xad\xc0\xde" | mtd -qq write - "$1"
}

#uci_apply_defaults() {(
#	cd /etc/uci-defaults || return 0
#	files="$(ls)"
#	[ -z "$files" ] && return 0
#	mkdir -p /tmp/.uci
#	for file in $files; do
#		( . "./$(basename $file)" ) && rm -f "$file"
#	done
#	uci commit
#)}
#
#[ -z "$IPKG_INSTROOT" -a -f /lib/config/uci.sh ] && . /lib/config/uci.sh

if_timeout_check_lock() {
    nsecs=$1
    lock_file=$2
    pid_be_check=$3
    looptimes=0
    #wait for 10 rounds
    while [ $looptimes -lt $nsecs ];
    do
        sleep 1
        LOCK_STAT=$(ps | sed '/grep/d' | grep "lock $lock_file" 2>&1 | wc -l)
        [ ! -f "${lock_file}.pid" ] && {
            #nothing to wait, leave
            lock -u $lock_file
            [ -f "${lock_file}.pid" ] && rm ${lock_file}.pid
            return
        }
        WAIT_PID=$(cat ${lock_file}.pid)
        [ $LOCK_STAT -gt 0 ] && {
            [ ! -f ${lock_file}.pid -o ! -d /proc/$WAIT_PID ] && {
                #process had gone, unlock and let our task keep go
                lock -u $lock_file
                [ -f "${lock_file}.pid" ] && rm ${lock_file}.pid
                return
            }
        }
        [ "$LOCK_STAT" = "0" ] && return
        looptimes=$(($looptimes + 1))
    done
    #before we left, unlock for timeout
    [ $LOCK_STAT -gt 0 ] && {
        lock -u $lock_file
        [ -f "${lock_file}.pid" ] && rm ${lock_file}.pid
    }
}


if_serialize_lock() {
    lock_file=$1
    pid_be_check=$2
    LOCK_STAT=$(ps | sed '/grep/d' | grep "lock $lock_file" 2>&1 | wc -l)
    # If interface action is processing and serialization is locked
    if [ $LOCK_STAT -gt 0 ]; then
        if [ -f ${lock_file}.pid ]; then
            # check if the process is still running
            WAIT_PID=$(cat ${lock_file}.pid)
            if [ -d /proc/$WAIT_PID ]; then
                # the process is running, wait for 10 seconds
                if_timeout_check_lock 60 $lock_file $pid_be_check &
                [ -n "$pid_be_check" -a -d "/proc/${pid_be_check}" ] && {
                    # lock and wait
                    lock $lock_file
                    echo "$pid_be_check" > ${lock_file}.pid
                    if_timeout_check_lock 20 $lock_file $pid_be_check &
                }
            else
                [ -n "$pid_be_check" -a -d "/proc/${pid_be_check}" ] && {
                    # if the process is not running, then we know this daemon we
                    # are waiting for comes with a wrong case, eq : pppoe but fail 
                    # to get an IP.
                    # In this case, we just need to unlock and lock again
                    # lock -u $lock_file
                    # lock $lock_file
                    # So we reuse the lock
                    echo "$pid_be_check" > ${lock_file}.pid
                    if_timeout_check_lock 20 $lock_file $pid_be_check &
                }
            fi
        else
            [ -n "$pid_be_check" -a -d "/proc/${pid_be_check}" ] && {
                # Basically here we should do 
                # lock -u ${lock_file}
                # lock ${lock_file}
                # Just leave it and replace the lock and been killed process
                # So we reuse the lock
                echo "$pid_be_check" > ${lock_file}.pid
                if_timeout_check_lock 20 $lock_file $pid_be_check &
            }
        fi
    else
        [ -n "$pid_be_check" -a -d "/proc/${pid_be_check}" ] && {
            # Just lock it and record the process to be killed
            lock $lock_file
            # if we are asked to wait something, then wait for timeout
            # else, we just lock
    	    echo "$pid_be_check" > ${lock_file}.pid
            if_timeout_check_lock 20 $lock_file $pid_be_check &
        }
    fi
}

if_serialize_ulock() {
    lock_file=$1
    pid_be_check=$2
    LOCK_STAT=$(ps | sed '/grep/d' | grep "lock $lock_file" 2>&1 | wc -l)
    [ "$LOCK_STAT" -gt "0" ] && {
        lock -u $lock_file
        [ -f "${lock_file}.pid" ] && rm ${lock_file}.pid
    }
}

if_serialize_check() {
    lock_file=$1
    pid_file=${lock_file}.pid
    LOCK_STAT=$(ps | sed '/grep/d' | grep "lock $lock_file" 2>&1 | wc -l)
    # we should check three things :
    # 1. locked
    # 2. waiting for some process
    # 3. the process is alive
    
    [ "$LOCK_STAT" -gt "0" -a -f "$pid_file" ] && {
    	pid_be_check="$(cat $pid_file)"
        [ -d "/proc/${pid_be_check}" ] && {
            lock -w $lock_file
            return
        }
    }
    [ "$LOCK_STAT" -gt "0" ] && lock -u $lock_file
    [ -f "$pid_file" ] && rm $pid_file
}

#kill_and_retry <process_name> <max times> <when to start force kill>
kill_and_retry() {
    local oldprocess=$1
    local maxtimes=$2
    local forcetimes=$3
    kill $(pidof $oldprocess)
    local count=0
    while [ -n "$(pidof $oldprocess)" ]
    do
         count=$(($count +1))
             [ "$count" -gt "$maxtimes" ] && {
                 return
             } || {
                 [ "$count" -gt "$forcetimes" ] && {
                     kill -9 $(pidof $oldprocess)
                 }
             }

         sleep 1
    done
}

#killpid_and_retry <process_id> <max times> <when to start force kill>
killpid_and_retry() {
    local oldpid=$1
    local maxtimes=$2
    local forcetimes=$3
    kill $oldpid
    local count=0
    while [ -d "/proc/$oldpid" ]
    do
         count=$(($count +1))
             [ "$count" -gt "$maxtimes" ] && {
                 return
             } || {
                 [ "$count" -gt "$forcetimes" ] && {
                     kill -9 $oldpid
                 }
             }

         sleep 1
    done
}

kill_and_wait() {
    oldpid=$1
    kill $oldpid
    local count=0
    while [ -d "/proc/$oldpid" ]
    do
         count=$(($count +1))
         [ "$count" -gt "30" ] && {
             err=1
             return 1
         }
         sleep 1
    done
    return 0
}

# added by Murphy on 2011/12/09
# we want to get system uptime with 1/100 accuracy 
# so we get it from /proc/uptime and get the 1st one then x 100
# 
get_system_time() {
    now=$(cat /proc/uptime | tr -s " "| tr -d . | cut -d" " -f1)
    echo $now
}

get_3g_stats() {
    local AC341U="$(cat /proc/bus/usb/devices | grep "Vendor=1199 ProdID=9057")"
    local AC340U="$(cat /proc/bus/usb/devices | grep "Vendor=1199 ProdID=9051")"
    local O3=$3
    [ -n "$AC341U" ] && {
        ezpcom -s /etc/chatscripts/script.GobiSig > /tmp/conn.info
        # LTE signal
        let RSRP_dBm=$(grep "RSRP" /tmp/conn.info | sed 's/^.*(dBm):    //g' | sed 's/TAC.*$//g')

        # if 4G(X), try to find 3G signal
        if [ -z "$RSRP_dBm" ]; then
            let RSSI_dBm=$(grep "RSSI" /tmp/conn.info | sed 's/^.*(dBm):    //g' | sed 's/ECIO.*$//g')
            [ -z "$RSSI_dBm" ] && {
                RSSI_dBm=-115
            }
            # dBm to Quality:
            local RSSI=0
            RSSI=$((($RSSI_dBm + 115) *20 / 9))

            local SIG=0
            [ -n "$RSSI" ] && {
                [ $RSSI -lt 0 ] && SIG=0
                [ $RSSI -ge 0  ] &&  [ $RSSI -lt 15  ] && SIG=0
                [ $RSSI -ge 15  ] &&  [ $RSSI -lt 32 ] && SIG=1
                [ $RSSI -ge 32 ] &&  [ $RSSI -lt 49 ] && SIG=2
                [ $RSSI -ge 49 ] &&  [ $RSSI -lt 66 ] && SIG=3
                [ $RSSI -ge 66 ] &&  [ $RSSI -le 83 ] && SIG=4
                [ $RSSI -gt 83 ] && SIG=5
                nvram replace attr wan_wwan_probe_rule $2 signal "$RSSI"
                nvram replace attr wan_wwan_probe_rule $2 sig "$SIG"
            }
        else
            # dBm to Quality:
            local RSRP=0
            RSRP=$((($RSRP_dBm * 217 / 100) + 256))

            local SIG=0
            [ -n "$RSRP" ] && {
                [ $RSRP -lt 0 ] && SIG=0
                [ $RSRP -ge 0  ] &&  [ $RSRP -lt 15  ] && SIG=0
                [ $RSRP -ge 15  ] &&  [ $RSRP -lt 32 ] && SIG=1
                [ $RSRP -ge 32 ] &&  [ $RSRP -lt 49 ] && SIG=2
                [ $RSRP -ge 49 ] &&  [ $RSRP -lt 66 ] && SIG=3
                [ $RSRP -ge 66 ] &&  [ $RSRP -le 83 ] && SIG=4
                [ $RSRP -gt 83 ] && SIG=5
                nvram replace attr wan_wwan_probe_rule $2 signal "$RSRP"
                nvram replace attr wan_wwan_probe_rule $2 sig "$SIG"
            }
        fi
        nvram replace attr wan_wwan_probe_rule $2 provider "Sprint"
        nvram replace attr wan_status_rule $2 trycount 0
        return
    }
    [ -n "$AC340U" ] && {
        nvram replace attr wan_wwan_probe_rule $2 provider "AT&T"
        nvram replace attr wan_status_rule $2 trycount 0
        return
    }

    local UML295="$(cat /proc/bus/usb/devices | grep "Vendor=10a9 ProdID=6064")"
    [ -n "$UML295" ] && {
        curl http://192.168.32.2/condata > /tmp/conn.info
        let LTE_RSSI_TO_PERCENT=$(grep "signal strength" /tmp/conn.info  | sed 's/\(.*\)\(<percent>\)\(.*\)\(<\/percent>\)\(.*\)/\3/')
        local LTE_SIG=0
        [ -n "$LTE_RSSI_TO_PERCENT" ] && {
            [ $LTE_RSSI_TO_PERCENT -lt 0 ] && LTE_SIG=0
            [ $LTE_RSSI_TO_PERCENT -ge 0  ] &&  [ $LTE_RSSI_TO_PERCENT -lt 15  ] && LTE_SIG=0
            [ $LTE_RSSI_TO_PERCENT -ge 15  ] &&  [ $LTE_RSSI_TO_PERCENT -lt 32 ] && LTE_SIG=1
            [ $LTE_RSSI_TO_PERCENT -ge 32 ] &&  [ $LTE_RSSI_TO_PERCENT -lt 49 ] && LTE_SIG=2
            [ $LTE_RSSI_TO_PERCENT -ge 49 ] &&  [ $LTE_RSSI_TO_PERCENT -lt 66 ] && LTE_SIG=3
            [ $LTE_RSSI_TO_PERCENT -ge 66 ] &&  [ $LTE_RSSI_TO_PERCENT -le 83 ] && LTE_SIG=4
            [ $LTE_RSSI_TO_PERCENT -gt 83 ] && LTE_SIG=5

            nvram replace attr wan_wwan_probe_rule $2 signal "$LTE_RSSI_TO_PERCENT"
            nvram replace attr wan_wwan_probe_rule $2 sig "$LTE_SIG"
        }
        local LTE_PROVIDER=$(awk '/<network><serving><name>/,/<\/name>/' /tmp/conn.info | sed 's/\(.*\)\(<network><serving><name>\)\(.*\)\(<\/name>\)\(.*\)/\3/')
        nvram replace attr wan_wwan_probe_rule $2 provider "$LTE_PROVIDER"
        nvram replace attr wan_status_rule $2 trycount 0
        return
    }

    times=$(($times+1))
    local PH8="$(cat /proc/bus/usb/devices | grep "Vendor=1e2d ProdID=0053")"
        [ -n "$PH8" ] && {
	    [ "$O3" == "V" ] && {
                /usr/sbin/ezpcom PH8_2 -d /dev/$1 > /tmp/3g_stats
	    } 
            [ "$O3" == "C200" -o "$O3" == "T" ] && {
                /usr/sbin/ezpcom c200_stats -d /dev/$1 > /tmp/3g_stats
            }
	}
	[ -z "$PH8" ] && {
            /usr/sbin/ezpcom stats -d /dev/$1 > /tmp/3g_stats
	}

    [ -s "/tmp/3g_stats" -o -n "$PH8" ] && {
        [ "$O3" == "SC7702" ] && {
            IMEI=$(SCRIPT='At+CGSN' ezpcom -d /dev/$1 -s /etc/chatscripts/script.gcom | sed -n '2p')
        } || {
            IMEI=$(cat /tmp/3g_stats | sed -n '1p' | awk '{FS=":";print $2}' | tr -d " ")
        }
        # some datacards will return the IMEI value is ERROR, change it to UNKNOWN
        echo $IMEI | grep -q "ERROR" && IMEI='UNKNOWN' 
        PROVIDER=$(cat /tmp/3g_stats | sed -n '2p' | awk '{FS="\"";print $2}' | tr -d "\"")
        # sometimes, AT&T will be retrieved incorrectly as AT& or AT
        # if AT& or AT retrieved, we will fix it to AT&T
        echo $PROVIDER | grep -q "AT" && PROVIDER='AT&T'
        RSSI=$(cat /tmp/3g_stats | sed -n '3p' | awk '{FS=":";print $2$3}' | awk '{FS=",";print $1}' | tr -d " " | tr -d "+CSQ")
        FIRMWARE=$(cat /tmp/3g_stats | sed -n '4p' | awk '{FS=":";print $2}' | tr -d " ")
        APN=$(cat /tmp/3g_stats | sed -n '5p' | awk '{FS="\"";print $4}')
        let dBm=0
        let RSSI_TO_PERCENT="NA"
        [ -n "$RSSI" ] && {
            let dBm=$RSSI*2-113
            let RSSI_TO_PERCENT=$RSSI*100/31
            [ $RSSI -ge 0  ] &&  [ $RSSI -lt 6  ] && SIG=1
            [ $RSSI -ge 6  ] &&  [ $RSSI -lt 12 ] && SIG=2
            [ $RSSI -ge 12 ] &&  [ $RSSI -lt 18 ] && SIG=3
            [ $RSSI -ge 18 ] &&  [ $RSSI -lt 24 ] && SIG=4
            [ $RSSI -ge 24 ] &&  [ $RSSI -le 31 ] && SIG=5
            [ $RSSI -eq 99 ] && SIG=0 && RSSI_TO_PERCENT=0 
        }
        [ "$RSSI_TO_PERCENT" = "NA" -o "$RSSI_TO_PERCENT" = "0" ] && \
        [ "$times" -lt "5" -a -z "$(cat /proc/bus/usb/devices | grep 'Vendor=1782 ProdID=0002')" ] && {
            sleep 1

            [ -n "$O3" ] && {
		[ "$O3" == "V" ] && {
		    get_3g_stats $1 0 V
		} || {
                    get_3g_stats $1 0 C200
		}
            } || {
                get_3g_stats $1
            }
            return
        }
        [ "$O3" == "C200" -o "$O3" == "SC7702" -o -n "$PH8" ] && {                                         
            let SIG=$RSSI*2-113          
        }        
        nvram replace attr wan_wwan_probe_rule $2 signal "$RSSI_TO_PERCENT"
        nvram replace attr wan_wwan_probe_rule $2 sig "$SIG"
        nvram replace attr wan_wwan_probe_rule $2 imei "$IMEI"
        nvram replace attr wan_wwan_probe_rule $2 provider "$PROVIDER"
        nvram replace attr wan_wwan_probe_rule $2 firmware "$FIRMWARE"
        nvram replace attr wan_wwan_probe_rule $2 apn "$APN"
        nvram replace attr wan_status_rule $2 trycount 0 
        logger EZP_USR 3G Signal Strength [RSSI:$RSSI][\($dBm dBm\)]
        return 
    }
}

ping_get_host(){
    ping -c 1 -w 1 $1 | grep PING | cut -d'(' -f2 | cut -d')' -f1
}

togo_find(){
    local MAC=$1
    local PROTO=$2
    local TOGO_NUM=$3
    local i=0
    local NUM=-1 
    while [ $i -lt $TOGO_NUM ]; do
        [ "$(nvram show togo_rule $i mac)" = "$MAC" -a \
            "$(nvram show togo_rule $i proto)" = "$PROTO" ] && {
            NUM=$i
            break;
        }
        i=$(($i+1))
    done
    echo $NUM
}

togo_add(){
    local NAME=$1
    local PROTO=$2
    local IP=$3
    local PORT=$4
    local MAC=$5
    local CONFIG=$6
    local TOGO="$(nvram get togo_rule)"
    local TOGO_NUM="$(nvram get togo_rule_num)"
    local PIPE="|"
    NUM=$(togo_find $MAC $PROTO $TOGO_NUM)
    [ "$NUM" = "-1" ] && {
        [ "$TOGO_NUM" = "0" ] && PIPE=""
            nvram set togo_rule="${TOGO}${PIPE}1^$NAME^$PROTO^$IP^$PORT^$MAC^$CONFIG"
            nvram set togo_rule_num=$(($TOGO_NUM+1))
    } || {
        nvram replace attr togo_rule $NUM status 1 
        nvram replace attr togo_rule $NUM name $NAME
        nvram replace attr togo_rule $NUM proto $PROTO
        nvram replace attr togo_rule $NUM ip $IP
        nvram replace attr togo_rule $NUM port $PORT
        nvram replace attr togo_rule $NUM mac $MAC
        nvram replace attr togo_rule $NUM config $CONFIG
    }
}

togo_del(){
    local MAC=$1
    local PROTO=$2
    local TOGO_NUM="$(nvram get togo_rule_num)"
    NUM=$(togo_find $MAC $PROTO $TOGO_NUM)
    [ "$NUM" != "-1" ] && nvram replace attr togo_rule $NUM status 0
}

USBSTORAGE_SERVICE() {
    local servname
    sleep 1
    [ -z "$(mount | grep /tmp/mnt)" ] && {
        SERVICE_ACTION=stop
    }
    [ -n "$(mount | grep /tmp/mnt)" ] && {
        SERVICE_ACTION=start
    }
    for servname in /etc/rc.d/U*; do
        [ -x $servname ] && {
            /etc/rc.common $servname $SERVICE_ACTION
        }
    done
}

POWER_LEVEL() {
    local LEVEL=$1
	case "$LEVEL" in
        0)
            local w_countdown="$(nvram show countdown_rule 0 w_countdown)"
            local p_countdown="$(nvram show countdown_rule 0 p_countdown)"
            nvram replace attr countdown_rule 0 w_count $w_countdown
            nvram replace attr countdown_rule 0 p_count $p_countdown
            nvram replace attr wl_basic_rule 0 enable 1
            sh /sbin/ezp-wl-ctrl wifi_on 
            ezp-i2c gauge host ready
        ;;
        1)
# Sony remove this function
#            sh /sbin/ezp-wl-ctrl wifi_off
#            ezp-i2c gauge host wifioff
#            nvram replace attr countdown_rule 0 w_count -1
            echo "power level 1" 
        ;;
        2)
            nvram replace attr countdown_rule 0 p_count -1
            ezp-i2c gauge host sleep
            sleep 1
            ezp-i2c gauge host sleep
            sleep 1
            ezp-i2c gauge host sleep
        ;;
	esac
}
