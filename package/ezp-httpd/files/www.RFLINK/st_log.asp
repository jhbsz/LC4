<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Log</title>
<% do_pagehead2(); %>

<script type="text/javascript">
var F;
var syslog_type_objects =
[ { name: 'remote_ip_ipaddr' },
  { name: 'proto' },
  { name: 'remote_port' }
];

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("remote_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable,1,F,syslog_type_objects);
    <%widget_start();%>
}
function to_submit(F) {
    F.submit_button.value = "st_log";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}
 

function getLogData () {
    var LogData_Tmp;
	$.ajax({
	   url:        "log.cgi",
	   type:       "POST",
	   dataType:   "text",
	   async:	   false,
	   //data:       { action:"Widget"},
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

				rows_data['Type'] = rows_dataArray[0];
				rows_data['Content'] = rows_dataArray[1];
				rows[i] = rows_data;
			}
			
			LogData_Tmp = rows;

	   },
	   error: function(xhr, textStatus, error){
		   //console.log(xhr.statusText, textStatus, error);
	   }
	});

	return LogData_Tmp;

}


$(document).ready(function(){

	var LogData = getLogData();
	Lang_Caption = "Sys Log";
	Lang_Type = "Type";
	Lang_Content = "Content";
	jQuery('#LogListData').jqGrid({
			data: LogData,
			datatype: 'local',
			colNames:[ Lang_Type, Lang_Content],
			colModel:[
					{name:'Type',index:'Type', width:60, align:"center"},
					{name:'Content',index:'Content', width:350}
			],
			autowidth: 'auto',
			width: 750,
			height: 'auto',
			rowNum: 20,
			rowList: [20,50,100],
			sortname: 'Content',
			sortorder: 'desc',
			pager: '#LogListDataPager',
			viewrecords: true,
			caption: Lang_Caption
	});

	jQuery("#LogListData").jqGrid('navGrid','#LogListDataPager',
										{edit: false,add: false,del: false,search: true,refresh: false}
	);

	jQuery("#LogListData").navButtonAdd('#LogListDataPager',{
	   caption:"", 
	   buttonicon:"ui-icon-refresh", 
	   onClickButton: function(){
			LogData = getLogData();
			$("#LogListData").jqGrid("clearGridData", true);
			jQuery("#LogListData").setGridParam({ 'data':  LogData }).trigger("reloadGrid");
	   }
	});

});


</script>

<style type="text/css">
#container table {
    border-spacing: 0em;
    margin: 0 10px 0 10px;
    width: 0%;
    }
</style>
</head>
   <body class="gui" onload="init()">
      <div id="wrapper">
            <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
               <% do_menu("Admin", "Log"); %>
        <script type="text/javascript">do_header_post()</script>
            </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Admin");%>', '<%lang("Log");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
               <div id="contentsInfo">
					<table id="LogListData"></table>
					<div id="LogListDataPager"></div>
				  
				  <form name="form">
                      <script type="text/javascript">
                           do_hidden_elements('log_rule')</script>
                  <fieldset>
                  <legend><%lang("Log Server Setting");%></legend>
                  <div class="setting">
                  <div class="label"><% lang("Syslog Server"); %></div>
                    <input type="radio" value="1" name="remote_enable" onclick="init()"
                      <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","1","checked");%>>
                      <%lang("Enable");%>
                    </input>
                    <input type="radio" value="0" name="remote_enable" onclick="init()"
                      <%nvg_attr_match("remote_enable","log_rule","$0","remote_enable","0","checked");%>>
                      <%lang("Disable");%>
                    </input>
                  </div>
                  <div class="setting">
                  <div class="label"><%lang("Remote IP Address");%></div>
                    <input type="text" maxLength="15" size="16" name="remote_ip_ipaddr"
                      value='<%nv_attr_get("log_rule", "$0", "remote_ip");%>' 
                      onblur='valid_ipaddr(this, "Remote IP Address")' />
                    </input>
                  </div>
                  <div class='setting'>
                  <div class='label'> <%lang(Protocol);%> </div>
                    <select id='proto' name='proto'>
                    <option value='tcp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "tcp", "selected"); %>>TCP</option>
                    <option value='udp' <% nvg_attr_match("proto", "log_rule", "0", "proto", "udp", "selected"); %>>UDP</option>
                    <option value='both' <% nvg_attr_match("proto", "log_rule", "0", "proto", "both", "selected"); %>>TCP/UDP</option>"
                    </select>
                  </div>
                  <div class="setting">
                  <div class="label"><% lang("Remote Port"); %>
                  </div><input class="num" maxlength="5" size="5" name="remote_port" onblur="valid_range(this,1,65535,'Port number')" value=
                    "<%nv_attr_get("log_rule", "0", "remote_port");%>">
                  </div>
                  </fieldset><br />
                  <div class="submitFooter">
                    <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                    <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
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
    <script type="text/javascript">create_return_window()</script>
   </body>
</html>
