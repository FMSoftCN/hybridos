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

#include "drawable/statecolordrawable.h"
#include "graphics/graphicscontext.h"

namespace hfcl {

void StateColorDrawable::draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data/* = 0*/, DR_DATA_TYPE type/* = DRDT_NONE*/)
{
    HTData color = getElement(SYS_GET_STATE_COLOR_ID(draw_state));

    gc->fillRect(rc, GetRValue(color), GetGValue(color), GetBValue(color), GetAValue(color));
}

} // namespace hfcl


