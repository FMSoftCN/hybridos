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
 * \file sysconfig.h
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file includes some interfaces used by Hybrid OS.
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
 * $Id: sysconfig.h 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
 */

#ifndef _SYSTEM_CONFIG
#define _SYSTEM_CONFIG

// configuration file
#define SYSTEM_CONFIG_FILE  "hybridos.cfg"  // configuration file

// user define message
#define MSG_MAINWINDOW_CHANGE       (MSG_USER + 1)
#define MSG_DYBKGND_DO_ANIMATION    (MSG_USER + 2)
#define MSG_BROWSER_SHOW            (MSG_USER + 3)

// Customer Require Id
#define FIXED_FORMAT_REQID          (MAX_SYS_REQID + 1)
#define UNFIXED_FORMAT_REQID        (MAX_SYS_REQID + 2)

// Customer sub require Id
#define REQ_SUBMIT_STATUSBAR_ZNODE  0   // status bar send znode index to server
#define REQ_GET_TOPMOST_TITLE       1   // get topmost normal window title
#define REQ_SUBMIT_TOGGLE           2   // toggle the application
#define REQ_SHOW_HOME_PAGE          3   // show home page
#define REQ_SHOW_SETTING_PAGE       4   // show setting page
#define REQ_SHOW_ABOUT_PAGE         5   // show about page


// Customer compositor name
#define MIME_COMPOSITOR         "mine-compositor"

typedef struct tagSysConfig 
{
    int iSystemConfigClientID;  // clientID of system manager process
    int iDyBKGndClientID;       // clientID of dynamic background process
    int iBrowserClientID;       // clientID of browser process
    pid_t iSystemConfigPid;     // pid of system manager process
    pid_t iDyBKGndPid;          // pid of dynamic background process
    pid_t iBrowserPid;          // pid of browser process
    int iStatusBarZNode;        // z node index of status bar
    HWND hWndStatusBar;         // the handle of status bar, it is invlid in sysmgr process
    int iTopmostClientID;       // topmost client ID of topmost process
} SysConfig;



typedef struct tagRequestInfo 
{
    int id;                     // sub request ID
    HWND hWnd;                  // the window handle of the sending window
    unsigned int iData0;
    unsigned int iData1;
    unsigned int iData2;
    unsigned int iData3;
} RequestInfo;

typedef struct tagReplyInfo 
{
    int id;                     // sub request ID
    unsigned int iData0;
    unsigned int request1;
    unsigned int request2;
    unsigned int request3;
} ReplyInfo;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SYSTEM_CONFIG */
