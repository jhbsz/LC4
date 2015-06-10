<% do_pagehead1(); %>

<html>
<head>
  <title><% nv_get("model"); %> - Status</title>
  <% do_pagehead2(); %>
  <script type="text/javascript">
        var F;
  function init() {
          F = document.getElementsByName("form")[0];
        <%widget_start();%>
  }
  var value=0;
  function Refresh() {
        if (value) {
                window.location.replace("st_basic.asp");
        }
        value++;
        <%widget_start();%>
  }
  </script>
</head>

<body class="gui" onload="init();">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Status", "Router"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Status");%>', '<%lang("Router");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id="status">
	        <script type="text/javascript">do_hidden_elements('ad_basic_rule')</script>
            <input type="hidden" name="submit_button"> 
            <input type="hidden" name="action">
              <fieldset>
              <legend><%lang("Router Information");%></legend>
                <div class="setting">
                  <div class="label">
                    <%lang("Model Name");%>
                  </div><% nv_get("model"); %>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Firmware Version");%>
                  </div><% show_version(""); %>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("License");%>
                  </div><% show_license_status(); %>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Current Time");%>
                  </div><% show_localtime(); %>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Running Time");%>
                  </div><% show_uptime(); %>
                </div>
              </fieldset> <br />
              <%st_show_wan_setting();%>
              <%st_show_lan_setting();%>
              <%prod_show_html("start", "wlan");%>
              <%st_show_wl_setting();%>
              <%prod_show_html("end", "wlan");%>
            <div id="floatKiller"></div>
            <div class="submitFooter">
              <input type="button" value='<%lang("Refresh");%>'
              onclick="window.location.replace('st_basic.asp')">
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
    <script type="text/javascript">create_return_window(1)</script>
</body>
</html>
