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

#include "nguxcommon.h"
#include "intrect.h"

NAMESPACE_BEGIN

void IntRect::join(int i_left, int i_top, int i_right, int i_bottom)
{
    // do nothing if the params are empty
    if (i_left >= i_right || i_top >= i_bottom)
        return;

    // if we are empty, just assign
    if (m_left >= m_right || m_top >= m_bottom) {
        this->setRect(i_left, i_top, i_right, i_bottom);
    } else {
        if (i_left < m_left) m_left = i_left;
        if (i_top < m_top) m_top = i_top;
        if (i_right > m_right) m_right = i_right;
        if (i_bottom > m_bottom) m_bottom = i_bottom;
    }
}

void IntRect::sort()
{
    int swapVal = 0;

    if (m_left > m_right) {
        swapVal = m_left;
        m_left  = m_right;
        m_right = swapVal;
    }
    if (m_top > m_bottom) {
        swapVal  = m_top;
        m_top    = m_bottom;
        m_bottom = swapVal;
    }
}

NAMESPACE_END

