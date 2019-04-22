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
#include "css/csscomputed.h"

namespace hfcl {

RootView::RootView()
    : ViewContainer(NULL, "root", 0)
    , m_window(0)
    , m_root_csc(0)
{
}

RootView::~RootView()
{
    if (m_window) {
        detachFromSysWindow();
    }

    if (m_root_csc) {
        delete m_root_csc;
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

        // FIXME: viewport should be defined in px
        m_viewport = rc;

        computeCss();
        makeCssBox();

        // create the default stacking context
        if (m_root_csc)
            delete m_root_csc;
        int zindex;
        m_css_computed->getZIndex(zindex);
        m_root_csc = new CssStackingContext(this, zindex,
            m_css_computed->getOpacity());
        makeStackingContext(m_root_csc);

        // create initial containing block
        if (m_cssbox_principal)
            delete m_cssbox_principal;
        m_cssbox_principal = new CssBox(m_css_computed);
        m_cssbox_principal->setContentWidth(m_viewport.width());
        m_cssbox_principal->setContentHeight(m_viewport.height());

        layOut(m_cssbox_principal);
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

} // namespace hfcl

