<script language="JavaScript">
dmz_objects[$0] = [ { name: 'fr_dmz$0_ipaddr' } ];
</script>
<div class="row">
    <div class="span12">
        <legend>DMZ - <%lang("WAN");%> <%increase("$0", "1");%></legend>

        <div class="row show-grid">
            <div class="span4">DMZ</div>
            <div class="span8">
                <label class="radio inline">
                    <input type="radio" value="1" name="fr_dmz$0_enable" id="fr_dmz$0_enable" onclick="init()"
                        <% nvg_attr_match("fr_dmz$0_enable","fr_dmz_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                </label>
                <label class="radio inline">
                    <input type="radio" value="0" name="fr_dmz$0_enable" id="fr_dmz$0_enable" onclick="init()"
                        <% nvg_attr_match("fr_dmz$0_enable","fr_dmz_rule","$0","enable","0","checked");%>><%lang("Disable");%>
                </label>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("DMZ IP Address"); %></div>
            <div class="span8">
                <input type="text" class="num" name="fr_dmz$0_ipaddr" id="fr_dmz$0_ipaddr" size="17" maxlength="15" value='<%nv_attr_get("fr_dmz_rule",$0,"ipaddr");%>'
                    onblur='valid_ipaddr(this, "DMZ IP Address")' />
            </div>
        </div>

    </div>
</div><!-- row -->
<br>
