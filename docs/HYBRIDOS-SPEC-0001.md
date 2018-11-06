# HybridOS App Framework

HybridOS Specification 0001<br/>
Author: Vincent Wei<br/>
Category: App Framework<br/>
Date: November 2018<br/>
Status: Proposal

Copyright Notice

  Copyright (C) 2018 Beijing FMSoft Technologies. All Rights Reserved.

## Introduction

Considering the performance and differences in computing power of different
IoT hardware platforms, HybridOS will provide a complete new GUI framework
for device apps and client apps. The key features follow:

1. If the hardware has enough computing power (at lease 64MB RAM and 600MHz CPU),
   the developer can write the device apps in JavaScript language. Or, the developer
   falls back to write the device apps in C++ language.

2. For the device apps wrotten in JavaScript language, the developer uses HVML
   (HybridOS View Markup Language, which defines a set of extended HTML5 tags)
   to describe the GUI. HybridOS provides an agent like a Web browser. 
   The agent contains the V8 JS engine and a HVML (a customized
   markup language) renderer. We name the agent as 'Hybrid Engine' (HE for short).

3. The developer can always describe the UIs of a device app in HVML combined with CSS,
   and
    - the tags can be interpreted to one or more C++ source files, and every UI can be
      mapped to a C++ class, the DOM elements in a UI can be mapped to the member
      variables of the C++ class. So the developer can easily populate the UIs by using
      C++ language.
    - the tags can be embedded in a standard HTML5 webpage, and the developer can
      populate the tags by using a HybridOS JavaScript library and combined with a 
      CSS file, like AngularJS does.

4. The device app wrotten in JavaScript can run on client (Linux/Windows/macOS/Android/iOS)
   directly. Under this situation, the developer need a HE implementation or SDK for
   the specific client operating system.

In this way, we can use the same UI description files for device apps, client apps, and
even web apps. When the target hardware has not enough performance, we fall back to use
C++ language directly. However, we can still describe the UIs in HVML.

This specification describes the extended HTML5 tags (HVML tags) supported by HybridOS,
and the framework to define a device app in JavaScript language.

## The HVML Tags

`HVML` means HybridOS View Markup Language, which defines some extended HTML5 tags.
There are three main tags: `app`, `act`, and `view`.

* app: define an app.
* act: define an activity.
* view: define a view.

For example, to define a UI, we use the HVML tag `view`:

    <view hbd-type="panel" hbd-name="main">
    </view>

We introduce some properties for view and act tags:

1. hbd-name: The name of a view and an activity. Generally, the value of this
   property will be a variable which you can refer to in your JavaScript or C++
   program.
2. hbd-type: The view type. It specifies the type of a view. Currently, HybridOS
    provides the following view types:
    * InvisibleView
    * PanelView
    * ScrollView
    * ImageView
    * AnimationImageView
    * TextView
    * ListView
    * ItemView
    * SlEditView
    * MlEditView
    * ...

### act

### view

### Properties

### Differences from the standard HTML5 tags

We DO NOT use text in the tags like:

    <view>I am Vincent</view>

Therefore, the DOM tree does not contain any text element. However, the HybridOS JavaScript
library will create the text elements if need when we embedded a HybridOS activity in a HTML5
webpage. For example, if there is a TextView.

## A Sample

In this section, we provide a sample to show how the HybridOS View Markup Language work.
In this sample, we show a user list with avatars and user names. When you clicked on a
user item, the app will show the detailed information of the user.

### Define activities

The following markup statments define the user list activity:

    <!-- The user list activity -->
    <act hbd-name="userList" hbd-app="firstSample">

        <!-- define a customized userItem view based on the standard item view for future use -->
        <view hbd-type="userItem:item" class="" >
            <view hbd-type="hidden" hbd-name="id" />
            </view>
            <view hbd-type="image" hbd-name="avatar" />
            </view>
            <view hbd-type="text" hbd-name="name" />
            </view>
        </view>

        <view hbd-type="panel" hbd-name="main" class="">
            <view hbd-type="list" hbd-name="userList" class="">
                <!-- use userItem view and iterate the view with an array variable: users -->
                <view hbd-type="userItem" hbd-name="userItem" hbd-iterate-by="users" class="">
                </view>
            </view>
        </view>

        <script>
            /* get the app object */
            var app = hybridos.app ('firstSample');

            /* create the activity object and initialize the variables of the activity */
            var act = app.activity ('userList', function ($app, $activity, $intent) {
                $activity.users [] = {avatar: "http://www.avatar.org/a.png", name: "John"};
                $activity.users [] = {avatar: "http://www.avatar.org/b.png", name: "Tom"};
            });

            /* bind the click event of userItem */
            act.userItem.on ('click', function ($item) {
                /* launcher userInfo activity */
                app.launchActivity ("userInfo", {id: $item.id});
            });

            /* show the view named 'userList' */
            act.showView ("userList");
        </script>
    </act>

The following markup statments define the user information activity:

    <!-- The user information activity -->
    <act hbd-name="userInfo" hbd-scope="firstSample">
        <intent>
            <meta name="id" content="" />
        </intent>

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

            /* show the view named 'userInfo' */
            act.showView ("userInfo");
        </script>
    </act>

### Define app

As you can see from the sample code above, we need to define an app object as the
entry of the app and control the different activities.

An app can be defined by using the following markup statments:

    <app hbd-name="firstSample">
        <!-- define the assets of the app, such as the activities, the UI resource, and so on -->
        <assets>
            <meta type="act" name="userList" content="userlist.hvml" />
            <meta type="act" name="userInfo" content="userinfo.hvml" />
            <meta type="img" name="defAvatar"
                content="http://hyridos.fmsoft.cn/samples/firstSample/assets/def-avatar.png" />
        </assets>

        <script>
            /* create the app object */
            var app = hybridos.app ('firstSample');

            /* launch the userList activity */
            app.launchActivity ("userList", {});
        </script>
    </app>

Obviously, if you use the method above to define the app, you need prepare three files:

* firstsample.hvml: the app;
* userlist.hvml: the userList activity;
* userinfo.hvml: the userInfo activity.

Otherwise, if your app is a simple one, you can organize your code in the following manner
to use only one file:

    <app hbd-name="firstSample">
        <assets>
            <meta type="act" name="userList" content="#" />
            <meta type="act" name="userInfo" content="#" />
            <meta type="img" name="defAvatar"
                content="http://hyridos.fmsoft.cn/samples/firstSample/assets/def-avatar.png" />
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
                    <meta type="act" name="userList" content="#" />
                    <meta type="act" name="userInfo" content="#" />
                    <meta type="img" name="defAvatar"
                        content="http://hyridos.fmsoft.cn/samples/firstSample/assets/def-avatar.png" />
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

## Relationship with C++ App Framework

Under C++ language, all the view types are implemented by the HybridOS 
Foundation C++ Class Library (`HFCL` for short, see HYBRIDOS-SPEC-0002.md).
HFCL is derived a C++ GUI framework called mGNGUX, which is based on MiniGUI.
We re-designed HFCL to make this library can be ported easily to other
host operating system, for example, Windows or macOS.

Indeed, Hybrid Engine is developed on HFCL.

