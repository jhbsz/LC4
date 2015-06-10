<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - FTP</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var ftpd_objects =
[ { name: 'ftpd_port' },
  { name: 'ftpd_bandwidth' },
  { name: 'ftpd_allow_ext_access' }
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
	F.submit_button.value = "ftp";
	F.action.value = "Apply";
    if (local_table_check("ftpd_table", ftpd_user_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "ftpd_table", "ftpd_user_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {

    if ($('[name=ftpd_enable]:checked').val()==1){
        for (var i=0; i<ftpd_objects.length; i++  ){
            $("input[name="+ ftpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",false);
            });
        }
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
    }else{
        for (var i=0; i<ftpd_objects.length; i++  ){
            $("input[name="+ ftpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",true);
            });
        }
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
    }

    <%widget_start();%>
}

function GetGridData(){
    var FTP_rows = [];
    var FTP_data = {};

    for (var i=0; i<ftpd_user_rule.length; i++ )  {
        FTP_data = {};
        FTP_data["enable"] = tbl_wrap_enable( ftpd_user_rule[i][enum_ftp._ENABLE] );
        FTP_data["username"] = ftpd_user_rule[i][enum_ftp._NAME];
        FTP_data["passwd"] = ftpd_user_rule[i][enum_ftp._PASSWD];
        FTP_data["upload"] = ftpd_user_rule[i][enum_ftp._UPLOAD];
        FTP_data["download"] = ftpd_user_rule[i][enum_ftp._DOWNLOAD];
        FTP_rows.push(FTP_data);
    }
    return FTP_rows;
}

function CreateFTPDataForm(){
    $("#FTPDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#FTPDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
    $("#FTPDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
    $("#FTPDataForm").append("<tr><td align='right'><%lang("Per session upload rate");%>:</td><td><input id='upload' name='upload' size='20' maxlength='20' onblur='valid_number(this, \"Upload\", 1)'/></td></tr>");
    $("#FTPDataForm").append("<tr><td align='right'><%lang("Per session download rate");%>:</td><td><input id='download' name='download' size='20' maxlength='20' onblur='valid_number(this, \"Download\", 1)'/></td></tr>");
}

$(document).ready(function(){
    var FTP_rows = GetGridData();

    jQuery('#FTPListData').jqGrid({
            data: FTP_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Enable");%>", "<%lang("User Name");%>", "<%lang("Password");%>", "<%lang("Per Session Upload");%> (KB/s)", "<%lang("Per Session Download");%> (KB/s)"],
            colModel:[
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'username',index:'username', width:8, align:"center", sortable:false, editable:true},
                      {name:'passwd',index:'passwd', width:8, align:"center", sortable:false, editable:true},
                      {name:'upload',index:'upload', width:8, align:"center", sortable:false, editable:true},
                      {name:'download',index:'download', width:8, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#FTPListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("User Rule");%>"
    });

    jQuery("#FTPListData").jqGrid('navGrid','#FTPListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#FTPListData").navButtonAdd('#FTPListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#FTPDataForm").empty();
            CreateFTPDataForm();

            $( "#FTPListData-dialog" ).dialog({
                resizable: false,
                height:300,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        ftpd_user_rule.push(Array_Tmp);
                        var FTP_rows = GetGridData();
                        $("#FTPListData").jqGrid("clearGridData", true);
                        jQuery("#FTPListData").setGridParam({ 'data':  FTP_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#FTPListData").navButtonAdd('#FTPListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#FTPDataForm").empty();
            var rowid = $("#FTPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                ftpd_user_rule.splice(DelItem,1);
            }
            var FTP_rows = GetGridData();
            $("#FTPListData").jqGrid("clearGridData", true);
            jQuery("#FTPListData").setGridParam({ 'data':  FTP_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#FTPListData").navButtonAdd('#FTPListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#FTPDataForm").empty();
            CreateFTPDataForm();
            var rowid = $("#FTPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            if (ftpd_user_rule[ModifyIndex][enum_ftp._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#username").val(ftpd_user_rule[ModifyIndex][enum_ftp._NAME]);
            $("#passwd").val(ftpd_user_rule[ModifyIndex][enum_ftp._PASSWD]);
            $("#upload").val(ftpd_user_rule[ModifyIndex][enum_ftp._UPLOAD]);
            $("#download").val(ftpd_user_rule[ModifyIndex][enum_ftp._DOWNLOAD]);

            $( "#FTPListData-dialog" ).dialog({
                resizable: false,
                height:300,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        ftpd_user_rule.splice(ModifyIndex,1, Array_Tmp);

                        var FTP_rows = GetGridData();
                        $("#FTPListData").jqGrid("clearGridData", true);
                        jQuery("#FTPListData").setGridParam({ 'data':  FTP_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

});//End Ready
</script>
<% tbl_get("ftpd_user_rule"); %>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "FTP Server"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("FTP Server");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('ftpd_rule')</script>	
              <fieldset>
                <legend><%lang("FTP Server");%></legend>
                <div class="setting">
                  <div class="label"><% lang("FTP Server"); %></div>
                  <input type="radio" value="1" name="ftpd_enable" onclick="init()"
                        <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="ftpd_enable" onclick="init()"
                        <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                <div class="label"><%lang("FTP Server Port");%></div>
                <input name="ftpd_port" size="5" maxLength="5"
                value='<%nvg_attr_get("ftpd_port", "ftpd_rule",   "0", "port");%>' />
                </div>
                <div class="setting">
                <div class="label"><%lang("FTP Bandwidth");%></div>
                <input name="ftpd_bandwidth" size="5" maxLength="5"
                value='<%nvg_attr_get("ftpd_bandwidth", "ftpd_rule",   "0", "userbandwidth");%>' /> KB/s
                </div>

                <div class="setting">
                <div class="label"><% lang("FTP Server External Access"); %></div>
                <input type="radio" value="1" name="ftpd_allow_ext_access"
                <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","1","checked");%>><%lang("Enable");%>
                </input>

                <input type="radio" value="0" name="ftpd_allow_ext_access"
                <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","0","checked");%>><%lang("Disable");%>
                </input>
                </div>

              </fieldset><br />
              <div id="FTPListData-dialog" title="FTP Data" style="display: none">
                <table id="FTPDataForm"></table>
              </div>
              <table id="FTPListData"></table>
              <div id="FTPListDataPager"></div>
              <%tbl_create_table("ftpd_table", "ftpd_user_rule", "1");%>
              <br />

              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.reload()" />
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <%widget_show();%>
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_input_window("ftpd_table", "ftpd_user_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
