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

var wps_display_objects =
[ { name: 'wl_wps_enable' },
  { name: 'wps_gen_pin' },
  { name: 'wps_pbc' },
  { name: 'wps_pin' }
];
    
var wps_objects =
[ { name: 'wps_gen_pin' },
  { name: 'wps_pbc' },
  { name: 'wps_pin' }
];
    
    var F;

    function to_submit(F) {
        F.action.value = "Apply";
        F.submit_button.value = "wl_wps";
        //ajax_preaction(F, "apply.cgi", true);
        ajax_preaction(F, "apply.cgi", 1000);
        display_mask(F);
    }

    function to_gen_pin(F) {
        F.action.value = "WPS_Gen";
        F.submit_button.value = "wl_wps_pin";
        ajax_preaction(F, "wps.cgi", true);
        display_mask(F);
    }

    function to_wps_pin(F) {
        F.action.value = "WPS_Pin";
        ajax_preaction(F, "wps.cgi", true);
        display_mask(F);
    }

    function to_wps_btn(F) {
        F.action.value = "WPS_Btn";
        ajax_preaction(F, "wps.cgi", true);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        var display = (document.getElementsByName("wl_wps_display")[0].value == "1") ? 1 : 0;
        if (display) {
            var enable = (document.getElementsByName("wl_wps_enable")[0].checked == true) ? 1 : 0;
            sel_change(enable, 1, F, wps_objects);
        } else {
            sel_change(enable, 1, F, wps_display_objects);
        }
	    <%widget_start();%>
    }
</script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "WPS"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("WPS");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" >
	        <script type="text/javascript">do_hidden_elements('wl_wps_rule')</script>	
              <fieldset>
                <input type="hidden" name="wl_wps_display" value="<%nvg_attr_get("wl_wps_display", "wl_wps_rule", 0, "display");%>"/>
                <legend>WPS <%lang("Enable");%></legend>
                <div class="setting">
                  <div class="label">WPS <% lang("Enable"); %></div>
                  <input type="radio" value="7" id="wl_wps_enable" name="wl_wps_enable" onclick="init()"
                        <%nvg_attr_match("wps_enable", "wl_wps_rule","0","mode","7","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="wl_wps_enable" onclick="init()"
                        <%nvg_attr_match("wps_enable", "wl_wps_rule","0","mode","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
             </fieldset><br />

            <fieldset>
            <legend><%lang("WPS Router PIN Code");%></legend>
            <div class="setting">
            <div class="label"><% lang("WPS Router PIN Code");%>:</div>
            <label><%nvg_attr_get("wps_pin", "wl_wps_rule", 0, "pin");%></label>
           <input type="button" name="wps_gen_pin" id="wps_gen_pin"
        value='<%lang("Generate PIN Code");%>' onclick= "to_gen_pin(this.form)">
           </div>
             </fieldset><br>

            <fieldset>
            <legend><%lang("WPS Connect");%></legend>
            <div class="setting">
            <div class="label"><% lang("WPS Push Button"); %>:</div>
           <input type="button" name="wps_pbc" id="wps_pbc"
        value='<%lang("Push Button");%>' onclick= "to_wps_btn(this.form)"></div>

            <div class="setting">
            <div class="label"><% lang("WPS Client Pin Code Connect"); %>:</div>
            <input type="text" maxLength="8" size="9" name="wps_pin"/>
           <input type="button" name="wps_pin" id="wps_pin"
        value='<%lang("Connection");%>' onclick= "to_wps_pin(this.form)">
           </div>
             </fieldset><br>

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
