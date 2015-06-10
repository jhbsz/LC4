<div class="setting">
    <div class="label"><%lang("TCP alive check time");%></div>
    <input type="text" name="alive" value="<%nv_attr_get("als_operating_rule", "$1", "alive");%>"/>(0 - 99 min)
</div>
<div class="setting">
    <div class="label"><%lang("Inactivity time");%></div>
    <input type="text" name="inactive" value="<%nv_attr_get("als_operating_rule", "$1", "inactive");%>"/>(0 - 65535 ms)
</div>
<div class="setting">
    <div class="label"><%lang("Ignore Disconnection IP");%></div>
    <input type="radio" name="jammed" onClick='$("#jammed_ip").val("1");' <%nvg_attr_match("jammed","als_operating_rule", "$1", "jammed_ip", "1", "checked");%>/><%lang("Yes");%>
    <input type="radio" name="jammed" onClick='$("#jammed_ip").val("0");' <%nvg_attr_match("jammed","als_operating_rule", "$1", "jammed_ip", "0", "checked");%>/><%lang("No");%>
</div>

<input type="hidden" id="jammed_ip" name="jammed_ip" value="<%nv_attr_get("als_operating_rule", "$1", "jammed_ip");%>"/>

<div class="setting" align="center"><%lang("TCP Client Mode");%></div>
<div class="setting" ><table border=0><td width="40%"></td><td width="24%"><%lang("Destination IP Adress");%></td><td><%lang("Port");%></td></table></div>
<div class="setting">                                                               
    <div class="label"><%lang("Destination IP address 1");%></div>                  
    <input type="text" name="ip_1" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_1");%>"/>:
    <input type="text" name="ip_port_1" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_1");%>"/>
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination IP address 2");%></div>                  
    <input type="text" name="ip_2" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_2");%>"/>:
    <input type="text" name="ip_port_2" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_2");%>"/>
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination IP address 3");%></div>                  
    <input type="text" name="ip_3" value="<%nv_attr_get("als_operating_option_rule", "$1","ip_3");%>"/>:
    <input type="text" name="ip_port_3" value="<%nv_attr_get("als_operating_option_rule", "$1","ip_port_3");%>"/>
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination IP address 4");%></div>                  
    <input type="text" name="ip_4" value="<%nv_attr_get("als_operating_option_rule", "$1","ip_4");%>"/>:
    <input type="text" name="ip_port_4" value="<%nv_attr_get("als_operating_option_rule", "$1","ip_port_4");%>"/>
</div>                                                       
<div class="setting">                                                               
    <div class="label"><%lang("Destination Local Port 1");%></div>                  
    <input type="text" name="port_1" value="<%nv_attr_get("als_operating_option_rule", "$1","port_1");%>"/>( 0 - 65535)( <%lang("0 represents assigned automatically.");%>)
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination Local Port 2");%></div>                  
    <input type="text" name="port_2" value="<%nv_attr_get("als_operating_option_rule", "$1","port_2");%>"/>( 0 - 65535)
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination Local Port 3");%></div>                  
    <input type="text" name="port_3" value="<%nv_attr_get("als_operating_option_rule", "$1","port_3");%>"/>( 0 - 65535)
</div>                                                                              
<div class="setting">                                                               
    <div class="label"><%lang("Destination Local Port 4");%></div>                  
    <input type="text" name="port_4" value="<%nv_attr_get("als_operating_option_rule", "$1","port_4");%>"/>( 0 - 65535)
</div>                                           
