<% do_pagehead1(); %>      
<html>
   <head>
      <title><% nv_get("model"); %> - User/Current</title>
      <% do_pagehead2(); %>
      <script type="text/javascript">
        var F;
        function init() {
          F = document.getElementsByName("form")[0];
          <%widget_start();%>
        }
      </script>
   </head>
   <body class="gui" onload="init();">
      <div id="wrapper">
            <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
               <% do_menu("Status", "User/Current"); %>
        <script type="text/javascript">do_header_post()</script>
            </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>
	  <script type="text/javascript">do_page_path('<%lang("Status");%>', '<%lang("User/Current");%>')</script>	
               <div id="contentsInfo">
                  <fieldset>
                  <legend>ARP <%lang("Table");%> (<%st_user_show_arp_user_num();%>)</legend>
                     <table class="table center Rule-Table">
                        <tr>
                           <th><% lang("IP Address"); %></th>
                           <th><% lang("MAC Address"); %></th>
                           <th>ARP <% lang("Type"); %></th>
                        </tr>
                        <% st_user_show_arp(); %>
                     </table>
                  </fieldset>
                  <div id="floatKiller"></div> <br />
                  <form name="form">
	                <script type="text/javascript">do_hidden_elements('st_usr_arp')</script>
                     <div class="submitFooter">
                        <input onclick="window.location.replace('st_user_arp.asp')" type="button" value='<%lang("Refresh");%>' />
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
