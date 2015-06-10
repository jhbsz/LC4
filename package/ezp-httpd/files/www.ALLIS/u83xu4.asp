<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get(""); %></title>
<% do_pagehead2(); %>
<script type="text/javascript">

var F;

function to_submit(F) {
	F.submit_button.value = "command_save";
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
<% do_menu("", ""); %>
<script type="text/javascript">do_header_post()</script>
</div>

<script type="text/javascript">do_body_pre()</script>	
<script type="text/javascript">do_page_path('<%lang("");%>','<%lang("");%>')</script>
<script type="text/javascript">do_table_pre()</script>	
<script type="text/javascript">do_mainframe_pre()</script>	
<div id="contents">
<form name="form" action="apply.cgi" method="post">
<script type="text/javascript">do_hidden_elements('command_save')</script>	

<fieldset>
<div class="setting">
<div class="label"><%lang("Wireless - 1 SSID Default Name");%></div>
<div class="label"><input type="text" id="wifi_1" name="w_1_d" value=""/></div>
<div class="label"><%lang("Wireless - 2 SSID Default Name");%></div>
<div class="label"><input type="text" id="wifi_2" name="w_2_d" value=""/></div>
<div class="label"><%lang("Model Name");%></div>
<div class="label"><input type="text" id="model" name="d_model" value=""/></div>

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
		$.ajax({                                                         
type: 'POST',                                             
url: "status.cgi",
data: {action:"command"},
success: function(i){     
var x;        
var group = i.split("\n");
for (x = 0;x < group.length && group[x];x++)
{
if(x == 0)
{
var wifi = group[x].split("|");
$("#wifi_1").val(wifi[0]);
$("#wifi_2").val(wifi[1]);
}else if (x == 1 )
$("#model").val(group[x]);

}
}
}); 

}); 

</script>
</body>
</html>
