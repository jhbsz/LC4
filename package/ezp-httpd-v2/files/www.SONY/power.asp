<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - <%lang("Power Management");%></title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;
var power_p_objects =
[ { name: 'p_countdown' }
];

function to_submit(F) {
	// Set default timer
    var p_enable = (document.getElementsByName("p_enable")[0].checked == true) ? 1 : 0;
	if (!p_enable) {
		$('#p_countdown').removeAttr('checked');
		$('input[id=p_countdown]:eq(0)').attr('checked', 'checked');
	}

    F.submit_button.value = "power";
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    F = document.getElementsByName("form")[1];
    var p_enable = (document.getElementsByName("p_enable")[0].checked == true) ? 1 : 0;
    //sel_change(p_enable, 1, F, power_p_objects); // Do not do timer disable action
	if (!p_enable) {
		$('#p_countdown').removeAttr('checked');
	}
    <%widget_start();%>
}

// If timer selected, power off mode
function setEnable() {
		$('#p_enable').removeAttr('checked');
		$('input[id=p_enable]:eq(0)').attr('checked', 'checked');
}

</script>
</head>
  <body class="gui" onload="init();menuFreezer();"> <% showad(); %>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Power", "Power Saving"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Power Management");%>', '')
      </script>
      <div id="description"><%lang("You can change the setting for power off timer.");%></div>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('power_saving_rule')</script>
                <div class="setting">
                  <div class="label"><% lang("Power off timer"); %></div>
				  <div style="display: none;">
                      <input id="p_enable" type="radio" value="1" name="p_enable" onclick="init()"
                        <%nvg_attr_match("p_enable", "countdown_rule","0","p_enable","1","checked");%>> <%lang("Enable");%>
                      </input>
				  </div>
                  <input id="p_enable" type="radio" value="0" name="p_enable" onclick="init()"
                        <%nvg_attr_match("p_enable", "countdown_rule","0","p_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label">&nbsp;</div>
				  <input id="p_countdown" type="radio" name="p_countdown" onclick="setEnable()" value="10" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "10", "checked");%>>10 <% lang("mins"); %><br/>
                  <div class="label">&nbsp;</div>
				  <input id="p_countdown" type="radio" name="p_countdown" onclick="setEnable()" value="60" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "60", "checked");%>>60 <% lang("mins"); %><br/>
                  <div class="label">&nbsp;</div>
				  <input id="p_countdown" type="radio" name="p_countdown" onclick="setEnable()" value="120" <%nvg_attr_match("p_countdown", "countdown_rule", "0", "p_countdown", "120", "checked");%>>120 <% lang("mins"); %><br/>
                </div>
              <br />
              <div class="submitFooter">
                <input type="reset" value="<%lang("Cancel");%>" onClick="window.location.reload()"/>
                <input type="button" name="save_button" value="<%lang("Save");%>" onClick="to_submit(this.form)" />
              </div>

            </form>

          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <!--<%widget_show();%>-->
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
