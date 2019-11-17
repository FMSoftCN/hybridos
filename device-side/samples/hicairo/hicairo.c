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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <cairo/cairo.h>

#ifdef CAIRO_HAS_MINIGUI_SURFACE
#include <cairo/cairo-minigui.h>
#endif

#include "hicairo.h"

#ifdef _MGGAL_DRI

#include <cairo/cairo-drm.h>
#include <minigui/exstubs.h>

static void _destroy_memdc (void *data)
{
    HDC hdc = (HDC)data;

    if (hdc != HDC_INVALID) {
        DeleteMemDC(hdc);
    }

    _WRN_PRINTF("_destroy_memdc: invalid DC passed");
}

static cairo_user_data_key_t _dc_key = { _MINIGUI_VERSION_CODE };

#undef MEMDC_TO_SURFACE

/* this function always create memdc */
static cairo_surface_t *create_cairo_surface (HDC hdc, int width, int height)
{
    GHANDLE sh;
    int fd;
    static cairo_device_t* cd = NULL;
    cairo_surface_t* cs = NULL;
    DriSurfaceInfo info;

    if (cd == NULL) {
#ifdef CAIRO_HAS_MINIGUI_SURFACE
        sh = GetSurfaceHandle(HDC_SCREEN);
        if (!sh) {
            _ERR_PRINTF("hicairo: failed to get the surface handle of screen\n");
            goto fallback;
        }

        fd = driGetDeviceFD(sh);
        if (fd < 0) {
            _ERR_PRINTF("hicairo: failed to get the DRI device fd: %m\n");
            goto fallback;
        }

        _WRN_PRINTF ("calling cairo_drm_device_get_for_fd with fd (%d)", fd);
        cd = cairo_drm_device_get_for_fd (fd);
        if (cd == NULL) {
            _ERR_PRINTF("hicairo: failed to create cairo_device object: %m\n");
            goto fallback;
        }
#else
        cd = cairo_drm_device_default ();
        if (cd == NULL) {
            _ERR_PRINTF("hicairo: failed to create cairo_device object: %m\n");
            goto fallback;
        }
#endif
    }

#ifdef MEMDC_TO_SURFACE
    hdc = CreateMemDC (width, height,
            32, MEMDC_FLAG_HWSURFACE,
            0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
    if (hdc == HDC_INVALID) {
        _ERR_PRINTF("hicairo: failed to create memory DC\n");
        goto fallback;
    }

    SetBrushColor (hdc, RGB2Pixel (hdc, 0xFF, 0x00, 0x00));
    FillBox (hdc, 0, 0, 200, 200);

    sh = GetSurfaceHandle (hdc);
    if (!sh) {
        _ERR_PRINTF ("hicairo: failed to get the surface handle of memory DC\n");
        goto fallback;
    }

    if (driGetSurfaceInfo (sh, &info)) {
        _WRN_PRINTF ("calling cairo_drm_surface_create_for_handle with handle (%u), size (%lu), width(%u), height(%u), pitch(%u)",
                info.handle, info.size, info.width, info.height, info.pitch);
        cs = cairo_drm_surface_create_for_handle (cd, info.handle, info.size,
                CAIRO_FORMAT_RGB24, info.width, info.height, info.pitch);
    }
#else
    cs = cairo_drm_surface_create (cd, CAIRO_FORMAT_RGB24, width, height);
#endif

    if (cairo_surface_get_type (cs) == CAIRO_SURFACE_TYPE_DRM) {
        cairo_surface_set_user_data(cs, &_dc_key, (void*)hdc, _destroy_memdc);
        return cs;
    }
    else {
        _MG_PRINTF ("hicairo: the status of newly created surface is: %d\n",
                cairo_surface_status (cs));
        cairo_surface_destroy (cs);
    }

#ifdef CAIRO_HAS_MINIGUI_SURFACE
    DeleteMemDC(hdc);
#endif
    _ERR_PRINTF("hicairo: failed to create cairo drm surface\n");

fallback:
    _WRN_PRINTF("hicairo: use fallback cairo surface");
#ifdef CAIRO_HAS_MINIGUI_SURFACE
    return cairo_minigui_surface_create_with_memdc (CAIRO_FORMAT_RGB24, width, height);
#else
    return cairo_image_surface_create (CAIRO_FORMAT_RGB24, width, height);
#endif
}

#else

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
#endif

typedef int (*draw_func_t)(cairo_t *, int, int);

static HDC create_memdc_from_drm_surface(cairo_surface_t* drm_surface,
        cairo_surface_t** image_surface)
{
    cairo_status_t status;
    MYBITMAP my_bmp = {
        flags: MYBMP_TYPE_RGB | MYBMP_FLOW_DOWN,
        frames: 1,
        depth: 32,
    };

    *image_surface = cairo_drm_surface_map_to_image(drm_surface);
    status = cairo_surface_status (*image_surface);
    if (status) {
        _WRN_PRINTF("failed to map DRM surface: status (%d)", status);
    }

    my_bmp.w = cairo_image_surface_get_width (*image_surface);
    my_bmp.h = cairo_image_surface_get_height (*image_surface);
    my_bmp.pitch = cairo_image_surface_get_stride (*image_surface);
    my_bmp.bits = cairo_image_surface_get_data (*image_surface);
    my_bmp.size = my_bmp.pitch * my_bmp.h;

    _WRN_PRINTF("mapped surface info: width (%d), height (%d), pitch (%d), bits (%p)",
            my_bmp.w, my_bmp.h, my_bmp.pitch, my_bmp.bits);

    return CreateMemDCFromMyBitmap(&my_bmp, NULL);
}

static HDC destroy_memdc_for_drm_surface(HDC hdc,
        cairo_surface_t* drm_surface, cairo_surface_t* image_surface)
{
    DeleteMemDC (hdc);
    cairo_drm_surface_unmap(drm_surface, image_surface);
}

static HDC create_memdc_from_image_surface (cairo_surface_t* image_surface)
{
    MYBITMAP my_bmp = {
        flags: MYBMP_TYPE_RGB | MYBMP_FLOW_DOWN,
        frames: 1,
        depth: 32,
    };

    my_bmp.w = cairo_image_surface_get_width (image_surface);
    my_bmp.h = cairo_image_surface_get_height (image_surface);
    my_bmp.pitch = cairo_image_surface_get_stride (image_surface);
    my_bmp.bits = cairo_image_surface_get_data (image_surface);
    my_bmp.size = my_bmp.pitch * my_bmp.h;

    _WRN_PRINTF("mapped surface info: width (%d), height (%d), pitch (%d), bits (%p)",
            my_bmp.w, my_bmp.h, my_bmp.pitch, my_bmp.bits);

    return CreateMemDCFromMyBitmap(&my_bmp, NULL);
}

static HDC destroy_memdc_for_image_surface(HDC hdc,
        cairo_surface_t* image_surface)
{
    DeleteMemDC (hdc);
}

static float paint (HWND hwnd, HDC hdc, draw_func_t draw_func,
        double sx, double sy, double angle,
        int width, int height)
{
    HDC csdc = HDC_INVALID;
    int count = 1;
    struct timespec start_ts, end_ts;
    float time_ms;

    cairo_t* cr = (cairo_t*)GetWindowAdditionalData(hwnd);
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

    {
        cairo_surface_t* image_surface = NULL;
        cairo_surface_t* target_surface = cairo_get_target(cr);
        //cairo_surface_flush (target_surface);
        cairo_surface_type_t cst = cairo_surface_get_type (target_surface);
#ifdef CAIRO_HAS_MINIGUI_SURFACE
        if (cst == CAIRO_SURFACE_TYPE_MINIGUI) {
            csdc = cairo_minigui_surface_get_dc (target_surface);
        }
#else
        if (cst == CAIRO_SURFACE_TYPE_IMAGE) {
            csdc = create_memdc_from_image_surface (target_surface);
        }
#endif
        else {
            _WRN_PRINTF("surface type: %d", cst);
#ifdef MEMDC_TO_SURFACE
            csdc = (HDC)cairo_surface_get_user_data(target_surface, &_dc_key);
#else
            csdc = create_memdc_from_drm_surface (target_surface, &image_surface);
#endif
        }

        if (csdc == HDC_INVALID) {
            _ERR_PRINTF("hicairo: failed to get the DC associated with the target surface\n");
            exit (1);
        }

        if (csdc != HDC_SCREEN && csdc != HDC_INVALID) {
            _MG_PRINTF("calling BitBlt\n");
            BitBlt(csdc, 0, 0, width, height, hdc, 0, 0, 0);
        }

        if (image_surface) {
            _MG_PRINTF("calling destroy_memdc_for_drm_surface\n");
            destroy_memdc_for_drm_surface (csdc, target_surface, image_surface);
        }
#ifndef CAIRO_HAS_MINIGUI_SURFACE
        else {
            destroy_memdc_for_image_surface (csdc, target_surface);
        }
#endif
    }

    return time_ms;
}

static int draw_test (cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb (cr, 1.0, 0, 0);
    cairo_rectangle (cr, width * 0.25, height * 0.25, width * 0.25, height * 0.25);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    cairo_set_source_rgb (cr, 1.0, 1.0, 0);
    cairo_rectangle (cr, width * 0.50, height * 0.50, width * 0.25, height * 0.25);
    cairo_fill (cr);
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

#ifndef CAIRO_HAS_MINIGUI_SURFACE
DriDriverOps* __dri_ex_driver_get(const char* driver_name)
{
    return NULL;
}

#endif

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

