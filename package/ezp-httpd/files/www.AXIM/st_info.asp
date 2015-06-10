<% do_pagehead1(); %>

<html>
<head>
  <title><% nv_get("model"); %> - System information</title>
  <% do_pagehead2(); %>
  <script type="text/javascript">
        var F;
  function init() {
          F = document.getElementsByName("form")[0];
  }
  var value=0;
  function Refresh() {
        if (value) {
                window.location.replace("st_info.asp");
        }
        value++;
  }
  </script>

<style type="text/css">
    table td {
      font-size:14px;
    } 
</style>
</head>

<body class="gui" onload="init();">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <script type="text/javascript">do_header_post()</script>
      </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id="status">
              <fieldset>
              <legend><%lang("Router Information");%></legend>
                    <table>
                    <tr>
                      <td width=150 >Model</td><td><%nv_get("model");%></td>
                    </tr>

                    <tr>
                      <td>Version</td><td><%show_version("factory");%></td>
                    </tr>

                    <tr>
                      <td>Custom Version</td> <td><%show_version("custom");%></td>
                    </tr>

                    <tr>
                      <td>License</td><td><%show_license_status();%></td>
                    </tr>

                    <tr>
                      <td>Running Time</td><td><%show_uptime();%></td>
                    </tr>

                    <tr>
                      <td>MAC Address</td><td><%show_eeprom_hwaddr();%></td>
                    </tr>

                    <tr>
                      <td>Wireless Region</td><td><% show_wl_region(); %></td>
                    </tr>

                    <tr>
                      <td>Wireless Band</td><td><% show_wl_band(); %></td>
                    </tr>

                    <tr>
                      <td>TX Power Channel</td><td><% show_tx_channel(); %></td>
                    </tr>

                    <tr>
                      <td>TX Frequency Offset</td> <td><% show_freq_off(); %></td>
                    </tr>

                    <tr>
                      <td>U-boot</td><td><% show_uboot(); %></td>
                    </tr>

                    <tr>
                      <td>Bulid Time</td><td><% show_build_time(); %></td>
                    </tr>
                    </table>

              </fieldset> <br />
            <div id="floatKiller"></div>
            <div class="submitFooter">
              <input type="button" value='<%lang("Refresh");%>'
              onclick="window.location.replace('st_info.asp')">
            </div>
          </form>
        </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
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
