<table border="0" cellpadding="0" cellspacing="0" class="box">
    <tr>
        <td class="l-header" colspan="2"><h2><% lang("WLAN"); %></h2></td>
    </tr>
    
    <tr class="top">
        <td class="l-head" width="100">Status:</td>
        <td class="bod" width="200">
    
    
         <table border="0" cellpadding="0" cellspacing="0" class="conn">
            <tr>
                <td><div id="wlan_status">&nbsp;</td>
                <td align="right"><div class="connect"><input type="button" id="wlan_connect" value="" onClick="" /></div></td>
             </tr>
         </table>
        
        </td>         
    </tr>
    
    <tr>
        <td class="l-head" width="100">Role:</td>
        <td class="bod" width="200">Access Point</td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Mode:</td>
        <td class="bod" width="200"><div id="wlan_mode"><%nv_attr_get("wl_basic_rule","0","net_mode");%></div></td>
    </tr>
    
    <tr>
    	<td class="l-head" width="100">Channel:</td>
        <td class="bod" width="200"><div id="wlan_channel"><%nv_attr_get("wl_basic_rule","0","channel");%></div></td>
    </tr>
    <% wlv_show_entry_setting("0", "st_widget_wlv.asp");%>
    
    <tr>
        <td class="l-head" width="100"># Clients:</td>
        <td class="bod" width="200"><div id="wlan_clients">&nbsp</div></td>
    </tr>
</table>

<script type="text/javascript">
	var net_mode=["<% lang("B/G Mixed"); %>", "<% lang("B only"); %>", "", "", "<% lang("G only"); %>", "", "<% lang("N only"); %>", "<% lang("G/N Mixed"); %>", "", "<% lang("B/G/N Mixed"); %>"];
	 var freq=["", "2.412", "2.417", "2.422", "2.427", "2.432", "2.437", "2.442", "2.447", "2.452", "2.457", "2.462", "2.467", "2.472", "2.484", ""];
	
        E('wlan_mode').innerHTML=net_mode[parseInt(E('wlan_mode').innerHTML)];
        E('wlan_channel').innerHTML="Channel " + E('wlan_channel').innerHTML + " [" + freq[parseInt(E('wlan_channel').innerHTML)] + "GHz]";
	
	if (  <% nv_attr_get("wl_basic_rule",0,"enable"); %>  ) {
		document.getElementById('wlan'+'_connect').value="Disable";
		document.getElementById('wlan'+'_connect').onclick=function(){to_action(0, 'wifi', 'disconnect',this);};
	}
	else {
		document.getElementById('wlan'+'_connect').value="Enable";
		document.getElementById('wlan'+'_connect').onclick=function(){to_action(0, 'wifi', 'connect',this);};
	}
</script>
