# hiWebKit, the Key Component of HybridOS, Released Yesterday

On Aug. 28, 2020, FMSoft released hiWebKit, the key component of the HybridOS.
This is another masterpiece of the development team since it announced
the open source collaboration plan for HybridOS in 2018. The team released
Graphics Stack for HybridOS in Dec. 2019, and released MiniGUI 5.0 in Mar. 2020.

hiWebKit is a derivative version of the well-known open source browser engine
[WebKit](https://www.webkit.org), which includes many extensions made by the
HybridOS development team on the basis of existing HTML/CSS standards and
specifications in the past year:

1. Extensible support for complex widgets. This feature is equivalent to providing
a lightweight plug-in mechanism for the browser. Developers can take advantage of
this feature to develop complex widgets that meet their needs and avoid using solutions
that have a serious impact on performance, such as traditional Netscape plug-ins.
1. Introduce new tags and CSS properties to support the spinning wheel widget.
Developers can use this feature to easily write wheel widgets for selecting time,
administrative divisions, etc.
1. Introduce new tags and CSS properties to support rotating meter widgets.
Developers can use this feature to easily write an interface that can show
rotating progress, such as a car dashboard.
1. Binding mechanism of attributes or content between elements. By defining the
binding relationship between HTML elements, the user agent automatically completes
the synchronization of attributes or content between the bound elements.
Developers can use this feature to create content between two elements without
using JavaScript.
1. Local dynamic generation technology of HTML content. Through the data mapping
technology, the dynamic adjustment of the DOM tree by the user agent can be realized
without script intervention. Developers can take advantage of this feature to
directly generate HTML document content based on JSON data without using JavaScript
scripts or background scripts.
1. Extended schema (LSQL and RSQL protocols) for direct access to a SQL database.
Developers can use this feature to directly use SQL statements to access local
or remote SQL databases without the need to develop background scripts.
1. Extended schema (LCMD protocol) for executing local system commands. Developers
can use this feature to directly call a local command line program in the browser,
and use its output as the input of the web page to generate page content in place,
without the need for background scripts.

The technical details of these extensions and the benefits to developers are
introduced in the album [Technical Breakthroughs in HybridOS (Chinese Version)](https://mp.weixin.qq.com/mp/appmsgalbum?action=getalbum&album_id=1442713925182767106&__biz=MzA5MTYwNTA3MA==#wechat_redirect). Interested readers can click to read.

To try hiWebKit, please visit the code repository on GitHub of HybridOS (**Please light up the stars in the HybridOS project repository**):

<https://github.com/FMSoftCN/hybridos>

And build the code of HybridOS for device side as described in the following README document:

<https://github.com/FMSoftCN/hybridos/tree/master/device-side>

At present, the development team is further improving hiWebKit. In conjunction
with other components of HybridOS (such as MiniGUI, hiBus, etc.),
our goal is that one developer can write GUIs by using HybridOS in extended HTML/CSS
but without using JavaScript. This will greatly reduce the size and the memory usage of HybridOS.

We expect to release the first official version of HybridOS at the end of 2020, and
will give priority to support the processor platforms of our chip partners.

