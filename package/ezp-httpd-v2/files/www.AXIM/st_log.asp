<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Log</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
        var syslog_type_objects =
        [ { name: 'remote_ip_ipaddr' },
          { name: 'proto' },
          { name: 'remote_port' }
        ];
         
        function init() {
            enable_snmp = $('input[name=remote_enable]:checked').val();
            sel_change(enable_snmp, 1, syslog_type_objects);
        }

        $(document).ready(function(){
            init();
            var LogData = getLogData();

            $('#LogListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LogData,
                "aoColumns": [
                    { "sTitle": "Type" },
                    { "sTitle": "Content" },
                ],
                "bSort": false,
            } );   

        });



        function to_submit(F) {
            $("#submit_button").val("st_log");
            $("#action").val("Apply");

            open_waiting_window();
            sel_change(1, 1, syslog_type_objects);
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

        }

        function getLogData () {
            var LogData_Tmp;
            $.ajax({
               url:        "log.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   false,
               data:       { action:"Widget"},
               success: function(data){
                    data = data.replace(/</g, "< ");
                    data = data.replace(/>/g, " >");
                    var rows = [];
                    var rows_data = {};
                    var rowsArray = data.split("\n");
                    var rows_dataArray;

                    for (var i=0; i<rowsArray.length; i++ ){
                        rows_dataArray = rowsArray[i].split("\t");
                        rows_data = {};
                        if (rows_dataArray[0]!=""){
                            rows_data[0] = rows_dataArray[0];
                            if (rows_dataArray[1]){
                                rows_data[1] = rows_dataArray[1];
                            }else{
                                rows_data[1] = "";
                            }
                            rows[i] = rows_data;
                        }
                    }
                    LogData_Tmp = rows;
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });
            return LogData_Tmp;
        }
    </script>
    </head>
    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form">
 	          <script type="text/javascript">do_hidden_elements('log_rule')</script>
                <h2><%lang("Admin");%>-<%lang("Log");%></h2>

        <div class="row">
            <div class="span12" style="">
                <table class="table table-bordered table-striped" id="LogListData"></table>
            </div>
        </div><!-- row -->
        
        <br>
        <div class="row">
            <div class="span12">
                <legend><%lang("Log Server Setting");%></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("Syslog Server"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="remote_enable" onclick="init()"
                              <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                        <input type="radio" value="0" name="remote_enable" onclick="init()"
                          <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Remote IP Address");%></div>
                    <div class="span8">
                       <input type="text" maxLength="15" size="16" name="remote_ip_ipaddr" id="remote_ip_ipaddr" value='<%nv_attr_get("log_rule", "$0", "remote_ip");%>' 
                        onblur='valid_ipaddr(this, "Remote IP Address")' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang(Protocol);%></div>
                    <div class="span8">
                        <select id='proto' name='proto'>
                            <option value='tcp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "tcp", "selected"); %>>TCP</option>
                            <option value='udp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "udp", "selected"); %>>UDP</option>
                            <option value='both' <% nvg_attr_match("proto", "log_rule", "0", "proto", "both", "selected"); %>>TCP/UDP</option>"
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Remote Port"); %></div>
                    <div class="span8">
                        <input type="text" class="num" maxlength="5" size="5" name="remote_port" id="remote_port" onblur="valid_range(this,1,65535,'Port number')" 
                               value="<%nv_attr_get("log_rule", "0", "remote_port");%>">
                    </div>
                </div>
    
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
                    <script type="text/javascript"><%show_copyright();%></script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>