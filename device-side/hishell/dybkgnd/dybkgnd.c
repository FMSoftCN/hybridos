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
#include <mgeff/mgeff.h>

#define WALLPAPER_FILE_TOP          "res/wallpaper-top.jpg"
#define WALLPAPER_FILE_BOTTOM       "res/wallpaper-bottom.jpg"

BITMAP bmpTop;
BITMAP bmpBottom;

int start = 100;
int end = 0;
int duration = 600;
enum EffMotionType motionType = OutQuart;
//enum EffMotionType motionType = OutQuint;

void loadBitmap(void)
{
    bmpTop.bmWidth = 0;
    bmpBottom.bmWidth = 0;
    LoadBitmapFromFile(HDC_SCREEN, &bmpTop, WALLPAPER_FILE_TOP);
    LoadBitmapFromFile(HDC_SCREEN, &bmpBottom, WALLPAPER_FILE_BOTTOM);
}

void clearBitmap(void)
{
    if (bmpTop.bmWidth > 0)
    {
        UnloadBitmap(&bmpTop);
    }

    if (bmpBottom.bmWidth > 0)
    {
        UnloadBitmap(&bmpBottom);
    }
}

int paintWallpaper (HDC hdc, int space)
{
    static int lastSpace = -1;
    int ret = 0;

    if (lastSpace == space)
    {
        return ret;
    }
    lastSpace = space;

    if (bmpTop.bmWidth > 0 && bmpTop.bmHeight > 0 && bmpBottom.bmWidth > 0 && bmpBottom.bmHeight > 0) {
        int x, y, w, h, y2;
        static int wp_w = 0, wp_h = 0, wp_half_h = 0;

        if (wp_w == 0) {
            wp_w = (int)GetGDCapability (hdc, GDCAP_HPIXEL);
            wp_h = (int)GetGDCapability (hdc, GDCAP_VPIXEL);
            wp_half_h = wp_h >> 1;
            start = wp_half_h - bmpTop.bmHeight;;
        }

        fprintf(stderr, "wp_w=%d|wp_h=%d\n", wp_w, wp_h);
        SetBrushColor(hdc, RGBA2Pixel(hdc, 0x00, 0x00, 0x0, 0x0));
        FillBox(hdc, 0, 0, wp_w, wp_h);

        w = bmpTop.bmWidth;
        h = bmpTop.bmHeight;

        x = (wp_w - w) >> 1;
        y = wp_half_h - h;
        y2 = wp_half_h;

        FillBoxWithBitmap(hdc, x, y - space, bmpTop.bmWidth, bmpTop.bmHeight, &bmpTop);
        FillBoxWithBitmap(hdc, x, y2 + space, bmpBottom.bmWidth, bmpBottom.bmHeight, &bmpBottom);

        SyncUpdateDC (hdc);
    }
    else {
        _ERR_PRINTF("Failed to get the size of wallpaper bitmap\n");
        ret = -2;
        goto ret;
    }

ret:
    return ret;
}

static void animated_cb(MGEFF_ANIMATION handle, HWND hwnd, int id, int *value)
{
    paintWallpaper(HDC_SCREEN, *value);
}

static void animated_end(MGEFF_ANIMATION handle)
{
}

void startAnimation (HWND hwnd)
{
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate((void *)hwnd, (void *)animated_cb, 1, MGEFF_INT);
    if (animation) {
        mGEffAnimationSetStartValue(animation, &start);
        mGEffAnimationSetEndValue(animation, &end);
        mGEffAnimationSetDuration(animation, duration);
        mGEffAnimationSetCurve(animation, motionType);
        mGEffAnimationSetFinishedCb(animation, animated_end);
        mGEffAnimationSyncRun(animation);
        mGEffAnimationDelete(animation);
    }
}

void doAnimationBack(HWND hwnd)
{
    MGEFF_ANIMATION animation;
    animation = mGEffAnimationCreate((void *)hwnd, (void *)animated_cb, 1, MGEFF_INT);
    if (animation) {
        mGEffAnimationSetStartValue(animation, &end);
        mGEffAnimationSetEndValue(animation, &start);
        mGEffAnimationSetDuration(animation, motionType);
        mGEffAnimationSetCurve(animation, motionType);
        mGEffAnimationSyncRun(animation);
        mGEffAnimationDelete(animation);
    }
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG msg;
    JoinLayer(NAME_DEF_LAYER , "wallpaper" , 0 , 0);

    loadBitmap();
    if (paintWallpaper(HDC_SCREEN, 0) < 0)
    {
        clearBitmap();
        exit (1);
    }

    mGEffInit();
    startAnimation(NULL);

    while (GetMessage (&msg, HWND_DESKTOP)) {
        DispatchMessage (&msg);
    }

    clearBitmap();
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

