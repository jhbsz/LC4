<script language="JavaScript">
wl_basic_objects[$0] =
[ { name: 'wl$0_net_mode' },
  { name: 'wl$0_txpower' },
  { name: 'wl$0_channel' },
  { name: 'wl$0_bisolation_enable' }
];
</script>
<fieldset>
  <legend><%lang("WLAN");%> <%increase("$0","1");%></legend>
  <div class="setting">
  <div class="label"><% lang("Wireless Connection"); %></div>
  <input type="radio" value="1" name= "wl$0_radio_enable" onclick='init()'
    <%nvg_attr_match("wl$0_radio_enable", "wl_basic_rule", "$0", "enable", "1", "checked");%>><% lang("Enable"); %> </input>
  <input type="radio" value="0" name= "wl$0_radio_enable" onclick='init()'
    <%nvg_attr_match("wl$0_radio_enable", "wl_basic_rule", "$0", "enable", "0", "checked");%>><% lang("Disable"); %> </input>
  <br/>

  <div class="label"><% lang("Wireless Mode"); %></div>
  <select name="wl$0_net_mode">
  <option value="1" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "1", "selected");%>><% lang("B only"); %></option>
  <option value="4" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "4", "selected");%>><% lang("G only"); %></option>
  <option value="0" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "0", "selected");%>><% lang("B/G Mixed"); %></option>
  <%wlv_no_11n_show_mark_begin();%>
  <option value="7" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "7", "selected");%>><% lang("G/N Mixed"); %></option>
  <option value="9" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "9", "selected");%>><% lang("B/G/N Mixed"); %></option>
  <option value="6" <%nvg_attr_match("wl$0_net_mode", "wl_basic_rule", "$0", "net_mode", "6", "selected");%>><% lang("N only"); %></option>
  <%wlv_no_11n_show_mark_end();%>
  </select>
  <br/>
  
  <div class="label"><% lang("Transmission Power"); %></div>
  <select name="wl$0_txpower">
  <option value="100"  <%nvg_attr_match("wl$0_txpower", "wl_basic_rule", "$0", "txpower", "100", "selected");%>><% lang("100%"); %></option>
  <option value="75" <%nvg_attr_match("wl$0_txpower", "wl_basic_rule", "$0", "txpower", "75", "selected");%>><% lang("75%"); %></option>
  <option value="50" <%nvg_attr_match("wl$0_txpower", "wl_basic_rule", "$0", "txpower", "50", "selected");%>><% lang("50%"); %></option>
  <option value="25" <%nvg_attr_match("wl$0_txpower", "wl_basic_rule", "$0", "txpower", "25", "selected");%>><% lang("25%"); %></option>
  <option value="10" <%nvg_attr_match("wl$0_txpower", "wl_basic_rule", "$0", "txpower", "10", "selected");%>><% lang("10%"); %></option>
  </select>
  <br/>
  
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
    if(0 == wl_channel) 
        buf = "selected";
    else
        buf = "";
    document.write("<option value=\"0\"" + buf + "> Auto Channel <\/option>");
    for(i=region[regnum][0]; i <= region[regnum][1]; i++){
        if(i == wl_channel) 
          buf = "selected";
        else
          buf = "";
        document.write("<option value=\"" + i + "\" " + buf + "> Channel " + i +"   [" + freq[i] + "GHz]<\/option>");
    }
    </script>
  </select>
  <br/>
  
  <%wlv_single_ssid_show_mark_begin();%>
  <div class="label"><% lang("Wireless Isolation Between SSIDs"); %></div>
  <input type="radio" value="1" name= "wl$0_bisolation_enable" onclick='init()'
    <%nvg_attr_match("wl$0_bisolation_enable", "wl_basic_rule", "$0", "bisolation", "1", "checked");%>><% lang("Enable"); %> </input>
  <input type="radio" value="0" name= "wl$0_bisolation_enable" onclick='init()'
    <%nvg_attr_match("wl$0_bisolation_enable", "wl_basic_rule", "$0", "bisolation", "0", "checked");%>><% lang("Disable"); %> </input>
  <br/>
  <%wlv_single_ssid_show_mark_end();%>

  </div>

</fieldset>
<br/>
<% wlv_show_entry_setting("$0", "wlv_setting.asp");%>

