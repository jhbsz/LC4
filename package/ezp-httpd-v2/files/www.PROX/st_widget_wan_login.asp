<table border="0" cellpadding="0" cellspacing="0" class="box">
    <tr>
        <td class="l-header" colspan="2"><h2><%lang("WAN");%> <%increase_wan("$0", "1");%></h2></td>
    </tr>
    
    <tr class="top">
        <td class="l-head" width="100">Status:</td>
        <td class="bod" width="200">
            <input type='hidden' id='wan$0_status' />
            <table border="0" cellpadding="0" cellspacing="0" class="conn">
                <tr>
                    <td><div id='wan$0_light'>&nbsp;</div></td>
                    <td align="right"><div class="connect" style="display:none;"><input type="button" id="wan$0_connect" value="" onClick="" /></div></td>
                </tr>
            </table>
        </td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Type:</td>
        <td class="bod" width="200"><div id='wan$0_widget_proto'>&nbsp;</div></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">IP Address:</td>
        <td class="bod" width="200"><div id='wan$0_ipaddr'>&nbsp;</div></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Subnet:</td>
        <td class="bod" width="200"><div id='wan$0_mask'><% nv_get("wan$0_mask"); %></div></td>
    </tr>
    
    <tr>
        <td class="l-head" width="100">Up Time:</td>
        <td class="bod" width="200"><div id='wan$0_uptime'>&nbsp;</div></td>
    </tr>
</table>

<script type="text/javascript">
    if (  <% nv_attr_get("wan_main_rule",$0,"enable"); %>  ) {
        document.getElementById('wan'+$0+'_connect').value="Disable";
        document.getElementById('wan'+$0+'_connect').onclick=function(){to_action($0, 'iface', 'disconnect'); window.location.reload();};
    } 
    else { 
        document.getElementById('wan'+$0+'_connect').value="Enable";
        document.getElementById('wan'+$0+'_connect').onclick=function(){to_action($0, 'iface', 'connect'); window.location.reload();};
    }
</script>
    
    
