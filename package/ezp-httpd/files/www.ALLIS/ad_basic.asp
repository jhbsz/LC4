<% do_pagehead1(); %>

<html>
<head>

<style type='text/css'>
#upgrade-time {
    font-size: 26px;
}
</style>

  <title><% nv_get("model"); %> -
  Administration</title><% do_pagehead2(); %>
  <script type='text/javascript' src='table.js'></script>
  <script type="text/javascript">
  
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
		alert('A ".bin" file expected.');
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

  function to_submit(F) {
        if( F.http_passwd.value != F.http_passwd_confirm.value ) {
                alert('Password confirmation is not matched.');
                return;
        }
        if(!valid_password(F)) return;
        var backup_password=F.http_passwd.value;
        F.http_passwd.value=encode64(F.http_passwd.value);
        F.submit_button.value = "ad_basic";
	    F.action.value = "Apply";
        ajax_preaction(F);
        F.http_passwd.value=backup_password;
        display_mask(F);
  }

  function init() {
        F = document.getElementsByName("form")[0];
        E("upgrade").disabled = false;
        E("import").disabled = false;
        <%widget_start();%>
  }
  </script>
</head>

<body class="gui" onload="init()">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Admin", "Management"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Admin");%>', '<%lang("Management");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id= "setup">
	        <script type="text/javascript">do_hidden_elements('http_rule')</script>
            <input type="hidden" name= "reboot_button"/> 
            <% ad_x_mode_switch(); %>
            <% ad_vlanport_config(); %>
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

<%prod_show_html_start("firmware", "0", "100");%>
            <fieldset>
              <legend><% lang("Firmware"); %></legend>
              <div class="setting">
                <div class="label">
                  <% lang("Firmware Upgrade"); %>
                </div>
              <input type="file" name="binfile" size="30" />
              <input type="button" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
              </div>
            </fieldset><br />
<%prod_show_html_end("firmware", "0", "100");%>
              <div class="submitFooter">
                <input type="button" name="save_button" value= '<%lang("Save Settings");%>' onclick= "to_submit(this.form)">
                <input type="reset" value= '<%lang("Cancel Changes");%>' onClick="window.location.reload()" >
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
    <script type="text/javascript">create_upgrade_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>
