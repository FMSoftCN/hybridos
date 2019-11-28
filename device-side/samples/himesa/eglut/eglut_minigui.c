/*
 * Copyright (C) 2019 FMSoft Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Vincent Wei <vincent@minigui.org>
 */

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "eglutint.h"

void
_eglutNativeInitDisplay(int argc, char* argv[])
{
    if (InitGUI (argc, (const char**)argv) != 0)
        _eglutFatal("failed to initialize native display");

    _eglut->native_dpy = GetVideoHandle(HDC_SCREEN);
    if (!_eglut->native_dpy)
        _eglutFatal("failed to get the handle of native display");

    _eglut->surface_type = EGL_WINDOW_BIT;
}

void
_eglutNativeFiniDisplay(void)
{
    TerminateGUI (0);
}

static LRESULT eglWinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        break;

    case MSG_PAINT: {
        struct eglut_window *win =
            (struct eglut_window *)GetWindowAdditionalData(hWnd);

        HDC hdc = BeginPaint (hWnd);
        if (win->display_cb) {
            win->display_cb();
        }
        eglSwapBuffers (_eglut->dpy, win->surface);
        EndPaint (hWnd, hdc);
        return 0;
    }

    case MSG_KEYDOWN: {
        struct eglut_window *win =
            (struct eglut_window *)GetWindowAdditionalData(hWnd);
        int key = 0;

        if (win->special_cb == NULL)
            break;

        switch (wParam) {
        case SCANCODE_ESCAPE:
            key = EGLUT_KEY_ESC;
            break;
        case SCANCODE_F1 ... SCANCODE_F10:
            key = EGLUT_KEY_F1 + (wParam - SCANCODE_F1);
            break;
        case SCANCODE_F11:
            key = EGLUT_KEY_F11;
            break;
        case SCANCODE_F12:
            key = EGLUT_KEY_F12;
            break;
        case SCANCODE_CURSORBLOCKLEFT:
            key = EGLUT_KEY_LEFT;
            break;
        case SCANCODE_CURSORBLOCKUP:
            key = EGLUT_KEY_UP;
            break;
        case SCANCODE_CURSORBLOCKRIGHT:
            key = EGLUT_KEY_RIGHT;
            break;
        case SCANCODE_CURSORBLOCKDOWN:
            key = EGLUT_KEY_DOWN;
            break;
        default:
            key = 0;
            break;
        }

        if (key)
            win->special_cb(key);
        break;
    }

    case MSG_CHAR: {
        struct eglut_window *win =
            (struct eglut_window *)GetWindowAdditionalData(hWnd);
        int key = 0;

        if (win->keyboard_cb) {
            win->keyboard_cb((unsigned char)wParam);
        }
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

    case MSG_SIZECHANGED: {
        struct eglut_window *win =
            (struct eglut_window *)GetWindowAdditionalData(hWnd);
        RECT* rc = (RECT*)lParam;
        win->native.width = RECTWP(rc);
        win->native.height = RECTHP(rc);
        if (win->reshape_cb)
            win->reshape_cb(win->native.width, win->native.height);
        break;
    }

    case MSG_LBUTTONUP: {
        ReleaseCapture();
        break;
    }

    default:
        break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void
_eglutNativeInitWindow(struct eglut_window *win, const char *title,
                       int x, int y, int w, int h)
{
    HWND    mgwin;
    MAINWINCREATE create_info;

    create_info.dwStyle = WS_VISIBLE;
    create_info.dwExStyle = WS_EX_USEPRIVATECDC;
    create_info.spCaption = title;
    create_info.hMenu = 0;
    create_info.hCursor = 0;
    create_info.hCursor = GetSystemCursor(0);
    create_info.hIcon = 0;
    create_info.MainWindowProc = eglWinProc;
    create_info.lx = x;
    create_info.ty = y;
    create_info.rx = x + w;
    create_info.by = y + h;
    create_info.iBkColor = COLOR_lightwhite;
    create_info.dwAddData = (DWORD)win;
    create_info.hHosting = HWND_DESKTOP;
    mgwin = CreateMainWindow(&create_info);

    if (mgwin == HWND_INVALID) {
        _eglutFatal ("failed to create a window");
    }

    ShowWindow(mgwin, SW_SHOWNORMAL);

    win->native.u.window = (EGLNativeWindowType)mgwin;
    win->native.width = w;
    win->native.height = h;
}

void
_eglutNativeDamageWindow(struct eglut_window *win)
{
    HWND mgwin = (HWND)win->native.u.window;

    InvalidateRect(mgwin, NULL, FALSE);
}

void
_eglutNativeFiniWindow(struct eglut_window *win)
{
    HWND mgwin = (HWND)win->native.u.window;

    PostQuitMessage(mgwin);
    DestroyMainWindow(mgwin);
    MainWindowThreadCleanup(mgwin);
}

void
_eglutNativeEventLoop(void)
{
    struct eglut_window *win = _eglut->current;
    HWND mgwin = (HWND)win->native.u.window;
    MSG msg;

    while (GetMessage(&msg, mgwin)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (_eglut->idle_cb) {
            _eglut->idle_cb();
        }
    }
}

