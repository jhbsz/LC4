<% do_pagehead1(); %>

<html>
<head>

<style type='text/css'>
#upgrade-time {
    font-size: 26px;
}
</style>

  <title><% nv_get("model"); %> -
  Administration</title><% do_pagehead2(); %>
<script type='text/javascript' src='table.js'></script>
<script type="text/javascript">
  
var F;

function to_submit(F) {
    F.submit_button.value = "ad_tools";
    F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

function to_adtool(F, cmd) {
    if (E(cmd+'host').value == "") {
        alert('<% lang("Needs a host name or an IP address"); %>');
        return false;
    }
    if ((isNaN(E(cmd+'cnt').value)) || (cmd != "traceroute" && ((E(cmd+'cnt').value < 1) || (E(cmd+'cnt').value > 10)))) {
        alert('<% lang("Please use 1 ~ 10 in Number of Packets"); %>');
        return false;
    }
    if ((isNaN(E(cmd+'cnt').value)) || (cmd == "traceroute" && ((E(cmd+'cnt').value < 1) || (E(cmd+'cnt').value > 15)))) {
        alert('<% lang("Please use 1 ~ 15 in Hop Count "); %>');
        return false;
    }
    F.action.value=cmd;
    F.submit_button.value = "ad_tools";
    ajax_preaction(F, "ad_tools.cgi");
    display_mask(F);
    return true;
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
        <% do_menu("Admin", "System Utilities"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Admin");%>', '<%lang("System Utilities");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id= "setup">
	        <script type="text/javascript">do_hidden_elements('adtool_rule')</script>

            <fieldset>
              <legend><% lang("Ping"); %></legend>

              <div class="setting">
                <div class="label">
                  <% lang("Interface"); %>
                </div>
                <select name="pingiface" id="pingiface">
                  <option value="any" 
                    <%nvg_attr_match("pingiface","adtool_rule", 0, "pingiface", "any", "selected");%>> 
                  <% lang("*"); %> </option>
                  <% ad_tools_showif("wan","ping"); %>
                  <% ad_tools_showif("lan","ping"); %>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Target Host"); %>
                </div>
                <input name="pinghost" id="pinghost" maxlength="45" size="16"
                     value="<% nvg_attr_get("pinghost", "adtool_rule", "0", "pinghost"); %>">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Number of Packets"); %> 
                </div>
                <input name="pingcnt" id="pingcnt" maxlength="15" size="16"
                     value="<% nvg_attr_get("pingcnt", "adtool_rule", "0", "pingcnt"); %>">
                <% lang("Packets"); %> <% lang("(1 ~ 10)"); %>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Ping"); %>
                </div>
                <input type="button" name="pingtest" id="pingtest" value='<%lang("Ping");%>' 
                  onclick="to_adtool(this.form, 'ping')">
              </div>

            </fieldset><br />

            <fieldset>
              <legend><% lang("ARPing"); %> (<% lang("Within the same broadcasting domain"); %>) </legend>

              <div class="setting">
                <div class="label">
                  <% lang("Interface"); %>
                </div>
                <select name="arpingiface" id="arpingiface">
                  <% ad_tools_showif("wan","arping"); %>
                  <% ad_tools_showif("lan","arping"); %>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Target Host"); %>
                </div>
                <input name="arpinghost" id="arpinghost" maxlength="45" size="16"
                     value="<% nvg_attr_get("arpinghost", "adtool_rule", "0", "arpinghost"); %>">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Number of Packets"); %>
                </div>
                <input name="arpingcnt" id="arpingcnt" maxlength="15" size="16"
                     value="<% nvg_attr_get("arpingcnt", "adtool_rule", "0", "arpingcnt"); %>">
                 <%lang("Packets"); %> <% lang("(1 ~ 10)"); %> 
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("ARPing"); %>
                </div>
                <input type="button" name="arpingtest" id="arpingtest" value='<%lang("ARPing");%>' 
                  onclick= "to_adtool(this.form, 'arping')">
              </div>
            </fieldset><br />

            <fieldset>
              <legend><% lang("Trace Route"); %></legend>

              <div class="setting">
                <div class="label">
                  <% lang("Interface"); %>
                </div>
                <select name="tracerouteiface" id="tracerouteiface">
                  <option value="any" 
                    <%nvg_attr_match("tracerouteiface","adtool_rule", 0, "tracerouteiface", "any", "selected");%>> 
                  <% lang("*"); %> </option>
                  <% ad_tools_showif("wan","traceroute"); %>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Target Host"); %>
                </div>
                <input name="traceroutehost" id="traceroutehost" maxlength="45" size="16"
                     value="<% nvg_attr_get("traceroutehost", "adtool_rule", "0", "traceroutehost"); %>">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Hop Count"); %> 
                </div>
                <input name="traceroutecnt" id="traceroutecnt" maxlength="15" size="16"
                     value="<% nvg_attr_get("traceroutecnt", "adtool_rule", "0", "traceroutecnt"); %>">
                <%lang("Counts"); %> <% lang("(1 ~ 15)"); %>  
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Trace route"); %>
                </div>
                <input type="button" name="traceroutetest" id="traceroutetest" value='<%lang("Trace Route");%>' 
                  onclick= "to_adtool(this.form, 'traceroute')">
              </div>
              <textarea cols=120 rows=10><% st_show_traceroute();%></textarea>
            </fieldset><br />
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
    <script type="text/javascript">create_upgrade_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>
