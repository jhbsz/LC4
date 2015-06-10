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
        var clone_wan_objects = new Array(<%nv_get("wan_num");%>);
        var clone_lan_objects = new Array(<%nv_get("lan_num");%>);
        var F;

        function to_submit(F) {
            $("#submit_button").val("clone");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

            /*
            F.submit_button.value = "clone";
            F.action.value = "Apply";
            ajax_preaction(F);
            display_mask(F);
            */
        }
        
        function to_get_mac(F, name) {
            $("#submit_button").val("clone");
            $("#action").val(name);
            ajaxData = ajax_data( $("#form").serializeArray() );
            $.ajax({
               url:        "get_mac.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   false,
               data: ajaxData,
               success: function(data){
                   ret = data.split('\n');
                   for (var i=0; i<ret.length; i++ ) {
                      var data_tmp = ret[i].split("="); 
                      if (data_tmp[0]=="message") {
                          MAC = data_tmp[1];
                      } else if (data_tmp[0]=="name") {
                          FieldName = data_tmp[1];
                      }
                   }
                   $("#"+FieldName).val(MAC);
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });
        }
        
        function init() {
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                var enable = $("input[name=wan"+i+"_hwaddr_clone_enable]:checked").val();
                sel_change(enable, 1, clone_wan_objects[i]);
            }
            for (var i = 0; i < <%nv_get("lan_num");%>; i++) {
                var enable = $("input[name=lan"+i+"_hwaddr_clone_enable]:checked").val();
                sel_change(enable, 1, clone_lan_objects[i]);
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
 	          <script type="text/javascript">do_hidden_elements('hwaddr_clone_rule');</script>
                <h2><%lang("Setup");%>-<%lang("MAC Address Clone");%></h2>

        <%clone_show_setting();%>
<!--
        <div class="row">
            <div class="span12">
                <legend><%lang("SNMP");%></legend>
                <div class="row show-grid">
                    <div class="span4">SNMP</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="snmp_enable" onclick="onoffsnmp()"
                            <%nvg_attr_match("snmp_enable", "snmp_rule", "0", "enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="snmp_enable" onclick="onoffsnmp()"
                            <%nvg_attr_match("snmp_enable", "snmp_rule", "0", "enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><% lang("SNMP Community"); %></div>
                    <div class="span8">
                        <input type="text" placeholder="" maxlength="16" size="25" name="snmp_community" id="snmp_community" 
                               onblur="valid_reserved_word(this,'snmp community')" value="<%nvg_attr_get("snmp_community", "snmp_rule", "0", "community");%>">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("SNMP Port"); %></div>
                    <div class="span8">
                        <input type="text" placeholder="" maxlength="5" size="5" name="snmp_tcpport" id="snmp_tcpport"  
                            onblur="valid_range(this,1,65535,'Port number')" value="<%nvg_attr_get("snmp_tcpport", "snmp_rule", "0", "tcpport");%>">
                    </div>
                </div>
            </div>
        </div><!-- row -->
<!--
        <br>
        <div class="row">
            <div class="span12">
                <legend><% lang("System Identification"); %></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("System Name"); %></div>
                    <div class="span8">
                        <input type="text" placeholder="" name="snmp_sysname" id='snmp_sysname' size="25" maxLength="25" onBlur="valid_name(this,'SYSTEM NAME')"
                        value='<%nvg_attr_get("snmp_sysname", "snmp_rule", "0", "sysname");%>'>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("System Location"); %></div>
                    <div class="span8">
                        <input type="text" placeholder="" name="snmp_location" id='snmp_location' size="25" maxLength="25" onBlur="valid_name(this,'SYSTEM LOCATION')"
                        value='<%nvg_attr_get("snmp_location", "snmp_rule", "0", "location");%>'>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("System Contact"); %></div>
                    <div class="span8">
                        <input type="text" name="snmp_contact" id="snmp_contact" size="25" maxLength="32" onBlur="valid_name(this,'SYSTEM CONTACT')" value="">
                    </div>
                </div>

                <br />
                <input type="hidden" name="snmp_udpport" id="snmp_udpport" value="<%nv_attr_get("snmp_rule", "0", "udpport");%>"></input>
                <input type="hidden" name="snmp_vendor" id="snmp_vendor" value="<%nv_attr_get("snmp_rule", "0", "vendor");%>"></input>
                <input type="hidden" name="snmp_timeout" id="snmp_timeout" value="<%nv_attr_get("snmp_rule", "0", "timeout");%>"></input>
                <input type="hidden" name="snmp_auth" id="snmp_auth" value="<%nv_attr_get("snmp_rule", "0", "auth");%>"></input>
        
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