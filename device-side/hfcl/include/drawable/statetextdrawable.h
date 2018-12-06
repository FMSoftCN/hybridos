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

#ifndef _HFCL_StateTextDrawable_H
#define _HFCL_StateTextDrawable_H

#include "../common/stlalternative.h"
#include "drawable.h"

namespace hfcl {

class StateTextDrawable : public SimpleDrawable
{
public:
    StateTextDrawable() { }
    StateTextDrawable(const TRStyleElement* style_res) : SimpleDrawable(style_res) { }
    StateTextDrawable(Style* s) : SimpleDrawable(s) { }
    StateTextDrawable(StateTextDrawable* s) : SimpleDrawable((Style*)s) { }

    CopyOnWriteable* clone() {
        return HFCL_NEW_EX(StateTextDrawable, (this));
    }

    void draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE);

    bool calcDrawableSize(int draw_state, int& w, int& h, DWORD data = 0, DR_DATA_TYPE type = DRDT_NONE);
};


DECLARE_DRFACTORY(StateTextDrawable)


class PushableStateTextDrawable : public StateTextDrawable
{
public:
    PushableStateTextDrawable() {}
    PushableStateTextDrawable(const TRStyleElement* elements) : StateTextDrawable(elements) { }
    PushableStateTextDrawable(Style* s) : StateTextDrawable(s) { }
    PushableStateTextDrawable(PushableStateTextDrawable* s) :StateTextDrawable((StateTextDrawable*)s) { }

    CopyOnWriteable* clone() {
        return HFCL_NEW_EX(PushableStateTextDrawable, (this));
    }

    void draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE)
    {
        IntRect rcoff = rc;
        if(draw_state == DRAWSTATE_PUSHED)
            rcoff.offset(0, 2);
        StateTextDrawable::draw(gc, draw_state, rcoff, data, type);
    }

    bool calcDrawableSize(int draw_state, int& w, int& h, DWORD data = 0, DR_DATA_TYPE type = DRDT_NONE)
    {
        bool bret = StateTextDrawable::calcDrawableSize(draw_state, w, h, data, type);
        if(bret)
            h += 2;
        return bret;
    }

};


DECLARE_DRFACTORY(PushableStateTextDrawable)

} // namespace hfcl

#endif


