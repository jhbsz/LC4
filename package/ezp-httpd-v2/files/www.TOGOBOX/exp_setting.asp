
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

        <link href='css/fontcustom.css' rel='stylesheet' type='text/css'>
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
    <%widget_start();%>
    function widget_postaction(data) {
        result = data;
        var ret = result.split('\n');
        var USB_usage, SD_usage;
        for (i = 0; i < ret.length; i++) {
            var pair = ret[i].split('=');
            if (pair[0] == "Battery") {
                var battery = pair[1] + "%";
                if(pair[1] < 20) {
                    $("#navbar_battery").attr("style", "display:inline;background-color: rgb(255,255,255);");
                    $("#nav_battery").html(battery);
                } else if(pair[1] >= 20){
                    $("#navbar_battery").attr("style", "display:none;");
                }
            } else if (pair[0] == "USB_usage") {
                USB_usage = pair[1];
                if(USB_usage == "N/A") {
                    USB_usage = 0;
                }
            } else if (pair[0] == "SD_usage") {
                SD_usage = pair[1];
                if(SD_usage == "N/A") {
                    SD_usage = 0;
                }
            }

        }

        if(USB_usage > 95 || SD_usage > 95) {
            var storage;
            if(SD_usage > USB_usage) {
                storage = SD_usage + "%";
            } else {
                storage = USB_usage + "%";
            }
            $("#navbar_storage").attr("style", "display:inline;background-color: rgb(255,255,255);");
            $("#nav_storage").html(storage);
        } else if((USB_usage <= 95) && (SD_usage <= 95)){
            $("#navbar_storage").attr("style", "display:none;background-color: rgb(255,255,255);");
        }

        if(USB_usage > 0 || SD_usage > 0) {
            $('#3').attr("style","display:inline;");
        } else if((USB_usage == 0) && (SD_usage == 0)){
            $('#3').attr("style","display:none;");
        }

        setTimeout('widget_query(F);', 5000);
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
	    function clock() {
            var t = ((new Date()).getTime() - startTime) / 1000;
            $("#upgrade-time").text(Math.floor(t / 60) + ':' + pad(Math.floor(t % 60)));
        }

        function disp_confirm(){
            return confirm("<%lang("Are you sure to restore to the default configuration?");%>");
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
                alert('<%lang("System update ".bin" file expected.");%>');
                return;
            }

            if (CheckBattery()==false) {
                alert("<% lang("Cannot begin update. Recharge the battery until the battery level is at least 60%."); %>\n <% lang("Battery level"); %>: " + BatteryLevel + "%");
                return;
            }


            if (!confirm('<%lang("Are you sure to upgrade by using");%> ' + name + '?')) 
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
                    if (data>=60){
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


        function to_reboot(F) {
            $("#submit_button").val("exp_setting");
            $("#action").val("Reboot");

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
            alert('<%lang("The Password must be no longer than 31 characters. The first and final characters must not be spaces.");%>');
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
            alert('<%lang("The Password must be no longer than 31 characters. The first and final characters must not be spaces.");%>');
            return false;
        }

        //if (F.http_passwd.value != F.http_passwd_confirm.value) {
        if (I != J){
            alert("Confirmed password did not match Entered Password.  Please re-enter password");
            $("#http_passwd").focus();
            $("#http_passwd").select();
            //F.http_passwd_confirm.focus();
            //F.http_passwd_confirm.select();
            return false;
        }
        return true;
    }

    function valid_ssid_char(I) {
        if(I.length==0 || I.length>=31) {
            alert('<%lang("The SSID name must be no longer than 31 characters. The first and final characters must not be spaces.");%>');
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
            alert('<%lang("The SSID name must be no longer than 31 characters. The first and final characters must not be spaces.");%>');
            return false;
        }
        return true;
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
                    $("#WEPErrorMsg").text("<%lang("Password is not the same.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
	    if(!valid_wifi_key($("#wl0_ssid0_key").val())){
                $("#WEPErrorMsg").text("<%lang("Password is not the same.");%>");
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
		return;
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
                    $("#WPAErrorMsg").text("<%lang("Password is not the same.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
	    if(!valid_wifi_key($("#wl0_ssid0_key").val())){
                $("#WEPErrorMsg").text("<%lang("Password is not the same.");%>");
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
		return;
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
                    $("#WPAErrorMsg").text("<%lang("Password is not the same.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }
	    if(!valid_wifi_key($("#wl0_ssid0_key").val())){
                $("#WEPErrorMsg").text("<%lang("Password is not the same.");%>");
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
		return;
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
    file_select_icon();
    var curIP = "<%nv_attr_get("lan_static_rule", "0", "ipaddr");%>";
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
        <script type="text/javascript">create_waiting_window();</script>
        <script type="text/javascript">create_upgrade_window();</script>
	    <%do_bootstrap_navbar_menu();%>
        <form class="form-horizontal" name="setting_form" id="setting_form" action="apply.cgi" method="post">

		<div class="main-container container-fluid" id="main-container">
        <a id="menu-toggler" class="menu-toggler" href="#"><span class="menu-text"></span></a>
			<div id="sidebar" class="sidebar">
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'fixed')}catch(e){}
                </script>
			    <%do_bootstrap_menu();%>
                <div id="sidebar-collapse" class="sidebar-collapse"><i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right" ></i></div>
			</div><!--/#sidebar-->
		
			<div id="main-content" class="main-content">
					
					<div id="breadcrumbs" class="breadcrumbs">
						<ul class="breadcrumb">
							<li><i class="icon-home"></i> <%lang("Home");%><span class="divider"><i class="icon-angle-right"></i></span></li>
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
                                <input type="text" name="wl0_ssid" id="wl0_ssid" style="width: 173px;" maxlength="31" value='<% nvg_attr_get("wl0_ssid","wl0_ssid_rule","0","ssid");%>'></input>
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
                                <select id="dhcp0_start" name="dhcp0_start" value='<%nvg_attr_get("dhcp0_start", "lan_dhcps_rule", "0", "start");%>' style="width: 100px;">
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
                                    <option value="disabled" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "sec_mode", "disabled", "selected");%>><% lang("Disable"); %></option>
                                    <option value="wep" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
                                    <option value="psk" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk", "selected");%>><% lang("WPA"); %></option>
                                    <option value="psk2" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk2", "selected");%>><% lang("WPA2"); %></option>
                                </select>
                                <span class="help-block">( <%lang("It's recommended to use WPA2 for security.");%> )</span>
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
                                <label for="http_passwd"><% lang("Change Password"); %></label>
                                <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd" id="http_passwd" style="width: 173px;">
                            </div>

                            <div class="row-fluid">
                                <label for="http_passwd_confirm"><% lang("Re-type Password"); %></label>
                                <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd_confirm" id="http_passwd_confirm" style="width: 173px;">                 

                                <span class="help-block"><font style="color:rgb(178,0,0);">(<%lang("Please use the combination with only number and English alphabet with upper or lower case.");%>)</font></span>
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
                                <label ><% lang("Default Configuration Restore"); %></label>
                                <input type="button" class="btn btn-small btn-primary" value='<%lang("Default");%>' onclick="to_default(this.form)">
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
                        <h4><i class='icon-download'></i>&nbsp;<%lang("Firmware");%></h4>
                    </div>
                    <div class="widget-body">
                        <div class="widget-main">
                            <div class="row-fluid">
                                <label ><%lang("Version");%></label>
                                <% nv_get("fw_version"); %>
                            </div>
                            <div class="row-fluid">
                                <label for="http_passwd_confirm"><%lang("Upgrade");%></label>
                                <input type="file" name="binfile" id="binfile" />
                                <input type="button" class="btn btn-small btn-primary" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
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
