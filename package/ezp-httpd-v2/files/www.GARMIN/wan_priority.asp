<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - WAN</title>
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
    var num = 3; 
    var value_set = ["dhcp", "wwan", "wisp"];
    var name_set = ["DHCP", "3G/4G Mobile Internet", "WISP"];

    function init() {
        F = EN('form')[0];
        var enable = (EN("priority_enable")[0].checked == true) ? 1 : 0;
        var high = '<%nvg_attr_get("", "wan_priority_rule", "0", "proto");%>';
        var middle = '<%nvg_attr_get("", "wan_priority_rule", "1", "proto");%>';
        var low = '<%nvg_attr_get("", "wan_priority_rule", "2", "proto");%>';

        var high_id = document.getElementById("high");
        var middle_id = document.getElementById("middle");
        var low_id = document.getElementById("low");

        var temp_value = [];
        var temp_name = [];

        for(var i=0;i<num;i++)
        {
            if(high==value_set[i])
            {
                temp_value.push(value_set[i]);
                temp_name.push(name_set[i]);
            }
        }

        for(var i=0;i<num;i++)
        {
            if(middle==value_set[i])
            {
                temp_value.push(value_set[i]);
                temp_name.push(name_set[i]);
            }
        }

        for(var i=0;i<num;i++)
        {
            if(low==value_set[i])
            {
                temp_value.push(value_set[i]);
                temp_name.push(name_set[i]);
            }
        }

        for(var i=0;i<num;i++)
        {
            high_id.options[i] = new Option(temp_name[i], temp_value[i]);
        }
        temp_value.splice(0,1);
        temp_name.splice(0,1);

        for(var i=0;i<num-1;i++)
        {
            middle_id.options[i] = new Option(temp_name[i], temp_value[i]);
        }
        temp_value.splice(0,1);
        temp_name.splice(0,1);

        for(var i=0;i<num-2;i++)
        {
            low_id.options[i] = new Option(temp_name[i], temp_value[i]);
        }
        temp_value.splice(0,1);
        temp_name.splice(0,1);

        if(enable == 1)
        {
            document.getElementById("high").disabled = false;
            document.getElementById("middle").disabled = false;
            document.getElementById("low").disabled = false;
            document.getElementById("priority_num").value = num;
        }
        else
        {
            document.getElementById("high").disabled = true;
            document.getElementById("middle").disabled = true;
            document.getElementById("low").disabled = true;
            document.getElementById("priority_num").value = 0;
        }
    }

    function do_high(proto) {
        var temp_value = [];
        var temp_name = [];
        for(var i=0;i<num;i++)
        {
            if(proto!=value_set[i])
            {
                temp_value.push(value_set[i]);
                temp_name.push(name_set[i]);
            }
        }
        var middle_id = document.getElementById("middle");
        for(var i=middle_id.options.length-1; i>=0 ;i--)
        {
            middle_id.options[i] = null;
        }
        for(var i=0; i<num-1 ;i++)
        {
            middle_id.options[i] = new Option(temp_name.pop(), temp_value.pop());
        }
        do_middle(middle_id.value);
    }
    function do_middle(proto) {
        var temp_value = [];
        var temp_name = [];
        var low_id = document.getElementById("low");
        for(var i=low_id.options.length-2; i>=0 ;i--)
        {
            low_id.options[i] = null;
        }
        for(var i=0;i<num;i++)
        {
            var high = document.getElementById("high").value;
            if(proto==value_set[i])
            {
                continue;
            }
            else if(high==value_set[i])
            {
                continue;
            }
            else
            {
                temp_value.push(value_set[i]);
                temp_name.push(name_set[i]);
            }
        }
        for(var i=0; i<num-2 ;i++)
        {
            low_id.options[i] = new Option(temp_name.pop(), temp_value.pop());
        }
    }

    function to_submit(F) {
	    F.action.value = "Apply";
        F.submit_button.value = "wan";
        ajax_preaction(F, "apply.cgi", 6000);
        display_mask(F);
    }

    function alarm () {
        if (useIE6) {
            alert('<%lang("For the security and display reasons, IE7.0+ and FireFox are strongly suggested.");%>');
        }
    }
     
    </script>
  </head>
    
  <body class="gui" onload="alarm(); init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "WAN"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("WAN");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" id="form" action="apply.cgi" method="post" >
            <script type="text/javascript">do_hidden_elements('wan_priority_rule')</script>
            <fieldset>
              <legend><% lang("WAN Priority"); %></legend>

                <div class="setting">
                  <div class="label">
                    <%lang("WAN Priority");%>
                  </div>
                  <input type="radio" value="3" name= "priority_enable" id="priority_num" <%nvg_attr_match("priority_enable","wan_status_rule","0","priority_num","3","checked");%> onClick="init()"><%lang("Enable");%>
                  <input type="radio" value="0" name= "priority_enable" id="priority_num" <%nvg_attr_match("priority_enable","wan_status_rule","0","priority_num","0","checked");%>  onClick="init()"><%lang("Disable");%>
                </div>

                <div class="setting">
                    <div class="label">
                        <%lang("High Priority");%>
                    </div>

                    <select name="high" id="high" onchange="do_high(this.value);">
                    <option value="dhcp"><%lang("DHCP");%></option>
                    <option value="wwan"><%lang("3G/4G Mobile Internet");%></option>
                    <option value="wisp"><%lang("WISP");%></option>
                    </select>
                </div>
                <div class="setting">
                    <div class="label">
                        <%lang("Middle Priority");%>
                    </div>
                    <select name="middle" id="middle" onchange="do_middle(this.value);">
                    <option value="wwan"><%lang("3G/4G Mobile Internet");%></option>
                    <option value="wisp"><%lang("WISP");%></option>
                    </select>
                </div>
                <div class="setting">
                    <div class="label">
                        <%lang("Low Priority");%>
                    </div>
                    <select name="low" id="low">
                    <option value="wisp"><%lang("WISP");%></option>
                    </select>
                </div>
            </fieldset><br>       
         <div id="submitFoooter" class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='wan_priority.asp'" />
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
    <script type="text/javascript">create_return_window("wan_priority.asp")</script>
  </body>
</html>
