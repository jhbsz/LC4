<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Streaming/VPN</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        
        function to_submit(F) {
            $("#submit_button").val("fr_nat_pass");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
       }
        
    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('nat_pass_rule');</script>
                <input type="hidden" name="nat_pass_enable" id="nat_pass_enable" value="1" />
                <h2><%lang("Applications");%>-<%lang("Streaming / VPN");%></h2>

        <div class="row">
            <div class="span12">
                <fieldset>
                <legend><%lang("Streaming");%></legend>
                <div class="row show-grid">
                    <div class="span4">RTSP</div>
                    <div class="span8">     
                    <label class="radio inline">                   
                            <input type="radio" value="1" name="nat_pass_rtsp_enable" id="nat_pass_rtsp_enable"
                            <% nvg_attr_match("rtsp_enable", "nat_pass_rule", "0", "rtsp_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                    </label>
                    <label class="radio inline">
                            <input type="radio" value="0" name="nat_pass_rtsp_enable" id="nat_pass_rtsp_enable" 
                            <% nvg_attr_match("rtsp_enable", "nat_pass_rule", "0", "rtsp_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                    </label>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4">MMS</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="nat_pass_mms_enable" id="nat_pass_mms_enable"
                            <% nvg_attr_match("mms_enable", "nat_pass_rule", "0", "mms_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="nat_pass_mms_enable" id="nat_pass_mms_enable" 
                            <% nvg_attr_match("mms_enable", "nat_pass_rule", "0", "mms_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
                </fieldset><br />

                <fieldset>
                <legend><%lang("Video Conference");%></legend>
                <div class="row show-grid">
                    <div class="span4">H.323</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="nat_pass_h323_enable" id="nat_pass_h323_enable"
                            <% nvg_attr_match("h323_enable", "nat_pass_rule", "0", "h323_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="nat_pass_h323_enable" id="nat_pass_h323_enable" 
                            <% nvg_attr_match("h323_enable", "nat_pass_rule", "0", "h323_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
                </fieldset><br />

                <fieldset>
                <legend><%lang("VPN");%></legend>
                <div class="row show-grid">
                    <div class="span4">IPsec</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="nat_pass_ipsec_enable" id="nat_pass_ipsec_enable"
                            <% nvg_attr_match("ipsec_enable", "nat_pass_rule", "0", "ipsec_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="nat_pass_ipsec_enable" id="nat_pass_ipsec_enable" 
                            <% nvg_attr_match("ipsec_enable", "nat_pass_rule", "0", "ipsec_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4">PPTP</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="nat_pass_pptp_enable" id="nat_pass_pptp_enable"
                            <% nvg_attr_match("pptp_enable", "nat_pass_rule", "0", "pptp_enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="nat_pass_pptp_enable" id="nat_pass_pptp_enable" 
                            <% nvg_attr_match("pptp_enable", "nat_pass_rule", "0", "pptp_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
                <input type="hidden" value="0" name="nat_pass_l2tp_enable" id="nat_pass_l2tp_enable"/>
                </fieldset><br />

            </div>
        </div><!-- row -->

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
