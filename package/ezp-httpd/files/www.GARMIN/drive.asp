<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Drive</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var drive_objects =
[ { name: 'drive_guest' },
  { name: 'drive_ext_access' },
  { name: 'drive_port' }
];
var F; 

function to_submit(F) {
    F.submit_button.value = "drive";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("drive_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, drive_objects);
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("TOGOBox", "Drive"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("TOGOBox");%>', '<%lang("Drive");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('drive_rule')</script>	

              <fieldset>
                <legend><%lang("Drive");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Drive"); %></div>
                  <input type="radio" value="1" name="drive_enable" onclick="init()"
                        <%nvg_attr_match("drive_enable","drive_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="drive_enable" onclick="init()"
                        <%nvg_attr_match("drive_enable","drive_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Drive Allow Guest"); %></div>
                  <input type="radio" value="1" name="drive_guest" onclick="init()"
                        <%nvg_attr_match("drive_guest","drive_rule","0","guest","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="drive_guest" onclick="init()"
                        <%nvg_attr_match("drive_guest","drive_rule","0","guest","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Drive External Access"); %></div>
                  <input type="radio" value="1" name="drive_ext_access" onclick="init()"
                        <%nvg_attr_match("drive_ext_access","drive_rule","0","ext_access","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="drive_ext_access" onclick="init()"
                        <%nvg_attr_match("drive_ext_access","drive_rule","0","ext_access","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label">
                    <% lang("Drive Port"); %>
                  </div>HTTP <input class="num" maxlength="5" size=
                  "5" name="drive_port" onblur=
                  "valid_range(this,1,65535,'Port number')" value=
                  "<%nv_attr_get("drive_rule", "0", "port");%>">
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
