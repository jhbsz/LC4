<% do_pagehead1(); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
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
    var wl_apcli_objects = new Array(<%nv_get("wl_rule_num");%>);
    var wl_apcli_key = new Array(<%nv_get("wl_rule_num");%>);
    function encodekey(backup) {
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            var apcli_num;
            if (i=="0") apcli_num = "<%nv_get("wl0_apcli_rule_num");%>";
            if (i=="1") apcli_num = "<%nv_get("wl1_apcli_rule_num");%>";
            if (backup) wl_apcli_key[i]=new Array(apcli_num);
            for (var j = 0; j < apcli_num; j++) {
                var secmode = EN('wl'+i+'_apcli'+j+'_secmode')[0].value;
                if (secmode == 'wep') {
                    if (EN('wl'+i+'_apcli'+j+'_key1')[0].value == "") return false;
                     if (backup) wl_apcli_key[i][j]=new Array(4);
                    for (var k = 1; k < 5; k++) {
                        if (backup) {
                            wl_apcli_key[i][j][k-1]=EN('wl'+i+'_apcli'+j+'_key'+k)[0].value;
                            EN('wl'+i+'_apcli'+j+'_key'+k)[0].value=encode64(EN('wl'+i+'_apcli'+j+'_key'+k)[0].value);
                        }else 
                            EN('wl'+i+'_apcli'+j+'_key'+k)[0].value=wl_apcli_key[i][j][k-1];
                    }
                }
                if (secmode == 'psk' || secmode == 'psk2') {
                    if (EN('wl'+i+'_apcli'+j+'_key')[0].value == "") return false;
                    if (backup) {
                        wl_apcli_key[0]=EN('wl'+i+'_apcli'+j+'_key')[0].value;
                        EN('wl'+i+'_apcli'+j+'_key')[0].value=encode64(EN('wl'+i+'_apcli'+j+'_key')[0].value);
                    }else
                        EN('wl'+i+'_apcli'+j+'_key')[0].value=wl_apcli_key[0];
                }
            }
        }
        return true;
    }

    function to_submit(F) {
        if (!encodekey(1)) {
            alert('The Key must have value.');
            return;
        }
        F.action.value = "Apply";
        F.submit_button.value = "wl_apcli";
        //ajax_preaction(F, "apply.cgi", true);
        ajax_preaction(F);
        encodekey(0);
        display_mask(F);
    }

    function SelAPCLI(F) {
        F.submit_button.value = "wl_apcli";
        F.action.value = "Gozila";
        F.submit();
    }

    function init() {
        F = EN("form")[0];

        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            /* XXX: wl0_apcli_rule_num represents for each wl interface. */
            for (var j = 0; j < <%nv_get("wl0_apcli_rule_num");%>; j++) {
                var enable = (EN("wl"+i+"_apcli"+j+"_enable")[0].checked == true) ? 1 : 0;
                sel_change(enable, 1, F, wl_apcli_objects[i]);
            }
        }
        <%widget_start();%>
    }
    </script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "Universal Repeater"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("Universal Repeater");%>')</script>
        <div id="contentsInfo">
          <form name="form" id="form" action="apply.cgi" method="post" >
	        <script type="text/javascript">do_hidden_elements('wl_apcli_rule')</script>	
            <% wl_show_setting("wl_apcli_setting.asp"); %>
            <div class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='wl_apcli.asp'" />
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
    <script type="text/javascript">create_return_window("wl_apcli.asp")</script>
  </body>
</html>
