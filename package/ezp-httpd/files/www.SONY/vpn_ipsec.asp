<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN / IPsec</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F; 

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

function local_rule_check(T) {
    if ($('#name').val() == '') {
        alert('Name is required.');
        return false;
    }
    if ($("select#mode option:selected").val() == 'net2net') {
        if ($('#remote_gateway').val() == '') {
            alert('Remote Gateway is required.');
            return false;
        }
        if ($('#remote_inipaddr').val() == '') {
            alert('Remote Subnet IP is required.');
            return false;
        }
    }
    if ($("select#ike_keymode option:selected").val() == 'psk') {
        if ($('#psk').val() == '') {
            alert('PSK is required');
            return false;
        }
        var len = $('#psk').val().length;
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
    var row_num = parseInt(ipsec_rule.length);
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
    var row_num = ipsec_rule.length ;
    $("<input>").attr({type: "hidden", id: "ipsec_rule", name: "ipsec_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: ipsec_rule[i][enum_ipsec._NAME]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: ipsec_rule[i][enum_ipsec._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_mode', name: P + i + '_mode', value: ipsec_rule[i][enum_ipsec._MODE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_l2tp', name: P + i + '_l2tp', value: ipsec_rule[i][enum_ipsec._L2TP]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_local_extif', name: P + i + '_local_extif', value: ipsec_rule[i][enum_ipsec._LOCAL_EXTIF]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_local_inipaddr', name: P + i + '_local_inipaddr', value: ipsec_rule[i][enum_ipsec._LOCAL_INIPADDR]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_local_netmask', name: P + i + '_local_netmask', value: ipsec_rule[i][enum_ipsec._LOCAL_NETMASK]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_remote_gateway', name: P + i + '_remote_gateway', value: ipsec_rule[i][enum_ipsec._REMOTE_GATEWAY]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_remote_inipaddr', name: P + i + '_remote_inipaddr', value: ipsec_rule[i][enum_ipsec._REMOTE_INIPADDR]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_remote_netmask', name: P + i + '_remote_netmask', value: ipsec_rule[i][enum_ipsec._REMOTE_NETMASK]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_conn_init', name: P + i + '_conn_init', value: ipsec_rule[i][enum_ipsec._CONN_INIT]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_ike_keymode', name: P + i + '_ike_keymode', value: ipsec_rule[i][enum_ipsec._IKE_KEYMODE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_psk', name: P + i + '_psk', value: ipsec_rule[i][enum_ipsec._PSK]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_rsa', name: P + i + '_rsa', value: ipsec_rule[i][enum_ipsec._RSA]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_ca', name: P + i + '_ca', value: ipsec_rule[i][enum_ipsec._CA]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_adv', name: P + i + '_adv', value: ipsec_rule[i][enum_ipsec._ADV]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_mode', name: P + i + '_phase1_mode', value: ipsec_rule[i][enum_ipsec._PHASE1_MODE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_auth', name: P + i + '_phase1_auth', value: ipsec_rule[i][enum_ipsec._PHASE1_AUTH]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_encrypt', name: P + i + '_phase1_encrypt', value: ipsec_rule[i][enum_ipsec._PHASE1_ENCRYPT]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_group', name: P + i + '_phase1_group', value: ipsec_rule[i][enum_ipsec._PHASE1_GROUP]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_left_id', name: P + i + '_phase1_left_id', value: ipsec_rule[i][enum_ipsec._PHASE1_LEFT_ID]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_right_id', name: P + i + '_phase1_right_id', value: ipsec_rule[i][enum_ipsec._PHASE1_RIGHT_ID]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase2_auth', name: P + i + '_phase2_auth', value: ipsec_rule[i][enum_ipsec._PHASE2_AUTH]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase2_encrypt', name: P + i + '_phase2_encrypt', value: ipsec_rule[i][enum_ipsec._PHASE2_ENCRYPT]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase2_group', name: P + i + '_phase2_group', value: ipsec_rule[i][enum_ipsec._PHASE2_GROUP]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase1_lifetime', name: P + i + '_phase1_lifetime', value: ipsec_rule[i][enum_ipsec._PHASE1_LIFETIME]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_phase2_lifetime', name: P + i + '_phase2_lifetime', value: ipsec_rule[i][enum_ipsec._PHASE2_LIFETIME]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_dpd_enable', name: P + i + '_dpd_enable', value: ipsec_rule[i][enum_ipsec._DPD_ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_dpd_interval', name: P + i + '_dpd_interval', value: ipsec_rule[i][enum_ipsec._DPD_INTERVAL]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_dpd_timeout', name: P + i + '_dpd_timeout', value: ipsec_rule[i][enum_ipsec._DPD_TIMEOUT]}).appendTo("form");
    }
}

function local_generate_rule(T) {

    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }

    var l2tp = 0;
    if ($('input[name=l2tp]').is(':checked')){
        l2tp = $('[name=l2tp]:checked').val();
    }

    var conn_init = 0;
    if ($('input[name=conn_init]').is(':checked')){
        conn_init = $('[name=conn_init]:checked').val();
    }

    var dpd_enable = 0;
    if ($('input[name=dpd_enable]').is(':checked')){
        dpd_enable = $('[name=dpd_enable]:checked').val();
    }

    var adv = 0;
    if ($('input[name=adv]').is(':checked')){
        adv = $('[name=adv]:checked').val();
    }

    var rule_tmp = Array();

    rule_tmp[enum_ipsec._NAME] = $("#name").val();
	rule_tmp[enum_ipsec._ENABLE] = enable;
	rule_tmp[enum_ipsec._MODE] = $("select#mode option:selected").val();
    if (rule_tmp[enum_ipsec._MODE] =="rw_server"){
	    rule_tmp[enum_ipsec._L2TP] = l2tp;
	    rule_tmp[enum_ipsec._REMOTE_GATEWAY] = "";
	    rule_tmp[enum_ipsec._REMOTE_INIPADDR] = "";
	    rule_tmp[enum_ipsec._REMOTE_NETMASK] = default_rule[enum_ipsec._REMOTE_NETMASK];
        // For Road Warrior, we don't initialise connection
	    rule_tmp[enum_ipsec._CONN_INIT] = "0";
    } else {    //net2net
	    rule_tmp[enum_ipsec._L2TP] = default_rule[enum_ipsec._L2TP];
	    rule_tmp[enum_ipsec._REMOTE_GATEWAY] = $("#remote_gateway").val();
	    rule_tmp[enum_ipsec._REMOTE_INIPADDR] = $("#remote_inipaddr").val();
	    rule_tmp[enum_ipsec._REMOTE_NETMASK] = $("#remote_netmask").val();
	    rule_tmp[enum_ipsec._CONN_INIT] = conn_init;
    }
    rule_tmp[enum_ipsec._LOCAL_EXTIF] = $("select#local_extif option:selected").val();
    if (l2tp=="0"){
	    rule_tmp[enum_ipsec._LOCAL_INIPADDR] = $("#local_inipaddr").val();
	    rule_tmp[enum_ipsec._LOCAL_NETMASK] = $("#local_netmask").val();
    } else {
	    rule_tmp[enum_ipsec._LOCAL_INIPADDR] = default_rule[enum_ipsec._LOCAL_INIPADDR];
	    rule_tmp[enum_ipsec._LOCAL_NETMASK] = default_rule[enum_ipsec._LOCAL_NETMASK];
    }

    rule_tmp[enum_ipsec._IKE_KEYMODE] = $("select#ike_keymode option:selected").val();
    if (rule_tmp[enum_ipsec._IKE_KEYMODE] == 'psk') {
	    rule_tmp[enum_ipsec._PSK] = $("#psk").val();
    } else {
	    rule_tmp[enum_ipsec._PSK] = default_rule[enum_ipsec._PSK];
    }

    rule_tmp[enum_ipsec._DPD_ENABLE] = dpd_enable;
    if (dpd_enable == '1') {
	    rule_tmp[enum_ipsec._DPD_INTERVAL] = $("#dpd_interval").val();
	    rule_tmp[enum_ipsec._DPD_TIMEOUT] = $("#dpd_timeout").val();
    } else {
	    rule_tmp[enum_ipsec._DPD_INTERVAL] = default_rule[enum_ipsec._DPD_INTERVAL];
	    rule_tmp[enum_ipsec._DPD_TIMEOUT] = default_rule[enum_ipsec._DPD_TIMEOUT];
    }

	rule_tmp[enum_ipsec._ADV] = adv;
    if (adv == '1') {
	    rule_tmp[enum_ipsec._PHASE1_MODE] = $("select#phase1_mode option:selected").val();
	    rule_tmp[enum_ipsec._PHASE1_AUTH] = $("select#phase1_auth option:selected").val();
	    rule_tmp[enum_ipsec._PHASE1_ENCRYPT] = $("select#phase1_encrypt option:selected").val();
	    rule_tmp[enum_ipsec._PHASE1_GROUP] = $("select#phase1_group option:selected").val();
	    rule_tmp[enum_ipsec._PHASE1_LEFT_ID] = $("#phase1_left_id").val();
	    rule_tmp[enum_ipsec._PHASE1_RIGHT_ID] = $("#phase1_right_id").val();
	    rule_tmp[enum_ipsec._PHASE2_AUTH] = $("select#phase2_auth option:selected").val();
	    rule_tmp[enum_ipsec._PHASE2_ENCRYPT] = $("select#phase2_encrypt option:selected").val();
	    rule_tmp[enum_ipsec._PHASE2_GROUP] = $("select#phase2_group option:selected").val();
	    rule_tmp[enum_ipsec._PHASE1_LIFETIME] = $("#phase1_lifetime").val();
	    rule_tmp[enum_ipsec._PHASE2_LIFETIME] = $("#phase2_lifetime").val();
    } else {
	    rule_tmp[enum_ipsec._PHASE1_MODE] = default_rule[enum_ipsec._PHASE1_MODE];
	    rule_tmp[enum_ipsec._PHASE1_AUTH] = default_rule[enum_ipsec._PHASE1_AUTH];
	    rule_tmp[enum_ipsec._PHASE1_ENCRYPT] = default_rule[enum_ipsec._PHASE1_ENCRYPT];
	    rule_tmp[enum_ipsec._PHASE1_GROUP] = default_rule[enum_ipsec._PHASE1_GROUP];
	    rule_tmp[enum_ipsec._PHASE1_LEFT_ID] = default_rule[enum_ipsec._PHASE1_LEFT_ID];
	    rule_tmp[enum_ipsec._PHASE1_RIGHT_ID] = default_rule[enum_ipsec._PHASE1_RIGHT_ID];
	    rule_tmp[enum_ipsec._PHASE2_AUTH] = default_rule[enum_ipsec._PHASE2_AUTH];
	    rule_tmp[enum_ipsec._PHASE2_ENCRYPT] = default_rule[enum_ipsec._PHASE2_ENCRYPT];
	    rule_tmp[enum_ipsec._PHASE2_GROUP] = default_rule[enum_ipsec._PHASE2_GROUP];
	    rule_tmp[enum_ipsec._PHASE1_LIFETIME] = default_rule[enum_ipsec._PHASE1_LIFETIME];
	    rule_tmp[enum_ipsec._PHASE2_LIFETIME] = default_rule[enum_ipsec._PHASE2_LIFETIME];
    }
   
    rule_tmp[enum_ipsec._PHASE1_STATUS] = "none";
    rule_tmp[enum_ipsec._PHASE2_STATUS] = "none";
    return rule_tmp;
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

function init() {
    if ($('[name=ipsec_enable]:checked').val()==1){
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
        $("#ItemUp").removeClass('ui-state-disabled');
        $("#ItemDown").removeClass('ui-state-disabled');
    }else{
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
        $("#ItemUp").addClass('ui-state-disabled');
        $("#ItemDown").addClass('ui-state-disabled');
    }

    <%widget_start();%>
}

function GetGridData(){
    var IPsec_rows = [];
    var IPsec_data = {};

    for (var i=0; i<ipsec_rule.length; i++ )  {
        IPsec_data = {};
        IPsec_data["name"] = ipsec_rule[i][enum_ipsec._NAME];
        IPsec_data["enable"] = tbl_wrap_enable( ipsec_rule[i][enum_ipsec._ENABLE] );
        IPsec_data["local_extif"] = tbl_wrap_wan( ipsec_rule[i][enum_ipsec._LOCAL_EXTIF] );
        if (ipsec_rule[i][enum_ipsec._REMOTE_GATEWAY] == ""){
            IPsec_data["remote_gateway"] = "<% lang("Any"); %>";
        } else {
            IPsec_data["remote_gateway"] = ipsec_rule[i][enum_ipsec._REMOTE_GATEWAY];
        }
        if (ipsec_rule[i][enum_ipsec._REMOTE_INIPADDR] == "") {
            IPsec_data["remote_inipaddr"] = "<% lang("Any"); %>";
        } else {
            IPsec_data["remote_inipaddr"] = ipsec_rule[i][enum_ipsec._REMOTE_INIPADDR] + "/" + ipsec_rule[i][enum_ipsec._REMOTE_NETMASK];
        }

        if (ipsec_rule[i][enum_ipsec._ENABLE] == 1) {
            IPsec_data["phase1_status"] = tbl_wrap_connection(ipsec_rule[i][enum_ipsec._PHASE1_STATUS]);
            IPsec_data["phase2_status"] = tbl_wrap_connection(ipsec_rule[i][enum_ipsec._PHASE2_STATUS]);
        } else {
            IPsec_data["phase1_status"] = tbl_wrap_enable( ipsec_rule[i][enum_ipsec._ENABLE] );
            IPsec_data["phase2_status"] = tbl_wrap_enable( ipsec_rule[i][enum_ipsec._ENABLE] );
        }

        IPsec_rows.push(IPsec_data);
    }
    return IPsec_rows;
}

function Mode_Change() {
    if ($("#mode").val() =="rw_server"){
        $("#l2tp").parents("tr").show();
        $("#remote_gateway").parents("tr").hide();
        $("#remote_gateway").attr("value","");
        $("#remote_inipaddr").parents("tr").hide();
        $("#remote_inipaddr").attr("value","");
        $("#remote_netmask").parents("tr").hide();
        $("#remote_netmask").attr("value","");
        $("#conn_init").parents("tr").hide();
        $('#conn_init').attr('checked', false);

    } else {    //net2net
        $('#l2tp').attr('checked', false);
        $("#l2tp").parents("tr").hide();
        $("#remote_gateway").parents("tr").show();
        $("#remote_inipaddr").parents("tr").show();
        $("#remote_netmask").parents("tr").show();
        $("#conn_init").parents("tr").show();

    }
    
}

function L2TP_Change(){
    if ($('[name=l2tp]:checked').val()=="1"){
        $("#local_inipaddr").attr("value","");
        $("#local_inipaddr").parents("tr").hide();
        $("#local_netmask").attr("value","");
        $("#local_netmask").parents("tr").hide();
    } else {
        $("#local_inipaddr").attr("value","<%nv_attr_get("lan_static_rule", i, "ipaddr");%>");
        $("#local_inipaddr").parents("tr").show();
        $("#local_netmask").attr("value","255.255.255.0");
        $("#local_netmask").parents("tr").show();
    }
}

function DPD_Change(){
    if ($('[name=dpd_enable]:checked').val()=="1"){
        $("#dpd_interval").attr("value","10");
        $("#dpd_interval").parents("tr").show();
        $("#dpd_timeout").attr("value","60");
        $("#dpd_timeout").parents("tr").show();
    }else{
        $("#dpd_interval").attr("value","");
        $("#dpd_interval").parents("tr").hide();
        $("#dpd_timeout").attr("value","");
        $("#dpd_timeout").parents("tr").hide();
    }
}

function ADV_Change(){
    if ($('[name=adv]:checked').val()=="1"){
        $("#phase1_mode").parents("tr").show();
        $("#phase1_left_id").parents("tr").show();
        $("#phase1_right_id").parents("tr").show();
        $("#phase1_lifetime").val("3600");
        $("#phase1_lifetime").parents("tr").show();
        $("#phase2_lifetime").val("28800");
        $("#phase2_lifetime").parents("tr").show();
        $("#phase1_auth").parents("tr").show();
        $("select[name='phase1_encrypt']").attr("value", "3des");
        $("#phase1_encrypt").parents("tr").show();
        $("select[name='phase1_group']").attr("value", "dh2");
        $("#phase1_group").parents("tr").show();
        $("#phase2_auth").parents("tr").show();
        $("select[name='phase2_encrypt']").attr("value", "3des");
        $("#phase2_encrypt").parents("tr").show();
        $("select[name='phase2_group']").attr("value", "dh2");
        $("#phase2_group").parents("tr").show();

    } else {
        $("#phase1_mode").parents("tr").hide();
        $("#phase1_left_id").parents("tr").hide();
        $("#phase1_right_id").parents("tr").hide();
        $("#phase1_lifetime").parents("tr").hide();
        $("#phase2_lifetime").parents("tr").hide();
        $("#phase1_auth").parents("tr").hide();
        $("#phase1_encrypt").parents("tr").hide();
        $("#phase1_group").parents("tr").hide();
        $("#phase2_auth").parents("tr").hide();
        $("#phase2_encrypt").parents("tr").hide();
        $("#phase2_group").parents("tr").hide();
    }
}

function CreateIPsecDataForm(){
    $("#IPsecDataForm").append("<tr><td align='right' width='200'><%lang("Connection Name");%>:</td><td><input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Connection Name\", 1)'/></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1'/></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("VPN Mode");%>:</td><td>"+
                               "<select id='mode' name='mode' onchange='Mode_Change()'>"+
                               "    <option value='net2net'>Net-to-Net</option>"+
                               "    <option value='rw_server'>Road Warrior</option>"+
                               "</select></td></tr>");

    $("#IPsecDataForm").append("<tr style='display: none' ><td align='right'><%lang("L2TP Enable");%>:</td><td><input id='l2tp' name='l2tp' type='checkbox' onchange='L2TP_Change()' value='1' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Local External Interface");%>:</td><td>"+
                                 "<select id='local_extif' name='local_extif' >"+
                                 "</select>"+
                                 "</td></tr>");

    var netmask_last = [ "0", "128", "192", "224", "240", "248", "252", "254", "255" ];
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Local Subnet IP");%>:</td><td><input id='local_inipaddr' name='local_inipaddr' size='17' maxlength='30' onchange='valid_ipaddr(this, \"Local IP Address\")' value='<%nv_attr_get("lan_static_rule", i, "ipaddr");%>'/></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Local Netmask");%>:</td><td><input id='local_netmask' name='local_netmask' size='17' maxlength='30' onchange='valid_ipaddr(this, \"Local Netmask\")' value='255.255.255.0' /></td></tr>");

    for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
        $('#local_extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
    }

    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Remote Gateway");%>:</td><td><input id='remote_gateway' name='remote_gateway' size='17' maxlength='30' onchange='valid_name(this, \"Remote Gateway\")' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Remote Subnet IP");%>:</td><td><input id='remote_inipaddr' name='remote_inipaddr' size='17' maxlength='30' onchange='valid_ipaddr(this, \"Remote Subnet IP\")' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Remote Netmask");%>:</td><td><input id='remote_netmask' name='remote_netmask' size='17' maxlength='30' onchange='valid_ipaddr(this, \"Remote Netmask\")' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Connection Initiation");%>:</td><td><input id='conn_init' name='conn_init' type='checkbox' value='1' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("IKE Key Mode");%>:</td><td>"+
                               "<select id='ike_keymode' name='ike_keymode' ><option value='psk'>PSK</option></select>"+
                               "</td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Preshared Key");%>:</td><td><input id='psk' name='psk' size='25' maxlength='40' /></td></tr>");
    $("#IPsecDataForm").append("<tr><td align='right'><%lang("DPD Enable");%>:</td><td><input id='dpd_enable' name='dpd_enable' type='checkbox' onchange='DPD_Change()' value='1' /></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("DPD Interval");%>:</td><td><input id='dpd_interval' name='dpd_interval' size='5' maxlength='5' /><%lang("Seconds");%> (10 ~ 1200)</td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("DPD Timeout");%>:</td><td><input id='dpd_timeout' name='dpd_timeout' size='5' maxlength='5' /><%lang("Seconds");%> (30 ~ 3600)</td></tr>");

    $("#IPsecDataForm").append("<tr><td align='right'><%lang("Advanced Options");%>:</td><td><input id='adv' name='adv' type='checkbox' onchange='ADV_Change()' value='1' /></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Mode");%>:</td><td>"+
                               "<select id='phase1_mode' name='phase1_mode' >"+
                               "<option value='main'>Main</option>"+
                               "<option value='aggressive'>Aggressive</option>"+ 
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Local ID");%>:</td><td><input id='phase1_left_id' name='phase1_left_id' size='17' maxlength='15' onblur='' /></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Remote ID");%>:</td><td><input id='phase1_right_id' name='phase1_right_id' size='17' maxlength='15' onblur='' /></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Lifetime");%>':</td><td><input id='phase1_lifetime' name='phase1_lifetime' size='5' maxlength='5' /><%lang("Seconds");%> (3600 ~ 28800)</td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 2 Lifetime");%>:</td><td><input id='phase2_lifetime' name='phase2_lifetime' size='5' maxlength='5' /><%lang("Seconds");%> (3600 ~ 28800)</td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Authentication");%>:</td><td>"+
                               "<select id='phase1_auth' name='phase1_auth' >"+ 
                               "<option value='md5'>MD5</option>"+
                               "<option value='sha1'>SHA1</option>"+
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Encryption");%>:</td><td>"+
                               "<select id='phase1_encrypt' name='phase1_encrypt' >"+
                               "<option value='des'>DES</option>"+
                               "<option value='3des'>3DES</option>"+
                               "<option value='aes128'>AES</option>"+
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 1 Group Key Management");%>:</td><td>"+
                               "<select id='phase1_group' name='phase1_group' >"+
                               "<option value='dh1'>DH1</option>"+
                               "<option value='dh2'>DH2</option>"+
                               "<option value='dh5'>DH5</option>"+
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 2 Authentication");%>:</td><td>"+
                               "<select id='phase2_auth' name='phase2_auth' >"+
                               "<option value='md5'>MD5</option>"+
                               "<option value='sha1'>SHA1</option>"+
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 2 Encryption");%>:</td><td>"+
                               "<select id='phase2_encrypt' name='phase2_encrypt' >"+ 
                               "<option value='null'>Null</option>"+
                               "<option value='des'>DES</option>"+
                               "<option value='3des'>3DES</option>"+
                               "<option value='aes128'>AES</option>"+
                               "</select></td></tr>");
    $("#IPsecDataForm").append("<tr style='display: none'><td align='right'><%lang("Phase 2 Group Key Management (PFS)");%>:</td><td>"+
                               "<select id='phase2_group' name='phase2_group' >"+
                               "<option value='none'>None</option>"+
                               "<option value='dh1'>DH1</option>"+
                               "<option value='dh2'>DH2</option>"+
                               "<option value='dh5'>DH5</option>"+
                               "</select></td></tr>");

}//End CreateIPsecDataForm

$(document).ready(function(){
    var IPsec_rows = GetGridData();

    jQuery('#IPsecListData').jqGrid({
            data: IPsec_rows,
            datatype: 'local',
            colNames:["<%lang("Connection Name");%>", "<%lang("Rule Enable");%>", "<%lang("External Interface");%>", "<%lang("Remote Gateway");%>", "<%lang("Remote Subnet IP");%> / <%lang("Netmask");%>", "<%lang("Phase 1");%>", "<%lang("Phase 2");%>"],
            colModel:[
                      {name:'name',index:'name', width:5, align:"center", sortable:false, editable:true},
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'local_extif',index:'local_extif', width:3, align:"center", sortable:false, editable:true},
                      {name:'remote_gateway',index:'remote_gateway', width:5, align:"center", sortable:false, editable:true},
                      {name:'remote_inipaddr',index:'remote_inipaddr', width:8, align:"center", sortable:false, editable:true},
                      {name:'phase1_status',index:'phase1_status', width:3, align:"center", sortable:false, editable:true},
                      {name:'phase2_status',index:'phase2_status', width:3, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#IPsecListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("User Rule");%>"
    });

    jQuery("#IPsecListData").jqGrid('navGrid','#IPsecListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#IPsecListData").navButtonAdd('#IPsecListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#IPsecDataForm").empty();
            CreateIPsecDataForm();

            $( "#IPsecListData-dialog" ).dialog({
                resizable: false,
                height:700,
                width:500,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        ipsec_rule.push(Array_Tmp);
                        var IPsec_rows = GetGridData();
                        $("#IPsecListData").jqGrid("clearGridData", true);
                        jQuery("#IPsecListData").setGridParam({ 'data':  IPsec_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#IPsecListData").navButtonAdd('#IPsecListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#IPsecDataForm").empty();
            var rowid = $("#IPsecListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                ipsec_rule.splice(DelItem,1);
            }
            var IPsec_rows = GetGridData();
            $("#IPsecListData").jqGrid("clearGridData", true);
            jQuery("#IPsecListData").setGridParam({ 'data':  IPsec_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#IPsecListData").navButtonAdd('#IPsecListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#IPsecDataForm").empty();
            CreateIPsecDataForm();
            var rowid = $("#IPsecListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#name").val(ipsec_rule[ModifyIndex][enum_ipsec._NAME]);
            
            if (ipsec_rule[ModifyIndex][enum_ipsec._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }

            $("select[name='mode']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._MODE]);
            if (ipsec_rule[ModifyIndex][enum_ipsec._MODE] == "rw_server"){
                $("#l2tp").parents("tr").show();
                $("#remote_gateway").parents("tr").hide();
                $("#remote_inipaddr").parents("tr").hide();
                $("#remote_netmask").parents("tr").hide();
                $("#conn_init").parents("tr").hide();
                $('#conn_init').attr('checked', false);
            } else {
                $("#l2tp").parents("tr").hide();
                $("#remote_gateway").parents("tr").show();
                $("#remote_inipaddr").parents("tr").show();
                $("#remote_netmask").parents("tr").show();
                $("#conn_init").parents("tr").show();
            }

            if (ipsec_rule[ModifyIndex][enum_ipsec._L2TP] == 1){
                $("#l2tp").attr('checked',true);
                $("#local_inipaddr").parents("tr").hide();
                $("#local_netmask").parents("tr").hide();
            }else{
                $("#l2tp").attr('checked',false);
                $("#local_inipaddr").parents("tr").show();
                $("#local_netmask").parents("tr").show();
            }

            $("select[name='local_extif']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._LOCAL_EXTIF]);
            $("#local_inipaddr").val(ipsec_rule[ModifyIndex][enum_ipsec._LOCAL_INIPADDR]);
            $("#local_netmask").val(ipsec_rule[ModifyIndex][enum_ipsec._LOCAL_NETMASK]);
            $("#remote_gateway").val(ipsec_rule[ModifyIndex][enum_ipsec._REMOTE_GATEWAY]);
            $("#remote_inipaddr").val(ipsec_rule[ModifyIndex][enum_ipsec._REMOTE_INIPADDR]);
            $("#remote_netmask").val(ipsec_rule[ModifyIndex][enum_ipsec._REMOTE_NETMASK]);

            if (ipsec_rule[ModifyIndex][enum_ipsec._CONN_INIT] == 1){
                $("#conn_init").attr('checked',true);
            }else{
                $("#conn_init").attr('checked',false);
            }

            $("select[name='ike_keymode']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._IKE_KEYMODE]);
            $("#psk").val(ipsec_rule[ModifyIndex][enum_ipsec._PSK]);
            if (ipsec_rule[ModifyIndex][enum_ipsec._DPD_ENABLE] == 1){
                $("#dpd_enable").attr('checked',true);
                $("#dpd_interval").parents("tr").show();
                $("#dpd_timeout").parents("tr").show();
            }else{
                $("#dpd_enable").attr('checked',false);
                $("#dpd_interval").parents("tr").hide();
                $("#dpd_timeout").parents("tr").hide();
            }

            $("#dpd_interval").val(ipsec_rule[ModifyIndex][enum_ipsec._DPD_INTERVAL]);
            $("#dpd_timeout").val(ipsec_rule[ModifyIndex][enum_ipsec._DPD_TIMEOUT]);

            if (ipsec_rule[ModifyIndex][enum_ipsec._ADV] == 1){
                $("#adv").attr('checked',true);
                $("#phase1_mode").parents("tr").show();
                $("#phase1_left_id").parents("tr").show();
                $("#phase1_right_id").parents("tr").show();
                $("#phase1_lifetime").parents("tr").show();
                $("#phase2_lifetime").parents("tr").show();
                $("#phase1_auth").parents("tr").show();
                $("#phase1_encrypt").parents("tr").show();
                $("#phase1_group").parents("tr").show();
                $("#phase2_auth").parents("tr").show();
                $("#phase2_encrypt").parents("tr").show();
                $("#phase2_group").parents("tr").show();

            }else{
                $("#adv").attr('checked',false);
                $("#phase1_mode").parents("tr").hide();
                $("#phase1_left_id").parents("tr").hide();
                $("#phase1_right_id").parents("tr").hide();
                $("#phase1_lifetime").parents("tr").hide();
                $("#phase2_lifetime").parents("tr").hide();
                $("#phase1_auth").parents("tr").hide();
                $("#phase1_encrypt").parents("tr").hide();
                $("#phase1_group").parents("tr").hide();
                $("#phase2_auth").parents("tr").hide();
                $("#phase2_encrypt").parents("tr").hide();
                $("#phase2_group").parents("tr").hide();
            }

            $("select[name='phase1_mode']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_MODE]);
            $("#phase1_left_id").val(ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_LEFT_ID]);
            $("#phase1_right_id").val(ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_RIGHT_ID]);
            $("#phase1_lifetime").val(ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_LIFETIME]);
            $("#phase2_lifetime").val(ipsec_rule[ModifyIndex][enum_ipsec._PHASE2_LIFETIME]);
            $("select[name='phase1_auth']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_AUTH]);
            $("select[name='phase1_encrypt']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_ENCRYPT]);
            $("select[name='phase1_group']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE1_GROUP]);
            $("select[name='phase2_auth']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE2_AUTH]);
            $("select[name='phase2_encrypt']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE2_ENCRYPT]);
            $("select[name='phase2_group']").attr("value", ipsec_rule[ModifyIndex][enum_ipsec._PHASE2_GROUP]);
            
            $( "#IPsecListData-dialog" ).dialog({
                resizable: false,
                height:700,
                width:500,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        ipsec_rule.splice(ModifyIndex,1, Array_Tmp);

                        var IPsec_rows = GetGridData();
                        $("#IPsecListData").jqGrid("clearGridData", true);
                        jQuery("#IPsecListData").setGridParam({ 'data':  IPsec_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    jQuery("#IPsecListData").navButtonAdd('#IPsecListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#IPsecDataForm").empty();
            var rowid = $("#IPsecListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = ipsec_rule[UpItem];
                ipsec_rule[UpItem] = ipsec_rule[UpItem - 1];
                ipsec_rule[UpItem - 1] = arr_tmp;
            }
            var IPsec_rows = GetGridData();
            $("#IPsecListData").jqGrid("clearGridData", true);
            $("#IPsecListData").setGridParam({ 'data':  IPsec_rows }).trigger("reloadGrid");
            $("#IPsecListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#IPsecListData").navButtonAdd('#IPsecListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#IPsecDataForm").empty();
            var rowid = $("#IPsecListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == ipsec_rule.length-1){
                    return false;
                }
                var arr_tmp = ipsec_rule[DownItem];
                ipsec_rule[DownItem] = ipsec_rule[DownItem + 1];
                ipsec_rule[DownItem + 1] = arr_tmp;
            }
            var IPsec_rows = GetGridData();
            $("#IPsecListData").jqGrid("clearGridData", true);
            $("#IPsecListData").setGridParam({ 'data':  IPsec_rows }).trigger("reloadGrid");
            $("#IPsecListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End Ready

</script>
<% tbl_get("ipsec_rule"); %>
</head>
  <body class="gui" onload="init();">
  <div id="IPsecListData-dialog" title="IPsec Data" style="display: none">
    <table id="IPsecDataForm"></table>
 </div>
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
              <table id="IPsecListData"></table>
              <div id="IPsecListDataPager"></div>
               <%tbl_create_table("ipsec_table", "ipsec_rule", "1");%>
                <br />

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
