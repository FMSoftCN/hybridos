function addRow(id, title, author, isbn) {
    if (name == "." || name == "..")
        return;

    var tbody = document.getElementById("tbody");
    var row = document.createElement("tr");

    row.appendChild(createCell(id, id));
    row.appendChild(createCell(title, title));
    row.appendChild(createCell(author, author));
    row.appendChild(createCell(isbn, isbn));

    tbody.appendChild(row);
}

function createCell(value, text) {
    var cell = document.createElement("td");
    cell.setAttribute("class", "detailsColumn");
    cell.dataset.value = value;
    cell.innerText = text;
    return cell;
}

function start(value) {
    var input = document.getElementById("cmdInput");
    input.value = decodeURI(value);
}

function onListingParsingError() {
    var box = document.getElementById("listingParsingErrorBox");
    box.innerHTML = box.innerHTML.replace("LOCATION", encodeURI(document.location)
        + "?raw");
    box.style.display = "block";
}

function getQueryStringByName(name)
{
    var result = location.search.match(new RegExp("[\?\&]" + name+ "=([^\&]+)","i"));
    if(result == null || result.length < 1){
        return "/";
    }
    return result[1];
}

function lsqlSelect()
{
    var cmd =  document.getElementById('cmdInput').value;
    if(typeof cmd == "undefined" || cmd == null || cmd == "")
    {
        return;
    }

    var xmlHttp = new XMLHttpRequest();
    var baseUrl = "lsql:///home/xue/work/hybridos/device-side/hishell/samples/samples.db?sqlQuery="; 
    var url = encodeURI(baseUrl + cmd);
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
                for (var i=0; i < obj.rows.length; i++)
                {
                    var line = obj.rows[i];
                    addRow(line.Id, line.Title, line.Author, line.Isbn);
                }
            }
        }
    }
    xmlHttp.send( null );
}

function onLoad() {
}


window.addEventListener('DOMContentLoaded', onLoad);

