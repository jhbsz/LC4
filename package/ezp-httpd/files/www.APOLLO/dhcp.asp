<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - DHCP</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var dhcp_objects = new Array(<%nv_get("lan_num");%>);
var dhcpdnsaddr_objects = new Array(<%nv_get("lan_num");%>);
var F;
function to_submit(F) {
    F.submit_button.value = "dhcp";
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
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


function init() {
    var F = document.getElementsByName("form")[0];
    for (var i = 0; i < <%nv_get("lan_num");%>; i++) {
        var enable = (document.getElementsByName("dhcp"+i+"_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, dhcp_objects[i]);
        sel_change(enable, 1, F, dhcpdnsaddr_objects[i]);
        SelDNSType(i);
    }
   
   <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Setup", "DHCP Server"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>		
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("DHCP Server");%>')</script>
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('dhcp_rule')</script>	

              <% dhcp_show_setting(); %>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
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
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
