# HybridOS Specification 0001

Author: Vincent Wei
Category: UI Framework
Date: November 2018
Status: Proposal

Copyright Notice

  Copyright (C) 2018 Beijing FMSoft Technologies. All Rights Reserved.

## Introduction

Considering the performance and differences in computing power of different
IoT hardware platforms, HybridOS will provide a complete new GUI framework
for device apps and client apps. The key features follow:

  1. If the hardware has enough computing power (at lease 64MB RAM and 600MHz CPU),
    the developer can write the device apps in JavaScript language. Or, the developer
    should write the device apps in C++ language.

  2. The developer can always describe the UIs of a device app in the extened HTML5
    tags combined with the CSS, and
    * the tags can be interpreted to one or more C++ source files, and every UI can be
      mapped to a C++ class, the DOM elements in a tag can be mapped to the member
      variables of the C++ class. So the developer can easily populate the UIs by using
      C++ language.
    * the tags can be embedded in a standard HTML5 webpage, and the developer can
      populate the tags by using a HybridOS JavaScript library and combined with a 
      CSS file, like AngularJS does.

  3. The device app wrotten in JavaScript can run on client (Windows/macOS/Android/iOS)
     directly. Under this situation, the developer need a HybridOS SDK for specific
     client operating system.

This specification describes the extended HTML5 tags supported by HybridOS,
and the method of binding a JavaScript variable or a C++ class member variable with
a HTML5 DOM element.

## The Extended Tags

HybridOS defines four extended tags: `act`, `view`.

  * act: define an activity.

### act

### view

## A Sample

To define a UI, we use a customized HTML5 markup tag `view`:

    <view hbd-type="panel" id="main" class="">
    </view>

For example, if we want to show a user List with avatar and user name, we use
the following markup statments:

    <!-- define a activity -->
    <act hbd-name="firstSample" hbd-scope="main" hbd-controller="myCtrl">

        <!-- define a customized userItem view based on the standard item view for future use -->
        <view hbd-type="userItem:item" class="" >
            <view hbd-type="image" hbd-name="avatar" />
            </view>
            <view hbd-type="text" hbd-name="name" />
            </view>
        </view>

        <view hbd-type="panel" hbd-name="main" class="">
            <view hbd-type="list" hbd-name="userList" class="">
                <!-- use userItem view and iterate the view with an array variable: users -->
                <view hbd-type="userItem" hbd-iterate-by="users" class="">
                </view>
            </view>
        </view>
    </act>

    <script>
        var act = hybridos.activity ('firstSample', {});
        act.controller ('myCtrl', function ($scope) {
            $scope.users [] = {avatar: "http://www.avatar.org/a.png", name: "John"};
            $scope.users [] = {avatar: "http://www.avatar.org/b.png", name: "Tom"};
        });
    </script>

## Complie the UI tags into C++ resource and class files


