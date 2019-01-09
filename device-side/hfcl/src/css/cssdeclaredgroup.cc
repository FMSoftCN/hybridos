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

#include "css/cssdeclaredgroup.h"

#include <algorithm>

namespace hfcl {

CssDeclaredGroup::~CssDeclaredGroup()
{
    CssDeclaredVec::iterator it;
    for (it = m_css_vec.begin(); it != m_css_vec.end(); ++it) {
        CssDeclared* css = *it;
        _DBG_PRINTF ("~CssDeclaredGroup: %p, %d\n", css, css->getRefCnt());
        css->unref();
    }
    m_css_vec.clear();
}

bool CssDeclaredGroup::append(CssDeclared* css)
{
    css->ref();
    _DBG_PRINTF ("CssDeclaredGroup::append: %p %d\n", css, css->getRefCnt());
    m_css_vec.push_back(css);
    return true;
}

CssDeclaredGroupWithSpecificity::~CssDeclaredGroupWithSpecificity()
{
    std::vector<CssWithSpecificity>::iterator it;
    for (it = m_css_specif_vec.begin(); it != m_css_specif_vec.end(); ++it) {
        CssWithSpecificity& css_specif = *it;
        _DBG_PRINTF ("~CssDeclaredGroupWithSpecificity: %p, %d\n",
                css_specif.css, css_specif.css->getRefCnt());
        css_specif.css->unref();
    }

    m_css_specif_vec.clear();
}

bool CssDeclaredGroupWithSpecificity::append(CssDeclared* css, DWORD specif)
{
    CssWithSpecificity tmp = {css, specif};

    css->ref();
    m_css_specif_vec.push_back(tmp);
    return true;
}

static bool _sort_by_specificity (const CssWithSpecificity &v1,
        const CssWithSpecificity &v2)
{
    return v1.specif < v2.specif;
}

void CssDeclaredGroupWithSpecificity::sort()
{
    std::sort(m_css_specif_vec.begin(), m_css_specif_vec.end(),
            _sort_by_specificity);
}

} // namespace hfcl

