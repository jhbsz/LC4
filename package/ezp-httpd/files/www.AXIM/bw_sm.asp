<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Services</title>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var F;
    var sm_objects =
[ { name: 'sm_mode'  , type: 'radio' }
];
    function to_submit(F) {
        F.submit_button.value = "bw_sm_mode";
	    F.action.value = "Apply";
        ajax_preaction(F);
        display_mask(F);
    }

    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
    }
    </script>
  </head>
  <body class="gui" onload="init()"> <% showad(); %>
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Bandwidth", "Session Manager"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Bandwidth");%>', '<%lang("Session Manager");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('sm_rule')</script>
              <fieldset>
                <legend><%lang("Session Manager");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Recycle Mode");%></div>
                <select name="bw_sm_mode">
                  <option value=
                  "fast" <% nvg_match("sm_mode", "fast", "selected"); %>>
                  <%lang("Fast");%>
                  </option>

                  <option value=
                  "regular" <% nvg_match("sm_mode", "regular", "selected"); %>>
                  <%lang("Regular");%>
                  </option>
                  <option value=
                  "slow" <% nvg_match("sm_mode", "slow", "selected"); %>>
                  <%lang("Slow");%>
                  </option>
                </select>
                </div>
              </fieldset> <br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick=to_submit(this.form) />
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
