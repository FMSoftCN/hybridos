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
/* 
** $Id: taskbar.c 381 2008-01-28 10:19:26Z wangjian $
**
** The taskbar of MDE
**
** Copyright (C) 2003 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <mgeff/mgeff.h>

#include "config.h"

extern HWND m_hStatusBar;                       // handle of status bar
extern HWND m_hDockBar;                         // handle of dock bar
static int m_DockBar_Height = 0;                // height of dock bar
static int m_DockBar_X = 0;                     // the X coordinate of top left corner
static MGEFF_ANIMATION m_animation = NULL;      // handle of animation
static int m_direction = DIRECTION_SHOW;        // the direction of animation

static int m_DockBar_Start_x = 0;               // it is only for convenience for animation
static int m_DockBar_Start_y = 0;
static int m_DockBar_End_x = 0;
static int m_DockBar_End_y = 0;
static int m_DockBar_Left_Length = 0;

// start another process
static pid_t exec_app (char * app)
{
    pid_t pid = 0;
    char buff [PATH_MAX + NAME_MAX + 1];

    memset(buff, 0, PATH_MAX + NAME_MAX + 1);

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        strcpy (buff, ".//");
        strcat (buff, app);
        execl (buff, app, NULL);
        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }
    return pid;
}

// callback function of animation
static void animated_cb(MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    if(m_DockBar_X != *value)
    {
        MoveWindow(hWnd, m_DockBar_X, m_DockBar_Start_y, m_DockBar_End_x - m_DockBar_Start_x, m_DockBar_Height, TRUE);
        m_DockBar_X = *value;
    }
}

// the function which will be invoked at the end of animation
static void animated_end(MGEFF_ANIMATION handle)
{
    HWND hWnd = (HWND)mGEffAnimationGetTarget(handle);
    mGEffAnimationDelete(m_animation);
    m_animation = NULL;

    if((m_direction == DIRECTION_SHOW) && hWnd)
        SetTimer(hWnd, ID_SHOW_TIMER, DOCKBAR_VISIBLE_TIME);
}

// create an animation and start, it is asynchronous
static void create_animation(HWND hWnd)
{
    if(m_animation)
    {
        mGEffAnimationDelete(m_animation);
        m_animation = NULL;
    }

    m_animation = mGEffAnimationCreate((void *)hWnd, (void *)animated_cb, 1, MGEFF_INT);
    if (m_animation) 
    {
        int start = 0;
        int end = 0;
        int duration = 0;
        enum EffMotionType motionType = InQuad;

        start = m_DockBar_X;
        if(m_direction == DIRECTION_HIDE)
        {
            end = g_rcScr.right - m_DockBar_Left_Length;
            motionType = InQuad;
            duration = DOCKBAR_ANIMATION_TIME * (g_rcScr.right - m_DockBar_Left_Length - m_DockBar_X) / (g_rcScr.right - m_DockBar_Left_Length - m_DockBar_Start_x);
        }
        else
        {
            end = m_DockBar_Start_x;
            motionType = OutQuad;
            duration = DOCKBAR_ANIMATION_TIME * (m_DockBar_X - m_DockBar_Start_x) / (g_rcScr.right -  m_DockBar_Left_Length- m_DockBar_Start_x);
        }

        if(duration == 0)
            duration = DOCKBAR_ANIMATION_TIME;

        mGEffAnimationSetStartValue(m_animation, &start);
        mGEffAnimationSetEndValue(m_animation, &end);
        mGEffAnimationSetDuration(m_animation, duration);
        mGEffAnimationSetCurve(m_animation, motionType);
        mGEffAnimationSetFinishedCb(m_animation, animated_end);
        mGEffAnimationAsyncRun(m_animation);
    }
}

static LRESULT DockBarWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int code = 0;
    int id = 0;

    switch (message) 
    {
        case MSG_CREATE:

            code = (m_DockBar_End_x - m_DockBar_Start_x) / 6;
            id = code - 2 * _MARGIN;

            CreateWindow (CTRL_BUTTON, "HIDE",     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_DISPLAY_BUTTON,  0 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);

            CreateWindow (CTRL_BUTTON, "HOME",     WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_HOME_BUTTON,     1 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);
            CreateWindow (CTRL_BUTTON, "TOGGLE",   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_TOGGLE_BUTTON,   2 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);
            CreateWindow (CTRL_BUTTON, "SETTING",  WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_SETTING_BUTTON,  3 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);
            CreateWindow (CTRL_BUTTON, "SHUTDOWN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_SHUTDOWN_BUTTON, 4 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);

            CreateWindow (CTRL_BUTTON, "ABOUT",    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, ID_ABOUT_BUTTON,    5 * code + _MARGIN, _MARGIN, code - 2 * _MARGIN, HEIGHT_DOCKBAR - 2 * _MARGIN, hWnd, 0);

            SetTimer(hWnd, ID_SHOW_TIMER, DOCKBAR_VISIBLE_TIME);
            m_direction = DIRECTION_HIDE;
            m_DockBar_X = m_DockBar_Start_x;
            break;

        case MSG_COMMAND:
            code = HIWORD (wParam);
            id   = LOWORD (wParam);
            switch(id) 
            {
                case ID_DISPLAY_BUTTON:
                    if(m_direction == DIRECTION_HIDE)
                        m_direction = DIRECTION_SHOW;
                    else
                        m_direction = DIRECTION_HIDE;
                    create_animation(hWnd);
                    break;
                case ID_HOME_BUTTON:
                    break;
                case ID_TOGGLE_BUTTON:
                    break;
                case ID_SETTING_BUTTON:
                    break;
                case ID_SHUTDOWN_BUTTON:
                    break;
                case ID_ABOUT_BUTTON:
                    break;
            }
            break;

        case MSG_TIMER:
            if(wParam == ID_SHOW_TIMER)
            {
                m_direction = DIRECTION_HIDE;
                create_animation(hWnd);
                KillTimer(hWnd, ID_SHOW_TIMER);
            }
            break;
        
        case MSG_CLOSE:
            KillTimer (hWnd, ID_SHOW_TIMER);
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

HWND create_dock_bar (void)
{
    MAINWINCREATE CreateInfo;
    HWND hDockBar;

    m_DockBar_Height = GetGDCapability(HDC_SCREEN, GDCAP_DPI);
    m_DockBar_Height = HEIGHT_DOCKBAR * m_DockBar_Height / 96;

    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_WINTYPE_DOCKER;
    CreateInfo.spCaption = "DockBar" ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DockBarWinProc;
    CreateInfo.lx = g_rcScr.right / 3;
    CreateInfo.ty = g_rcScr.bottom * 2 / 3;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = CreateInfo.ty + m_DockBar_Height;

    m_DockBar_Start_x = CreateInfo.lx;
    m_DockBar_Start_y = CreateInfo.ty;
    m_DockBar_End_x = CreateInfo.rx;
    m_DockBar_End_y = CreateInfo.by;
    m_DockBar_Left_Length = (m_DockBar_End_x - m_DockBar_Start_x) / BUTTON_COUNT;

    CreateInfo.iBkColor = RGBA2Pixel(HDC_SCREEN, 0xFF, 0xFF, 0xFF, 0x80); 
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    hDockBar = CreateMainWindowEx2 (&CreateInfo, 0L, NULL, NULL, ST_PIXEL_ARGB8888,
                                MakeRGBA (SysPixelColor[IDX_COLOR_darkgray].r,
                                          SysPixelColor[IDX_COLOR_darkgray].g,
                                          SysPixelColor[IDX_COLOR_darkgray].b,
                                          0xA0),
                                CT_ALPHAPIXEL, 0x80);

    return hDockBar;
}

