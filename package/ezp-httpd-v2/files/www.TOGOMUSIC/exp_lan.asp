<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - LAN Setting</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">

    function clock() {
        var t = ((new Date()).getTime() - startTime) / 1000;
        $("#upgrade-time").text(Math.floor(t / 60) + ':' + pad(Math.floor(t % 60)));
    }

    function to_upgrade(F) {
        $("#cfgfile").attr("disabled", true);
        name = $("#binfile").val();

        if (name.search(/\.(bin)$/i) == -1) {
            alert('A ".bin" file expected.');
            return;
        }
        if (!confirm('Are you sure to upgrade by using ' + name + '?')) 
            return;

        startTime = (new Date()).getTime();
        setInterval('clock()', 1000);
        F = document.getElementsByName("form")[0];
        $("#upgrade").attr("disabled", true);
        F.getAttributeNode("action").value = "upgrade.cgi";
        F.encoding = "multipart/form-data";
        F.submit_button.value = "exp_lan";
        open_upgrade_window();
        F.submit();
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

    function valid_password() {
        //if (F.http_passwd.value != F.http_passwd_confirm.value) {
        if ($("#http_passwd").val() != $("#http_passwd_confirm").val()){
            alert("Confirmed password did not match Entered Password.  Please re-enter password");
            $("#http_passwd").focus();
            $("#http_passwd").select();
            //F.http_passwd_confirm.focus();
            //F.http_passwd_confirm.select();
            return false;
            }
        return true;
    }

    var F;
    function to_submit(F) {
        if(!valid_password(F)) return;
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
                    $("#WEPErrorMsg").text("<%lang("Password is not same.");%>");
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
                    $("#WPAErrorMsg").text("<%lang("Password is not same.");%>");
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
                    $("#WPAErrorMsg").text("<%lang("Password is not same.");%>");
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
        $("#submit_button").val("exp_lan");
        $("#action").val("Apply");
        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
    }

    function SelEWL(F) {
        F.submit_button.value = "exp_lan";
        F.action.value = "Gozila";
        F.submit();
    }

$(document).ready(function(){
    $("#upgrade").attr("disabled", false);
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
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>    
        <script type="text/javascript">create_upgrade_window();</script>

        <form class="form-horizontal" name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('exp_lan_rule');</script>
                <h3><%lang("Setup");%>-<%lang("LAN");%></h3>

        <div class="row">
            <div class="span12">
                <legend><%lang("LAN");%></legend>
                <div class="row show-grid">
                <div class="span4"><% lang("Network address"); %></div>
                    <div class="span8">
                    192.168.
                    <select name="lan0_static_ipclass" id="lan0_static_ipclass" style="width: 65px;">
                    </select>
                    .1
                    </div>
                </div>

                <div class="row show-grid">
                <div class="span4"><% lang("Subnet Mask"); %></div>
                    <div class="span8">
                    255.255.255.0
                    </div>
                </div>
            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><%lang("DHCP");%></legend>

                <div class="row show-grid">
                <div class="span4"><% lang("IP Address pool"); %></div>
                    <div class="span8">
                    <input type="text" style="width: 85px;" name="dhcp0_display_start" id="dhcp0_display_start" value='<%show_ipaddr_prefix24("lan0_ipaddr");%>' readonly />
                    <select id="dhcp0_start" name="dhcp0_start" value='<%nvg_attr_get("dhcp0_start", "lan_dhcps_rule", "0", "start");%>' style="width: 85px;">
                    </select>
                    </div>
                </div>

                <div class="row show-grid">
                <div class="span4"><% lang("Address lease period"); %></div>
                    <div class="span8">
                    <input type="hidden" id="dhcp0_lease" name="dhcp0_lease" value='<%nvg_attr_get("dhcp0_lease", "lan_dhcps_rule", "0", "lease");%>' />
                    2 minutes
                    </input>   
                    </div>
                </div>

                <div class="row show-grid">
                <div class="span4"><%lang("DNS server address");%></div>
                    <div class="span8">
                        <select id="dhcp0_dnstype" name="dhcp0_dnstype" onchange="SelDNSType()" style="width: 188px;"
                            value='<%nvg_attr_get("dhcp0_dnstype", "lan_dhcps_rule", "0", "dnstype");%>'>
                        <option value="dnsrelay" 
                        <%nvg_attr_match("dhcp0_dnstype","lan_dhcps_rule", "0", "dnstype", "dnsrelay", "selected"); %>>
                        <%lang("Auto");%></option>
                        <option value="custom"
                        <%nvg_attr_match("dhcp0_dnstype","lan_dhcps_rule","0","dnstype", "custom","selected"); %>>
                        <%lang("Manual");%></option>
                        </select>
                    </div>
                </div>



                <div class="row show-grid">
                <div class="span4"><% lang("DNS1"); %></div>
                    <div class="span8">
                    <input size="20" type="text" maxLength="20" id="dhcp0_dnsaddr1" name="dhcp0_dnsaddr1" 
                        value='<%nvg_attr_get("dhcp0_dnsaddr1", "lan_dhcps_rule", "0", "dnsaddr1");%>' style="width: 173px;" 
                        <%nvg_attr_match("dhcp0_dnstype","lan_dhcps_rule","0","dnstype","dnsrelay","disabled"); %>>
                    </input> 
                    </div>
                </div>

                <div class="row show-grid">
                <div class="span4"><% lang("DNS2"); %></div>
                    <div class="span8">
                    <input size="20" type="text" maxLength="20" id="dhcp0_dnsaddr2" name="dhcp0_dnsaddr2" 
                        value='<%nvg_attr_get("dhcp0_dnsaddr2", "lan_dhcps_rule", "0", "dnsaddr2");%>' style="width: 173px;"
                        <%nvg_attr_match("dhcp0_dnstype","lan_dhcps_rule","0","dnstype","dnsrelay","disabled"); %>>
                    </input>
                    </div>
                </div>

            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><%lang("Wireless");%></legend>
                <div class="row show-grid">
                <div class="span4"><% lang("SSID"); %></div>
                    <div class="span8">
                    <input type="text" name="wl0_ssid" id="wl0_ssid" style="width: 173px;" value='<% nvg_attr_get("wl0_ssid","wl0_ssid_rule","0","ssid");%>'></input>
                    </div>
                </div>

                <div class="row show-grid">
                <div class="span4"><% lang("Security type"); %></div>
                    <div class="span8">

                    <select name="wl0_ssid0_secmode" id="wl0_ssid0_secmode" style="width: 188px;" onchange="SelEWL(this.form)">
                    <option value="disabled" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "sec_mode", "disabled", "selected");%>><% lang("Disable"); %></option>
                    <option value="wep" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
                    <option value="psk" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk", "selected");%>><% lang("WPA"); %></option>
                    <option value="psk2" <%nvg_attr_match("wl0_ssid0_secmode", "wl0_sec_rule", "0", "secmode", "psk2", "selected");%>><% lang("WPA2"); %></option>
                    </select>
                    <div style="color: red;">( <% lang("It's recommended to use WPA2 for security."); %> )</div>
                    </div>
                </div>

                    <% wlv_sec_show_setting(0, 0);%>

            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><%lang("Password");%></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("Administrator Password"); %></div>
                    <div class="span8">
                        <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd" id="http_passwd" style="width: 173px;">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Re-type Password"); %></div>
                    <div class="span8">
                        <input type="password" value='<%nvg_attr_get("value", "http_rule", "0", "passwd");%>' name="http_passwd_confirm" id="http_passwd_confirm" style="width: 173px;">
                    </div>
                </div>

            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><% lang("Firmware"); %></legend>


                <div class="row show-grid">
                    <div class="span4"><% lang("Firmware Version"); %></div>
                    <div class="span8">
                        <%nvg_get("fw_version");%>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><% lang("Firmware Upgrade"); %></div>
                    <div class="span8">
                        <div class="fileupload fileupload-new" data-provides="fileupload">
                          <span class="btn btn-file">
                            <span class="fileupload-new">Select file</span>
                            <span class="fileupload-exists">Change</span>
                            <input type="file" name="binfile" id="binfile" />
                          </span>
                          <span class="fileupload-preview"></span>
                          <a href="#" class="close fileupload-exists" data-dismiss="fileupload" style="float: none">×</a>
                          <input type="button" class="fileupload-exists" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
                        </div><!-- fileupload -->

                      <!--
                      <input type="file" name="binfile" id="binfile" size="30" />
                      <input type="button" class="btn btn-small" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
                      -->
                    </div>
                </div>
            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <br><br>
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save");%>">
                    <input type="reset" class="btn" value="<%lang("Cancel");%>">
                </div>
            </div>
        </div><!-- row -->

        </form>
        
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>
