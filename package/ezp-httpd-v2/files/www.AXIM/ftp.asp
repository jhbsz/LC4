<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - FTP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>
    
    <% tbl_get("ftpd_user_rule"); %>
    <script type="text/javascript">
        var ftpd_objects =
        [ { name: 'ftpd_port' },
          { name: 'ftpd_bandwidth' },
          { name: 'ftpd_allow_ext_access' },
          { name: 'btn_add' },
          { name: 'btn_delete' },
          { name: 'btn_modify' }
        ];
        var F; 
        function local_rule_check(T) {
            if (!$('#username').val()) {
                alert('User name is required.');
                return false;
            }

            if (!$('#passwd').val()) {
                alert('Password is required.');
                return false;
            }

            return true;
        }
        function local_table_check(T, R) {
            return rlist_check_dup_field(T, R, enum_ftp._NAME);
        }

        var enum_ftp = {                                                                                                          
            _ENABLE:0,
            _NAME:1,
            _PASSWD:2,
            _UPLOAD:3,
            _DOWNLOAD:4,
            _IPADDR:5
        } 


        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = ftpd_user_rule.length ;
            $("<input>").attr({type: "hidden", id: "ftpd_rule", name: "ftpd_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: ftpd_user_rule[i][enum_ftp._ENABLE]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: ftpd_user_rule[i][enum_ftp._NAME] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_passwd', name: P + i + '_passwd' , value: ftpd_user_rule[i][enum_ftp._PASSWD] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_upload', name: P + i + '_upload' , value: ftpd_user_rule[i][enum_ftp._UPLOAD] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_download', name: P + i + '_download' , value: ftpd_user_rule[i][enum_ftp._DOWNLOAD] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: ftpd_user_rule[i][enum_ftp._IPADDR] }).appendTo("form");
            }
        }

        function local_generate_rule(T) {

            var rule_tmp = Array();
            var enable = 0;
            if ($('input[name=enable]').is(':checked')){
                enable = $('[name=enable]:checked').val();
            }

            rule_tmp[enum_ftp._ENABLE]   = enable;
            rule_tmp[enum_ftp._NAME]     = $('#username').val();
            rule_tmp[enum_ftp._PASSWD]   = $('#passwd').val();
            rule_tmp[enum_ftp._UPLOAD]   = $('#upload').val();
            rule_tmp[enum_ftp._DOWNLOAD] = $('#download').val();
            rule_tmp[enum_ftp._IPADDR]   = '';

            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("ftp");
            $("#action").val("Apply");
            if (local_table_check("ftpd_table", ftpd_user_rule) == false) {
                return;
            }
            local_pack_key_value_pair(F, "ftpd_table", "ftpd_user_rule");

            sel_change(1, 1, ftpd_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }


        function init() {
            enable = $('[name=ftpd_enable]:checked').val();
            sel_change(enable, 1, ftpd_objects);
        }

        function GetGridData(){
            var FTP_rows = [];
            var FTP_data = [];

            for (var i=0; i<ftpd_user_rule.length; i++ )  {
                FTP_data = [];
                FTP_data[0] = tbl_wrap_enable( ftpd_user_rule[i][enum_ftp._ENABLE] );
                FTP_data[1] = ftpd_user_rule[i][enum_ftp._NAME];
                FTP_data[2] = ftpd_user_rule[i][enum_ftp._PASSWD];
                FTP_data[3] = ftpd_user_rule[i][enum_ftp._UPLOAD];
                FTP_data[4] = ftpd_user_rule[i][enum_ftp._DOWNLOAD];
                FTP_rows[i] = FTP_data;
            }
            return FTP_rows;
        }

        function CreateFTPDataForm(){
            $("#FTPDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
            $("#FTPDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input type='text' id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
            $("#FTPDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input type='text' id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
            $("#FTPDataForm").append("<tr><td align='right'><%lang("Per session upload rate");%>:</td><td><input type='text' id='upload' name='upload' size='20' maxlength='20' onblur='valid_number(this, \"Upload\", 1)'/></td></tr>");
            $("#FTPDataForm").append("<tr><td align='right'><%lang("Per session download rate");%>:</td><td><input type='text' id='download' name='download' size='20' maxlength='20' onblur='valid_number(this, \"Download\", 1)'/></td></tr>");
        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var FTP_rows = GetGridData();
            DrawDataTables(FTP_rows);

             $('#btn_add').click(function(){
                $("#FTPDataForm").empty();
                CreateFTPDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#FTPListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                ftpd_user_rule.push(Array_Tmp);
                var FTP_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(FTP_rows);
                $('#FTPListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#FTPDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    ftpd_user_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });

             $("#btn_modify").click(function(){
                 $("#FTPDataForm").empty();
                 CreateFTPDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 if (ftpd_user_rule[selectID][enum_ftp._ENABLE] == 1){
                     $("#enable").attr('checked',true);
                 }else{
                     $("#enable").attr('checked',false);
                 }
                 $("#username").val(ftpd_user_rule[selectID][enum_ftp._NAME]);
                 $("#passwd").val(ftpd_user_rule[selectID][enum_ftp._PASSWD]);
                 $("#upload").val(ftpd_user_rule[selectID][enum_ftp._UPLOAD]);
                 $("#download").val(ftpd_user_rule[selectID][enum_ftp._DOWNLOAD]);

                 $('#FTPListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 ftpd_user_rule.splice(selectID,1, Array_Tmp);

                var FTP_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(FTP_rows);
                 $('#FTPListData-modal').modal('hide');
             });

        });//End Ready

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }

        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#FTPListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center", "sWidth": "200px" },
                    { "sTitle": "<%lang("User Name");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Password");%>", "sClass":"center", "sWidth": "200px" },
                    { "sTitle": "<%lang("Per Session Upload");%> (KB/s)", "sClass":"center" },
                    { "sTitle": "<%lang("Per Session Download");%> (KB/s)", "sClass":"center" }
                ],
                "bSort": false,
                "bPaginate": false,
                "bLengthChange": false,
                "bFilter": false,
                "bInfo": false,
                "fnRowCallback": function( nRow, aData, iDisplayIndex ) {
                   if (iDisplayIndex == selectID){
                       $(nRow).addClass("row_selected");
                       return nRow;
                   }
                },
            } );   

            $("#FTPListData tbody tr").click( function() {  // selete a row data
                if ( $(this).hasClass('row_selected') ) {
                    $(this).removeClass('row_selected');
                    selectID=null;
                }else {
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                    $(this).addClass('row_selected');
                    selectID = $(this).index();
                }
            });

        }

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('ftpd_rule');</script>
                <h2><%lang("Storage");%>-<%lang("FTP Server");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("FTP Server");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("FTP Server"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="ftpd_enable" id="ftpd_enable" onclick="init()"
                                <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="ftpd_enable" id="ftpd_enable" onclick="init()"
                                <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("FTP Server Port");%></div>
                    <div class="span8">
                        <input type="text" name="ftpd_port" id="ftpd_port" size="5" maxLength="5"
                            value='<%nvg_attr_get("ftpd_port", "ftpd_rule",   "0", "port");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("FTP Bandwidth");%></div>
                    <div class="span8">
                        <input type="text" name="ftpd_bandwidth" id="ftpd_bandwidth" size="5" maxLength="5"
                        value='<%nvg_attr_get("ftpd_bandwidth", "ftpd_rule",   "0", "userbandwidth");%>' /> KB/s
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("FTP Server External Access"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="ftpd_allow_ext_access" id="ftpd_allow_ext_access"
                            <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="ftpd_allow_ext_access" id="ftpd_allow_ext_access"
                            <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="FTPListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="FTPDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="FTPListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                </div>

              <%tbl_create_table("ftpd_table", "ftpd_user_rule", "1");%>
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