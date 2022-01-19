var timer;
preloadImages();
var interval = 1000;
var httpMethod = "POST";
var historyDataType = "XML";
var rowsPerPage = 50;
var currentPage = 1;
var maxRowsPerPage = 1000;
var importColors = false;
var rebuildPage = true;
var doLoad = true;
var activePanel = "#data";
var defaultContainerWidth = "550px";
var pageDifference = 8;
var DataMode = 0;
var history_Start;
var history_Stop;
var iAttr = new Array();
iAttr = ["Value", "Avg", "Min", "Max", "Count", "Invalids"];
var sAttr = new Array();
sAttr = ["Value", "Average", "Minimum", "Maximum", "Number of valid", "Number of invalid"];
var moment = new Date();
var webVersion = "Web&nbsp;ver.&nbsp;1.4.1";

var graphs = [];


function dataRefresh()
{
	if(doLoad){
		clearTimeout(timer);
		$.ajax({
			url:'panels.xml',
			datatype:'XML',
			cache: false,
			error: function() {$("#data").empty(); appendErrorMessage('No data found.', 'Please make sure the application is runnung.'); updateTime(); $("#data").slideDown("fast");},
			success: renderDataTable,
			type: httpMethod
		});
		timer = setTimeout("dataRefresh()", 1*interval);
	}
}

$(document).ready(
	function () {
		dataRefresh();
		$("#parrot").mouseover(function() {$(this).css({'border-right' : '2px solid #F1F1F1', 'border-bottom' : '2px solid #F1F1F1'});});
		$("#parrot").mouseout(function() {$(this).css('border', '2px solid #FFF');});
		$("#parrot").click(function () {$("#menu").fadeIn(200);});
		$("#menu-container").bind('mouseleave', function () {$("#menu").fadeOut(200);});
		$(".menu-item").mouseover(function() {$(this).css({"background-color": "#e3f6ea"});});
		$(".menu-item").mouseout(function() {$(this).css({"background-color": "#FFFFFF"});})
		$(".menu-item").click(function () {$("#menu").fadeOut(200); window[$(this).attr("data-handler")]();});
		$("input.number").keyup(function () { evalInput($(this).attr("id")) });
		$("#changeHSettings").click(resetHistory);
		//$('.changeState_btn').live('click', function () {$.ajax({ url: '/setbooloutput.xml?Guid=' + $(this).attr("data-guid") + '&Value=' + $(this).attr("data-setval"), success: function () { window.location.href = window.location.href; }, error: function () { window.location.href = window.location.href; } }) });
		//$('.changeState_btn').live('click', function () {
		$('body').on('click', '.changeState_btn', function () {
		//$(".changeState_btn").click(function () {		
			var cmdurl = 'setbooloutput.xml?Guid=' + $(this).attr("data-guid") + '&Value=' + $(this).attr("data-setval");
			//alert(cmdurl)
			$.ajax({
				url: cmdurl,
				datatype:'XML',
				cache: false,
				success: dataRefresh,
				//success:function () {alert('ok')},
				error: function () {alert('error')},
				type: httpMethod
			})
		});
	}
);

function getElementsByClassName(node, classname) {
    var a = [];
    var re = new RegExp('(^| )'+classname+'( |$)');
    var els = node.getElementsByTagName("*");
    for(var i=0,j=els.length; i<j; i++)
        if(re.test(els[i].className))a.push(els[i]);
    return a;
}

function renderDataTable (data) {
	if (doLoad) {

		if ($(data).find("Cf").attr("Refresh") != null) { interval = $(data).find("Cf").attr("Refresh")*1000; };
		if ($(data).find("Cf").attr("Rainbow") != null) { importColors = ($(data).find("Cf").attr("Rainbow") == "true") ? true : false; };
		if ($(data).find("Cf").attr("Crowded") != null) { rowsPerPage = $(data).find("Cf").attr("Crowded"); };

		var devices = $(data).find("Device");

		if (devices.length > 0)
		{

			//if (!rebuildPage)
			//{
			//	updateDataFields(data);
			//}
			//else
			{
				var c = "";

				//$("#data").empty();
				//var errors = document.querySelectorAll(".error");
				var errors = getElementsByClassName(document.body, "error");
				for (var i = 0; i < errors.length; ++i)
				{
					errors[i].parentNode.removeChild(errors[i]);
				}

				if ($("#data").find(".page-header").length == 0)
				{
					var divPageHeader = document.createElement('div');
					$("#data").append(divPageHeader);
					divPageHeader.className = 'page-header';

					var divPageHeaderInner = document.createElement('div');
					divPageHeader.appendChild(divPageHeaderInner);
					divPageHeaderInner.className = 'page-header-inner';

					var spanPageLabel = document.createElement('span');
					divPageHeaderInner.appendChild(spanPageLabel);
					spanPageLabel.innerHTML = 'Current data';
				}

/*
				$("#data").append(''
					+ '<div class="page-header">'
					+ '  <div class="page-header-inner">'
					+ '    <span class="page-label">Current data</span>'
					+ '  </div>'
					+ '</div>'
				);
*/

				//devices.each(function ()
				for (var deviceIndex = 0; deviceIndex < devices.length; ++deviceIndex)
				{
					var device = devices[deviceIndex];

					var d = "";
					//var inputs = $(this).find("Input");
					if (device.childNodes.length > 0)
					{
						var table;
						var tables = $("#data").find(".datafields");
						if (tables.length == 0)
						{
							table = document.createElement('table');
							$("#data").append(table);
							table.className = 'datafields';
						}
						else
						{
							table = tables[0];
						}

						var tableBody;
						var tableBodys = $(".datafields").find('.datafields_container');
						if (tables.length == 0)
						{
							tableBody = document.createElement('tbody');
							table.appendChild(tableBody);
							tableBody.className = 'datafields_container';
						}
						else
						{
							tableBody = tableBodys[0];
						}

						//tableBody.innerHTML = '';

						//tableBody.innerHTML = table.rows.length;
						/*
						var tableBody = document.createElement('tbody');
						table.appendChild(tableBody);
						tableBody.className = 'datafields_container';
				*/

						var rows = table.rows;
						var rowIndex = 0;

						var i;
						for (i = 0; i < device.childNodes.length; ++i)
						{
							var xItem = device.childNodes[i];

							if ((xItem.getAttribute("Visible") == "True") || (xItem.nodeName == 'Graph')) // TODO: dost nepekne
							{
								var tableRow;
							  if (rows.length > rowIndex)
								{
									tableRow = rows[rowIndex];
								}
								else
								{
									tableRow = document.createElement('tr');
									tableBody.appendChild(tableRow);
								}
								++rowIndex;

								//d += getSingleRow(

								if (xItem.nodeName == 'Input')
								{

									getSingleRow(
										tableRow,
										xItem.getAttribute("GUID"         ),
										xItem.getAttribute("ValueRaw"     ),
										xItem.getAttribute("Value"        ),
										xItem.getAttribute("ValueStr"     ),
										xItem.getAttribute("Valid"        ),
										xItem.getAttribute("Name"         ),
										xItem.getAttribute("DateTime"     ),
										xItem.getAttribute("Units"        ),
										xItem.getAttribute("ValueType"    ),
										xItem.getAttribute("DecDigCount"  ),
										xItem.getAttribute("DigitalOutput"),
										xItem.getAttribute("Color"        )
									);

								}
								else
								if (xItem.nodeName == 'Graph')
								{
									getGraphRow(
									  tableRow,
										xItem
									);

									//tableRow.innerHTML = xItem.nodeName;
								}

								//tableRow.innerHTML = xItem.nodeName;
							}
							
							if (xItem.nodeName == 'Input')
								c += '<input type="checkbox" value="' + xItem.getAttribute("GUID") + '" name="inputs" />&nbsp;' + xItem.getAttribute("Name") + '<br />';
						}

						for (var j = rows.length - 1; j >= rowIndex; --j)
						{
							rows[j].parentNode.removeChild(rows[j]);
						}

						//tableBody.innerHTML = d;
						//$("#data").append(d);
					} else {
						appendErrorMessage('No inputs found for given device.', 'Please make sure the device is properly configured.');
					}
				}//);

			$("#input_fields").html(c);

			}
		} else {
		  $("#data").empty();
			appendErrorMessage('No devices found.', 'Please check the connections.');
		}

		// SAVED
		$("#build").html (webVersion + '&nbsp;|&nbsp;WiX&nbsp;ver.&nbsp;' + $(data).find("Cf").attr("Ver"));

		rebuildPage = false;

		updateTime();
		$("#data").slideDown("fast");
	};
};

function changeText(elem, changeVal) {
    if (typeof elem.textContent !== "undefined") {
        elem.textContent = changeVal;
    } else {
        elem.innerText = changeVal;
    }
}

function getSingleRow(TableRow, GUID, ValueRaw, Value, ValueStr, Valid, Name, DateTime, Units, ValueType, DecDigCount, DigitalOutput, Color)
{
	var d = '';

	var GUIDr = GUID;
	GUIDr = GUIDr.replace("{", "").replace("}", "");

	TableRow.id = GUIDr + '_row';

	if (Valid == "True")
	{
		if (ValueType == "0")
			Value = parseFloat(Value.replace(",", ".")).toFixed(parseInt(DecDigCount));
		else
			Value = ValueStr;

		////d += '<tr id="' + GUIDr + '_row"';
		if (importColors)
			TableRow.style.backgroundColor = Color;
		else
		  TableRow.style.backgroundColor = "White";

		while (TableRow.cells.length < 3)
			TableRow.insertCell(0);
		
		var cell = TableRow.cells[0];	
	  changeText(cell, Name);
		
		var cell = TableRow.cells[1];	
	  cell.id = GUIDr + '_value';
		cell.className = "val";
		cell.title = 'Time of value: ' + DateTime;
		changeText(cell, Value);
		
		////d += '>';
		//d += '<td>' + Name + '</td>';
		//d += '<td class="val" id="' + GUIDr + '_value" title="Time of value: ' + DateTime + '">' + Value + '</td>';
		
		var cell = TableRow.cells[2];		  		

		if (DigitalOutput == 1)
		{
		  d = '';
			//d += '<td>';
			if (ValueRaw == 1)
			{
				d += '<a class="on_btn on_btn_active"></a>';
				d += '<a class="off_btn changeState_btn" data-guid="' + GUID + '" data-setval="0" href="javascript:void(0);"></a>';
			}
			else
			{
				d += '<a class="on_btn changeState_btn" data-guid="' + GUID + '" data-setval="1" href="javascript:void(0);"></a>';
				d += '<a class="off_btn off_btn_active"></a>';
			}
			//d += '</td>';

		  cell.innerHTML = d;
		}
		else
		{
		  cell.style.width = "50px";
			changeText(cell, Units);
			//d += '<td style="width:50px;">' + Units + '</td>';
		}
		//d += '</tr>';
	}
	else
	{
		////d += '<tr';
		////d += (importColors) ? ' style="background-color:' + Color + ';"' : ' style="background-color: #ed9898;"';
		////d += '>';
		if (importColors)
			TableRow.style.backgroundColor = Color;
		else
			TableRow.style.backgroundColor = "#ed9898";


		//d += '<td>' + Name + '</td>';
		//d += '<td class="val" id="' + GUIDr + '_value">N/A</td>';
		//d += '<td style="width:50px;"></td>';
		////d += '</tr>';
		
		while (TableRow.cells.length < 3)
			TableRow.insertCell(0);
		
		var cell = TableRow.cells[0];	
	  changeText(cell, Name);
		
		var cell = TableRow.cells[1];	
	  cell.id = GUIDr + '_value';
		cell.className = "val";		
		changeText(cell, "N/A");
		
		var cell = TableRow.cells[2];	
		cell.style.width = "50px";
	}

	//TableRow.innerHTML = d;

	return d;
}

function getGraphRow(TableRow, xItem)
{
	var d = '';

	var GUID = xItem.getAttribute("GUID");

	var GUIDr = GUID;
	GUIDr = GUIDr.replace("{", "").replace("}", "");

	if (importColors)
		TableRow.style.backgroundColor = xItem.getAttribute("BackColor");
	else
		TableRow.style.backgroundColor = "White";

	//TableRow.style.backgroundColor = "Gray";
	
	if (TableRow.id != GUIDr + '_row')
	{
		TableRow.id = GUIDr + '_row';
		
		var cell = TableRow.insertCell(0);	
	  cell.colSpan="3"; 
		
		d +=   '<div id="' + GUIDr + '_name"  style="text-align: center;">' + xItem.getAttribute("Name") + '</div>';
		d +=   '<div id="' + GUIDr + '_graph" style="height: 240px;"></div>';	

		cell.innerHTML = d;

		var graph = Morris.Line({
			element          : GUIDr + '_graph',
			data             : [],
			ykeys            : [],
			xLabels          : "hour",
			pointSize        : 2.5,
			parseTime        : false,
			hideHover        : true,
			//resize           : true,
			resize           : false,
			smooth           : false
		});

		if (importColors)
		{
			//var hover = $('#' + GUIDr + '_row').find('.morris-hover')[0];
			//hover.style.backgroundColor = 'Black';
			//hover.style.borderColor = 'Green';
			//hover.style.borderWidth = '1px';
			//hover.style.opacity = '0.8';
						
			graph.options.gridTextColor = xItem.getAttribute("TextColor");
		}

		// - morris-hover morris-hover-row-label

		graphs[GUIDr] = graph;

		graph_parseInfo(xItem, graphs[GUIDr]);
		graph_downloadData(graphs[GUIDr]);

		//TableRow.dataset.lastUpdate = Math.round(new Date().getTime() / 1000);
		if (TableRow.dataset !== undefined) 
				TableRow.dataset.lastUpdate = Math.round(new Date().getTime() / 1000);
			else
				TableRow.setAttribute('data-lastUpdate', Math.round(new Date().getTime() / 1000));
	}
	else
	{
		var lastUpdate = 0;
		if (TableRow.dataset !== undefined) 
			lastUpdate = TableRow.dataset.lastUpdate;
		else
			lastUpdate = TableRow.getAttribute('data-lastUpdate'); 
		
		var now = Math.round(new Date().getTime() / 1000);

		var cellLen = graph_getGraphRange(graphs[GUIDr]) / graphs[GUIDr].options.dataCount;

		if (graph_parseInfo(xItem, graphs[GUIDr]) // graph info changed
		|| (now > 1*lastUpdate + cellLen))        // ackoliv se to nezda, tak to vynasobeni jednickou je dulezite
		{
			graph_downloadData(graphs[GUIDr]);
			//TableRow.dataset.lastUpdate = now;
			if (TableRow.dataset !== undefined) 
				TableRow.dataset.lastUpdate = now;
			else
				TableRow.setAttribute('data-lastUpdate', now);
		}
	}
	
	/*
	if (graphs[GUIDr].options.dataLen != null)
	{
		graphs[GUIDr].options.goals  = [0, graphs[GUIDr].options.ymin, graphs[GUIDr].options.ymax];
		graphs[GUIDr].options.events = [0, 1];
		graphs[GUIDr].options.events = [0, graphs[GUIDr].options.dataLen - 1];
	//alert(graphs[GUIDr].options.dataLen); 
		graphs[GUIDr].options.goalLineColors  = ["Red", "Red", "Red"];
		graphs[GUIDr].options.goalStrokeWidth = 1;
		
		graphs[GUIDr].options.eventLineColors = ["Red", "Red"];
		graphs[GUIDr].options.eventStrokeWidth = 1;
	}
	*/
	graphs[GUIDr].options.pointStrokeColors = graphs[GUIDr].options.lineColors;
	//alert(importColors);
	if (importColors)
	{
		//var hover = $('#' + GUIDr + '_row').find('.morris-hover')[0];
		//hover.style.backgroundColor = 'Black';
		//hover.style.borderColor = 'Green';
		//hover.style.borderWidth = '1px';
		//hover.style.opacity = '0.8';
		
		graphs[GUIDr].options.gridTextColor = xItem.getAttribute("TextColor");		
		
		var graphName = document.getElementById(GUIDr + '_name');
		if (graphName != null)
		{
			graphName.style.color =  xItem.getAttribute("NameColor");			
		}
	}
	else
	{			
		graphs[GUIDr].options.gridTextColor = "#888";
	}
	
	
	var graphName = document.getElementById(GUIDr + '_name');
	if (graphName != null)
	{
		graphName.innerText = xItem.getAttribute("Name");			
	}

}

function appendErrorMessage (m, d, e)
{
	if (e == null)
		e = "#data";

	var eHtml = ''
		+ '<div class="error">'
		+ 	'<table>'
		+ 		'<tr>'
		+ 			'<td>'
		+ 				'<img src="images/error_icon.png" alt="Error" />'
		+ 			'</td>'
		+ 			'<td>'
		+ 				'<span class="errorMessage">' + m + '</span>';

	if (d != null)
	{
		eHtml += ''
		+ 				'<br />'
		+ 				'<span class="details">' + d + '</span>'
	};

	eHtml += ''
		+ 			'</td>'
		+ 		'</tr>'
		+ 	'</table>'
		+ '</div>';

	$(e).append(eHtml);
};

function updateTime ()
{
	var d = new Date ();
	var m = d.getMinutes();
	var s = d.getSeconds();
	var month = d.getMonth() + 1;
	if (m < 10) { m = '0' + m };
	if (s < 10) { s = '0' + s };
	$("#updateTime").html(d.getDate() + '.' + month + '.' + d.getFullYear() + '&nbsp;v&nbsp;' + d.getHours() + ':' + m + ':' + s);
};

function preloadImages() {
	var image = new Image ();
	image.src = 'images/error_icon.png';
}

// UI
function transition (to, newWidth, callback) {
	if (to != activePanel){
		$(activePanel).slideUp("fast", function () {
			if (newWidth != null) {	$("#container").css("width", newWidth); };
			activePanel = to;
			$(to).slideDown("fast", callback);
		});
	}
}
function showData() {
	doLoad = true;
	(importColors) ? $("#import_colors").attr("checked", "checked") : $("#import_colors").removeAttr("checked");
	transition ("#data", defaultContainerWidth, dataRefresh);
}


//NASTAVENÍ
function showSettings() {
	clearTimeout (timer);
	doLoad = false;
	$("#seconds").val(interval/1000);
	$("#rowsPerPage_history").val(rowsPerPage);
	(importColors) ? $("#import_colors").attr("checked", "checked") : $("#import_colors").removeAttr("checked");
	transition ("#settings", defaultContainerWidth);
}
var seconds = interval/1000;
var rowsPerPage_history = rowsPerPage;
var history_interval = "";
function evalInput(e) {
	if (isNaN($("#" + e).val())){ alert("Please input numerical value."); $("#" + e).val(window[e]);} else { window[e] = $("#" + e).val();};
}
function evalRows() {
	if ($("#rowsPerPage_history").val() > maxRowsPerPage ) { $("#rowsPerPage_history").val(1000); alert("Maximum number of rows per page is 1000."); };
}
function cancelSettings() {
	doLoad = true;
	transition ("#data", defaultContainerWidth, dataRefresh);
}
function saveSettings() {
	interval = $("#seconds").val()*1000;
	rowsPerPage = $("#rowsPerPage_history").val();
	($("#import_colors:checked").length > 0) ? importColors = true : importColors = false;
	doLoad = true;
	var poststring = 'Save=Refresh=' + $("#seconds").val() + ',Rainbow=' + importColors + ',Crowded=' + rowsPerPage;
	$.ajax({url: "data.xml", data:poststring, success:function (d) { transition ("#data", defaultContainerWidth, dataRefresh); }, error:function () { appendErrorMessage('Settings were not saved.', 'If the problem persists, please contact the ¨supplier of this software.'); transition ("#data", defaultContainerWidth, dataRefresh); } });
}


//HISTORIE
function showHistory() {
	doLoad = false;
	clearTimeout (timer);
	$('#hist_beg').val(dateFormat(moment.getTime() - 86400000, "yyyy-mm-dd HH:MM"));
	$('#hist_end').val(moment.format("yyyy-mm-dd HH:MM"));
	transition ("#history", "100%");
}
function closeHistory() {
	transition ("#data", defaultContainerWidth, dataRefresh);
}
function loadHistory () {
	if (evalHistorySettings()){
		$("#history_settings").slideUp("fast");
		$("#loader").slideDown("fast");
		$.ajax({url:'history.xml',data: returnHistoryPostString(), datatype:historyDataType, cache: false, error: function() {
			$("#history_data").empty();
			appendErrorMessage('No data found.', 'Please make sure the application is running.', "#history_data");
			$("#history_displayDivision").slideDown("fast");},
		success: parseHistoryData });
	}
}


function parseHistoryData(data) {
	$("#history-pages").empty();

	var pageCount = parseInt($(data).find("DataHead").attr("PageCount"));
	if (pageCount < pageDifference*2+1){
		for (i=1; i<=pageCount; i++){
			if (i==currentPage){
				$("#history-pages").append('&nbsp;<b>' + i + '</b>');
			} else {
				$("#history-pages").append('&nbsp;<a href="#hist' + i + '" + class="histPage" onclick="gotoPage(' + i +')">' + i + '</a>');
			}
		};
	} else {
		var c = "";
		if (currentPage-(pageDifference+1) > 1) { c += '<a href="#hist' + 1 + '" + class="histPage" onclick="gotoPage(' + 1 +')">' + 1 + '</a>&nbsp;...' };
		for ((currentPage-pageDifference > 1) ? i=currentPage-pageDifference : i=1; (currentPage-pageDifference > 0) ? i>currentPage-pageDifference : i>0,  (currentPage+pageDifference > pageCount) ? i <= pageCount : i <= currentPage+pageDifference; i++){
			if (i==currentPage){
				c += '&nbsp;<b>' + i + '</b>';
			} else {
				c += '&nbsp;<a href="#hist' + i + '" + class="histPage" onclick="gotoPage(' + i +')">' + i + '</a>';
			}
		};
		if (currentPage+pageDifference < pageCount) { c += '&nbsp;...&nbsp;<a href="#hist' + pageCount + '" + class="histPage" onclick="gotoPage(' + pageCount +')">' + pageCount + '</a>' };
		$("#history-pages").html(c);
	}

	var uAttr = new Array();
	var lAttr = new Array();
	var decPlaces = new Array();
	var partial = DataMode;
	for (i=64, j=6; i>0; i=i/2, j--){
		if (partial-i>=0){
			uAttr.push(iAttr[j]);
			lAttr.push(sAttr[j]);
			partial = partial-i;
		}
	};
	var e = '<table cellspacing="10"><tr><th></th>';
	$(data).find("Input").each(function (index) {
		e += '<th colspan="' + uAttr.length + '">' + $(this).attr("Name") + '&nbsp;(' + $(this).attr("Units") + ')</th>';
		decPlaces[index] = $(this).attr("DecDigCount");
	});
	e += '</tr><tr><th></th>';
	$(data).find("Input").each(function () {
		for (var u = lAttr.length-1; u >= 0; u--){
			e += '<th>' + lAttr[u] + '</th>';
		}
	});
	e += '</tr>';
	$(data).find("DataBody").find("Row").each (function () {
		e += '<tr><td class="dateCell">' + $(this).attr("DateTime") + '</td>';
		$(this).find("Cell").each (function (index) {
			if ($(this).attr("Valid") == "False"){
				for (var u = uAttr.length-1; u >= 0; u--){
					e += '<td style="background-color:#ed9898;">N/A</td>';
				}
			} else {
				for (var u = uAttr.length-1; u >= 0; u--){
					if (uAttr[u] == "Count" || uAttr[u] == "Invalids"){
						e += '<td>' + parseFloat($(this).attr(uAttr[u]).replace(",",".")).toFixed(0) + '</td>';
					} else {
						e += '<td>' + parseFloat($(this).attr(uAttr[u]).replace(",",".")).toFixed(decPlaces[index]) + '</td>';
					}
				}
			}
		});
		e += '</tr>';
	});
	$("#history_data").html(e);
	generateXMLDownloadLink();
	generateCSVDownloadLink();
	$("#container").css("width", "100%");
	$("#history_displayDivision").slideDown("fast");
	$("#loader").slideUp("fast");
}

function returnHistoryPostString() {
	var string = "";
	var beginning = ($("#hist_beg").val().length >1) ? $("#hist_beg").val() + ':00' : '';
	var end = ($("#hist_end").val().length >1) ? $("#hist_end").val() + ':00' : '';
	string += "Start=" + beginning + "&Stop=" + end;
	string += "&Interval=" + $("#history_interval").val() + "&IntervalType=" + $("#interval_units option:selected").val();
	DataMode = 0;
	$("input[name=dataMode]:checked").each(function () {
		DataMode += parseInt($(this).val());
	});
	string += "&DataMode=" + DataMode + "&DataType=" + historyDataType + "&Page=" + currentPage +"&RowPerPage=" + rowsPerPage;
	var i=0;
	$("input[name=inputs]:checked").each (function(index) { string += "&Input" + index + "=" + $(this).val(); });
	return string;
}
function generateXMLDownloadLink () {
	var beginning = ($("#hist_beg").val().length >1) ? $("#hist_beg").val() + ':00' : '';
	var end = ($("#hist_end").val().length >1) ? $("#hist_end").val() + ':00' : '';
	var dlink = "history.xml?";
	dlink += "Start=" + beginning + "&Stop=" + end;
	dlink += "&Interval=" + $("#history_interval").val() + "&IntervalType=" + $("#interval_units option:selected").val();
	dlink += "&DataMode=" + DataMode ;//+ "&Page=" + currentPage +"&RowPerPage=" + rowsPerPage;
	var i=0;
	$("input[name=inputs]:checked").each (function(index) { dlink += "&Input" + index + "=" + $(this).val(); });
	$("#download_xml").attr("href", dlink);
}
function generateCSVDownloadLink() {
	var beginning = ($("#hist_beg").val().length >1) ? $("#hist_beg").val() + ':00' : '';
	var end = ($("#hist_end").val().length >1) ? $("#hist_end").val() + ':00' : '';
	var dlink = "history.csv?";
	dlink += "Start=" + beginning + "&Stop=" + end;
	dlink += "&Interval=" + $("#history_interval").val() + "&IntervalType=" + $("#interval_units option:selected").val();
	dlink += "&DataMode=" + DataMode ;//+ "&Page=1&RowPerPage=" + rowsPerPage;
	$("input[name=inputs]:checked").each (function(index) { dlink += "&Input" + index + "=" + $(this).val(); });
	$("#download_csv").attr("href", dlink);
}
function evalHistorySettings () {
	if ($("input[name=inputs]:checked").length == 0) { alert ("Please chose at least one input."); return false; };
	if ($("#hist_all:checked").length == 0){ if ($("#hist_beg").length == 0) { alert ("Please enter the beginning of data range or chose all data."); return false; } };
	if ($("#hist_all:checked").length == 0){ if ($("#hist_end").length == 0) { alert ("Please enter the end of data range or chose all data."); return false; } };
	if ($("#hist_all:checked").length == 0){ if (!validateDateToMask ($("#hist_beg").val(), "yyyy-mm-dd HH:MM")) { alert ("Please enter the beginning of data range in format\n\n2010-12-31 10:00"); return false; } };
	if ($("#hist_all:checked").length == 0){ if (!validateDateToMask ($("#hist_end").val(), "yyyy-mm-dd HH:MM")) { alert ("Please enter the end of data range in format\n\n2010-12-31 10:00"); return false; } };
	if ($("#hist_all:checked").length == 0){ if ($("#hist_end").val() == $("#hist_beg").val()) { alert ("Beginning and end are the same. No data selected."); return false; } };
	if ($("#history_interval").val().length == 0){ alert ("Please enter period to select data."); return false; };
	if ($("input[name=dataMode]:checked").length == 0) { alert ("Please chose at least one value to show."); return false; };
	if ($("#hist_all:checked").length > 0){ return window.confirm("You have chosen to show all data. This operation may take long time. Do you wish to proceed?"); };
	return true;
}
function toggleHistAll () {
	if ($("#hist_all:checked").length > 0){
		$("#hist_beg").attr('disabled', 'disabled');
		$("#hist_end").attr('disabled', 'disabled');
		history_Start = $("#hist_beg").val();
		history_Stop = $("#hist_end").val();
		$("#hist_beg").val("");
		$("#hist_end").val("");
	} else {
		$("#hist_beg").removeAttr('disabled');
		$("#hist_end").removeAttr('disabled');
		$("#hist_beg").val(history_Start);
		$("#hist_end").val(history_Stop);
	}
}
function gotoPage (pgn){
	currentPage = pgn;
	$("#history_displayDivision").slideUp("fast");
	$("#loader").slideDown("fast");
	$.ajax({url:'history.xml',data: returnHistoryPostString(), datatype:"XML", cache: false, error: function() {
		$("#history_data").empty();
		appendErrorMessage('No data found.', 'Please make sure the application is running', "#history_data");
		$("#history_displayDivision").slideDown("fast");
	}, success: parseHistoryData });
}
function resetHistory () {
	$("#history_displayDivision").slideUp("fast", function () {
		$("#history_settings").slideDown("fast");
		$("#history-pages").empty();
	});
}

function updateDataFields (data) {

	$(data).find("Device").each(function () {

		$(this).find("Input").each(function () {

			if ($(this).attr("Visible") != "True") return;

			var GUID = $(this).attr("GUID");
			GUID = GUID.replace("{", "").replace("}", "");

			if ($('#' + GUID + '_value').length == 0) {
				rebuildPage = true;
				renderDataTable(data);
				return;
			}

			var value = $(this).attr("Value");
			if ($(this).attr("ValueType") == "0") {
			  value = parseFloat(value.replace(",",".")).toFixed(parseInt($(this).attr("DecDigCount")));
			} else{
			  value = $(this).attr("ValueStr");
			}

			$('#' + GUID + '_value').text(value);

		});

	});

	updateTime();

}