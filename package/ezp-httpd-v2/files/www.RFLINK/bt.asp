<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - BT</title>
<% do_pagehead2("no_table"); %>


<script type="text/javascript">
var bt_objects =
[ { name: 'bt_peer_num' },
  { name: 'bt_max_peer_num' },
  { name: 'bt_port' },
  { name: 'bt_dht_port' },
  { name: 'bt_dl_rate' },
  { name: 'bt_ul_rate' }
];



    function percent(val){
        if (val == '') {
            return;
        } else {
            return val + '%';
        }
    }

    function adj_size(val) {
        if (val == '') {
            return;
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
        if (val == '') {
            return;
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


    function bt_update_status() {
	var jqxhr = $.post("bt.cgi", 
			   {action: 'Update'} , 
			   function(data, textStatus,jqXHR) {},"json");
	jqxhr.success(function(response,textStatus,XMLHttpRequest) {
			var BTData = new Array();
			var Rows = new Array();
			var Fields = new Array();
			var dataObj=eval("("+XMLHttpRequest.responseText+")");
			    if (dataObj.msg == '') {
				jQuery("#BTListData").clearGridData(true);
			        setTimeout('bt_update_status();', 15000);
				return;
			    }

			dumpData = dataObj.msg;
			jQuery("#BTListData").clearGridData(true);
			Rows = dumpData.split("|");
			for (var i=0; i<Rows.length; i++ ){
			    Fields = [];
			    Fields = Rows[i].split("^");
		 	    BTData[i] = {FileName:Fields[0],BTFile:'',FileSize:adj_size(Fields[1]),Dowonloaded:adj_size(Fields[2]),Progress:percent(Fields[3]),Peers:Fields[4],UploadSpeed:adj_speed(Fields[5]),DownloadSpeed:adj_speed(Fields[6]),Status:Fields[7],St:Fields[8],Hash:Fields[9]};
			}

			for(var i=0;i<=BTData.length;i++)
			    jQuery("#BTListData").jqGrid('addRowData',i+1,BTData[i]);
			setTimeout('bt_update_status();', 15000);
       });

	jqxhr.error(function(XMLHttpRequest, textStatus, errorThrown) { 
		/* TODO */
	});

}


var F;
function init() {
    F = document.getElementsByName('form')[0];
    var enable = (document.getElementsByName("bt_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, bt_objects);

    setTimeout('bt_update_status();', 3000);

	
    var dumpData = "";
    <%bt_show_content();%>

    var BTData = new Array();
    var Rows = new Array();
    var Fields = new Array();
    if (dumpData !=""){
	Rows = dumpData.split("|");
	    for (var i=0; i<Rows.length; i++ ){
		Fields = [];
		Fields = Rows[i].split("^");
		BTData[i] = {FileName:Fields[0],BTFile:'',FileSize:adj_size(Fields[1]),Dowonloaded:adj_size(Fields[2]),Progress:percent(Fields[3]),Peers:Fields[4],UploadSpeed:adj_speed(Fields[5]),DownloadSpeed:adj_speed(Fields[6]),Status:Fields[7],St:Fields[8],Hash:Fields[9]};
	    }

    }

    jQuery("#BTListData").jqGrid({
	data: BTData,
	datatype: "local",
	colNames:['File Name','BT File','File Size', 'Downloaded','Progress','Peers','Upload Speed','Download Speed','Status','St','Hash'],
	colModel:[
		  {name:'FileName',index:'FileName', width:100},
		  {name:'BTFile',index:'BTFile', width:100, edittype: "file", editable:true, hidden:true, editrules:{edithidden:true}},
		  {name:'FileSize',index:'FileSize', width:60, align:"center"},
		  {name:'Dowonloaded',index:'Downloaded', width:60, align:"center"},
		  {name:'Progress',index:'Progress', width:50, align:"center"},
		  {name:'Peers',index:'Peers', width:50, align:"center"},
		  {name:'UploadSpeed',index:'UploadSpeed', width:60, align:"center"},
		  {name:'DownloadSpeed',index:'DownloadSpeed', width:60, align:"center"},
		  {name:'Status',index:'Status', width:60, align:"left"},
		  {name:'St',index:'St', width:60, align:"left", hidden:true},
		  {name:'Hash',index:'Hash', width:60, align:"left", hidden:true}
		 ],
	autowidth: "auto",
	width: 730,
	height: "auto",
	//rowNum: 10,
	//rowList: [10,20,30],
	sortname: 'FileName',
	sortorder: "asc",
	pager: "#BTListDataPager",
	viewrecords: true,
	pgbuttons: false,
	pgtext: null,
	editurl: '',
	caption: "BT"
    });					

    jQuery("#BTListData").jqGrid('navGrid','#BTListDataPager',
				 {edit: false,
				  add: false,addtext:"Add",addtitle:"Add",
				  del: false,deltext:"Del",deltitle:"Del",
				  search: false,refresh: false},
				 {closeAfterEdit: false,closeAfterAdd: false},
				 {closeAfterEdit: true,closeAfterAdd: true}
		          );


    jQuery("#BTListData").navButtonAdd('#BTListDataPager',{
		   caption:"Add", 
		   buttonicon:"ui-icon-plusthick", 
		   onClickButton: function(){
				      jQuery("#BTListData").jqGrid("editGridRow", "new", 
								   {closeAfterEdit: true,closeAfterAdd: true, addCaption: "Upload BT File",
								    onInitializeForm : function(formid){
								    			   $("#BTListData").setGridParam({editurl:'bt_upload.cgi'});
										       },
								    onclickSubmit: function(postdata, formid){
										       var BT_Dialog = $.Zebra_Dialog('<strong>Uploading BT File</strong>', 
															{'type':     'information',
															 'title':    'information',
															 'buttons':  false,
															 'overlay_close': false
															});

											$.ajaxFileUpload({url: 'bt_upload.cgi', 
													  secureuri:false,
													  fileElementId:'BTFile',
													  dataType: 'json',
													  success: function (data, status) {
														       BT_Dialog.close();
														   }
													 });
										   }
								  }
			             );
		   }
		});


    jQuery("#BTListData").navButtonAdd('#BTListDataPager',{
		caption:"Del", 
		buttonicon:"ui-icon-closethick", 
		onClickButton: function(){
				var RowID = jQuery("#BTListData").getGridParam('selrow');
				var hash_val = $('#BTListData').getCell(RowID, 'Hash');
				var jqxhr = $.post("bt.cgi", 
						   {action: 'Delete', hash: hash_val} , 
						   function(data, textStatus,jqXHR) {},"json");
				jqxhr.success(function(response,textStatus,XMLHttpRequest) {
						  show_mask();
						  var msg = "Delete: " + '<%lang("succeeded");%>';
						  display_return(-3, msg);
					      });
				jqxhr.error(function(XMLHttpRequest, textStatus, errorThrown) { 
						  show_mask();
						  var msg = "Delete: " + '<%lang("failed");%>';
						  display_return(-3, msg);
					      });

		   }
		});

		jQuery("#BTListData").navButtonAdd('#BTListDataPager',{
		   caption:"Stop", 
		   buttonicon:"ui-icon-pause", 
		   onClickButton: function(){
				var RowID = jQuery("#BTListData").getGridParam('selrow');
				var hash_val = $('#BTListData').getCell(RowID, 'Hash');

				var jqxhr = $.post("bt.cgi", 
						   {action: 'Stop', hash: hash_val} , 
					 	    function(data, textStatus,jqXHR) {},"json");
				jqxhr.success(function(response,textStatus,XMLHttpRequest) {
						   show_mask();
						   var msg = "Stop: " + '<%lang("succeeded");%>';
						   display_return(-3, msg);
					      });
				jqxhr.error(function(XMLHttpRequest, textStatus, errorThrown) { 
						   show_mask();
						   var msg = "Stop: " + '<%lang("failed");%>';
						   display_return(-3, msg);
					    });

		   }
		});


		jQuery("#BTListData").navButtonAdd('#BTListDataPager',{
		   caption:"Start", 
		   buttonicon:"ui-icon-play", 
		   onClickButton: function(){ 
				var RowID = jQuery("#BTListData").getGridParam('selrow');
				var hash_val = $('#BTListData').getCell(RowID, 'Hash');

				var jqxhr = $.post("bt.cgi", 
						   {action: 'Start', hash: hash_val} , 
						   function(data, textStatus,jqXHR) {},"json");
				jqxhr.success(function(response,textStatus,XMLHttpRequest) {
						   show_mask();
						   var msg = "Start: " + '<%lang("succeeded");%>';
						   display_return(-3, msg);
					      });
				jqxhr.error(function(XMLHttpRequest, textStatus, errorThrown) { 
					 	   show_mask();
						   var msg = "Start: " + '<%lang("failed");%>';
						   display_return(-3, msg);
					   });
		   }
		});


		jQuery("#BTListData").navButtonAdd('#BTListDataPager',{
		   caption:"Delet All Completed", 
		   buttonicon:"ui-icon-closethick", 
		   onClickButton: function(){
				var jqxhr = $.post("bt.cgi", 
						   {action: 'delallcpl', hash: '0'} , 
						   function(data, textStatus,jqXHR) {},"json");
				jqxhr.success(function(response,textStatus,XMLHttpRequest) {
						   show_mask();
						   var msg = "Delete all completed torrent: " + '<%lang("succeeded");%>';
						   display_return(-3, msg);
					     });
				jqxhr.error(function(XMLHttpRequest, textStatus, errorThrown) { 
						alert(XMLHttpRequest.responseText);
						show_mask();
						var msg = "Delete all completed torrent: " + '<%lang("failed");%>';
						display_return(-3, msg);
					   });
		   }
		});


	
	<%widget_start();%>

}



function to_submit(F) {
    F.submit_button.value = "bt";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 35000);
    display_mask(F);
}

</script>

</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
            <% do_menu("Storage", "BT Download"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("BT Download");%>')</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('bt_rule')</script>
              <fieldset>
                <legend><%lang("BT Download");%></legend>
                <div class="setting">
                  <div class="label"><% lang("BT Download"); %></div>
                  <input type="radio" value="1" name="bt_enable" onclick="init()"
                        <%nvg_attr_match("bt_enable","bt_rule","0","enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="bt_enable" onclick="init()"
                        <%nvg_attr_match("bt_enable","bt_rule","0","enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Peers Per File");%></div>
                  <input name="bt_peer_num" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_peer_num", "bt_rule", "0", "peer");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("BT Engine Maximum Peers");%></div>
                  <input name="bt_max_peer_num" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_max_peer_num", "bt_rule", "0", "max_peer");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("BT Port");%></div>
                  <input name="bt_port" size="5" maxLength="5"
                      value='<%nvg_attr_get("port", "bt_rule", "0", "port_range_start");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("DHT Port");%></div>
                  <input name="bt_dht_port" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_dht_port", "bt_rule", "0", "dht_port");%>' />
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Download Rate");%></div>
                  <input name="bt_dl_rate" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_dl_rate", "bt_rule", "0", "download_rate");%>' /> KB/s (0: unlimited)
                </div>

                <div class="setting">
                  <div class="label"><%lang("Maximum Upload Rate");%></div>
                  <input name="bt_ul_rate" size="5" maxLength="5"
                      value='<%nvg_attr_get("bt_ul_rate", "bt_rule", "0", "upload_rate");%>' /> KB/s (0: unlimited)
                </div>

              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
              </div>

            </form>

            <br/><br/>


   			  <table id="BTListData"></table>
			  <div id="BTListDataPager"></div>
			  <br>

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
