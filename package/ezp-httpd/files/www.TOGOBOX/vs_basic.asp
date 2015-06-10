<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Virtual Hosts</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

function local_rule_check(T) {

    if ((!$('#name').val())) {
        alert('Rule Name is required.');
        return false;
    }

    if (!$('#wan_ipaddr').val()) {
        alert('Please specify a External IP Address.');
        return false;
    }

    if (!$('#mapped_ipaddr').val()) {
        alert('Please specify a Mapped LAN IP Address.');
        return false;
    }

    return true;
}

 
var enum_vs_basic = {
    _NAME:0,
    _ENABLE:1,
    _EXT_IF:2,
    _IF_INDEX:3,
    _WAN_ADDR:4,
    _MAPPED_ADDR:5
} 


function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = vs_rule.length ;
    $("<input>").attr({type: "hidden", id: "vs_rule", name: "vs_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name' , value: vs_rule[i][enum_vs_basic._NAME] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: vs_rule[i][enum_vs_basic._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif' , value: vs_rule[i][enum_vs_basic._EXT_IF] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_wan_ipaddr', name: P + i + '_wan_ipaddr' , value: vs_rule[i][enum_vs_basic._WAN_ADDR] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_mapped_ipaddr', name: P + i + '_mapped_ipaddr' , value: vs_rule[i][enum_vs_basic._MAPPED_ADDR] }).appendTo("form");
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();
    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }

    rule_tmp[enum_vs_basic._NAME]   = $('#name').val();
    rule_tmp[enum_vs_basic._ENABLE] = enable;
    rule_tmp[enum_vs_basic._EXT_IF] = E('extif').value;
    rule_tmp[enum_vs_basic._WAN_ADDR] = $('#wan_ipaddr').val();
    rule_tmp[enum_vs_basic._MAPPED_ADDR] = $('#mapped_ipaddr').val();

    return rule_tmp;

}

function to_submit(F) {
	F.submit_button.value = "vs_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "vs_table", "vs_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    if ($('[name=vs_enable]:checked').val()==1){
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
    }else{
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
    }
    <%widget_start();%>
}

function GetGridData(){
    var VS_rows = [];
    var VS_data = {};

    for (var i=0; i<vs_rule.length; i++ )  {
        VS_data = {};
        VS_data["name"] = vs_rule[i][enum_vs_basic._NAME];
        VS_data["enable"] = tbl_wrap_enable( vs_rule[i][enum_vs_basic._ENABLE] );
        VS_data["extif"] = tbl_wrap_wan(vs_rule[i][enum_vs_basic._EXT_IF]);
        VS_data["wan_ipaddr"] = "<%lang("From");%>:" + vs_rule[i][enum_vs_basic._WAN_ADDR];
        VS_data["mapped_ipaddr"] = "<%lang("To");%>:" + vs_rule[i][enum_vs_basic._MAPPED_ADDR];
        VS_rows.push(VS_data);
    }
    return VS_rows;
}

function CreateVSDataForm(){
    $("#VSDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></td></tr>");
    $("#VSDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#VSDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
                                 "<select id='extif' name='extif' >"+
                                 "</select>"+
                                 "<td></td></tr>");
    $("#VSDataForm").append("<tr><td align='right'><%lang("External IP Address");%>:</td><td><input id='wan_ipaddr' name='wan_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");
    $("#VSDataForm").append("<tr><td align='right'><%lang("Mapped LAN IP Address");%>:</td><td><input id='mapped_ipaddr' name='mapped_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");

    for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
        $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
    }

}

$(document).ready(function(){
    var VS_rows = GetGridData();

    jQuery('#VSListData').jqGrid({
            data: VS_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>","<%lang("Rule Enable");%>","<%lang("External Interface");%>","<%lang("External IP Address");%>","<%lang("Mapped LAN IP Address");%>"],
            colModel:[
                      {name:'name',index:'name', width:8, align:"center", sortable:false, editable:true},
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'extif',index:'extif', width:5, align:"center", sortable:false, editable:true},
                      {name:'wan_ipaddr',index:'wan_ipaddr', width:8, align:"left", sortable:false, editable:true},
                      {name:'mapped_ipaddr',index:'mapped_ipaddr', width:8, align:"left", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#VSListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Virtual Host Rule");%>"
    });

    jQuery("#VSListData").jqGrid('navGrid','#VSListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#VSListData").navButtonAdd('#VSListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#VSDataForm").empty();
            CreateVSDataForm();

            $( "#VSListData-dialog" ).dialog({
                resizable: false,
                height:300,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        vs_rule.push(Array_Tmp);
                        var VS_rows = GetGridData();
                        $("#VSListData").jqGrid("clearGridData", true);
                        jQuery("#VSListData").setGridParam({ 'data':  VS_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#VSListData").navButtonAdd('#VSListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#VSDataForm").empty();
            var rowid = $("#VSListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                vs_rule.splice(DelItem,1);
            }
            var VS_rows = GetGridData();
            $("#VSListData").jqGrid("clearGridData", true);
            jQuery("#VSListData").setGridParam({ 'data':  VS_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#VSListData").navButtonAdd('#VSListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#VSDataForm").empty();
            CreateVSDataForm();
            var rowid = $("#VSListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#name").val(vs_rule[ModifyIndex][enum_vs_basic._NAME]);
            if (vs_rule[ModifyIndex][enum_vs_basic._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#extif").val(vs_rule[ModifyIndex][enum_vs_basic._EXT_IF]);
            $("#wan_ipaddr").val(vs_rule[ModifyIndex][enum_vs_basic._WAN_ADDR]);
            $("#mapped_ipaddr").val(vs_rule[ModifyIndex][enum_vs_basic._MAPPED_ADDR]);

            $( "#VSListData-dialog" ).dialog({
                resizable: false,
                height:300,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        vs_rule.splice(ModifyIndex,1, Array_Tmp);

                        var VS_rows = GetGridData();
                        $("#VSListData").jqGrid("clearGridData", true);
                        jQuery("#VSListData").setGridParam({ 'data':  VS_rows }).trigger("reloadGrid");
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
<% tbl_get("vs_rule"); %>
</head>
<body class="gui" onload="init();">
  <div id="wrapper">
      <div id="header">
      <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Applications", "Virtual Hosts"); %>
      <script type="text/javascript">do_header_post()</script>
      </div>

    <script type="text/javascript">do_body_pre()</script>	
    <script type="text/javascript">do_page_path('<%lang("Applications");%>', '<%lang("Virtual Hosts");%>')</script>
    <script type="text/javascript">do_table_pre()</script>	
    <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post">
            <script type="text/javascript">do_hidden_elements()</script>	
            <input type="hidden" name="fr_dmz_num" value=<%nv_get("wan_num");%>/>
            <fieldset>
              <legend><%lang("Virtual Hosts");%></legend>
              <div class="setting">
                <div class="label"><% lang("Virtual Hosts"); %></div>
                <input type="radio" value="1" name="vs_enable" onclick="init()"
                      <%nv_match("vs_enable","1","checked");%>><%lang("Enable");%>
                </input>

                <input type="radio" value="0" name="vs_enable" onclick="init()"
                      <%nv_match("vs_enable","0","checked");%>><%lang("Disable");%>
                </input>
              </div>
            </fieldset><br />
              <div id="VSListData-dialog" title="VS Data" style="display: none">
                <table id="VSDataForm"></table>
              </div>
              <table id="VSListData"></table>
              <div id="VSListDataPager"></div>
              <%tbl_create_table("vs_table", "vs_rule", "1");%>
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
  <script type="text/javascript">create_input_window("vs_table", "vs_rule", 1)</script>
  <script type="text/javascript">create_return_window()</script>
</body>
</html>
