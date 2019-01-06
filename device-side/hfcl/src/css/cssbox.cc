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

#include "css/csscomputed.h"

namespace hfcl {

CssBox::CssBox(CssComputed* css, bool anonymous) :
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

CssLineBox::~CssLineBox()
{
    CssInlineBoxVec::iterator it;
    for (it = m_inlines.begin(); it != m_inlines.end(); ++it) {
        CssInlineBox* inline_box = *it;
        HFCL_DELETE(inline_box);
    }
}

CssLineBoxContainer::~CssLineBoxContainer()
{
    CssLineBoxVec::iterator it;
    for (it = m_lines.begin(); it != m_lines.end(); ++it) {
        CssLineBox* line = *it;
        HFCL_DELETE(line);
    }
}

CssBlockBoxContainer::~CssBlockBoxContainer()
{
    CssBlockBoxVec::iterator it;
    for (it = m_blocks.begin(); it != m_blocks.end(); ++it) {
        CssBox* box = *it;
        HFCL_DELETE(box);
    }
}

} // namespace hfcl

