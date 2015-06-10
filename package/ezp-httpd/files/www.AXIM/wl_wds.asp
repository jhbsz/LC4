<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <% do_pagehead2(); %>
    <script language="JavaScript">
        var useIE6 = false;
    </script>
    <!--[if lte IE 6]>
        <script language="JavaScript">
            var useIE6 = true;
        </script>
    <![endif]-->
    <script language="JavaScript">
    
    var F;

    function getWISPData() {
        var WISPData_Tmp;
            $.ajax({
                url:        "survey.cgi",
                type:       "POST",
                dataType:   "text",
                async:          false,
                data:       { action:"Update", submit_button:"wan"},
                success: function(data){
                    var obj = jQuery.parseJSON(data);           
    		    data = obj.msg
    		    var rows = [];
    		    var rows_data = {};
    		    var rowsArray = data.split("|");
    		    var rows_dataArray;
    		    
    		    for (var i=0; i<rowsArray.length; i++ ){
    		        rows_dataArray = rowsArray[i].split("^");
    		        rows_data = {};
    		        rows_data['ch'] = rows_dataArray[0];
    		        rows_data['ssid'] = rows_dataArray[1];
    		        rows_data['bssid'] = rows_dataArray[2];
    		        rows_data['sec'] = rows_dataArray[3];
    		        rows_data['sig'] = rows_dataArray[4];
    		        rows_data['mode'] = rows_dataArray[5];
    		        
    		        if (rows_dataArray[6] == '1') 
    		            rows_data['extcha'] = 'ABOVE';
    		        else
    		            rows_data['extcha'] = 'BELOW';
    		        
    		        rows_data['extcha'] = 'BELOW';
    	                rows[i] = rows_data;
    		    }
    		    WISPData_Tmp = rows;
    		},
    		error: function(xhr, textStatus, error){
  		}
 	});
 	return WISPData_Tmp;
    }

    var wl_wds_key=new Array(<%nv_get("wl_rule_num");%>);
    function encodekey(backup) {
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            var wds_num;
            if (EN('wl'+i+'_wds_mode')[0].value == "disabled") return true;
            if (i=="0") wds_num = "<%nv_get("wl0_wds_rule_num");%>";
            if (i=="1") wds_num = "<%nv_get("wl1_wds_rule_num");%>";
            if (backup) wl_wds_key[i]=new Array(wds_num);
            for (var j = 0; j < wds_num; j++) {
                var secmode = EN('wl'+i+'_wds'+j+'_secmode')[0].value;
                if (secmode == 'wep') {
                    if (EN('wl'+i+'_wds'+j+'_key')[0].value == "") return 0;
                    if (!valid_wl_sec_wep_key(EN('wl'+i+'_wds'+j+'_key')[0],'WDS Key')) return -1;
                    if (backup) {
                        wl_wds_key[i][j]=EN('wl'+i+'_wds'+j+'_key')[0].value;
                        EN('wl'+i+'_wds'+j+'_key')[0].value=encode64(EN('wl'+i+'_wds'+j+'_key')[0].value);
                    } else
                        EN('wl'+i+'_wds'+j+'_key')[0].value=wl_wds_key[i][j];
                }
                if (secmode == 'psk') {
                    if (EN('wl'+i+'_wds'+j+'_key')[0].value == "") return 0;
                    if (!valid_wl_sec_wpa_psk_key(EN('wl'+i+'_wds'+j+'_key')[0],'WDS Key')) return -1;
                    if (backup) {
                        wl_wds_key[i][j]=EN('wl'+i+'_wds'+j+'_key')[0].value;
                        EN('wl'+i+'_wds'+j+'_key')[0].value=encode64(EN('wl'+i+'_wds'+j+'_key')[0].value);
                    }else
                        EN('wl'+i+'_wds'+j+'_key')[0].value=wl_wds_key[i][j];
                }
            }
        }
        return true;
    }

    function to_submit(F) {
        var val=encodekey(1);
        if (val=='0') {
            alert('The Key must have value.');
            return;
        } else if (val=='-1') return;
        F.action.value = "Apply";
        F.submit_button.value = "wl_wds";
        ajax_preaction(F);
        encodekey(0);
        display_mask(F);
    }

    function SelWDS(F) {
        F.submit_button.value = "wl_wds";
        F.action.value = "Gozila";
        F.submit();
    }

    function init() {
	 //initialise all environments
	  <%widget_start();%>
    }
    
    $(document).ready(function(){
    	var WISPData = [];
    	
    	jQuery('#surveyData').jqGrid({
    	    data: WISPData,
    	    datatype: 'local',
    	    colNames:[ '<%lang("Channel");%>', '<%lang("SSID");%>', '<%lang("BSSID");%>', '<%lang("Security Mode");%>', '<%lang("Signal");%>','<%lang("ExtCH");%>','<%lang("Wireless Mode");%>'],
    	    colModel:[
    	    	{name:'ch',index:'ch', width:50, align:"center"},
    	    	{name:'ssid',index:'ssid', width:50, align:"center"},
    	    	{name:'bssid',index:'bssid', width:150, align:"center"},
    	    	{name:'sec',index:'sec', width:50, align:"center"},
    	    	{name:'sig',index:'', width:50, align:"center"},
    	    	{name:'extcha',index:'extcha', width:60, align:"center"},
    	    	{name:'mode',index:'mode', width:80, align:"center"}
    	    ],
    	    autowidth: 'auto',
    	    width: 750,
    	    height: 'auto',
    	    rowNum: 50,
    	    rowList: [20,50,100],
    	    sortname: 'Content',
    	    sortorder: 'desc',
    	    pager: '#LogListDataPager',
    	    viewrecords: true,
    	    caption: '<%lang("Site Survey");%>',
    	    onSelectRow: function(ids){
    	        $("#mac_choose").val(ids);
    	    }
    	});	                        
    	
    	$("input[name='survey']").click(function() {
    	    var obj_text = $(this);
    	    $("#surveyData").jqGrid("clearGridData", true);
    	    $("#MSG").append("Searching...");
    	    setTimeout(function(){WISPData = getWISPData(); 
    	        jQuery("#surveyData").setGridParam({ 'data':  WISPData }).trigger("reloadGrid");
    	        $("#MSG").empty();
    	    }, 500);
    	
    	    $("#surveyData-dialog").dialog({
    	    	resizable: false,
    	    	height:500,
    	    	width:'auto',
    	    	modal: true,    
    	    	buttons: {
    	            "<%lang("Choose");%>": function(ids){
    	        	var rowData = jQuery('#surveyData').getRowData($("#mac_choose").val());
    	    		obj_text.prev("input").val(rowData['bssid']);
    	    		$( this ).dialog( "close" );
    	            }, 	
    	            "<%lang("Cancel");%>": function() {
    	            	$( this ).dialog( "close" );
    	            }
    	     	}
    	    });
            setTimeout(function(){WISPData = getWISPData();                                                
                jQuery("#surveyData").setGridParam({ 'data':  WISPData }).trigger("reloadGrid");
                $("#MSG").empty();                                                             
            }, 5000);
        });
    });
			
    </script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "WDS"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("WDS");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" >
	        <script type="text/javascript">do_hidden_elements('wl_wds_rule')</script>	
            <% wl_show_setting("wl_wds_setting.asp"); %>
            <div class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='wl_wds.asp'" />
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
    <div id="surveyData-dialog" title="WISP" style="display: none">
            <table id="surveyData"></table>
            <div id="MSG"></div>
    </div>
    <input type="hidden" id="mac_choose" value=""/>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window("wl_wds.asp")</script>
  </body>
</html>
