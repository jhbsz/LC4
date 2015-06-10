
<style type="text/css">
    table {
        border-spacing: 0em;
    }
</style>
<script language="JavaScript">
    wl_apcli_objects[$0] =
    [ { name: 'wl$0_apcli$1_ssid' },
      { name: 'wl$0_apcli$1_bssid' },
      { name: 'wl$0_apcli$1_secmode' },
      { name: 'wl$0_apcli$1_key_index' },
      { name: 'wl$0_apcli$1_key1' },
      { name: 'wl$0_apcli$1_key2' },
      { name: 'wl$0_apcli$1_key3' },
      { name: 'wl$0_apcli$1_key4' },
      { name: 'wl$0_apcli$1_key4' },
      { name: 'wl$0_apcli$1_key' },
      { name: 'wl$0_apcli$1_crypto' },
      { name: 'wl$0_survey' },
      { name: 'wl$0_channel' },
      { name: 'wl$0_extcha' }
    ];
    

function getWISPData() {
    var WISPData_Tmp;
	$.ajax({
	   url:        "survey.cgi",
	   type:       "POST",
	   dataType:   "text",
	   async:	   false,
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
                if (rows_dataArray[6] == '1') {
                    rows_data['extcha'] = 'ABOVE';
                } else {
                    rows_data['extcha'] = 'BELOW';
                }
				rows[i] = rows_data;
			}
			
			WISPData_Tmp = rows;

	   },
	   error: function(xhr, textStatus, error){
		   //console.log(xhr.statusText, textStatus, error);
	   }
	});

	return WISPData_Tmp;

}

function getWISPCrypto(sec) {
    var crypto = sec.split("/");
    if (crypto[1] == 'TKIP') {
        F.wl$0_apcli$1_crypto.value = 'tkip';
    } else if (crypto[1] == 'AES') {
        F.wl$0_apcli$1_crypto.value = 'aes';
    } else {
        F.wl$0_apcli$1_crypto.value = 'mixed';
    }
}

$(document).ready(function(){

	var WISPData = [];

    jQuery('#wl$0_surveyData').jqGrid({
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
			var rowData = jQuery(this).getRowData(ids);
			F.wl$0_apcli$1_ssid.value = rowData['ssid'];
			F.wl$0_apcli$1_bssid.value = rowData['bssid'];
			F.wl$0_channel.value = rowData['ch'];
            if (rowData['extcha'] == "ABOVE") {
                F.wl$0_extcha.value = 1;
            } else {
                F.wl$0_extcha.value = 0;
            }
			var sec = rowData['sec'];
						
			if(sec == 'NONE') {
			    F.wl$0_apcli$1_secmode.value = 'disabled';
			} else if(sec == 'WEP') {
			    F.wl$0_apcli$1_secmode.value = 'wep';
			} else if(sec.match('WPA2')) {
                getWISPCrypto(sec);
			    F.wl$0_apcli$1_secmode.value = 'psk2';
			} else if(sec.match('WPA')) {
                getWISPCrypto(sec);
			    F.wl$0_apcli$1_secmode.value = 'psk';
			}

			if('$2' == 'WLAN'){
			    F.wl$0_apcli$1_enable.value = "1";
			}
			    F.submit_button.value = "$4";
			    F.action.value = "Gozila";
			    F.submit();
			}
    });

    $("#wl$0_survey").click(function() {
        $("#wl$0_surveyData").jqGrid("clearGridData", true);
	$("#wl$0_MSG").append("Searching...");
	setTimeout(function(){WISPData = getWISPData(); 
			jQuery("#wl$0_surveyData").setGridParam({ 'data':  WISPData }).trigger("reloadGrid"); 
			$("#wl$0_MSG").empty();
		  }, 500);
		
	$("#wl$0_surveyData-dialog").dialog({
	    resizable: false,
	    height:500,
	    width:'auto',
	    modal: true,
	    buttons: {
		"<%lang("Refresh");%>": function(){
					$("#wl$0_MSG").append("Searching...");
					$(".ui-dialog-buttonpane button:contains('<%lang("Refresh");%>')").button("disable");
					$("#wl$0_surveyData").jqGrid("clearGridData", true);
					setTimeout(function(){
						WISPData = getWISPData(); 
						jQuery("#wl$0_surveyData").setGridParam({ 'data':  WISPData }).trigger("reloadGrid");
						$(".ui-dialog-buttonpane button:contains('<%lang("Refresh");%>')").button("enable");
						$("#wl$0_MSG").empty();
					}, 500);
				},
		"<%lang("Cancel");%>": function() {
					$( this ).dialog( "close" );
				}
			}
		});
	});
});

</script>

<div id="wl$0_surveyData-dialog" title="WISP" style="display: none">
	<table id="wl$0_surveyData"></table>
	<div id="wl$0_MSG"></div>
</div>

<input type='hidden' name='wan0_mtu' value='1500'/>

<% apcli_fieldset_need("start", $2);%>
<fieldset>
  <legend> $2 <%increase("$0","1");%></legend>
  <div class="setting">
    <div class="label"> $3 <%lang("Enable");%> </div>
    <input type="radio" value="1" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "1", "checked"); %>><% lang("Enable"); %></input>
    <input type="radio" value="0" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "0", "checked"); %>><% lang("Disable"); %></input>
  </div>
<% apcli_fieldset_need("end", $2);%>
  <div class="setting">

  <div class="label"><% lang("SSID"); %></div>
  <input type="text" name="wl$0_apcli$1_ssid" value='<% nvg_attr_get("wl$0_apcli$1_ssid","wl$0_apcli_rule","$1","ssid");%>'><br/>

  <div class="label"><% lang("Target BSSID (MAC)"); %></div>
  <input type="text" name="wl$0_apcli$1_bssid" value='<% nvg_attr_get("wl$0_apcli$1_bssid","wl$0_apcli_rule","$1","bssid");%>'><br/>

  <div class="label"><% lang("Wireless Channel"); %></div>
  <select name="wl$0_channel">
    <script language="javascript" type="text/javascript"> 
    var max_channel = 12;
    var wl_channel = <%nvg_attr_get("wl$0_channel","wl_basic_rule","$0","channel");%>;
    var buf = "";
    var reg = '<% nv_get("wl_region"); %>';
    if (reg == '0') {
        regnum = 0;
    } else if (reg == '1') {
        regnum = 1;
    } else if (reg == '2') {
        regnum = 2;
    } else if (reg == '5') {
        regnum = 5;
    } else {
        regnum = 0;
    }
    var region = new Array();
    region[0] = new Array();
    region[1] = new Array();
    region[2] = new Array();
    region[3] = new Array();
    region[4] = new Array();
    region[5] = new Array();
    region[6] = new Array();
    region[7] = new Array();
    region[0][0] = 1;
    region[0][1] = 11;
    region[1][0] = 1;
    region[1][1] = 13;
    region[2][0] = 10;
    region[2][1] = 11;
    region[3][0] = 10;
    region[3][1] = 13;
    region[4][0] = 14;
    region[4][1] = 14;
    region[5][0] = 1;
    region[5][1] = 14;
    region[6][0] = 3;
    region[6][1] = 9;
    region[7][0] = 5;
    region[7][1] = 13;
    var freq = new Array("","2.412","2.417","2.422","2.427","2.432","2.437","2.442","2.447","2.452",
                   "2.457","2.462", "2.467","2.472","2.484","");
    for(i=region[regnum][0]; i <= region[regnum][1]; i++){
        if(i == wl_channel) 
          buf = "selected";
        else
          buf = "";
        document.write("<option value=\"" + i + "\" " + buf + "> Channel " + i +"   [" + freq[i] + "GHz]<\/option>");
    }
    </script>
  </select>
  <div >
  <div class="label"><% lang("Extention Channel"); %></div>
  <select name="wl$0_extcha">
  <option value="1" <%nvg_attr_match("wl$0_extcha", "wl_advanced_rule", "$0", "extcha", "1", "selected");%>><% lang("Above"); %></option>
  <option value="0" <%nvg_attr_match("wl$0_extcha", "wl_advanced_rule", "$0", "extcha", "0", "selected");%>><% lang("Below"); %></option>
    </select>
  </div>
  <div >
  <div class="label"><% lang("Site Survey"); %></div>
  <input type="button" id="wl$0_survey" value='<%lang("Survey");%>'>
  </div>
  <div id="survey_window"></div>

  <div class="label"><% lang("Security Mode"); %></div>
  <select name="wl$0_apcli$1_secmode" onchange="SelAPCLI(this.form)">
  <option value="disabled" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli__rule", "$1", "secmode", "disabled", "selected");%>><% lang("Disable"); %></option>
  <option value="wep" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
  <option value="psk" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "psk", "selected");%>><% lang("WPA PSK (Pre-Shared Key)"); %></option>
  <option value="psk2" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "psk2", "selected");%>><% lang("WPA2 PSK (Pre-Shared Key)"); %></option>
  </select>
  <br/>

  <% wlv_apcli_sec_show_setting($0, $1); %>
  </div>
<% apcli_fieldset_need("start", $2);%>
</fieldset>
<% apcli_fieldset_need("end", $2);%>
<br/>
