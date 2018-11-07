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
* It provides a complete set of rich views with support for CSS.
* It is the foundation of Hybrid Engine.
* It is cross multiple windowing platforms including MiniGUI, Windows, macOS,
  and others.

## Define UI resource

HFCL uses pre-defined C++ macros to define the UI resource. For our user list sample,
the UI resource file will be like:

    begin_app(firstSample)

        begin_assets()
            image(defAvatar, "http://hyridos.fmsoft.cn/samples/firstSample/assets/def-avatar.png")
            css("http://hyridos.fmsoft.cn/samples/firstSample/assets/plain.css")
        end_assets

        begin_activity(userList)
            def_name(list)
            def_name(list_event)

            begin_view_template(UserItemView, ItemView)
                begin_view(HiddenView, NULL))
                    set(Name, "id")
                end_view
                begin_view(ImageView, "css-class-name"))
                    set(Name, "avatar")
                end_view
                begin_view(TextView, "css-class-name"))
                    set(Name, "name")
                end_view
            end_view_template

            begin_view(ListView, "userList", "css-class-name")
                begin_view(ListView, "css-class-name")
                    map(my(list))
                    on(CustomEvent::CUSTOM_NOTIFY, my(list_event))
                end_view
            end_view
        end_activity

        begin_activity(userInfo)
            begin_view(PanelView, "userInfo", "css-class-name")
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
    #include "phonebook.res.c"
    #include "resundefines.h"

    #include "resdefines.init.h"
    #include "phonebook.res.c"
    #include "resundefines.h"

By using the magic way described above, we do not need a utility tool
to interpret the resource file and generate the souce code, the C++
compiler will do this for us.

The another advantage is that we seperate the resource data with the
app logical code in a GREAT way.

The last advantage is that the resource file can be generated from
the HVML tags. Thus, if you have to use C++ to write your device app
because the poor hardware performance, you can easily keep up your C++
code with the JavaScript code.

## CSS

## Assets management

