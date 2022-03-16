/*
 * Date Format 1.2.3
 * (c) 2007-2009 Steven Levithan <stevenlevithan.com>
 * MIT license
 *
 * Includes enhancements by Scott Trenda <scott.trenda.net>
 * and Kris Kowal <cixar.com/~kris.kowal/>
 *
 * Accepts a date, a mask, or a date and a mask.
 * Returns a formatted version of the given date.
 * The date defaults to the current date/time.
 * The mask defaults to dateFormat.masks.default.
 */

var dateFormat = function () {
    var token = /d{1,4}|m{1,4}|yy(?:yy)?|([HhMsTt])\1?|[LloSZ]|"[^"]*"|'[^']*'/g,
        timezone = /\b(?:[PMCEA][SDP]T|(?:Pacific|Mountain|Central|Eastern|Atlantic) (?:Standard|Daylight|Prevailing) Time|(?:GMT|UTC)(?:[-+]\d{4})?)\b/g,
        timezoneClip = /[^-+\dA-Z]/g,
        pad = function (val, len) {
            val = String(val);
            len = len || 2;
            while (val.length < len) val = "0" + val;
            return val;
        };

    // Regexes and supporting functions are cached through closure
    return function (date, mask, utc) {
        var dF = dateFormat;

        // You can't provide utc if you skip other args (use the "UTC:" mask prefix)
        if (arguments.length == 1 && Object.prototype.toString.call(date) == "[object String]" && !/\d/.test(date)) {
            mask = date;
            date = undefined;
        }

        // Passing date through Date applies Date.parse, if necessary
        date = date ? new Date(date) : new Date;
        if (isNaN(date)) throw SyntaxError("invalid date");

        mask = String(dF.masks[mask] || mask || dF.masks["default"]);

        // Allow setting the utc argument via the mask
        if (mask.slice(0, 4) == "UTC:") {
            mask = mask.slice(4);
            utc = true;
        }

        var _ = utc ? "getUTC" : "get",
            d = date[_ + "Date"](),
            D = date[_ + "Day"](),
            m = date[_ + "Month"](),
            y = date[_ + "FullYear"](),
            H = date[_ + "Hours"](),
            M = date[_ + "Minutes"](),
            s = date[_ + "Seconds"](),
            L = date[_ + "Milliseconds"](),
            o = utc ? 0 : date.getTimezoneOffset(),
            flags = {
                d:    d,
                dd:   pad(d),
                ddd:  dF.i18n.dayNames[D],
                dddd: dF.i18n.dayNames[D + 7],
                m:    m + 1,
                mm:   pad(m + 1),
                mmm:  dF.i18n.monthNames[m],
                mmmm: dF.i18n.monthNames[m + 12],
                yy:   String(y).slice(2),
                yyyy: y,
                h:    H % 12 || 12,
                hh:   pad(H % 12 || 12),
                H:    H,
                HH:   pad(H),
                M:    M,
                MM:   pad(M),
                s:    s,
                ss:   pad(s),
                l:    pad(L, 3),
                L:    pad(L > 99 ? Math.round(L / 10) : L),
                t:    H < 12 ? "a"  : "p",
                tt:   H < 12 ? "am" : "pm",
                T:    H < 12 ? "A"  : "P",
                TT:   H < 12 ? "AM" : "PM",
                Z:    utc ? "UTC" : (String(date).match(timezone) || [""]).pop().replace(timezoneClip, ""),
                o:    (o > 0 ? "-" : "+") + pad(Math.floor(Math.abs(o) / 60) * 100 + Math.abs(o) % 60, 4),
                S:    ["th", "st", "nd", "rd"][d % 10 > 3 ? 0 : (d % 100 - d % 10 != 10) * d % 10]
            };

        return mask.replace(token, function ($0) {
            return $0 in flags ? flags[$0] : $0.slice(1, $0.length - 1);
        });
    };
}();

// Some common format strings
dateFormat.masks = {
    "default":      "ddd mmm dd yyyy HH:MM:ss",
    shortDate:      "m/d/yy",
    mediumDate:     "mmm d, yyyy",
    longDate:       "mmmm d, yyyy",
    fullDate:       "dddd, mmmm d, yyyy",
    shortTime:      "h:MM TT",
    mediumTime:     "h:MM:ss TT",
    longTime:       "h:MM:ss TT Z",
    isoDate:        "yyyy-mm-dd",
    isoTime:        "HH:MM:ss",
    isoDateTime:    "yyyy-mm-dd'T'HH:MM:ss",
    isoUtcDateTime: "UTC:yyyy-mm-dd'T'HH:MM:ss'Z'"
};

// Internationalization strings
dateFormat.i18n = {
    dayNames: [
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    ],
    monthNames: [
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    ]
};

// For convenience...
Date.prototype.format = function (mask, utc) {
    return dateFormat(this, mask, utc);
};








// TODO: udelat pres prototyp
function graph_generateDataLink(graph) 
{

  var dateFrom = graph.options.xmin;
  var dateTo   = graph.options.xmax;

  if (((typeof dateFrom == 'object') && (dateFrom instanceof Date))
  &&  ((typeof dateTo   == 'object') && (dateTo   instanceof Date)))
  {

    if (graph.options.xActual)
    {      
      dateTo   = new Date();
      dateFrom = new Date();
			dateFrom.setTime(dateTo.getTime() - 1000 * graph.options.xInterval);
    }

    var params = 'Count='     + graph.options.dataCount
           + '&DateFrom=' + dateFrom.format('yyyy-mm-dd HH:MM:ss') 
           + '&DateTo='   + dateTo.format('yyyy-mm-dd HH:MM:ss');

    for (i = 0; i < graph.options.ykeys.length; ++i)
    {
        params = params + '&Input' + i + '=' + graph.options.ykeys[i];
    }

    return params;
  }
  
  return '';
}

function graph_getGraphRange(graph) 
{
	if (graph.options.xActual)
		return graph.options.xInterval;
	else
		return (graph.options.xmax.getTime() - graph.options.xmin.getTime()) / 1000;
}

function graph_parseInfo(xNode, graph)
{

	var ymin      = xNode.getAttribute('ValueFrom'   ).replace(",", ".") * 1;
	var ymax      = xNode.getAttribute('ValueTo'     ).replace(",", ".") * 1;
	var xminStr   = xNode.getAttribute('DateFrom'    );
	var xmaxStr   = xNode.getAttribute('DateTo'      );
	var xInterval = xNode.getAttribute('DateInterval').replace(",", ".") * 1;
	var dataCount = 40;
	var xActual   = xNode.getAttribute('Continual'   ) == 'True';

	xminStr = xminStr.replace(' ', '-');
	xminStr = xminStr.replace(':', '-');
	xminStr = xminStr.replace(':', '-');
	
	xmaxStr = xmaxStr.replace(' ', '-');
	xmaxStr = xmaxStr.replace(':', '-');
	xmaxStr = xmaxStr.replace(':', '-');
	
	var xminArr = xminStr.split('-');
	var xmaxArr = xmaxStr.split('-');
	
	var xmin = new Date(xminArr[0], xminArr[1] - 1, xminArr[2], xminArr[3], xminArr[4], xminArr[5], 0);
	var xmax = new Date(xmaxArr[0], xmaxArr[1] - 1, xmaxArr[2], xmaxArr[3], xmaxArr[4], xmaxArr[5], 0);
	
	var ykeys      = [];
	var labels     = [];
	var lineColors = [];

	var ok = true
		&& (graph.options.ymin           == ymin          )
		&& (graph.options.ymax           == ymax          )
		&& (graph.options.xmin != null)
		&& (graph.options.xmin != null)
		&& (graph.options.xmin.getTime() == xmin.getTime())
		&& (graph.options.xmax.getTime() == xmax.getTime())
		&& (graph.options.xInterval      == xInterval     )
		&& (graph.options.dataCount      == dataCount     )
		&& (graph.options.xActual        == xActual       );	
			
	for (i = 0; i < xNode.childNodes.length; ++i)
	{
		var xItem = xNode.childNodes[i];

		if (xItem.nodeName == 'Item')
		{ 
			var guid    = xItem.getAttribute('Guid'   );
			var caption = xItem.getAttribute('Caption');
			var color   = xItem.getAttribute('Color'  );

			ykeys     [i] = guid   ;
			labels    [i] = caption;
			lineColors[i] = color  ;

			ok = ok 
				&& (graph.options.ykeys     [i] == ykeys     [i])
				&& (graph.options.labels    [i] == labels    [i])
				&& (graph.options.lineColors[i] == lineColors[i]);
		}
	}
	
	if (!ok)
	{
		graph.options.ymin       = ymin     ;
		graph.options.ymax       = ymax     ;
		graph.options.xmin       = xmin     ;
		graph.options.xmax       = xmax     ;
		graph.options.xInterval  = xInterval;
		graph.options.dataCount  = dataCount;
		graph.options.xActual    = xActual  ; 

		graph.options.ykeys      = ykeys     ;
		graph.options.labels     = labels    ;
		graph.options.lineColors = lineColors;		
	}
	
	return !ok; // changed
}

function graph_parseData(xNode, graph) 
{
  var ret = [];
  
	graph.options.dataLen = xNode.childNodes.length;
 
  for (i = 0; i < xNode.childNodes.length; ++i)
  {
    var xItem = xNode.childNodes[i];
          
    if (xItem.nodeName == 'Item')
    { 
      var date = xItem.getAttribute('DateStr');
      var time = xItem.getAttribute('TimeStr');
      var it = [];
   
      if (time != '')
        it[graph.options.xkey] = time + '\n' + date; //<br> 
      else
        it[graph.options.xkey] = date;
    
      for (j = 0; j < graph.options.ykeys.length; ++j) 
      {
        var valStr = xItem.getAttribute('Value' + j);
        
        if ((valStr != null) && (valStr != ''))
        {        
          value = parseFloat(valStr.replace(",","."));
          value = Math.round(10*value)/10;
          it[graph.options.ykeys[j]] = value;
        }
        else
        {
          it[graph.options.ykeys[j]] = null;
        }
      }         

      ret.push(it);
    }
  }
  return ret;
}

function graph_downloadData(graph)
{
	$.ajax({              
	url: '../GraphData.xml?' + graph_generateDataLink(graph),
	//data: xml,
	//dataType: "xml",
		contentType: "text/xml",
	type: 'GET',
	cache: false,
	error:
		function(xhr, textStatus) 
		{

		},
	success:
		function(xml)
		{
			var xNode = xml.documentElement;
			var data = graph_parseData(xNode, graph);      
			graph.setData(data);      
		}
	});
	
	
//-----------------  
/*
  $.ajax({              
	url: '../GraphData.xml?' + graph_generateDataLink(graph),
	//data: xml,
	//dataType: "xml",
    contentType: "text/xml",
	type: 'GET',
	cache: false
  })
  .fail(
    function(xhr, textStatus)
    {
      
    }
  )
  .done(
    function(xml)
    {
      var xNode = xml.documentElement;
      data = graph_parseData(xNode, graph);      
      graph.setData(data);      
    }
  );
*/
}

function graph_downloadInfo(graph, guid)
{
 
  $.ajax({              
	url: '../GraphInfo.xml?Guid=' + guid,
	//data: xml,
	//dataType: "xml",
    contentType: "text/xml",
	type: 'GET',
	cache: false
  })
  .fail(
    function(xhr, textStatus)
    {
      
    }
  )
  .done(
    function(xml)
    {
      var xNode = xml.documentElement;
      
      graph_parseInfo(xNode, graph);
    }
  );
}
