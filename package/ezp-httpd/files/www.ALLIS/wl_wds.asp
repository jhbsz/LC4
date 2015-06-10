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
    <script type="text/javascript" src="ext/js/ext-base.js"></script>         
    <script type="text/javascript" src="ext/js/ext-all.js"></script>
    <link rel="stylesheet" type="text/css" href="ext/css/ext-all.css"/>
    <style type="text/css">
        table {border-spacing: 0em;}
    </style>
    <script language="JavaScript">
    
    var F;
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
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("WDS");%>')</script>
        <div id="contentsInfo">
          <form name="form" id="form" action="apply.cgi" method="post" >
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
    <div id="survey_window"></div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window("wl_wds.asp")</script>
    <script type="text/javascript">
    Ext.grid.dummyData = [];
    Ext.BLANK_IMAGE_URL = "icons/s.gif";

    var op_window;
    var textname='';	
    
    function renderExtCh(value) {
    	if(value == '1') 
    	    return 'ABOVE';
    	else
    	    return 'BELOW';
    }
    
    var cm = new Ext.grid.ColumnModel([ 
    	    {header:'<%lang("Channel");%>',  dataIndex:'ch', width:50},
    	    {header:'<%lang("SSID");%>',  dataIndex:'ssid'},
    	    {header:'<%lang("BSSID");%>',  dataIndex:'bssid', width:150},
    	    {header:'<%lang("Security Mode");%>',  dataIndex:'sec'},
    	    {header:'<%lang("Signal");%>',  dataIndex:'sig', width:50},
    	    {header:'<%lang("ExtCH");%>',  dataIndex:'extcha', width:60,renderer:renderExtCh},
    	    {header:'<%lang("Wireless Mode");%>',  dataIndex:'mode', width:80}
    ]);
    	
    var store = new Ext.data.Store({                                      
    	reader: new Ext.data.ArrayReader({}, [                        
    	    	{name: 'ch'},                                                                                            
    	    	{name: 'ssid'},                                               
    	    	{name: 'bssid'},                                              
    	    	{name: 'sec'},                                                
    	    	{name: 'sig'},                                                
    	    	{name: 'mode'},                                               
    	    	{name: 'extcha'}                                                          
    	]),                                                           
	data: Ext.grid.dummyData                                          
    });
    
    var contain = new Ext.grid.GridPanel({                                
    	store:store,                                                      
    	cm:cm                                                             
    });
    
    function to_sruvey(F,obj){
        textname=obj;
        if(!op_window){
            op_window = new Ext.Window({                                  
            	title: '<%lang("Site Survey");%>',                        
	    	applyTo:'survey_window',                                  
            	layout:'fit',                                             
            	width:610,
            	height:300,
            	closeAction:'hide',                                       
            	plain: true,                                              
            	modal: true,                               
            	items: [contain],
            	buttons: [{
                    text:'<%lang("Select");%>',
                    handler: function(){get_station_info(F)}
                },{
                    text:'<%lang("Refresh");%>',
                    handler: function(){survey_status(F)}
                }]
            });
        }
    	survey_status(F);                                                              
    	op_window.show(this);                              
    }
    
    function survey_status(F) {
    	contain.store.removeAll(); 
    	F.action.value='Update';
    	F.submit_button.value = "wan";
        var postform = new Ext.form.BasicForm('form');
    	postform.submit({
    	    url: 'survey.cgi',
    	    success: function(formPost, o) {
    	        if (o.result == undefined) return;
    	        var msg = o.result.msg;
    	        if (msg == '') {
    	            Ext.grid.dummyData = [];
    	            return;
    	        }
    	        
    	        var seeds = msg.split('|');
    	        for (var i = 0; seeds[i] != undefined; i++) {
    	            if (Ext.grid.dummyData[i] == undefined) {
    	                Ext.grid.dummyData[i] = new Array();
    	            }
    	            
    	            /* check the noise and filter it out. */
    	            var check = seeds[i].search('^');
    	            if (check == -1) return;
    	        
    	            var items = seeds[i].split('^');
    	            /* name size complete progress dl_rate ul_rate hash */
    	            for (var j = 0; items[j] != undefined; j++)
    	            	Ext.grid.dummyData[i][j] = items[j];
    	         }
    	         contain.store.loadData(Ext.grid.dummyData);
    	    },
   	    failure: function(formpost, o) { 	    
    		/* todo */
           }
       });
    }
    
    function get_station_info(F) {
        var tar = contain.getSelectionModel().getSelections();
        if (tar == '') { 
            alert(lang("please select a wireless station"));
            return;
        }
        $("input[name='"+ textname +"']").val(tar[0].get("bssid"));
    	op_window.hide();                              
    } 
</script>
  </body>
</html>
