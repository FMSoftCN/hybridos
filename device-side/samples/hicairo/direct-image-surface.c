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
#include <time.h>

#define _DEBUG

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <cairo/cairo.h>
#include <cairo/cairo-minigui.h>

#include "hicairo.h"

static cairo_surface_t *create_direct_image_surface (HDC hdc, const RECT* rc)
{
    cairo_surface_t *surface;
    Uint8* bits;
    int width, height, pitch;

    bits = LockDC (hdc, rc, &width, &height, &pitch);

    if (bits == NULL || width <= 0 || height <= 0 || pitch <= 0) {
        _ERR_PRINTF("hicairo: failed LockDC()\n");
        goto FAIL;
    }

    // format here must be CAIRO_FORMAT_RGB16_565.
    // See call of CreateMainWindowEx2.
    surface = cairo_image_surface_create_for_data (bits,
            CAIRO_FORMAT_RGB16_565, width, height, pitch);
    if (surface == NULL) {
        _ERR_PRINTF("hicairo: failed when creating direct image surface\n");
        goto FAIL;
    }

    return surface;

FAIL:
    return NULL;
}

static cairo_surface_t *finish_direct_image_surface (cairo_surface_t *surface, HDC hdc)
{
    _DBG_PRINTF("calling finish_direct_image_surface\n");

    cairo_surface_finish(surface);
    cairo_surface_destroy(surface);

    UnlockDC (hdc);
    return NULL;
}

typedef int (*draw_func_t)(cairo_t *, int, int);

static float paint (HWND hwnd, HDC hdc, draw_func_t draw_func,
        double sx, double sy, double angle,
        int width, int height)
{
    cairo_surface_t* surface = NULL;
    int count = 100;
    struct timespec start_ts, end_ts;
    float time_ms;

    surface = create_direct_image_surface (hdc, NULL);
    cairo_t* cr = cairo_create (surface);
    if (cr == NULL) {
        _ERR_PRINTF("hicairo: failed when creating cairo context\n");
        exit (1);
    }

    if (cr == NULL) {
        _ERR_PRINTF("hicairo: failed to get the cairo context\n");
        exit (1);
    }

    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_ts);
    while (count--) {
        cairo_save(cr);

        cairo_scale(cr, sx, sy);
        cairo_rotate(cr, angle);
        draw_func(cr, width, height);
        cairo_restore(cr);
    }
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_ts);

    {
        DWORD ds, dms;

        ds = (end_ts.tv_sec - start_ts.tv_sec);

        if (end_ts.tv_sec == start_ts.tv_sec) {
            dms = (end_ts.tv_nsec - start_ts.tv_nsec) / 1000000L;
        }
        else if (end_ts.tv_nsec >= start_ts.tv_nsec) {
            dms = (end_ts.tv_nsec - start_ts.tv_nsec) / 1000000L;
        }
        else {
            assert(ds > 0);

            ds--;
            dms = 1000L - (start_ts.tv_nsec - end_ts.tv_nsec) / 1000000L;
        }

        time_ms = ds * 1000 + dms;
    }

    if (cr) {
        cairo_destroy(cr);
        finish_direct_image_surface(surface, hdc);
    }

    return time_ms;
}

static int draw_test (cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb (cr, 1.0, 0, 0);
    cairo_rectangle (cr, width * 0.25, height * 0.25, width * 0.5, height * 0.5);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    //cairo_set_source_rgba (cr, 1.0, 1.0, 0, 0.5);
    //cairo_rectangle (cr, width * 0.50, height * 0.50, width * 0.5, height * 0.5);
    //cairo_fill (cr);
}

static int _curr_draw_func_idx = 0;
static draw_func_t _draw_func_list [] = {
    draw_test,
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
        float time_ms;
        char buff[256];

        HDC hdc = BeginPaint (hWnd);
        time_ms = paint (hWnd, hdc, _draw_func_list[_curr_draw_func_idx],
            _curr_scale_x, _curr_scale_y, _curr_rotate_angle,
            _curr_width, _curr_height);
        sprintf (buff, "One hundred renderings took %.6f micro seconds", time_ms);
        TextOut (hdc, 10, 10, buff);
        EndPaint (hWnd, hdc);
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

#ifdef _MGSCHEMA_COMPOSITING
int MiniGUIMain(int argc, const char *argv[])
{
    MSG     Msg;
    HWND    hMainWnd;
    MAINWINCREATE CreateInfo;

    int retval = 0;

    JoinLayer(NAME_DEF_LAYER, "hicairo", 0, 0);

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
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindowEx2 (&CreateInfo, 0L, NULL, NULL,
            ST_PIXEL_XRGB565, MakeRGBA (0x00, 0x00, 0x00, 0xFF),
            CT_OPAQUE, 0);

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
    return retval;
}

#else   /* defined _MGSCHEMA_COMPOSITING */

int main (int argc, const char* argv[])
{
    _WRN_PRINTF ("This sample program only works on compositing schema.\n");
    return 0;
}

#endif  /* not defined _MGSCHEMA_COMPOSITING */

