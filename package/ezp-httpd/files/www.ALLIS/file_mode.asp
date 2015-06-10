<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - File Mode</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    function to_submit(F) {
        F.submit_button.value = "file_mode";
        F.action.value = "Apply";
        if(F.file_usb.checked)
            F.file_usb.value = 1;
        else
            F.file_usb.value = 0;
            
        if(F.file_sd.checked)
            F.file_sd.value = 1;
        else
            F.file_sd.value = 0;
            
        ajax_preaction(F);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
    }
    
    </script>
  </head>
  <body class="gui" onload="init()">
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Serial", "File Mode"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Serial");%>','<%lang("File Mode");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('file_mode')</script>	
	          <input type="hidden" name="fime_mode" value="1" />
              <fieldset>
             	<legend><%lang("Global Setting");%></legend>
                <div class="setting">
                  <div class="label"><%lang("File Mode");%></div>
                  <div class="label">
                  	<input type="radio" name="enable" onclick="$('#file_enable').val('1');" <% nvg_attr_match("enable", "file_mode_rule", "0", "enable", "1", "checked"); %>><%lang("Enable");%></input>
                  	<input type="radio" name="enable" onclick="$('#file_enable').val('0');" <% nvg_attr_match("enable", "file_mode_rule", "0", "enable", "0", "checked"); %>><%lang("Disable");%></input>
                  </div>
                </div>
              
                <div class="setting">
                  <div class="label"><%lang("Destination");%></div>
                  <div class="label">
                  	<input type="checkbox" name="file_sd" <% nvg_attr_match("file_sd", "file_mode_rule", "0", "sd", "1", "checked"); %>><%lang("SD (Default)");%></input>
                  	<input type="checkbox" name="file_usb" <% nvg_attr_match("file_usb", "file_mode_rule", "0", "usb", "1", "checked"); %>><%lang("USB Storage");%></input>
                  </div>
                </div>
              </fieldset>
              
              <fieldset>
             	<legend><%lang("Signal Setting");%></legend>
                <div class="setting">
                  <div class="label"><%lang("UART Source");%></div>
                  <div class="label"><%lang("Record Method");%></div>
                </div>
                
                <div class="setting">
                  <div class="label"><%lang("Port 1 ");%></div>
                  <div class="label">
                  	<input type="radio" name="record_1" onclick="$('#file_record_1').val('1');"<% nvg_attr_match("record_1", "file_mode_rule", "0", "record_1", "1", "checked"); %>><%lang("Re Cycle");%></input>
                  	<input type="radio" name="record_1" onclick="$('#file_record_1').val('0');"<% nvg_attr_match("record_1", "file_mode_rule", "0", "record_1", "0", "checked"); %>><%lang("Full Stop");%></input>
                  </div>
                </div>
                
                <div class="setting">
                  <div class="label"><%lang("Port 2 ");%></div>
                  <div class="label">
                  	<input type="radio" name="record_2" onclick="$('#file_record_2').val('1');"<% nvg_attr_match("record_2", "file_mode_rule", "0", "record_2", "1", "checked"); %>><%lang("Re Cycle");%></input>
                  	<input type="radio" name="record_2" onclick="$('#file_record_2').val('0');"<% nvg_attr_match("record_2", "file_mode_rule", "0", "record_2", "0", "checked"); %>><%lang("Full Stop");%></input>
                  </div>
                </div>
                
                <div class="setting">
                  <div class="label"><%lang("Port 3 ");%></div>
                  <div class="label">
                  	<input type="radio" name="record_3" onclick="$('#file_record_3').val('1');"<% nvg_attr_match("record_3", "file_mode_rule", "0", "record_3", "1", "checked"); %>><%lang("Re Cycle");%></input>
                  	<input type="radio" name="record_3" onclick="$('#file_record_3').val('0');"<% nvg_attr_match("record_3", "file_mode_rule", "0", "record_3", "0", "checked"); %>><%lang("Full Stop");%></input>
                  </div>
                </div>
                
                <input type="hidden" value="<%nv_attr_get("file_mode_rule" ,"0", "enable");%>" name="file_enable" id="file_enable" />
                <input type="hidden" value="<%nv_attr_get("file_mode_rule" ,"0", "record_1");%>" name="file_record_1" id="file_record_1" />
                <input type="hidden" value="<%nv_attr_get("file_mode_rule" ,"0", "record_2");%>" name="file_record_2" id="file_record_2" />
                <input type="hidden" value="<%nv_attr_get("file_mode_rule" ,"0", "record_3");%>" name="file_record_3" id="file_record_3" />
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick='to_submit(this.form)' />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick=init() />
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
