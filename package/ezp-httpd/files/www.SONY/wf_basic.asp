<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Web Filtering</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

function local_rule_check() {

    if ((!$('#keyword').val())) {
        alert('Keyword is required for filtering.');
        return false;
    }
    
    return true;
}

 
var enum_wf_basic = {
    _ENABLE:0,
    _KEYWORD:1,
    _TYPE:2,
    _ACCESS:3
}; 

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = wf_rule.length ;
    $("<input>").attr({type: "hidden", id: "wf_rule", name: "wf_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: wf_rule[i][enum_wf_basic._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_keyword', name: P + i + '_keyword' , value: wf_rule[i][enum_wf_basic._KEYWORD] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_type', name: P + i + '_type' , value: wf_rule[i][enum_wf_basic._TYPE] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_access', name: P + i + '_access' , value: wf_rule[i][enum_wf_basic._ACCESS] }).appendTo("form");
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }
    rule_tmp[enum_wf_basic._ENABLE]   = enable;
    rule_tmp[enum_wf_basic._KEYWORD]  = $('#keyword').val();
    rule_tmp[enum_wf_basic._TYPE]     = $('#type').val();
    rule_tmp[enum_wf_basic._ACCESS]   = $('#access').val();
    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "wf_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "wf_table", "wf_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    if ($('[name=wf_enable]:checked').val()==1){
        $("input[name=activex_enable][@type=radio]").attr("disabled",false);
        $("input[name=java_enable][@type=radio]").attr("disabled",false);
        $("input[name=proxy_enable][@type=radio]").attr("disabled",false);
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
        $("#ItemUp").removeClass('ui-state-disabled');
        $("#ItemDown").removeClass('ui-state-disabled');
    }else{
        $("input[name=activex_enable][@type=radio]").attr("disabled",true);
        $("input[name=java_enable][@type=radio]").attr("disabled",true);
        $("input[name=proxy_enable][@type=radio]").attr("disabled",true);
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
        $("#ItemUp").addClass('ui-state-disabled');
        $("#ItemDown").addClass('ui-state-disabled');
    }

    <%widget_start();%>
}

function GetGridData(){
    var Page_rows = [];
    var Page_data = {};

    for (var i=0; i<wf_rule.length; i++ )  {
        Page_data = {};
        Page_data["enable"] = tbl_wrap_enable( wf_rule[i][enum_wf_basic._ENABLE] );
        Page_data["keyword"] = wf_rule[i][enum_wf_basic._KEYWORD];
        Page_data["type"] = wf_rule[i][enum_wf_basic._TYPE];
        Page_data["access"] = tbl_wrap_action( wf_rule[i][enum_wf_basic._ACCESS] );
        Page_rows.push(Page_data);
    }
    return Page_rows;
}

function CreatePageDataForm(){
    $("#PageDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#PageDataForm").append("<tr><td align='right'><%lang("Filter Keyword");%>:</td><td><input id='keyword' name='keyword' size='17' maxlength='15' /></td></tr>");
    $("#PageDataForm").append("<tr><td align='right'><%lang("Filter Type");%>:</td><td>"+
                                 "<select id='type' name='type' \>"+
                                 "  <option value='url' selected><%lang("url");%></option>"+
                                 "  <option value='host'><%lang("host");%></option>"+
                                 "</select>"+
                                 "</td></tr>");
    $("#PageDataForm").append("<tr><td align='right'><%lang("Action");%>:</td><td>"+
                                 "<select id='access' name='access' \>"+
                                 "  <option value='1'><%lang("ALLOW");%></option>"+
                                 "  <option value='0'><%lang("DENY");%></option>"+
                                 "</select>"+
                                 "</td></tr>");

}

$(document).ready(function(){
    var Page_rows = GetGridData();

    jQuery('#PageListData').jqGrid({
            data: Page_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Enable");%>", "<%lang("Filter Keyword");%>", "<%lang("Filter Type");%>", "<%lang("Action");%>"],
            colModel:[
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'keyword',index:'keyword', width:8, align:"center", sortable:false, editable:true},
                      {name:'type',index:'type', width:5, align:"center", sortable:false, editable:true},
                      {name:'access',index:'access', width:5, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#PageListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Web Filtering Rule");%>"
    });

    jQuery("#PageListData").jqGrid('navGrid','#PageListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#PageListData").navButtonAdd('#PageListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#PageDataForm").empty();
            CreatePageDataForm();

            $( "#PageListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        wf_rule.push(Array_Tmp);
                        var Page_rows = GetGridData();
                        $("#PageListData").jqGrid("clearGridData", true);
                        jQuery("#PageListData").setGridParam({ 'data':  Page_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#PageListData").navButtonAdd('#PageListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#PageDataForm").empty();
            var rowid = $("#PageListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                wf_rule.splice(DelItem,1);
            }
            var Page_rows = GetGridData();
            $("#PageListData").jqGrid("clearGridData", true);
            jQuery("#PageListData").setGridParam({ 'data':  Page_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#PageListData").navButtonAdd('#PageListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#PageDataForm").empty();
            CreatePageDataForm();
            var rowid = $("#PageListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            if (wf_rule[ModifyIndex][enum_wf_basic._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#keyword").val(wf_rule[ModifyIndex][enum_wf_basic._KEYWORD]);
            $("select[name='type']").attr("value", wf_rule[ModifyIndex][enum_wf_basic._TYPE]);
            $("select[name='access']").attr("value", wf_rule[ModifyIndex][enum_wf_basic._ACCESS]);

            $( "#PageListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        wf_rule.splice(ModifyIndex,1, Array_Tmp);

                        var Page_rows = GetGridData();
                        $("#PageListData").jqGrid("clearGridData", true);
                        jQuery("#PageListData").setGridParam({ 'data':  Page_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    jQuery("#PageListData").navButtonAdd('#PageListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#PageDataForm").empty();
            var rowid = $("#PageListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = wf_rule[UpItem];
                wf_rule[UpItem] = wf_rule[UpItem - 1];
                wf_rule[UpItem - 1] = arr_tmp;
            }
            var Page_rows = GetGridData();
            $("#PageListData").jqGrid("clearGridData", true);
            $("#PageListData").setGridParam({ 'data':  Page_rows }).trigger("reloadGrid");
            $("#PageListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#PageListData").navButtonAdd('#PageListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#PageDataForm").empty();
            var rowid = $("#PageListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == wf_rule.length-1){
                    return false;
                }
                var arr_tmp = wf_rule[DownItem];
                wf_rule[DownItem] = wf_rule[DownItem + 1];
                wf_rule[DownItem + 1] = arr_tmp;
            }
            var Page_rows = GetGridData();
            $("#PageListData").jqGrid("clearGridData", true);
            $("#PageListData").setGridParam({ 'data':  Page_rows }).trigger("reloadGrid");
            $("#PageListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End ready

</script>
<% tbl_get("wf_rule"); %>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Security", "Web Filtering"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("Web Filtering");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>	
              <fieldset>
                <legend><%lang("Web Filtering");%></legend>
                <div class="setting">
                  <!-- Enable/disable web filtering -->
                  <div class="label"><% lang("Web Filtering"); %></div>
                  <input type="radio" value="1" name="wf_enable" onclick="init()"
                      <%nvg_match("wf_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="wf_enable" onclick="init()"
                      <%nvg_match("wf_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br />
                </div>
              </fieldset><br />

              <fieldset>
                <legend><%lang("Web Content Filtering");%></legend>
                <div class="setting">
                  <!-- Enable/disable activex filtering -->
                  <div class="label"><% lang("Activex Filtering"); %></div>
                  <input type="radio" value="1" name="activex_enable" onclick="init()"
                      <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                       "activex_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="activex_enable" onclick="init()"
                      <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                       "activex_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                  
                  <!-- Enable/disable java filtering -->
                  <div class="label"><% lang("Java/JavaScript Filtering"); %></div>
                  <input type="radio" value="1" name="java_enable" onclick="init()"
                      <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                       "java_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="java_enable" onclick="init()"
                      <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                       "java_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                  
                  <!-- Enable/disable proxy filtering -->
                  <div class="label"><% lang("Proxy Filtering"); %></div>
                  <input type="radio" value="1" name="proxy_enable" onclick="init()"
                      <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                       "proxy_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="proxy_enable" onclick="init()"
                      <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                       "proxy_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                </div>
              </fieldset><br />
              <div id="PageListData-dialog" title="Page Data" style="display: none">
                <table id="PageDataForm"></table>
              </div>
              <table id="PageListData"></table>
              <div id="PageListDataPager"></div>
                <%tbl_create_table("wf_table", "wf_rule", "1");%>
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
    <script type="text/javascript">create_input_window("wf_table", "wf_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
