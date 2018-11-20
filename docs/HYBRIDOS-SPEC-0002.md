# HybridOS Foundation Class Library

HybridOS Specification 0002<br/>
Author: Vincent Wei<br/>
Category: App Framework<br/>
Date: November 2018<br/>
Status: Proposal

Copyright Notice

  Copyright (C) 2018 Beijing FMSoft Technologies. All Rights Reserved.

## Introduction

HFCL (HybridOS Foundation Class Library) is a C++ class library. It is derived
from mGNGUX, a C++ library for MiniGUI. mGNGUX had been used as the foundation
of two MMI (man-machine interface) solutions for feature phones.

mGNGUX introduced a new framework for feature phone, and borrowed some concepts
from other operating system (especially Android). mGNGUX also provided a new set
of controls/widgets in C++ classes. These classes construct the view hierarchy
of mGNGUX app framework.

The most important feature of mGNGUX is it introduced a new way to define 
the UI elements, including resource, views, and menus. And we can easily define
the maps of views to C++ class member variables. Indeed, the UI definition files
are C++ source files, but we wrote them in pre-defined C++ macros. This method
makes there is no any performance loss when the program loads the UI elements.
At the same time, we get a good separation of the data (UI resource) and the code 
(the application logics). 

However, there are some disadvantages in the design of mGNGUX, such as it does
not use a style definition method like CSS, some design are specific to feature
phone, there is no support for touch panel, and so on.

So we design HFCL, as the successor of mGNGUX. 

The key features of HFCL are follow:

* It acts as the framework for HybridOS native app.
* It provides a complete set of rich views with support for Style Set.
* It is the foundation of Hybrid Engine.
* It is cross multiple windowing platforms including MiniGUI, Windows, macOS,
  Gtk+, and others.

## Define UI resource

HFCL uses pre-defined C++ macros to define the UI resource. For our user list sample,
the UI resource file will be like:

    begin_app(firstSample)

        begin_assets()
            image(defAvatar, "assets/def-avatar.png")

            begin_style_set(userAvatar)
                style(Display,      HS_DISPLAY_INLINE_BLOCK)
                style(Position,     HS_POSITION_RELATIVE)
                style(Margin,       HS_METRICS_UNIT_PX, 0, 0, 0, 0)
                style(Padding,      HS_METRICS_UNIT_PX, 0, 0, 0, 0)
                style(Height,       HS_METRICS_UNIT_PX, 20)
                style(Width,        HS_METRICS_UNIT_PX, 20)
                style(BorderRadius, HS_METRICS_UNIT_PX, 5, 5, 5, 5)
            end_style_set

            begin_style_set(userName)
                style(Display,      HS_DISPLAY_INLINE_BLOCK)
                style(Color,        Color::BLACK)
                style(Background,   Color::WHITE, ...)
                style(Height,       HS_METRICS_UNIT_PX, 20)
                style(Width,        HS_METRICS_UNIT_PX, HS_METRICS_AUTO)
            end_style_set

            begin_style_set(userItem)
                style(Display,      HS_DISPLAY_BLOCK)
                style(Height,       HS_METRICS_UNIT_PX, 20)
                style(Width,        HS_METRICS_UNIT_PX, 120)
                style(Border,       HS_BORDER_SOLID, Color::GRAY, 1)
                style(BorderRadius, HS_METRICS_UNIT_PX, 5, 5, 5, 5)
            end_style_set

            begin_style_set(activeItem)
                style(Color,        Color::WHITE)
                style(Background,   Color::BLUE, ...)
            end_style_set

            begin_style_set(userList)
                style(Display,      HS_DISPLAY_BLOCK)
                style(Color,        Color::BLACK)
                style(Background,   Color::WHITE, ...)
                style(Margin,       HS_METRICS_UNIT_PX, 5, 10, 5, 10)
                style(Padding,      HS_METRICS_UNIT_PX, 10, 10, 10, 10)
                style(Width,        HS_METRICS_UNIT_PX, HS_METRICS_AUTO)
                style(Height,       HS_METRICS_UNIT_PX, 100)
                style(Overflow,     HS_OVERFLOW_SCROLL)
            end_style_set

            begin_style_set(panelHeader)
                ...
            end_style_set

            begin_style_set(panelFooter)
                ...
            end_style_set
        end_assets

        begin_activity(userList)
            def_name(thePanel)
            def_name(theHeader)
            def_name(theList)
            def_name(theFooter)

            begin_view_template(UserItemView, ItemView, my_style_set(normalItem))
                begin_view(HiddenView, NULL))
                    set(Name, "id")
                end_view
                begin_view(ImageView, my_style_set(userAvatar)))
                    set(Name, "avatar")
                end_view
                begin_view(TextView, my_style_set(userName)))
                    set(Name, "name")
                end_view
            end_view_template

            begin_view(PanelView, my_style_set(panel))
                map(my(thePanel))
                begin_view(TextView, my_style_set(panelHeader))
                    map(my(theHeader))
                    set(Content, STRID_TITLE)
                end_view
                begin_view(ListView, my_style_set(userList))
                    map(my(theList))
                    set(ItemTemplate, my_template(UserItemView))
                    set(Name, "userItem")
                end_view
                begin_view(TextView, my_style_set(panelFooter))
                    map(my(theFooter))
                    set(Content, STRID_COPYING)
                end_view
            end_view
        end_activity

        begin_activity(userInfo)
            begin_view(PanelView, ...)
                ...
            end_view
        end_activity

    end_app

This file (assume named `firstsample.res.c") will be pre-compiled by 
your C++ compiler in a magic way:

* In a C++ header file, include the resource file in the following way
  to generate the identifiers and names of the resource:

        #include "resdefines.head.h"
        #include "firstsample.res.c"
        #include "resundefines.h"

        #include "resdefines.name.h"
        #include "firstsample.res.c"
        #include "resundefines.h"

* In a C++ source file, include the resource file in the following way to
  generate the source code and the initialization code:

        #include "resdefines.source.h"
        #include "firstsample.res.c"
        #include "resundefines.h"

        #include "resdefines.init.h"
        #include "firstsample.res.c"
        #include "resundefines.h"

By using the method described above, we do not need a utility tool
to interpret the resource file and generate the souce code, the C++
compiler will do this for us.

The another advantage is that we seperate the resource data with the
app logical code in a good way.

The last advantage is that the resource file can be generated from
the HVML tags. Thus, if you have to use C++ to write your device app
because the poor hardware performance, you can easily keep up your C++
code with the JavaScript code.

## Style Set

If you have some expirience with HTML and CSS, you will easily get the
the define the style set of views with the same manner of CSS. For example,
the style set of the user name corresponds to the following CSS definition:

    .userAvatar {
        display: inline-block;
        position: relative;
        margin: 0px 0px 0px 0px;
        padding: 0px 0px 0px 0px;
        height: 20px;
        width: 20px;
        border-radius: 5px 5px 5px 5px;
    }

For performance reason, we do not use the direct literal CSS definition in HFCL.
We use the marcros to define every style element in C++ way instead:

    begin_style_set(userAvatar)
        style(Display,      HS_DISPLAY_INLINE_BLOCK)
        style(Position,     HS_POSITION_RELATIVE)
        style(Margin,       HS_METRICS_UNIT_PX, 0, 0, 0, 0)
        style(Padding,      HS_METRICS_UNIT_PX, 0, 0, 0, 0)
        style(Height,       HS_METRICS_UNIT_PX, 20)
        style(Width,        HS_METRICS_UNIT_PERCENT, 100)
        style(BorderRadius, HS_METRICS_UNIT_PX, 5, 5, 5, 5)
    end_style_set

In the definition of a view which uses a style set, we passed the name of
the style set in the following way:

    begin_view(ImageView, my_style_set(userAvatar)))
        ...
    end_view

### Translate HVML and CSS

It will be a trouble if we write the style sets and views by hand in the way
above. So we introduce a interpreting tool which can translate the HVML tags
and CSS into the HFCL resource source files.

This tool is called `hfclify`, and it will be written in Python.

### Change the style set on the fly

If you want to change the style set of one view on the fly, you can call
one of the following APIs of HFCL:

    view->addStyleSet (HFCL_STYLE_SET_NAME (firstSample, activeItem));
    view->useStyleSet (HFCL_STYLE_SET_NAME (firstSample, focusItem));
    view->setStyleProperty (HCFL_STYLE_COLOR, Color::WHITE);

### View template

In HFCL, a view template is defined as a derived class of one standard view,
as shown in the sample:

    begin_view_template(UserItemView, ItemView, my_style_set(normalItem))
        ...
    end_view_template

The `UserItemView` will be defined by the macro as a new view class
in the resource header file, just a simply subclass of ItemView:

    class UserItemView : public ItemView {
        public:
            UserItemView (View* parent) : ItemView (parent) { }
            UserItemView (View* parent, StyleSet* style_set) : ItemView (parent, style_set) { }
            virtual ~UserItemView () {};
    }

----
The `UserItemView` will be defined by the macro as an interface class
in the resource header file. You should define the implemetation class
by yourself (in separated files):

    class UserItemViewIf : public ItemView {
        public:
            UserItemViewIf (View* parent) : ItemView (parent) { }
            UserItemViewIf (View* parent, StyleSet* ss) : ItemView (parent, ss) { }

            virtual View* getChildByName (const char* name) = 0;
            virtual ~UserItemViewIf () {};
    }

    class UserItemViewIm : public UserItemViewIf {
        public:
            UserItemViewIm (View* parent);
            UserItemViewIm (View* parent, StyleSet* ss);
            virtual View* getChildByName (const char* name);
            ...
            
        private:
            ...
    }
----

Note that the resource source file can not reflect all details in your 
HVML tags. You need to write the interaction code in your C++ source
file. For example, in HVML tags, you can define the iteration of
an item from a template view by using the property `hbd-iterate-by`,
but in HFCL, we can not do this for you. You need to initialize the
list view in your implementation classes manually.

When you re-generate the resource source file from HVML tags, 
because the real implemenations are seperated from the
resource source files, the translator will not override your own code.

## Assets management

Not like HVML apps, the link of an asset (e.g., an image file) defined in
a HFCL app is handled as a reference key first. We can integrate the assets
data into the executable image of your app. We call them as in-core resource.
If HFCL could not load a specific asset from the in-core resource, it will
try to load if from the URL specified in the `begin_assets` section.

In this way, HFCL will provide a maximal flexibility for the app developers.

## L10N

For the L10N (localization) text, HFCL uses identifiers instead of text
as GNU `gettext` does.

The interpreter `hfclify` can generate the C++ L10N translation 
source files for the app according to the L10N translation file specified
in the assets section of the app.

The identifiers referred as `STRID_TITLE` or `STRID_COPYING` will be
defined as C++ macros, and the translation text can be zipped by
using `zlib`.

## Components of HFCL

The components of HFCL are classified into the folllowing categories:

1. Basic utility classes
    * The simplified implementation of string, list, vector, and map. 
      You can also configure HFCL to use the STL (standard template library).
    * The common classes: Object, Event, EventListner, Date, Time, UrlParser, and so on.

1. System classes
    * File and filesystem
    * SQLite queries
    * Key-value queries
    * hBus event and task management
    * AsyncTask management
    * Peripheral management

1. Networking classes
    * WebSocket client
    * MQTT client

1. App framework
    * Graphics
    * Resource management
    * Style sheets
    * View hierarchy
    * Activity and intent
    * Animation and transition

