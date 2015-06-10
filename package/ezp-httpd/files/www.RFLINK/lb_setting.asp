<script language="JavaScript">
lb_objects[$0] =
[
  { name: 'wan$0_detect_threshold' },
  { name: 'wan$0_detect_timeout' },
  { name: 'wan$0_detect_type' },
  { name: 'wan$0_detect_host' },
  { name: 'wan$0_detect_threshold' },
  { name: 'wan$0_failback_enable' }
];

host_objects[$0] =
[
  { name: 'wan$0_detect_host' },
];

</script>
<fieldset>
<legend> <%lb_legend();%> <%lang("WAN");%> <%increase("$0", "1");%> </legend>

<%prod_show_html("start", "lb");%>
    <div class="setting">
    <div class="label"><%lang("Connection Mode");%></div>
    <select name="wan$0_mode" onchange="available_update()">
      <option value="alwayson" <%nvg_attr_match("wan$0_mode", "wan_lb_rule", "$0", "type", "alwayson", "selected"); %>>Always On</option>
     <option value="backup_standby" <%nvg_attr_match("wan$0_mode", "wan_lb_rule", "$0", "type", "backup_standby", "selected"); %>>Backup Standby</option>
     <option value="backup" <%nvg_attr_match("wan$0_mode", "wan_lb_rule", "$0", "type", "backup", "selected"); %>>Backup</option>
    </select>
    </div>

    <div class="setting">
    <div class="label"><%lang("Backup WAN");%></div>
    <select name="wan$0_backup" onchange="available_update()">
        <%lb_show_available($0);%>
    </select>
    </div>

  <div class="setting" id="wan$0_weight">
    <div class="label"><%lang("Load Balance Weight");%></div>
    <input type="num" name="wan$0_weight" size="2" maxlength="2" 
            value='<%nvg_attr_get("wan$0_weight", "wan_lb_rule", "$0", "weight");%>'
            onblur='valid_number(this, "Load Balance Priority")' />
    </input>
  </div>

  <input type="hidden" name="wan$0_maxcount" value="10"/>
<%prod_show_html("end", "lb");%>

  <div class="setting">
    <div class="label">
      <%lang("External Connection Detection");%>
    </div><input type="radio" value="1" name="wan$0_detect_enable"
    onclick="init()" <%nvg_attr_match("wan$0_detect_enable","wan_detect_rule","$0","enable","1","checked");%>><%lang("Enable");%>
    <input type="radio" value="0" name="wan$0_detect_enable" onclick="init()"
<%nvg_attr_match("wan$0_detect_enable","wan_detect_rule","$0","enable","0","checked");%>><%lang("Disable");%>
  </div>
  <div class="setting">
    <div class="label">
    <%lang("Connection Detection Threshold");%></div>
    <input class="num" name="wan$0_detect_threshold" size="2"
        maxlength="2" onblur='valid_number(this, "Connection Detection Threshold")' value='<%nvg_attr_get("wan$0_detect_threshold", "wan_detect_rule", "$0", "threshold");%>'> <%lang("Time(s)");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><%lang("Detection Timeout");%></div>
    <input type="text" name="wan$0_detect_timeout" size="10"
            maxlength="11" value='<%nvg_attr_get("wan$0_detect_timeout", "wan_detect_rule", "$0", "timeout");%>'onblur='valid_number(this, "Detection Timeout")'/><%lang("Seconds");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><%lang("Detection Type");%></div>
    <select name="wan$0_detect_type" onchange="init()">
      <option value="gateway" <%nvg_attr_match("wan$0_detect_type", "wan_detect_rule", "$0", "host_type", "gateway", "selected"); %>> Gateway</option> <option value="custom" <%nvg_attr_match("wan$0_detect_type", "wan_detect_rule", "$0", "host_type", "custom", "selected"); %>> Custom Host</option>
    </select>
  </div>

  <div class="setting">
    <div class="label"><%lang("Custom Detection Host");%></div>
    <input type="text" name="wan$0_detect_host" size="20"
            maxlength="32" value='<%nvg_attr_get("wan$0_detect_host", "wan_detect_rule", "$0", "host");%>' onblur="valid_reserved_word(this, 'Detection Host')" /> </input>
  </div>                                                                        


<!--
  <div class="setting">
    <div class="label">
      <%lang("Failback");%>
    </div><input type="radio" value="1" name="wan$0_failback_enable" onclick="init()" <%nvg_attr_match("wan$0_failback_enable","wan_failover_rule","$0","enable","1","checked");%>><%lang("Enable");%>
    <input type="radio" value="0" name="wan$0_failback_enable" onclick="init()"
<%nvg_attr_match("wan$0_failover_enable","wan_failover_rule","$0","enable","0","checked");%>><%lang("Disable");%>
</div>                                                   
-->
</fieldset><br />
