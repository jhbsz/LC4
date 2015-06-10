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
        function to_adtool(F, cmd) {
            if ($("#"+cmd+'host').val() == "") {
                alert('<% lang("Needs a host name or an IP address"); %>');
                return false;
            }
            if ((isNaN($("#"+cmd+'cnt').val())) || (cmd != "traceroute" && (($("#"+cmd+'cnt').val() < 1) || ($("#"+cmd+'cnt').val() > 10)))) {
                alert('<% lang("Please use 1 ~ 10 in Number of Packets"); %>');
                return false;
            }
            if ((isNaN($("#"+cmd+'cnt').val())) || (cmd == "traceroute" && (($("#"+cmd+'cnt').val() < 1) || ($("#"+cmd+'cnt').val() > 15)))) {
                alert('<% lang("Please use 1 ~ 15 in Hop Count "); %>');
                return false;
            }

            $("#submit_button").val("ad_tools");
            $("#action").val(cmd);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("ad_tools.cgi", "POST", "text", true, ajaxData);

            return true;
        }
        
        function to_submit(F) {
            $("#submit_button").val("ad_tools");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);
       }
        
    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('adtool_rule');</script>
                <h2><%lang("Admin");%>-<%lang("System Utilities");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("System Utilities");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Interface"); %></div>
                    <div class="span8">
                        <select name="pingiface" id="pingiface">
                          <option value="any" 
                            <%nvg_attr_match("pingiface","adtool_rule", 0, "pingiface", "any", "selected");%>> 
                          <% lang("*"); %> </option>
                          <% ad_tools_showif("wan","ping"); %>
                          <% ad_tools_showif("lan","ping"); %>
                        </select>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><% lang("Target Host"); %></div>
                    <div class="span8">
                        <input type="text" name="pinghost" id="pinghost" maxlength="45" size="16" value="<% nvg_attr_get("pinghost", "adtool_rule", "0", "pinghost"); %>">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Number of Packets"); %></div>
                    <div class="span8">
                        <input type="text" name="pingcnt" id="pingcnt" maxlength="15" size="16" value="<% nvg_attr_get("pingcnt", "adtool_rule", "0", "pingcnt"); %>">
                        <% lang("Packets"); %> <% lang("(1 ~ 10)"); %>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Ping"); %></div>
                    <div class="span8">
                        <input type="button" name="pingtest" id="pingtest" value='<%lang("Ping");%>' onclick="to_adtool(this.form, 'ping')">
                    </div>
                </div>


            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12">
                <legend><% lang("ARPing"); %> (<% lang("Within the same broadcasting domain"); %>)</legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("Interface"); %></div>
                    <div class="span8">
                        <select name="arpingiface" id="arpingiface">
                          <% ad_tools_showif("wan","arping"); %>
                          <% ad_tools_showif("lan","arping"); %>
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Target Host"); %></div>
                    <div class="span8">
                        <input type="text" name="arpinghost" id="arpinghost" maxlength="45" size="16" value="<% nvg_attr_get("arpinghost", "adtool_rule", "0", "arpinghost"); %>">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Number of Packets"); %></div>
                    <div class="span8">
                        <input type="text" name="arpingcnt" id="arpingcnt" maxlength="15" size="16" value="<% nvg_attr_get("arpingcnt", "adtool_rule", "0", "arpingcnt"); %>">
                         <%lang("Packets"); %> <% lang("(1 ~ 10)"); %> 
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("ARPing"); %></div>
                    <div class="span8">
                        <input type="button" name="arpingtest" id="arpingtest" value='<%lang("ARPing");%>' onclick= "to_adtool(this.form, 'arping')">
                    </div>
                </div>

            </div>
        </div><!-- row -->

        <br>
        <div class="row">
            <div class="span12">
                <legend><% lang("Trace Route"); %></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("Interface"); %></div>
                    <div class="span8">
                        <select name="tracerouteiface" id="tracerouteiface">
                          <option value="any" 
                            <%nvg_attr_match("tracerouteiface","adtool_rule", 0, "tracerouteiface", "any", "selected");%>> 
                          <% lang("*"); %> </option>
                          <% ad_tools_showif("wan","traceroute"); %>
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Target Host"); %></div>
                    <div class="span8">
                    <input type="text" name="traceroutehost" id="traceroutehost" maxlength="45" size="16" value="<% nvg_attr_get("traceroutehost", "adtool_rule", "0", "traceroutehost"); %>">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Hop Count"); %></div>
                    <div class="span8">
                        <input type="text" name="traceroutecnt" id="traceroutecnt" maxlength="15" size="16" value="<% nvg_attr_get("traceroutecnt", "adtool_rule", "0", "traceroutecnt"); %>">
                        <%lang("Counts"); %> <% lang("(1 ~ 15)"); %>  
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Trace route"); %></div>
                    <div class="span8">
                        <input type="button" name="traceroutetest" id="traceroutetest" value='<%lang("Trace Route");%>' onclick= "to_adtool(this.form, 'traceroute')"><br>
                        <textarea class="input-xxlarge" rows='10'><% st_show_traceroute();%></textarea>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>

        <!--
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