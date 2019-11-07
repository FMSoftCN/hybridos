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
#include <math.h>

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
    else if (hdc != HDC_SCREEN && width > 0 && height > 0) {
        return cairo_minigui_surface_create_with_memdc2 (hdc, width, height);
    }

    return cairo_minigui_surface_create (hdc);
}

typedef int (*draw_func_t)(cairo_t *, int, int);

static void paint (HWND hwnd, HDC hdc, draw_func_t draw_func,
        double sx, double sy, double angle,
        int width, int height)
{
    HDC csdc;

    cairo_t* cr = (cairo_t*)GetWindowAdditionalData(hwnd);
    if (cr == NULL) {
        _ERR_PRINTF("hicairo: failed to get the cairo context\n");
        exit (1);
    }

    cairo_save(cr);

    cairo_scale(cr, sx, sy);
    cairo_rotate(cr, angle);
    draw_func(cr, width, height);

    csdc = cairo_minigui_surface_get_dc (cairo_get_target(cr));
    if (csdc == HDC_INVALID) {
        _ERR_PRINTF("hicairo: failed to get the DC associated with the target surface\n");
        exit (1);
    }

    if (csdc != HDC_SCREEN)
        BitBlt(csdc, 0, 0, width, height, hdc, 0, 0, 0);
    else {
        Rectangle (HDC_SCREEN, 0, 0, width, height);
    }

    cairo_restore(cr);
}

static int _curr_draw_func_idx = 0;
static draw_func_t _draw_func_list [] = {
    draw_tiger,
    draw_lion,
    draw_world_map_stroke,
    draw_world_map_fill,
    draw_world_map_both,
};

static int _curr_width = WIDTH;
static int _curr_height = HEIGHT;
static double _curr_scale_x = 1.0;
static double _curr_scale_y = 1.0;
static double _curr_rotate_angle = 0.0;

static LRESULT SampleWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        break;

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        paint (hWnd, hdc, _draw_func_list[_curr_draw_func_idx],
            _curr_scale_x, _curr_scale_y, _curr_rotate_angle,
            _curr_width, _curr_height);
        TextOut (hdc, 10, 10, "Drag the mouse up and down." );
        EndPaint(hWnd, hdc);
        return 0;
    }

    case MSG_KEYDOWN: {
        BOOL repaint = FALSE;
        switch (wParam) {
        case SCANCODE_ESCAPE:
            SendNotifyMessage(hWnd, MSG_CLOSE, 0, 0);
            break;

        case SCANCODE_ENTER:
            _curr_draw_func_idx++;
            _curr_draw_func_idx %= TABLESIZE(_draw_func_list);
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKLEFT:
            _curr_scale_x /= 1.2;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKRIGHT:
            _curr_scale_x *= 1.2;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKUP:
            _curr_scale_y *= 1.2;
            repaint = TRUE;
            break;

        case SCANCODE_CURSORBLOCKDOWN:
            _curr_scale_y /= 1.2;
            repaint = TRUE;
            break;

        case SCANCODE_PAGEUP:
            _curr_rotate_angle += acos(-1.0)/6;
            repaint = TRUE;
            break;

        case SCANCODE_PAGEDOWN:
            _curr_rotate_angle -= acos(-1.0)/6;
            repaint = TRUE;
            break;

        }

        if (repaint)
            InvalidateRect(hWnd, NULL, FALSE);
        break;
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

    surface = create_cairo_surface (HDC_INVALID, WIDTH, HEIGHT);
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

