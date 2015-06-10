<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - SNMP</title>
<% do_pagehead2(); %>
<script language="javascript" type="text/javascript">
var F;

var snmp_objects =
[ 
{ name: 'snmp_udpport'  },
{ name: 'snmp_tcpport'  },
{ name: 'snmp_community'},
{ name: 'snmp_sysname'  },
{ name: 'snmp_location' },
{ name: 'snmp_contact'  },
{ name: 'snmp_timeout'  },
{ name: 'snmp_auth'     }
];                                    


function onoffsnmp() {
    F=EN('form')[0];
    var enable_snmp;
    enable_snmp=(EN("snmp_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable_snmp, 1,  F, snmp_objects);
}

function to_submit(F) {
    var snmp_contact_backup;
    F.submit_button.value = "snmp";
    F.action.value = "Apply";
    snmp_contact_backup=EN("snmp_contact")[0].value;
    EN("snmp_contact")[0].value=encode64(EN("snmp_contact")[0].value);	
    ajax_preaction(F, "apply.cgi", 6000);
    EN("snmp_contact")[0].value=snmp_contact_backup;
    display_mask(F);
}

function init() {
    onoffsnmp();
    <%widget_start();%>
}

</script>
</head>
  <body class="gui" onload="init();"> <% showad(); %>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Applications", "SNMP"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Admin");%>', '<%lang("SNMP");%>')
      </script>
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('snmp_rule')</script>
              <fieldset>
                <legend><%lang("SNMP");%></legend>
                <div class="setting">
                  <div class="label">
                    SNMP
                  </div>
                  <div class='radiobutton'><input type="radio" value="1" name="snmp_enable" onclick="onoffsnmp()"
                  <%nvg_attr_match("snmp_enable", "snmp_rule", "0", "enable", "1", "checked"); %>><% lang("Enable"); %></input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="snmp_enable" onclick="onoffsnmp()"
                  <%nvg_attr_match("snmp_enable", "snmp_rule", "0", "enable", "0", "checked"); %>><% lang("Disable"); %></input></div>
                </div>
		<div class="setting">
			<div class="label">
			<% lang("SNMP Read Community"); %>
			</div><input class="num" maxlength="16" size=
			"25" name="snmp_community" id="snmp_community" onblur=
			"valid_reserved_word(this,'snmp community')" value=
			"<%nvg_attr_get("snmp_community", "snmp_rule", "0", "community");%>" </input>
		</div>
		<div class="setting">
			<div class="label">
			<% lang("SNMP UDP Port"); %>
			</div><input class="" maxlength="5" size=
			"5" name="snmp_udpport" id="snmp_udpport"  onblur=
			"valid_range(this,1,65535,'Port number')" value=
			"<%nvg_attr_get("snmp_udpport", "snmp_rule", "0", "udpport");%>" </input>
		</div>
		</fieldset>
		                                                                                                                                
		<fieldset>
		<legend><% lang("System Identification"); %></legend>
		<div class="setting">
			<div class="label"><%lang("System Name");%></div>
			<input name="snmp_sysname" id='snmp_system' size="25" maxLength="25" onBlur="valid_name(this,'SYSTEM NAME')"
			value='<%nvg_attr_get("snmp_sysname", "snmp_rule", "0", "sysname");%>' </input>
		</div>
		<div class="setting">
			<div class="label"><%lang("System Location");%></div>
			<input name="snmp_location" id='snmp_location' size="25" maxLength="25" onBlur="valid_name(this,'SYSTEM LOCATION')"
			value='<%nvg_attr_get("snmp_location", "snmp_rule", "0", "location");%>' </input>
		</div>
		
		<div class="setting">
			<div class="label"><%lang("System Contact");%></div>
            <script type="text/javascript">
              document.write('<input type="text" id="snmp_contact" name="snmp_contact" size="25" maxlength="25"');
              document.write('onBlur="valid_name(this,\'SYSTEM CONTACT\')"');
              document.write('value="');
              document.write(decode64('<%b64_nvg_attr_get("snmp_contact","snmp_rule", "0", "contact");%>'));
              document.write('">');
</script>
		</div>
		
		</fieldset><br/>
		
		</script>
		<input type="hidden" name="snmp_tcpport" value="<%nv_attr_get("snmp_rule", "0", "tcpport");%>"></input>
		<input type="hidden" name="snmp_timeout" value="<%nv_attr_get("snmp_rule", "0", "timeout");%>"></input>
		<input type="hidden" name="snmp_auth"    value="<%nv_attr_get("snmp_rule", "0", "auth");%>"></input>
	
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
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
    <script type="text/javascript">create_return_window("snmp.asp")</script>
  </body>
</html>
