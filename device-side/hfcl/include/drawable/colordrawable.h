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

#ifndef _HFCL_ColorDrawable_h
#define _HFCL_ColorDrawable_h

#include "../common/stlalternative.h"
#include "drawable.h"

namespace hfcl {

class ColorDrawable : public Drawable
{
public:
    ColorDrawable() { init(); }
    ColorDrawable(const TRStyleElement* style_res) : Drawable(style_res) { init(style_res); }
    ColorDrawable(Style* s) : Drawable(s) { init(); }
    ColorDrawable(ColorDrawable* dr);

    CopyOnWriteable* clone() {
        return HFCL_NEW_EX(ColorDrawable, (this));
    }

    void draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE);
    bool setElement(int e_id, HTData value);
    HTData getElement(int e_id) const;

protected:
    DWORD m_color;
    void init(const TRStyleElement *elements = NULL);
};

DECLARE_DRFACTORY(ColorDrawable)

} // namespace hfcl

#endif

