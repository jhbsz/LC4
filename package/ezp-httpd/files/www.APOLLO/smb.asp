<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Samba</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var smbd_objects =
[ { name: 'smbd_share_name' },
  { name: 'smbd_read_only' },
  { name: 'smbd_guest_ok' },
  { name: 'smbd_ext_access' },
  { name: 'smbd_codepage' },
  { name: 'smbd_passwd' },
  { name: 'smbd_passwd_confirm' }
];
var F; 

function to_submit(F) {
    if (F.smbd_passwd.value != F.smbd_passwd_confirm.value) {
        alert("Confirmed password did not match Entered Password.  Please re-enter password");
        F.smbd_passwd_confirm.focus();
        F.smbd_passwd_confirm.select();
        return false;
    }
    F.submit_button.value = "samba";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("smbd_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, smbd_objects);
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "Samba Server"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("Samba Server");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('smbd_rule')</script>	

              <fieldset>
                <legend><%lang("Samba Server");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Samba Server"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="smbd_enable" onclick="init()"
                        <%nvg_attr_match("smbd_enable","smbd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="smbd_enable" onclick="init()"
                        <%nvg_attr_match("smbd_enable","smbd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
                <div class="setting">
                <div class="label"><%lang("Samba Server Share Name");%></div>
                <input name="smbd_share_name" size="30" maxLength="30"
                value='<%nvg_attr_get("smbd_share_name", "smbd_rule",   "0", "share_name");%>' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("Samba Server Allow Guest"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="smbd_guest_ok" onclick="init()"
                        <%nvg_attr_match("smbd_guest_ok","smbd_rule","0","guest_ok","1","checked");%>><%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="smbd_guest_ok" onclick="init()"
                        <%nvg_attr_match("smbd_guest_ok","smbd_rule","0","guest_ok","0","checked");%>><%lang("Disable");%>
                  </input></div>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Samba Server Read Only"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="smbd_read_only" onclick="init()"
                        <%nvg_attr_match("smbd_read_only","smbd_rule","0","read_only","1","checked");%>><%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="smbd_read_only" onclick="init()"
                        <%nvg_attr_match("smbd_read_only","smbd_rule","0","read_only","0","checked");%>><%lang("Disable");%>
                  </input></div>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Samba Server External Access"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="smbd_ext_access" onclick="init()"
                        <%nvg_attr_match("smbd_ext_access","smbd_rule","0","ext_access","1","checked");%>><%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="smbd_ext_access" onclick="init()"
                        <%nvg_attr_match("smbd_ext_access","smbd_rule","0","ext_access","0","checked");%>><%lang("Disable");%>
                  </input></div>
                </div>

              <div class="setting">
                <div class="label">
                  <%lang("Language");%>
                </div><select name="smbd_codepage">
                  <option value=
                  "cp437"<%nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "cp437", "selected");%>>
                  <%lang("United States, Canada");%>
                  </option>
                  <option value=
                  "cp850" <% nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "cp850", "selected");%>>
                  <%lang("Europe");%>
                  </option>
                  <option value=
                  "cp936" <% nvg_attr_match("smbd_codepage", 
                    "smbd_rule", "0", "codepage", "cp936", "selected");%>>
                  <%lang("Simplified Chinese");%>
                  </option>
                  <option value=
                  "cp950" <% nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "cp950", "selected");%>>
                  <%lang("Traditional Chinese");%>
                  </option>
                  <option value=
                  "cp932" <% nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "cp932", "selected");%>>
                  <%lang("Japanese");%>
                  </option>
                  <option value=
                  "cp949" <% nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "cp949", "selected");%>>
                  <%lang("Korean");%>
                  </option>
                  <option value=
                  "utf8" <% nvg_attr_match("smbd_codepage",
                    "smbd_rule", "0", "codepage", "utf8", "selected");%>>
                  <%lang("Unicode");%>
                  </option>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Samba Password"); %>
                </div><input type="password" maxlength="32" size=
                "30" value="slide" name=
                "smbd_passwd" onblur=
                "valid_name(this,'Password',SPACE_NO)">
              </div>

              <div class="setting">
                <div class="label">
                  <% lang("Re-type Password"); %>
                </div><input type="password" maxlength="32" size=
                "30" value="slide" name=
                "smbd_passwd_confirm" onblur=
                "valid_name(this,'Password',SPACE_NO)">
              </div>
              <div class="label"><% lang("( Default user name is admin, default password is admin )"); %></div> 
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
