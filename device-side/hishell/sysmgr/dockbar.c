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

#include "config.h"

static char* mk_time (char* buff)
{
    time_t t;
    struct tm * tm;

    time (&t);
    tm = localtime (&t);
    sprintf (buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

    return buff;
}

APPINFO app_info;

static void free_app_info (void)
{
    int i;
    APPITEM* item;

    item = app_info.app_items;
    for (i = 0; i < app_info.nr_apps; i++, item++) {
        if (item->bmp.bmBits) {
            UnloadBitmap (&item->bmp);
            item->bmp.bmBits = NULL;
        }
    }

    free (app_info.app_items);
    app_info.app_items = NULL;
}

static void strsubchr (char* string, int c1, int c2)
{
    char* tmp;

    while (string && (tmp = strchr (string, c1))) {
        *tmp = c2;
        string = tmp;
    }
}

static BOOL get_app_info (void)
{
    int i;
    APPITEM* item;
    char section [32];

    if (GetIntValueFromEtcFile (APP_INFO_FILE, "taskbar", "nr", &app_info.nr_apps) != ETC_OK)
        return FALSE;

    if (app_info.nr_apps <= 0)
        return FALSE;

    GetIntValueFromEtcFile (APP_INFO_FILE, "taskbar", "autostart", &app_info.autostart);
    
    if (GetValueFromEtcFile (APP_INFO_FILE, "taskbar", "logo", app_info.logo_path, PATH_MAX + NAME_MAX) != ETC_OK)
        return FALSE;
           

    if (app_info.autostart >= app_info.nr_apps || app_info.autostart < 0)
        app_info.autostart = 0;

    if ((app_info.app_items = (APPITEM*)calloc (app_info.nr_apps, sizeof (APPITEM))) == NULL) {
        return FALSE;
    }

    item = app_info.app_items;
    for (i = 0; i < app_info.nr_apps; i++, item++) {

        sprintf (section, "app%d", i);
        if (GetValueFromEtcFile (APP_INFO_FILE, section, "path", item->path, PATH_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "name", item->name, NAME_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "layer", item->layer, LEN_LAYER_NAME) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "tip", item->tip, TIP_MAX) != ETC_OK)
            goto error;

        strsubchr (item->tip, '&', ' ');

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "icon", item->bmp_path, PATH_MAX + NAME_MAX) != ETC_OK)
            goto error;

        if (LoadBitmap (HDC_SCREEN, &item->bmp, item->bmp_path) != ERR_BMP_OK)
            goto error;

        item->cdpath = TRUE;
    }
    return TRUE;

error:
    free_app_info ();
    return FALSE;
}

static HWND create_app_coolbar (HWND hWnd)
{
    int i;
    HWND hCoolBar;
    COOLBARITEMINFO bar_item;
    APPITEM* app_item;

    hCoolBar = CreateWindow (CTRL_COOLBAR, "",
                        WS_CHILD | WS_VISIBLE, _ID_APPS_COOLBAR,
                        _WIDTH_START + _MARGIN * 2, _MARGIN,
                        _WIDTH_APPS, _HEIGHT_CTRL,
                        hWnd, 0);

    bar_item.insPos = 0;
    bar_item.id = 0;
    bar_item.ItemType = TYPE_TEXTITEM;
    bar_item.Bmp = NULL;
    bar_item.ItemHint = "Quit MDE";
    bar_item.Caption = "Quit";
    bar_item.dwAddData= 0;

    SendMessage (hCoolBar, CBM_ADDITEM, 0, (LPARAM)&bar_item);

    app_item = app_info.app_items;
    for (i = 0; i < app_info.nr_apps; i++, app_item++) {
        bar_item.insPos = i + 1;
        bar_item.id = i + 1;
        bar_item.ItemType = TYPE_BMPITEM;
        bar_item.Bmp = &app_item->bmp;
        bar_item.ItemHint = app_item->tip;
        bar_item.Caption = NULL;
        bar_item.dwAddData= 0;

        SendMessage (hCoolBar, CBM_ADDITEM, 0, (LPARAM)&bar_item);
    }

    return hCoolBar;
}

static void under_construction (HWND hwnd)
{
    MessageBox (hwnd, 
            "This function is under construction.",
            "MDE!",
            MB_OK | MB_ICONEXCLAMATION);
}

static pid_t exec_app (int app)
{
    pid_t pid = 0;
    char buff [PATH_MAX + NAME_MAX + 1];

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        if (app_info.app_items [app].cdpath) {
            if (chdir (app_info.app_items [app].path)) {
                fprintf (stderr, "error on chdir.\n");
                return 0;
            }
        }
        strcpy (buff, app_info.app_items [app].path);
        strcat (buff, app_info.app_items [app].name);

        if (app_info.app_items [app].layer [0]) {
            execl (buff, app_info.app_items [app].name, 
                        "-layer", app_info.app_items [app].layer, NULL);
        }
        else {
            execl (buff, app_info.app_items [app].name, NULL);
        }

        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }

    return pid;
}

//static BITMAP * plogo = NULL;
static LRESULT DockBarWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char buff [20];

    switch (message) {
    case MSG_CREATE:
    {
 //       if (!get_app_info ())
//            return 1;
//        plogo = calloc (1 , sizeof(BITMAP));
//        LoadBitmap (HDC_SCREEN , plogo , app_info.logo_path);
//        CreateWindow (CTRL_STATIC, "", SS_REALSIZEIMAGE | SS_CENTERIMAGE | SS_BITMAP | WS_CHILD | WS_VISIBLE | WS_BORDER, _ID_START_BUTTON, 
//                    _MARGIN, _MARGIN, g_rcScr.right / 2, /*_WIDTH_START,*/ _HEIGHT_CTRL, hWnd, (DWORD)plogo);
        CreateWindow (CTRL_STATIC, "dock bar", WS_CHILD | WS_BORDER | WS_VISIBLE | SS_CENTER, _ID_TITLE_STATIC,                           
                    _MARGIN, _MARGIN, g_rcScr.right / 2, /*_WIDTH_START,*/ _HEIGHT_CTRL, hWnd, 0); 

        CreateWindow (CTRL_STATIC, mk_time (buff), WS_CHILD | WS_BORDER | WS_VISIBLE | SS_CENTER, 
                    _ID_TIME_STATIC, g_rcScr.right / 2, _MARGIN, g_rcScr.right / 2 - _MARGIN, _HEIGHT_CTRL, hWnd, 0);

//        create_app_coolbar (hWnd);

#ifdef _MGTIMER_UNIT_10MS
        SetTimer (hWnd, _ID_TIMER, 100);
#else
        SetTimer (hWnd, _ID_TIMER, 10);
#endif
        break;
    }

    case MSG_COMMAND:
    {
        int code = HIWORD (wParam);
        int id   = LOWORD (wParam);
        switch (id) {
        case _ID_APPS_COOLBAR:
            exec_app (code - 1);
            break;
        }

        if (id == _ID_LAYER_BOX && code == 0) {
            MG_Layer* layer = (MG_Layer*) GetWindowAdditionalData ((HWND)lParam);

            if (layer) {
                ServerSetTopmostLayer (layer);
            }
        }
    }
    break;

    case MSG_TIMER:
    {
        SetDlgItemText (hWnd, _ID_TIME_STATIC, mk_time (buff));
        break;
    }
        
    case MSG_CLOSE:
        KillTimer (hWnd, _ID_TIMER);
//        free (plogo);
        DestroyAllControls (hWnd);
//        free_app_info ();
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

    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_WINTYPE_DOCKER;
    CreateInfo.spCaption = "DockBar" ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DockBarWinProc;
    CreateInfo.lx = g_rcScr.left;
    CreateInfo.ty = g_rcScr.bottom - HEIGHT_TASKBAR;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;

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

