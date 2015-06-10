<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - MAC Rule</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

function local_rule_check(T) {
    if (!$('#name').val()) {
        alert('Please enter the name.');
        return false;
    }

    if (!$('#hwaddr').val()) {
        alert('Please specify a MAC.');
        return false;
    }

    if ( $('input[name=arp_enable]').is(':checked') || $('input[name=dhcp_enable]').is(':checked') ) {
        if (!$('#ipaddr').val()) {
            alert('Please specify an IP.');
            return false;
        }
    }

    return true;
}


var enum_fl_hwaddr = {
    _NAME:0,
    _HWADDR:1,
    _OP:2,
    _ACL_ENABLE:3,
    _ARP_ENABLE:4,
    _DHCP_ENABLE:5,
    _IPADDR:6
} 


function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = fl_hwaddr_rule.length ;
    $("<input>").attr({type: "hidden", id: "fl_hwaddr_rule", name: "fl_hwaddr_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: fl_hwaddr_rule[i][enum_fl_hwaddr._NAME]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_hwaddr', name: P + i + '_hwaddr' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._HWADDR] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_op', name: P + i + '_op' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._OP] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_acl_enable', name: P + i + '_acl_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._ACL_ENABLE] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_arp_enable', name: P + i + '_arp_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._ARP_ENABLE] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_dhcp_enable', name: P + i + '_dhcp_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._DHCP_ENABLE] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._IPADDR] }).appendTo("form");
    }
}

function local_generate_rule() {
    var acl_enable = 0;
    if ($('input[name=acl_enable]').is(':checked')){
        acl_enable = $('[name=acl_enable]:checked').val();
    }

    var arp_enable = 0;
    if ($('input[name=arp_enable]').is(':checked')){
        arp_enable = $('[name=arp_enable]:checked').val();
    }
    var dhcp_enable = 0;
    if ($('input[name=dhcp_enable]').is(':checked')){
        dhcp_enable = $('[name=dhcp_enable]:checked').val();
    }

    var rule_tmp = Array();
    rule_tmp[enum_fl_hwaddr._NAME]        = $('#name').val();
    rule_tmp[enum_fl_hwaddr._HWADDR]      = $('#hwaddr').val();
    rule_tmp[enum_fl_hwaddr._OP]          = $("select#op option:selected").val();
    rule_tmp[enum_fl_hwaddr._ACL_ENABLE]  = acl_enable;
    rule_tmp[enum_fl_hwaddr._ARP_ENABLE]  = arp_enable;
    rule_tmp[enum_fl_hwaddr._DHCP_ENABLE] = dhcp_enable;
    rule_tmp[enum_fl_hwaddr._IPADDR]      = $('#ipaddr').val();
    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "fl_hwaddr";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "fl_hwaddr_table", "fl_hwaddr_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    if ($('[name=fl_hwaddr_enable]:checked').val()==1){
        $("input[name=fl_hwaddr_rule_default][@type=radio]").attr("disabled",false);
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
        $("#ItemUp").removeClass('ui-state-disabled');
        $("#ItemDown").removeClass('ui-state-disabled');
    }else{
        $("input[name=fl_hwaddr_rule_default][@type=radio]").attr("disabled",true);
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
        $("#ItemUp").addClass('ui-state-disabled');
        $("#ItemDown").addClass('ui-state-disabled');
    }

    <%widget_start();%>
}

function GetGridData(){
    var MAC_rows = [];
    var MAC_data = {};

    for (var i=0; i<fl_hwaddr_rule.length; i++ )  {
        MAC_data = {};
        MAC_data["name"] = fl_hwaddr_rule[i][enum_fl_hwaddr._NAME];
        MAC_data["action"] = tbl_wrap_action( fl_hwaddr_rule[i][enum_fl_hwaddr._OP] );
        MAC_data["hwaddr"] = fl_hwaddr_rule[i][enum_fl_hwaddr._HWADDR];
        MAC_data["ACLenable"] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._ACL_ENABLE] );
        MAC_data["ARPenale"] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._ARP_ENABLE] );
        MAC_data["DHCPenable"] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._DHCP_ENABLE] );
        MAC_data["ipaddr"] = fl_hwaddr_rule[i][enum_fl_hwaddr._IPADDR];
        MAC_rows.push(MAC_data);
    }
    return MAC_rows;
}

function CreateMACDataForm(){

    $("#MACDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' name='name' id='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)'></td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("MAC");%>:</td><td><input id='hwaddr' name='hwaddr' size='18' maxlength='17' onblur='valid_hwaddr(this, \"MAC\")' /></td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("Action");%>:</td><td>"+
                                 "<select id='op' name='op' \>"+
                                 "  <option value='1'><%lang("ALLOW");%></option>"+
                                 "  <option value='0'><%lang("DENY");%></option>"+
                                 "</select>"+
                                 "</td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("ACL Enable");%>:</td><td><input id='acl_enable' name='acl_enable' type='checkbox' size='10' value='1' /></td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("Static ARP Enable");%>:</td><td><input id='arp_enable' name='arp_enable' type='checkbox' size='10' value='1' /></td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("Static DHCP Enable");%>:</td><td><input id='dhcp_enable' name='dhcp_enable' type='checkbox' size='10' value='1' /></td></tr>");
    $("#MACDataForm").append("<tr><td align='right'><%lang("IP");%>:</td><td><input id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");

}


$(document).ready(function(){
    var MAC_rows = GetGridData();

    jQuery('#MACListData').jqGrid({
            data: MAC_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Enable");%>", "<%lang("Action");%>", "<%lang("MAC");%>", "<%lang("ACL Enable");%>", "<%lang("Static ARP Enable");%>", "<%lang("Static DHCP Enable");%>", "<%lang("IP");%>"],
            colModel:[
                      {name:'name',index:'name', width:8, align:"center", sortable:false, editable:true},
                      {name:'action',index:'action', width:5, align:"center", sortable:false, editable:true},
                      {name:'hwaddr',index:'hwaddr', width:6, align:"center", sortable:false, editable:true},
                      {name:'ACLenable',index:'ACLenable', width:5, align:"center", sortable:false, editable:true},
                      {name:'ARPenale',index:'ARPenale', width:5, align:"center", sortable:false, editable:true},
                      {name:'DHCPenable',index:'DHCPenable', width:6, align:"center", sortable:false, editable:true},
                      {name:'ipaddr',index:'ipaddr', width:6, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#MACListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("MAC Access Control Rule");%>"
    });

    jQuery("#MACListData").jqGrid('navGrid','#MACListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#MACListData").navButtonAdd('#MACListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#MACDataForm").empty();
            CreateMACDataForm();

            $( "#MACListData-dialog" ).dialog({
                resizable: false,
                height:350,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        fl_hwaddr_rule.push(Array_Tmp);
                        var MAC_rows = GetGridData();
                        $("#MACListData").jqGrid("clearGridData", true);
                        jQuery("#MACListData").setGridParam({ 'data':  MAC_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#MACListData").navButtonAdd('#MACListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#MACDataForm").empty();
            var rowid = $("#MACListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                fl_hwaddr_rule.splice(DelItem,1);
            }
            var MAC_rows = GetGridData();
            $("#MACListData").jqGrid("clearGridData", true);
            jQuery("#MACListData").setGridParam({ 'data':  MAC_rows }).trigger("reloadGrid");
       }
    });


    jQuery("#MACListData").navButtonAdd('#MACListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#MACDataForm").empty();
            CreateMACDataForm();
            var rowid = $("#MACListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }
            $("#name").val(fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._NAME]);
            $("#hwaddr").val(fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._HWADDR]);
            $("select[name='op']").attr("value", fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._OP]);

            if (fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._ACL_ENABLE] == 1){
                $("#acl_enable").attr('checked',true);
            }else{
                $("#acl_enable").attr('checked',false);
            }
            if (fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._ARP_ENABLE] == 1){
                $("#arp_enable").attr('checked',true);
            }else{
                $("#arp_enable").attr('checked',false);
            }
            if (fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._DHCP_ENABLE] == 1){
                $("#dhcp_enable").attr('checked',true);
            }else{
                $("#dhcp_enable").attr('checked',false);
            }
            $("#ipaddr").val(fl_hwaddr_rule[ModifyIndex][enum_fl_hwaddr._IPADDR]);

            $( "#MACListData-dialog" ).dialog({
                resizable: false,
                height:350,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        fl_hwaddr_rule.splice(ModifyIndex,1, Array_Tmp);

                        var MAC_rows = GetGridData();
                        $("#MACListData").jqGrid("clearGridData", true);
                        jQuery("#MACListData").setGridParam({ 'data':  MAC_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    jQuery("#MACListData").navButtonAdd('#MACListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#MACDataForm").empty();
            var rowid = $("#MACListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = fl_hwaddr_rule[UpItem];
                fl_hwaddr_rule[UpItem] = fl_hwaddr_rule[UpItem - 1];
                fl_hwaddr_rule[UpItem - 1] = arr_tmp;
            }
            var MAC_rows = GetGridData();
            $("#MACListData").jqGrid("clearGridData", true);
            $("#MACListData").setGridParam({ 'data':  MAC_rows }).trigger("reloadGrid");
            $("#MACListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#MACListData").navButtonAdd('#MACListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#MACDataForm").empty();
            var rowid = $("#MACListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == fl_hwaddr_rule.length-1){
                    return false;
                }
                var arr_tmp = fl_hwaddr_rule[DownItem];
                fl_hwaddr_rule[DownItem] = fl_hwaddr_rule[DownItem + 1];
                fl_hwaddr_rule[DownItem + 1] = arr_tmp;
            }
            var MAC_rows = GetGridData();
            $("#MACListData").jqGrid("clearGridData", true);
            $("#MACListData").setGridParam({ 'data':  MAC_rows }).trigger("reloadGrid");
            $("#MACListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End ready

</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
         <% do_menu("Security", "MAC Access Control"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Security");%>', '<%lang("MAC Access Control");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>
              <% tbl_get("fl_hwaddr_rule"); %>
              <fieldset>
                <legend><%lang("MAC Access Control");%></legend>
                <div class="setting">
                  <div class="label"><% lang("MAC Access Control"); %></div>
                  <input type="radio" value="1" name="fl_hwaddr_enable" onclick="init()"
                        <%nv_match("fl_hwaddr_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="fl_hwaddr_enable" onclick="init()"
                        <%nv_match("fl_hwaddr_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label"><% lang("Default MAC Access Control Action"); %></div>
                  <input type="radio" value="1" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                        <%nv_match("fl_hwaddr_rule_default","1","checked");%>><%lang("ALLOW");%>
                  </input>

                  <input type="radio" value="0" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                        <%nv_match("fl_hwaddr_rule_default","0","checked");%>><%lang("DENY");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="MACListData-dialog" title="MAC Data" style="display: none">
                <table id="MACDataForm"></table>
              </div>
              <table id="MACListData"></table>
              <div id="MACListDataPager"></div>
                <%tbl_create_table("fl_hwaddr_table", "fl_hwaddr_rule", "1");%>
              <br />
              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick='window.location.reload()'/>
              </div>
            </form>

          </div>
        <div id="floatKiller"></div>
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
    <script type="text/javascript">create_input_window("fl_hwaddr_table", "fl_hwaddr_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
