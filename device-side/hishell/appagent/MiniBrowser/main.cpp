/* 
 * Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Or,
 * 
 * As this component is a program released under LGPLv3, which claims
 * explicitly that the program could be modified by any end user
 * even if the program is conveyed in non-source form on the system it runs.
 * Generally, if you distribute this program in embedded devices,
 * you might not satisfy this condition. Under this situation or you can
 * not accept any condition of LGPLv3, you need to get a commercial license
 * from FMSoft, along with a patent license for the patents owned by FMSoft.
 * 
 * If you have got a commercial/patent license of this program, please use it
 * under the terms and conditions of the commercial license.
 * 
 * For more information about the commercial license and patent license,
 * please refer to
 * <https://hybridos.fmsoft.cn/blog/hybridos-licensing-policy/>.
 * 
 * Also note that the LGPLv3 license does not apply to any entity in the
 * Exception List published by Beijing FMSoft Technologies Co., Ltd.
 * 
 * If you are or the entity you represent is listed in the Exception List,
 * the above open source or free software license does not apply to you
 * or the entity you represent. Regardless of the purpose, you should not
 * use the software in any way whatsoever, including but not limited to
 * downloading, viewing, copying, distributing, compiling, and running.
 * If you have already downloaded it, you MUST destroy all of its copies.
 * 
 * The Exception List is published by FMSoft and may be updated
 * from time to time. For more information, please see
 * <https://www.fmsoft.cn/exception-list>.
 */ 

#define PLATFORM(HBD)   1

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <WebKit/WKGeometry.h>
#include <WebKit/WKView.h>
#include <WebKit/WKURL.h>
#include <WebKit/WKPage.h>

#include <glib.h>
#include "WebKitBrowserWindow.h"

#ifdef _LANG_ZHCN
#include "browser_res_cn.h"
#elif defined _LANG_ZHTW
#include "browser_res_tw.h"
#else
#include "browser_res_en.h"
#endif

RECT m_ScreenRect;

extern WebKitBrowserWindow& toWebKitBrowserWindow(const void *clientInfo);
extern std::string createString(WKStringRef wkString);
extern std::string createString(WKURLRef wkURL);

//static const char *gUrl = "https://hybridos.fmsoft.cn/";
static const char *gUrl = "http://www.sina.com.cn/";
static LRESULT MainFrameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static void create_control(HWND hwnd, const char * url)
{
    BrowserWindow * browserWindow = NULL;
    int i = 0;
    RECT rect = m_ScreenRect;
    
    browserWindow = WebKitBrowserWindow::create(IDC_BROWSER, rect, hwnd, HWND_INVALID);
    if (browserWindow)
    {
        ShowWindow(browserWindow->hwnd(), SW_SHOW);

        if(url)
        {
            browserWindow->loadURL(url);
        }
        else
        {
            browserWindow->loadURL(gUrl);
        }
    }
}

static HWND CreateAPPAgentMaindow(char * url)
{
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    
    m_ScreenRect = GetScreenRect();

    CreateInfo.dwStyle = 
        WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = HL_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MainFrameProc;
    CreateInfo.lx = m_ScreenRect.left;
    CreateInfo.ty = m_ScreenRect.top;
    CreateInfo.rx = m_ScreenRect.right;
    CreateInfo.by = m_ScreenRect.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = (DWORD) url;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return HWND_INVALID;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    return hMainWnd;
}

static LRESULT MainFrameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    char * url = NULL;
    BrowserWindow * view = NULL;
    int i = 0;

    switch (message) {
        case MSG_CREATE:
            url = (char *) GetWindowAdditionalData(hWnd);
            create_control(hWnd, url);
            break;

        case MSG_USER_NEW_VIEW:
            url = (char *)lParam;
            CreateAPPAgentMaindow(url);
            break;

        case MSG_CLOSE:
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
//            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void performMessageLoopTasks()
{
    g_main_context_iteration(0, false);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    
    m_ScreenRect = GetScreenRect();

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "HybridOS" , 0 , 0);
#endif

    CreateInfo.dwStyle = 
        WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = HL_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = MainFrameProc;
    CreateInfo.lx = m_ScreenRect.left;
    CreateInfo.ty = m_ScreenRect.top;
    CreateInfo.rx = m_ScreenRect.right;
    CreateInfo.by = m_ScreenRect.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, HWND_DESKTOP)) {
        performMessageLoopTasks();
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        performMessageLoopTasks();
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
