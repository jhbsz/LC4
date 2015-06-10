<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - License</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var ddns_objects = new Array(<%nv_get("wan_num");%>);
var F;

function to_submit(F) {
    F.submit_button.value = 'license';
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    var license_invalid = <%nv_get("license_invalid");%>;
    if (license_invalid == '0') {
        E('license_key').disabled = true;
    } else {
        E('license_key').disabled = false;
    }
}

</script>
</head>
  <body class="gui" onload='init()'>
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("DDNS");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('license')</script>	

              <%st_show_wan_priv_setting();%>
              <%st_show_lan_priv_setting();%>
              <%st_show_wl_priv_setting();%>

              <fieldset>
                <legend><%lang("License");%></legend>
                <div class="setting">
                  <div class="label">
                    <%lang("License");%>
                  </div><% show_license_status(); %>
                </div>
                <div class="setting">
                  <div class="label"><%lang("License Key");%></div>
                  <input type='text' maxlength='70' size='50' id='license_key' name='license_key' value=''>
                </div>

              </fieldset>
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
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
