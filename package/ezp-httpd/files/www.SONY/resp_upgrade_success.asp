<html>
    <head>
        <script language='javascript' type='text/javascript' src='ext/js/jquery-min.js'></script>
        <script language="JavaScript">
        function init_wait() {
            var lanip = "<%nv_get("lan0_ipaddr");%>" + ":" + "<%nv_attr_get("http_rule", 0, "port");%>";
            var val = "wait('"+lanip+"');";
            setTimeout(val, 60000);
        }
        function wait(lanip) {
            var file = "http://" + lanip;
            var val = "wait('"+lanip+"');";
            location.replace(file);
            setTimeout(val, 3000);
        }
$(document).ready(function(){
    var lang = '<% nv_get("lang"); %>';
    if(lang == 'AR') {
        $("#formnow").attr("dir", "RTL");
        $("#formnow").attr("style", "text-align:right;");
    }
});
        </script>
    </head>
    <body>
        <div class="message">
            <div>
                <form name="formnow" id="formnow">
                <script type="text/javascript">
                    document.write('<input type="hidden"');
                    document.write(init_wait());
                    document.write('>');
                </script>
                    <% lang("WG-C10 will reboot to complete the update. The connection between WG-C10 and the wireless LAN will end, so reconnect and confirm that the update has been properly completed after the rebooting."); %>
                </form>
            </div>
        </div>
    </body>
</html>
