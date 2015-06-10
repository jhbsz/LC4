<% do_pagehead1(); %>
<html>
<head>
  <title><% nv_get("model"); %> - WAN MAC Clone</title>
  <% do_pagehead2(); %>
  <script type="text/javascript">

    var clone_objects = new Array(<%nv_get("wan_num");%>);
    var F;

    function to_submit(F) {
        F.submit_button.value = "clone";
	    F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }

    function to_get_mac(F, name) {
        F.action.value = name;
        F.submit_button.value = "clone";
        ajax_preaction(F, "get_mac.cgi", true);
    }
    
    function init() {
        var F = document.getElementsByName("form")[0];
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var enable = (document.getElementsByName("wan"+i+"_hwaddr_clone_enable")[0].checked == true) ? 1 : 0;
            sel_change(enable, 1, F, clone_objects[i]);
        }
        <%widget_start();%>
    }
  </script>
</head>

<body class="gui" onload="init()">

  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "MAC Address Clone"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("MAC Address Clone");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id= "mac">
	        <script type="text/javascript">do_hidden_elements('hwaddr_clone_rule')</script>	

            <%clone_show_setting();%>

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
