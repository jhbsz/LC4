<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - BT</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script language='javascript' type='text/javascript' src='ext/js/ajaxfileupload.js'></script>
    
    <script type="text/javascript">

        var bt_objects =
        [ { name: 'bt_peer_num' },
          { name: 'bt_max_peer_num' },
          { name: 'bt_port' },
          { name: 'bt_dht_port' },
          { name: 'bt_dl_rate' },
          { name: 'bt_ul_rate' },
          { name: 'btn_add' },
          { name: 'btn_delete' },
          { name: 'btn_stop' },
          { name: 'btn_start' },
          { name: 'btn_del_completed' }

        ];


        function percent(val){
            if (val == '') {
                return;
            } else {
                return val + '%';
            }
        }

        function adj_size(val) {
            v = "";
            if (val == '') {
                return v;
            }
            var v = parseInt(val) + ' B';
            if (val > 1024) {
                val = val / 1024;
                v = parseInt(val) + ' KB'
            }
            if (val > 1024) {
                val = val / 1024;
                v = parseInt(val) + ' MB'
            }
            return v;
        }

        function adj_speed(val) {
            v = "";
            if (val == '') {
                return v;
            }
            var v = val + ' B/s';
            if (val > 1024) {
                val = val / 1024;
                v = parseInt(val) + ' KB/s'
            }
            if (val > 1024) {
                val = val / 1024;
                v = parseInt(val) + ' MB/s'
            }
            return v;
        }

        function getBTData() {
            var BTData_Tmp = [];
            $.ajax({
               url:        "bt.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   false,
               data:       { action:"Update"},
               success: function(data){
                    if (data!=""){
                        var data_tmp = eval("("+data+")");
                        
                        var rows = [];
                        var rows_data = {};
                        var rowsArray = data_tmp["msg"].split("|");
                        var rows_dataArray;

                        for (var i=0; i<rowsArray.length; i++ ){
                            rows_dataArray = rowsArray[i].split("^");
                            rows_data = {};
                            if (rows_dataArray[0]!=""){
                                rows_data[0] = rows_dataArray[0];
                                rows_data[1] = '';
                                rows_data[2] = adj_size(rows_dataArray[1]);
                                rows_data[3] = adj_size(rows_dataArray[2]);
                                rows_data[4] = percent(rows_dataArray[3]);
                                rows_data[5] = rows_dataArray[4];
                                rows_data[6] = adj_speed(rows_dataArray[5]);
                                rows_data[7] = adj_speed(rows_dataArray[6]);
                                rows_data[8] = rows_dataArray[7];
                                rows_data[9] = rows_dataArray[8];
                                rows_data[10] = rows_dataArray[9];

                                rows[i] = rows_data;
                            }
                        }
                        BTData_Tmp = rows;
                    }
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });
            return BTData_Tmp;
        }



        var F;
        function init() {
            var enable = $("input[name=bt_enable]:checked").val();
            sel_change(enable, 1, bt_objects);            
        }



        function to_submit(F) {
            sel_change(1, 1, bt_objects);            

            $("#submit_button").val("bt");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        var selectID=null;
        var BTData = [];
        $(document).ready(function () {
            init();

            BTData = getBTData();
            DrawDataTables(BTData);
            $('#btn_add').click(function(){
                $('#remove').trigger('click');
                $('#BTListData-modal').modal({
                   keyboard: false,
                   backdrop: 'static'
                })
            });

            $("#upload").click(function(){
                $('#BTListData-modal').modal('hide');
                open_waiting_window();            
                
                setTimeout(function(){
                    $.ajaxFileUpload({
                        url: 'bt_upload.cgi', 
                        secureuri:false,
                        fileElementId:'BTFile',
                        dataType: 'json',
                        success: function (data, status) {
                            if (data["success"]=="true"){
                                /* upload successfully */
                            } else {
                                alert(data["msg"]);
                            }
                            window.location.reload();
                        }
                    });
                }, 1000);

            });
            

            $("#btn_delete").click(function(){
                if (selectID ==null) {
                    alert("Please, select row");
                    return false;
                }

                var hash_val = BTData[selectID][10];
                setTimeout(function(){
                    $.ajax({
                       url:        "bt.cgi",
                       type:       "POST",
                       dataType:   "json",
                       async:	   true,
                       data:       { action: 'Delete', hash: hash_val },
                       success: function(data){
                           selectID = null;
                           BTData = getBTData();
                           DataTable.fnDestroy();
                           DrawDataTables(BTData);
                       },
                       error: function(xhr, textStatus, error){
                           //console.log(xhr.statusText, textStatus, error);
                       }
                    });
                }, 1000);

            });
            

            $("#btn_stop").click(function(){
                if (selectID ==null) {
                    alert("Please, select row");
                    return false;
                }

                var hash_val = BTData[selectID][10];
                $.ajax({
                   url:        "bt.cgi",
                   type:       "POST",
                   dataType:   "json",
                   async:	   true,
                   data:       { action: 'Stop', hash: hash_val },
                   success: function(data){
                       selectID = null;
                       BTData = getBTData();
                       DataTable.fnDestroy();
                       DrawDataTables(BTData);
                   },
                   error: function(xhr, textStatus, error){
                       //console.log(xhr.statusText, textStatus, error);
                   }
                });

            });

            $("#btn_start").click(function(){
                if (selectID ==null) {
                    alert("Please, select row");
                    return false;
                }

                var hash_val = BTData[selectID][10];
                $.ajax({
                   url:        "bt.cgi",
                   type:       "POST",
                   dataType:   "text",
                   async:	   true,
                   data:       { action: 'Start', hash: hash_val },
                   success: function(data){
                       selectID = null;
                       BTData = getBTData();
                       DataTable.fnDestroy();
                       DrawDataTables(BTData);
                   },
                   error: function(xhr, textStatus, error){
                       //console.log(xhr.statusText, textStatus, error);
                   }
                });

            });

            $("#btn_del_completed").click(function(){
                $.ajax({
                   url:        "bt.cgi",
                   type:       "POST",
                   dataType:   "text",
                   async:	   true,
                   data:       { action: 'delallcpl', hash: "0" },
                   success: function(data){
                       selectID = null;
                       BTData = getBTData();
                       DataTable.fnDestroy();
                       DrawDataTables(BTData);
                   },
                   error: function(xhr, textStatus, error){
                       //console.log(xhr.statusText, textStatus, error);
                   }
                });

            });

            setInterval(function(){
                BTData = getBTData();
                DataTable.fnDestroy();
                DrawDataTables(BTData);
            }, 5000);
            
        }); //end document ready

        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#BTListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "FileName" },
                    { "sTitle": "BTFile", "bVisible":false },
                    { "sTitle": "FileSize" },
                    { "sTitle": "Dowonloaded" },
                    { "sTitle": "Progress" },
                    { "sTitle": "Peers" },
                    { "sTitle": "UploadSpeed" },
                    { "sTitle": "DownloadSpeed" },
                    { "sTitle": "Status" },
                    { "sTitle": "St", "bVisible":false },
                    { "sTitle": "Hash", "bVisible":false }
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

            $("#BTListData tbody tr").click( function() {  // selete a row data
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
 	          <script type="text/javascript">do_hidden_elements('bt_rule');</script>
                <h2><%lang("Storage");%>-<%lang("BT Download");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("BT Download");%></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("BT Download"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="1" name="bt_enable" id="bt_enable" onclick="init()"
                                <%nvg_attr_match("bt_enable","bt_rule","0","enable","1","checked");%>>
                            <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name="bt_enable" id="bt_enable" onclick="init()"
                                <%nvg_attr_match("bt_enable","bt_rule","0","enable","0","checked");%>>
                            <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Maximum Peers Per File");%></div>
                    <div class="span8">
                        <input type="text" name="bt_peer_num" id="bt_peer_num" size="5" maxLength="5" 
                            value='<%nvg_attr_get("bt_peer_num", "bt_rule", "0", "peer");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("BT Engine Maximum Peers");%></div>
                    <div class="span8">
                        <input type="text" name="bt_max_peer_num" id="bt_max_peer_num" size="5" maxLength="5"
                            value='<%nvg_attr_get("bt_max_peer_num", "bt_rule", "0", "max_peer");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("BT Port");%></div>
                    <div class="span8">
                        <input type="text" name="bt_port" id="bt_port" size="5" maxLength="5"
                            value='<%nvg_attr_get("port", "bt_rule", "0", "port_range_start");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DHT Port");%></div>
                    <div class="span8">
                        <input type="text" name="bt_dht_port" id="bt_dht_port" size="5" maxLength="5"
                            value='<%nvg_attr_get("bt_dht_port", "bt_rule", "0", "dht_port");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Maximum Download Rate");%></div>
                    <div class="span8">
                        <input type="text" name="bt_dl_rate" id="bt_dl_rate" size="5" maxLength="5"
                            value='<%nvg_attr_get("bt_dl_rate", "bt_rule", "0", "download_rate");%>' /> KB/s (0: unlimited)
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Maximum Upload Rate");%></div>
                    <div class="span8">
                        <input type="text" name="bt_ul_rate" id="bt_ul_rate" size="5" maxLength="5"
                            value='<%nvg_attr_get("bt_ul_rate", "bt_rule", "0", "upload_rate");%>' /> KB/s (0: unlimited)
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">

                <div id="BTListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-header">
                        <h3>Upload BT File</h3>
                    </div>
                    <div class="modal-body">
                        <form name="BTDataForm" id="BTDataForm">
                            <div class="fileupload fileupload-new" data-provides="fileupload">
                                <div class="input-append">
                                    <div class="uneditable-input span3"><i class="icon-file fileupload-exists"></i><span class="fileupload-preview"></span>
                                </div>
                                <span class="btn btn-file">
                                    <span class="fileupload-new">Select file</span>
                                    <span class="fileupload-exists">Change</span>
                                    <input type="file" name="BTFile" id="BTFile"/>
                                 </span>
                                 <a href="#" class="btn fileupload-exists" data-dismiss="fileupload" name="remove" id="remove">Remove</a>
                              </div>
                            </div>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="upload" id="upload" value="<% lang("Upload"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>

                <table class="table table-bordered table-striped" id="BTListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_stop" name="btn_stop" value="<% lang("Stop"); %>">
                    <input type="button" class="btn" id="btn_start" name="btn_start" value="<% lang("Start"); %>">
                    <input type="button" class="btn" id="btn_del_completed" name="btn_del_completed" value="<% lang("Delet All Completed"); %>">
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
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>