<script type="text/javascript" src="ext/js/ext-base.js"></script>
<script type="text/javascript" src="ext/js/ext-all.js"></script>
<style type="text/css">
    table {
        border-spacing: 0em;
    }
</style>
<script language="JavaScript">

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
    
Ext.BLANK_IMAGE_URL = "icons/s.gif";

Ext.grid.dummyData = [];

Ext.onReady(function(){
    var op_window;
    var survey_button = Ext.get('wl$0_survey');
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
        {header:'<%lang("ExtCH");%>',  dataIndex:'extcha', width:60,
                     renderer:renderExtCh},
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
    survey_button.on('click',function(){
        if(!op_window) {
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
                    handler: function(){get_station_info()}
                },{
                    text:'<%lang("Refresh");%>',
                    handler: function(){survey_status()}
                }]
            });
        }
        survey_status();
        op_window.show(this);
    });

    function survey_status() {
        contain.store.removeAll();
        F.action.value='Update';
        F.submit_button.value = "wan";
        var postform = new Ext.form.BasicForm('form');    
        postform.submit({
            url: 'survey.cgi',
            //params: {act:'Update'},
            success: function(formPost, o) {
                if (o.result == undefined) {
                    return;
                }
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
                    if (check == -1) {                                    
                        return;                                           
                    }      

                    var items = seeds[i].split('^');
                    /* name size complete progress dl_rate ul_rate hash */
                    for (var j = 0; items[j] != undefined; j++) {
                        Ext.grid.dummyData[i][j] = items[j];
                    }
                }
                contain.store.loadData(Ext.grid.dummyData);
            },
            failure: function(formpost, o) {
                /* todo */
            }
        });
    }
    function get_station_info() {
        var tar = contain.getSelectionModel().getSelections();
        if (tar == '') {
            alert(lang("please select a wireless station"));
            return;
        }
        F.wl$0_apcli$1_ssid.value = tar[0].get("ssid");
        F.wl$0_channel.value = tar[0].get("ch");
        F.wl$0_extcha.value = tar[0].get("extcha");
        var sec = tar[0].get("sec");
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
        if('$2' == 'WLAN')
            F.wl$0_apcli$1_enable.value = "1";
        F.submit_button.value = "$4";
        F.action.value = "Gozila";
        F.submit();
    }

});
</script>
<input type='hidden' name='wan0_mtu' value='1500'/>

<% apcli_fieldset_need("start", $2);%>
<fieldset>
  <legend> $2 </legend>
  <div class="setting">
    <div class="label"> $3 <%lang("Enable");%> </div>
    <div class='radiobutton'><input type="radio" value="1" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "1", "checked"); %>><% lang("Enable"); %></input></div>
    <div class='radiobutton'><input type="radio" value="0" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "0", "checked"); %>><% lang("Disable"); %></input></div>
  </div>
<% apcli_fieldset_need("end", $2);%>
  <div class="setting">

  <div class="label"><% lang("Target SSID"); %></div>
  <input type="text" name="wl$0_apcli$1_ssid" value='<% nvg_attr_get("wl$0_apcli$1_ssid","wl$0_apcli_rule","$1","ssid");%>'><br/>
  <input type="hidden" name="wl$0_apcli$1_bssid" value=''>
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
