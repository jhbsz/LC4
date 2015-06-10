<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <% do_pagehead2(); %>
    <script language="JavaScript">
        var useIE6 = false;
    </script>
    <!--[if lte IE 6]>
        <script language="JavaScript">
            var useIE6 = true;
        </script>
    <![endif]-->
    <script language="JavaScript">
    
    var F;

    function to_submit(F) {
        F.action.value = "Apply";
        F.submit_button.value = "mac_sense";
        ajax_preaction(F);
        encodekey(0);
        display_mask(F);
    }

    function init() {
	  <%widget_start();%>
    }
    </script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	<script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "SenseWiFi"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("SenseWiFi");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" >
	      <script type="text/javascript">do_hidden_elements('mac_sense')</script>	
              	<fieldset>
              	  <legend><%lang("SenseWiFi");%></legend>
              	  <div class="setting">
                          <div class="label"><%lang("Server IP Address");%></div>
                          <input type="text" id="server_ip" name="server_ip" value='<%nvg_attr_get("server_ip", "mac_sense_rule", "0", "server_ip");%>'/>
                  </div>        
                  <div class="setting">
                          <div class="label"><%lang("Server Port");%></div>
                          <input type="text" id="server_port" name="server_port" value='<%nvg_attr_get("server_port", "mac_sense_rule", "0", "server_port");%>'/>
                  </div>        
                  <div class="setting">
                          <div class="label"><%lang("Tramsmission Interval");%></div>
                          <input type="text" id="server_time" name="server_time" value='<%nvg_attr_get("server_time", "mac_sense_rule", "0", "server_time");%>'/>(>30)Sec.
                   </div>       
                  <div class="setting">
                          <div class="label"><%lang("Timeout");%></div>
                          <input type="text" id="server_timeout" name="server_timeout" value='<%nvg_attr_get("server_timeout", "mac_sense_rule", "0", "server_timeout");%>'/>(>5)Sec.
                   </div>       
              	</fieldset>    
              <div>
              	<fieldset>
              	  <legend><%lang("Force Gateway");%></legend>
              	  <div class="setting">
              	  <div class="label"><%lang("Force Gateway");%></div>
              	  	<input type="hidden" name="wds_enable" id="wds_enable" value="<%nvg_attr_get("wds_enable", "mac_sense_rule", "0", "wds_enable");%>"/>
                        <input type="radio" name="wds_enable_option" value="1" <%nvg_attr_match("wds_enable_option", "mac_sense_rule", "0", "wds_enable", "1", "checked");%>/><%lang("Enable");%>
                        <input type="radio" name="wds_enable_option" value="0" <%nvg_attr_match("wds_enable_option", "mac_sense_rule", "0", "wds_enable", "1", "checked");%>/><%lang("Disable");%>
                  </div>
              	  <div class="setting">
              	      <div class="label"><%lang("Gateway IP Address");%></div>
              	      <input typ="text" name="remote_ip" value="<%nvg_attr_get("remote_ip", "mac_sense_rule", "0", "remote_ip");%>"/>
              	  </div>  	    
              	</fieldset>	
              </div>
              <div class="submitFooter">
                  <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                  <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='mac_sense.asp'" />
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
    <script type="text/javascript">create_return_window("mac_sense.asp")</script>
    <script type="text/javascript">
    	$(function(){$("input[value='<%nvg_attr_get("wds_enable", "mac_sense_rule", "0", "wds_enable");%>']").click();});
   	$("input[name='wds_enable_option']").click(function(){
   	    $("#wds_enable").val($(this).val());
   	    if($(this).val() == 1)
   	    {
   	    	$("input[name='remote_ip']").attr("disabled",false);
   	    }
   	    else	
   	    	$("input[name='remote_ip']").attr("disabled",true);
   	}); 
	
	$("input[name='server_time']").blur(function(){
	    if( $(this).val() < 5 )
	    {
		alert("Tramsmission Interval can't less than 10");
		$(this).val("5");
	    }
	});

	$("input[name='server_timeout']").blur(function(){
	    if( $(this).val() < 5 )
	    {
		alert("Timeout can't less than 5");
		$(this).val("5");
	    }
	});
    </script>
  </body>
</html>
