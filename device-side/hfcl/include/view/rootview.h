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
#include "../css/cssbox.h"
#include "../graphics/graphicscontext.h"
#include "../resource/restypes.h"

namespace hfcl {

class Window;

class RootView : public ViewContainer {
public:
    RootView();
    virtual ~RootView();

    /* methods specific to RootView */
    bool attachToSysWindow(Window* window);
    bool detachFromSysWindow();
    bool applyCssGroup(HTResId cssgId);
    const RealRect& viewport() const { return m_viewport; }

    /* overloaded public virtual functions */
    virtual const char* type() const { return "hvroot"; }
    virtual bool isRoot() const { return true; }
    virtual Window* getSysWindow() const { return m_window; }

protected:

private:
    Window* m_window;
    RealRect m_viewport;
};

} // namespace hfcl

#endif /* HFCL_VIEW_ROOTVIEW_H_ */
