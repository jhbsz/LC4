<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - DHCP</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var opendns_objects = new Array(<%nv_get("wan_num");%>);
var F;
function to_submit(F) {
    var enable_check = 0;
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var enable = (document.getElementsByName("opendns"+i+"_enable")[0].checked == true) ? 1 : 0;
        if ( enable == "1"  ){
            enable_check = 1;
        }
    }
    if ( enable_check == "1" ){
        if ( confirm("<%lang("All traffic will be redirected to OpenDNS servers if this feature is enabled.");%>") ) {
            F.submit_button.value = "opendns";
	        F.action.value = "Apply";
            ajax_preaction(F);
            display_mask(F);
        } 
    }else{
        F.submit_button.value = "opendns";
        F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }
}


function init() {
    var F = document.getElementsByName("form")[0];
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var enable = (document.getElementsByName("opendns"+i+"_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, opendns_objects[i]);
    }
   
   <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Security", "OpenDNS"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("OpenDNS");%>')</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('wan_opendns_rule')</script>	

              <% opendns_show_setting(); %>

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
