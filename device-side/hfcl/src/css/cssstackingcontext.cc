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

#include "css/cssstackingcontext.h"

#include <algorithm>

namespace hfcl {

CssStackingContext::CssStackingContext(const View* view, int zindex,
            unsigned char opacity)
    : m_view(view)
    , m_zindex(zindex)
    , m_opacity(opacity)
    , m_mem_gc(0)
{
    // TODO: create memgc if opacity is not 0xFF.
}

CssStackingContext::~CssStackingContext()
{
    std::vector<CssStackingContext*>::iterator it;
    for (it = m_children.begin(); it != m_children.end(); ++it) {
        CssStackingContext* child = *it;
        _DBG_PRINTF ("%s: Deleting CssStackingContext object: %p\n", child);
        delete child;
    }

    if (m_mem_gc) {
        _DBG_PRINTF ("%s: Deleting GraphicsContext object: %p\n", m_mem_gc);
        delete m_mem_gc;
    }
}

static bool _sort_by_zindex (const CssStackingContext* v1,
        const CssStackingContext* v2)
{
    return v1->getZIndex() < v2->getZIndex();
}

void CssStackingContext::sort()
{
    std::sort(m_children.begin(), m_children.end(), _sort_by_zindex);
}

} // namespace hfcl

