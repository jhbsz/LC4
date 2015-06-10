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
  { name: 'wl$0_channel' }
];
</script>

<fieldset>
  <legend> $2 <%increase("$0","1");%></legend>
  <div class="setting">
    <div class="label"> $3 </div>
    <input type="radio" value="1" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "1", "checked"); %>><% lang("Enable"); %></input>
    <input type="radio" value="0" name="wl$0_apcli$1_enable" onclick='init()'
          <% nvg_attr_match("wl$0_apcli$1_enable", "wl$0_apcli_rule", "$1", "enable", "0", "checked"); %>><% lang("Disable"); %></input>
  </div>
  <div class="setting">

  <div class="label"><% lang("Target SSID"); %></div>
  <input type="text" name="wl$0_apcli$1_ssid" value='<% nvg_attr_get("wl$0_apcli$1_ssid","wl$0_apcli_rule","$1","ssid");%>'><br/>

  <div class="label"><% lang("Target BSSID (MAC)"); %></div>
  <input type="text" name="wl$0_apcli$1_bssid" value='<% nvg_attr_get("wl$0_apcli$1_bssid","wl$0_apcli_rule","$1","bssid");%>'><br/>

  <div class="label"><% lang("Wireless Channel"); %></div>
  <select name="wl$0_channel">
    <script language="javascript" type="text/javascript"> 
    var max_channel = 12;
    var wl_channel = <%nvg_attr_get("wl$0_channel","wl_basic_rule","$0","channel");%>;
    var buf = "";
    var reg = '<% nvg_get("wl_region"); %>';
    if (reg == '0') {
        regnum = 0;
    } else if (reg == '1') {
        regnum = 1;
    } else if (reg == '2') {
        regnum = 1;
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
</fieldset>
<br/>
