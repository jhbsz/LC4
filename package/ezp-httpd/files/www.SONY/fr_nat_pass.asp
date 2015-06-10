<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Streaming/VPN</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    function to_submit(F) {
        F.submit_button.value = "fr_nat_pass";
	    F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
    }
    </script>
  </head>
  <body class="gui" onload="init()"> <% showad(); %>
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Applications", "Streaming / VPN"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Applications");%>', '<%lang("Streaming / VPN");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('nat_pass_rule')</script>
	          <input type="hidden" name="nat_pass_enable" value="1" />
              <fieldset>
                <legend><%lang("Streaming");%></legend>
                <div class="setting">
                  <div class="label">RTSP</div>
                  <input type="radio" value="1" name="nat_pass_rtsp_enable" 
                        <% nvg_attr_match("rtsp_enable", "nat_pass_rule", "0", "rtsp_enable", "1", "checked"); %>>
                        <% lang("Enable"); %>
                  </input>
                  <input type="radio" value="0" name="nat_pass_rtsp_enable" 
                        <% nvg_attr_match("rtsp_enable", "nat_pass_rule", "0", "rtsp_enable", "0", "checked"); %>>
                        <% lang("Disable"); %>
                  </input>
                </div>
                <div class="setting">
                  <div class="label">MMS</div>
                  <input type="radio" value="1" name="nat_pass_mms_enable" 
                        <% nvg_attr_match("mms_enable", "nat_pass_rule", "0", "mms_enable", "1", "checked"); %>>
                        <% lang("Enable"); %>
                  </input>
                  <input type="radio" value="0" name="nat_pass_mms_enable" 
                        <% nvg_attr_match("mms_enable", "nat_pass_rule", "0", "mms_enable", "0", "checked"); %>>
                        <% lang("Disable"); %>
                  </input>
                </div>
              </fieldset><br />
              <fieldset>
                <legend><%lang("Video Conference");%></legend>
                <div class="setting">
                  <div class="label">H.323</div>
                  <input type="radio" value="1" name="nat_pass_h323_enable" 
                        <% nvg_attr_match("h323_enable", "nat_pass_rule", "0", "h323_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="nat_pass_h323_enable" 
                        <% nvg_attr_match("h323_enable", "nat_pass_rule", "0", "h323_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
              </fieldset><br />
              <fieldset>
                <legend><%lang("VPN");%></legend>
                <div class="setting">
                  <div class="label">IPSec</div>
                  <input type="radio" value="1" name="nat_pass_ipsec_enable" 
                        <% nvg_attr_match("ipsec_enable", "nat_pass_rule", "0", "ipsec_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="nat_pass_ipsec_enable" 
                        <% nvg_attr_match("ipsec_enable", "nat_pass_rule", "0", "ipsec_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
                <div class="setting">
                  <div class="label">PPTP</div>
                  <input type="radio" value="1" name="nat_pass_pptp_enable" 
                        <% nvg_attr_match("pptp_enable", "nat_pass_rule", "0", "pptp_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="nat_pass_pptp_enable" 
                        <% nvg_attr_match("pptp_enable", "nat_pass_rule", "0", "pptp_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
                <input type="hidden" value="0" name="nat_pass_l2tp_enable" />
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick=to_submit(this.form) />
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
