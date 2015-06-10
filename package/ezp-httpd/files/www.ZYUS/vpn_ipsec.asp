<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN / IPsec</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

function get_wan_title(idx) {
    var msg = '';
    if (idx)
        msg = 'Mobile';
    else
        msg = 'Ethernet';
    return msg;
}

function to_submit(F) {
	F.submit_button.value = "vpn_ipsec";
	F.action.value = "Apply";
    if (local_table_check("ipsec_table", ipsec_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "ipsec_table", "ipsec_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

function wrap_vpn_mode(v) {
    if (v == "net2net") {
        return "Site-to-Site";
/*
    } else if (v == 'rw_client') {
        return 'Road Warrior Client';
    } else if (v == 'rw_server') {
        return 'Road Warrior Server';
*/
    } else if (v == 'rw_server') {
        return 'Remote User';
    } else {
        return v.toUpperCase();
    }
}

function wrap_key_type(v) {
    if (v == "psk") {
        return "PSK";
    } else if (v == "rsa") {
        return "RSA";
    } else if (v = "caauth") {
        return "CA Authentication";
    } else {
        return v.toUpperCase();
    }
}

var F; 

var ipsec_objects =
[ { id: 'ipsec_table_add' },
  { id: 'ipsec_table_delete' },
  { id: 'ipsec_table_modify' },
  { id: 'ipsec_table_element' }
];

function local_rule_check(T) {
    if (E('name').value == '') {
        alert('Name is required.');
        return false;
    }
    if (E('mode').value == 'Site-to-Site') {
        if (E('remote_gateway').value == '') {
            alert('Remote Gateway is required.');
            return false;
        }
        if (E('remote_inipaddr').value == '') {
            alert('Remote Subnet IP is required.');
            return false;
        }
    }
    if (E('ike_keymode').value == 'PSK') {
        if (E('psk').value == '') {
            alert('PSK is required');
            return false;
        }
        var len = E('psk').value.length;
        if (len < 8 || len > 40) {
            alert('PSK length should be from 8 to 40 digits.');
            return false;
        }
    }
    return true;
}

function local_table_check(T, R) {
    /* Check the duplicate Conn Name */
    if (rlist_check_dup_field(T, R, enum_ipsec._NAME) == false) {
        return false;
    }

    /* Check the duplicate WAN's connection rule. */
    var row_num = parseInt(E(T+'_cur_rule_num').value);
    var wan_occupy = new Array(<%nv_get("wan_num");%>);
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        wan_occupy[i] = 0;
    }

    for (i = 0; i < row_num; i++) {
        if (R[i][enum_ipsec._ENABLE] == 0) {
            /* The rule is disable */
            continue;
        }
        var mode = R[i][enum_ipsec._MODE];
        if (mode == 'net2net') {
            /* Check 'Road Warrior' only */
            continue;
        }
        /* wanX -> X */
        var iface = R[i][enum_ipsec._LOCAL_EXTIF];
        var n = parseInt(iface.substring(3));
        if (n < 0 || n > <%nv_get("wan_num");%>) {
            alert('Local External Interface: ' + iface + ' format error.');
            return false;
        }
        /* Check whether the connection rule has been defined. */
        if (wan_occupy[n] == 1) {
            alert(tbl_wrap_wan(iface) + '\'s connection rule has been defined.');
            return false
        }
        wan_occupy[n] = 1;
    }
    return true;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'ipsec_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', ipsec_rule[i][enum_ipsec._NAME]);
        tbl_append_element(F, P + i + '_enable', ipsec_rule[i][enum_ipsec._ENABLE]);
        tbl_append_element(F, P + i + '_mode', ipsec_rule[i][enum_ipsec._MODE]);
        tbl_append_element(F, P + i + '_l2tp', ipsec_rule[i][enum_ipsec._L2TP]);
        tbl_append_element(F, P + i + '_local_extif', ipsec_rule[i][enum_ipsec._LOCAL_EXTIF]);
        tbl_append_element(F, P + i + '_local_inipaddr', ipsec_rule[i][enum_ipsec._LOCAL_INIPADDR]);
        tbl_append_element(F, P + i + '_local_netmask', ipsec_rule[i][enum_ipsec._LOCAL_NETMASK]);
        tbl_append_element(F, P + i + '_remote_gateway', ipsec_rule[i][enum_ipsec._REMOTE_GATEWAY]);
        tbl_append_element(F, P + i + '_remote_inipaddr', ipsec_rule[i][enum_ipsec._REMOTE_INIPADDR]);
        tbl_append_element(F, P + i + '_remote_netmask', ipsec_rule[i][enum_ipsec._REMOTE_NETMASK]);
        tbl_append_element(F, P + i + '_conn_init', ipsec_rule[i][enum_ipsec._CONN_INIT]);
        tbl_append_element(F, P + i + '_ike_keymode', ipsec_rule[i][enum_ipsec._IKE_KEYMODE]);
        tbl_append_element(F, P + i + '_psk', ipsec_rule[i][enum_ipsec._PSK]);
        tbl_append_element(F, P + i + '_rsa', ipsec_rule[i][enum_ipsec._RSA]);
        tbl_append_element(F, P + i + '_ca', ipsec_rule[i][enum_ipsec._CA]);
        tbl_append_element(F, P + i + '_adv', ipsec_rule[i][enum_ipsec._ADV]);
        tbl_append_element(F, P + i + '_phase1_mode', ipsec_rule[i][enum_ipsec._PHASE1_MODE]);
        tbl_append_element(F, P + i + '_phase1_auth', ipsec_rule[i][enum_ipsec._PHASE1_AUTH]);
        tbl_append_element(F, P + i + '_phase1_encrypt', ipsec_rule[i][enum_ipsec._PHASE1_ENCRYPT]);
        tbl_append_element(F, P + i + '_phase1_group', ipsec_rule[i][enum_ipsec._PHASE1_GROUP]);
        tbl_append_element(F, P + i + '_phase1_left_id', ipsec_rule[i][enum_ipsec._PHASE1_LEFT_ID]);
        tbl_append_element(F, P + i + '_phase1_right_id', ipsec_rule[i][enum_ipsec._PHASE1_RIGHT_ID]);
        tbl_append_element(F, P + i + '_phase2_auth', ipsec_rule[i][enum_ipsec._PHASE2_AUTH]);
        tbl_append_element(F, P + i + '_phase2_encrypt', ipsec_rule[i][enum_ipsec._PHASE2_ENCRYPT]);
        tbl_append_element(F, P + i + '_phase2_group', ipsec_rule[i][enum_ipsec._PHASE2_GROUP]);
        tbl_append_element(F, P + i + '_phase1_lifetime', ipsec_rule[i][enum_ipsec._PHASE1_LIFETIME]);
        tbl_append_element(F, P + i + '_phase2_lifetime', ipsec_rule[i][enum_ipsec._PHASE2_LIFETIME]);
        tbl_append_element(F, P + i + '_dpd_enable', ipsec_rule[i][enum_ipsec._DPD_ENABLE]);
        tbl_append_element(F, P + i + '_dpd_interval', ipsec_rule[i][enum_ipsec._DPD_INTERVAL]);
        tbl_append_element(F, P + i + '_dpd_timeout', ipsec_rule[i][enum_ipsec._DPD_TIMEOUT]);
    }
}

function local_generate_rule(T) {
    /* rule[] is existing. r[] is temporary used. Therefore, when we check the
     * original value of the input element, we need to use rule[] instead of
     * r[].
     */
    var r = new Array();

    r[enum_ipsec._NAME] =  E('name').value;
	r[enum_ipsec._ENABLE] = tbl_dewrap_enable('enable');
	r[enum_ipsec._MODE] = E('mode').value;
    if (rule[enum_ipsec._MODE] == 'rw_server') {
	    r[enum_ipsec._L2TP] = tbl_dewrap_enable('l2tp');
	    r[enum_ipsec._REMOTE_GATEWAY] = "";
	    r[enum_ipsec._REMOTE_INIPADDR] = ""; 
	    r[enum_ipsec._REMOTE_NETMASK] = rule[enum_ipsec._REMOTE_NETMASK];
        // For Road Warrior, we don't initialise connection
	    r[enum_ipsec._CONN_INIT] = "0";
    } else {
	    r[enum_ipsec._L2TP] = rule[enum_ipsec._L2TP];
	    r[enum_ipsec._REMOTE_GATEWAY] = E('remote_gateway').value;
	    r[enum_ipsec._REMOTE_INIPADDR] = E('remote_inipaddr').value;
	    r[enum_ipsec._REMOTE_NETMASK] = E('remote_netmask').value;
	    r[enum_ipsec._CONN_INIT] = tbl_dewrap_enable('conn_init');
    }
	r[enum_ipsec._LOCAL_EXTIF] = (E('local_extif') != undefined) ? E('local_extif').value : rule[enum_ipsec._LOCAL_EXTIF];
    if ((rule[enum_ipsec._MODE] == 'net2net') ||
        (rule[enum_ipsec._MODE] == 'rw_server' && rule[enum_ipsec._L2TP] == '0')) {
	    r[enum_ipsec._LOCAL_INIPADDR] = E('local_inipaddr').value;
	    r[enum_ipsec._LOCAL_NETMASK] = E('local_netmask').value;
    } else {
	    r[enum_ipsec._LOCAL_INIPADDR] = rule[enum_ipsec._LOCAL_INIPADDR];
	    r[enum_ipsec._LOCAL_NETMASK] = rule[enum_ipsec._LOCAL_NETMASK];
    }
	r[enum_ipsec._IKE_KEYMODE] = E('ike_keymode').value;
    if (rule[enum_ipsec._IKE_KEYMODE] == 'psk') {
	    r[enum_ipsec._PSK] = E('psk').value;
    } else {
	    r[enum_ipsec._PSK] = rule[enum_ipsec._PSK];
    }
	r[enum_ipsec._DPD_ENABLE] = tbl_dewrap_enable('dpd_enable');
    if (rule[enum_ipsec._DPD_ENABLE] == '1') {
	    r[enum_ipsec._DPD_INTERVAL] = E('dpd_interval').value;
	    r[enum_ipsec._DPD_TIMEOUT] = E('dpd_timeout').value;
    } else {
	    r[enum_ipsec._DPD_INTERVAL] = rule[enum_ipsec._DPD_INTERVAL];
	    r[enum_ipsec._DPD_TIMEOUT] = rule[enum_ipsec._DPD_TIMEOUT];
    }
	r[enum_ipsec._ADV] = tbl_dewrap_enable('adv');
    if (rule[enum_ipsec._ADV] == '1') {
	    r[enum_ipsec._PHASE1_MODE] = E('phase1_mode').value;
	    r[enum_ipsec._PHASE1_AUTH] = E('phase1_auth').value;
	    r[enum_ipsec._PHASE1_ENCRYPT] = E('phase1_encrypt').value;
	    r[enum_ipsec._PHASE1_GROUP] = tbl_dewrap('phase1_group');
	    r[enum_ipsec._PHASE1_LEFT_ID] = E('phase1_left_id').value;
	    r[enum_ipsec._PHASE1_RIGHT_ID] = E('phase1_right_id').value;
	    r[enum_ipsec._PHASE2_AUTH] = E('phase2_auth').value;
	    r[enum_ipsec._PHASE2_ENCRYPT] = E('phase2_encrypt').value;
	    r[enum_ipsec._PHASE2_GROUP] = E('phase2_group').value;
	    r[enum_ipsec._PHASE1_LIFETIME] = E('phase1_lifetime').value;
	    r[enum_ipsec._PHASE2_LIFETIME] = E('phase2_lifetime').value;
    } else {
	    r[enum_ipsec._PHASE1_MODE] = rule[enum_ipsec._PHASE1_MODE];
	    r[enum_ipsec._PHASE1_AUTH] = rule[enum_ipsec._PHASE1_AUTH];
	    r[enum_ipsec._PHASE1_ENCRYPT] = rule[enum_ipsec._PHASE1_ENCRYPT];
	    r[enum_ipsec._PHASE1_GROUP] = rule[enum_ipsec._PHASE1_GROUP];
	    r[enum_ipsec._PHASE1_LEFT_ID] = rule[enum_ipsec._PHASE1_LEFT_ID];
	    r[enum_ipsec._PHASE1_RIGHT_ID] = rule[enum_ipsec._PHASE1_RIGHT_ID];
	    r[enum_ipsec._PHASE2_AUTH] = rule[enum_ipsec._PHASE2_AUTH];
	    r[enum_ipsec._PHASE2_ENCRYPT] = rule[enum_ipsec._PHASE2_ENCRYPT];
	    r[enum_ipsec._PHASE2_GROUP] = rule[enum_ipsec._PHASE2_GROUP];
	    r[enum_ipsec._PHASE1_LIFETIME] = rule[enum_ipsec._PHASE1_LIFETIME];
	    r[enum_ipsec._PHASE2_LIFETIME] = rule[enum_ipsec._PHASE2_LIFETIME];
    }
    r[enum_ipsec._PHASE1_STATUS] = "none";
    r[enum_ipsec._PHASE2_STATUS] = "none";
    return r;
}

var enum_ipsec = {
    _NAME:0,
	_ENABLE:1,
	_MODE:2,
	_L2TP:3,
	_LOCAL_EXTIF:4,
	_LOCAL_INIPADDR:5,
	_LOCAL_NETMASK:6,
	_REMOTE_GATEWAY:7,
	_REMOTE_INIPADDR:8,
	_REMOTE_NETMASK:9,
	_CONN_INIT:10,
	_IKE_KEYMODE:11,
	_PSK:12,
	_RSA:13,
	_CA:14,
	_ADV:15,
	_PHASE1_MODE:16,
	_PHASE1_LEFT_ID:17,
	_PHASE1_RIGHT_ID:18,
	_PHASE1_LIFETIME:19,
	_PHASE2_LIFETIME:20,
	_PHASE1_AUTH:21,
	_PHASE1_ENCRYPT:22,
	_PHASE1_GROUP:23,
	_PHASE2_AUTH:24,
	_PHASE2_ENCRYPT:25,
	_PHASE2_GROUP:26,
	_DPD_ENABLE:27,
	_DPD_INTERVAL:28,
	_DPD_TIMEOUT:29,
	_PHASE1_STATUS:30,
	_PHASE2_STATUS:31
}

var default_rule = ['','1','net2net','0','wan0','<%nv_attr_get("lan_static_rule", i, "ipaddr");%>','255.255.255.0','','','255.255.255.0','1','psk','','','','0','main','','','3600','28800','md5','3des','dh2','md5','3des','dh2','0','10','60','none','none'];
var rule;
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;

    _local_create_input_field(T, nth);
}

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='ipsec_table_old_seq' value='"+nth+"' />"
        + "<input id='ipsec_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Connection Name");%>' + "</div>"
        + "<input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Connection Name\", 1)'/>"
        +"</div>";
    
    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='enable' name='enable' type='checkbox' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("VPN Mode");%>' + "</div>"
        + "<select id='mode' name='mode' onchange='tbl_input_choose(\""+T+"\", this, enum_ipsec._MODE)' >"
        +  "<option value='net2net'>Site-to-Site</option>"
        +  "<option value='rw_server'>Remote User</option>"
        + "</select>"
        +"</div>";
    
    if (rule[enum_ipsec._MODE] == 'rw_server') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("L2TP Enable");%>' + "</div>"
            + "<input id='l2tp' name='l2tp' type='checkbox' "
            +           "onchange='tbl_input_choose(\""+T+"\", this, enum_ipsec._L2TP)' />"
            +"</div>";
    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Local External Interface");%>' + "</div>"
        + "<select id='local_extif' name='local_extif' >";
    for (i = 0; i < <%nv_get("wan_num");%>; i++) {
        var msg = get_wan_title(i);
        s += "<option value='wan" + (i) + "' >" + msg + " WAN</option>";
    }
    s +=  "</select>"
        +"</div>";

    var netmask_last = [ "0", "128", "192", "224", "240", "248", "252", "254", "255" ];
    if ((rule[enum_ipsec._MODE] == 'net2net') ||
        (rule[enum_ipsec._MODE] == 'rw_server' && rule[enum_ipsec._L2TP] == '0')) {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Local Subnet IP");%>' + "</div>"
            + "<input id='local_inipaddr' name='local_inipaddr' size='17' maxlength='30' "
            + "onchange='valid_ipaddr(this, \"Local IP Address\")' />"
            +"</div>";
/*
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Local Netmask");%>' + "</div>"
            + "<select id='local_netmask' name='local_netmask' >";
        for (var i = 0; i < netmask_last.length; i++) {
            var j=i+24;
            s += "<option value=" + j + ">" + "255.255.255." + netmask_last[i] + "</option>";
        }
        s +=  "</select>"
            +"</div>";
*/
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Local Netmask");%>' + "</div>"
            + "<input id='local_netmask' name='local_netmask' size='17' maxlength='30' "
            + "onchange='valid_ipaddr(this, \"Local Netmask\")' />"
            +"</div>";
    }
    
    if (rule[enum_ipsec._MODE] == 'net2net') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Remote Gateway");%>' + "</div>"
            + "<input id='remote_gateway' name='remote_gateway' size='17' maxlength='30' "
            +     "onchange='valid_name(this, \"Remote Gateway\")' />"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Remote Subnet IP");%>' + "</div>"
            + "<input id='remote_inipaddr' name='remote_inipaddr' size='17' maxlength='30' "
            +     "onchange='valid_ipaddr(this, \"Remote Subnet IP\")' />"
            +"</div>";
/*
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Remote Netmask");%>' + "</div>"
            + "<select id='remote_netmask' name='remote_netmask' >";
        for (var i = 0; i < netmask_last.length; i++) {
            var j=i+24;
            s += "<option value=" + j + ">" + "255.255.255." + netmask_last[i] + "</option>";
        }
        s +=  "</select>"
            +"</div>";
*/
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Remote Netmask");%>' + "</div>"
            + "<input id='remote_netmask' name='remote_netmask' size='17' maxlength='30' "
            + "onchange='valid_ipaddr(this, \"Remote Netmask\")' />"
            +"</div>";
        
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Connection Initiation");%>' + "</div>"
            + "<input id='conn_init' name='conn_init' type='checkbox' />"
            +"</div>";
    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("IKE Key Mode");%>' + "</div>"
        + "<select id='ike_keymode' name='ike_keymode' >"
        +  "<option value='psk'>PSK</option>"
        + "</select>"
        +"</div>";

    if (rule[enum_ipsec._IKE_KEYMODE] == 'psk') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Preshared Key");%>' + "</div>"
            + "<input type='password' id='psk' name='psk' size='25' maxlength='40' />"
            +"</div>";
    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("DPD Enable");%>' + "</div>"
        + "<input id='dpd_enable' name='dpd_enable' type='checkbox' "
        +           "onchange='tbl_input_choose(\""+T+"\", this, enum_ipsec._DPD_ENABLE)' />"
        +"</div>";

    if (rule[enum_ipsec._DPD_ENABLE] == '1') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("DPD Interval");%>'+"</div>"
            + "<input id='dpd_interval' name='dpd_interval' size='5' maxlength='5' />"
            +    '<%lang("Seconds");%>'+"(10 ~ 1200)"
            +"</div>"
         
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("DPD Timeout");%>'+"</div>"
            + "<input id='dpd_timeout' name='dpd_timeout' size='5' maxlength='5' />"
            +    '<%lang("Seconds");%>'+"(30 ~ 3600)"
            +"</div>";

    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Advanced Options");%>' + "</div>"
        + "<input id='adv' name='adv' type='checkbox' "
        +           "onchange='tbl_input_choose(\""+T+"\", this, enum_ipsec._ADV)' />"
        +"</div>";

    if (rule[enum_ipsec._ADV] == '1') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Mode");%>' + "</div>"
            + "<select id='phase1_mode' name='phase1_mode' >"
            +  "<option value='main'>Main</option>"
            +  "<option value='aggressive'>Aggressive</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Local ID");%>' + "</div>"
            + "<input id='phase1_left_id' name='phase1_left_id' size='17' maxlength='15' onblur='' />"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Remote ID");%>' + "</div>"
            + "<input id='phase1_right_id' name='phase1_right_id' size='17' maxlength='15' onblur='' />"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Lifetime");%>'+"</div>"
            + "<input id='phase1_lifetime' name='phase1_lifetime' size='5' maxlength='5' />"
            +    '<%lang("Seconds");%>'+"(1200 ~ 86400)"
            +"</div>"
         
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 2 Lifetime");%>'+"</div>"
            + "<input id='phase2_lifetime' name='phase2_lifetime' size='5' maxlength='5' />"
            +    '<%lang("Seconds");%>'+"(1200 ~ 86400)"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Authentication");%>'+"</div>"
            + "<select id='phase1_auth' name='phase1_auth' >"
            +  "<option value='md5'>MD5</option>"
            +  "<option value='sha1'>SHA1</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Encryption");%>'+"</div>"
            + "<select id='phase1_encrypt' >name='phase1_encrypt' >"
            +  "<option value='des'>DES</option>"
            +  "<option value='3des'>3DES</option>"
            +  "<option value='aes128'>AES</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 1 Group Key Management");%>'+"</div>"
            + "<select id='phase1_group' name='phase1_group' >"
            +  "<option value='dh1'>DH1</option>"
            +  "<option value='dh2'>DH2</option>"
            +  "<option value='dh5'>DH5</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 2 Authentication");%>'+"</div>"
            + "<select id='phase2_auth' name='phase2_auth' >"
            +  "<option value='md5'>MD5</option>"
            +  "<option value='sha1'>SHA1</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 2 Encryption");%>'+"</div>"
            + "<select id='phase2_encrypt' name='phase2_encrypt' >"
            +  "<option value='null'>Null</option>"
            +  "<option value='des'>DES</option>"
            +  "<option value='3des'>3DES</option>"
            +  "<option value='aes128'>AES</option>"
            + "</select>"
            +"</div>";

        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Phase 2 Group Key Management (PFS)");%>'+"</div>"
            + "<select id='phase2_group' name='phase2_group' >"
            +  "<option value='none'>None</option>"
            +  "<option value='dh1'>DH1</option>"
            +  "<option value='dh2'>DH2</option>"
            +  "<option value='dh5'>DH5</option>"
            + "</select>"
            +"</div>";
    }

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value = rule[enum_ipsec._NAME];
    tbl_assign_enable('enable', rule[enum_ipsec._ENABLE]);
    E('mode').value = rule[enum_ipsec._MODE];
    if (rule[enum_ipsec._MODE] == 'rw_server') {
        tbl_assign_enable('l2tp', rule[enum_ipsec._L2TP]);
    }
    E('local_extif').value = rule[enum_ipsec._LOCAL_EXTIF];
    if ((rule[enum_ipsec._MODE] == 'net2net') ||
        (rule[enum_ipsec._MODE] == 'rw_server' && rule[enum_ipsec._L2TP] == '0')) {
        E('local_inipaddr').value = rule[enum_ipsec._LOCAL_INIPADDR];
        E('local_netmask').value = rule[enum_ipsec._LOCAL_NETMASK];
    }
    if (rule[enum_ipsec._MODE] == 'net2net') {
        E('remote_gateway').value = rule[enum_ipsec._REMOTE_GATEWAY];
        E('remote_inipaddr').value = rule[enum_ipsec._REMOTE_INIPADDR];
        E('remote_netmask').value = rule[enum_ipsec._REMOTE_NETMASK];
        tbl_assign_enable('conn_init', rule[enum_ipsec._CONN_INIT]);
    }
    E('ike_keymode').value = rule[enum_ipsec._IKE_KEYMODE];
    if (rule[enum_ipsec._IKE_KEYMODE] == 'psk') {
        E('psk').value = rule[enum_ipsec._PSK];
    } else if (rule[enum_ipsec._IKE_KEYMODE] == 'rsa') {
        E('rsa').value = rule[enum_ipsec._RSA];
    } else if (rule[enum_ipsec._IKE_KEYMODE] == 'ca') {
        E('ca').value = rule[enum_ipsec._CA];
    }
    tbl_assign_enable('dpd_enable', rule[enum_ipsec._DPD_ENABLE]);
    if (rule[enum_ipsec._DPD_ENABLE] == '1') {
        E('dpd_interval').value = rule[enum_ipsec._DPD_INTERVAL];
        E('dpd_timeout').value = rule[enum_ipsec._DPD_TIMEOUT];
    }
    tbl_assign_enable('adv', rule[enum_ipsec._ADV]);
    if (rule[enum_ipsec._ADV] == '1') {
        E('phase1_mode').value = rule[enum_ipsec._PHASE1_MODE];
        E('phase1_left_id').value = rule[enum_ipsec._PHASE1_LEFT_ID];
        E('phase1_right_id').value = rule[enum_ipsec._PHASE1_RIGHT_ID];
        E('phase1_lifetime').value = rule[enum_ipsec._PHASE1_LIFETIME];
        E('phase2_lifetime').value = rule[enum_ipsec._PHASE2_LIFETIME];
        E('phase1_auth').value = rule[enum_ipsec._PHASE1_AUTH];
        E('phase1_encrypt').value = rule[enum_ipsec._PHASE1_ENCRYPT];
        E('phase1_group').value = rule[enum_ipsec._PHASE1_GROUP];
        E('phase2_auth').value = rule[enum_ipsec._PHASE2_AUTH];
        E('phase2_encrypt').value = rule[enum_ipsec._PHASE2_ENCRYPT];
        E('phase2_group').value = rule[enum_ipsec._PHASE2_GROUP];
    }
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Connection Name");%></th>'
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("Remote Gateway");%></th>'
        + '<th><%lang("Remote Subnet IP");%> / <%lang("Netmask");%></th>'
        + '<th><%lang("Phase 1");%></th>'
        + '<th><%lang("Phase 2");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_ipsec._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_ipsec._ENABLE]) + "</td>"
             + "<td>" + tbl_wrap_wan(R[i][enum_ipsec._LOCAL_EXTIF]) + "</td>";
        if (R[i][enum_ipsec._REMOTE_GATEWAY] == "") {
            c += '<td><% lang("Any"); %></td>';
        } else {
            c += "<td>" + R[i][enum_ipsec._REMOTE_GATEWAY] + "</td>";
        }
        if (R[i][enum_ipsec._REMOTE_INIPADDR] == "") {
            c += '<td><% lang("Any"); %></td>';
        } else {
            c += "<td>" + R[i][enum_ipsec._REMOTE_INIPADDR] + "/" + R[i][enum_ipsec._REMOTE_NETMASK] + "</td>";
        }
        if (R[i][enum_ipsec._ENABLE] == 1) {
            c += "<td>" + tbl_wrap_connection(R[i][enum_ipsec._PHASE1_STATUS]) + "</td>"
               + "<td>" + tbl_wrap_connection(R[i][enum_ipsec._PHASE2_STATUS]) + "</td>"
               + "</tr>";
        } else {
            c += "<td>" + tbl_wrap_enable(R[i][enum_ipsec._ENABLE]) + "</td>"
               + "<td>" + tbl_wrap_enable(R[i][enum_ipsec._ENABLE]) + "</td>"
               + "</tr>";
        }
    }

    return c;
}

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("ipsec_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, ipsec_objects);
    if (enable == 1) {
        tbl_disable_row_edit('ipsec_table');
    }

    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Security", "VPN / IPsec"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("VPN / IPsec");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>
              <% tbl_get("ipsec_rule"); %>
              <% get_ipsec_status(); %>
              <% tbl_get("ipsec_status_rule"); %>
              <script type="text/javascript">
              tbl_add_additional_rule_element(ipsec_rule, enum_ipsec._PHASE1_STATUS, enum_ipsec._PHASE2_STATUS, ipsec_status_rule,0,1);
              </script>
              <fieldset>
                <legend><%lang("IPsec");%></legend>
                <div class="setting">
                  <div class="label"><% lang("IPsec"); %></div>
                  <input type="radio" value="1" name="ipsec_enable" id='ipsec_enable' onclick="init()"
                        <%nvg_match("ipsec_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="ipsec_enable" id='ipsec_enable' onclick="init()"
                        <%nvg_match("ipsec_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

              </fieldset><br />

              <fieldset>
                <legend><%lang("User Rule");%></legend>
                <%tbl_create_table("ipsec_table", "ipsec_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("ipsec_table", ipsec_rule);
                </script>
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.reload()" />
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <%widget_show();%>
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_input_window("ipsec_table", "ipsec_rule", 0, 10)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
