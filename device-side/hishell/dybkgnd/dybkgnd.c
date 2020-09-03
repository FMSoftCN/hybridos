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
** wallpaper-dynamic.c: a daynamic wallpaper renderer.
**
** Copyright (C) 2019 ~ 2020 FMSoft (http://www.fmsoft.cn).
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
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

static int load_wallpaper (HDC hdc, const char* top, const char* bottom,  double scale)
{
    BITMAP bmp;
    BITMAP bmpBottom;
    int ret = 0;

    int loadRet = LoadBitmapFromFile(HDC_SCREEN, &bmp, top);
    int loadRetBottom = LoadBitmapFromFile(HDC_SCREEN, &bmpBottom, bottom);

    if (bmp.bmWidth > 0 && bmp.bmHeight > 0 && bmpBottom.bmWidth > 0 && bmpBottom.bmHeight > 0) {
        int x, y, w, h, y2, space;
        static int wp_w = 0, wp_h = 0, wp_half_h = 0;

        if (wp_w == 0) {
            wp_w = (int)GetGDCapability (hdc, GDCAP_HPIXEL);
            wp_h = (int)GetGDCapability (hdc, GDCAP_VPIXEL);
            wp_half_h = wp_h >> 1;
        }

        SetBrushColor(hdc, 0xFFFFFF);
        FillBox(hdc, 0, 0, wp_w, wp_h);

        w = bmp.bmWidth;
        h = bmp.bmHeight;

        x = (wp_w - w) >> 1;
        y = wp_half_h - h;
        y2 = wp_half_h;

        space = (wp_half_h >> 2) * (1 - scale);

        FillBoxWithBitmap(hdc, x, y - space, bmp.bmWidth, bmp.bmHeight, &bmp);
        FillBoxWithBitmap(hdc, x, y2 + space, bmpBottom.bmWidth, bmpBottom.bmHeight, &bmpBottom);

        SyncUpdateDC (hdc);
    }
    else {
        _ERR_PRINTF("Failed to get the size of wallpaper bitmap\n");
        ret = -2;
        goto ret;
    }

ret:
    if (loadRet == 0)
        UnloadBitmap(&bmp);

    if (loadRetBottom == 0)
        UnloadBitmap(&bmpBottom);

    return ret;
}

#define WALLPAPER_FILE_TOP          "res/wallpaper-top.jpg"
#define WALLPAPER_FILE_BOTTOM       "res/wallpaper-bottom.jpg"

int MiniGUIMain (int argc, const char* argv[])
{
    double scale = 1.0;
    double step = 0.1;
    MSG msg;
    RECT rc_scr = GetScreenRect();
    DWORD old_tick_count;

    _MG_PRINTF("Screen rect: %d, %d, %d, %d\n",
            rc_scr.left, rc_scr.top,
            rc_scr.right, rc_scr.bottom);

    _MG_PRINTF("Wallpaper pattern size: %d, %d\n",
            GetGDCapability (HDC_SCREEN, GDCAP_HPIXEL),
            GetGDCapability (HDC_SCREEN, GDCAP_VPIXEL));

    JoinLayer(NAME_DEF_LAYER , "wallpaper" , 0 , 0);

    if (load_wallpaper (HDC_SCREEN, WALLPAPER_FILE_TOP, WALLPAPER_FILE_BOTTOM, scale) < 0)
        exit (1);

    old_tick_count = GetTickCount ();

    /* this is a trick in order that GetMessage can return fast */
    SetTimer (HWND_DESKTOP, (LINT)&old_tick_count, 2);

    while (GetMessage (&msg, HWND_DESKTOP)) {
        DWORD curr_tick_count = GetTickCount ();
        if (curr_tick_count > old_tick_count + 20) {
            scale += step;
            if (scale >= 1.0) {
                step = -0.1;
            }
            if (scale < 0.1) {
                step = 0.1;
            }

            _DBG_PRINTF("It is time to load another wallpaper.\n");
            if (load_wallpaper (HDC_SCREEN, WALLPAPER_FILE_TOP, WALLPAPER_FILE_BOTTOM, scale) < 0)
                exit (1);
            old_tick_count = curr_tick_count;
        }

        DispatchMessage (&msg);
    }

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

