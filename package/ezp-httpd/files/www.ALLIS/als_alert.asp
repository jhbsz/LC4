<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Alert</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    
    function to_submit(F) {
        F.submit_button.value = "alert";
        F.action.value = "Apply";
        
        display_mask(F);
        ajax_preaction(F);
    }

    </script>
  </head>
  <body class="gui" >
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "Alert"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>','<%lang("Alert");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('alert')</script>	
	          <input type="hidden" name="alert" value="1" />
	          
              <fieldset>
                <legend><%lang("Alert Notice");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Event");%></div>
                  <div class="label"><input name="battery" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "battery", "1", "checked"); %>/><%lang("Battery low");%>
                  		     <input name="sd" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "sd", "1", "checked"); %>/><%lang("SD Card Event");%><br>
                  		     <input name="usb" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "usb", "1", "checked"); %>/><%lang("USB Storage Event");%>
                  		     <input name="relay_opt" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "relay", "1", "checked"); %>/><%lang("Relay On/Off");%><br>
                  		     <!--<input name="gpio" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "gpio", "1", "checked"); %>/><%lang("GPIO Input Event");%>--></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Event Send");%></div>
                  <div class="label"><!--<input name="sms" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "sms", "1", "checked"); %>/><%lang("SMS");%>-->
                  		     <input name="mail" type="checkbox" <%nvg_attr_match("", "mail_rule", "0", "mail", "1", "checked"); %>/><%lang("E-mail");%></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Email setting");%></div>
                  <div class="label"><%lang("E-mail");%>:<input name="address_1" type="text" value="<%nv_attr_get("mail_rule" ,"0", "address_1");%>"/><br>
                  		     <%lang("E-mail");%>:<input name="address_2" type="text" value="<%nv_attr_get("mail_rule" ,"0", "address_2");%>"/></div>
                </div>
<!--
                <div class="setting">
                  <div class="label"><%lang("SMS setting");%></div>
                  <div class="label"><%lang("Phone");%><input name="phone" type="text" value="<%nv_attr_get("mail_rule" ,"0", "phone");%>"/></div>
                </div>-->
              </fieldset>
              
              <fieldset>
                <legend><%lang("Email setting");%></legend>
                <div class="setting">
                  <div class="label"><%lang("E-Mail");%></div>
                  <div class="label"><input name="email" type="text" value="<%nv_attr_get("mail_rule" ,"0", "email");%>"/></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("SMTP Server");%></div>
                  <div class="label"><input name="server" type="text" value="<%nv_attr_get("mail_rule" ,"0", "server");%>"/></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Send Mail");%></div>
                  <div class="label"><input name="send_mail" type="text" value="<%nv_attr_get("mail_rule" ,"0", "send_mail");%>"/></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("SMTP Account");%></div>
                  <div class="label"><input name="act" type="text" value="<%nv_attr_get("mail_rule" ,"0", "act");%>"/></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("SMTP Password");%></div>
                  <div class="label"><input name="pwd" type="password" value="<%nv_attr_get("mail_rule" ,"0", "pwd");%>"/></div>
                </div>
              </fieldset>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick='to_submit(this.form)' />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload();" />
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
    <script type="text/javascript">
        
        $(function(){

        F = document.getElementsByName("form")[0];
        <%widget_start();%>
        }); 

</script>
  </body>
</html>
