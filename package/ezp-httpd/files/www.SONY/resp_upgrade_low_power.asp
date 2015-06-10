<html>
    <head>
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
        </script>
    </head>
    <body>
        <div class="message">
            <div>
                <form name="formnow">
                <script type="text/javascript">
                    document.write('<input type="hidden"');
                    document.write(init_wait());
                    document.write('>');
                </script>
                <%lang("Cannot begin update. Recharge the battery until the battery level is at least 60%.");%><br/>
                    <br />
                </form>
            </div>
        </div>
    </body>
</html>
