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
 *   This file is part of mGPlus, a component for MiniGUI.
 *
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Or,
 *
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 *
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 *
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <ctype.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <cairo/cairo.h>
#include <cairo/cairo-minigui.h>

#include "lion.inc"

#define WIDTH 800
#define HEIGHT 600

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

static LRESULT SampleWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        break;

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        TextOut (hdc, 10, 190, "Drag the mouse up and down." );
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

