<table border="0" cellpadding="0" cellspacing="0" class="box">
    <tr>
        <td class="l-header" colspan="2"><h2>STATUS</h2></td>
    </tr>
    
    <tr class="top">
        <td class="l-head" width="100">System Time:</td>
        <td class="bod" width="200"><div id='local_time'><% show_localtime(); %></div></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Up Time:</td>
        <td class="bod" width="200"><div id='system_uptime'><% show_uptime(); %></div></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Firmware:</td>
        <td class="bod" width="200"><% show_version("custom"); %></td>
    </tr>
</table>