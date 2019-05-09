# HybridOS App Framework

HybridOS Specification 0001  
Author: Vincent Wei  
Category: App Framework  
Date: November 2018  
Status: Proposal

*Copyright Notice*

Copyright (C) 2018, 2019 [FMSoft Technologies]  
All Rights Reserved.

## Introduction

Considering the performance and differences in computing power of different
IoT hardware platforms, HybridOS will provide a complete new GUI framework
for device apps and client apps. The key features follow:

1. If the hardware has enough computing power (at lease 64MB RAM and 600MHz CPU),
   the developer can write the device apps in JavaScript language. Or, the developer
   falls back to write the device apps in C++ language.

2. For the device apps written in JavaScript language, the developer uses HVML
   (HybridOS View Markup Language, which defines a few new tags based on HTML 5.2)
   to describe the GUI. HybridOS provides a user agent like a Web browser, which
   contains the V8 JS engine and the HVML renderer. We name the user agent as
   'HybridOS App Engine' (`HAE` for short).

3. The developer can always describe the UIs of a device app in HVML combined with CSS,
   and the HVML documents and CSS definitions can be interpreted to one or
   more C++ source files, the developer can manipulate all DOM elements,
   their attributes, styles, and content like JavaScript does.

4. The device app written in JavaScript can run on client (Linux/Windows/macOS/Android/iOS)
   directly. Under this situation, the developer need a HAE implementation or SDK for
   the specific client operating system.

In this way, we can use the same UI description files for device apps, client apps, and
even web apps. When the target hardware has not enough performance, we fall back to use
C++ language directly. However, we can still describe the UIs in HVML and CSS.

This specification describes the extended HTML 5.2 tags (HVML tags) supported by
HybridOS, and the framework to define a device app in JavaScript language.

## The HVML Tags

`HVML` means HybridOS View Markup Language, which defines a few new tags based on
[HTML 5.2].

The HVML tag `view` represents a complex widget which can not be described and
rendered easily by the standard HTML 5.2 tags and CSS, for example, a chart,
a calendar, a meter panel, and so on.

For example, to define a meter panel, we use the following HVML tag `view`:

    <view type="meter" class="CLASS" name="NAME" id="IDENTIFIER" />

As we know, HTML 5.2 introduced some new tags especially the ones for interaction,
such as `details`, `summary`, and `dialog`. HVML provides the support for these
tags. However, if we want to show a complex widget, we either use a plugin or
use the `canvas` to render them by using JavaScript. In order to reduce the
development efforts, we introduce the `view` tag for HVML.

HTML 5.2 also introduced the `template` tag. A template is a virtual element,
which can be use to generate other real elements by substituting some
attributes and inserting them to the DOM tree.

However, you still need to write a piece of script code to clone the
template in HTML. But in HVML, you can use the following new tags to
clone a template element without the script code:

* `iterator`: define an iterator.
* `subsitute` and `condition`: define conditional substitution.

The tags above use the metadata defined in the head element of a HVML document.

For example:

    <!DOCTYPE hvml>
    <hvml>
        <head>
            <meta name="users"  content="<JSON>" once />
            <meta name="global" content="<JSON>" />
        </head>

        <body>

            <template id="footer-cn">
                <p><a href="http://www.baidu.com">Baidu</a></p>
            </template>

            <template id="footer-tw">
                <p><a href="http://www.bing.com">Bing</a></p>
            </template>

            <template id="footer-def">
                <p><a href="http://www.google.com">Google</a></p>
            </template>

            <template id="user-item">
                <li>
                    <data data-key="id" data-attr="value"></data>
                    <img data-key="avatar" data-attr="src" />
                    <p data-key="name" data-attr="content"></p>
                </li>
            </template>

            <template id="no-user">
                <li>
                    <img src="def-avatar.png" />
                    <p>Sign Up for the first user!</p>
                </li>
            </template>

            <header>
                <h1>User List</h1>
            </header>

            <main>
                <ul>
                    <iterator template="user-item" metadata="users" alt="no-user" />
                </ul>
            </main>

            <footer class="footer">
                <substitute metadata="global">
                    <condition template="footer-cn"  key="locale" value="zh_CN" />
                    <condition template="footer-tw"  key="locale" value="zh_TW" />
                    <condition template="footer-def" />
                </substitute>
            </footer>
        </body>

    </hvml>


~~
We use a HTML document to define an activity. However, different from HTML,
we can define multiple view clients for one activity, and at the same time,
only one view client can be kept active. The other view clients are virtual
and invisible from the users.

In a `view` or `template`, we use the standard sectioning and/or heading
content tags of HTML5 to define the structure of the view:

* `article`, `aside`, `nav`, `section`
* `header`, `footer`, `main`, `div`
* `ul`, `ol`, `dl`
* `address`
* `blockquote`
* `figure`
* `details`

and use the following tags to define the text paragraphs:

* `h1`, `h2`, `h3`, `h4`, `h5`, `h6`
* `p`, `prev`
* `li`, `dt`, `dd`
* `summary`
* `figcaption`

In a text paragraph, we use the phrasing content elements (text-level
elements and embedded elements) of HTML5 to define the content.
Therefore, you can use the following HTML5 tags:

* The text-level elements:
  * `a`, `abbr`, `b`, `bdi`, `bdo`, `br`, `cite`, `code`, `data`, `datalist`
  * `del`, `dfn`, `em`, `i`, `ins`, `kbd`, `label`, `mark`,
  * `output`, `q`, `ruby`, `s`, `samp`, `small`, `span`
  * `strong`, `sub`, `sup`, `time`, `u`, `var`, `wbr`

* The embedded elements:
  * `img`, `button`, `input`, `math`, `meter`, `picture`, `progress`
  * `math`, `svg`,
  * `audio`, `video`

### Tag properties

1. `class`: A global HTML5 property, which specifies the class of the view.

1. `name`: A global HTML5 property, which specifies the name of an app,
an activity, a view, or a template, Generally, the value of this property
will be a variable which you can refer to in your JavaScript or C++ code.

1. `id`: A global HTML5 property, which specifies the identifier of an app,
an activity, a view, or a template, Generally, the value of this property
will be a variable which you can refer to in your JavaScript or C++ code.

1. `content`: A global HVML property, which specifies the content of an
element.

### Differences between HVML and HTML

1. HVML is defined for user interfaces not documents

1. Easy to localize

In HTML5, we define a text paragraph like this:

    <p>
        Welcome to the world of <em>HybridOS</em>!
    </p>

When we want to localize the paragraph for other locale, we generally
generate a different webpage for the locale.

However, in HVML, we define a text paragraph in the following way:

    <p content="$Welcome to the world of <em>HybridOS</em>!">
    </p>

So a JavaScript or C++ code can easily translate the content into other
locale by using a GNU message file or a JSON table.
~~

## A Sample

In this section, we provide a sample to show how the HybridOS View Markup Language works.
In this sample, we show a user list with avatars and user names. When you clicked on a
user item, the app will show the detailed information of the user.

### Define activities

The following markup statements define the user list activity:

    <!DOCTYPE hvml>
    <!-- The user list activity -->
    <hvml>
    <head>
        <meta name="activity"   content="act-user-list" />
        <meta name="users"      content="" />
        <meta name="global"     content="" />
    </head>

    <body>

        <!-- define templates for future use -->
        <template id="user-item">
            <li class="user-item">
                <data data-key="id" data-attr="value"></data>
                <img data-key="avatar" data-attr="src" />
                <span data-key="name" data-attr="content"></span>
            </li>
        </template>

        <template id="footer-cn">
            <p><a href="http://www.baidu.com">Baidu</a></p>
        </template>

        <template id="footer-tw">
            <p><a href="http://www.bing.com">Bing</a></p>
        </template>

        <template id="footer-def">
            <p><a href="http://www.google.com">Google</a></p>
        </template>

        <header>
            <h1 name="theHeader" content="$STRID_TITLE" class="panel-header"></h1>
        </header>

        <main>
            <ul>
                <iterator template="user-item" metadata="users" />
            </ul>
        </main>

        <footer class="footer">
            <substitute metadata="global">
                <condition template="footer-cn"  key="locale" value="zh_CN" />
                <condition template="footer-tw"  key="locale" value="zh_TW" />
                <condition template="footer-def" />
            </substitute>
            <p content="$STRID_COPYING" class="footer-copying"></p>
        </footer>

        <script>
            /* bind the click event of userItem */
            $(".user-item").on ('click', function ($item) {
                /* launcher userInfo activity */
                launchActivity ("act-user-info", {id: $item.id});
            });
        </script>
    </body>
    </html>

The following markup statements define the user information activity:

    <!DOCTYPE hvml>
    <!-- The user information activity -->
    <hvml>
    <head>
        <meta name="activity"   content="act-user-info" />
        <meta name="userinfo"   content="<JSON DATA>" />
    </head>
    <body>

    </body>
    </hvml>

### Define app

As you can see from the sample code above, we need to define an app object as the
entry of the app and control the different activities.

An app can be defined by using the following markup statements:

    <!DOCTYPE hvml>
    <hvml name="firstSample" lang="en">
        <!-- define the assets of the app, such as the activities, images, L10N text, CSS, and so on -->
        <head>
            <meta name="activity:act-user-list" content="userlist.html" />
            <meta name="activity:userInfo" content="userinfo.html" />
            <meta name="img:defAvatar" content="/firstSample/assets/default.png" />

            <!-- the links to the localization translation files */
            <link rel="localtext" type="text/json" hreflang="zh_CN"
                    href="/firstSample/assets/messages/zh_CN.json" />
            <link rel="localtext" type="text/json" hreflang="zh_TW"
                    href="/firstSample/assets/messages/zh_TW.json" />
            <link rel="localtext" type="text/json" hreflang="en_US"
                    href="/firstSample/assets/messages/en_US.json" />

            <link rel="stylesheet" type="text/css" href="/firstSample/assets/default.css" />
        </head>

        <script>
            /* create the app object
             *  'firstSample': the name of app.
             *  'zh_CN': the initial locale name. The locale name will be used to load
             *      the real L10N text file.
             */
            var app = hybridos.app ('firstSample', 'zh_CN');

            /* launch the act-user-list activity */
            app.launchActivity ("act-user-list", {});
        </script>
    </hvml>

Obviously, if you use the method above to define the app, you need prepare three files:

* `firstsample.hvml`: the app;
* `userlist.html`: the `act-user-list` activity;
* `userinfo.html`: the `act-user-info` activity.

Otherwise, if your app is a simple one, you can organize your code in the following manner
to use only one file:

    <!DOCTYPE hvml>
    <hvml name="firstSample" lang="en">
        <head>
            <meta name="activity:act-user-list" content="userlist.html" />
            <meta name="activity:act-user-info" content="userinfo.html" />
            <meta name="img:defAvatar" content="/firstSample/assets/default.css" />

            <link rel="localtext" type="text/json" href="/firstSample/assets/messages/zh_CN.json" />
            <link rel="stylesheet" type="text/css" href="/firstSample/assets/default.css" />
        </head>

        <activity id="act-user-list" hbd-scope="firstSample">
            ...
        </activity>

        <activity id="act-user-info" hbd-scope="firstSample">
            ...
        </activity>

        <script>
            ...
        </script>
    </hvml>

~~
## Embed App into HTML5 Pages

It is possible to embed a HybridOS app into an existed HTML5 page:

    <!DOCTYPE html>
    <html>
        <head>
            <meta name="viewport"
                content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no"/>
            <meta name="format-detection" content="telephone=yes"/>
            <meta name="apple-touch-fullscreen" content="yes"/>

            <title>The first sample of HybridOS embedded in a HTML5 page</title>

            <link rel="stylesheet" type="text/css" href="//hybridos.fmsoft.cn/js/hybridos.css" />
            <script type="text/javascript" src="//hybridos.fmsoft.cn/js/hybridos.js"></script>
        </head>

        <body>

            <header>
                <h1>The First Sample of HybridOS</h1>
            </header>

            <p>This page shows howto embedded a HybridOS app in a HTML5 page.</p>

            <hvapp id="firstSample">
                <assets>
                    ...
                </assets>

                <activity id="act-user-list" hbd-scope="firstSample">
                    ...
                </activity>

                <activity id="act-user-info" hbd-scope="firstSample">
                    ...
                </activity>

                <script>
                    ...
                </script>
            </hvapp>

            <footer>
                ...
            </footer>
        </body>
    </html>

Note that we import `hybridos.css` and `hybridos.js` in the head of the HTML5 page.
~~

## HVML and CSS

HybridOS uses CSS to define the style of all view types. You can apply your own
CSS to any HVML element as well.

## L10N

Different from webpages, HVML handles the L10N text as an asset of an app. 
HAE (the HVML user agent) loads the L10N file in JSON and translate the
localization text according to the identifier.

If you specify a content of `text` with a string started with `$`, the
string will be treated as the identifier of a text instead of the literal text.
To escape from the rule, use `\$` for the prefix.

The content of a L10N text file will look like:

    {
        "app": "firstSample",
        "locale": "zh_CN",
        "STRID_TITLE": "HybridOS 的第一个示例应用",
        "STRID_COPYRING": "版权所有 (C) 2018 飞漫软件",
        "Welcome to the world of <em>HybridOS</em>!": "欢迎来到 <em>HybridOS</em> 的世界！",
    }

Obviously, `app` and `locale` are reserved keywords for L10N text file.

## The HTML5 Technologies Supported

The following HTML5 Technologies will be supported by HVML in the first official release:

* canvas and canvas object.
* WebStorage (including localStorage and sessionStorage)
* WebSocket
* Geolocation

The following HTML5 Technologies will be supported in the future releases:

* SVG
* Audio
* Video

## Extended JavaScript Objects

HybridOS will provide the following extended JavaScript Objects for app:

* SQLite for local database.
* hBus for communication with the local system services.
* MQTT for communication with the cloud and clients.

## Relationship with C++ App Framework

Under C++ language, all the view types are implemented by the HybridOS 
Foundation C++ Class Library (`HFCL` for short, see [HybridOS Foundation Class Library]).
HFCL is derived a C++ GUI framework called mGNGUX, which is based on MiniGUI.
We re-designed HFCL to make this library can be ported easily to other
host operating system, for example, Windows or macOS.

Indeed, Hybrid Engine is developed based on HFCL.

[Beijing FMSoft Technologies Co., Ltd.]: https://www.fmsoft.cn
[FMSoft Technologies]: https://www.fmsoft.cn
[HybridOS Official Site]: https://hybrid.fmsoft.cn

[HybridOS Architecture]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0000.md
[HybridOS App Framework]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0001.md
[HybridOS Foundation Class Library]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0002.md


[HTML 5.2]: https://www.w3.org/TR/html52/index.html
