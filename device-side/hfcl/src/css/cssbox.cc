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

#include "css/cssbox.h"

#include <typeinfo>

#include "css/csscomputed.h"

namespace hfcl {

CssBox::CssBox(CssComputed* css, bool anonymous) :
    m_x(0), m_y(0),
    m_cw(0), m_ch(0),
    m_l(0), m_t(0), m_r(0), m_b(0),
    m_ml(0), m_mt(0), m_mr(0), m_mb(0),
    m_pl(0), m_pt(0), m_pr(0), m_pb(0),
    m_blw(0), m_btw(0), m_brw(0), m_bbw(0)
{
    if (anonymous) {
        m_css = new CssComputed();
        if (css)
            m_css->inherit(css);
    }
    else if (m_css) {
        m_css->ref();
    }
}

CssBox::~CssBox()
{
    if (m_css) {
        m_css->unref();
    }
}

// For atomic inline-level box and general block box
void CssBox::calcBox(const View* view, CssBox* ctnBlock)
{
    m_css->calcWidthsMargins(view, ctnBlock, this);
    m_css->calcHeightsMargins(view, ctnBlock, this);
}

// For splitted inline boxes
void CssSubInlineBox::calcBox(const View* view, CssBox* ctnBlock)
{
    m_cw = m_view->getContentWidth(m_split_ctxt);
    m_ch = m_view->getContentHeight();
}

CssLineBox::~CssLineBox()
{
}

bool CssLineBox::tryToAddBox(const CssBox* box)
{
    return true;
}

CssInlineBox::CssInlineBox(CssComputed* css, bool anonymous)
    : CssBox(css, anonymous)
{
}

CssInlineBox::~CssInlineBox()
{
    std::vector<CssLineBox*>::iterator it;
    for (it = m_lines.begin(); it != m_lines.end(); ++it) {
        CssLineBox* splitted = *it;
        delete splitted;
    }
}

// For inline-level box
void CssInlineBox::calcBox(const View* view, CssBox* ctnBlock)
{
    CssInlineBoxContainer* ibc;
    if (typeid(ctnBlock) == typeid(ibc)) {
        ibc = static_cast<CssInlineBoxContainer*>(ctnBlock);
    }
    else {
    }
}

CssInlineBoxContainer::~CssInlineBoxContainer()
{
    std::vector<CssLineBox*>::iterator it;
    for (it = m_lines.begin(); it != m_lines.end(); ++it) {
        CssLineBox* line = *it;
        delete line;
    }
}

// For inline-level box container
void CssInlineBoxContainer::calcBox(const View* view, CssBox* ctnBlock)
{
}

CssBlockBoxContainer::~CssBlockBoxContainer()
{
}

// For block-level box container
void CssBlockBoxContainer::calcBox(const View* view, CssBox* ctnBlock)
{
}

} // namespace hfcl

