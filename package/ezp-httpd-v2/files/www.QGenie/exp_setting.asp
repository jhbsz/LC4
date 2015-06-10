
<!DOCTYPE html>
<html lang="en">
	<head>
		
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Setting</title>
		
		<%do_headmeta();%>
		<%do_basic_css();%>

		<%do_ace_font_css();%>
		<%do_ace_font_ie7_css();%>

        <%do_ace_basic_css();%>
		<%do_ace_basic_ie9_css();%>
		<%do_custom_css();%>

        <%do_custom_js();%>
		<%do_basic_js();%>

		<!-- page specific plugin scripts -->
		
		<!--[if lt IE 9]>
		<script type="text/javascript" src="js/excanvas.min.js"></script>
		<![endif]-->

		<%do_jquery_ui_js();%>

		<!-- ace scripts -->
		<%do_ace_basic_js();%>

    <script type="text/javascript">
    var sel_vpn_val = new Array(6);
    var sel_vpn_profile_name = new Array(6);
    var sel_vpn_ip = new Array(6);
    var sel_vpn_name = new Array(6);
    var sel_vpn_password = new Array(6);
    // var all_vpn_enable = 0 ;
    var vpn_enabled = "<%nvg_attr_get("", "wan_pptp_l2tp_global_rule", "0", "enable");%>";

    sel_vpn_val[0] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "0","enable");%>";
    sel_vpn_profile_name[0] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","0","name");%>');
    sel_vpn_ip[0] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","0","host");%>');
    sel_vpn_name[0] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","0","username");%>');
    sel_vpn_password[0] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","0","passwd");%>');

    sel_vpn_val[1] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "1","enable");%>";
    sel_vpn_profile_name[1] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","1","name");%>');
    sel_vpn_ip[1] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","1","host");%>');
    sel_vpn_name[1] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","1","username");%>');
    sel_vpn_password[1] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","1","passwd");%>');

    sel_vpn_val[2] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "2","enable");%>";
    sel_vpn_profile_name[2] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","2","name");%>');
    sel_vpn_ip[2] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","2","host");%>');
    sel_vpn_name[2] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","2","username");%>');
    sel_vpn_password[2] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","2","passwd");%>');

    sel_vpn_val[3] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "3","enable");%>";
    sel_vpn_profile_name[3] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","3","name");%>');
    sel_vpn_ip[3] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","3","host");%>');
    sel_vpn_name[3] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","3","username");%>');
    sel_vpn_password[3] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","3","passwd");%>');

    sel_vpn_val[4] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "4","enable");%>";
    sel_vpn_profile_name[4] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","4","name");%>');
    sel_vpn_ip[4] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","4","host");%>');
    sel_vpn_name[4] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","4","username");%>');
    sel_vpn_password[4] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","4","passwd");%>');

    sel_vpn_val[5] = "<%nvg_attr_get("","wan_pptp_l2tp_rule", "5","enable");%>";
    sel_vpn_profile_name[5] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","5","name");%>');
    sel_vpn_ip[5] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","5","host");%>');
    sel_vpn_name[5] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","5","username");%>');
    sel_vpn_password[5] = decode64('<%b64_nvg_attr_get("","wan_pptp_l2tp_rule","5","passwd");%>');
    
    function sel_vpn(I){
        if(I == 1){
            document.getElementById('vpn_enable_id').value = '1';
            document.getElementsByName('vpn_enable_radio')[0].checked = true;
            document.getElementsByName('vpn_enable_radio')[1].checked = false;
        } else {
            document.getElementById('vpn_enable_id').value='0';
            document.getElementsByName('vpn_enable_radio')[0].checked = false;
            document.getElementsByName('vpn_enable_radio')[1].checked = true;

        }
    }

    function setLanguage(lang){
    /*  setCookie("language", lang);*/
        var data_array = {};
        data_array['action'] = 'lang';
        data_array['lang'] = lang;
        data_array['sub_button'] = 'exp_setting';
        do_ajax_action ("apply.cgi", "POST", "text", true, data_array, 3000);
    }

    function sel_profile(I) {
        if(I == 1){
            document.getElementsByName('vpn_profile_enable_radio')[0].checked = true;
            document.getElementsByName('vpn_profile_enable_radio')[1].checked = false;
            document.getElementById('vpn_profile_enable').value='1';
        } else {
            document.getElementsByName('vpn_profile_enable_radio')[0].checked = false;
            document.getElementsByName('vpn_profile_enable_radio')[1].checked = true;
            document.getElementById('vpn_profile_enable').value='0';
        }
   }

   function file_select_icon() {
        $('#binfile').ace_file_input({
				no_file:'No File ...',
				btn_choose:'Choose',
				btn_change:'Change',
				droppable:false,
				onchange:null,
				thumbnail:false //| true | large
				//whitelist:'gif|png|jpg|jpeg'
				//blacklist:'exe|php'
				//onchange:''
				//
		});
	}
    function sel_guest_access(I){
        if(I == '1'){
            document.getElementById('allow_guest').value = '1';
        } else {
            document.getElementById('allow_guest').value = '0';
        }
    }
    function sel_ftp_access(I){
        if(I == '1'){
            document.getElementById('allow_ftpd').value = '1';
        } else {
            document.getElementById('allow_ftpd').value = '0';
        }
    }
	    function clock() {
            var t = ((new Date()).getTime() - startTime) / 1000;
            $("#upgrade-time").text(Math.floor(t / 60) + ':' + pad(Math.floor(t % 60)));
        }

        function disp_confirm(){
            return confirm("<%lang("Are you sure you want to restore to the default configuration?");%>");
        }

        function to_default(F) {

            $("#submit_button").val("exp_setting");
            $("#action").val("Default");
            if (disp_confirm() == false) {
                return;
            }
            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("default.cgi", "POST", "text", true, ajaxData, 3000);
            return true;
        }

        function to_upgrade(F) {
            $("#cfgfile").attr("disabled", true);
            name = $("#binfile").val();
    
            if (name.search(/\.(bin)$/i) == -1) {
                alert('A ".bin" file expected.');
                return;
            }
            if (CheckBattery()==false) {
                 alert("<% lang("The system cannot update when the battery is under 30%."); %>\n <% lang("Battery level"); %>: " + BatteryLevel + "%");
                 return;
            }
            if (!confirm('<%lang("Are you sure you want to upgrade using");%> ' + name + '?')) 
                return;
    
            startTime = (new Date()).getTime();
            setInterval('clock()', 1000);
            F = document.getElementsByName('setting_form')[0];
            $("#upgrade").attr("disabled", true);
            F.getAttributeNode("action").value = "upgrade.cgi";
            F.encoding = "multipart/form-data";
            F.submit_button.value = "exp_setting";
            open_upgrade_window();
            F.submit();

        }
      
        var BatteryLevel=0;
        function CheckBattery(){
            BatteryAvailable = false;
            $.ajax({
                url:        "percentage.cgi",
                type:       "POST",
                dataType:   "text",
                async:      false,
                data:       {},
                success: function(data){
                    if (data>=30){
                        BatteryAvailable = true;
                    } else {
                        BatteryAvailable = false;
                    }
                    BatteryLevel = data;
                },
                error: function(xhr, textStatus, error){
                    //console.log(xhr.statusText, textStatus, error);
                }
            });
            return BatteryAvailable;
        }
        function disp_reboot(){
            return confirm("<%lang("Are you sure you want to reboot?"  );%>");
        }

        
        function to_reboot(F) {
            $("#submit_button").val("exp_setting");
            $("#action").val("Reboot");
            if (disp_reboot() == false) { 
                return;
            }       

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("reboot.cgi", "POST", "text", true, ajaxData, 3000);
            return true;
        } 

    function SelDNSType() {
        if ($("#dhcp0_dnstype").val() == "dnsrelay") {
            $("#dhcp0_dnsaddr1").attr("disabled",true);
            $("#dhcp0_dnsaddr2").attr("disabled",true);
        }else if ($("#dhcp0_dnstype").val() == "custom") {
            $("#dhcp0_dnsaddr1").attr("disabled",false);
            $("#dhcp0_dnsaddr1").val("<%nvg_attr_get("dhcp0_dnsaddr1", "lan_dhcps_rule", "0", "dnsaddr1");%>");
            $("#dhcp0_dnsaddr2").attr("disabled",false);
            $("#dhcp0_dnsaddr2").val("<%nvg_attr_get("dhcp0_dnsaddr2", "lan_dhcps_rule", "0", "dnsaddr2");%>");
        }
    }

    function valid_password(I, J) {
        if(I.length==0 || I.length>=31) {
            alert('<%lang("The password cannot be longer than 31 characters.");%>');
            return false;
        }
        for(i=0 ; i<I.length; i++){
            ch = I.charAt(i);
            if((ch > '9' || ch < '0') && (ch > 'z' || ch < 'a') && (ch > 'Z' || ch < 'A') && (ch != '-') && (ch != '_') && (ch != '(') && (ch != ')') && (ch != ' ')){
                alert('<%lang("The allowed characters are: A-Z, a-z, 0-9, - _ () and spaces.");%>');
                return false;
            }
        }
        if(I.charAt(0)==' ' || I.charAt(I.length-1)==' ') {
            alert('<%lang("The first and final characters must not be spaces.");%>');
            return false;
        }

        //if (F.http_passwd.value != F.http_passwd_confirm.value) {
        if (I != J){
            alert("<%lang("Please enter the same password in both fields.");%>");
            $("#http_passwd").focus();
            $("#http_passwd").select();
            //F.http_passwd_confirm.focus();
            //F.http_passwd_confirm.select();
            return false;
        }
        if ($("#vpn_password").val() != $("#vpn_Re_type_password").val()){
            alert("<%lang("Confirmed VPN user password did not match Entered Password.  Please re-enter VPN user password");%>");
            $("#vpn_password").focus();
            $("#vpn_password").select();
            return false;
        }

        return true;
    }

    function valid_ssid_char(I) {
        if(I.length==0 || I.length>=31) {
            alert('<%lang("The SSID name cannot be longer than 31 characters. The first and final characters must not be spaces.");%>');
            return false;
        }
        for(i=0 ; i<I.length; i++){
            ch = I.charAt(i);
            if((ch > '9' || ch < '0') && (ch > 'z' || ch < 'a') && (ch > 'Z' || ch < 'A') && (ch != '-') && (ch != '_') && (ch != '(') && (ch != ')') && (ch != ' ')){
                alert('<%lang("The allowed characters are: A-Z, a-z, 0-9, - _ () and spaces.");%>');
                return false;
            }
        }
        if(I.charAt(0)==' ' || I.charAt(I.length-1)==' ') {
            alert('<%lang("The SSID name cannot be longer than 31 characters. The first and final characters must not be spaces.");%>');
            return false;
        }
        return true;
    }

    function vpn_valid_check(){
        if ($("#vpn_ip_server").val() == ''){
            alert("<%lang("Confirmed VPN Server can not be empty.  Please enter VPN Server");%>");
            $("#vpn_ip").focus();
            $("#vpn_ip").select();
            return false;

        }
        if ($("#vpn_name").val() == ''){
            alert("<%lang("Confirmed VPN User Name can not be empty.  Please enter VPN User Name");%>");
            $("#vpn_name").focus();
            $("#vpn_name").select();
            return false;
        }
        if ($("#vpn_password").val() == ''){
            alert("<%lang("Confirmed VPN User password can not be empty.  Please enter VPN user password");%>");
            $("#vpn_password").focus();
            $("#vpn_password").select();
            return false;
        }
        return true;
    }   

    function vpn_select_fn(obj){
        var j = obj - 1;
        document.getElementById('vpn_ip_server').value = sel_vpn_ip[j];
        document.getElementById('vpn_profile_name').value = sel_vpn_profile_name[j];
        document.getElementById('vpn_name').value = sel_vpn_name[j];
        document.getElementById('vpn_password').value = sel_vpn_password[j];
        document.getElementById('vpn_Re_type_password').value = sel_vpn_password[j];
        sel_profile(sel_vpn_val[j]);
    }

    var F;
    function to_submit(F) {
            
        // check ssid 
        var ssid = $("#wl0_ssid").val();
        if(!valid_ssid_char(ssid)) return;

        // check password
        var http_passwd = $("#http_passwd").val();
        var http_passwd_confirm = $("#http_passwd_confirm").val();
        if(!valid_password(http_passwd, http_passwd_confirm)) return;

        // if(!vpn_valid_check(F)) return;
        var en64key_vpn_ip_server;
        var en64key_vpn_name;
        var en64key_vpn_password;
        var en64key_vpn_Re_type_password;
        if($("#vpn_ip_server").val() != "") {
            en64key_vpn_ip_server = encode64($("#vpn_ip_server").val());
            $("#vpn_ip_server").val(en64key_vpn_ip_server);
        }
        if($("#vpn_name").val() != "") {
            en64key_vpn_name = encode64($("#vpn_name").val());
            $("#vpn_name").val(en64key_vpn_name);
        }
        if($("#vpn_password").val() != "") {
            en64key_vpn_password = encode64($("#vpn_password").val());
            $("#vpn_password").val(en64key_vpn_password);
        }
        if($("#vpn_Re_type_password").val() != "") {
            en64key_vpn_Re_type_password = encode64($("#vpn_Re_type_password").val());
            $("#vpn_Re_type_password").val(en64key_vpn_Re_type_password);
        }
        if($("#vpn_profile_name").val() != "") {
            en64key_vpn_name = encode64($("#vpn_profile_name").val());
            $("#vpn_profile_name").val(en64key_vpn_name);
        }
        
        var en64key;
        if($("#wl0_ssid0_secmode").val()=="disabled"){
            SaveEXP();
        }else if($("#wl0_ssid0_secmode").val()=="wep"){
            PasswdVal = $("#wl0_ssid0_key").val();
            MsgVal = "WEP Password";
            if (PasswdVal.length==0){
                $("#WEPErrorMsg").text("( <%lang("5 or 13 characters");%> )");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            if(valid_wl_sec_wep_key(PasswdVal, MsgVal, 0)){
                $("#WEPErrorMsg").text("");

                if($("#wl0_ssid0_key").val()!= $("#wl0_ssid0_keyc").val()){
                    $("#WEPErrorMsg").text("<%lang("The password does not match.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            en64key = encode64($("#wl0_ssid0_key").val());
            $("#wl0_ssid0_key").val(en64key);
            SaveEXP();
        }else if($("#wl0_ssid0_secmode").val()=="psk"){
            PasswdVal = $("#wl0_ssid0_key").val();
            MsgVal = "WPA Password";
            if (PasswdVal.length==0){
                $("#WPAErrorMsg").text("( <%lang("8-63 characters");%> )");
                $("#wl0_ssid0_key").val("");
                return
            }
            if(valid_wl_sec_wpa_psk_key(PasswdVal, MsgVal, 0)){
                $("#WPAErrorMsg").text("");

                if($("#wl0_ssid0_key").val()!= $("#wl0_ssid0_keyc").val()){
                    $("#WPAErrorMsg").text("<%lang("The password does not match.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            en64key = encode64($("#wl0_ssid0_key").val());
            $("#wl0_ssid0_key").val(en64key);
            SaveEXP();
        }else if($("#wl0_ssid0_secmode").val()=="psk2"){
            PasswdVal = $("#wl0_ssid0_key").val();
            MsgVal = "WPA Password";
            if (PasswdVal.length==0){
                $("#WPAErrorMsg").text("( <%lang("8-63 characters");%> )");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            if(valid_wl_sec_wpa_psk_key(PasswdVal, MsgVal, 0)){
                $("#WPAErrorMsg").text("");

                if($("#wl0_ssid0_key").val()!= $("#wl0_ssid0_keyc").val()){
                    $("#WPAErrorMsg").text("<%lang("The password does not match.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            en64key = encode64($("#wl0_ssid0_key").val());
            $("#wl0_ssid0_key").val(en64key);
            SaveEXP();
        }
    }

    function SaveEXP(){
        $("#submit_button").val("exp_setting");
        $("#action").val("Apply");
        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#setting_form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 5000);
    }

    function SelEWL(F) {
        F.submit_button.value = "exp_setting";
        F.action.value = "Gozila";
        F.submit();
    }

    $(document).ready(function(){
            var vpn_enable_init = 0;
            file_select_icon();
            for(var i = 0; i < 6 ; i++){
                var j = i + 1;
                $("#vpn_select").append("<option value='"+j+"'>"+j+"</option>");
                if(sel_vpn_val[i] == '1'){
                    vpn_enable_init = j;
                    // all_vpn_enable = 1;
                }
            }
                                                                 
            if(vpn_enable_init == 0){
                vpn_enable_init = 1;
                document.getElementById('vpn_select').value = vpn_enable_init;
            } else{
                document.getElementById('vpn_select').value = vpn_enable_init;
            }
            // document.getElementById('vpn_enable_id').value = all_vpn_enable;
            document.getElementById('vpn_enable_id').value = vpn_enabled;
            vpn_select_fn(vpn_enable_init);

            /*
            if(all_vpn_enable == '1'){
                document.getElementById('vpn_enable_radio').checked = 'checked';
            } else{
                document.getElementById('vpn_enable_radio').checked = 'checked';
            }

            sel_vpn(all_vpn_enable);
            */
            sel_vpn(vpn_enabled);

            var curIP = "<%nvg_attr_get("","lan_static_rule", "0", "ipaddr");%>";
            var IPArr = curIP.split(".");
            for (var i=0; i<255 ; i++ ){
                if (IPArr[2]==i) {
                    $("#lan0_static_ipclass").append("<option value='"+i+"' selected>"+i+"</option>");
                } else {
                    $("#lan0_static_ipclass").append("<option value='"+i+"'>"+i+"</option>");
                }
            }

            $("#lan0_static_ipclass").change(function() {
                    $("#lan0_static_ipaddr").val("192.168." + $("#lan0_static_ipclass").val() + ".1");
                    $("#lan0_static_ipaddr_t").val("192.168." + $("#lan0_static_ipclass").val() + ".1");
                    $("#dhcp0_display_start").val("192.168." + $("#lan0_static_ipclass").val() + ".");
                    });

            var curLease = "<%nvg_attr_get("dhcp0_lease", "lan_dhcps_rule", "0", "lease");%>";
            var DisplayLease = curLease / 60 /60;
            $("#dhcp0_lease_display").val(DisplayLease+" <%lang("hours");%>");

            var curStartIP = "<%nvg_attr_get("dhcp0_start", "lan_dhcps_rule", "0", "start");%>";
            for (var i=2; i<255-<%nvg_attr_get("", "lan_dhcps_rule", "0", "num");%>; i++ ){
                var IPRange = i + <%nvg_attr_get("", "lan_dhcps_rule", "0", "num");%>;
                if (curStartIP == i){
                    $("#dhcp0_start").append("<option value='"+i+"' selected>"+ i + "-" +IPRange+"</option>");
                } else {
                    $("#dhcp0_start").append("<option value='"+i+"'>"+ i + "-" +IPRange+"</option>");
                }
            }

    });


    </script>

	</head>

	<body>
	  <%do_bootstrap_navbar_menu();%>
        <script type="text/javascript">create_waiting_window();</script>
        <script type="text/javascript">create_upgrade_window();</script>
        <form class="form-horizontal" name="setting_form" id="setting_form" action="apply.cgi" method="post">
		<div class="main-container container-fluid" id="main-container">
            <a id="menu-toggler" class="menu-toggler" href="#"><span class="menu-text"></span></a>
			<div id="sidebar" class="sidebar">
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'fixed')}catch(e){}
                </script>
			    <%do_bootstrap_menu();%>
                <div id="sidebar-collapse" class="sidebar-collapse"><i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right" ></i></div>
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'collapsed')}catch(e){}
                </script>
			</div><!--/#sidebar-->
		
			<div id="main-content" class="main-content">
					
					<div id="breadcrumbs" class="breadcrumbs">
						<ul class="breadcrumb">
							<li><i class="icon-home"></i> Home<span class="divider"><i class="icon-angle-right"></i></span></li>
							<li class="active"><%lang("Setting");%></li>
						</ul><!--.breadcrumb-->


					</div><!--breadcrumbs-->



					<div id="page-content" class="page-content">
						
						<div class="page-header position-relative">
							<h1><%lang("Setting");%> <small><i class="icon-double-angle-right"></i> </small></h1>
							 <script type="text/javascript">do_hidden_elements('exp_lan_rule');</script>
						</div><!--/page-header-->
						

						<div class="row-fluid">
        <!-- PAGE CONTENT BEGINS HERE -->


        <!--row1-->
        <div class="row-fluid">

            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-lock'></i>&nbsp;<%lang("Device");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label for="wl0_ssid"><% lang("Device Name"); %></label>
                                <input type="text" name="wl0_ssid" id="wl0_ssid" style="width: 173px;" value='<% nvg_attr_get("wl0_ssid","wl0_ssid_rule","0","ssid");%>'></input>
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->

            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-link'></i>&nbsp;<%lang("DHCP");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label for="dhcp0_display_start"><% lang("IP Address Pool"); %></label>
                                <input type="text" style="width: 85px;" name="dhcp0_display_start" id="dhcp0_display_start" value='<%show_ipaddr_prefix24("lan0_ipaddr");%>' readonly />
						<select id="dhcp0_start" name="dhcp0_start" value='<%nvg_attr_get("dhcp0_start", "lan_dhcps_rule", "0", "start");%>' style="width: 85px;">
						</select>
                            </div>

                            <input type="hidden" id="dhcp0_dnstype" name="dhcp0_dnstype" value='<%nvg_attr_get("dhcp0_dnstype", "lan_dhcps_rule", "0", "dnstype");%>' />
                            <input type="hidden" id="dhcp0_dnsaddr1" name="dhcp0_dnsaddr1" value='<%nvg_attr_get("dhcp0_dnsaddr1", "lan_dhcps_rule", "0", "dnsaddr1");%>' />
                            <input type="hidden" id="dhcp0_dnsaddr2" name="dhcp0_dnsaddr2" value='<%nvg_attr_get("dhcp0_dnsaddr2", "lan_dhcps_rule", "0", "dnsaddr2");%>' />
                        </div>
                    </div>
                </div>
            </div><!--/span-->

            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-sitemap'></i>&nbsp;<%lang("LAN");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label for="lan0_static_ipclass"><% lang("Network Address"); %></label>
                                192.168.
                                <select name="lan0_static_ipclass" id="lan0_static_ipclass" style="width: 65px;">
                                </select>
                                .1
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->

        </div>
        <!--row2-->
        <div class="row-fluid">

            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-lock'></i>&nbsp;<%lang("Wireless Security");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label for="wl0_ssid0_secmode"><% lang("Security Type"); %></label>
                                <select name="wl0_ssid0_secmode" id="wl0_ssid0_secmode" style="width: 188px;" onchange="SelEWL(this.form)">
                                    <option value="disabled" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "disabled", "selected");%>><% lang("Disable"); %></option>
                                    <option value="wep" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
                                    <option value="psk" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk", "selected");%>><% lang("WPA"); %></option>
                                    <option value="psk2" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk2", "selected");%>><% lang("WPA2"); %></option>
                                </select>
                                <span class="help-block">( <% lang("WPA2 is recommended for the highest security."); %> )</span>
                            </div>
                            <% wlv_sec_show_setting(0, 0);%>
                        </div>
                    </div>
                </div>
            </div><!--/span-->


            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-user'></i>&nbsp;<%lang("Admin Password");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label for="http_passwd"><% lang("Password"); %></label>
                                <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd" id="http_passwd" style="width: 173px;">
                            </div>

                            <div class="row-fluid">
                                <label for="http_passwd_confirm"><% lang("Re-type Password"); %></label>
                                <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd_confirm" id="http_passwd_confirm" style="width: 173px;">                 
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->

            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-undo'></i>&nbsp;<% lang("System"); %></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label ><% lang("Reset to default settings"); %></label>
                                <input type="button" class="btn btn-small btn-primary" value='<%lang("Reset");%>' onclick="to_default(this.form)">
                            </div>

                            <div class="row-fluid">
                                <label ><% lang("System Reboot"); %></label>
                                <input type="button" class="btn btn-small btn-primary" value='<%lang("Reboot");%>' onclick="to_reboot(this.form)">
                            </div>

                        </div>
                    </div>
                </div>
            </div><!--/span-->
        </div>

        <!--row3-->
        <div class="row-fluid">
            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-download'></i>&nbsp;<% lang("Firmware"); %></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label ><% lang("Version"); %></label>
                                <% nv_get("fw_version"); %>
                            </div>
                            <div class="row-fluid">
                                <label for="http_passwd_confirm"><% lang("Upgrade"); %></label>
                                <input type="file" name="binfile" id="binfile" />
                                <input type="button" class="btn btn-small btn-primary" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->
            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-user'></i>&nbsp;<% lang("Guest Access"); %></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label ><% lang("Allow guests to login and access files on QGenie."); %></label>
                                <input name='radio_allow_guest' id='radio_allow_guest' value='1'
                                       type="radio" <% nv_match("allow_guest", "1", "checked"); %> 
                                       onclick='sel_guest_access("1")' >
                                       <%lang("Allow");%> </input>
                                <input name='radio_allow_guest' id='radio_allow_guest' value='0'
                                       type='radio' <% nv_match("allow_guest", "0", "checked"); %> 
                                       onclick='sel_guest_access("0")' > 
                                       <%lang("Deny");%> </input>
                                <input name='allow_guest' id='allow_guest' value='<% nv_get("allow_guest"); %>' type='hidden'></input>
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->
            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-user'></i>&nbsp;<% lang("QGenie FTP Service"); %></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label ><% lang("Allow user to use FTP service on QGenie."); %></label>
                                <input name='radio_allow_ftp_access' id='radio_allow_ftp_access' value='1'
                                       type="radio" <%nvg_attr_match("", "ftpd_rule", "0", "enable", "1", "checked  ");%>
                                       onclick='sel_ftp_access("1")' >
                                       <%lang("Allow");%> </input>
                                <input name='radio_allow_ftp_access' id='radio_allow_ftp_access' value='0'
                                       type='radio' <%nvg_attr_match("allow_ftpd", "ftpd_rule", "0", "enable", "0", "checked  ");%>
                                       onclick='sel_ftp_access("0")' > 
                                       <%lang("Deny");%> </input>
                        <input name='allow_ftpd' id='allow_ftpd' value='<%nvg_attr_get("allow_ftpd", "ftpd_rule", "0", "enable"); %>' type='hidden'></input>
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->
 <div class="span4" style="display:none;">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-link'></i>&nbsp;<%lang("VPN");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
							<div class="row-fluid">
						 <label for="vpn_ip"><% lang("Service"); %><br>
						<input type="hidden" name="vpn_enable_id" id="vpn_enable_id"/>
    <input type="radio" name="vpn_enable_radio" id="vpn_enable_radio" onclick='sel_vpn("1")' value='1'/>
      <%lang("Enable");%>
    <input type="radio" name="vpn_enable_radio" id="vpn_enable_radio" onclick='sel_vpn("0")' value='0'/>
      <%lang("Disable");%>
							</label>
							</div>
							<div class="row-fluid">
							<label for="vpn_ip"><% lang("Profile"); %><br>
								<select name="vpn_select" id="vpn_select" style="width: 85px;" onchange="vpn_select_fn(this.value)">
								</select>
							</label>
							</div>
							<div class="row-fluid">
    					    <label for="vpn_ip"><% lang("Profile Enabled"); %><br>
						    <input type="hidden" name="vpn_profile_enable" id="vpn_profile_enable"/>
                            <input type="radio" name="vpn_profile_enable_radio" id="vpn_profile_enable_radio" onclick='sel_profile("1")' value='1'/>
                            <%lang("Enable");%>
                            <input type="radio" name="vpn_profile_enable_radio" id="vpn_profile_enable_radio" onclick='sel_profile("0")' value='0'/>
                            <%lang("Disable");%>
							</label>
							</div>
                            <div class="row-fluid">
                                <label for="vpn_profile_name"><% lang("VPN Profile Name"); %></label>
                                <input type="text" name="vpn_profile_name" id="vpn_profile_name" style="width: 173px;" />
                            </div>
                            <div class="row-fluid">
                                <label for="vpn_ip"><% lang("VPN Server"); %></label>
                                <input type="text" name="vpn_ip_server" id="vpn_ip_server" style="width: 173px;" />
                            </div>
                            <div class="row-fluid">
                                <label for="vpn_name"><% lang("VPN User Name"); %></label>
                                <input type="text" name="vpn_name" id="vpn_name" style="width: 173px;" />
                            </div>

                            <div class="row-fluid">
                                <label for="vpn_password"><% lang("VPN User Password"); %></label>
                                <input type="password" name="vpn_password" id="vpn_password"  style="width: 173px;" />
                            </div>
                            <div class="row-fluid">
                                <label for="vpn_Re_type_password"><% lang("Re-type VPN User Password"); %></label>
                                <input type="password" name="vpn_Re_type_password" id="vpn_Re_type_password" style="width: 173px;" />
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/span-->

        </div>

        <!--row4-->
        <div class="row">
            <div class="span12 center">
                <div class="submitFooter">
                    <br><br>
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save");%>">
                    <input type="reset" class="btn btn-inverse" value="<%lang("Cancel");%>">
                </div>
            </div>
            <div class="span12">
                <br><br><script type="text/javascript"><%show_copyright();%></script>
            </div>
        </div><!-- row -->

<!-- PAGE CONTENT ENDS HERE -->
						 </div><!--/row-->
	
					</div><!--/#page-content-->
					  

					<!--<div id="ace-settings-container">
						<div class="btn btn-app btn-mini btn-warning" id="ace-settings-btn">
							<i class="icon-cog"></i>
						</div>
						<div id="ace-settings-box">
							<div>
								<div class="pull-left">
									<select id="skin-colorpicker" class="hidden">
                                        <option data-class="default" value="#F39770">#F39770</option>
                                        <option data-class="skin-1" value="#438EB9">#438EB9</option>
                                        <option data-class="skin-2" value="#C6487E">#C6487E</option>
									</select>
								</div>
								<span>&nbsp; Choose Skin</span>
							</div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-header" /><label class="lbl" for="ace-settings-header"> Fixed Header</label></div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-sidebar" /><label class="lbl" for="ace-settings-sidebar"> Fixed Sidebar</label></div>
						</div>
					</div>/#ace-settings-container-->


			</div><!-- #main-content -->


		</div><!--/.fluid-container#main-container-->




		<a href="#" id="btn-scroll-up" class="btn btn-small btn-inverse">
			<i class="icon-double-angle-up icon-only"></i>
		</a>

    </form>
	                    


		

	</body>
</html>
