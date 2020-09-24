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
 * \file statusbar.c
 * \author Gengyue <gengyue@minigui.org>
 * \date 2020/09/16
 *
 * \brief This file implements status bar in system manager process.
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
 * $Id: statusbar.c 13674 2020-09-16 06:45:01Z Gengyue $
 *
 *      HybridOS for Linux, VxWorks, NuCleus, OSE.
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

#include "../include/sysconfig.h"
#include "config.h"

extern HWND m_hStatusBar;                       // handle of status bar
extern HWND m_hDockBar;                         // handle of dock bar
static int m_StatusBar_Height = 0;              // height of status bar
static int m_StatusBar_Y = 0;                   // the Y coordinate of top left corner
static MGEFF_ANIMATION m_animation = NULL;      // handle of animation
static int m_direction = DIRECTION_SHOW;        // the direction of animation

// get current time
static char* mk_time(char* buff)
{
    time_t t;
    struct tm * tm;

    time (&t);
    tm = localtime (&t);
    sprintf(buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

    return buff;
}

// callback function of animation
static void animated_cb(MGEFF_ANIMATION handle, HWND hWnd, int id, int *value)
{
    if(m_StatusBar_Y != *value)
    {
        m_StatusBar_Y = *value;
        MoveWindow(hWnd, g_rcScr.left, m_StatusBar_Y, g_rcScr.right, m_StatusBar_Height, TRUE);
    }
}

// the function which will be invoked at the end of animation
static void animated_end(MGEFF_ANIMATION handle)
{
    HWND hWnd = (HWND)mGEffAnimationGetTarget(handle);
    mGEffAnimationDelete(m_animation);
    m_animation = NULL;

    if((m_direction == DIRECTION_SHOW) && hWnd)
        SetTimer(hWnd, ID_SHOW_TIMER, STATUSBAR_VISIBLE_TIME);
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
        enum EffMotionType motionType = InCirc;

        start = m_StatusBar_Y;
        if(m_direction == DIRECTION_HIDE)
        {
            end = -1 * m_StatusBar_Height;
            motionType = OutCirc;
            duration = STATUSBAR_ANIMATION_TIME * (m_StatusBar_Height + m_StatusBar_Y) / m_StatusBar_Height;
        }
        else
        {
            end = 0;
            motionType = OutCirc;
            duration = -1 * STATUSBAR_ANIMATION_TIME * m_StatusBar_Y / m_StatusBar_Height;
        }

        if(duration == 0)
            duration = STATUSBAR_ANIMATION_TIME;

        mGEffAnimationSetStartValue(m_animation, &start);
        mGEffAnimationSetEndValue(m_animation, &end);
        mGEffAnimationSetDuration(m_animation, duration);
        mGEffAnimationSetCurve(m_animation, motionType);
        mGEffAnimationSetFinishedCb(m_animation, animated_end);
        mGEffAnimationAsyncRun(m_animation);
    }
}

// the window proc of status bar
static LRESULT StatusBarWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static PLOGFONT font;
    char buff [20];
    int length = 0;
    RECT rect[2] = {{10 * MARGIN_STATUS, MARGIN_STATUS, g_rcScr.right - TIME_INFO_X,  m_StatusBar_Height - MARGIN_STATUS}, {g_rcScr.right - TIME_INFO_X, MARGIN_STATUS, g_rcScr.right - MARGIN_STATUS, m_StatusBar_Height - MARGIN_STATUS}};

    switch (message) 
    {
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            SetTextColor (hdc, DWORD2Pixel (hdc, 0xFFFFFFFF));
            SetBkMode (hdc, BM_TRANSPARENT);
            SelectFont(hdc, font);
            length = GetWindowTextLength(hWnd);
            {
                char title[length + 1];
                memset(title, 0, length + 1);
                GetWindowText(hWnd, title, length);
                DrawText (hdc, title, strlen(title), rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            }

            mk_time(buff);
            DrawText (hdc, buff, strlen(buff), rect + 1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint (hWnd, hdc);
            return 0;

        case MSG_MAINWINDOW_CHANGE:
            if((wParam & 0xFFFF) == 0)          // It is Desk Top
            {
                m_direction = DIRECTION_SHOW;
                create_animation(hWnd);

                SetWindowText(hWnd, STRING_OS_NAME);
                InvalidateRect(hWnd, rect + 0, TRUE);
            }
            else
            {
                length = (int)lParam;
                if(length == 0)            // no title for main window
                {
                    m_direction = DIRECTION_SHOW;
                    create_animation(hWnd);

                    SetWindowText(hWnd, STRING_OS_NAME);
                    InvalidateRect(hWnd, rect + 0, TRUE);
                }
                else                            // get the title
                {
                    REQUEST request;
                    RequestInfo requestinfo;
                    char title[length + 1];

                    memset(title, 0, length + 1);

                    requestinfo.id = REQ_GET_TOPMOST_TITLE;
                    requestinfo.hWnd = m_hStatusBar;
                    requestinfo.iData0 = wParam;
                    requestinfo.iData1 = lParam;

                    request.id = UNFIXED_FORMAT_REQID;
                    request.data = (void *)&requestinfo;
                    request.len_data = sizeof(requestinfo);

                    ClientRequest(&request, title, length + 1);
                    if(title[0])
                    {
                        m_direction = DIRECTION_SHOW;
                        create_animation(hWnd);

                        SetWindowText(hWnd, title);
                        InvalidateRect(hWnd, rect + 0, TRUE);
                    }
                }
            }
            break;

        case MSG_CREATE:
            font = CreateLogFont (FONT_TYPE_NAME_SCALE_TTF, "ttf-Source", "UTF-8",
                        FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN, FONT_FLIP_NIL,
                        FONT_OTHER_AUTOSCALE, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                        m_StatusBar_Height * 0.4, 0);

            SetTimer(hWnd, ID_TIMER, 100);
            SetTimer(hWnd, ID_SHOW_TIMER, STATUSBAR_VISIBLE_TIME);
            m_direction = DIRECTION_HIDE;
            m_StatusBar_Y = 0;
            break;

        case MSG_TIMER:
            if(wParam == ID_TIMER)
                InvalidateRect(hWnd, rect + 1, TRUE);
            else if(wParam == ID_SHOW_TIMER)
            {
                m_direction = DIRECTION_HIDE;
                create_animation(hWnd);
                KillTimer(hWnd, ID_SHOW_TIMER);
            }
            break;

        case MSG_DESTROY:
            DestroyLogFont(font);
        case MSG_CLOSE:
            KillTimer (hWnd, ID_TIMER);
            KillTimer (hWnd, ID_SHOW_TIMER);
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

// create status bar
HWND create_status_bar (void)
{
    MAINWINCREATE CreateInfo;
    HWND hStatusBar;
    REQUEST request;
    const WINDOWINFO *pWindowInfo = NULL;
    RequestInfo requestinfo;
    ReplyInfo replyInfo;

    m_StatusBar_Height = GetGDCapability(HDC_SCREEN, GDCAP_DPI);
    m_StatusBar_Height = HEIGHT_STATUSBAR * m_StatusBar_Height / 96;

    // create a main window
    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_WINTYPE_DOCKER;
    CreateInfo.spCaption = STRING_OS_NAME ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = StatusBarWinProc;
    CreateInfo.lx = g_rcScr.left; 
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = m_StatusBar_Height;
    CreateInfo.iBkColor = RGBA2Pixel(HDC_SCREEN, 0xFF, 0xFF, 0xFF, 0xFf); 
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hStatusBar = CreateMainWindowEx2 (&CreateInfo, 0L, NULL, NULL, ST_PIXEL_ARGB8888,
                                MakeRGBA (SysPixelColor[IDX_COLOR_darkgray].r,
                                          SysPixelColor[IDX_COLOR_darkgray].g,
                                          SysPixelColor[IDX_COLOR_darkgray].b,
                                          0xE0),
                                CT_ALPHAPIXEL, 0xFF);

    if (hStatusBar == HWND_INVALID)
        return HWND_INVALID;

    // send status bar zNode index
    pWindowInfo = GetWindowInfo(hStatusBar);

    // submit zNode index to server
    requestinfo.id = REQ_SUBMIT_STATUSBAR_ZNODE;
    requestinfo.hWnd = hStatusBar;
    requestinfo.iData0 = pWindowInfo->idx_znode;
    request.id = FIXED_FORMAT_REQID;
    request.data = (void *)&requestinfo;
    request.len_data = sizeof(requestinfo);

    memset(&replyInfo, 0, sizeof(ReplyInfo));
    ClientRequest(&request, &replyInfo, sizeof(ReplyInfo));
    if((replyInfo.id == REQ_SUBMIT_STATUSBAR_ZNODE) && (replyInfo.iData0))
    {
    }
    else
        return HWND_INVALID;

    return hStatusBar;
}

