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

bool Window::m_updateLocked = false;

Window::Window()
    : PanelView(0, NULL, NULL)
    , m_viewWindow(HWND_INVALID)
    , m_context(0)
    , m_keyLockable(false)
    , m_keyLocked(false)
    , m_freezePaint(false)
{
    m_updateLocked = false;
    m_bkg_alpha = 255;
    setParent(NULL);
}

Window::~Window()
{
    destroy();
}

void Window::show(bool bUpdateBack)
{
    ShowWindow(m_viewWindow, SW_SHOWNORMAL);
    InvalidateRect(m_viewWindow, NULL, bUpdateBack);
}

void Window::getClientRect(IntRect& irc)
{
    RECT rc;

    GetClientRect(m_viewWindow, &rc);
    irc.setRect(rc.left, rc.top, rc.right, rc.bottom);
}

void Window::setWindowRect(const IntRect& irc)
{
    MoveWindow(m_viewWindow, irc.left(), irc.top(), irc.width(), irc.height(),
               TRUE);
}

void Window::hide(void)
{
    ShowWindow(m_viewWindow, SW_HIDE);
}

// destroy view window and other resources
void Window::destroy(void)
{
    if(GetWindowAdditionalData(m_viewWindow) == (DWORD)this) {
        SetWindowAdditionalData(m_viewWindow, 0);
        DestroyMainWindow(m_viewWindow);
    }
}

bool Window::onKey (int keyCode, KeyEvent* event)
{
    View* _focus = NULL;

    if ((View *)0 != (_focus = getFocusedChild())) {
        _focus->dispatchEvent(event);
    }

    return true;
}

void Window::onClick(POINT pt, Event::EventType type)
{
    // nothing...
}

void Window::inner_updateView(int x, int y, int w, int h, bool upBackGnd)
{
    asyncUpdateRect(x, y, w, h, upBackGnd);
}

void Window::asyncUpdateRect(int x, int y, int w, int h, bool upBackGnd)
{
    RECT rc = {x, y, x + w, y + h};
    if (m_keyLockable) {
        m_keyLocked = true;
    }

    if (!m_updateLocked) {
        _DBG_PRINTF ("Window::asyncUpdateRect called with (%d, %d, %d, %d)",
                     rc.left, rc.top, rc.right, rc.bottom);
        InvalidateRect(m_viewWindow, &rc, FALSE);
    }
}

void Window::syncUpdateRect(int x, int y, int w, int h, bool upBackGnd)
{
    RECT rc = {x, y, x + w, y + h};

    InvalidateRect (m_viewWindow, &rc, FALSE);
    UpdateInvalidClient (m_viewWindow, FALSE);
}

void Window::drawScrollBar(GraphicsContext* context, IntRect &rc)
{
    PanelView::drawScrollBar (context, rc);
    m_keyLocked = false;
}

void Window::drawBackground(GraphicsContext* context, IntRect &rc)
{
    context->fillRect(rc,
            GetRValue(Color::LAYER_COLOR_KEY),
            GetGValue(Color::LAYER_COLOR_KEY),
            GetBValue(Color::LAYER_COLOR_KEY), 0xFF);
}

HWND Window::getSysWindow(void)
{
    return m_viewWindow;
}

HWND Window::viewWindow(void) const
{
    return m_viewWindow;
}

Window* Window::window(HWND hwnd)
{
    return reinterpret_cast<Window*>(GetWindowAdditionalData(hwnd));
}

HWND Window::setActiveWindow(HWND hMainWnd)
{
    return SetActiveWindow(hMainWnd);
}

HWND Window::getActiveWindow(void)
{
    return GetActiveWindow();
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

//create main window and set m_viewWindow. only need call once in onCreate.
bool Window::createMainWindow(int x, int y, int w, int h, bool visible)
{
    //has valid window
    if (m_viewWindow != HWND_INVALID)
        return false;

    m_viewWindow = createMainWindow ("window", defaultAppProc,
            x, y, w, h, (DWORD)this, visible);

    if (m_viewWindow == HWND_INVALID)
        return false;

    setRect(0, 0, w, h);
    SetWindowAdditionalData(m_viewWindow, (DWORD)this);

    if (visible)
        setActiveWindow(m_viewWindow);

    return true;
}

bool Window::createMainWindow(void)
{
    return createMainWindow(0, 0,
            GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL),
            GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL));
}

void Window::updateWindow(bool isUpdateBkg)
{
    UpdateWindow(m_viewWindow, isUpdateBkg);
}

int Window::doModal(bool bAutoDestory)
{
    return mgclDoModal(m_viewWindow, bAutoDestory);
}

unsigned int Window::doModalView()
{
    return mgclDoModalView(m_viewWindow);
}

void Window::endDlg(int endCode)
{
    SendNotifyMessage(m_viewWindow, MGCL_MSG_MNWND_ENDDIALOG,
            0, (LPARAM)endCode);
}

} // namespace hfcl

