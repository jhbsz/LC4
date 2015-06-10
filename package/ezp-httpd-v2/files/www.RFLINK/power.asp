<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Power Saving</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;

function to_submit(F) {
    F.submit_button.value = "power";
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();"> <% showad(); %>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Power", "Power Saving"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Power");%>', '<%lang("Power Saving");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('power_saving_rule')</script>
              <fieldset>
                <legend><%lang("Wireless");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Wireless Auto Disable"); %></div>
                  <input type="radio" value="1" name="w_enable" onclick="init()"
                        <%nvg_attr_match("w_enable", "countdown_rule","0","w_state","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="w_enable" onclick="init()"
                        <%nvg_attr_match("w_enable", "countdown_rule","0","w_state","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label"><% lang("Interval"); %></div>
                  <select name="w_countdown">
                  <option value="15" <%nvg_attr_match("w_countdown", "countdown_rule", "0", "w_countdown", "15", "selected");%>><% lang("15 mins"); %></option>
                  <option value="30" <%nvg_attr_match("w_countdown", "countdown_rule", "0", "w_countdown", "30", "selected");%>><% lang("30 mins"); %></option>
                  <option value="60" <%nvg_attr_match("w_countdown", "countdown_rule", "0", "w_countdown", "60", "selected");%>><% lang("60 mins"); %></option>
                  </select>
                </div>
              </fieldset><br />

              <fieldset>
                <legend><%lang("System");%></legend>
                <div class="setting">
                  <div class="label"><% lang("System Auto Turn off"); %></div>
                  <input type="radio" value="1" name="p_enable" onclick="init()"
                        <%nvg_attr_match("p_enable", "countdown_rule","0","p_state","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="p_enable" onclick="init()"
                        <%nvg_attr_match("p_enable", "countdown_rule","0","p_state","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label"><% lang("Interval"); %></div>
                  <select name="p_countdown">
                  <option value="15" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "15", "selected");%>><% lang("15 mins"); %></option>
                  <option value="30" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "30", "selected");%>><% lang("30 mins"); %></option>
                  <option value="60" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "60", "selected");%>><% lang("60 mins"); %></option>
                  </select>
                </div>
              </fieldset><br />
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
