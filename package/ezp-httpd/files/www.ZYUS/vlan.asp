<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VLAN</title>
<% do_pagehead2("no_table"); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="ext/css/grid-examples.css" />
<style type="text/css">
#container table {
    border-spacing: 0em;
    margin: 0 10px 0 10px;
    width: 0%;
    }

</style>

<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="ext/js/CheckColumn.js"></script>
<script type="text/javascript" src="ext/js/RadioColumn.js"></script>
<script type="text/javascript" src="vlan.js"></script>

<script type="text/javascript">

Ext.BLANK_IMAGE_URL = "icons/s.gif";

var F;
function init() {
    F = document.getElementsByName('form');
    <%widget_start();%>
}

function to_submit(F) {
    F.submit_button.value = "vlan";
	F.action.value = "Apply";
    /* js_ajax_preaction(F, ds, attr, rule, 1); */
    js_ajax_preaction(F, ds_array, attr_array, rule_array, 2);
    display_mask(F);
}
<%vlan_show_content();%>
</script>

</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
            <% do_menu("Setup", "VLAN"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("VLAN");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('vlan_rule')</script>
              <div id="container" style="width:740px;height:250px">
                  <input type='hidden' name='type' />
                  <div id="gridpanel"></div>
              </div>
                <br>
              <div id="container" style="width:740px;height:200px">
                  <input type='hidden' name='type' />
                  <div id="gridpanel1"></div>
              </div>
            <br/>
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
