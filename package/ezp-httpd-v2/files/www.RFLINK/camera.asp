<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Camera</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var camera_objects =
[ { name: 'camera_port' },
  { name: 'camera_auth_enable' }
];

var F; 

function to_submit(F) {
    F.submit_button.value = "camera";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("camera_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, camera_objects);
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("AirCloud", "Camera"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("AirCloud");%>', '<%lang("Camera");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('camera_rule')</script>	

              <fieldset>
                <legend><%lang("Camera");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Camera"); %></div>
                  <input type="radio" value="1" name="camera_enable" onclick="init()"
                        <%nvg_attr_match("camera_enable","camera_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="camera_enable" onclick="init()"
                        <%nvg_attr_match("camera_enable","camera_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label">
                    <% lang("Camera Port"); %>
                  </div>HTTP <input class="num" maxlength="5" size=
                  "5" name="camera_port" onblur=
                  "valid_range(this,1,65535,'Port number')" value=
                  "<%nv_attr_get("camera_rule", "0", "port");%>">
                </div>

                <div class="setting">
                  <div class="label"><% lang("Camera Account Login"); %></div>
                  <input type="radio" value="1" name="camera_auth_enable" onclick="init()"
                        <%nvg_attr_match("camera_auth_enable","camera_rule","0","auth_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="camera_auth_enable" onclick="init()"
                        <%nvg_attr_match("camera_auth_enable","camera_rule","0","auth_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                <div class="label"><% lang("Camera Resolution"); %></div>
                <select name="camera_resolution">
                <option value="1280x720" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "1280x720", "selected");%>><% lang("1280x720"); %></option>
                <option value="640x480" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "640x480", "selected");%>><% lang("640x480"); %></option>
                <option value="320x240" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "320x240", "selected");%>><% lang("320x240"); %></option>
                </select>
                </div>

              </fieldset><br />


              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
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
