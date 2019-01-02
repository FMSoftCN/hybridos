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

#include "activity/window.h"
#include "view/rootview.h"

#include "resource/respkgmanager.h"
#include "css/cssdeclaredgroup.h"
#include "css/cssselector.h"
#include "css/cssbox.h"

namespace hfcl {

RootView::RootView()
    : ViewContainer(NULL, NULL, 0)
    , m_window(0)
{
}

RootView::~RootView()
{
    if (m_window) {
        detachFromSysWindow();
    }
}

bool RootView::attachToSysWindow(Window* window)
{
    if (m_window) {
        detachFromSysWindow();
    }
    else if (window && m_window != window) {
        m_window = window;

        IntRect rc;
        window->getClientRect(rc);

        /* FIXME: viewport should be defined in px */
        m_viewport = rc;
        computeCss();
        calcLayout(m_viewport);
        return true;
    }

    return false;
}

bool RootView::detachFromSysWindow()
{
    if (m_window == NULL)
        return false;

    m_window = NULL;
    return true;
}

bool RootView::applyCssGroup(HTResId cssgId)
{
    CssDeclaredGroup* cssdg = GetCssGroupRes(cssgId);
    if (cssdg == NULL) {
        return false;
    }

    CssDeclaredVec::iterator it;
    for (it = cssdg->m_css_vec.begin(); it != cssdg->m_css_vec.end(); ++it) {
        CssDeclared* css = *it;
        CssSelectorGroup selector;
        selector.compile(css->getSelector());
        applyCss(css, selector);
    }

    return true;
}

void RootView::calcLayout(const RealRect& ctnBlock)
{
    // determine width and margins
    ViewContainer::calcLayout(ctnBlock);
}

} // namespace hfcl

