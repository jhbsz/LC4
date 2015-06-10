<% do_pagehead1(); %>

<html>
<head>
  <title><% nv_get("model"); %> -
  Graph</title><% do_pagehead2(); %>
  <script type='text/javascript' src='svg-common.js'>
</script>
  <script type='text/javascript'>

  function SelInterval(F) {
        F.submit_button.value = "st_ct";
        F.action.value = "Gozila";
        F.submit();
  }
  function init()
  {
        try {
        <%st_mrtg_show("graph", "ct");%>
        } 
        catch (ex) {
                mrtg_data = {};
        }
        updateInt = '<% nvg_get("stats_interval"); %>';
        initSVG(1, 0);
        Refresh();
  }

  var value=0;
  function Refresh() {
        if (value) {
                window.location.replace("st_ct.asp");
        }
        value++;
        setTimeout("Refresh()",60000);
  }

var colors = ['#6495ed', '#118811', '#cf0000', '#b8860b', '#ff0000', '#00ff00', '#0000ff'];                                                          

function showTab(name)
{
    var h, i;
    var max = 0;
    var p = new Array(2);
  
    E('total-current').innerHTML = mrtg_data['total'].cur;
    E('total-max-seen-life-time').innerHTML = mrtg_data['total'].lifetime_max;
    E('total-max-seen').innerHTML = mrtg_data['total'].max;
    E('total-opened').innerHTML = mrtg_data['total'].opened;
    E('total-closed').innerHTML = mrtg_data['total'].opened -
                                     mrtg_data['total'].cur;
    E('tcp-current').innerHTML = mrtg_data['tcp'].cur;
    E('tcp-max-seen-life-time').innerHTML = mrtg_data['tcp'].lifetime_max;
    E('tcp-max-seen').innerHTML = mrtg_data['tcp'].max;
    E('tcp-opened').innerHTML = mrtg_data['tcp'].opened;
    E('tcp-closed').innerHTML = mrtg_data['tcp'].opened -
                                     mrtg_data['tcp'].cur;
    E('udp-current').innerHTML = mrtg_data['udp'].cur;
    E('udp-max-seen-life-time').innerHTML = mrtg_data['udp'].lifetime_max;
    E('udp-max-seen').innerHTML = mrtg_data['udp'].max;
    E('udp-opened').innerHTML = mrtg_data['udp'].opened;
    E('udp-closed').innerHTML = mrtg_data['udp'].opened -
                                     mrtg_data['udp'].cur;
    E('icmp-current').innerHTML = mrtg_data['icmp'].cur;
    E('icmp-max-seen-life-time').innerHTML = mrtg_data['icmp'].lifetime_max;
    E('icmp-max-seen').innerHTML = mrtg_data['icmp'].max;
    E('icmp-opened').innerHTML = mrtg_data['icmp'].opened;
    E('icmp-closed').innerHTML = mrtg_data['icmp'].opened -
                                     mrtg_data['icmp'].cur;
    if (svgOK) {
        for (i = 0; i < names.length; i++) {
            h = mrtg_data[names[i]];
            if (!h) return;

            max = (h.max > max) ? h.max : max;
            /* The numbers of living sessions are put in h.points_1 */
            p[i] = h.points_1;
        }
    
        if (max > 10000) {                             
            max = (Math.round(max / 1000)+1) * 1000;
        } else if (max > 1000) {
            max = (Math.round(max / 200)+1) * 200;
        } else if (max > 200) {
            max = (Math.round(max / 50)+1) * 50;
        } else if (max > 50) {
            max = (Math.round(max / 10)+1) * 10;
        } else {                                     
            max = 50;
        }       
    
        /* Configure the polylines' attributes */
        setAttribute('poly1', 'stroke-dasharray', '1, 1.5');
        setAttribute('poly2', 'stroke-dasharray', '1, 1.5');
        setAttribute('poly3', 'stroke-dasharray', '1, 1.5');

        updateSVG(names.length, p, colors, max, drawMode, updateInt, 0);
    }
}

  </script>
</head>

<body class="gui" onload='init()'>
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Status", "Session"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Status");%>', '<%lang("Session");%>', 
	  				              ' (<%lang("Max Allowed");%>: <%nvg_get("ct_max");%>)')
	  </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
      <div id="contentsInfo">
            <form name="st_ct" action="apply.cgi" method="post">
              <input type="hidden" name="submit_button" />
              <input type="hidden" name="action" />
              <input type="hidden" name="stats_proto" value="all"/>

  <div class="setting">
    <div class="label"><% lang("Graph Scale"); %></div>
    <select name="stats_interval" onchange="SelInterval(this.form)">
      <option value="60" <% nvg_match("stats_interval", "60", "selected"); %>><%lang("Two Hours");%></option>
      <option value="600" <% nvg_match("stats_interval", "600", "selected"); %>><%lang("One Day");%></option>
      <option value="3600" <% nvg_match("stats_interval", "3600", "selected"); %>><%lang("One Week");%></option>
      <option value="21600" <% nvg_match("stats_interval", "21600", "selected"); %>><%lang("One Month");%></option>
    </select>
  </div>
                <div id="floatKiller"></div>

                  <div align=center style='visibility:hidden;padding:0;margin:0' id='graph'>
                    <embed src='svg-graph.svg' wmode='transparent' style='width:700px;height:300px;margin:0;padding:0' type='image/svg+xml' pluginspage='http://www.adobe.com/svg/viewer/install/'>
                  </div>

                  <div id="controls">
                    <small>
                      <font color=#6495ed><%lang("Total Sessions");%></font>&nbsp;&nbsp;
                      <font color=#118811><%lang("TCP Sessions");%></font>&nbsp;&nbsp;
                      <font color=#cf0000><%lang("UDP Sessions");%></font>&nbsp;&nbsp;
                      <font color=#b8860b><%lang("ICMP Sessions");%></font>&nbsp;&nbsp;
                      <br/>
                      <br/>
                      (<%lang("SVG plugin is required to display the graphs");%>)
                      [<a href="http://www.adobe.com/svg/viewer/install/main.html"><%lang("Download");%></a>]
                    </small>
                  </div><br>
          <table><tr><td>
                  <div class="lcol2l">
                  <fieldset>
                  <legend><%lang("Total Sessions");%></legend>
                    <div class="setting">
                        <div class="label"><%lang("Current");%></div>
                        <div id="total-current"></div>
                    </div>
                    <div class="setting">
                      <div class="label">
                        <%lang("Max Seen");%> (<%lang("Since Boot");%>)
                      </div>
                      <div id="total-max-seen-life-time"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Max Seen");%> (<%st_show_period());%>)
                    </div>
                    <div id="total-max-seen"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Opened");%> (<%st_show_period());%>)
                    </div>
                    <div id="total-opened"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Closed");%> (<%st_show_period());%>)
                    </div>
                    <div id="total-closed"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td><td>
                  <div class="lcol2r">
                  <fieldset>
                  <legend><%lang("TCP Sessions");%></legend>
                    <div class="setting">
                        <div class="label"><%lang("Current");%></div>
                        <div id="tcp-current"></div>
                    </div>
                    <div class="setting">
                      <div class="label">
                        <%lang("Max Seen");%> (<%lang("Since Boot");%>)
                      </div>
                      <div id="tcp-max-seen-life-time"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                          <%lang("Max Seen");%> (<%st_show_period());%>)
                        </div>
                        <div id="tcp-max-seen"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Opened");%> (<%st_show_period());%>)
                    </div>
                    <div id="tcp-opened"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Closed");%> (<%st_show_period());%>)
                    </div>
                    <div id="tcp-closed"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td></tr><tr><td>
                  <div class="lcol2l">
                  <fieldset>
                  <legend><%lang("UDP Sessions");%></legend>
                    <div class="setting">
                        <div class="label"><%lang("Current");%></div>
                        <div id="udp-current"></div>
                    </div>
                    <div class="setting">
                      <div class="label">
                        <%lang("Max Seen");%> (<%lang("Since Boot");%>)
                      </div>
                      <div id="udp-max-seen-life-time"></div>
                    </div>
                    <div class="setting">
                      <div class="label">
                        <%lang("Max Seen");%> (<%st_show_period());%>)
                      </div>
                      <div id="udp-max-seen"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Opened");%> (<%st_show_period());%>)
                    </div>
                    <div id="udp-opened"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Closed");%> (<%st_show_period());%>)
                    </div>
                    <div id="udp-closed"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td><td>
                  <div class="lcol2r">
                  <fieldset>
                  <legend><%lang("ICMP Sessions");%></legend>
                    <div class="setting">
                        <div class="label"><%lang("Current");%></div>
                        <div id="icmp-current"></div>
                    </div>
                    <div class="setting">
                 <div class="label">
                        <%lang("Max Seen");%> (<%lang("Since Boot");%>)
                      </div>
                      <div id="icmp-max-seen-life-time"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                          <%lang("Max Seen");%> (<%st_show_period());%>)
                        </div>
                        <div id="icmp-max-seen"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Opened");%> (<%st_show_period());%>)
                    </div>
                    <div id="icmp-opened"></div>
                    </div>
                    <div class="setting">
                    <div class="label">
                      <%lang("Closed");%> (<%st_show_period());%>)
                    </div>
                    <div id="icmp-closed"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td></tr></table>
                <div id="floatKiller"></div>
               <br/>
             </form>
           </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
  </div>
</body>
</html>
