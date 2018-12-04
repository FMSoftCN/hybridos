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


#include "statusbarview.h"

namespace hfcl {


#define SBW_SPACE_SIZE      0//2
#define SBW_WIDTH           16

StatusBarView::StatusBarView(View* p_parent)
    : PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(StatusBarView))
    , m_space(SBW_SPACE_SIZE)
    , m_width(SBW_WIDTH)
    , m_sbvLeft(0)
    , m_sbvRight(_ngux_screen_w)
{
}

StatusBarView::StatusBarView(View* p_parent, DrawableSet* drset)
    : PanelView(p_parent, drset)
    , m_space(SBW_SPACE_SIZE)
    , m_width(SBW_WIDTH)
    , m_sbvLeft(0)
    , m_sbvRight(_ngux_screen_w)
{
}

void StatusBarView::updateStatusBarView(void)
{
    autoRecalc();
    updateView();
}

bool StatusBarView::dispatchEvent(Event* event)
{
    return DISPATCH_CONTINUE_MSG;
}

bool StatusBarView::setSBView(int loca, View* v)
{
    int v_w = 0;
    int v_h = 0;
    int v_id = v->id();
    SBViewNode* node = find(v_id);

    if (node) {
        node->pv = v;
        node->location = loca;
        return true;
    }

    v->getSize(&v_w, &v_h);
    if (v_w > m_width)
        v_w = m_width;

    SBViewNode *sbvn = HFCL_NEW_EX(SBViewNode, (v_id, loca, v, v_w, v_h));
    m_sbwList.push_back(sbvn);

    return true;
}

View* StatusBarView::getSBView(int v_id) const
{
    SBViewNode* node = find(v_id);

    return node ? node->pv : NULL;
}

StatusBarView::SBViewNode* StatusBarView::find(int v_id) const
{
    for (SBVList::const_iterator it = m_sbwList.begin(); it != m_sbwList.end(); ++it) {
		SBViewNode* nd = (*it);
        if (nd && nd->id == v_id) {
            return nd;
        }
    }

    return NULL;
}

void StatusBarView::setAutoRecalc(bool bsar)
{
    if (bsar)
        autoRecalc();
}

// void StatusBarView::RefreshViewState(int id, int level)
// {
// }

DEFINE_CLASS_NAME(StatusBarView)

void StatusBarView::autoRecalc(void)
{
    for (SBVList::iterator it = m_sbwList.begin(); it != m_sbwList.end(); ++it) {
        resetViewRect((SBViewNode*)(*it));
    }

    m_sbvLeft  = 0;
    m_sbvRight = _ngux_screen_w;
}

void StatusBarView::resetViewRect(SBViewNode* sbvn)
{
    if (StatusBarView::SHOW_IN_RIGHT == sbvn->location) {
        m_sbvRight -= m_space;
        if (sbvn->pv->isVisible() && m_sbvLeft < m_sbvRight) {
            sbvn->pv->setRect(m_sbvRight - sbvn->pvWidth, 0, m_sbvRight, _ngux_phonebar_h);
            m_sbvRight -= sbvn->pvWidth;
        } else {
            sbvn->pv->setRect(0, 0, 0, 0);
        }
    } else {
        m_sbvLeft += m_space;
        if (sbvn->pv->isVisible() && m_sbvLeft < m_sbvRight) {
            sbvn->pv->setRect(m_sbvLeft, 0, m_sbvLeft + sbvn->pvWidth, _ngux_phonebar_h);
            m_sbvLeft += sbvn->pvWidth;
        } else {
            sbvn->pv->setRect(0, 0, 0, 0);
        }
    }
}

} // namespace hfcl

