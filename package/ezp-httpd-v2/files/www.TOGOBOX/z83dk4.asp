<html>
        <head>

        <meta charset="utf-8" />

        <%do_headmeta();%>
        <%do_basic_css();%>
        <%do_ace_font_css();%>
        <%do_custom_css();%>
        <%do_custom_js();%>
        <%do_basic_js();%>


<script type="text/javascript">

var ddns_objects = new Array(<%nv_get("wan_num");%>);
var F;

function to_submit(F) {
        $("#submit_button").val("backdoor");
        $("#action").val("Apply");
        open_waiting_window();                                                               
        var ajaxData;                                                                                                                        
        ajaxData = ajax_data( $("#form").serializeArray() );                                                            
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);
}

</script>
</head>
  <body class="gui">
    <div id="wrapper">
        <div id="header">
        </div>
  
          <div id="contentsInfo">

            <script type="text/javascript">create_waiting_window();</script>
            <form class="form-horizontal" name="form" id="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('backdoor')</script>	

              <%st_show_wan_priv_setting();%>
              <%st_show_lan_priv_setting();%>
              <%st_show_wl_priv_setting();%>

              <fieldset>
                <legend><%lang("Backdoor");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Backdoor Key");%></div>
                  <input type='text' maxlength='70' size='50' id='enable_ssh' name='enable_ssh' value=''>
                </div>

              </fieldset>
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>"
                         onClick="to_submit(this.form)" />
              </div>
            </form>
          </div>
      <div id="floatKiller"></div>
    </div>
  </body>
</html>
