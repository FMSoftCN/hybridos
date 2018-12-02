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


#include "layoutview.h"


namespace hfcl {

/**
 * @brief LayoutView container
 *
 * @param margin  Around the blank.
 * @param space  Two View of between to distance.
 * @param direction  The Layout of direction and size type of scalar.
 *
 * @return 
 */
LayoutView::LayoutView(int margin, int space, int direction)
    : m_margin(margin)
    , m_space(space)
    , m_direction(direction)
{

}

LayoutView::LayoutView(View *p_parent)
    : ContainerView(p_parent)
    , m_margin(0)
    , m_space(0)
    , m_direction(LVDST_HORZ | LVSAT_AUTO)
{

}

LayoutView::LayoutView(View* p_parent, DrawableSet* drset)
    : ContainerView(p_parent, drset)
    , m_margin(0)
    , m_space(0)
    , m_direction(LVDST_HORZ | LVSAT_AUTO)
{

}

LayoutView::LayoutView()
    : m_margin(0)
    , m_space(0)
    , m_direction(LVDST_HORZ | LVSAT_AUTO)
{

}

/**
 * @brief LayoutView destructor
 *
 * @return 
 */
LayoutView::~LayoutView()
{

}

} // namespace hfcl {

