<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - SNMP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
        var F;

        var wl_apcli_objects = new Array(<%nv_get("wl_rule_num");%>);
        var wl_apcli_key = new Array(<%nv_get("wl_rule_num");%>);
        function encodekey(backup) {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var apcli_num;
                if (i=="0") apcli_num = "<%nv_get("wl0_apcli_rule_num");%>";
                if (i=="1") apcli_num = "<%nv_get("wl1_apcli_rule_num");%>";
                if (backup) wl_apcli_key[i]=new Array(apcli_num);
                for (var j = 0; j < apcli_num; j++) {
                    var secmode = $('#wl'+i+'_apcli'+j+'_secmode').val();
                    if (secmode == 'wep') {
                        if ($('#wl'+i+'_apcli'+j+'_key1').val() == "") return false;
                         if (backup) wl_apcli_key[i][j]=new Array(4);
                        for (var k = 1; k < 5; k++) {
                            if (backup) {
                                wl_apcli_key[i][j][k-1]=$('#wl'+i+'_apcli'+j+'_key'+k).val();
                                $('#wl'+i+'_apcli'+j+'_key'+k).val(encode64($('#wl'+i+'_apcli'+j+'_key'+k)[0].val()));
                            }else 
                                $('#wl'+i+'_apcli'+j+'_key'+k).val(wl_apcli_key[i][j][k-1]);
                        }
                    }
                    if (secmode == 'psk' || secmode == 'psk2') {
                        if ($('#wl'+i+'_apcli'+j+'_key').val() == "") return false;
                        if (backup) {
                            wl_apcli_key[0]=$('#wl'+i+'_apcli'+j+'_key').val();
                            $('#wl'+i+'_apcli'+j+'_key').val(encode64($('#wl'+i+'_apcli'+j+'_key').val()));
                        }else
                            $('#wl'+i+'_apcli'+j+'_key').val(wl_apcli_key[0]);
                    }
                }
            }
            return true;
        }

        function to_submit(F) {
            if (!encodekey(1)) {
                alert('The Key must have value.');
                return;
            }

            $("#submit_button").val("wl_apcli");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
            encodekey(0);
        }

        function SelAPCLI(F) {
            F = document.getElementsByName('form')[0];
            F.submit_button.value = "wl_apcli";
            F.action.value = "Gozila";
            F.submit();
        }

        function init() {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                /* XXX: wl0_apcli_rule_num represents for each wl interface. */
                for (var j = 0; j < <%nv_get("wl0_apcli_rule_num");%>; j++) {
                    var enable = $("input[name=wl"+i+"_apcli"+j+"_enable]:checked").val();
                    //var enable = (EN("wl"+i+"_apcli"+j+"_enable")[0].checked == true) ? 1 : 0;
                    sel_change(enable, 1, wl_apcli_objects[i]);
                }
            }
        }

        $(document).ready(function () {
            init();
        });

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window("wl_apcli.asp");</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('wl_apcli_rule');</script>
                <h2><%lang("Wireless");%>-<%lang("Universal Repeater");%></h2>
            <% wl_show_setting("wl_apcli_setting.asp"); %>

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