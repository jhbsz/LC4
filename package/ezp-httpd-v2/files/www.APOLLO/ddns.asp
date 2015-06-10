<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Dynamic DNS</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var ddns_objects = new Array(<%nv_get("wan_num");%>);
var ddns_type_objects = new Array(<%nv_get("wan_num");%>);
var ddns_update_objects = new Array(<%nv_get("wan_num");%>);
var F;

function to_submit(F) {
    F.submit_button.value = 'ddns';
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}
function disable_update() {
    F = document.getElementsByName('form')[0];
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        sel_change(0, 1, F, ddns_update_objects[i]);
    }
}
function init() {
    F = document.getElementsByName('form')[0];
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var enable = (document.getElementsByName("ddns"+i+"_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, ddns_objects[i]);
        if (enable) {
            var type = (document.getElementsByName("ddns"+i+"_type")[0].value == "dyndns") ? 1 : 0;
            sel_change(type, 1, F, ddns_type_objects[i]);
        }
        var type_check = (EN("ddns"+i+"_type")[0].value == EN("ddns"+i+"_type_org")[0].value ) ? 0 : 1;
        var username_check = (EN("ddns"+i+"_username")[0].value == EN("ddns"+i+"_username_org")[0].value ) ? 0 : 1;
        var passwd_check = (EN("ddns"+i+"_passwd")[0].value == EN("ddns"+i+"_passwd_org")[0].value ) ? 0 : 1;
        var hostname_check = (EN("ddns"+i+"_hostname")[0].value == EN("ddns"+i+"_hostname_org")[0].value ) ? 0 : 1;
        var server_check = (EN("ddns"+i+"_server")[0].value == EN("ddns"+i+"_server_org")[0].value ) ? 0 : 1;
        if (type_check || username_check || passwd_check || hostname_check ||
server_check) disable_update();
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" id="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
            <% do_menu("Setup", "DDNS"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>		
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("DDNS");%>')</script>
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('ddns_rule')</script>	
              <% ddns_show_setting(); %>

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
    <script type="text/javascript">
    document.getElementById('gui').addEventListener('change', function(){
      disable_update()
    }, false);
    </script>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
