<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Services</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    var tos_objects =
[ { name: 'tos_ack_enable'  , type: 'radio' },
  { name: 'tos_icmp_enable' , type: 'radio' },
  { name: 'tos_dns_enable'  , type: 'radio' },
  { name: 'tos_ssh_enable'  , type: 'radio' },
  { name: 'tos_telnet_enable', type: 'radio' },
  { name: 'tos_check_enable', type: 'radio' }
];
    function to_submit(F) {
        F.submit_button.value = "bw_tos";
	    F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        var enable = (document.getElementsByName("tos_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, tos_objects);
        <%widget_start();%>
    }
    </script>
  </head>
  <body class="gui" onload="init()"> <% showad(); %>
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Bandwidth", "Throughput Optimizer"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Bandwidth");%>', '<%lang("Throughput Optimizer");%>')
      </script>
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('tos_rule')</script>
              <fieldset>
                <legend><%lang("Throughput Optimizer");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Throughput Optimizer");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_enable" onclick="init()"
                        <%nvg_attr_match("tos_enable", "tos_rule", "0", "enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_enable" onclick="init()"
                        <%nvg_attr_match("tos_enable", "tos_rule", "0", "enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
              </fieldset> <br />
              <fieldset>
                <legend><%lang("Application Priority");%></legend>
                <div class="setting">
                  <div class="label"><%lang("TCP ACK");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_ack_enable" 
                        <%nvg_attr_match("tos_ack_enable", "tos_rule", "0", "ack_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_ack_enable" 
                        <%nvg_attr_match("tos_ack_enable", "tos_rule", "0", "ack_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("ICMP");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_icmp_enable" 
                        <%nvg_attr_match("tos_icmp_enable", "tos_rule", "0", "icmp_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_icmp_enable" 
                        <%nvg_attr_match("tos_icmp_enable", "tos_rule", "0", "icmp_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("DNS");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_dns_enable" 
                        <% nvg_attr_match("tos_dns_enable", "tos_rule", "0", "dns_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_dns_enable" 
                        <% nvg_attr_match("tos_dns_enable", "tos_rule", "0", "dns_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("SSH");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_ssh_enable" 
                        <% nvg_attr_match("tos_ssh_enable", "tos_rule", "0", "ssh_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_ssh_enable" 
                        <% nvg_attr_match("tos_ssh_enable", "tos_rule", "0", "ssh_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Telnet (BBS)");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_telnet_enable" 
                        <%nvg_attr_match("tos_telnet_enable", "tos_rule", "0", "telnet_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_telnet_enable" 
                        <%nvg_attr_match("tos_telnet_enable", "tos_rule", "0", "telnet_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("TCP Max Segment Size");%></div>
                  <div class='radiobutton'><input type="radio" value="1" name="tos_check_enable" 
                        <%nvg_attr_match("tos_check_enable", "tos_rule", "0", "check_enable", "1", "checked");%>><%lang("Enable");%>
                  </input></div>
                  <div class='radiobutton'><input type="radio" value="0" name="tos_check_enable" 
                        <%nvg_attr_match("tos_check_enable", "tos_rule", "0", "check_enable", "0", "checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick=to_submit(this.form) />
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
