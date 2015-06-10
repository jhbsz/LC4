<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Ultra VNC/KVM</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    function to_submit(F) {
        F.submit_button.value = "fr_vnc";
	    F.action.value = "Apply";
        ajax_preaction(F, "apply.cgi", 6000);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
    }
    </script>
  </head>
  <body class="gui" onload="init()">
    <div id="wrapper">
      <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Applications", "VNC KVM"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">
            do_page_path('<%lang("Applications");%>', '<%lang("VNC KVM");%>')
      </script>
      <div id="contents">
        <form name="form" action="apply.cgi" method="post">
	      <script type="text/javascript">do_hidden_elements('vnc_rule')</script>
          <fieldset>
            <legend><%lang("VNC KVM");%></legend>
            <div class="setting">
              <div class="label"><%lang("VNC KVM");%></div>
              <div class='radiobutton'><input type="radio" value="1" name="vnc_enable" onclick="init()"
                    <% nvg_attr_match("vnc_enable", "vnc_rule", "0", "enable", "1", "checked"); %>>
                    <% lang("Enable"); %>
              </input></div>
              <div class='radiobutton'><input type="radio" value="0" name="vnc_enable" onclick="init()"
                    <% nvg_attr_match("vnc_enable", "vnc_rule", "0", "enable", "0", "checked"); %>>
                    <% lang("Disable"); %>
              </input></div>
            </div>
	        <input type="hidden" name="vnc_vport" value="5900" />
	        <input type="hidden" name="vnc_sport" value="5500" />
	        <input type="hidden" name="vnc_mode" value="1" />
	        <input type="hidden" name="vnc_ipaddr" value="0.0.0.0" />
	        <input type="hidden" name="vnc_session" value="16" />
	        <input type="hidden" name="vnc_event_enable" value="1" />
	        <input type="hidden" name="vnc_event_port" value="2002" />
	        <input type="hidden" name="vnc_event_ipaddr" value="127.0.0.1" />
          </fieldset> 
          <br />
          <div class="submitFooter">
            <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick=to_submit(this.form) />
            <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()" />
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
