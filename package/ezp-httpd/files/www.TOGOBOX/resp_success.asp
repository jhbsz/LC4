<% do_pagehead1(); %>
<html>
   <head>
      <% do_pagehead2(); %>
      <script language="JavaScript">
var submit_button = '<% show_next_page(); %>';
function to_submit()
{
	if(submit_button == "")
		history.go(-1);
	else
		document.location.href =  submit_button;
}
</script></head>
   <body>
      <div class="message">
         <div>
            <input type="hidden" name="success" />
            <form><% lang("Settings are saved successfully. Settings take effect immediately."); %><br />
            <input type="button" name="action" value="<%lang("Continue");%>" OnClick="to_submit()" /></form>
         </div>
      </div>
   </body>
</html>
