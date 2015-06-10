<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - BT</title>
<% do_pagehead2("no_table"); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="ext/css/toolbars.css" />
<link rel="stylesheet" type="text/css" href="ext/css/file-upload.css"/>
<style type="text/css">
    .icon-add {
        background-image: url(icons/add.gif) !important;
    }
    .icon-delete {
        background-image: url(icons/cross.gif) !important;
    }
    .icon-stop {
        background-image: url(icons/delete.gif) !important; 
    }
    .upload-icon { 
        background: url('icons/image_add.png') no-repeat 0 0 !important;
    }
    #fi-button-msg {
        border: 2px solid #ccc;
        padding: 5px 10px;
        background: #eee;
        margin: 5px;
        float: left;
    }
</style>

<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="bt.js"></script>
<script type="text/javascript" src="ext/js/FileUploadField.js"></script>

<script type="text/javascript">
var bt_objects =
[ { name: 'bt_peer_num' },
  { name: 'bt_max_peer_num' },
  { name: 'bt_port' },
  { name: 'bt_dht_port' },
  { name: 'bt_dl_rate' },
  { name: 'bt_ul_rate' }
];

Ext.BLANK_IMAGE_URL = "icons/s.gif";

var F;
function init() {
    F = document.getElementsByName('form')[0];
    var enable = (document.getElementsByName("bt_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, bt_objects);

    setTimeout('bt_update_status();', 3000);
    <%widget_start();%>
}

function to_submit(F) {
    F.submit_button.value = "bt";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 35000);
    display_mask(F);
}

<%bt_show_content();%>
</script>

</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
            <% do_menu("Storage", "BT Download"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("BT Download");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('bt_rule')</script>
              <fieldset>
                <legend><%lang("BT Download");%></legend>
                <div class="setting">
                  <div class="label"><% lang("BT Download"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="bt_enable" onclick="init()"
                        <%nvg_attr_match("bt_enable","bt_rule","0","enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="bt_enable" onclick="init()"
                        <%nvg_attr_match("bt_enable","bt_rule","0","enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input></div>
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Peers Per File");%></div>
                  <input name="bt_peer_num" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_peer_num", "bt_rule", "0", "peer");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("BT Engine Maximum Peers");%></div>
                  <input name="bt_max_peer_num" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_max_peer_num", "bt_rule", "0", "max_peer");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("BT Port");%></div>
                  <input name="bt_port" size="5" maxLength="5"
                      value='<%nvg_attr_get("port", "bt_rule", "0", "port_range_start");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("DHT Port");%></div>
                  <input name="bt_dht_port" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_dht_port", "bt_rule", "0", "dht_port");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Download Rate");%></div>
                  <input name="bt_dl_rate" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_dl_rate", "bt_rule", "0", "download_rate");%>' /> KB/s (0: unlimited)
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Upload Rate");%></div>
                  <input name="bt_ul_rate" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_ul_rate", "bt_rule", "0", "upload_rate");%>' /> KB/s (0: unlimited)
                </div>

              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
              </div>

            </form>

            <br/><br/>

            <form name="form" action="apply.cgi" method="post">
              <div id="container" style="width:740px; height:350px">
                  <input type='hidden' name='type' />
                  <div id="gridpanel"></div>
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
