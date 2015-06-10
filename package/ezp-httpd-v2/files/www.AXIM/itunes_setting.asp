
<div class="row">
    <div class="span12">
        <legend><%lang("iTunes");%></legend>
        <div class="row show-grid">
            <div class="span4"><%lang("iTunes Server");%></div>
            <div class="span8">
                <label class="radio inline">
                    <input type="radio" value="1" name="enable" onclick="init()"
                          <%nvg_attr_match("enable","itunes_rule","0","enable","1","checked");%>><%lang("Enable");%>
                </label>
                <label class="radio inline">
                    <input type="radio" value="0" name="enable" onclick="init()"
                          <%nvg_attr_match("enable","itunes_rule","0","enable","0","checked");%>><%lang("Disable");%>
                </label>
            </div>
        </div>
        <div class="row show-grid">
            <div class="span4"><%lang("iTunes Password");%></div>
            <div class="span8">
                <input type="text" size="20" name="itunes_passwd" id="itunes_passwd" 
                    value='<%nvg_attr_get("itunes_passwd", "itunes_rule", "0","password");%>'
                    <%nvg_attr_match("itunes_passwd","itunes_rule","0","enable","0","checked");%>>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("iTunes Server Name");%></div>
            <div class="span8">
                <input type="text" size="20" name="itunes_servername" id="itunes_servername" 
                    value='<%nvg_attr_get("itunes_servername", "itunes_rule", "0","servername");%>'
                    <%nvg_attr_match("itunes_servername","itunes_rule","0","enable","0","checked");%>>
            </div>
        </div>
    </div>
</div><!-- row -->
<br>
