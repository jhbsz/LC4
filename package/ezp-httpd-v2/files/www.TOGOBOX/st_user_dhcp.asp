<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Status</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
  		<% st_user_show_dhcp(); %>

        $(document).ready(function(){
            var DHCPData = getDHCPData();
            
            $('#DHCPListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": DHCPData,
                "aoColumns": [
                    { "sTitle": "<% lang("Name"); %>", "sClass":"center" },
                    { "sTitle": "<% lang("IP Address"); %>", "sClass":"center" },
                    { "sTitle": "<% lang("MAC Address"); %>", "sClass":"center" },
                    { "sTitle": "<% lang("Expiration Time"); %>", "sClass":"center" },
                ],
                "bSort": false,
                "bPaginate": false,
                "bLengthChange": false,
                "bFilter": false,
                "bInfo": false,
            } );   
            
            $("#refresh").click(function(){
                window.location.reload();
            });
        });

        function getDHCPData () {
            var DHCPData_Tmp = [];
            for (var i=0; i<DHCPData.length; i++ ){
                var rows_data = {};
                rows_data[0] = DHCPData[i]["Name"];
                rows_data[1] = DHCPData[i]["IP"];
                rows_data[2] = DHCPData[i]["MAC"];
                rows_data[3] = DHCPData[i]["Expiration"];
                DHCPData_Tmp[i] = rows_data;
            } 
            return DHCPData_Tmp;
        }

    </script>

    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('st_user_dhcp');</script>
                <h2><%lang("Status");%>-<%lang("User / DHCP");%></h2>

            <div class="row">
                <div class="span12" style="">
                    <table class="table table-bordered table-striped" id="DHCPListData"></table>
                </div>
            </div><!-- row -->

        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" name="refresh" id="refresh" value="<%lang("Refresh");%>">
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