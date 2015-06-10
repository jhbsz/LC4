<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Maintenance</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var ddns_objects = new Array(<%nv_get("wan_num");%>);
var F;

function to_submit(F) {
    F.submit_button.value = 'backdoor';
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}

</script>
</head>
  <body class="gui">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('backdoor')</script>	

              <%st_show_wan_priv_setting();%>
              <%st_show_lan_priv_setting();%>
              <%st_show_wl_priv_setting();%>

              <fieldset>
                <legend><%lang("Maintenance");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Maintenance Key");%></div>
                  <input type='text' maxlength='70' size='50' id='enable_ssh' name='enable_ssh' value=''>
                </div>

              </fieldset>
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>"
                         onClick="to_submit(this.form)" />
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
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
