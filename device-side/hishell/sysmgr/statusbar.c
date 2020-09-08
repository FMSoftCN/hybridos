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

#include "../include/sysconfig.h"
#include "statusbar.h"


extern HWND m_hStatusBar;
extern HWND m_hDockBar;

static char* mk_time(char* buff)
{
    time_t t;
    struct tm * tm;

    time (&t);
    tm = localtime (&t);
    sprintf (buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

    return buff;
}

static void Start_Animation(HWND hWnd, int * counter, int direction)
{
    KillTimer (hWnd, _ID_ANIMATE_TIMER);
    KillTimer (hWnd, _ID_SHOW_TIMER);

    if((direction == DIRECTION_SHOW) && (*counter == 10)) 
        *counter = 9;

    SetTimer(hWnd, _ID_ANIMATE_TIMER, 10);
}

static LRESULT StatusBarWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    char str_time_buff [20];
    int length = 0;
    RECT rect[2] = {{_MARGIN, _MARGIN, g_rcScr.right - TIME_INFO_X,  _HEIGHT_CTRL}, {g_rcScr.right - TIME_INFO_X, _MARGIN, g_rcScr.right - _MARGIN, _HEIGHT_CTRL}};
    static int counter = 1;
    static int direction = DIRECTION_SHOW;

    switch (message) 
    {
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            SetTextColor (hdc, DWORD2Pixel (hdc, RGBA_black));
            SetBkMode (hdc, BM_TRANSPARENT);
            length = GetWindowTextLength(hWnd);
            {
                char title[length + 1];
                memset(title, 0, length + 1);
                GetWindowText(hWnd, title, length);
                DrawText (hdc, title, strlen(title), rect, DT_CENTER);
            }

            mk_time(str_time_buff);
            DrawText (hdc, str_time_buff, strlen(str_time_buff), rect + 1, DT_CENTER);

            EndPaint (hWnd, hdc);
            return 0;

        case MSG_MAINWINDOW_CHANGE:
            if((wParam & 0xFFFF) == 0)          // It is Desk Top
            {
                direction = DIRECTION_SHOW;
                Start_Animation(hWnd, &counter, direction);

                SetWindowText(hWnd, STRING_OS_NAME);
                InvalidateRect(hWnd, rect + 0, TRUE);
            }
            else
            {
                length = (int)lParam;
                if(length == 0)            // no title for main window
                {
                    direction = DIRECTION_SHOW;
                    Start_Animation(hWnd, &counter, direction);

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

                    request.id = GET_TITLE_REQID;
                    request.data = (void *)&requestinfo;
                    request.len_data = sizeof(requestinfo);

                    ClientRequest(&request, title, length + 1);
                    if(title[0])
                    {
                        direction = DIRECTION_SHOW;
                        Start_Animation(hWnd, &counter, direction);

                        SetWindowText(hWnd, title);
                        InvalidateRect(hWnd, rect + 0, TRUE);
                    }
                }
            }
            break;

        case MSG_CREATE:
            SetTimer(hWnd, _ID_TIMER, 100);
            SetTimer(hWnd, _ID_SHOW_TIMER, 200);
            break;

        case MSG_TIMER:
            if(wParam == _ID_TIMER)
                InvalidateRect(hWnd, rect + 1, TRUE);
            else if(wParam == _ID_SHOW_TIMER)
            {
                KillTimer(hWnd, _ID_SHOW_TIMER);
                direction = DIRECTION_HIDE;
                SetTimer(hWnd, _ID_ANIMATE_TIMER, 10);
            }
            else if(wParam == _ID_ANIMATE_TIMER)
            {
printf("============== counter is %d\n", counter);
                MoveWindow(hWnd, g_rcScr.left, -1 * counter * HEIGHT_TASKBAR / 10, g_rcScr.right, HEIGHT_TASKBAR, TRUE);
                if(direction == DIRECTION_HIDE)
                {
                    if(counter == 10)
                        KillTimer (hWnd, _ID_ANIMATE_TIMER);
                    else
                        counter ++;
                }
                else
                {
                    if(counter == 0)
                    {
                        KillTimer (hWnd, _ID_ANIMATE_TIMER);
                        SetTimer(hWnd, _ID_SHOW_TIMER, 200);
                    }
                    else
                        counter --;
                }
            }
            break;

        case MSG_CLOSE:
            KillTimer (hWnd, _ID_TIMER);
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

HWND create_status_bar (void)
{
    MAINWINCREATE CreateInfo;
    HWND hStatusBar;
    REQUEST request;
    const WINDOWINFO *pWindowInfo = NULL;
    RequestInfo requestinfo;
    ReplyInfo replyInfo;

    // create a main window
    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_WINTYPE_DOCKER;
    CreateInfo.spCaption = "StatusBar" ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = StatusBarWinProc;
    CreateInfo.lx = g_rcScr.left; 
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = HEIGHT_TASKBAR;
    CreateInfo.iBkColor = RGBA2Pixel(HDC_SCREEN, 0xFF, 0xFF, 0xFF, 0x80); 
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hStatusBar = CreateMainWindowEx2 (&CreateInfo, 0L, NULL, NULL, ST_PIXEL_ARGB8888,
                                MakeRGBA (SysPixelColor[IDX_COLOR_darkgray].r,
                                          SysPixelColor[IDX_COLOR_darkgray].g,
                                          SysPixelColor[IDX_COLOR_darkgray].b,
                                          0xA0),
                                CT_ALPHAPIXEL, 0x80);

    if (hStatusBar == HWND_INVALID)
        return HWND_INVALID;

    // send status bar zNode index
    pWindowInfo = GetWindowInfo(hStatusBar);

    // submit zNode index to server
    requestinfo.id = REQ_SUBMIT_STATUSBAR_ZNODE;
    requestinfo.hWnd = hStatusBar;
    requestinfo.iData0 = pWindowInfo->idx_znode;
    request.id = ZNODE_INFO_REQID;
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

