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


#undef DEBUG

#include "activity/window.h"

#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "view/rootview.h"
#include "mgcl/mgcl.h"
#include "activity/activitymanager.h"
#include "graphics/color.h"

#if 0
#include "appmanager.h"
#include "graphics/graphicscontext.h"
#include "resource/respackage.h"
#include "resource/respkgmanager.h"
#endif

namespace hfcl {

Window::Window()
    : m_sysWnd(HWND_INVALID)
    , m_rootView(0)
{
}

Window::~Window()
{
    destroy();
}

HWND Window::createMainWindow (const char* caption, WNDPROC proc,
        int x, int y, int width, int height, DWORD addData, bool visible)
{
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = visible?WS_VISIBLE:WS_NONE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = caption;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = proc;
    CreateInfo.lx = x;
    CreateInfo.ty = y;
    CreateInfo.rx = x + width;
    CreateInfo.by = y + height;
    CreateInfo.iBkColor = COLOR_black;
    CreateInfo.dwAddData = addData;
    CreateInfo.hHosting = ActivityManager::getInstance()->hosting();

    return CreateMainWindow(&CreateInfo);
}

//create main window and set m_sysWnd. only need call once in onCreate.
bool Window::doCreate(int x, int y, int w, int h, bool visible)
{
    //has valid window
    if (m_sysWnd != HWND_INVALID)
        return false;

    m_sysWnd = createMainWindow ("window", defaultMainWindowProc,
            x, y, w, h, (DWORD)this, visible);

    if (m_sysWnd == HWND_INVALID)
        return false;

    SetWindowAdditionalData(m_sysWnd, (DWORD)this);

    if (visible)
        setActiveWindow(m_sysWnd);

    return true;
}

bool Window::create()
{
    return doCreate(0, 0,
            GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL),
            GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL));
}

// destroy view window and other resources
void Window::destroy()
{
    if(GetWindowAdditionalData(m_sysWnd) == (DWORD)this) {
        SetWindowAdditionalData(m_sysWnd, 0);
        DestroyMainWindow(m_sysWnd);
    }
}

Window* Window::getObject(HWND hwnd)
{
    return reinterpret_cast<Window*>(GetWindowAdditionalData(hwnd));
}

void Window::show(bool updateBg)
{
    ShowWindow(m_sysWnd, SW_SHOWNORMAL);
    InvalidateRect(m_sysWnd, NULL, updateBg);
}

void Window::getClientRect(IntRect& irc) const
{
    RECT rc;

    GetClientRect(m_sysWnd, &rc);
    irc.setRect(rc.left, rc.top, rc.right, rc.bottom);
}

void Window::setWindowRect(const IntRect& irc)
{
    MoveWindow(m_sysWnd, irc.left(), irc.top(), irc.width(), irc.height(),
               TRUE);
}

void Window::hide()
{
    ShowWindow(m_sysWnd, SW_HIDE);
}

void Window::asyncUpdateRect(int x, int y, int w, int h, bool upBackGnd)
{
    RECT rc = {x, y, x + w, y + h};
    _DBG_PRINTF ("Window::asyncUpdateRect called with (%d, %d, %d, %d)",
                 rc.left, rc.top, rc.right, rc.bottom);
    InvalidateRect(m_sysWnd, &rc, FALSE);
}

void Window::syncUpdateRect(int x, int y, int w, int h, bool upBackGnd)
{
    RECT rc = {x, y, x + w, y + h};

    InvalidateRect (m_sysWnd, &rc, FALSE);
    UpdateInvalidClient (m_sysWnd, FALSE);
}

void Window::drawBackground(GraphicsContext* context, IntRect &rc)
{
    context->fillRect(rc,
            GetRValue(Color::LAYER_COLOR_KEY),
            GetGValue(Color::LAYER_COLOR_KEY),
            GetBValue(Color::LAYER_COLOR_KEY), 0xFF);
}

HWND Window::setActiveWindow(HWND hMainWnd)
{
    return SetActiveWindow(hMainWnd);
}

HWND Window::getActiveWindow()
{
    return GetActiveWindow();
}

void Window::updateWindow(bool isUpdateBkg)
{
    UpdateWindow(m_sysWnd, isUpdateBkg);
}

int Window::doModal(bool bAutoDestory)
{
    return mgclDoModal(m_sysWnd, bAutoDestory);
}

unsigned int Window::doModalView()
{
    return mgclDoModalView(m_sysWnd);
}

void Window::endDlg(int endCode)
{
    SendNotifyMessage(m_sysWnd, MGCL_MSG_MNWND_ENDDIALOG,
            0, (LPARAM)endCode);
}

KeyEvent::KeyCode Window::scancode2keycode(int scancode)
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

    return (KeyEvent::KeyCode)keycode;
}

bool Window::onKeyEvent(const KeyEvent* event)
{
    return true;
}

bool Window::onMouseEvent(const MouseEvent* event)
{
    return true;
}

bool Window::onMouseWheelEvent(const MouseWheelEvent* event)
{
    return true;
}

int Window::onKeyMessage(KeyEvent::KeyEventType keytype,
        WPARAM wParam, LPARAM lParam)
{
    KeyEvent::KeyCode keycode = scancode2keycode(wParam);
    KeyEvent event(keytype, keycode, wParam, lParam);
    onKeyEvent(&event);
    return 0;
}

int Window::onMouseMessage(MouseEvent::MouseEventType mouseType,
        WPARAM wParam, LPARAM lParam)
{
    int x_pos = LOSWORD (lParam);
    int y_pos = HISWORD (lParam);
    MouseEvent event(mouseType, x_pos, y_pos, wParam);
    onMouseEvent(&event);
    return 0;
}

LRESULT Window::defaultMainWindowProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam)
{
    Window* mainwnd = Window::getObject(hWnd);
    if (mainwnd == NULL) {
        _DBG_PRINTF("Window::defaultMainWindowProc: mainwnd is NULL\n");
        return 0;
    }

    switch (message) {
    case MSG_KEYDOWN:
        return mainwnd->onKeyMessage(KeyEvent::KEY_DOWN, wParam, lParam);

    case MSG_KEYUP:
        return mainwnd->onKeyMessage(KeyEvent::KEY_UP, wParam, lParam);

    case MSG_KEYLONGPRESS:
        return mainwnd->onKeyMessage(KeyEvent::KEY_LONGPRESSED, wParam, lParam);

    case MSG_KEYALWAYSPRESS:
        return mainwnd->onKeyMessage(KeyEvent::KEY_ALWAYSPRESS, wParam, lParam);

    case MSG_LBUTTONDOWN:
        return mainwnd->onMouseMessage(MouseEvent::MOUSE_L_DOWN, wParam, lParam);

    case MSG_LBUTTONUP:
        return mainwnd->onMouseMessage(MouseEvent::MOUSE_L_UP, wParam, lParam);

    case MSG_MOUSEMOVE:
        return mainwnd->onMouseMessage(MouseEvent::MOUSE_MOVE, wParam, lParam);

    case MSG_IDLE:
        mainwnd->onIdle();
        return 0;

    case MSG_ERASEBKGND:
        return 0;

    case MSG_PAINT: {
        HDC hdc = BeginPaint(hWnd);
        if (mainwnd && IsWindowVisible(hWnd)) {
            RECT rcBounds;
            GetBoundsRect(hdc, &rcBounds);
            IntRect rcInv(rcBounds);

            GraphicsContext gc(hdc);
            RootView* root = mainwnd->getRootView();
            if (root) {
                root->onPaint(&gc);

                if (!IsWindowEnabled(hWnd)) {
                    GraphicsContext overlay_gc (hdc);
                    overlay_gc.fillRect(rcInv, 0, 0, 0, 256*3/10);
                }
            }
            else {
                gc.fillRect(rcInv, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }

        EndPaint(hWnd, hdc);
        SyncUpdateDC(HDC_SCREEN);
        return 0;
    }

    default:
        break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

bool Window::setRootView(RootView* root)
{
    if (m_rootView) {
        m_rootView->freeze();
    }

    if (root) {
        if (root->attachToSysWindow(this)) {
            m_rootView = root;
            root->unfreeze();
            return true;
        }
        else
            return false;
    }

    m_rootView = NULL;
    return true;
}

} // namespace hfcl

