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
    : PanelView(NULL)
    , m_viewWindow(HWND_INVALID)
    , m_context(0)
    , m_keyLockable(false)
    , m_keyLocked(false)
    , m_freezePaint(false)
{
    m_updateLocked = false;
    m_bkg_alpha = 255;
    setDrawableSet(NULL);
    setParent(NULL);
    setLayer(-1);
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

    if ((View *)0 != (_focus = focusView())) {
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

void Window::drawScroll(GraphicsContext* context, IntRect &rc)
{
    PanelView::drawScroll (context, rc);
    m_keyLocked = false;
}

void Window::drawBackground(GraphicsContext* context, IntRect &rc)
{
    if (m_drawLayer == -1) {
        for (int i = 0; i < context->getLayers(); i++) {
            context->setLayer(i);
            context->fillRect(rc,
                              GetRValue(Color::LAYER_COLOR_KEY),
                              GetGValue(Color::LAYER_COLOR_KEY),
                              GetBValue(Color::LAYER_COLOR_KEY), m_alpha);
                              //the default value is 255
                              //GetBValue(Color::LAYER_COLOR_KEY), 0xFF);
            if (i != 0){
                context->setLayerColorKey(i, TRUE,
                        GetRValue(Color::LAYER_COLOR_KEY),
                        GetGValue(Color::LAYER_COLOR_KEY),
                        GetBValue(Color::LAYER_COLOR_KEY));
            }
        }
    }
    else {
        context->setLayer(m_drawLayer);
        context->fillRect(rc,
                GetRValue(Color::LAYER_COLOR_KEY),
                GetGValue(Color::LAYER_COLOR_KEY),
                GetBValue(Color::LAYER_COLOR_KEY), m_alpha);
                // the default value is 255
                //GetBValue(Color::LAYER_COLOR_KEY), 0xFF);
        if (m_drawLayer != 0){
            context->setLayerColorKey(m_drawLayer, TRUE,
                    GetRValue(Color::LAYER_COLOR_KEY),
                    GetGValue(Color::LAYER_COLOR_KEY),
                    GetBValue(Color::LAYER_COLOR_KEY));
        }
        //PanelView::drawBackground (context, rc, status);
    }
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

int Window::sendKeyMessage(Event::EventType keytype,
        WPARAM wParam, LPARAM lParam)
{
    if (!m_keyLocked) {
        KeyEvent event(keytype, wParam, lParam);
        onKey (event.keyCode(), &event);
    }
    return 0;
}

int Window::sendMouseMessage(Event::EventType mouseType,
        WPARAM wParam, LPARAM lParam)
{
    //DWORD key_flags = (DWORD)wParam;
    int x_pos = LOSWORD (lParam);
    int y_pos = HISWORD (lParam);

    View* f = getChildByPosition(x_pos, y_pos);

    if (f == NULL)
        return 0;

    f->windowToView(&x_pos, &y_pos);
    MouseEvent evt(mouseType, x_pos, y_pos);
    f->dispatchEvent(&evt);

    //FIXME, haven't be tested.
    if (mouseType == Event::MOTION_DOWN) {
        m_mouseDownView = f;
    } else if (mouseType == Event::MOTION_UP) {
        if (m_mouseDownView == f){
            MouseEvent ev(Event::MOTION_CLICK, x_pos, y_pos);
            f->dispatchEvent(&ev);
        }
        m_mouseDownView = NULL;
    } else if (mouseType == Event::MOTION_MOVE) {
        if (m_mouseMoveView == NULL) {
            m_mouseMoveView = f;
        } else if (m_mouseMoveView != f) {
            MouseEvent e(Event::MOTION_MOVEOUT, x_pos, y_pos);
            m_mouseMoveView->dispatchEvent(&e);
            MouseEvent ev(Event::MOTION_MOVEIN, x_pos, y_pos);
            m_mouseMoveView->dispatchEvent(&ev);
            m_mouseMoveView = f;
        }
    }
    return 0;
}

int Window::sendIdleMessage()
{
    onIdle();
    return 0;
}

LRESULT Window::defaultAppProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* window = Window::window(hWnd);
    switch (message) {
        case MSG_KEYDOWN:
            return window ? window->sendKeyMessage(Event::KEY_DOWN, wParam, lParam) : 0;

        case MSG_KEYUP:
            return window ? window->sendKeyMessage(Event::KEY_UP, wParam, lParam) : 0;

        case MSG_KEYLONGPRESS:
            return window ? window->sendKeyMessage(Event::KEY_LONGPRESSED, wParam, lParam) : 0;

        case MSG_KEYALWAYSPRESS:
            return window ? window->sendKeyMessage(Event::KEY_ALWAYSPRESS, wParam, lParam) : 0;

        // VincentWei: disable mouse events.
        case MSG_LBUTTONDOWN:
            return 0; //window ? window->sendMouseMessage(Event::MOTION_DOWN, wParam, lParam) : 0;

        case MSG_LBUTTONUP:
            return 0; //window ? window->sendMouseMessage(Event::MOTION_UP, wParam, lParam) : 0;

        case MSG_MOUSEMOVE:
            return 0; //window ? window->sendMouseMessage(Event::MOTION_MOVE, wParam, lParam) : 0;

        case MSG_USER:
            return window ? window->sendKeyMessage(Event::CUSTOM_NOTIFY, wParam, lParam) : 0;

        case MSG_IDLE:
            return window ? window->sendIdleMessage() : 0;

        case MSG_ERASEBKGND: {
            // VincentWei: skip erasing background.
#if 0
#  ifdef __MMI_SAMSUNG_GT_FEATURE__
            if(window) {
                GraphicsContext gc((HDC)wParam);
                IntRect rc;
                GetBoundsRect((HDC)wParam, (RECT*)&rc);
                if (window->layer() >= 0) {
                    gc.setLayer(window->layer());
                }
                window->drawBackground(&gc, rc, 0);
            }
#  else
            TRANSPARENTINFO  trans_info;
            HDC hdc_trans = mgclBeginTransparentBkgnd(hWnd, (HDC)wParam, window->bkgAlpha(),  &trans_info);
            if(window)
            {
                GraphicsContext gc(hdc_trans);
                IntRect rc;
                GetClientRect(hWnd, (RECT*)&rc);
                window->drawBackground(&gc, rc, 0);
            }
            mgclEndTransparentBkgnd(&trans_info);
#  endif
//#else
            if(window) {
                GraphicsContext gc((HDC)wParam);
                IntRect rc;
                GetBoundsRect((HDC)wParam, (RECT*)&rc);
                window->markDirty(rc);
            }
#endif
            return 0;
        }

        case MSG_PAINT: {
#if 0 /* VincentWei: no use code */
            MMI_PROFILE_FUNC_ENTER(defaultAppProc_PAINT);
#endif
            HDC hdc = BeginPaint(hWnd);
            if (window && !window->isPaintFrozen() && IsWindowVisible (hWnd)) {
                RECT rcBounds;
                GetBoundsRect (hdc, &rcBounds);

                GraphicsContext gc(hdc);
                IntRect rcView (0, 0, window->getRect().width(), window->getRect().height());
                IntRect rcInv (rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom);
                if (rcInv.intersect (rcView)) {
                    if (window->layer() >= 0) {
                        gc.setLayer(window->layer());
                    }
                    window->drawContent (&gc, rcView);
                    window->drawScroll (&gc, rcView);

                    if (!IsWindowEnabled (hWnd)) {
                        /* VincentWei: we need a new gc for overlay mask */
                        GraphicsContext overlay_gc (hdc);
                        overlay_gc.fillRect (rcView, 0, 0, 0, 256*3/10);
                    }
                }
            }

            EndPaint(hWnd, hdc);
            SyncUpdateDC (HDC_SCREEN);
#if 0 /* VincentWei: no use code */
            MMI_PROFILE_FUNC_EXIT(defaultAppProc_PAINT);
#endif
            return 0;
        }

        case MSG_DESTROY:
            if(window) {
                SetWindowAdditionalData(hWnd, 0);
                HFCL_DELETE(window);
            }
            return 0;

        case MSG_CLOSE:
            if (window)
                window->destroy();
            return 0;

        // TODO: change theme msg
#if 0
        case HFCL_MSG_CHANGE_THEME:
            if(window) {
                window->changeTheme();
                // may crash here!!! FIXME
                window->updateView();
            }
            return 0;
#endif

        default:
            break;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
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

