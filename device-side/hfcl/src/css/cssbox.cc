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

namespace hfcl {

CssBox::CssBox() :
    m_l(0), m_t(0), m_r(0), m_b(0),
    m_ml(0), m_mt(0), m_mr(0), m_mb(0),
    m_pl(0), m_pt(0), m_pr(0), m_pb(0),
    m_blw(0), m_btw(0), m_brw(0), m_bbw(0)
{
}

CssBoxLineBoxContainer::~CssBoxLineBoxContainer()
{
    CssBoxLineVec::iterator it;
    for (it = m_lines.begin(); it != m_lines.end(); ++it) {
        CssBoxLine* box = *it;
        HFCL_DELETE(box);
    }
}

} // namespace hfcl

