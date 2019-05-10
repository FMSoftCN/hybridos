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

#ifndef __HFCL_REALRECT_H__
#define __HFCL_REALRECT_H__

#include "../common/common.h"
#include "../common/intrect.h"

namespace hfcl {

class RealRect {
public:
    RealRect()
        : m_left (0)
        , m_top (0)
        , m_right (0)
        , m_bottom (0) {
    }

    RealRect(HTReal left, HTReal top = 0, HTReal right = 0, HTReal bottom = 0)
        : m_left (left)
        , m_top (top)
        , m_right (right)
        , m_bottom (bottom) {
    }

    RealRect(const RealRect& rc)
        : m_left (rc.m_left)
        , m_top (rc.m_top)
        , m_right (rc.m_right)
        , m_bottom (rc.m_bottom)
    {
    }

    RealRect(const IntRect& rc)
        : m_left (rc.m_left)
        , m_top (rc.m_top)
        , m_right (rc.m_right)
        , m_bottom (rc.m_bottom) {
    }

    RealRect(const RECT& rc)
        : m_left (rc.left)
        , m_top (rc.top)
        , m_right (rc.right)
        , m_bottom (rc.bottom) {
    }

    HTReal left()     const { return m_left;   }
    HTReal top()      const { return m_top;    }
    HTReal right()    const { return m_right;  }
    HTReal bottom()   const { return m_bottom; }

    HTReal width()    const { return m_right - m_left; }
    HTReal height()   const { return m_bottom - m_top; }

    HTReal minWH() const {
        HTReal w = width();
        HTReal h = height();
        if (w > h)
            return h;
        return w;
    }

    HTReal maxWH() const {
        HTReal w = width();
        HTReal h = height();
        if (w > h)
            return w;
        return h;
    }

public:
    HTReal m_left;
    HTReal m_top;
    HTReal m_right;
    HTReal m_bottom;
};

} // namespace hfcl

#endif /* __HFCL_REALRECT_H__ */

