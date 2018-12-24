/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "view/rootview.h"

#include "graphics/image.h"
#include "drawable/imagedrawable.h"

namespace hfcl {

RootView::RootView()
    : ViewContainer(0, NULL, NULL)
{
    m_hwnd = HWND_INVALID;
    m_old_proc = NULL;
}

RootView::~RootView()
{
    if (m_hwnd != HWND_INVALID) {
        detachFromSysWindow();
    }
}

bool RootView::attachToSysWindow(HWND hwnd)
{
    if (m_hwnd != HWND_INVALID && hwnd != HWND_INVALID) {
        m_old_proc = SetWindowCallbackProc (hwnd, rootViewProc);
        SetWindowAdditionalData (hwnd, (DWORD)this);
        m_hwnd = hwnd;

        InvalidateRect (hwnd, NULL, true);
        return true;
    }

    return false;
}

bool RootView::detachFromSysWindow()
{
    if (m_hwnd == HWND_INVALID)
        return false;

    SetWindowCallbackProc (m_hwnd, m_old_proc);
    SetWindowAdditionalData (m_hwnd, 0);
    m_hwnd = HWND_INVALID;
    m_old_proc = NULL;
    return true;
}

RootView* RootView::hwndToView(HWND hwnd)
{
    return reinterpret_cast<RootView*>(GetWindowAdditionalData(hwnd));
}

LRESULT RootView::rootViewProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam)
{
    RootView* _view = RootView::hwndToView(hWnd);
    if (_view == NULL) {
        _DBG_PRINTF ("RootView::rootViewProc: _view is NULL\n");
        return 0;
    }

    switch (message) {
    case MSG_KEYDOWN:
        return _view->onKeyMessage(Event::KEY_DOWN, wParam, lParam);

    case MSG_KEYUP:
        return _view->onKeyMessage(Event::KEY_UP, wParam, lParam);

    case MSG_KEYLONGPRESS:
        return _view->onKeyMessage(Event::KEY_LONGPRESSED, wParam, lParam);

    case MSG_KEYALWAYSPRESS:
        return _view->onKeyMessage(Event::KEY_ALWAYSPRESS, wParam, lParam);

    case MSG_LBUTTONDOWN:
        return _view->onMouseMessage(Event::MOUSE_DOWN, wParam, lParam);

    case MSG_LBUTTONUP:
        return _view->onMouseMessage(Event::MOUSE_UP, wParam, lParam);

    case MSG_MOUSEMOVE:
        return _view->onMouseMessage(Event::MOUSE_MOVE, wParam, lParam);

    case MSG_IDLE:
        _view->onIdle();
        return 0;

    case MSG_ERASEBKGND:
        return 0;

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        if (_view && !_view->isFrozen() && IsWindowVisible(hWnd)) {
            RECT rcBounds;
            GetBoundsRect(hdc, &rcBounds);

            GraphicsContext gc(hdc);
            IntRect rcView(0, 0, _view->getRect().width(),
                            _view->getRect().height());
            IntRect rcInv(rcBounds.left, rcBounds.top,
                            rcBounds.right, rcBounds.bottom);
            if (rcInv.intersect(rcView)) {
                _view->drawContent(&gc, rcView);
                _view->drawScrollBar(&gc, rcView);

                if (!IsWindowEnabled(hWnd)) {
                    GraphicsContext overlay_gc (hdc);
                    overlay_gc.fillRect(rcView, 0, 0, 0, 256*3/10);
                }
            }
        }

        EndPaint(hWnd, hdc);
        SyncUpdateDC(HDC_SCREEN);
        return 0;
    }

    case MSG_CLOSE:
        _view->detachFromSysWindow();
        return 0;

    default:
        break;
    }

    return _view->m_old_proc(hWnd, message, wParam, lParam);
}

int RootView::scancode2keycode(int scancode)
{
    int keycode = KeyEvent::KEYCODE_UNKNOWN;
    if (scancode >= SCANCODE_1 && scancode <= SCANCODE_9) {
        keycode = KeyEvent::KEYCODE_1 + (scancode - SCANCODE_1);
    }
    else if (scancode >= SCANCODE_F1 && scancode <= SCANCODE_F10) {
        keycode = KeyEvent::KEYCODE_STAR + (scancode - SCANCODE_F10);
    }
    else if (scancode >= SCANCODE_HOME && scancode <= SCANCODE_PAGEDOWN) {
        keycode = KeyEvent::KEYCODE_HOME + (scancode - SCANCODE_HOME);
    }
    else {
        switch (scancode) {
        case SCANCODE_0:
            keycode = KeyEvent::KEYCODE_0;
            break;

        case SCANCODE_SPACE:
            keycode = KeyEvent::KEYCODE_SPACE;
            break;

        case SCANCODE_ESCAPE:
            keycode = KeyEvent::KEYCODE_BACK;
            break;

        case SCANCODE_F11:
            keycode = KeyEvent::KEYCODE_START;
            break;

        case SCANCODE_F12:
            keycode = KeyEvent::KEYCODE_STOP;
            break;

        case SCANCODE_PAUSE:
            keycode = KeyEvent::KEYCODE_PAUSE;
            break;

        case SCANCODE_ENTER:
            keycode = KeyEvent::KEYCODE_ENTER;
            break;

        case SCANCODE_POWER:
            keycode = KeyEvent::KEYCODE_POWER;
            break;

        case SCANCODE_BACKSPACE:
            keycode = KeyEvent::KEYCODE_BACKSPACE;
            break;

        case SCANCODE_REMOVE:
            keycode = KeyEvent::KEYCODE_REMOVE;
            break;
        }
    }

    return keycode;
}

int RootView::onKeyMessage(Event::EventType keytype,
        WPARAM wParam, LPARAM lParam)
{
    int keycode = scancode2keycode(wParam);
    KeyEvent event(keytype, keycode, wParam, lParam);
    dispatchEvent(&event);
    return 0;
}

int RootView::onMouseMessage(Event::EventType mouseType,
        WPARAM wParam, LPARAM lParam)
{
    int x_pos = LOSWORD (lParam);
    int y_pos = HISWORD (lParam);

    MouseEvent ev(mouseType, x_pos, y_pos, lParam);
    dispatchEvent(&ev);
    return 0;
}

} // namespace hfcl

