<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - USB Led Blink Behavior</title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;

function to_submit(F) {
    F.submit_button.value = 'usbled';
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
	          <script type="text/javascript">do_hidden_elements('usbled')</script>	

            <fieldset>
              <legend><% lang("USB Led"); %></legend>
              <div class="setting">
                <div class="label">
                  <%lang("USB Led Blink When Unlicenced:");%>
                </div>
                <select name="usbled">
                  <option value="0" 
                    <% nvg_attr_match("usbled", "appmgr_rule", "$0", "usbled", "0", "selected"); %>>
                  <%lang("off");%>
                  </option>
                  <option value="1" 
                    <% nvg_attr_match("usbled", "appmgr_rule", "$0", "usbled", "1", "selected"); %>>
                  <%lang("on");%>
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
