<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Serial Setting</title>
    <% do_pagehead2(); %>
  </head>
  <body class="gui" onload="init()">
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Serial Setting", ""); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Serial setting");%>', '<%lang("Port $0");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('als_serial')</script>	
              <input type="hidden" name="als_page" value="$1"/>
              <fieldset>
                <legend><%lang("Serial Setting");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Port alias");%></div>
                  <input type="text" id="alias" name="alias" value="<%nv_attr_get("als_serial_rule" ,"$1", "alias");%>"/>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Baud Rate");%></div>
                  <select id="baudrate" name="baudrate" ></select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Data bits");%></div>
                  <select id="databit" name="databit">
                      <option value="5" <%nvg_attr_match("","als_serial_rule", "$1", "databit", "5", "selected");%>>5</option>
                      <option value="6" <%nvg_attr_match("","als_serial_rule", "$1", "databit", "6", "selected");%>>6</option>
                      <option value="7" <%nvg_attr_match("","als_serial_rule", "$1", "databit", "7", "selected");%>>7</option>
                      <option value="8" <%nvg_attr_match("","als_serial_rule", "$1", "databit", "8", "selected");%>>8</option>
                  </select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Stop bits");%></div>
                  <select id="stopbit" name="stopbit">
                      <option value="1" <%nvg_attr_match("","als_serial_rule", "$1", "stopbit", "1", "selected");%>>1</option>
                      <option value="1.5" <%nvg_attr_match("","als_serial_rule", "$1", "stopbit", "1.5", "selected");%>>1.5</option>
                      <option value="2" <%nvg_attr_match("","als_serial_rule", "$1", "stopbit", "2", "selected");%>>2</option>
                  </select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Parity");%></div>
                  <select id="parity" name="parity">
                      <option value="none" <%nvg_attr_match("","als_serial_rule", "$1", "parity", "none", "selected");%>>None</option>
                      <option value="odd" <%nvg_attr_match("","als_serial_rule", "$1", "parity", "odd", "selected");%>>Odd</option>
                      <option value="even" <%nvg_attr_match("","als_serial_rule", "$1", "parity", "even", "selected");%>>Even</option>
                      <option value="mark" <%nvg_attr_match("","als_serial_rule", "$1", "parity", "mark", "selected");%>>Mark</option>
                      <option value="space" <%nvg_attr_match("","als_serial_rule", "$1", "parity", "space", "selected");%>>Space</option>
                  </select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Flow Control");%></div>
                  <select id="flow" name="flow">
                      <option value="none" <%nvg_attr_match("","als_serial_rule", "$1", "flow", "none", "selected");%>>None</option>
                      <option value="rts" <%nvg_attr_match("","als_serial_rule", "$1", "flow", "rts", "selected");%>>RTS/CTS</option>
                      <option value="xon" <%nvg_attr_match("","als_serial_rule", "$1", "flow", "xon", "selected");%>>XON/XOFF</option>
                      <option value="dtr" <%nvg_attr_match("","als_serial_rule", "$1", "flow", "dtr", "selected");%>>DTR/DSR</option>
                  </select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("FIFO");%></div>
                  <input type="radio" name="fifo_set" value="1" <%nvg_attr_match("","als_serial_rule", "$1", "fifo", "1", "checked");%>><%lang("Enable");%></input>
                  <input type="radio" name="fifo_set" value="0" <%nvg_attr_match("","als_serial_rule", "$1", "fifo", "0", "checked");%>><%lang("Disable");%></input>
                </div>
                
                <input type="hidden" value="<%nv_attr_get("als_serial_rule", "$1", "fifo");%>" id="als_fifo" name="als_fifo" />
              </fieldset><br />

              <fieldset>
                <legend><%lang("Operating Setting");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Operation Mode");%></div>
                  <select id="mode" name="mode" >
                      <option value="0" <%nvg_attr_match("","als_operating_rule", "$1", "mode", "0", "selected"  );%>><%lang("TCP Server Mode");%></option>
                      <option value="1" <%nvg_attr_match("","als_operating_rule", "$1", "mode", "1", "selected"  );%>><%lang("TCP Client Mode");%></option>
                      <option value="2" <%nvg_attr_match("","als_operating_rule", "$1", "mode", "2", "selected"  );%>><%lang("UDP Mode");%></option>
                  </select>
                </div>

                <div class="setting" align="center"><%lang("Data Packing");%></div>

                <div class="setting">
                    <div class="label"><%lang("Packing length");%></div>
                    <input type="text" name="length" value="<%nv_attr_get("als_operating_data_packing_rule","$1", "length");%>"/>(0 - 1024)
                </div>
                <div class="setting">
                    <div class="label"><%lang("Force transmit");%></div>
                    <input type="text" name="transmit" value="<%nv_attr_get("als_operating_data_packing_rule","$1", "transmit");%>"/>(0 - 65535 ms)
                </div>
                
                <div id="operating"></div>

              </fieldset>
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
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
    <script type="text/javascript">
        var F;
        var baudrate = ["110","134","150","300","600","1200","2400","4800","7200","9600","19200","38400","57600","115200","230400"];
    $(document).ready(function(){
        for(var x = 0; x < baudrate.length; x++)
            $("#baudrate").append($("<option></option>").attr("value",baudrate[x]).text(baudrate[x]));
            
        $("#baudrate option[value='<%nv_attr_get("als_serial_rule", "$1" , "baudrate");%>']").attr("selected",true);
        $("#mode option[value='<%nv_attr_get("als_operating_rule", "$1" , "mode");%>']").attr("selected",true);
        $("#mode").change();
        init();
    });    

    $("input[type='radio']").click(function()
    {
        $("#als_fifo").val($(this).val());
    });

    $("#inherit").click(function(){
        if($(this).attr("checked") == "checked")
     	    $(this).val("1");
     	else    
     	    $(this).val("0");
    });

    function to_submit(F) {
    	F.als_page.value = $("input[name='als_page']").val();
        F.submit_button.value = "als_serial";
        F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
    }
    $("#mode").change(function(){
        if($(this).val() != 2){
            $("#operating").load("als_operating_tcp_server.asp?$0&$1");
            if($(this).val() == 1)
                $("#operating").load("als_operating_tcp_client.asp?$0&$1");
            }else
              $("#operating").load("als_operating_udp.asp?$0&$1");
    });
    </script>
  </body>
</html>
