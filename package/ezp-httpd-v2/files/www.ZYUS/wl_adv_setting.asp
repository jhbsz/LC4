<script language="JavaScript">
wl_adv_objects[$0] =
[ { name: 'wl$0_frag' },
  { name: 'wl$0_rts' },
  { name: 'wl$0_dtim' },
  { name: 'wl$0_bcn' },
  { name: 'wl$0_plcphdr' },
  { name: 'wl$0_antdiv' }
];
</script>
<fieldset>
  <legend><%lang("WLAN");%> </legend>
  <input type="hidden" name="wl$0_enable" id="wl$0_enable" value='<%nv_attr_get("wl_basic_rule", "$0", "enable");%>'>
  <input type="hidden" name="wl_region" value="0"/>
  <!-- TODO: Ask why they still stay. -->
  <input type="hidden" name="wl$0_infra" value='<% nvg_attr_get("wl$0_infra" ,"wl_advanced_rule", "$0", "infra");%>'>
  <input type="hidden" name="wl$0_ap_mode" value='<% nvg_attr_get("wl$0_ap_mode" ,"wl_advanced_rule", "$0", "ap_mode");%>'>
  <input type="hidden" name="wl$0_rateset" value='<% nvg_attr_get("wl$0_rateset" ,"wl_advanced_rule", "$0", "rateset");%>'>
  <input type="hidden" name="wl$0_mrate" value='<% nvg_attr_get("wl$0_mrate" ,"wl_advanced_rule", "$0", "mrate");%>'>
  <input type="hidden" name="wl$0_gmode" value='<% nvg_attr_get("wl$0_gmode" ,"wl_advanced_rule", "$0", "gmode");%>'>
  <input type="hidden" name="wl$0_gmode_protection" 
        value='<% nvg_attr_get("wl$0_gmode_protection" ,"wl_advanced_rule", "$0", "gmode_protection");%>'>
  <input type="hidden" name="wl$0_afterburner" value='<% nvg_attr_get("wl$0_afterburner" ,"wl_advanced_rule", "$0", "afterburner");%>'>
  <input type="hidden" name="wl$0_frameburst" value='<% nvg_attr_get("wl$0_frameburst" ,"wl_advanced_rule", "$0", "frameburst");%>'>
  <input type="hidden" name="wl$0_antdiv" value='<% nvg_attr_get("wl$0_antdiv" ,"wl_advanced_rule", "$0", "antdiv");%>'>

  <div class="setting">
  <div class="label"><%lang("Fragmentation");%></div>
  <input name="wl$0_frag" value='<%nvg_attr_get("wl$0_frag" ,"wl_advanced_rule", "$0", "frag");%>' size="6" maxlength="4" 
            onchange="valid_range(this, 256, 2346, 'Fragmentation value is between 256 and 2346')"> <%lang("Bytes");%> (256 ~ 2346)
  <br/>

  <div class="label"><%lang("RTS");%></div>
  <input name="wl$0_rts" value='<%nvg_attr_get("wl$0_rts" ,"wl_advanced_rule", "$0", "rts");%>' size="6" maxlength="4"
            onchange="valid_range(this, 1, 2347, 'RTS value is between 1 and 2347')"> <%lang("Seconds");%> (1 ~ 2347)
  <br/>

  <div class="label"><%lang("DTim");%></div>
  <input name="wl$0_dtim" value='<%nvg_attr_get("wl$0_dtim" ,"wl_advanced_rule", "$0", "dtim");%>' size="6" maxlength="3"
            onchange="valid_range(this, 1, 255, 'DTim value is between 1 and 255')"> (1 ~ 255)
  <br/>

  <div class="label"><%lang("Beacon Interval");%></div>
  <input name="wl$0_bcn" value='<% nvg_attr_get("wl$0_bcn" ,"wl_advanced_rule", "$0", "bcn");%>' size="6" maxlength="4"
            onchange="valid_range(this, 20, 1024, 'Beacon value is between 20 and 1024')"> <%lang("Milliseconds");%> (20 ~ 1024)
  <br/>

  <div class="label"><% lang("Header Preamble"); %></div>
  <select name="wl$0_plcphdr">
  <option value="long" <%nvg_attr_match("wl$0_plcphdr", "wl_advanced_rule", "$0", "plcphdr", "long", "selected");%>><% lang("Long"); %></option>
  <option value="short" <%nvg_attr_match("wl$0_plcphdr", "wl_advanced_rule", "$0", "plcphdr", "short", "selected");%>><% lang("Short"); %></option>
  </select>
  <br/>

  </div>
  
  <div class="setting">
    <div class="label"><% lang("TxMode"); %></div>
    <select name="wl$0_txmode">
    <option value="0" <%nvg_attr_match("wl$0_txmode", "wl_advanced_rule", "$0", "txmode", "0", "selected");%>><% lang("None"); %></option>
    <option value="1" <%nvg_attr_match("wl$0_txmode", "wl_advanced_rule", "$0", "txmode", "1", "selected");%>><% lang("CCK"); %></option>
    <option value="2" <%nvg_attr_match("wl$0_txmode", "wl_advanced_rule", "$0", "txmode", "2", "selected");%>><% lang("OFDM"); %></option>
    </select>
  </div>
  <div class="setting">
    <div class="label"><% lang("MPDU"); %></div>
    <select name="wl$0_mpdu"> 
    <option value="0" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "0", "selected");%>>
      <% lang("No Restrictions"); %></option>
    <option value="1" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "1", "selected");%>>
      1/4  </option>
    <option value="2" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "2", "selected");%>>
      1/2 </option>
    <option value="3" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "3", "selected");%>>
      1 </option>
    <option value="4" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "4", "selected");%>>
      2 </option>
    <option value="5" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "5", "selected");%>>
      4 </option>
    <option value="6" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "6", "selected");%>>
      8 </option>
    <option value="7" <%nvg_attr_match("wl$0_mpdu", "wl_advanced_rule", "$0", "mpdu", "7", "selected");%>>
      16 </option>
    </select> <% lang("Microseconds"); %>
  </div>
  <div class="setting">
    <div class="label"><% lang("MSDU Aggregate"); %></div>
    <input type="radio" value="1" name="wl$0_amsdu" onclick="init()"
          <%nvg_attr_match("wl$0_amsdu","wl_advanced_rule","$0","amsdu","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_amsdu" onclick="init()"
          <%nvg_attr_match("wl$0_amsdu","wl_advanced_rule","$0","amsdu","0","checked");%>><%lang("Disable");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><% lang("Tx Burst"); %></div>
    <input type="radio" value="1" name="wl$0_txburst" onclick="init()"
          <%nvg_attr_match("wl$0_txburst","wl_advanced_rule","$0","txburst","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_txburst" onclick="init()"
          <%nvg_attr_match("wl$0_txburst","wl_advanced_rule","$0","txburst","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Packet Aggregate"); %></div>
    <input type="radio" value="1" name="wl$0_pktaggr" onclick="init()"
          <%nvg_attr_match("wl$0_pktaggr","wl_advanced_rule","$0","pktaggr","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_pktaggr" onclick="init()"
          <%nvg_attr_match("wl$0_pktaggr","wl_advanced_rule","$0","pktaggr","0","checked");%>><%lang("Disable");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><% lang("HT Control Field"); %></div>
    <input type="radio" value="1" name="wl$0_htc" onclick="init()"
          <%nvg_attr_match("wl$0_htc","wl_advanced_rule","$0","htc","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_htc" onclick="init()"
          <%nvg_attr_match("wl$0_htc","wl_advanced_rule","$0","htc","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Reverse Direction Grant"); %></div>
    <input type="radio" value="1" name="wl$0_rdg" onclick="init()"
          <%nvg_attr_match("wl$0_rdg","wl_advanced_rule","$0","rdg","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_rdg" onclick="init()"
          <%nvg_attr_match("wl$0_rdg","wl_advanced_rule","$0","rdg","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Link Adapt"); %></div>
    <input type="radio" value="1" name="wl$0_linkadapt" onclick="init()"
          <%nvg_attr_match("wl$0_linkadapt","wl_advanced_rule","$0","linkadapt","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_linkadapt" onclick="init()"
          <%nvg_attr_match("wl$0_linkadapt","wl_advanced_rule","$0","linkadapt","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Short Guard Interval(GI)"); %></div>
    <input type="radio" value="1" name="wl$0_gi" onclick="init()"
          <%nvg_attr_match("wl$0_gi","wl_advanced_rule","$0","gi","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_gi" onclick="init()"
          <%nvg_attr_match("wl$0_gi","wl_advanced_rule","$0","gi","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Operation Mode"); %></div>
    <select name="wl$0_opmode"> 
    <option value="0" <%nvg_attr_match("wl$0_opmode", "wl_advanced_rule", "$0", "opmode", "0", "selected");%>>
      <%lang("Mixed Mode");%> </option>
    <option value="1" <%nvg_attr_match("wl$0_opmode", "wl_advanced_rule", "$0", "opmode", "1", "selected");%>>
      <%lang("Green Field");%> </option>
    </select>
  </div>
  <div class="setting">
    <div class="label"><% lang("HT Band Width"); %></div>
    <select name="wl$0_htbw"> 
    <option value="0" <%nvg_attr_match("wl$0_htbw", "wl_advanced_rule", "$0", "htbw", "0", "selected");%>>
      20 </option>
    <option value="1" <%nvg_attr_match("wl$0_htbw", "wl_advanced_rule", "$0", "htbw", "1", "selected");%>>
      20/40 </option>
    <option value="2" <%nvg_attr_match("wl$0_htbw", "wl_advanced_rule", "$0", "htbw", "2", "selected");%>>
      40 </option>
    </select>
    <%lang("MHz");%>
  </div>

  <div class="setting">
    <div class="label"><% lang("Block Ack Setup Automatically"); %></div>
    <input type="radio" value="1" name="wl$0_autoba" onclick="init()"
          <%nvg_attr_match("wl$0_autoba","wl_advanced_rule","$0","autoba","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_autoba" onclick="init()"
          <%nvg_attr_match("wl$0_autoba","wl_advanced_rule","$0","autoba","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <div class="setting">
    <div class="label"><% lang("Block Ack Window Size"); %></div>
    <input name="wl$0_bawinsize" size="6" maxlength="2" value='<% nvg_attr_get("wl$0_bawinsize" ,"wl_advanced_rule", "$0", "bawinsize");%>'
            onchange="valid_range(this, 1, 64, 'Block Ack Window Size is between 1 and 64')">  </input> x16 <%lang("Bits");%> (1 ~ 64)
  </div>
  <div class="setting">
    <div class="label"><% lang("Reject Block Ack"); %></div>
    <input type="radio" value="1" name="wl$0_badecline" onclick="init()"
          <%nvg_attr_match("wl$0_badecline","wl_advanced_rule","$0","badecline","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wl$0_badecline" onclick="init()"
          <%nvg_attr_match("wl$0_badecline","wl_advanced_rule","$0","badecline","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  <input type="hidden" name="wl$0_mcs" value="33" >


</fieldset>
<br/>
