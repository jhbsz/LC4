<% do_pagehead1(); %>

<html>
<head>
  <title><% nv_get("model"); %> -
  Graph</title><% do_pagehead2(); %>
  <script type='text/javascript' src='svg-common.js'></script>
  <script type='text/javascript'>

  function SelInterval(F) {
        F.submit_button.value = "st_thp";
        F.action.value = "Gozila";
        F.submit();
  }
  function SelIf(F) {
        F.submit_button.value = "st_thp";
        F.action.value = "Gozila";
        F.submit();
  }
  function init()
  {
        try {
        <%st_mrtg_show("graph", "thp");%>
        } 
        catch (ex) {
                mrtg_data = {};
        }
        ifname = '<% nvg_get("stats_ifname"); %>';
        updateInt = '<% nvg_get("stats_interval"); %>';
        initSVG(1, 0);
        Refresh();
  }

  var value=0;
  function Refresh() {
        if (value) {
                window.location.replace("st_thp.asp");
        }
        value++;
        setTimeout("Refresh()",60000);
  }

var colors = ['#6495ed', '#118811'];
var ifname;

function showTab(name)
{
	var h;
	var max;
    var p = new Array(2);
	
    ifname = '<% nvg_get("stats_ifname"); %>';
	h = mrtg_data[ifname];
    /* For no ifname matched, keep the most essential struct to maintain the 
     * MRTG graph.
     */
    if (!h) {                                                                                                                  
        h = { points_0: [0], cur_0: 0, avg_0: 0, max_0: 0, total_0: 0,                                                     
              points_1: [0], cur_1: 0, avg_1: 0, max_1: 0, total_1: 0 };                                                   
    }    

    /* xxx_0 is used for recording the rx related values. */
	E('rx-current').innerHTML = xpsb(h.points_0[h.points_0.length - 1]);
	E('rx-avg').innerHTML = xpsb(h.avg_0);
	E('rx-max').innerHTML = xpsb(h.max_0);
	E('rx-total').innerHTML = xb(h.total_0);
	
    /* xxx_1 is used for recording the tx related values. */
	E('tx-current').innerHTML = xpsb(h.points_1[h.points_1.length - 1]);
	E('tx-avg').innerHTML = xpsb(h.avg_1);
	E('tx-max').innerHTML = xpsb(h.max_1);
	E('tx-total').innerHTML = xb(h.total_1);

	if (svgOK) {
		scaleMode = 1;
		max = scaleMode ? Math.max(h.max_0, h.max_1) : xx_max
        if (max > 102400) {
            max = (Math.round(max / 102400)+1) * 102400; 
        } else if (max > 10240) {
            max = (Math.round(max / 10240)+1) * 10240;
        } else {
            max = (Math.round(max / 1024)+1) * 1024;
        }               
        p[0] = h.points_0;
        p[1] = h.points_1;
		updateSVG(2, p, colors, max, drawMode, updateInt, 1)
	}
}

  </script>
</head>

<body class="gui" onload='init()'>
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Status", "Traffic"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Status");%>', '<%lang("Traffic");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
      <div id="contentsInfo">
            <form name="st_thp" action="apply.cgi" method="post">
              <input type="hidden" name="submit_button" />
              <input type="hidden" name="action" />
              <input type="hidden" name="stats_ifname" value="br0"/>

      <div class="lcol2l">
        <div class="setting">
          <span class="label"><% lang("Graph Scale"); %></span>
          <select name="stats_interval" onchange="SelInterval(this.form)">
            <option value="60" <% nvg_match("stats_interval", "60", "selected"); %>><%lang("Two Hours");%></option>
            <option value="600" <% nvg_match("stats_interval", "600", "selected"); %>><%lang("One Day");%></option>
            <option value="3600" <% nvg_match("stats_interval", "3600", "selected"); %>><%lang("One Week");%></option>
            <option value="21600" <% nvg_match("stats_interval", "21600", "selected"); %>><%lang("One Month");%></option>
          </select>
        </div>
      </div>
      <div class="lcol2r">
        <div class="setting">
          <span class="label"><% lang("Interface"); %></span>
          <select name="stats_ifname" onchange="SelIf(this.form)">
            <%st_show_if();%>
          </select>
        </div>
      </div>
      <br/><br/>
                <div id="floatKiller"></div>

                  <div align=center style='visibility:hidden;padding:0;margin:0;' id='graph'>
                    <embed src='svg-graph.svg' wmode='transparent' style='width:700px;height:300px;margin:0;padding:0;z-index:3' type='image/svg+xml' pluginspage='http://www.adobe.com/svg/viewer/install/'>
                  </div>

                  <div id="controls">
                    <small>
                      <font color=#118811><%lang("Upload");%></font>&nbsp;&nbsp;
                      <font color=#6495ed><%lang("Download");%></font>
                      <br/>
                      <br/>
                      (<%lang("SVG plugin is required to display the graphs");%>)
                      [<a href="http://www.adobe.com/svg/viewer/install/main.html"><%lang("Download");%></a>]
                    </small>
                  </div><br>
                  <table><tr><td>
                  <div class="lcol2l">
                  <fieldset>
                  <legend><%lang("Download");%></legend>
                    <div class="setting">
                        <div class="label">
                        <%lang("Current");%>
                        </div><div id="rx-current"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Max");%> (<%st_show_period();%>)
                        </div><div id="rx-max"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Averaged");%> (<%st_show_period();%>)
                        </div><div id="rx-avg"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Total Traffic");%> (<%st_show_period();%>)
                        </div><div id="rx-total"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td><td>

                  <div class="lcol2r">
                  <fieldset>
                  <legend><%lang("Upload");%></legend>
                    <div class="setting">
                        <div class="label">
                        <%lang("Current");%>
                        </div><div id="tx-current"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Max");%> (<%st_show_period();%>)
                        </div><div id="tx-max"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Averaged");%> (<%st_show_period();%>)
                        </div><div id="tx-avg"></div>
                    </div>
                    <div class="setting">
                        <div class="label">
                        <%lang("Total Traffic");%> (<%st_show_period();%>)
                        </div><div id="tx-total"></div>
                    </div>
                  </fieldset>
                  </div>
                  </td></tr></table>
                <div id="floatKiller"></div>
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
