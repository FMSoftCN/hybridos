# HybridOS Foundation Class Library

HybridOS Specification 0002<br/>
Author: Vincent Wei<br/>
Category: App Framework<br/>
Date: November 2018<br/>
Status: Proposal

Copyright Notice

  Copyright (C) 2018 Beijing FMSoft Technologies. All Rights Reserved.

## Introduction

HFCL (HybridOS Foundation Class Library) is C++ class library. It is derived
from mGNGUX, a C++ library for MiniGUI. mGNGUX had been used as the foundation
of two MMI solutions (man-machine-interface) for feature phones.

mGNGUX introduced a new framework for feature phone, and borrowed some concepts
from other operating system (especially Android). mGNGUX also provided a new set
of controls/widgets in C++ classes. These classes construct the view hierarchy
of HybridOS app framework.

The most important feature of mGNGUX is that it introduced a new way to define 
the UI elements, including resource, views, and menus. And we can easily define
the maps of views to C++ class member variables. Indeed, the UI definition files
are C++ source files, but we wrote them in pre-defined C++ macros. This method
makes there is no any performance loss when the program loads the UI elements.
At the same time, we get a good separation of the data (UI resource) and the code 
(the application logics). 

However, there are some disadvantages in the design of mGNGUX, such as it does
not use style definition method like CSS, some design are specific to feature
phone, there is no support for touch panel, and so on.

So we design HFCL, as the successor of mGNGUX. 

The key features of HFCL are follow:

* It acts as the framework for HybridOS native app.
* It provides a complete set of rich views with support for CSS.
* It is the foundation of Hybrid Engine.
* It is cross multiple windowing platforms including MiniGUI, Windows, macOS.

## 
