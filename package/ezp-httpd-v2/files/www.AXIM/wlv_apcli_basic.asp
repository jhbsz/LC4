
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

				rows_data[0] = rows_dataArray[0];
				rows_data[1] = rows_dataArray[1];
				rows_data[2] = rows_dataArray[2];
				rows_data[3] = rows_dataArray[3];
				rows_data[4] = rows_dataArray[4];
                if (rows_dataArray[6] == '1') {
                    rows_data[5] = 'ABOVE';
                } else {
                    rows_data[5] = 'BELOW';
                }
				rows_data[6] = rows_dataArray[5];

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

var DataTable;
function DrawDataTables(LV_Data){
    DataTable = $('#wl$0_surveyData').dataTable( {
        "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
        "sPaginationType": "bootstrap",
        "oLanguage": {
            "sLengthMenu": "_MENU_ records per page"
        },
        "aaData": LV_Data,
        "aoColumns": [
            { "sTitle": "<%lang("Channel");%>", "sClass":"center" },
            { "sTitle": "<%lang("SSID");%>", "sClass":"center" },
            { "sTitle": "<%lang("BSSID");%>", "sClass":"center" },
            { "sTitle": "<%lang("Security Mode");%>", "sClass":"center" },
            { "sTitle": "<%lang("Signal");%>" },
            { "sTitle": "<%lang("ExtCH");%>", "sClass":"center" },
            { "sTitle": "<%lang("Wireless Mode");%>" },
        ],
        "bSort": false,
        "bPaginate": false,
        "bLengthChange": false,
        "bFilter": false,
        "bInfo": false
    } );   
    
    
    $("#wl$0_surveyData tbody tr").click( function() {  // selete a row data
       
        if ( $(this).hasClass('row_selected') ) {
            $(this).removeClass('row_selected');
            selectID=null;
        }else {
            DataTable.$('tr.row_selected').removeClass('row_selected');
            $(this).addClass('row_selected');
            var IndexID = $(this).index();

            F = document.getElementsByName('form')[0];
			F.wl$0_apcli$1_ssid.value = LV_Data[IndexID][1];
			F.wl$0_apcli$1_bssid.value = LV_Data[IndexID][2];
			F.wl$0_channel.value = LV_Data[IndexID][0];
            if (LV_Data[IndexID][5] == "ABOVE") {
                F.wl$0_extcha.value = 1;
            } else {
                F.wl$0_extcha.value = 0;
            }
			var sec = LV_Data[IndexID][3];
						
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
    

}

function getWISPCrypto(sec) {
    var crypto = sec.split("/");
    if (crypto[1] == 'TKIP') {
        $("#wl$0_apcli$1_crypto").val("tkip");
    } else if (crypto[1] == 'AES') {
        $("#wl$0_apcli$1_crypto").val("aes");
    } else {
        $("#wl$0_apcli$1_crypto").val("mixed");
    }
}

$(document).ready(function(){
    $("#wl$0_survey").click(function() {
        $("#wl$0_MSG").append("<br><br><br><br><br><br>Searching...");
        $("#wl$0_surveyData").hide();
        $('#wl$0_surveyData-modal').modal({
            keyboard: false,
            backdrop: 'static'
        });
        setTimeout(function(){
                   $("#wl$0_MSG").empty();
                   $("#wl$0_surveyData").show();
                   var WISPData = getWISPData(); 
                   DrawDataTables(WISPData);
              }, 500);

    });

    
    $("#refresh").click(function(){
        $("#wl$0_MSG").append("<br><br><br><br><br><br>Searching...");
        $("#wl$0_surveyData").hide();
        setTimeout(function(){
                   $("#wl$0_MSG").empty();
                   $("#wl$0_surveyData").show();
                   var WISPData = getWISPData();
                    DataTable.fnClearTable();
                    DataTable.fnDestroy();
                   DrawDataTables(WISPData);
              }, 500);
    })


    $("#close").click(function(){
        DataTable.fnClearTable();
        DataTable.fnDestroy();
    })

});

</script>

<div id="wl$0_surveyData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-body" style="height:400px;">
        <div id="wl$0_MSG"></div>
        <table class="table" id="wl$0_surveyData"></table>
    </div>
    <div class="modal-footer">
        <input type="button" class="btn btn-primary" name="refresh" id="refresh" value="<% lang("Refresh"); %>" >
        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
    </div>
</div>

<input type='hidden' name='wan0_mtu' value='1500'/>

<% apcli_fieldset_need("start", $2);%>
        <div class="row">
            <div class="span12">
                <legend>$2 <%increase("$0","1");%></legend>


                <div class="row show-grid">
                    <div class="span4"> $3 <%lang("Enable");%> </div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="wl$0_apcli$1_enable" id="wl$0_apcli$1_enable" onclick='init()'
                                  <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="wl$0_apcli$1_enable" id="wl$0_apcli$1_enable" onclick='init()'
                                  <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>
 <% apcli_fieldset_need("end", $2);%>
   
    <div class="row show-grid">
        <div class="span4"><% lang("Target SSID"); %></div>
        <div class="span8">
          <input type="text" name="wl$0_apcli$1_ssid" id="wl$0_apcli$1_ssid" value='<% nvg_attr_get("wl$0_apcli$1_ssid","wl$0_apcli_rule","$1","ssid");%>'>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Target BSSID (MAC)"); %></div>
        <div class="span8">
            <input type="text" name="wl$0_apcli$1_bssid" id="wl$0_apcli$1_bssid" value='<% nvg_attr_get("wl$0_apcli$1_bssid","wl$0_apcli_rule","$1","bssid");%>'>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Wireless Channel"); %></div>
        <div class="span8">
          <select name="wl$0_channel" id="wl$0_channel">
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
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Extention Channel"); %></div>
        <div class="span8">
          <select name="wl$0_extcha" id="wl$0_extcha">
              <option value="1" <%nvg_attr_match("wl$0_extcha", "wl_advanced_rule", "$0", "extcha", "1", "selected");%>><% lang("Above"); %></option>
              <option value="0" <%nvg_attr_match("wl$0_extcha", "wl_advanced_rule", "$0", "extcha", "0", "selected");%>><% lang("Below"); %></option>
          </select>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Site Survey"); %></div>
        <div class="span8">
            <input type="button" class="btn" name="wl$0_survey" id="wl$0_survey" value='<%lang("Survey");%>'>
          <div id="survey_window"></div>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Security Mode"); %></div>
        <div class="span8">
          <select name="wl$0_apcli$1_secmode" id="wl$0_apcli$1_secmode" onchange="SelAPCLI(this.form)">
              <option value="disabled" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli__rule", "$1", "secmode", "disabled", "selected");%>><% lang("Disable"); %></option>
              <option value="wep" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
              <option value="psk" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "psk", "selected");%>><% lang("WPA PSK (Pre-Shared Key)"); %></option>
              <option value="psk2" <%nvg_attr_match("wl$0_apcli$1_secmode", "wl$0_apcli_rule", "$1", "secmode", "psk2", "selected");%>><% lang("WPA2 PSK (Pre-Shared Key)"); %></option>
          </select>
        </div>
    </div>
  <% wlv_apcli_sec_show_setting($0, $1); %>

<% apcli_fieldset_need("start", $2);%>
            </div>
        </div>
<% apcli_fieldset_need("end", $2);%>
<br/>
