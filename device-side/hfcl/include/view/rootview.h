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

#ifndef _HFCL_VIEW_ROOTVIEW_H_
#define _HFCL_VIEW_ROOTVIEW_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "../common/realrect.h"
#include "../view/viewcontainer.h"
#include "../css/cssdeclared.h"
#include "../graphics/graphicscontext.h"
#include "../resource/restypes.h"

namespace hfcl {

class RootView : public ViewContainer
{
public:
    RootView();
    virtual ~RootView();

    /* methods specific to RootView */
    bool attachToSysWindow(HWND hwnd);
    bool detachFromSysWindow();
    bool applyCssGroup(HTResId cssgId);
    const RealRect& viewport() const { return m_viewport; }

    /* overloaded virtual functions */
    virtual const char* type() const { return "hvroot"; }
    virtual bool isRoot() const { return true; }
    virtual HWND getSysWindow() const { return m_hwnd; }

    virtual void onPaint(GraphicsContext* context);

    /* new virtual functions for RootView */
    // you can overload the method to define customized keycode.
    virtual int scancode2keycode(int scancode);

    /* static methods */
    static RootView* hwndToView(HWND hwnd);
    static LRESULT rootViewProc(HWND hWnd, UINT message,
            WPARAM wParam, LPARAM lParam);

protected:
    /* helpers to handle event */
    int onKeyMessage(Event::EventType keytype,
        WPARAM wParam, LPARAM lParam);
    int onMouseMessage(Event::EventType mouseType,
        WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd;
    RealRect m_viewport;
    WNDPROC m_old_proc;
};

} // namespace hfcl

#endif /* HFCL_VIEW_ROOTVIEW_H_ */
