<% do_pagehead1(); %>
<html>
   <head>
      <% do_pagehead2(); %>
   </head>
   <body>
      <div class="message">
         <div>
           Firmware Version: <%show_version();%>
         <div>
           <table border=1>
            <tr>
              <td>MAC Address</td><td class="opt_value_center"><%show_hwaddr("wan0");%></td>
            </tr>
            <tr>
              <td>Wireless MAC Address</td><td class="opt_value_center"><%show_hwaddr("wl0");%></td>
            </tr>
            <tr>
              <td>Ethernet MAC Address</td><td class="opt_value_center"><%show_hwaddr("lan0");%></td>
            </tr>
           </table>
         </div>
      </div>
   </body>
</html>
