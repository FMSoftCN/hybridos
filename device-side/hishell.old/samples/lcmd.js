function addRow(name, url, isdir, size, size_string, time_field_1, time_field_2, time_field_3) {
    if (name == "." || name == "..")
        return;

    var tbody = document.getElementById("tbody");
    var row = document.createElement("tr");
    var file_cell = document.createElement("td");
    var link = document.createElement("a");

    link.className = isdir ? "icon dir" : "icon file";

    if (isdir) {
        name = name + "/";
        url = url + "/";
    }
    link.innerText = name;
    link.onclick = function() {
        lcmdGet(url);
    }

    file_cell.dataset.value = name;
    file_cell.appendChild(link);

    row.appendChild(file_cell);
    row.appendChild(createCell(size, size_string));
    row.appendChild(createCell(time_field_1, time_field_1));
    row.appendChild(createCell(time_field_2, time_field_2));
    row.appendChild(createCell(time_field_3, time_field_3));

    tbody.appendChild(row);
}

function createCell(value, text) {
    var cell = document.createElement("td");
    cell.setAttribute("class", "detailsColumn");
    cell.dataset.value = value;
    cell.innerText = text;
    return cell;
}

function start(location) {
    var header = document.getElementById("header");
    header.innerText = location + " 的索引";
}

function onHasParentDirectory(url) {
    var path = url;
    if (url.length >= 2)
    {
        var idx = url.lastIndexOf("/", url.length - 2);
        if (idx > 0)
        {
            path = url.substr(0, idx + 1);
        }
        else
        {
            path = "/";
        }
    }

    var box = document.getElementById("parentDirLinkBox");
    box.style.display = "block";

    var link = document.getElementById("parentDirLink");
    link.onclick = function() {
        lcmdGet(path);
    }
}

function onNoParentDirectory() {
    var box = document.getElementById("parentDirLinkBox");
    box.style.display = "none";
}

function onListingParsingError() {
    var box = document.getElementById("listingParsingErrorBox");
    box.innerHTML = box.innerHTML.replace("LOCATION", encodeURI(document.location)
        + "?raw");
    box.style.display = "block";
}

function sortTable(column) {
    var theader = document.getElementById("theader");
    var oldOrder = theader.cells[column].dataset.order || '1';
    oldOrder = parseInt(oldOrder, 10)
    var newOrder = 0 - oldOrder;
    theader.cells[column].dataset.order = newOrder;

    var tbody = document.getElementById("tbody");
    var rows = tbody.rows;
    var list = [], i;
    for (i = 0; i < rows.length; i++) {
        list.push(rows[i]);
    }

    list.sort(function(row1, row2) {
        var a = row1.cells[column].dataset.value;
        var b = row2.cells[column].dataset.value;
        if (column) {
            a = parseInt(a, 10);
            b = parseInt(b, 10);
            return a > b ? newOrder : a < b ? oldOrder : 0;
        }

        // Column 0 is text.
        if (a > b)
            return newOrder;
        if (a < b)
            return oldOrder;
        return 0;
    });

    // Appending an existing child again just moves it.
    for (i = 0; i < list.length; i++) {
        tbody.appendChild(list[i]);
    }
}

// Add event handlers to column headers.
function addHandlers(element, column) {
    element.onclick = (e) => sortTable(column);
    element.onkeydown = (e) => {
        if (e.key == 'Enter' || e.key == ' ') {
            sortTable(column);
            e.preventDefault();
        }
    };
}

function getQueryStringByName(name)
{
    var result = location.search.match(new RegExp("[\?\&]" + name+ "=([^\&]+)","i"));
    if(result == null || result.length < 1){
        return "/";
    }
    return result[1];
}

function lcmdGet(path)
{
    if (path == undefined)
        path = getQueryStringByName("path");

    start(path);
    if (path == "/")
    {
        onNoParentDirectory()
    }
    else
    {
        onHasParentDirectory(path);
    }

    var xmlHttp = new XMLHttpRequest();
    var baseUrl = "lcmd:///bin/bash?cmdfilter=delimiter('%20');array()&cmdline=ls%20-l%20"; 
    var url =  baseUrl + path;
    var tbody = document.getElementById("tbody");
    tbody.innerHTML="";
    xmlHttp.open( "GET", url, true);
    xmlHttp.onreadystatechange=function()
    {
        if (xmlHttp.readyState==4 && xmlHttp.status==200)
        {
            var responseText = xmlHttp.responseText;
            var obj = JSON.parse(responseText);
            if (obj.statusCode == 200)
            {
                for (var i=0; i < obj.lines.length; i++)
                {
                    var line = obj.lines[i];
                    if (line.length <= 2)
                    {
                        continue;
                    }
                    var type = line[0].charAt(0)=='d' ? 1 : 0;
                    addRow(line[8], path + line[8], type, line[4], line[4], line[5], line[6], line[7]);
                }
            }
        }
    }
    xmlHttp.send( null );
}

function onLoad() {
    addHandlers(document.getElementById('nameColumnHeader'), 0);
    addHandlers(document.getElementById('sizeColumnHeader'), 1);
    addHandlers(document.getElementById('dateColumnHeader'), 2);
    lcmdGet();
}


window.addEventListener('DOMContentLoaded', onLoad);

