<% do_pagehead1(); %>
<html>
    <head>
    <% do_pagehead2(); %>
        <script language="JavaScript">
        function init_wait() {
            var lanip = "<%nv_get("lan0_ipaddr");%>" + ":" + "<%nv_attr_get("http_rule", 0, "port");%>";
            var val = "wait('"+lanip+"');";
            setTimeout(val, 45000);
        }
        function wait(lanip) {
            var file = "http://" + lanip;
            var val = "wait('"+lanip+"');";
            location.replace(file);
            setTimeout(val, 3000);
        }
        function waitstyle()
        {
        
            document.getElementById('waiting-window').style.left=get_left_offset(340);
        
        }

        </script>
        
        <style>
            .waiting-msg {
                font-size: 14px;
                color: #444444;
                text-align:center;
            }
        </style>
    </head>
    <body onload="waitstyle();">
        <div class="message">
                <script type="text/javascript">
                    document.write('<input type="hidden"');
                    document.write(init_wait());
                    document.write('>');
                </script>
                    
             
                <div id='waiting-window-wrapper' style='display:block;'></div>
                <div id='waiting-window' style='display:block; width:340px;'>
                    <div class='widgetInfo'>
                        <br/>
                        <div><img src='style/images/radial.gif' width='202px' height='19px' /></div><br>
                        <span class='waiting-msg'>The firmware you provided is invalid.
                        <br />Please try again.
                        <br /><br />Rebooting... Please wait....</span>
                        <span id='fromnow' style='font-size:20px;'></span>
                    </div>
                </div>


        </div>
    </body>
</html>
