<% do_pagehead1(); %>

<html>
<head>

<style type='text/css'>
#upgrade-time {
    font-size: 26px;
}
</style>

  <title><% nv_get("model"); %> - <%lang("System");%></title>
  <% do_pagehead2(); %>
  <script type='text/javascript' src='table.js'></script>
  <script type="text/javascript">
  
	var dhcp_objects = new Array(<%nv_get("lan_num");%>);
	var dhcpdnsaddr_objects = new Array(<%nv_get("lan_num");%>);

  var F;

  function clock()
  {
	var t = ((new Date()).getTime() - startTime) / 1000;
	E('upgrade-time').innerHTML =  Math.floor(t / 60) + ':' + pad(Math.floor(t % 60));
  }

  function valid_password(F) {
        if (F.http_passwd.value != F.http_passwd_confirm.value) {
                alert("Confirmed password did not match Entered Password.  Please re-enter password");
                F.http_passwd_confirm.focus();
                F.http_passwd_confirm.select();
                return false;
        }
        return true;
  }

  function to_reboot(F) {
        F.action.value='Reboot';
        ajax_preaction(F, "reboot.cgi");
        display_mask(F);
        return true;
  }

  function to_import(F) {
	F.binfile.disabled = true;
	name = F.cfgfile.value;
	if (name.search(/\.(cfg)$/i) == -1) {
		alert('A ".cfg" file expected.');
		return;
	}
	if (!confirm('Are you sure to import configuration by using ' + name + '?')) 
        	return;

    F.getAttributeNode("action").value = "upgrade.cgi";
    F.encoding = "multipart/form-data";
    F.submit_button.value = "ad_basic";
    E("import").disabled = true;
    /* TODO: Ajax support */
    F.submit();
    display_mask(F);

    return true;
  }

  function to_export(F) {
    window.location.replace("<%nv_get("brand");%>.cfg");
  }

  function disp_confirm()
  {
        return confirm("<%lang("Are you sure to restore to the default configuration?");%>");
  }
  function to_default(F) {
        F.action.value='Default';
		F.submit_button.value = "ad_basic";

        if (disp_confirm() == false) {
            return;
        }

        ajax_preaction(F, "default.cgi");
        display_mask(F);
        return true;
  }

  function to_upgrade(F) {
	F.cfgfile.disabled = true;
	name = F.binfile.value;
	if (name.search(/\.(bin)$/i) == -1) {
   		alert("<%lang("Invalid firmware file.");%>");
		return;
	}

    if (CheckBattery()==false) {
        alert("<% lang("Cannot begin update. Recharge the battery until the battery level is at least 60%."); %>\n <% lang("Battery level"); %>: "+BatteryLevel+"%");
        return;
    }

	if (!confirm('Are you sure to upgrade by using ' + name + '?')) 
        	return;

    startTime = (new Date()).getTime();
    setInterval('clock()', 1000);
    E("upgrade").disabled = true;
    F.getAttributeNode("action").value = "upgrade.cgi";
    F.encoding = "multipart/form-data";
    F.submit_button.value = "ad_basic";

    display_upgrade_mask(F);
    F.submit();
  }

  var BatteryLevel=0;
  function CheckBattery(){
    BatteryAvailable = false;
	$.ajax({
	   url:        "percentage.cgi",
	   type:       "POST",
	   dataType:   "text",
	   async:	   false,
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

  function to_submit(F) {
        /*if( F.http_passwd.value != F.http_passwd_confirm.value ) {
                alert('Password confirmation is not matched.');
                return;
        }
        if(!valid_password(F)) return;
        var backup_password=F.http_passwd.value;
        F.http_passwd.value=encode64(F.http_passwd.value);
        F.submit_button.value = "ad_basic";
	    F.action.value = "Apply";
        ajax_preaction(F);
        F.http_passwd.value=backup_password;*/
        display_mask(F);

		// Do LAN/DHCP form submit
		setTimeout(function(){
			lanFormSubmit();
			dhcpFormSubmit();
		}, 1000);
		// End
  }

  function init() {
        F = document.getElementsByName("form")[1];
        E("upgrade").disabled = false;
        E("import").disabled = false;

		for (var i = 0; i < <%nv_get("lan_num");%>; i++) {
			var enable = (document.getElementsByName("dhcp"+i+"_enable")[0].checked == true) ? 1 : 0;
			sel_change(enable, 1, F, dhcp_objects[i]);
			sel_change(enable, 1, F, dhcpdnsaddr_objects[i]);
			SelDNSType(i);
		}

        <%widget_start();%>
  }

  // Function to show copy right content
  function showPWS() {
	var pwsver = "PWS_System_Software_License-English.asp";
	var chkLangType = '<% nvg_match("lang", "JP", "selected"); %>';
	// Change reference to Loading
	$("#Ref").val('<% lang("Loading"); %>...');
	if ( chkLangType.length > 0) {
		pwsver = "PWS_System_Software_License-Japanese.asp";
	}
	$("#copyRight").css({"overflow":"scroll","padding-bottom":"2px","padding-left":"4px","padding-right":"4px","padding-top":"2px"});

	$("#copyRight").load(pwsver, function(response, status, xhr) {
	  if (status == "error") {
		var msg = "Sorry but there was an error: ";
		alert(msg + xhr.status + " " + xhr.statusText);
	  }
	});
  }

function SelDNSType(nth) {
    var enable = (document.getElementsByName("dhcp"+nth+"_enable")[0].checked == true) ? 1 : 0;
    if (enable == 0) {
        return;
    }

    if (E('dhcp'+nth+'_dnstype').value == "dnsrelay") {
        E('dhcp'+nth+'_dnsaddr1').disabled = true;
        E('dhcp'+nth+'_dnsaddr1').value = "";
        E('dhcp'+nth+'_dnsaddr2').disabled = true;
        E('dhcp'+nth+'_dnsaddr2').value = "";
    } else if (E('dhcp'+nth+'_dnstype').value == "ispdns") {
        E('dhcp'+nth+'_dnsaddr1').disabled = true;
        E('dhcp'+nth+'_dnsaddr1').value = "";
        E('dhcp'+nth+'_dnsaddr2').disabled = true;
        E('dhcp'+nth+'_dnsaddr2').value = "";
    } else if (E('dhcp'+nth+'_dnstype').value == "opendns") {
        E('dhcp'+nth+'_dnsaddr1').disabled = true;
        E('dhcp'+nth+'_dnsaddr1').value = "208.67.220.220";
        E('dhcp'+nth+'_dnsaddr2').disabled = true;
        E('dhcp'+nth+'_dnsaddr2').value = "208.67.222.222";
    } else if (E('dhcp'+nth+'_dnstype').value == "googledns") {
        E('dhcp'+nth+'_dnsaddr1').disabled = true;
        E('dhcp'+nth+'_dnsaddr1').value = "8.8.8.8";
        E('dhcp'+nth+'_dnsaddr2').disabled = true;
        E('dhcp'+nth+'_dnsaddr2').value = "8.8.4.4";
    } else if (E('dhcp'+nth+'_dnstype').value == "custom") {
        E('dhcp'+nth+'_dnsaddr1').disabled = false;
        E('dhcp'+nth+'_dnsaddr1').value = "<%nvg_attr_get("dhcp$0_dnsaddr1", "lan_dhcps_rule", "$0", "dnsaddr1");%>";
        E('dhcp'+nth+'_dnsaddr2').disabled = false;
        E('dhcp'+nth+'_dnsaddr2').value = "<%nvg_attr_get("dhcp$0_dnsaddr2", "lan_dhcps_rule", "$0", "dnsaddr2");%>";
    } else{
        E('dhcp'+nth+'_dnsaddr1').disabled = true;
        E('dhcp'+nth+'_dnsaddr2').disabled = true;
    }
}

  </script>
</head>

<body class="gui" onload="init();menuFreezer();">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Admin", "Management"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("System");%>', '')</script>
      <div id="description"><%lang("Please confirm that the latest system software is available by visiting the URL below.");%> <br><a href='http://www.sony.net/pws/' target='_new'>http://www.sony.net/pws/</a></div>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id= "setup">
	        <script type="text/javascript">do_hidden_elements('http_rule')</script>

            <input type="hidden" name= "reboot_button"/> 
            <% ad_x_mode_switch(); %>
            <% ad_vlanport_config(); %>

			<div style="display: none;">
			<!-- Hide some setting not used begin -->
            <fieldset>
              <legend><% lang("Administration Interface"); %></legend>

              <div class="setting">
                <div class="label">
                  <%lang("Language");%>
                </div><select name="lang">
                  <option value=
                  "EN" <% nvg_match("lang", "EN", "selected"); %>>
                  English
                  </option>
                  <option value=
                  "TW" <% nvg_match("lang", "TW", "selected"); %>>
                  正體中文
                  </option>
                  <option value=
                  "GE" <% nvg_match("lang", "GE", "selected"); %>>
                  Deutsch 
                  </option>
                  <option value=
                  "GB" <% nvg_match("lang", "GB", "selected"); %>>
                  簡體中文
                  </option>
                  <option value=
                  "JP" <% nvg_match("lang", "JP", "selected"); %>>
                  日本語
                  </option>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Administrator Password"); %>
                </div><input type="password" maxlength="32" size=
                "30" value="slide" name=
                "http_passwd">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Re-type Password"); %>
                </div><input type="password" maxlength="32" size=
                "30" value="slide" name=
                "http_passwd_confirm">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Remote Management"); %>
                </div>
                <input type="radio" value="1" name= "http_rmgt_enable"
                <% nvg_attr_match("http_rmgt_enable","http_rule","0","rmgt_enable","1","checked"); %>><% lang("Enable"); %></input>
                <input type="radio" value="0" name= "http_rmgt_enable"  
                <% nvg_attr_match("http_rmgt_enable","http_rule","0","rmgt_enable","0","checked"); %>><% lang("Disable"); %></input>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Management Port"); %>
                </div>HTTP <input class="num" maxlength="5" size=
                "5" name="http_port" onblur=
                "valid_range(this,1,65535,'Port number')" value=
                "<%nv_attr_get("http_rule", "0", "port");%>">
              </div>

            </fieldset><br />

            <fieldset>
              <legend><% lang("Reboot"); %></legend>
              <div class="setting">
                <div class="label"><% lang("Reboot"); %></div>
                <input type="button" value='<%lang("Reboot Router");%>' onclick= "to_reboot(this.form)">
              </div>
            </fieldset><br />

            <fieldset>
              <legend><% lang("Configuration"); %></legend>
              <div class="setting">
                <div class="label">
                  <% lang("Configuration Export"); %>
                </div>
                <input type="button"
                value='<%lang("Export");%>' onclick=
                "to_export(this.form)">
              </div>
              <div class="setting">
                <div class="label">
                  <% lang("Default Configuration Restore"); %>
                </div>
                <input type="button"
                value='<%lang("Default");%>' onclick=
                "to_default(this.form)">
              </div>
              <div class="setting">
                <div class="label">
                  <% lang("Configuration Import"); %>
                </div>
                <input type="file" name="cfgfile" size="30" />
                <input type="button" name="import" id="import"
                    value='<%lang("Import");%>' onclick= "to_import(this.form)">
              </div>
            </fieldset><br />
			<!-- Hide some setting not used end -->
			</div>

<%prod_show_html_start("firmware", "0", "100");%>
            <!-- <fieldset>
              <legend><% lang("Firmware"); %></legend> -->
              <div class="setting">
                <div class="label">
                  <% lang("System software version"); %>
                </div>
                <input style="background-color: #DCDCDC; border: 0;" name="SystemVer" value="<% show_version(""); %>" size="30" readonly />
				<br>
				<br>

				<div class="label">
                  <% lang("System software update"); %>
                </div>
                <input class="f_class" style="background-color: #FFF;" type="file" name="binfile" size="30" />
				<br>

                <div class="label">&nbsp;</div>
                 <input type="button" name="upgrade" id="upgrade" value='<%lang("Update");%>' onclick= "to_upgrade(this.form)">
				<br>

              </div>
            <!-- </fieldset> -->
			<br />
<%prod_show_html_end("firmware", "0", "100");%>

              <% lan_show_setting(); %>

              <% dhcp_show_setting(); %>

              <div class="submitFooter">
                <input type="reset" value= '<%lang("Cancel");%>' onClick="window.location.reload()" >
                <input type="button" name="save_button" value= '<%lang("Save");%>' onclick= "to_submit(this.form)">
              </div>
          </form>

		  <br>
		  <div class="setting">
		   <div class="label"><% lang("Copyright"); %></div>
		   <div class="label" id="copyRight"><input id="Ref" type="button" onClick="showPWS();" value="<% lang("Reference"); %>"></div>
		  </div>
        </div>

	  <script type="text/javascript">do_mainframe_post()</script>	
      <!--<%widget_show();%>-->
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_upgrade_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>
