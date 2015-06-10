<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN</title>
<% do_pagehead2("no_table"); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="style/ext-style.css" />

<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="vpn_wizard.js"></script>

<script type="text/javascript">

Ext.BLANK_IMAGE_URL = "icons/s.gif";

var F;
var pptp_max=<%wizard_vpn_show_max("pptp");%>;
var ipsec_max=<%wizard_vpn_show_max("ipsec");%>;
var pptp_num=<%wizard_vpn_show_num("pptp");%>;
var ipsec_num=<%wizard_vpn_show_num("ipsec");%>;
function init() {
    F = document.getElementsByName('form');
}

function to_submit(F) {
    F.submit_button.value = "vpn_wizard";
	F.action.value = "Apply";
    /* js_ajax_preaction(F, ds, attr, rule, 1); */
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}
</script>

</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
             <% do_menu("Wizard" , "VPN Wizard"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wizard");%>', '<%lang("VPN Wizard");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form id="wizard_form" name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('vpn_wizard_rule')</script>
              <div id="container" style="width:700px;height:500px">
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
