<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - SNMP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var dhcp_objects = new Array(<%nv_get("lan_num");%>);
        var dhcpdnsaddr_objects = new Array(<%nv_get("lan_num");%>);
        var F;
        function to_submit(F) {
            $("#submit_button").val("dhcp");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

            /*
            F.submit_button.value = "dhcp";
            F.action.value = "Apply";
            ajax_preaction(F);
            display_mask(F);
            */
        }

        function SelDNSType(nth) {
            var enable = $("input[name=dhcp"+nth+"_enable]:checked").val();
            if (enable == 0) {
                return;
            }

            if ($('#dhcp'+nth+'_dnstype').val()== "dnsrelay") {
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr1').val("");
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr2').val("");
            } else if ($('#dhcp'+nth+'_dnstype').val()== "ispdns") {
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr1').val("");
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr2').val("");
            } else if ($('#dhcp'+nth+'_dnstype').val()== "opendns") {
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr1').val("208.67.220.220");
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr2').val("208.67.222.222");
            } else if ($('#dhcp'+nth+'_dnstype').val()== "googledns") {
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr1').val("8.8.8.8");
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",true)
                $('#dhcp'+nth+'_dnsaddr2').val("8.8.4.4");
            } else if ($('#dhcp'+nth+'_dnstype').val()== "custom") {
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",false);
                $('#dhcp'+nth+'_dnsaddr1').val("<%nvg_attr_get("dhcp$0_dnsaddr1", "lan_dhcps_rule", "$0", "dnsaddr1");%>");
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",false);
                $('#dhcp'+nth+'_dnsaddr2').val("<%nvg_attr_get("dhcp$0_dnsaddr2", "lan_dhcps_rule", "$0", "dnsaddr2");%>");
            } else{
                $('#dhcp'+nth+'_dnsaddr1').attr("disabled",true);
                $('#dhcp'+nth+'_dnsaddr2').attr("disabled",true);
            }
        }


        function init() {
            var F = document.getElementsByName("form")[0];
            for (var i = 0; i < <%nv_get("lan_num");%>; i++) {
                var enable = $("input[name=dhcp"+i+"_enable]:checked").val();
                sel_change(enable, 1, dhcp_objects[i]);
                sel_change(enable, 1, dhcpdnsaddr_objects[i]);
                SelDNSType(i);
            }
        }

        $(document).ready(function () {
            init();
        });

    /*
        var snmp_objects =
        [ 
        { name: 'snmp_udpport' },
        { name: 'snmp_tcpport' },
        { name: 'snmp_community' },
        { name: 'snmp_vendor' },
        { name: 'snmp_sysname' },
        { name: 'snmp_location' },
        { name: 'snmp_contact' },
        { name: 'snmp_timeout' },
        { name: 'snmp_auth' }
        ];                                    

        function onoffsnmp() {
            enable_snmp = $('input[name=snmp_enable]:checked').val();
            sel_change(enable_snmp, 1, snmp_objects);
        }

        $(document).ready(function () {
            onoffsnmp();
            $("#snmp_contact").val(decode64('<%b64_nvg_attr_get("snmp_contact", "snmp_rule", "0", "contact");%>'));
        });

        
        function to_submit(F) {
            $("#snmp_contact").val( encode64($("#snmp_contact").val()) );
            $("#submit_button").val("snmp");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
       }
        
*/
    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('dhcp_rule');</script>
                <h2><%lang("Setup");%>-<%lang("DHCP Server");%></h2>
              <% dhcp_show_setting(); %>

        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save Settings");%>">
                    <input type="reset" class="btn" value="<%lang("Cancel Changes");%>">
                </div>
            </div>
        </div><!-- row -->

        </form>
        
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>