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

namespace hfcl {

bool CssStackingContext::append(CssBox* cssbox, int zindex)
{
    CssBoxWithZIndex tmp = {cssbox, zindex};
    m_boxes.push_back(tmp);
    return true;
}

static bool _sort_by_zindex (const CssBoxWithZIndex &v1,
        const CssBoxWithZIndex &v2)
{
    return v1.zindex < v2.zindex;
}

void CssStackingContext::sort()
{
    std::sort(m_boxes.begin(), m_boxes.end(), _sort_by_zindex);
}

} // namespace hfcl

