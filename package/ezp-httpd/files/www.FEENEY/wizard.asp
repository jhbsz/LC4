<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Wizard</title>
<% do_pagehead2("no_table"); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="style/ext-style.css" />

<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="wizard.js"></script>
<script type="text/javascript" src="wizard_wan_setting.js"></script>
<script type="text/javascript" src="wizard_mobile_setting.js"></script>
<script type="text/javascript" src="wizard_bandwidth.js"></script>
<script type="text/javascript" src="wizard_wifi.js"></script>

<script type="text/javascript">

Ext.BLANK_IMAGE_URL = "icons/s.gif";

var F;
function init() {
    F = document.getElementsByName('form');
}

function to_submit(F) {
    F.submit_button.value = "wizard";
	F.action.value = "Apply";
    /* js_ajax_preaction(F, ds, attr, rule, 1); */
    ajax_preaction(F);
    display_mask(F);
}
</script>

<style type="text/css">
.x-form-check-wrap {
    padding-right:10px;
}
</style>


</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
             <% do_menu("Wizard", "Setup"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wizard");%>', '<%lang("Setup Wizard");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form id="wizard_form" name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('wizard_rule')</script>
              <div id="container" style="width:700px;height:550px">
                  <div id="wizard_window"></div>
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
    <script type="text/javascript">create_return_window("st_basic.asp")</script>
  </body>
</html>
