# HybridOS Foundation Class Library

## Introduction

HybridOS Foundation Class Library (HFCL for short) is a C++ class library.
It is derived from mGNGUX, a C++ library for MiniGUI. mGNGUX had been used
as the foundation of two MMI (man-machine interface) solutions for
feature phones which are based on MiniGUI and a certain RTOS.

mGNGUX introduced a new framework for feature phone, and borrowed some
concepts from other operating system (especially Android). mGNGUX also
provided a new set of controls/widgets in C++ classes. These classes
construct the view hierarchy of mGNGUX app framework.

The most important feature of mGNGUX is it introduced a new way to define
the UI elements, including resource, views, and menus. And we can easily
define the maps of views to C++ class member variables. Indeed, the UI
definition files are C++ source files, but we wrote them in pre-defined C++
macros. This method makes there is no any performance loss when the
program loads the UI elements. At the same time, we get a good separation
of the data (UI resource) and the code (the application logics).

However, there are some disadvantages in the design of mGNGUX, such as
it does not use the style definition method like CSS, some design are
specific to feature phone, there is no support for touch panel, and so on.

So we design HFCL as the successor of mGNGUX.

The key features of HFCL are follow:

* It acts as the framework for HybridOS native app.
* It provides a complete set of rich views with support for CSS
  (Cascading Style Sheet).
* It is the foundation of Hybrid App Engine.
* It will be cross multiple windowing platforms including MiniGUI, Windows, macOS,
  Gtk+, and others.

## Current status

At present, HFCL is under development actively. We do not recommend to use
HFCL in real products.

## Building for Ubuntu Linux

### Prerequisites

1. Ubuntu Linux 16.04 or later
1. GVFB V1.2.2 or later
1. MiniGUI Core 4.0.0 or later

### Building

Run the following commands to install HFCL:

    $ cmake .
    $ make
    $ sudo make install

### Samples

We provide some samples for HFCL in `hybridos/device-side/samples`.

To build the samples, please change to the `samples/` directory and
refer to the `README.md` file.

## Copying

    Copyright (C) 2009~2019, Beijing FMSoft Technologies Co., Ltd.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Note that HFCL uses some open source libraries in different licenses.
These software are located in `thirdparty/`. For the copyright owners
and licenses for these software, please refer to
the `README` or `LICENSE` files contained in the source tree.

## Authors

The following people are the main developers of HFCL:

* WEI Yongming

The following people had been involved in the development of mGNGUX:

* DONG Junjie
* YAN Xiaowei
* WANG Jian
* HE Jia
* WANG Yulong
* ZHONG Dalin
