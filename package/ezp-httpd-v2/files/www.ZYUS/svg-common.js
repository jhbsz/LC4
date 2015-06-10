
var tabs = [];
var rx_max, rx_avg;
var tx_max, tx_avg;
var xx_max = 0;
var htmlOK = 0;
var svgOK = 0;
var updating = 0;
var drawMode = 0;
var scaleMode = 0;
var drawColor = 0;
var colorX = 0;

function fixInt(n, min, max, def)
{
    if (n === null) return def;
        n *= 1;
    if (isNaN(n)) return def;
    if (n < min) return min;
    if (n > max) return max;
    return n;
}
                        
function xpsb(n)
{
	return (n / 1024).toFixed(2) + ' KB/s (' + (n / 128).toFixed(2) + ' Kbit/s)';
}

function xb(n)
{
    if (n < 1024 * 1024)
        return (n / 1024).toFixed(2) + ' KB';
    else if (n < 1024 * 1024 * 1024)
        return (n / (1024 * 1024)).toFixed(2) + ' MB';
    else 
        return (n / (1024 * 1024 * 1024)).toFixed(2) + ' GB';
}

function loadSVGData()
{
	var old;
	var t, e;
	var name;
	var i;
	var changed;

	xx_max = 0;
	old = tabs;
	tabs = [];

	if (!mrtg_data) {
		mrtg_data = {};
	}
	else {
		for (var i in mrtg_data) {
			var h = mrtg_data[i];
            if ((typeof(h.points_0) == 'undefined') ||
                (typeof(h.points_1) == 'undefined')) {
            	delete mrtg_data[i];
            	continue;
            }

			tabs.push(['speed-tab-' + i, t]);
		}

		tabs = tabs.sort(
			function(a, b) {
				if (a[1] < b[1]) return -1;
				if (a[1] > b[1]) return 1;
				return 0;
			}
        );
	}

	if (tabs.length == old.length) {
		for (i = tabs.length - 1; i >= 0; --i)
			if (tabs[i][0] != old[i][0]) break;
		changed = i > 0;
	}
	else changed = 1;

	showTab(tabs[0]);
}

function initSVGData()
{
	if (htmlOK) {
		loadSVGData();
		if (svgOK) {
			E('graph').style.visibility = 'visible';
		}
	}
}

function initSVG(defDrawMode, defDrawColor, defScaleMode)
{
	drawMode = defDrawMode;
    drawColor = defDrawColor;
	scaleMode = defScaleMode;
	htmlOK = 1;
	initSVGData();	
}
