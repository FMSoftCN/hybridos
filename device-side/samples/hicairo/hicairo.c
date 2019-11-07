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
** hicairo.c:
**  Sample for hiCairo.
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
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
#include <ctype.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <cairo/cairo.h>
#include <cairo/cairo-minigui.h>

#include "hicairo.h"

static cairo_surface_t *create_cairo_surface (HDC hdc, int width, int height)
{
    if (hdc == HDC_INVALID) {
        return cairo_minigui_surface_create_with_memdc (CAIRO_FORMAT_RGB24, width, height);
    }
    else if (width > 0 && height > 0) {
        return cairo_minigui_surface_create_with_memdc2 (hdc, width, height);
    }

    return cairo_minigui_surface_create (hdc);
}

static void paint (HWND hwnd, HDC hdc, int width, int height)
{
    HDC csdc;

    cairo_t* cr = (cairo_t*)GetWindowAdditionalData(hwnd);
    if (cr == NULL) {
        _ERR_PRINTF("hicairo: failed to get the cairo context\n");
        exit (1);
    }

    draw_tiger(cr, width, height);

    csdc = cairo_minigui_surface_get_dc (cairo_get_target(cr));
    if (csdc == HDC_INVALID) {
        _ERR_PRINTF("hicairo: failed to get the DC associated with the target surface\n");
        exit (1);
    }

    BitBlt(csdc, 0, 0, width, height, hdc, 0, 0, 0);
}

static LRESULT SampleWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        break;

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        paint (hWnd, hdc, WIDTH, HEIGHT);
        TextOut (hdc, 10, 10, "Drag the mouse up and down." );
        EndPaint(hWnd, hdc);
        return 0;
    }

    case MSG_LBUTTONDOWN: {
        SetCapture(hWnd);
        break;
    }

    case MSG_MOUSEMOVE: {
        int x, y;
        x = LOSWORD(lParam);
        y = HISWORD(lParam);
        if (GetCapture() == hWnd)
            ScreenToClient (hWnd, &x, &y);
        break;
    }

    case MSG_LBUTTONUP: {
        ReleaseCapture();
        break;
    }

    case MSG_CLOSE:
        DestroyMainWindow(hWnd);
        PostQuitMessage(hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

    int retval = 0;
    cairo_surface_t* surface = NULL;
    cairo_t* cr = NULL;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER, "hicairo", 0, 0);
#endif

    surface = create_cairo_surface (HDC_SCREEN, WIDTH, HEIGHT);
    if (surface == NULL) {
        _ERR_PRINTF("hicairo: failed when creating surface\n");
        retval = 1;
        goto FAIL;
    }

    cr = cairo_create (surface);
    if (cr == NULL) {
        _ERR_PRINTF("hicairo: failed when creating cairo context\n");
        retval = 2;
        goto FAIL;
    }

    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Sample for hiCairo";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = SampleWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = WIDTH;
    CreateInfo.by = HEIGHT;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = (DWORD)cr;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID) {
        retval = 3;
    }

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
    retval = 0;

FAIL:
    if (cr) {
        cairo_destroy(cr);
    }

    if (surface) {
        cairo_surface_finish(surface);
        cairo_surface_destroy(surface);
    }

    return retval;
}

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

