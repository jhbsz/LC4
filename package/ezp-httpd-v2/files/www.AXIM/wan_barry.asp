
    <div class="row show-grid">
        <div class="span4"><%lang("BlackBerry Password");%></div>
        <div class="span8">
            <input name="wan$0_barry_barrypasswd" id='wan$0_barry_barrypasswd' size="25" maxLength="36" onBlur="valid_name(this,'BarryPassword',SPACE_NO)" 
                type="password" value="slide" />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("APN Type");%></div>
        <div class="span8">
            <label class="radio inline">
                <input type="radio" value="0" name="wan$0_barry_apn_type" id="wan$0_barry_apn_type"
                  onclick='sel_barry_apn_type("0","$0")'
                  <%nvg_attr_match("wan$0_barry_apn_type","wan_barry_rule","$0","apn_type","0","checked");%>>
                  <%lang("Service Provider");%> 
            </label>
            <label class="radio inline">
                <input type="radio" value="1" name="wan$0_barry_apn_type" id="wan$0_barry_apn_type"
                  onclick='sel_barry_apn_type("1","$0")'
                  <%nvg_attr_match("wan$0_barry_apn_type","wan_barry_rule","$0","apn_type","1","checked");%>>
                  <%lang("Manual");%>
            </label>
        </div>
    </div>
 <%wan_barry_show_isp("$0");%>

    <div class="row show-grid">
        <div class="span4"><%lang("Access Point Name");%> (APN)</div>
        <div class="span8">
            <input type="text" name="wan$0_barry_apn" id='wan$0_barry_apn' size="25" maxLength="36" onBlur="valid_name(this,'APN')" 
                value='<%nvg_attr_get("wan$0_barry_apn", "wan_barry_rule", "$0", "apn");%>' />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("User Name");%></div>
        <div class="span8">
            <input type="text" name="wan$0_barry_username" id='wan$0_barry_username' size="25" maxLength="36" onBlur="valid_name(this,'User Name')" 
                value='<%nvg_attr_get("wan$0_barry_username", "wan_barry_rule", "$0", "username");%>' />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("Password");%></div>
        <div class="span8">
            <input name="wan$0_barry_passwd" id='wan$0_barry_passwd' size="25" maxLength="36" onBlur="valid_name(this,'Password',SPACE_NO)" 
                type="password" value="slide" />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("Dial Number");%></div>
        <div class="span8">
            <input type="text" name="wan$0_barry_dialstr" id='wan$0_barry_dialstr' size="25" maxLength="30" onBlur="valid_name(this,'Dial Number')" 
                value='<%nvg_attr_get("wan$0_barry_dialstr", "wan_barry_rule", "$0", "dialstr");%>' />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("TurboLink");%> (<%lang("This might increase data charge");%>)</div>
        <div class="span8">
            <label class="radio inline">
                <input type="radio" name="wan$0_barry_turbolink" id="wan$0_barry_turbolink"
                  value="1" <%nvg_attr_match("wan$0_barry_turbolink","wan_barry_rule","$0","turbolink","1","checked");%>/>
                  <%lang("Enable");%>
            </label>
            <label class="radio inline">
                <input type="radio" name="wan$0_barry_turbolink" id="wan$0_barry_turbolink"
                  value="0" <%nvg_attr_match("wan$0_barry_turbolink","wan_barry_rule","$0","turbolink","0","checked");%>/>
                  <%lang("Disable");%>
            </label>
        </div>
    </div>

  <input type="hidden" name="wan$0_mtu" value="1500" />
