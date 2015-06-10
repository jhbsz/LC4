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
var ext_storage_objects = new Array(1);
ext_storage_objects[0] = [
  { name: 'storage_target_ip' },
  { name: 'storage_target_dir' },
  { name: 'storage_user' },
  { name: 'storage_passwd' },
  { name: 'storage_group' },
  { name: 'storage_ext_connect' }
];

  function to_format(F) {
        F.action.value='Format';
        if (!confirm('<%lang("Are you sure to format USB storage?\nAll data on it will be lost!");%>')) 
            return;
        E("format").disabled = true;
        F.getAttributeNode("action").value = "format.cgi";
        F.submit_button.value = "ad_storage";
        F.submit();
        E('waiting-window-msg').style.fontSize = '14px';
        display_mask(F, '<%lang("Please note: Router will blink a red light during the re-formatting process. Do not remove USB device or turn off the router.");%>');
        return true;
  }


  function to_submit(F) {
      F.submit_button.value = "ad_storage";
      F.action.value = "Apply";
      ajax_preaction(F);
      display_mask(F);
  }

  function init() {
      F = document.getElementsByName("form")[0];
      /* for some browser reload issue ex:firefox */
      E("format").disabled = false;
      var enable = (document.getElementsByName("storage_ext_enable")[0].checked == true) ? 1 : 0;
      sel_change(enable, 1, F, ext_storage_objects[0]);
      if (enable) {
          E("format_type").disabled = true;
          E("format").disabled = true;
      } else {
          E("format_type").disabled = false;
          E("format").disabled = false;
      }
      <%widget_start();%>
  }
  </script>
</head>

<body class="gui" onload="init()">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Storage", "Storage Device"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("Storage Management");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id= "setup">
	        <script type="text/javascript">do_hidden_elements('storage_rule')</script>

            <fieldset>
              <legend><% lang("Storage Device"); %></legend>

              <div class="setting">
                <div class="label">
                  <%lang("Language");%>
                </div><select name="storage_codepage">
                  <option value=
                  "437"<%nvg_attr_match("storage_codepage",
                    "storage_rule", "0", "codepage", "437", "selected");%>>
                  <%lang("United States, Canada");%>
                  </option>
                  <option value=
                  "850" <% nvg_attr_match("storage_codepage",
                    "storage_rule", "0", "codepage", "850", "selected");%>>
                  <%lang("Europe");%>
                  </option>
                  <option value=
                  "936" <% nvg_attr_match("storage_codepage", 
                    "storage_rule", "0", "codepage", "936", "selected");%>>
                  <%lang("Simplified Chinese");%>
                  </option>
                  <option value=
                  "950" <% nvg_attr_match("storage_codepage",
                    "storage_rule", "0", "codepage", "950", "selected");%>>
                  <%lang("Traditional Chinese");%>
                  </option>
                  <option value=
                  "932" <% nvg_attr_match("storage_codepage",
                    "storage_rule", "0", "codepage", "932", "selected");%>>
                  <%lang("Japanese");%>
                  </option>
                  <option value=
                  "949" <% nvg_attr_match("storage_codepage",
                    "storage_rule", "0", "codepage", "949", "selected");%>>
                  <%lang("Korean");%>
                  </option>
                </select>
              </div>

              <div class="setting">
                <div class="label">
                  <%lang("Swap File Size");%>
                </div><select name="storage_swap_size">
                  <option value=
                  "16384" <% nvg_attr_match("storage_swap_size",
                    "storage_rule", "0", "swap_size", "16384", "selected");%>>
                  <%lang("64MB");%>
                  </option>
                  <option value=
                  "32768" <% nvg_attr_match("storage_swap_size",
                    "storage_rule", "0", "swap_size", "32768", "selected");%>>
                  <%lang("128MB");%>
                  </option>
                  <option value=
                  "65535" <% nvg_attr_match("storage_swap_size",
                    "storage_rule", "0", "swap_size", "65535", "selected");%>>
                  <%lang("256MB");%>
                  </option>
                </select>
              </div>

            </fieldset><br />
<%prod_comment_html_start("PROD_CAT_K", "0", "7" , "");%>
            <fieldset>
              <legend><% lang("External Storage"); %></legend>
                <div class="setting">
                  <div class="label"><% lang("External Storage"); %></div>
                  <input type="radio" value="1" name="storage_ext_enable" onclick="init()"
                        <%nvg_attr_match("External Storage","storage_rule","0","ext_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="storage_ext_enable" onclick="init()"
                        <%nvg_attr_match("External Storage","storage_rule","0","ext_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

              <div class="setting">
               <div class="label"> <%lang("Target IP");%></div>
                <input name="storage_target_ip" size="16" maxLength="15"
                value='<%nvg_attr_get("Target IP", "storage_rule", "0", "target_ip");%>' /> </div>
              <div class="setting">
               <div class="label"> <%lang("Target Directory");%></div>
                <input name="storage_target_dir" size="21" maxLength="20"
                value='<%nvg_attr_get("Target Diretcory", "storage_rule", "0", "target_dir");%>' /> </div>
              <div class="setting">
               <div class="label"> <%lang("User Name");%></div>
                <input name="storage_user" size="21" maxLength="20"
                value='<%nvg_attr_get("User Name", "storage_rule", "0", "user");%>' /> </div>
              <div class="setting">
               <div class="label"> <%lang("Password");%></div>
                <input name="storage_passwd" size="21" maxLength="20"
                value='<%nvg_attr_get("Password", "storage_rule", "0", "pw");%>' /> </div>
              <div class="setting">
               <div class="label"> <%lang("Group");%></div>
                <input name="storage_group" size="21" maxLength="20"
                value='<%nvg_attr_get("Group", "storage_rule", "0", "group");%>' /> </div>

     <div class="setting">
        <div class="label"><%lang("Reconnect");%></div>
        <input type="button" name="storage_ext_connect"
         onclick="to_update('0', 'smbc')" value="<%lang("Connect");%>" />
      </div>


            </fieldset><br />
<%prod_comment_html_end("PROD_CAT_K", "0", "7" , "");%>

            <fieldset>
              <legend><% lang("Format"); %></legend>
              <div class="setting">
                <div class="label">
                  <%lang("File System Type");%>
                </div><select name="format_type" id="format_type">
                  <option value="vfat"> 
                  <%lang("FAT");%>
                  </option>
                  <option value="ext3">
                  <%lang("EXT3");%>
                  </option>
                </select>
              </div>


              <div class="setting">
                <div class="label"><% lang("Format USB HDD"); %></div>
                <input type="button" name="format" id="format" value='<%lang("Format");%>' onclick= "to_format(this.form)">
              </div>
            </fieldset><br />


              <div class="submitFooter">
                <input type="button" name="save_button" value= '<%lang("Save Settings");%>' onclick= "to_submit(this.form)">
                <input type="reset" value= '<%lang("Cancel Changes");%>' onClick="window.location.reload()" >
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
    <script type="text/javascript">create_upgrade_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>
