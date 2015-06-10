<div class="setting">
    <div class="label"><%lang("TCP alive check time");%></div>
    <input type="text" name="alive" value="<%nv_attr_get("als_operating_rule","$1","alive");%>"/>(0 - 99 min)
</div>
<div class="setting">
    <div class="label"><%lang("Inactivity time");%></div>
    <input type="text" name="inactive" value="<%nv_attr_get("als_operating_rule","$1","inactive");%>"/>(0 - 65535 ms)
</div>
<div class="setting" >
    <div class="label"><%lang("Connection Limit");%></div>
    <select id="max_connect" name="max_connect">
        <option value="4" <%nvg_attr_match("","als_operating_rule", "$1", "max_connect", "4", "selected")%> >4</option>
        <option value="3" <%nvg_attr_match("","als_operating_rule", "$1", "max_connect", "3", "selected")%> >3</option>
        <option value="2" <%nvg_attr_match("","als_operating_rule", "$1", "max_connect", "2", "selected")%> >2</option>
        <option value="1" <%nvg_attr_match("","als_operating_rule", "$1", "max_connect", "1", "selected")%> >1</option>
    </select>
</div>
<div class="setting">
    <div class="label"><%lang("Ignore Disconnection IP");%></div>
    <input type="radio" name="jammed" onClick="$('#jammed_ip').val('0')" <%nvg_attr_match("","als_operating_rule", "$1", "jammed_ip", "0", "checked");%>/><%lang("No");%>
    <input type="radio" name="jammed" onClick="$('#jammed_ip').val('1')" <%nvg_attr_match("","als_operating_rule", "$1", "jammed_ip", "1", "checked");%>/><%lang("Yes");%>
    <input type="hidden" id="jammed_ip" name="jammed_ip" value="<%nv_attr_get("als_operating_rule", "$1", "jammed_ip");%>"/>
</div>

<div class="setting" align="center"><%lang("TCP Server Mode");%></div>
<div class="setting">                                                         
    <div class="label"><%lang("Local TCP Port");%></div>                        
    <input type="text" name="tcp_port" value="<%nv_attr_get("als_operating_option_rule","$1","tcp_port");%>"/>
</div>           
<script type="text/javascript">$("#max_connect option[value='<%nv_attr_get("als_operating_rule", "$1" , "max_connect");%>']").attr("selected",true);</script>
