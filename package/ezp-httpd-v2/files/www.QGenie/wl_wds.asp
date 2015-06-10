<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var F;

        var wl_wds_key=new Array(<%nv_get("wl_rule_num");%>);
        function encodekey(backup) {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var wds_num;
                if ($('#wl'+i+'_wds_mode').val() == "disabled") return true;
                if (i=="0") wds_num = "<%nv_get("wl0_wds_rule_num");%>";
                if (i=="1") wds_num = "<%nv_get("wl1_wds_rule_num");%>";
                if (backup) wl_wds_key[i]=new Array(wds_num);
                for (var j = 0; j < wds_num; j++) {
                    var secmode = $('#wl'+i+'_wds'+j+'_secmode').val();
                    if (secmode == 'wep') {
                        if ($('#wl'+i+'_wds'+j+'_key').val() == "") return 0;
                        if (!valid_wl_sec_wep_key(document.getElementsByName('wl'+i+'_wds'+j+'_key')[0],'WDS Key')) return -1;
                        if (backup) {
                            wl_wds_key[i][j]=$('#wl'+i+'_wds'+j+'_key').val();
                            $('#wl'+i+'_wds'+j+'_key').val(encode64($('#wl'+i+'_wds'+j+'_key').val()));
                        } else
                            $('#wl'+i+'_wds'+j+'_key').val(wl_wds_key[i][j]);
                    }
                    if (secmode == 'psk') {
                        if ($('#wl'+i+'_wds'+j+'_key').val() == "") return 0;
                        if (!valid_wl_sec_wpa_psk_key(document.getElementsByName('wl'+i+'_wds'+j+'_key')[0],'WDS Key')) return -1;
                        if (backup) {
                            wl_wds_key[i][j]=$('#wl'+i+'_wds'+j+'_key').val();
                            $('#wl'+i+'_wds'+j+'_key').val(encode64($('#wl'+i+'_wds'+j+'_key').val()));
                        }else
                            $('#wl'+i+'_wds'+j+'_key').val(wl_wds_key[i][j]);
                    }
                }
            }
            return true;
        }

        function to_submit(F) {
            var val=encodekey(1);
            if (val=='0') {
                alert('<%lang("The Key must have value.");%>');
                return;
            } else if (val=='-1') return;

            $("#submit_button").val("wl_wds");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function SelWDS(F) {
            F = document.getElementsByName('form')[0];;
            F.submit_button.value = "wl_wds";
            F.action.value = "Gozila";
            F.submit();
        }

        function init() {
        }

        $(document).ready(function () {
            init();
        });

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window("wl_wds.asp");</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('wl_wds_rule');</script>
                <h2><%lang("Wireless");%>-<%lang("WDS");%></h2>
            <% wl_show_setting("wl_wds_setting.asp"); %>
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
