<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Firewall</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    function to_submit(F) {
        F.submit_button.value = "fw_basic";
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
  <body class="gui" onload="init()">
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Security", "Firewall"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("Firewall");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('fw_rule')</script>	
	          <input type="hidden" name="fw_enable" value="1" />
              <fieldset>
                <legend><%lang("Firewall Protection");%></legend>
                <div class="setting">
                  <div class="label"><%lang("SPI Firewall Protection");%></div>
                  <input type="radio" value="1" name="fw_state_enable" 
                        <% nvg_attr_match("state_enable", "fw_rule", "0", "state_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="fw_state_enable" 
                        <% nvg_attr_match("state_enable", "fw_rule", "0", "state_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
                <div class="setting">
                  <div class="label"><%lang("TCP SYN DoS Protection");%></div>
                  <input type="radio" value="1" name="fw_syn_enable" 
                        <% nvg_attr_match("syn_enable", "fw_rule", "0", "syn_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="fw_syn_enable" 
                        <% nvg_attr_match("syn_enable", "fw_rule", "0", "syn_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
                <div class="setting">
                  <div class="label"><%lang("ICMP Broadcasting Protection");%></div>
                  <input type="radio" value="1" name="fw_icmp_enable" 
                        <% nvg_attr_match("icmp_enable", "fw_rule", "0", "icmp_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                  <input type="radio" value="0" name="fw_icmp_enable" 
                        <% nvg_attr_match("icmp_enable", "fw_rule", "0", "icmp_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                </div>
                <div class="setting">
                  <div class="label"><%lang("ICMP Redirect Protection");%></div>
                  <input type="radio" value="1" name="fw_icmp_redir_enable" 
                        <%nvg_attr_match("icmp_redir_enable", "fw_rule", "0", "icmp_redir_enable", "1", "checked");%>><%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="fw_icmp_redir_enable" 
                        <%nvg_attr_match("icmp_redir_enable", "fw_rule", "0", "icmp_redir_enable", "0", "checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <input type="hidden" value="0" name="fw_rpfilter_enable" />
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick='to_submit(this.form)' />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick=init() />
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
