<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - iSCSI</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var isuser_objects =
[ { name: 'iscsi_isns' },
  { name: 'iscsi_id' },
  { name: 'iscsi_alias' },
  { name: 'iscsi_max_conn' },
  { name: 'iscsi_max_tx' },
  { name: 'iscsi_max_rx' }
];
var F; 
function local_rule_check(T) {
    if ((!$('#username').val())) {
        alert('User name is required.');
        return false;
    }

    if (!$('#password').val()) {
        alert('Password is required.');
        return false;
    }
    
    if ($('#password').val().length < 14) {
            alert('Password length must > 13');
                    return false;
    }
                        
    return true;
}
function local_table_check(T, R) {
    return rlist_check_dup_field(T, R, enum_is._NAME);
}

var enum_is = {                                                                                                          
    _ENABLE:0,
    _NAME:1,
    _PASSWD:2
} 

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = isuser_rule.length ;
    $("<input>").attr({type: "hidden", id: "isuser_rule", name: "isuser_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: isuser_rule[i][enum_is._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: isuser_rule[i][enum_is._NAME] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_password', name: P + i + '_password' , value: isuser_rule[i][enum_is._PASSWD] }).appendTo("form");
    }

}

function local_generate_rule(T) {
    var rule_tmp = Array();
    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }

    rule_tmp[enum_is._ENABLE]   = enable;
    rule_tmp[enum_is._NAME]     = $('#username').val();
    rule_tmp[enum_is._PASSWD]   = $('#password').val();

    return rule_tmp;
}

function to_submit(F) {
    F.submit_button.value = "iscsi";
	F.action.value = "Apply";
    if (local_table_check("isuser_table", isuser_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "isuser_table", "isuser_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

function init() {
    if ($('[name=iscsi_enable]:checked').val()==1){
        for (var i=0; i<isuser_objects.length; i++  ){
            $("input[name="+ isuser_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",false);
            });
        }
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
    }else{
        for (var i=0; i<isuser_objects.length; i++  ){
            $("input[name="+ isuser_objects[i]["name"] +"]").each(function(){
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
    var iSCSI_rows = [];
    var iSCSI_data = {};

    for (var i=0; i<isuser_rule.length; i++ )  {
        iSCSI_data = {};
        iSCSI_data["enable"] = tbl_wrap_enable( isuser_rule[i][enum_is._ENABLE] );
        iSCSI_data["username"] = isuser_rule[i][enum_is._NAME];
        iSCSI_data["passwd"] = isuser_rule[i][enum_is._PASSWD];
        iSCSI_rows.push(iSCSI_data);
    }
    return iSCSI_rows;
}

function CreateiSCSIDataForm(){
    $("#iSCSIDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#iSCSIDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
    $("#iSCSIDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input id='password' name='password' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
}

$(document).ready(function(){
    var iSCSI_rows = GetGridData();

    jQuery('#iSCSIListData').jqGrid({
            data: iSCSI_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Enable");%>", "<%lang("User Name");%>", "<%lang("Password");%>"],
            colModel:[
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'username',index:'username', width:8, align:"center", sortable:false, editable:true},
                      {name:'passwd',index:'passwd', width:8, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#iSCSIListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("User Rule");%>"
    });

    jQuery("#iSCSIListData").jqGrid('navGrid','#iSCSIListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#iSCSIListData").navButtonAdd('#iSCSIListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#iSCSIDataForm").empty();
            CreateiSCSIDataForm();

            $( "#iSCSIListData-dialog" ).dialog({
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
                        isuser_rule.push(Array_Tmp);
                        var iSCSI_rows = GetGridData();
                        $("#iSCSIListData").jqGrid("clearGridData", true);
                        jQuery("#iSCSIListData").setGridParam({ 'data':  iSCSI_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#iSCSIListData").navButtonAdd('#iSCSIListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#iSCSIDataForm").empty();
            var rowid = $("#iSCSIListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                isuser_rule.splice(DelItem,1);
            }
            var iSCSI_rows = GetGridData();
            $("#iSCSIListData").jqGrid("clearGridData", true);
            jQuery("#iSCSIListData").setGridParam({ 'data':  iSCSI_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#iSCSIListData").navButtonAdd('#iSCSIListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#iSCSIDataForm").empty();
            CreateiSCSIDataForm();
            var rowid = $("#iSCSIListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            if (isuser_rule[ModifyIndex][enum_is._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#username").val(isuser_rule[ModifyIndex][enum_is._NAME]);
            $("#password").val(isuser_rule[ModifyIndex][enum_is._PASSWD]);

            $( "#iSCSIListData-dialog" ).dialog({
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
                        isuser_rule.splice(ModifyIndex,1, Array_Tmp);

                        var iSCSI_rows = GetGridData();
                        $("#iSCSIListData").jqGrid("clearGridData", true);
                        jQuery("#iSCSIListData").setGridParam({ 'data':  iSCSI_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

});
</script>
<% tbl_get("isuser_rule"); %>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "iSCSI"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("iSCSI");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('is_rule')</script>	

              <fieldset>
                <legend><%lang("iSCSI");%></legend>
                <div class="setting">
                  <div class="label"><% lang("iSCSI"); %></div>
                  <input type="radio" value="1" name="iscsi_enable" id="iscsi_enable" onclick="init()"
                        <%nvg_attr_match("iscsi_enable","is_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="iscsi_enable" id="iscsi_enable" onclick="init()"
                        <%nvg_attr_match("iscsi_enable","is_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                <div class="label"><%lang("iSNS Server");%></div>
                <input name="iscsi_isns" id="iscsi_isns" size="20" maxLength="80"
                value='<%nvg_attr_get("iscsi_isns", "is_rule", "0", "isns_serv");%>' />
                (<%lang("IP address or domain name");%>)
                </div>
                <div class="setting">
                <div class="label"><%lang("Target ID");%></div>
                <input name="iscsi_id" id="iscsi_id" size="20" maxLength="80"
                value='<%nvg_attr_get("iscsi_id", "is_rule", "0", "isid");%>' />
                </div>

                <div class="setting">
                <div class="label"><%lang("LUN Alias");%></div>
                <input name="iscsi_alias" id="iscsi_alias" size="20" maxLength="20"
                value='<%nvg_attr_get("iscsi_alias", "is_rule", "0", "alias");%>' />
                (20 <%lang("Characters");%>)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Connextion");%></div>
                <input name="iscsi_max_conn" id="iscsi_max_conn" size="10" maxLength="10"
                value='<%nvg_attr_get("iscsi_max_conn", "is_rule", "0", "max_conn");%>' />
                <%lang("Connections");%> (1 ~ 10)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Transmision Data Length");%></div>
                <input name="iscsi_max_tx" id="iscsi_max_tx" size="10" maxLength="30"
                value='<%nvg_attr_get("iscsi_max_tx", "is_rule", "0", "max_tx");%>' />
                <%lang("Bytes");%> (512 ~ 16384)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Receiving Data Length");%></div>
                <input name="iscsi_max_rx" id="iscsi_max_rx" size="10" maxLength="30"
                value='<%nvg_attr_get("iscsi_max_rx", "is_rule", "0", "max_rx");%>' />
                <%lang("Bytes");%> (512 ~ 16384)
                </div>
              </fieldset><br />

              <div id="iSCSIListData-dialog" title="iSCSI Data" style="display: none">
                <table id="iSCSIDataForm"></table>
              </div>
              <table id="iSCSIListData"></table>
              <div id="iSCSIListDataPager"></div>
              <%tbl_create_table("isuser_table", "isuser_rule", "1");%>
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
    <script type="text/javascript">create_input_window("isuser_table", "isuser_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
