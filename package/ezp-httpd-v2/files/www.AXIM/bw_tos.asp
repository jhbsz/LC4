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
        var F;
        var tos_objects =
        [ { name: 'tos_ack_enable'  , type: 'radio' },
          { name: 'tos_icmp_enable' , type: 'radio' },
          { name: 'tos_dns_enable'  , type: 'radio' },
          { name: 'tos_ssh_enable'  , type: 'radio' },
          { name: 'tos_telnet_enable', type: 'radio' },
          { name: 'tos_check_enable', type: 'radio' }
        ];

        function to_submit(F) {
            $("#submit_button").val("bw_tos");
            $("#action").val("Apply");
            sel_change(1, 1, tos_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function init() {
            enable = $('input[name=tos_enable]:checked').val();
            sel_change(enable, 1, tos_objects);
        }

        $(document).ready(function () {
            init();
        });

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('tos_rule');</script>
                <h2><%lang("Bandwidth");%>-<%lang("Throughput Optimizer");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Throughput Optimizer");%></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("Throughput Optimizer");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_enable" id="tos_enable" onclick="init()"
                                <%nvg_attr_match("tos_enable", "tos_rule", "0", "enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_enable" id="tos_enable" onclick="init()"
                                <%nvg_attr_match("tos_enable", "tos_rule", "0", "enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>

        <div class="row">
            <div class="span12">
                <legend><%lang("Application Priority");%></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("TCP ACK");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_ack_enable" id="tos_ack_enable" 
                                <%nvg_attr_match("tos_ack_enable", "tos_rule", "0", "ack_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_ack_enable" id="tos_ack_enable" 
                                <%nvg_attr_match("tos_ack_enable", "tos_rule", "0", "ack_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><%lang("ICMP");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_icmp_enable" id="tos_icmp_enable" 
                                <%nvg_attr_match("tos_icmp_enable", "tos_rule", "0", "icmp_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_icmp_enable" id="tos_icmp_enable" 
                                <%nvg_attr_match("tos_icmp_enable", "tos_rule", "0", "icmp_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DNS");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_dns_enable" id="tos_dns_enable" 
                                <% nvg_attr_match("tos_dns_enable", "tos_rule", "0", "dns_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_dns_enable" id="tos_dns_enable" 
                                <% nvg_attr_match("tos_dns_enable", "tos_rule", "0", "dns_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("SSH");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_ssh_enable" id="tos_ssh_enable" 
                                <% nvg_attr_match("tos_ssh_enable", "tos_rule", "0", "ssh_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_ssh_enable" id="tos_ssh_enable" 
                                <% nvg_attr_match("tos_ssh_enable", "tos_rule", "0", "ssh_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>

                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Telnet (BBS)");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_telnet_enable" id="tos_telnet_enable" 
                                <%nvg_attr_match("tos_telnet_enable", "tos_rule", "0", "telnet_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_telnet_enable" id="tos_telnet_enable" 
                                <%nvg_attr_match("tos_telnet_enable", "tos_rule", "0", "telnet_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><%lang("TCP Max Segment Size");%></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="tos_check_enable" id="tos_check_enable" 
                                <%nvg_attr_match("tos_check_enable", "tos_rule", "0", "check_enable", "1", "checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="tos_check_enable" id="tos_check_enable" 
                                <%nvg_attr_match("tos_check_enable", "tos_rule", "0", "check_enable", "0", "checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
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