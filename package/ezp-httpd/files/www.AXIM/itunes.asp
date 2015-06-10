<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - ITUNES</title>
<% do_pagehead2(); %>
<script type="text/javascript">

itunes_objects =
[
  { name: 'itunes_passwd' },
  { name: 'itunes_servername' },
];
var F;
function to_submit(F) {
	F.submit_button.value = "itunes";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}
function init() {
        F = document.getElementsByName("form")[0];
        var enable = (document.getElementsByName("enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, itunes_objects);
   <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "iTunes"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("iTunes");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('itunes_rule')</script>	

              <% itunes_show_setting(); %>

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
