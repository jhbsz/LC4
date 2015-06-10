<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <% do_pagehead2(); %>
    <script language="JavaScript">
        var useIE6 = false;
    </script>
    <!--[if lte IE 6]>
        <script language="JavaScript">
            var useIE6 = true;
        </script>
    <![endif]-->
    <script language="JavaScript">
    
    var F;

    var wl_adv_objects = new Array(<%nv_get("wl_rule_num");%>);

    function to_submit(F) {
        F.action.value = "Apply";
        F.submit_button.value = "wl_adv";
        ajax_preaction(F);
        display_mask(F);
    }

    function SelWlAdv(F) {
        F.submit_button.value = "wl_adv";
        F.action.value = "Gozila";
        F.submit();
    }

    function init() {
        F = document.getElementsByName("form")[0];
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            var enable = E("wl"+i+"_enable").value;
            sel_change(enable, 1, F, wl_adv_objects[i]);
        }
	    <%widget_start();%>
    }
    </script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "Advanced"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("Advanced");%>')</script>
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" >
	        <script type="text/javascript">do_hidden_elements('wl_advanced_rule')</script>	

            <fieldset>
              <legend><%lang("Region Setting");%></legend>
              <div class="setting">
                <div class="label">
                  <%lang("Region");%>
                </div>
                <select name="wl_region">
                  <option value='0' <%nvg_match("wl_region", "0", "selected");%>>
                    <%lang("US, Canada and Taiwan");%> (<%lang("channel");%> 1 - 11)
                  </option>
                  <option value='1' <%nvg_match("wl_region", "1", "selected");%>>
                    <%lang("Europe, Australia and Hong Kong");%> (<%lang("channel");%> 1 - 13)
                  </option>
                  <option value='5' <%nvg_match("wl_region", "5", "selected");%>>
                    <%lang("Japan");%> (<%lang("channel");%> 1 - 14)
                  </option>
                </select>
              </div>
            </fieldset>
            <br/>

            <% wl_show_setting("wl_adv_setting.asp"); %>
            <div class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.reload()" />
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
