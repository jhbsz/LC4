<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Log</title>
<% do_pagehead2(); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="log.js"></script>

<script type="text/javascript">
var F;
var syslog_type_objects =
[ { name: 'remote_ip_ipaddr' },
  { name: 'proto' },
  { name: 'remote_port' }
];

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("remote_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable,1,F,syslog_type_objects);
    <%widget_start();%>
}
function to_submit(F) {
    F.submit_button.value = "st_log";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}
 
Ext.BLANK_IMAGE_URL = "icons/s.gif";
</script>

<style type="text/css">
#container table {
    border-spacing: 0em;
    margin: 0 10px 0 10px;
    width: 0%;
    }
</style>
</head>
   <body class="gui" onload="init()">
      <div id="wrapper">
            <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
               <% do_menu("Admin", "Log"); %>
        <script type="text/javascript">do_header_post()</script>
            </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Admin");%>', '<%lang("Log");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
               <div id="contentsInfo">
                  <div id="container" style="height:550px">
                      <div id="log_form"></div>
                  </div>
                  <div id="floatKiller"></div><br/>
                  <form name="form">
                      <script type="text/javascript">
                           do_hidden_elements('log_rule')</script>
                  <fieldset>
                  <legend><%lang("Log Server Setting");%></legend>
                  <div class="setting">
                  <div class="label"><% lang("Syslog Server"); %></div>
                    <input type="radio" value="1" name="remote_enable" onclick="init()"
                      <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","1","checked");%>>
                      <%lang("Enable");%>
                    </input>
                    <input type="radio" value="0" name="remote_enable" onclick="init()"
                      <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","0","checked");%>>
                      <%lang("Disable");%>
                    </input>
                  </div>
                  <div class="setting">
                  <div class="label"><%lang("Remote IP Address");%></div>
                    <input type="text" maxLength="15" size="16" name="remote_ip_ipaddr"
                      value='<%nv_attr_get("log_rule", "$0", "remote_ip");%>' 
                      onblur='valid_ipaddr(this, "Remote IP Address")' />
                    </input>
                  </div>
                  <div class='setting'>
                  <div class='label'> <%lang(Protocol);%> </div>
                    <select id='proto' name='proto'>
                    <option value='tcp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "tcp", "selected"); %>>TCP</option>
                    <option value='udp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "udp", "selected"); %>>UDP</option>
                    <option value='both' <% nvg_attr_match("proto", "log_rule", "0", "proto", "both", "selected"); %>>TCP/UDP</option>"
                    </select>
                  </div>
                  <div class="setting">
                  <div class="label"><% lang("Remote Port"); %>
                  </div><input class="num" maxlength="5" size="5" name="remote_port" onblur="valid_range(this,1,65535,'Port number')" value=
                    "<%nv_attr_get("log_rule", "0", "remote_port");%>">
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
