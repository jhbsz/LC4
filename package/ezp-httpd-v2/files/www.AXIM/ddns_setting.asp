<script language="JavaScript">
ddns_objects[$0] =
[
  { name: 'ddns$0_type' },
  { name: 'ddns$0_username' },
  { name: 'ddns$0_passwd' },
  { name: 'ddns$0_hostname' },
  { name: 'ddns$0_server' },
  { name: 'ddns$0_update' }
];
ddns_type_objects[$0] =
[
  { name: 'ddns$0_server' }
];
ddns_update_objects[$0] =
[
  { name: 'ddns$0_update' }
];
</script>
<input type="hidden" name="ddns$0_type_org" id="ddns$0_type_org" 
value='<% nvg_attr_get("ddns$0_type_org","wan_ddns_rule","$0","type");%>'>
<input type="hidden" name="ddns$0_username_org" id="ddns$0_username_org" 
value='<% nvg_attr_get("ddns$0_username_org","wan_ddns_rule","$0","username");%>'>
<input type="hidden" name="ddns$0_passwd_org" id="ddns$0_passwd_org" 
value='<% nvg_attr_get("ddns$0_passwd_org","wan_ddns_rule","$0","passwd");%>'>
<input type="hidden" name="ddns$0_hostname_org" id="ddns$0_hostname_org" 
value='<% nvg_attr_get("ddns$0_hostname_org","wan_ddns_rule","$0","hostname");%>'>
<input type="hidden" name="ddns$0_server_org" id="ddns$0_server_org" 
value='<% nvg_attr_get("ddns$0_server_org","wan_ddns_rule","$0","server");%>'>


<div class="row">
    <div class="span12">
        <legend><%lang("Dynamic Domain Name Service");%> - <%lang("WAN");%> <%increase("$0", "1");%></legend>

        <div class="row show-grid">
            <div class="span4"><%lang("DDNS Service");%></div>
            <div class="span8">
                <label class="radio inline">
                    <input type="radio" value="1" name="ddns$0_enable" id="ddns$0_enable" onclick="init()"
                        <%nvg_attr_match("ddns$0_enable","wan_ddns_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                </label>
                <label class="radio inline">
                    <input type="radio" value="0" name="ddns$0_enable" id="ddns$0_enable" onclick="init()" 
                        <%nvg_attr_match("ddns$0_enable","wan_ddns_rule","$0","enable","0","checked");%>><%lang("Disable");%>
                </label>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("DDNS Type");%></div>
            <div class="span8">
                <select name="ddns$0_type" id="ddns$0_type" onchange="init(this.form)">
                    <option value="dyndns" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","dyndns","selected");%>>DynDNS.org</option>
                    <option value="tzo" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","tzo","selected");%>>TZO.com</option>
                    <option value="zoneedit" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","zoneedit","selected");%>>ZoneEdit.com</option>
                    <option value="changeip" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","changeip","selected");%>>ChangeIP.com</option>
                    <option value="eurodns" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","eurodns","selected");%>>EuroDNS.com</option>
                    <option value="noip" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","noip","selected");%>>NoIP.com</option>
                    <option value="ods" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","ods","selected");%>>ODS.org</option>
                    <option value="ovh" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","ovh","selected");%>>OVH.co.uk</option>
                    <option value="regfish" <%nvg_attr_match("ddns$0_type","wan_ddns_rule","$0","type","regfish","selected");%>>Regfish.com</option>
                </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("User Name");%></div>
            <div class="span8">
                <input type="text" name="ddns$0_username" id="ddns$0_username" size="25" maxlength="32" onblur="valid_name(this,'User Name')" 
                    value='<%nvg_attr_get("ddns$0_username","wan_ddns_rule","$0","username");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Password");%></div>
            <div class="span8">
                <input type="password" name="ddns$0_passwd" id="ddns$0_passwd" size="25" maxlength="32" 
                    onblur="valid_name(this,'Password')" value='<%nvg_attr_get("ddns$0_passwd","wan_ddns_rule","$0","passwd");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Host Name");%></div>
            <div class="span8">
                <input type="text" name="ddns$0_hostname" id="ddns$0_hostname" size="25" maxlength="128" 
                    onblur= "valid_name(this,'Host Name')" value='<%nvg_attr_get("ddns$0_hostname","wan_ddns_rule","$0","hostname");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Custom Server(Optional)");%></div>
            <div class="span8">
                <input type="text" name="ddns$0_server" id="ddns$0_server" size="25" maxlength="128"
                    onblur= "valid_name(this,'Host Name')" value='<%nvg_attr_get("ddns$0_server","wan_ddns_rule","$0","server");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Action");%></div>
            <div class="span8">
                <input type="button" class="btn" name="ddns$0_update" id="ddns$0_update" onclick="to_update($0, 'ddns')" value="<%lang("Update");%>" />
            </div>
        </div>

    </div>
</div><!-- row -->
<br>
