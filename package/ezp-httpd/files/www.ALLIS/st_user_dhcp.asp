<% do_pagehead1(); %>      
<html>
   <head>
      <title><% nv_get("model"); %> - User/DHCP</title>
      <% do_pagehead2(); %>
      <script type="text/javascript">
        var F;
        $(function(){
          F = document.getElementsByName("form")[0];
          <%widget_start();%>
        });

	var Lang_Caption = "DHCP <%lang("Table");%> (<% st_user_show_dhcp_user_num();%>)";
	$(document).ready(function(){
		$("#user_list").append("<td><% lang("Name"); %></td><td><% lang("IP Address"); %></td><td><% lang("MAC Address"); %></td><td><% lang("Expiration Time"); %></td>");
		$("#user_list").append("<% st_user_show_dhcp(); %>");
	});


      </script>
   </head>
   <body class="gui">
      <div id="wrapper">
            <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
               <% do_menu("Status", "User/DHCP"); %>
        <script type="text/javascript">do_header_post()</script>
            </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Status");%>', '<%lang("User/DHCP");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
               <div id="contentsInfo">
				  <table id="DHCPListData" class="table center"></table>
				  <div id="DHCPListDataPager"></div>
				<div id="floatKiller"></div> <br />
                  <form name="form">
	                <script type="text/javascript">do_hidden_elements('st_user_dhcp')</script>
	                <table id="user_list" border=1 ></table>
                     <div class="submitFooter">
                        <input onclick="window.location.replace('st_user_dhcp.asp')" type="button" value='<%lang("Refresh");%>' />
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
    <script type="text/javascript">create_return_window(1)</script>
   </body>
</html>
