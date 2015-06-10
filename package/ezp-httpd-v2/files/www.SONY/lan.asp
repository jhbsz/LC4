<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - LAN Setting</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;
function to_submit(F) {
	if(lan_check(F,"save") == true) {
		F.submit_button.value = "lan";
	    F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
	}
}

function lan_check(F,T) {
    return true; 
}

function init() {
    F = document.getElementsByName("form")[0];
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "LAN"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("LAN");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('lan_rule')</script>	

              <% lan_show_setting(); %>

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
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
