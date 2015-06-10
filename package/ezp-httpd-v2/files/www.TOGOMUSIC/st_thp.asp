<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Graph</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <link rel='stylesheet' type='text/css' href='ext/css/jquery.jqplot.css' />
    <script type='text/javascript' src='svg-common.js'></script>
    <!--[if lt IE 9]><script language='javascript' type='text/javascript' src='ext/js/jqPlot/excanvas.js'></script><![endif]-->
    <script language='javascript' type='text/javascript' src='ext/js/date.js'></script>
    <script language='javascript' type='text/javascript' src='ext/js/jqPlot/jquery.jqplot.min.js'></script>
    <script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.highlighter.min.js'></script>
    <script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.cursor.min.js'></script>
    <script language='javascript' type='text/javascript' src='ext/js/jqPlot/plugins/jqplot.dateAxisRenderer.min.js'></script>

    <script type="text/javascript">

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

        function init(){
            try {
            <%st_mrtg_show("graph", "thp");%>
                var colors = ['#6495ed', '#118811'];
                var ifname;

                var Interval_Sec = $('select#stats_interval option:selected').val();

                ifname = '<% nvg_get("stats_ifname"); %>';
                h = mrtg_data[ifname];


                var NowTime_0 = Date.parse("now");
                var DataArray_0 = new Array();
                var X_DATA_0 = new Array();
                var Y_DATA_0 = new Array();

                var StartTime_0 = NowTime_0.add({seconds: -1 * Interval_Sec * h.points_0.length });
                
                for (i=0;i<h.points_0.length;i++ ){
                    Tmp_Time_0 = StartTime_0.add({seconds: Interval_Sec});
                    X_DATA_0_Tmp = Tmp_Time_0.toString("yyyy-MM-dd HH:mm:ss"); 
                    Y_DATA_0_Tmp =  h.points_0[i]/1024;
                    X_DATA_0[i] = X_DATA_0_Tmp;
                    Y_DATA_0[i] = Y_DATA_0_Tmp;
                    DataArray_0[i] = [ X_DATA_0_Tmp, Y_DATA_0_Tmp];
                }//End for
                
                DateTime_Min = X_DATA_0[0];
                DateTime_Max = X_DATA_0[h.points_0.length-1];

                var Y_DATA_0_Min = Math.min.apply(Math, Y_DATA_0);
                var Y_DATA_0_Max = Math.max.apply(Math, Y_DATA_0);

                var NowTime_1 = Date.parse("now");
                var DataArray_1=new Array();
                var X_DATA_1 = new Array();
                var Y_DATA_1 = new Array();

                var StartTime_1 = NowTime_1.add({seconds: -1 * Interval_Sec * h.points_1.length });

                for (i=0;i<h.points_1.length;i++ ){
                    Tmp_Time_1 = StartTime_1.add({seconds: Interval_Sec});
                    X_DATA_1_Tmp = Tmp_Time_1.toString("yyyy-MM-dd HH:mm:ss");
                    Y_DATA_1_Tmp = h.points_1[i]/1024;
                    X_DATA_1[i] = X_DATA_1_Tmp;
                    Y_DATA_1[i] = Y_DATA_1_Tmp;
                    DataArray_1[i] = [X_DATA_1_Tmp, Y_DATA_1_Tmp];
                }//End for

                var Y_DATA_1_Min = Math.min.apply(Math, Y_DATA_1);
                var Y_DATA_1_Max = Math.max.apply(Math, Y_DATA_1);

                var Y_DATA_Min = Math.min(Y_DATA_1_Min,Y_DATA_0_Min);
                var Y_DATA_Max = Math.max(Y_DATA_1_Max,Y_DATA_0_Max);

                var plot1 = $.jqplot('graph', [DataArray_0,DataArray_1], {
                    //title:'Data Point Highlighting',
                    seriesColors: colors,				
                    series:[ 
                        {
                            showMarker:false,
                            lineWidth:2, 
                        }, 
                        {
                            showMarker:false,
                            lineWidth:2, 
                        }
                    ],

                    axes:{
                        xaxis:{
                            renderer:$.jqplot.DateAxisRenderer,
                            min:DateTime_Min,
                            max:DateTime_Max,
                            tickOptions:{show: false, formatString:'%m/%d %X'},
                            //tickInterval:'6 hour'
                        },
                        yaxis:{
                            min:Y_DATA_Min,
                            max:Y_DATA_Max,
                            tickOptions:{formatString:'%.2f KB/s'}
                            //tickInterval:'0.1'
                        }
                    },
                    highlighter: {
                        show: false,
                        sizeAdjust: 7.5
                    },
                    cursor: {
                        show: true,
                        tooltipLocation:'sw'
                    }
                }); //End plot1
                /* xxx_0 is used for recording the rx related values. */
                $("#rx-current").html(xpsb(h.points_0[h.points_0.length - 1]));
                $("#rx-avg").html(xpsb(h.avg_0));
                $("#rx-max").html(xpsb(h.max_0));
                $("#rx-total").html(xb(h.total_0));

                /* xxx_1 is used for recording the tx related values. */
                $("#tx-current").html(xpsb(h.points_1[h.points_1.length - 1]));
                $("#tx-avg").html(xpsb(h.avg_1));
                $("#tx-max").html(xpsb(h.max_1));
                $("#tx-total").html(xb(h.total_1));

            } 
            catch (ex) {
                    mrtg_data = {};
            }
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

        $(document).ready(function () {
            init();
        });

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
              <input type="hidden" name="submit_button" />
              <input type="hidden" name="action" />
              <input type="hidden" name="stats_ifname" value="br0"/>

                <h2><%lang("Status");%>-<%lang("Traffic");%></h2>

        <div class="row">
            <div class="span12">

                <div class="row show-grid">
                    <div class="span4"><% lang("Graph Scale"); %></div>
                    <div class="span8">
                         <select name="stats_interval" id="stats_interval" onchange="SelInterval(this.form)">
                            <option value="60" <% nvg_match("stats_interval", "60", "selected"); %>><%lang("Two Hours");%></option>
                            <option value="600" <% nvg_match("stats_interval", "600", "selected"); %>><%lang("One Day");%></option>
                            <option value="3600" <% nvg_match("stats_interval", "3600", "selected"); %>><%lang("One Week");%></option>
                            <option value="21600" <% nvg_match("stats_interval", "21600", "selected"); %>><%lang("One Month");%></option>
                         </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Interface"); %></div>
                    <div class="span8">
                        <select name="stats_ifname" onchange="SelIf(this.form)">
                            <%st_show_if();%>
                        </select>
                    </div>
                </div>
        
            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="graph"></div>
                <br>
                 <small>
                      <font color=#118811><%lang("Upload");%></font>&nbsp;&nbsp;
                      <font color=#6495ed><%lang("Download");%></font>
                 </small>
            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span6">
                <legend><%lang("Download");%></legend>
                <div class="row show-grid">
                    <div class="span3"><%lang("Current");%></div>
                    <div class="span3"><div id="rx-current"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Max");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="rx-max"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Averaged");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="rx-avg"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Total Traffic");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="rx-total"></div></div>
                </div>
            
            </div>

            <div class="span6">
                <legend><%lang("Upload");%></legend>
                <div class="row show-grid">
                    <div class="span3"><%lang("Current");%></div>
                    <div class="span3"><div id="tx-current"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Max");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="tx-max"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Averaged");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="tx-avg"></div></div>
                </div>

                <div class="row show-grid">
                    <div class="span3"><%lang("Total Traffic");%> (<%st_show_period();%>)</div>
                    <div class="span3"><div id="tx-total"></div></div>
                </div>

            </div>

        </div><!-- row -->

        </form>
        
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>