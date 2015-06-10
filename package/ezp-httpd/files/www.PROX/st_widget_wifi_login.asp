<table border="0" cellpadding="0" cellspacing="0" class="box">
    <tr>
        <td class="l-header" colspan="2"><h2><% lang("WLAN"); %></h2></td>
    </tr>
    
    <tr class="top">
        <td class="l-head" width="100">Status:</td>
        <td class="bod" width="200">
    
    
         <table border="0" cellpadding="0" cellspacing="0" class="conn">
            <tr>
                <td><div id="wwan_status"><% %></td>
                <td align="right"><div class="connect" style="display:none;"><input type="button" id="wwan_connect" value="Enable" onClick="to_submit(this.form)" /></div></td>
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
        <td class="bod" width="200"><%nv_attr_get("wl_basic_rule","0","net_mode");%></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Channel:</td>
        <td class="bod" width="200"><%nv_attr_get("wl_basic_rule","0","channel");%></td>
    </tr>
    <% wlv_show_entry_setting("0", "st_widget_wlv.asp");%>
    
    <tr>
        <td class="l-head" width="100"># Clients:</td>
        <td class="bod" width="200">2</td>
    </tr>
</table>

