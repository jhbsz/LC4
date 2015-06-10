<tr>
    <td class="l-head" width="100">SSID<%increase("$1", "1");%>:</td>
    <td class="bod" width="200"><%nv_attr_get("wl$0_ssid_rule","$1", "ssid");%>
    
    <script type="text/javascript">
    if (!<%nv_attr_get("wl_basic_rule","$0", "enable");%>) {
      document.write('(Disabled)');
    } 
    else {  
    	if (!<%nv_attr_get("wl$0_basic_rule","$1", "enable");%>) {
      	  document.write('(Disabled)');
      	}
    } 
    </script>
    
    </td>
</tr>

<tr>
    <td class="l-head" width="100">Security<%increase("$1", "1");%>:</td>
    <td class="bod" width="200"><div id='wl$0_security$1'>&nbsp;</div></td>
</tr>
    
