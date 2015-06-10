<div class="setting" align="center"><%lang("UDP Mode");%></div>
<div class="setting">                                                                       
    <div class="label"><%lang("Destination IP address 1");%></div>
    <input type="text" name="ip_1" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_1");%>"/>
    <input type="text" name="ip_port_1" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_1");%>"/>
</div>                               
<div class="setting">
    <div class="label"><%lang("Destination IP address 2");%></div>
    <input type="text" name="ip_2" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_2");%>"/>
    <input type="text" name="ip_port_2" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_2");%>"/>
</div>                               
<div class="setting">
    <div class="label"><%lang("Destination IP address 3");%></div>
    <input type="text" name="ip_3" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_3");%>"/>
    <input type="text" name="ip_port_3" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_3");%>"/>
</div>                               
<div class="setting">
    <div class="label"><%lang("Destination IP address 4");%></div>
    <input type="text" name="ip_4" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_4");%>"/>
    <input type="text" name="ip_port_4" value="<%nv_attr_get("als_operating_option_rule", "$1", "ip_port_4");%>"/>
</div>                               
<div class="setting">                                             
    <div class="label"><%lang("Local Listen port");%></div>
    <input type="text" name="udp_port" value="<%nv_attr_get("als_operating_option_rule", "$1", "udp_port");%>"/>
</div>      
