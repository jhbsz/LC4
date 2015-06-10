<script language="JavaScript">
</script>
<fieldset>
  <legend><%lang("iTunes");%></legend>

  <div class="setting">
    <div class="label"><%lang("iTunes Server");%></div>
    <input type="radio" value="1" name="enable" onclick="init()"
          <%nvg_attr_match("enable","itunes_rule","0","enable","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="enable" onclick="init()"
          <%nvg_attr_match("enable","itunes_rule","0","enable","0","checked");%>><%lang("Disable");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><%lang("iTunes Password");%></div>
      <input size="20" name="itunes_passwd" 
            value='<%nvg_attr_get("itunes_passwd", "itunes_rule", "0","password");%>'
            <%nvg_attr_match("itunes_passwd","itunes_rule","0","enable","0","checked");%>>
  </div>


  <div class="setting">
    <div class="label"><%lang("iTunes Server Name");%></div>
      <input size="20" name="itunes_servername" 
            value='<%nvg_attr_get("itunes_servername", "itunes_rule", "0","servername");%>'
            <%nvg_attr_match("itunes_servername","itunes_rule","0","enable","0","checked");%>>
  </div>
</fieldset>
