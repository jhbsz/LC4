<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - UPnP</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var upnp_objects = [ { name: 'upnp_port' },
                         { name: 'pmp_enable', type: 'radio' }  ];
    var F;

    function to_submit(F) {
        F.submit_button.value = "fr_upnp";
	    F.action.value = "Apply";
        ajax_preaction(F, "apply.cgi", 6000);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        var enable = (document.getElementsByName("upnp_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, upnp_objects);
        <%widget_start();%>
    }
    </script>
  </head>
  <body class="gui" onload="init()"> <% showad(); %>
    <div id="wrapper">
      <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Applications", "UPnP"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
            do_page_path('<%lang("Applications");%>', '<%lang("UPnP / NAT-PMP");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
      <div id="contents">
        <form name="form" action="apply.cgi" method="post">
	      <script type="text/javascript">do_hidden_elements('upnp_rule')</script>
          <fieldset>
            <legend>UPnP</legend>
            <div class="setting">
              <div class="label">UPnP</div>
              <input type="radio" value="1" name="upnp_enable" onclick="init()"
                    <% nvg_attr_match("upnp_enable", "upnp_rule", "0", "enable", "1", "checked"); %>>
                    <% lang("Enable"); %>
              </input>
              <input type="radio" value="0" name="upnp_enable" onclick="init()"
                    <% nvg_attr_match("upnp_enable", "upnp_rule", "0", "enable", "0", "checked"); %>>
                    <% lang("Disable"); %>
              </input>
            </div>
	        <input type="hidden" name="upnp_extif" value="wan0" />
	        <input type="hidden" name="upnp_inif" value="lan0" />
<!--
            <div class="setting">
              <div class="label">NAT-PMP</div>
              <input type="radio" value="1" name="pmp_enable"
                    <% nvg_attr_match("pmp_enable", "upnp_rule", "0", "pmp_enable", "1", "checked"); %>>
                    <% lang("Enable"); %>
              </input>
              <input type="radio" value="0" name="pmp_enable"
                    <% nvg_attr_match("pmp_enable", "upnp_rule", "0", "pmp_enable", "0", "checked"); %>>
                    <% lang("Disable"); %>
              </input>
            </div>
-->
            <div class="setting">
              <div class="label">UPnP <% lang("Port"); %></div>
              <input class="num" name="upnp_port" size="5" maxlength="5" 
                     onBlur="valid_range(this,0,65535,'UPnP')" 
                     value='<%nvg_attr_get("upnp_port", "upnp_rule", "0", "port");%>'/>
            </div>
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
