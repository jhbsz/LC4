<table border="0" cellpadding="0" cellspacing="0" class="box">
    <tr>
        <td class="l-header" colspan="2"><h2>LAN</h2></td>
    </tr>
    
    <tr class="top">
        <td class="l-head" width="100">IP Address:</td>
        <td class="bod" width="200"><% nv_get("lan$0_ipaddr"); %></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Subnet:</td>
        <td class="bod" width="200"><% nv_get("lan$0_mask"); %></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100"># Clients:</td>
        <td class="bod" width="200"><div id="lan_clients">&nbsp</div></td>
    </tr>
</table>