///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/**
 * \file config.h
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file includes some interfaces used by system manager process.
 *
 \verbatim

    This file is part of HybridOS, a developing operating system based on
    MiniGUI. HybridOs will support embedded systems and smart IoT devices.

    Copyright (C) 2002~2020, Beijing FMSoft Technologies Co., Ltd.
    Copyright (C) 1998~2002, WEI Yongming

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

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/blog/minigui-licensing-policy/>.

 \endverbatim
 */

/*
 * $Id: config.h 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

// some macro of animation 
#define DIRECTION_SHOW      0               // show the main window
#define DIRECTION_HIDE      1               // hide the main window

#define STATUSBAR_ANIMATION_TIME    1000    // 1000 ms
#define DOCKBAR_ANIMATION_TIME      500     // 500 ms

// some macro of physical dimension 
#define MARGIN_STATUS       2               // margin of text area in status bar
#define MARGIN_DOCK         15              // margin of button in dock bar
#define HEIGHT_STATUSBAR    40              // height of status bar 
#define HEIGHT_DOCKBAR      60              // height of dock bar
#define TIME_INFO_X         100             // the width of time area in status bar
#define DOCK_ICON_WIDTH     42              // the width of icon in dock bar
#define DOCK_ICON_HEIGHT    42              // the height of icon in dock bar

// control ID in dock bar
#define BUTTON_COUNT        6               // the number of button on the dock bar
#define ID_DISPLAY_BUTTON   0               // show and hide button
#define ID_HOME_BUTTON      1               // home button
#define ID_TOGGLE_BUTTON    2               // toggle apps
#define ID_SETTING_BUTTON   3               // system settings
#define ID_ABOUT_BUTTON     4               // about button, navigate to hybridos.com
#define ID_SHUTDOWN_BUTTON  5               // shutdown the device

// timer
#define ID_TIMER            100             // for time display
#define ID_SHOW_TIMER       101             // for display status and dock bar

// default string in status bar, when none app exists.
#define STRING_OS_NAME      "Hybrid OS V1.1.0"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

HWND create_status_bar (void);
HWND create_dock_bar (void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _CONFIG_h */
