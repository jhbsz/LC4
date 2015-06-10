<script type="text/javascript">

var SSIDForm;
function init() {
    SSIDForm = document.getElementsByName("wl0_SSIDForm")[0];
}

function getWISPData() {
    //var WISPData_Tmp;
	$.ajax({
	   url:        "survey.cgi",
	   type:       "POST",
	   dataType:   "text",
	   async:	   true,
	   data:       { action:"Update", submit_button:"wan"},
	   success: function(data){
		   var obj = jQuery.parseJSON(data);
		   data = obj.msg
			//alert("data="+data);
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
		        rows_data['extcha'] = rows_dataArray[6];
				rows[i] = rows_data;
			}
			
			//WISPData_Tmp = rows;
            WISPData = rows;
	   },
	   error: function(xhr, textStatus, error){
		   //console.log(xhr.statusText, textStatus, error);
	   }
	});
	//return WISPData_Tmp;

}

var msg = "<%lang("Router will reconnect to the selected wifi channel. The connection between router and the wireless LAN will end, please reconnect after rebooting.");%>";
var WISPData = [];
<% wan_show_wisp_swap_list(); %>
var CurrentSSID = "<%nvg_attr_get("", "wl0_apcli_rule", "0", "ssid");%>";     
var CurrentBSSID = "<%nvg_attr_get("", "wl0_apcli_rule", "0", "bssid");%>";

$(document).ready(function(){

    LoadingWispData();

    jQuery('#wl0_surveyData').jqGrid({
        data: WISPData,
        datatype: 'local',
        colNames:[ '','Channel', 'SSID', 'BSSID', 'Security Mode', 'Signal','ExtCH','Wireless Mode',"Active"],
        colModel:[
              {name:'choose',index:'choose', width:50, align:"center"},
              {name:'ch',index:'ch', width:50, align:"center", hidden:true},
              {name:'ssid',index:'ssid', width:400, align:"left"},
              {name:'bssid',index:'bssid', width:120, align:"center", hidden:true},
              {name:'sec',index:'sec', width:100, align:"center", hidden:true},
              {name:'sig',index:'', width:100, align:"right"},
              {name:'extcha',index:'extcha', width:60, align:"center", hidden:true},
              {name:'mode',index:'mode', width:80, align:"center", hidden:true},
              {name:'active',index:'active', width:150, align:"center"}
            ],
        autowidth: 'auto',
        width: 'auto',
        height: 'auto',
        rowNum: 50,
        rowList: [20,50,100],
        /*sortname: 'Content',*/
        /*sortorder: 'desc',*/
        cmTemplate: {sortable: false},
        sortable: false,
        pager: '#LogListDataPager',
        viewrecords: true,
        hoverrows:false,
        /*caption: 'Site Survey',*/
        beforeSelectRow : function (){
            return false;   // not allow select the row
        }

    });

    $(".ui-jqgrid-hdiv").hide();    //Hide field titles.
    $("#wl0_surveyData_div").hide();
    $("#submitFoooter").hide();
    $("#wl0_survey").click(function() {
        LoadingWispData();
	});


});


    function LoadingWispData(){
        $("#wl0_surveyData_div").hide();
        $("#wl0_surveyData").jqGrid("clearGridData", true);
        $("#wl0_MSG").append("<%lang("Searching");%>...");
        var ssid, bssid, sec, sig;
        display_mask(SSIDForm);

        getWISPData();
        
        setTimeout(function(){
            jQuery("#wl0_surveyData").setGridParam({ 'data':  WISPData }).trigger("reloadGrid"); 
            
            $("#wl0_MSG").empty();
            for(i=0; i<$('#wl0_surveyData tr').length; i++){
            
                var index = i+1;
                //var index = i;
                var cell_0 = $('#wl0_surveyData tr:eq('+index+') td:eq(0)');
                var cell_2 = $('#wl0_surveyData tr:eq('+index+') td:eq(2)');
                ssid = cell_2.text();
                var cell_3 = $('#wl0_surveyData tr:eq('+index+') td:eq(3)');
                bssid = cell_3.text();

                if(ssid==CurrentSSID && bssid==CurrentBSSID){
                    $("<img src='picts/web_checkmark.png'/>").prependTo(cell_0);
                }

                var cell_4 = $('#wl0_surveyData tr:eq('+index+') td:eq(4)');
                sec = cell_4.text();
                var cell_5 = $('#wl0_surveyData tr:eq('+index+') td:eq(5)');
                sig = cell_5.text();
                
                if (cell_5.length == 0) {
                    //alert("Undefined");
                } else {
                    //alert(cell.text());
                    cell_5.empty();
                    if(sec=="NONE"){
                        if(sig>=76 && sig<=100){
                            $("<img src='picts/web_wavelevel4.png'/>").prependTo(cell_5);
                        } else if(sig>=51 && sig<=75) {
                            $("<img src='picts/web_wavelevel3.png'/>").prependTo(cell_5);
                        } else if(sig>=26 && sig<=50) {
                            $("<img src='picts/web_wavelevel2.png'/>").prependTo(cell_5);
                        } else {
                            $("<img src='picts/web_wavelevel1.png'/>").prependTo(cell_5);
                        }
                    }else{
                        if(sig>=76 && sig<=100){
                            $("<img src='picts/web_wavelevel4.png'/>").prependTo(cell_5);
                            $("<img src='picts/web_securitykey.png'/>").prependTo(cell_5);
                        } else if(sig>=51 && sig<=75) {
                            $("<img src='picts/web_wavelevel3.png'/>").prependTo(cell_5);
                            $("<img src='picts/web_securitykey.png'/>").prependTo(cell_5);
                        } else if(sig>=26 && sig<=50) {
                            $("<img src='picts/web_wavelevel2.png'/>").prependTo(cell_5);
                            $("<img src='picts/web_securitykey.png'/>").prependTo(cell_5);
                        } else {
                            $("<img src='picts/web_wavelevel1.png'/>").prependTo(cell_5);
                            $("<img src='picts/web_securitykey.png'/>").prependTo(cell_5);
                        }
                    }
                }


                var cell_8 = $('#wl0_surveyData tr:eq('+index+') td:eq(8)');
                if(CheckSavedWispData(bssid) && bssid==CurrentBSSID){
                    $("<input type='button' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' onClick='Connection("+index+")' disabled>&nbsp;&nbsp;"+
                      "<input type='button' id='wl0_Forget' value='<%lang("Forget");%>' onClick='Forget("+index+")'>").prependTo(cell_8);
                } else if(CheckSavedWispData(bssid)){
                    $("<input type='button' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' onClick=Connection("+index+",'connect')>&nbsp;&nbsp;"+
                      "<input type='button' id='wl0_Forget' value='<%lang("Forget");%>' onClick='Forget("+index+")'>").prependTo(cell_8);
                } else {
                    $("<input type='button' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' onClick=Connection("+index+",'save')>&nbsp;&nbsp;"+
                      "<input type='button' id='wl0_Forget' value='<%lang("Forget");%>' onClick='Forget("+index+")' disabled>").prependTo(cell_8);
                }
                
                for (var j=0;j<9;j++ ) {
                    $('#wl0_surveyData tr:eq('+index+') td:eq('+j+')').css( {'vertical-align':'middle', 'border-left': '0px solid #ccc', 'border-right': '0px solid #ccc'});
                }

            }//End for
            $("#wl0_surveyData_div").show();
            hidden_mask();
        }, 5000);
    }

    function CheckSavedWispData(LV_bssi){
        var bssid_flag = false;
        for (var i=0; i<SavedWispData.length; i++){
            if(SavedWispData[i]['BSSID']==LV_bssi){
                bssid_flag = true;
            }
        }
        return bssid_flag;
    }

    function Connection(index,action){

        var cell_1 = $('#wl0_surveyData tr:eq('+index+') td:eq(1)');
        var ch = cell_1.text();
        var cell_2 = $('#wl0_surveyData tr:eq('+index+') td:eq(2)');
        var ssid = cell_2.text();
        var cell_3 = $('#wl0_surveyData tr:eq('+index+') td:eq(3)');
        var bssid = cell_3.text();
        var cell_4 = $('#wl0_surveyData tr:eq('+index+') td:eq(4)');
        var sec = cell_4.text();
        var cell_6 = $('#wl0_surveyData tr:eq('+index+') td:eq(6)');
        var extcha = cell_6.text();


        $("#wl0_apcli0_ssid").attr("value", ssid);
        $("#wl0_apcli0_bssid").attr("value", bssid);
        $("#wl0_channel").attr("value", ch);
        $("select[name='wl0_extcha']").attr("value", extcha);
        
        if (sec=="NONE"){
            $("select[name='wl0_apcli0_secmode']").attr("value", "disabled");
        } else if(sec.match('WEP')) {
            $("select[name='wl0_apcli0_secmode']").attr("value", "wep");
        } else if(sec.match('WPAPSK') || sec.match('WPA1PSK')) {
            $("select[name='wl0_apcli0_secmode']").attr("value", "psk");
        } else if(sec.match('WPA2PSK')) {
            $("select[name='wl0_apcli0_secmode']").attr("value", "psk2");
        }


        if(sec.indexOf('TKIP')!=-1 & sec.indexOf('AES')!=-1) {
            $("select[name='wl0_apcli0_crypto']").attr("value", "mixed");
        } else if(sec.indexOf('TKIP')!=-1 && sec.indexOf('AES')==-1) {
            $("select[name='wl0_apcli0_crypto']").attr("value", "tkip");
        } else if(sec.indexOf('TKIP')==-1 && sec.indexOf('AES')!=-1){
            $("select[name='wl0_apcli0_crypto']").attr("value", "aes");
        }

        if(sec!="NONE"){
            if(sec=="WEP"){
                var trs1 = $("tr[class='css_wep']");
                for(i = 0; i < trs1.length; i++){     
                    trs1[i].style.display = "";
                }  

                var trs2 = $("tr[class='css_notwep']");
                for(i = 0; i < trs2.length; i++){     
                    trs2[i].style.display = "none";
                }
                
            } else {
                var trs1 = $("tr[class='css_wep']");
                for(i = 0; i < trs1.length; i++){     
                    trs1[i].style.display = "none";
                }  

                var trs2 = $("tr[class='css_notwep']");
                for(i = 0; i < trs2.length; i++){     
                    trs2[i].style.display = "";
                }
            }
        }else{
                var trs1 = $("tr[class='css_wep']");
                for(i = 0; i < trs1.length; i++){     
                    trs1[i].style.display = "none";
                }  

                var trs2 = $("tr[class='css_notwep']");
                for(i = 0; i < trs2.length; i++){     
                    trs2[i].style.display = "none";
                }
        } 

        $("#wl0_apcli0_secmode").attr("disabled", true);
        $("#wl0_apcli0_crypto").attr("disabled", true);

        if(action=="save"){ //Save
            $("#wl0_surveyData-dialog").dialog({
                resizable: false,
                height:'auto',
                width:'auto',
                modal: true,
                buttons: {
                    "<%lang("Cancel");%>": function() {
                            $( this ).dialog( "close" );
                    },
                    "<%lang("OK");%>": function(){
                            if(sec!="NONE"){
                                if($("#wl0_apcli0_key1").val()=="" && $("#wl0_apcli0_key").val()==""){
                                    alert("<%lang("Please input the password!");%>");
                                    return
                                }
                            }

                            $("#wl0_apcli0_secmode").attr("disabled", false);
                            $("#wl0_apcli0_crypto").attr("disabled", false);
                            display_mask(SSIDForm);
                            $( "#wl0_surveyData-dialog" ).dialog( "close" );

                            setTimeout(function(){
                                $.ajax({
                                   url:        "apply.cgi",
                                   type:       "POST",
                                   dataType:   "text",
                                   async:	   true,
                                   data:       { action:"Apply", 
                                                 submit_button:"save", 
                                                 wisp_swap_rule:"", 
                                                 wl0_apcli0_ssid:$("#wl0_apcli0_ssid").val(),
                                                 wl0_apcli0_bssid:$("#wl0_apcli0_bssid").val(), 
                                                 wl0_channel:$("#wl0_channel").val(),
                                                 wl0_extcha:$("#wl0_extcha").val(), 
                                                 wl0_apcli0_secmode:$("#wl0_apcli0_secmode").val(),
                                                 wl0_apcli0_key_index:$("#wl0_apcli0_key_index").val(), 
                                                 wl0_apcli0_key1:encode64($("#wl0_apcli0_key1").val()),
                                                 wl0_apcli0_key2:encode64($("#wl0_apcli0_key2").val()),
                                                 wl0_apcli0_key3:encode64($("#wl0_apcli0_key3").val()),
                                                 wl0_apcli0_key4:encode64($("#wl0_apcli0_key4").val()), 
                                                 wl0_apcli0_key:encode64($("#wl0_apcli0_key").val()),
                                                 wl0_apcli0_crypto:$("#wl0_apcli0_crypto").val() },
                                   success: function(data){
                                          show_mask();
                                          //var msg = "Setting: " + '<%lang("succeeded");%>';
                                          display_return(-3, msg);
                                          //$( "#wl0_surveyData-dialog" ).dialog( "close" );
                                   },
                                   error: function(xhr, textStatus, error){
                                       //console.log(xhr.statusText, textStatus, error);
                                   }
                                });

                            }, 1000);
                    }
                }

            });

        }else{  //Connect
            display_mask(SSIDForm);
            setTimeout(function(){
                $.ajax({
                   url:        "apply.cgi",
                   type:       "POST",
                   dataType:   "text",
                   async:	   true,
                   data:       { action:"Apply", 
                                 submit_button:"save", 
                                 wisp_swap_rule:"", 
                                 wl0_apcli0_ssid:$("#wl0_apcli0_ssid").val(),
                                 wl0_apcli0_bssid:$("#wl0_apcli0_bssid").val(), 
                                 wl0_channel:$("#wl0_channel").val(),
                                 wl0_extcha:$("#wl0_extcha").val(), 
                                 wl0_apcli0_secmode:$("#wl0_apcli0_secmode").val(),
                                 wl0_apcli0_key_index:$("#wl0_apcli0_key_index").val(), 
                                 wl0_apcli0_key1:encode64($("#wl0_apcli0_key1").val()),
                                 wl0_apcli0_key2:encode64($("#wl0_apcli0_key2").val()),
                                 wl0_apcli0_key3:encode64($("#wl0_apcli0_key3").val()),
                                 wl0_apcli0_key4:encode64($("#wl0_apcli0_key4").val()), 
                                 wl0_apcli0_key:encode64($("#wl0_apcli0_key").val()),
                                 wl0_apcli0_crypto:$("#wl0_apcli0_crypto").val() },
                   success: function(data){
                          show_mask();
                          //var msg = "Setting: " + '<%lang("succeeded");%>';
                          display_return(-3, msg);
                          //$( "#wl0_surveyData-dialog" ).dialog( "close" );
                   },
                   error: function(xhr, textStatus, error){
                       //console.log(xhr.statusText, textStatus, error);
                   }
                });

            }, 1000);

        }//End if

    }

    function Forget(index){
        var cell_2 = $('#wl0_surveyData tr:eq('+index+') td:eq(2)');
        var ssid = cell_2.text();
        var cell_3 = $('#wl0_surveyData tr:eq('+index+') td:eq(3)');
        var bssid = cell_3.text();

        display_mask(SSIDForm);
        setTimeout(function(){
            $.ajax({
               url:        "apply.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   true,
               data:       { action:"Apply", 
                             submit_button:"forget", 
                             wisp_swap_rule:"", 
                             wl0_apcli0_ssid:ssid,
                             wl0_apcli0_bssid:bssid},
               success: function(data){
                      show_mask();
                      //var msg = "Setting: " + '<%lang("succeeded");%>';
                      display_return(-3, msg);
                      $( "#wl0_surveyData-dialog" ).dialog( "close" );
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });
        }, 1000);

    }
/*
    function to_submit(Action) {
    }
*/
</script>

    <form id="wl0_SSIDForm" name="wl0_SSIDForm" action="apply.cgi" method="post" >
        <div id="wl0_surveyData-dialog" title="" style='display:none;'>
            <table id="wl0_SSIDTable">
                <tr><td align='right'><%lang("Target SSID");%>:</td><td><input type='text' name='wl0_apcli0_ssid' id='wl0_apcli0_ssid' value='' readonly/></td></tr>
                <tr style='display:none;'><td align='right'><%lang("Target BSSID (MAC)");%>:</td><td><input type='text' name='wl0_apcli0_bssid' id='wl0_apcli0_bssid' value=''/></td></tr>
                <tr style='display:none;'><td align='right'><%lang("Wireless Channel");%>:</td><td><input type='text' name='wl0_channel' id='wl0_channel' value=''/></td></tr>
                <tr style='display:none;'><td align='right'><%lang("Extention Channel");%>:</td><td><select name='wl0_extcha' id='wl0_extcha'><option value='1' >Above</option>
                               <option value='0' selected>Below</option></select></td></tr>
                <tr><td align='right'><%lang("Security Mode");%>:</td><td><select name='wl0_apcli0_secmode' id='wl0_apcli0_secmode'>
                               <option value='disabled' >Disable</option><option value='wep' >WEP</option><option value='psk' >WPA PSK (Pre-Shared Key)</option>
                               <option value='psk2' >WPA2 PSK (Pre-Shared Key)</option></select></td></tr>

                <tr style='display:none;'><td align='right'><%lang("Key Index");%>:</td><td><select name='wl0_apcli0_key_index' id='wl0_apcli0_key_index'>
                                      <option value='1' selected>1</option><option value='2' >2</option><option value='3'>3</option><option value='4'>4</option></select></td></tr>
                <tr class="css_wep"><td align='right'><%lang("Password");%>:</td><td>
                                      <input type='password' name='wl0_apcli0_key1' id='wl0_apcli0_key1' size='17' maxlength='32' onChange="valid_wl_sec_wep_key(this,'WEP Key 1')" value=''>
                                      </td></tr>
                <tr style='display:none;'><td align='right'><%lang("Key 2");%>:</td><td>
                                      <input type='password' name='wl0_apcli0_key2' id='wl0_apcli0_key2' size='17' maxlength='32' onChange="valid_wl_sec_wep_key(this,'WEP Key 2')" value=''>
                                      </td></tr>
                <tr style='display:none;'><td align='right'><%lang("Key 3");%>:</td><td>
                                      <input type='password' name='wl0_apcli0_key3' id='wl0_apcli0_key3' size='17' maxlength='32' onChange="valid_wl_sec_wep_key(this,'WEP Key 3')" value=''>
                                      </td></tr>
                <tr style='display:none;'><td align='right'><%lang("Key 4");%>:</td><td>
                                      <input type='password' name='wl0_apcli0_key4' id='wl0_apcli0_key4' size='17' maxlength='32' onChange="valid_wl_sec_wep_key(this,'WEP Key 4')" value=''>
                                      </td></tr>

                <tr class="css_notwep"><td align='right'><%lang("Password");%>:</td><td>
                                      <input type='password' name='wl0_apcli0_key' id='wl0_apcli0_key' size='17' maxlength='32' onChange=\"valid_wl_sec_wep_key(this,'WEP Key')\" value=''>
                                      </td></tr>
                <tr style='display:none;'><td align='right'><%lang("Encryption Method");%>:</td><td>
                                      <select name='wl0_apcli0_crypto' id='wl0_apcli0_crypto'><option value='tkip' >TKIP</option><option value='aes' >AES</option>
                                      <option value='mixed' >Mixed (TKIP+AES)</option></select></td></tr>
            </table>
        </div>
    </form>

          <div id="contentsInfo">
              <div id="wl0_surveyData_div">
                <table id="wl0_surveyData"></table>
              </div>
              <div id="wl0_MSG"></div>
          </div>
