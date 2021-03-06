<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Operation Mode</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var ddns_objects = new Array(<%nv_get("wan_num");%>);
var F;

function to_submit(F) {
    F.submit_button.value = 'op_mode';
	F.action.value = "Apply";
    ajax_preaction(F);
    display_mask(F);
}

</script>
</head>
  <body class="gui">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('op_mode')</script>	

            <fieldset>
              <legend><% lang("Operation Mode"); %></legend>
              <div class="setting">
                <div class="label">
                  <%lang("Operation Mode");%>
                </div><select name="op_mode">
                  <option value=
                  "gateway" <% nvg_match("op_mode", "gateway", "selected"); %>>
                  <%lang("Gateway");%>
                  </option>
                  <option value=
                  "bridge" <% nvg_match("op_mode", "bridge", "selected"); %>>
                  <%lang("Bridge");%>
                  </option>
                </select>
              </div>
            </fieldset>
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>"
                         onClick="to_submit(this.form)" />
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
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
