<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - TurboNAT</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;

function to_submit(F) {
    F.submit_button.value = "turbonat";
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
        <% do_menu("Bandwidth", "TurboNAT"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Bandwidth");%>', '<%lang("TurboNAT");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('turbonat_rule')</script>
<%prod_show_html_start("hwnat", "0", "100");%>
              <fieldset>
                <legend><%lang("Hardware NAT");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Hardware NAT"); %></div>
                  <input type="radio" value="1" name="hwnat_enable" onclick="init()"
                        <%nvg_attr_match("hwnat_enable", "nat_rule","0","hw_enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="hwnat_enable" onclick="init()"
                        <%nvg_attr_match("hwnat_enable", "nat_rule","0","hw_enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
<%prod_show_html_end("hwnat", "0", "100");%>
              <fieldset>
                <legend><%lang("TurboNAT");%></legend>
                <div class="setting">
                  <div class="label"><% lang("TurboNAT"); %></div>
                  <input type="radio" value="1" name="turbonat_enable" onclick="init()"
                        <%nvg_match("turbonat_enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="turbonat_enable" onclick="init()"
                        <%nvg_match("turbonat_enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
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
