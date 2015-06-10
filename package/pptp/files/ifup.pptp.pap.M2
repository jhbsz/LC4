#!/bin/sh -x
. /etc/network.sh


# ifup.pptp $iface $type $NUM $num
wan_num="$(nvram get wan_num)"
unitid="$(expr $wan_num + $3 \* 2 )"
username="$(nvram show wan_pptp_l2tp_rule $4 username)"
username_decode="$(echo ${username} == | /etc/wl/b64 -d)"
password="$(nvram show wan_pptp_l2tp_rule $4 passwd)"
password_decode="$(echo ${password} == | /etc/wl/b64 -d)"
host="$(nvram show wan_pptp_l2tp_rule $4 host)"
host_decode="$(echo ${host} == | /etc/wl/b64 -d)"
#encmode="$(nvram show wan_pptp_l2tp_rule $3 encmode)"
#ipaddr="$(nvram get ${1}_ipaddr)"
conn_name=conn${3}

#construct /etc/ppp/chap-secrets
passline="${username_decode} PPTP ${password_decode} *"
if [ -f "/tmp/pap-secrets" ]; then
    retline=$(cat /etc/ppp/pap-secrets | grep "$passline")
    [ -z "${retline}" ] && echo $passline >> /etc/ppp/pap-secrets
else
    cat /etc/ppp/chap-secrets.template >> /tmp/pap-secrets
    echo "$passline" >> /tmp/pap-secrets
fi
cp -a /tmp/pap-secrets /etc/ppp/

#if [ "$encmode" = "mppe" ]; then
#    encmode="mppe required,stateless"
#else
    encmode=""
#fi
#construct /etc/ppp/peers/connextion-name
cat /etc/ppp/peers/conn.template.M2 | sed "s/HOST/${host_decode}/g" | \
    sed "s/USERNAME/${username_decode}/g" | sed "s/UNITID/$unitid/g"  | \
    sed "s/IFACE/$1/g" | sed "s/ENCMODE/$encmode/g" > \
    /tmp/${conn_name}

ln -sf /tmp/conn${3} /etc/ppp/peers/conn${3}
pppd call ${conn_name}
