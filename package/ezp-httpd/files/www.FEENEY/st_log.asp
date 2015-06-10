<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Log</title>
<% do_pagehead2(); %>
<link rel="stylesheet" type="text/css" href="ext/css/ext-all.css" />
<link rel="stylesheet" type="text/css" href="style/ext-style.css" />
<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<script type="text/javascript" src="log.js"></script>

<script type="text/javascript">
    function init() {
        F = document.getElementsByName("form")[0];
        <%widget_start();%>
     }

Ext.BLANK_IMAGE_URL = "icons/s.gif";
</script>

<style type="text/css">
#container table {
    border-spacing: 0em;
    margin: 0 10px 0 10px;
    width: 0%;
    }
</style>
</head>
   <body class="gui" onload="init()">
      <div id="wrapper">
            <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
               <% do_menu("Admin", "Log"); %>
        <script type="text/javascript">do_header_post()</script>
            </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Admin");%>', '<%lang("Log");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
               <div id="contentsInfo">
                  <div id="container" style="height:550px">
                      <div id="log_form"></div>
                  </div>
                  <div id="floatKiller"></div><br/>
<!--
                  <form name="form">
                      <script type="text/javascript">
                           do_hidden_elements('st_log')</script>
                      <div class="submitFooter">
                          <input onclick="window.location.replace('st_log.asp')" type="button" value='<%lang("Refresh");%>' />
                      </div>
                  </form>
-->
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
