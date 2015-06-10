<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Serial Setting</title>
    <% do_pagehead2(); %>
  </head>
  <body class="gui" >
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Serial", ""); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Serial");%>', '<%lang("Status");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('als_status')</script>	
              <div>
                  <table border=1 id="status" >
                      <tr align=center>
                        <td colspan=8>LINE</td>
                      </tr>
                      <tr align=center><td>Port</td><td>OP Mode</td><td>Data</td><td>ISP1</td><td>ISP2</td><td>ISP3</td><td>ISP4</td></tr>
                      <tr align=center><td>Port 1</td><td>Unknow</td><td>0</td><td>Unknow</td><td>Unknow</td><td>Unknow</td><td>Unknow</td></tr>
                      <tr align=center><td>Port 2</td><td>Unknow</td><td>0</td><td>Unknow</td><td>Unknow</td><td>Unknow</td><td>Unknow</td></tr>
                      <tr align=center><td>Port 3</td><td>Unknow</td><td>0</td><td>Unknow</td><td>Unknow</td><td>Unknow</td><td>Unknow</td></tr>
                  </table>
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
      $(document).ready(function(){
          getstatus();
    <%widget_start();%>
      });    
          function getstatus(){
              var status;
              $.ajax({
                      type: 'POST',
                      url: "status.cgi",
                      data: {action:"serial"},
                      success: function(i){
                          var port = i.split("port=");	
                          var x;
                          for(x = 0;x < port.length;x++)
                          {
                              var data = port[x].split("\n");
                              var y;
                              for( y = 1 ; y < data.length;y++)
                              {
                                  if(data[y])
                                  {
                                      if($("#status tr:eq(" + (x + 1) + ")").find("td").eq(y).text() > 0 && data[y] == 0)continue; 
				                      $("#status tr:eq(" + (x + 1) + ")").find("td").eq(y).text(data[y]);
                                  }
                              }
                          }
                          setTimeout("getstatus()",3000);
                      }
                  });
             }

</script>
</body>
</html>
