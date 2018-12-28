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

#ifndef HFCL_ACTIVITY_WINDOW_H_
#define HFCL_ACTIVITY_WINDOW_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "../common/common.h"
#include "../common/object.h"
#include "../common/intrect.h"
#include "../common/event.h"
#include "../graphics/graphicscontext.h"

namespace hfcl {

class RootView;

class Window : public Object {
public:
    Window();
    virtual ~Window();

    /* public methods */
    void setWindowRect(const IntRect& rc);
    void getClientRect(IntRect& rc) const;

    void show(bool updateBg = true);
    void hide();
    void destroy();
    bool create();

    HWND getSysWindow() const { return m_sysWnd; }
    static Window* getObject(HWND hwnd);

    HWND setActiveWindow(HWND hMainWnd);
    HWND getActiveWindow();

    void updateWindow(bool updateBg = true);
    void asyncUpdateRect(int x, int y, int w, int h, bool updateBg = true);
    void syncUpdateRect(int x, int y, int w, int h, bool updateBg = true);

    unsigned int doModalView();

    int doModal(bool bAutoDestory = true);
    void endDlg(int endCode);

    RootView* getRootView() { return m_rootView; }
    bool setRootView(RootView* root);

    /* event handlers */
    virtual void drawBackground(GraphicsContext* gc, IntRect &rc);

    virtual bool onKeyEvent(const KeyEvent* event);
    virtual bool onMouseEvent(const MouseEvent* event);
    virtual bool onMouseWheelEvent(const MouseWheelEvent* event);
    virtual bool onIdle() { return false; }

    // you can overload the method to define customized keycode.
    virtual KeyEvent::KeyCode scancode2keycode(int scancode);

protected:
    HWND m_sysWnd;
    RootView* m_rootView;

    static LRESULT defaultMainWindowProc(HWND hWnd, UINT message,
            WPARAM wParam, LPARAM lParam);
    static HWND createMainWindow(const char* caption, WNDPROC proc,
            int x, int y, int width, int height, DWORD addData,
            bool visible = true);

    /* helpers to handle event */
    int onKeyMessage(KeyEvent::KeyEventType keytype,
        WPARAM wParam, LPARAM lParam);
    int onMouseMessage(MouseEvent::MouseEventType mouseType,
        WPARAM wParam, LPARAM lParam);

    // overloaded this function to change the default position of main window
    virtual bool doCreate(int x, int y, int w, int h, bool visible = true);

};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_WINDOW_H_ */

