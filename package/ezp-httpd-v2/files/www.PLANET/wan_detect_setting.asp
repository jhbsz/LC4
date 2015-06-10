<script language="JavaScript">
detect_objects[$0] =
[
  { name: 'wan$0_detect_host' },
  { name: 'wan$0_detect_threshold' }
];
</script>
  <fieldset>
    <legend><%lang("WAN Detect");%> -
    <%lang("WAN");%>
    <%increase("$0", "1");%></legend>

      <div class="setting">
        <div class="label"><%lang("External Connection Detection");%></div>
        <input type="radio" value="1" name="wan$0_detect_enable" onclick="init()" 
        <%nvg_attr_match("wan$0_detect_enable","wan_detect_rule","$0","enable","1","checked");%>>
        <%lang("Enable");%>
        <input type="radio" value="0" name="wan$0_detect_enable" onclick="init()"
        <%nvg_attr_match("wan$0_detect_enable","wan_detect_rule","$0","enable","0","checked");%>>
        <%lang("Disable");%>
      </div>

      <input type="hidden" name="wan$0_detect_type" 
      value='<%nvg_attr_get("wan$0_detect_type", "wan_detect_rule", "$0","type");%>'>
      </input>

      <div class="setting">
        <div class="label"><%lang("Detection Host");%></div>
        <input type="text" name="wan$0_detect_host" size="20" maxlength="32" 
        value='<%nvg_attr_get("wan$0_detect_host", "wan_detect_rule", "$0", "host");%>'
        onblur="valid_reserved_word(this, 'Detection Host')" />
        (<%lang("IP address or domain name");%>)
        </input>
      </div>

      <div class="setting">
        <div class="label"><%lang("Detection Interval");%></div>
        <input type="hidden" name="wan$0_detect_interval" value="60"> 60 <%lang("Seconds");%>
       </input>
      </div>

      <div class="setting">
        <div class="label"><%lang("Connection Detection Threshold");%></div>
        <input class="num" name="wan$0_detect_threshold" size="2" maxlength="2" 
        onblur='valid_number(this, "Connection Detection Threshold")' 
        value='<%nvg_attr_get("wan$0_detect_threshold", "wan_detect_rule", "$0", "threshold");%>'>
        <%lang("Time(s)");%>(1~32)
        </input>
      </div>

  </fieldset><br>
