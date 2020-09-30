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

#include "../include/sysconfig.h"

RECT m_ScreenRect;

extern WebKitBrowserWindow& toWebKitBrowserWindow(const void *clientInfo);
extern std::string createString(WKStringRef wkString);
extern std::string createString(WKURLRef wkURL);

//static const char *gUrl = "https://hybridos.fmsoft.cn/";
//static const char *gUrl = "http://www.sina.com.cn/";
static const char *gUrl = "file://localhost/home/projects/hiwebkit/Websites/fmsoft.webkit.org/e.html";
struct Window_Info window_info[MAX_TARGET_NUMBER];
int m_target_blank_index = -1;

static LRESULT MainFrameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static void create_control(HWND hwnd, int index)
{
    BrowserWindow * browserWindow = NULL;
    int i = 0;
    RECT rect = m_ScreenRect;
    
    browserWindow = WebKitBrowserWindow::create(IDC_BROWSER, rect, hwnd, HWND_INVALID);
    if (browserWindow)
    {
        window_info[index].hWnd = hwnd;
        window_info[index].view = browserWindow;
        ShowWindow(browserWindow->hwnd(), SW_SHOW);

        if(window_info[index].target_url)
        {
            browserWindow->loadURL(window_info[index].target_url);
        }
        else
        {
            browserWindow->loadURL(gUrl);
        }
    }
}

static HWND CreateAPPAgentMaindow(int index)
{
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    
    m_ScreenRect = GetScreenRect();

    CreateInfo.dwStyle = WS_VISIBLE;
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
    CreateInfo.dwAddData = (DWORD)index;
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
    int index = 0;
    BrowserWindow * view = NULL;
    int i = 0;

    switch (message) {
        case MSG_CREATE:
            index = (int) GetWindowAdditionalData(hWnd);
            create_control(hWnd, index);
            break;

        case MSG_USER_NEW_VIEW:
            i = (int)wParam;
            if(window_info[i].hWnd)
            {
                SetActiveWindow(window_info[i].hWnd);
                window_info[i].view->loadURL(window_info[i].target_url);
            }
            else
                CreateAPPAgentMaindow(i);
            break;

        case MSG_CLOSE:
            for(i == 0; i < MAX_TARGET_NUMBER; i++)
            {
                if(window_info[i].hWnd == hWnd)
                    break;
            }
            if(i < MAX_TARGET_NUMBER)
            {
                window_info[i].hWnd = NULL;
                window_info[i].view = NULL;
                if(i > 2)
                    window_info[i].target_url[0] = 0;
            }

            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void performMessageLoopTasks()
{
    g_main_context_iteration(0, false);
}

static void filter_browser_message(MSG * msg)
{
    int id = -1;

    if(msg->message == MSG_BROWSER_SHOW)
    {
        id = msg->wParam;


        if(id == -1)
            id = m_target_blank_index;

        if(window_info[id].hWnd)
        {
            REQUEST request;
            RequestInfo requestinfo;
            ReplyInfo replyInfo;

            SetActiveWindow(window_info[id].hWnd);
            SetFocus(window_info[id].hWnd);
            window_info[id].view->loadURL(window_info[id].target_url);

            requestinfo.id = REQ_SUBMIT_TOPMOST;
            requestinfo.hWnd = window_info[id].hWnd;
            requestinfo.iData0 = 0;
            request.id = FIXED_FORMAT_REQID;
            request.data = (void *)&requestinfo;
            request.len_data = sizeof(requestinfo);

            memset(&replyInfo, 0, sizeof(ReplyInfo));
            ClientRequest(&request, &replyInfo, sizeof(ReplyInfo));
            if((replyInfo.id == REQ_SUBMIT_TOGGLE) && (replyInfo.iData0))
            {
            }
            else
            {
            }
        }
        else
            CreateAPPAgentMaindow(id);
    }
}

static void initial_window_info()
{
    char config_path[MAX_PATH + 1];
    char* etc_value = NULL;
    char unknown_target[MAX_TARGET_NAME_LEN];
    int i = 0;

    if ((etc_value = getenv ("HISHELL_CFG_PATH")))
    {
        int len = strlen(etc_value);
        if (etc_value[len-1] == '/')
        {
            sprintf(config_path, "%s%s", etc_value, SYSTEM_CONFIG_FILE);
        }
        else
        {
            sprintf(config_path, "%s/%s", etc_value, SYSTEM_CONFIG_FILE);
        }
    }
    else
    {
        sprintf(config_path, "%s", SYSTEM_CONFIG_FILE);
    }

    memset(&window_info, 0, sizeof(struct Window_Info) * MAX_TARGET_NUMBER);
    memset(unknown_target, 0, MAX_TARGET_NAME_LEN);

    GetValueFromEtcFile(config_path, "system", "target0", window_info[0].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url0", window_info[0].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target1", window_info[1].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url1", window_info[1].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target2", window_info[2].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url2", window_info[2].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target3", window_info[3].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url3", window_info[3].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target4", window_info[4].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url4", window_info[4].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target5", window_info[5].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url5", window_info[5].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target6", window_info[6].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url6", window_info[6].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target7", window_info[7].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url7", window_info[7].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "target8", window_info[8].target_name, MAX_TARGET_NAME_LEN);
    GetValueFromEtcFile(config_path, "system", "url8", window_info[8].target_url, MAX_TARGET_URL_LEN);

    GetValueFromEtcFile(config_path, "system", "unknown_target", unknown_target, MAX_TARGET_NAME_LEN);

    for(i = 0; i < MAX_TARGET_NUMBER; i++)
        if(window_info[i].target_name[0] && (strcmp(window_info[i].target_name, unknown_target) == 0))
            break;

    if(i < MAX_TARGET_NUMBER)
        m_target_blank_index = i;
    else
        m_target_blank_index = -1;

}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;

#if 0
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
#endif
    
    m_ScreenRect = GetScreenRect();

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "HybridOS" , 0 , 0);
#endif

    initial_window_info();

#if 0
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
#endif
    while (GetMessage(&Msg, HWND_DESKTOP)) {
        performMessageLoopTasks();
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        filter_browser_message(&Msg);
        performMessageLoopTasks();
    }

#if 0
    MainWindowThreadCleanup (hMainWnd);
#endif

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
