# HybridOS App Framework

HybridOS Specification 0001  
Author: Vincent Wei  
Category: App Framework  
Date: November 2018  
Status: Proposal

*Copyright Notice*

Copyright (C) 2018 [FMSoft Technologies]  
All Rights Reserved.

## Introduction

Considering the performance and differences in computing power of different
IoT hardware platforms, HybridOS will provide a complete new GUI framework
for device apps and client apps. The key features follow:

1. If the hardware has enough computing power (at lease 64MB RAM and 600MHz CPU),
   the developer can write the device apps in JavaScript language. Or, the developer
   falls back to write the device apps in C++ language.

2. For the device apps written in JavaScript language, the developer uses HVML
   (HybridOS View Markup Language, which defines a set of extended HTML5 tags)
   to describe the GUI. HybridOS provides a user agent like a Web browser. 
   The user agent contains the V8 JS engine and a HVML (a customized
   markup language) renderer. We name the user agent as 'HybridOS App Engine'
   (`HAE` for short).

3. The developer can always describe the UIs of a device app in HVML combined with CSS,
   and
    - the tags can be interpreted to one or more C++ source files, and every UI can be
      mapped to a C++ class, the DOM elements in a UI can be mapped to the member
      variables of the C++ class. So the developer can easily populate the UIs by using
      C++ language.
    - the tags can be embedded in a standard HTML5 webpage, and the developer can
      populate the tags by using a HybridOS JavaScript library and combined with a 
      CSS file, like AngularJS does.

4. The device app written in JavaScript can run on client (Linux/Windows/macOS/Android/iOS)
   directly. Under this situation, the developer need a HAE implementation or SDK for
   the specific client operating system.

In this way, we can use the same UI description files for device apps, client apps, and
even web apps. When the target hardware has not enough performance, we fall back to use
C++ language directly. However, we can still describe the UIs in HVML.

This specification describes the extended HTML5 tags (HVML tags) supported by HybridOS,
and the framework to define a device app in JavaScript language.

## The HVML Tags

`HVML` means HybridOS View Markup Language, which defines some extended HTML5 tags.
There are four main tags: `app`, `act`, `tmpl`, and `view`.

* app: define an app.
* act: define an activity.
* tmpl: define a template.
* view: define a view.

For example, to define a UI, we use the HVML tag `view`:

    <view hbd-type="panel" hbd-name="main">
    </view>

We introduce some properties for the tags:

1. hbd-name: The name of a view and an activity. Generally, the value of this
   property will be a variable which you can refer to in your JavaScript or C++
   program.
2. hbd-type: The view type. It specifies the type of a view. Currently, HybridOS
    provides the following view types:
    * HiddenView
    * ImageView
    * TextView
    * PanelView
    * ListView
    * ItemView
    * InputView
    * EditBoxView
    * CanvasView

    * ProgressView
    * DropdownView
    * TooltipView
    * PopoverView

### Tag app

### Tag act

### Tag view

### Tag tmpl

### Tag properties

### Differences from the standard HTML5 tags

We DO NOT use text in the tags like:

    <view>I am Vincent</view>

Therefore, the DOM tree does not contain any text element. However, the HybridOS JavaScript
library will create the text elements if need when we embed a HybridOS activity in a HTML5
webpage. Especially if there is a `TextView`.

## A Sample

In this section, we provide a sample to show how the HybridOS View Markup Language works.
In this sample, we show a user list with avatars and user names. When you clicked on a
user item, the app will show the detailed information of the user.

### Define activities

The following markup statements define the user list activity:

    <!-- The user list activity -->
    <act hbd-name="userList" hbd-app="firstSample">

        <!-- define a template view based on the standard item view for future use -->
        <tmpl hbd-type="UserItemView:ItemView" class="userItem">
            <view hbd-type="HiddenView" hbd-name="id" />
            </view>
            <view hbd-type="ImageView" hbd-name="avatar" />
            </view>
            <view hbd-type="TextView" hbd-name="name" />
            </view>
        </tmpl>

        <view hbd-type="panel" hbd-name="thePanel" class="panel">
            <view hbd-type="TextView" hbd-name="theHeader" hbd-content="$STRID_TITLE"
                    class="panel-header">
            </view>
            <view hbd-type="ListView" hbd-name="theList" class="userList">
                <!-- use userItem view and iterate the view with an array variable: users -->
                <view hbd-type="UserItemView" hbd-iterate-by="users">
                </view>
            </view>
            <view hbd-type="TextView" hbd-name="theFooter" hbd-content="$STRID_COPYING"
                    class="panel-footer">
            </view>
        </view>

        <script>
            /* get the app object */
            var app = hybridos.app ('firstSample');

            /* create the activity object and initialize the variables of the activity */
            var act = app.activity ('userList', function ($app, $activity, $intent) {
                $activity.users [] = {id: "1", avatar: "http://www.avatar.org/a.png", name: "John"};
                $activity.users [] = {id: "2", avatar: "http://www.avatar.org/b.png", name: "Tom"};
            });

            /* bind the click event of userItem */
            act.userItem.on ('click', function ($item) {
                /* launcher userInfo activity */
                app.launchActivity ("userInfo", {id: $item.id});
            });

            /* show the view named 'thePanel' */
            act.showView ("thePanel");
        </script>
    </act>

The following markup statements define the user information activity:

    <!-- The user information activity -->
    <act hbd-name="theUserInfo" hbd-scope="firstSample">

        <view hbd-type="panel" hbd-name="main" class="">
        </view>

        <script>
            /* get the app object */
            var app = hybridos.app ('firstSample');

            /* create the activity object and initialize the variables of the activity */
            var act = app.activity ('userList', function ($app, $activity, $intent) {
                $activity.id = $intent.id;

                /* fill the user information by calling method of $app */
                $activity.avatar = "";
                $activity.name = "";
                $activity.bio = "";
                $activity.org = "";
                $activity.email = "";
                $activity.link = "";
            });

            /* show the view named 'theUserInfo' */
            act.showView ("theUserInfo");
        </script>
    </act>

### Define app

As you can see from the sample code above, we need to define an app object as the
entry of the app and control the different activities.

An app can be defined by using the following markup statements:

    <app hbd-name="firstSample">
        <!-- define the assets of the app, such as the activities, images, L10N text, CSS, and so on -->
        <assets>
            <meta name="act:userList" content="userlist.hvml" />
            <meta name="act:userInfo" content="userinfo.hvml" />
            <meta name="img:defAvatar" content="/firstSample/assets/default.png" />

            <!-- the links to the localization translation files */
            <link rel="localtext" type="text/json" hreflang="zh_CN"
                    href="/firstSample/assets/messages/zh_CN.json" />
            <link rel="localtext" type="text/json" hreflang="zh_TW"
                    href="/firstSample/assets/messages/zh_TW.json" />
            <link rel="localtext" type="text/json" hreflang="en_US"
                    href="/firstSample/assets/messages/en_US.json" />

            <link rel="stylesheet" type="text/css" href="/firstSample/assets/default.css" />
        </assets>

        <script>
            /* create the app object
             *  'firstSample': the name of app.
             *  'zh_CN': the initial locale name. The locale name will be used to load
             *      the real L10N text file.
             */
            var app = hybridos.app ('firstSample', 'zh_CN');

            /* launch the userList activity */
            app.launchActivity ("userList", {});
        </script>
    </app>

Obviously, if you use the method above to define the app, you need prepare three files:

* firstsample.hvml: the app;
* userlist.hvml: the `userList` activity;
* userinfo.hvml: the `userInfo` activity.

Otherwise, if your app is a simple one, you can organize your code in the following manner
to use only one file:

    <app hbd-name="firstSample">
        <assets>
            <meta name="act:userList" content="userlist.hvml" />
            <meta name="act:userInfo" content="userinfo.hvml" />
            <meta name="img:defAvatar" content="/firstSample/assets/default.css" />

            <link rel="localtext" type="text/json" href="/firstSample/assets/messages/zh_CN.json" />
            <link rel="stylesheet" type="text/css" href="/firstSample/assets/default.css" />
        </assets>

        <act hbd-name="userList" hbd-app="firstSample">
            ...
        </act>

        <act hbd-name="userInfo" hbd-app="firstSample">
            ...
        </act>

        <script>
            ...
        </script>
    </app>

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

            <app hbd-name="firstSample">
                <assets>
                    ...
                </assets>

                <act hbd-name="userList" hbd-app="firstSample">
                    ...
                </act>

                <act hbd-name="userInfo" hbd-app="firstSample">
                    ...
                </act>

                <script>
                    ...
                </script>
            </app>

            <footer>
                ...
            </footer>
        </body>
    </html>

Note that we import `hybridos.css` and `hybridos.js` in the head of the HTML5 page.

## HVML and CSS

HybridOS uses CSS to define the style of all view types. You can apply your own
CSS to any HVML element as well.

## L10N

Different from webpages, HVML handles the L10N text as an asset of an app. 
HAE (the HVML user agent) loads the L10N file in JSON and translate the
localization text according to the identifier.

If you specify a content of `TextView` with a string started with `$`, the
string will be treated as the identifier of a text instead of the literal text.

The content of a L10N text file will look like:

    {
        "app": "firstSample",
        "locale": "zh_CN",
        "STRID_TITLE": "HybridOS 的第一个示例应用",
        "STRID_COPYRING": "版权所有 (C) 2018 飞漫软件",
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

HybridOS will provide the following exteneded JavaScript Objects for app:

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
