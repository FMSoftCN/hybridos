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

#include "drawable/colordrawable.h"
#include "graphics/graphicscontext.h"

namespace hfcl {

ColorDrawable::ColorDrawable(ColorDrawable* dr)
:Drawable(dr)
{
	if(dr)
		m_color = dr->m_color;
	else
		init();
}

void ColorDrawable::init(const TRStyleElement* elements)
{
	m_color = 0;
	if(elements)
		Style::init(elements);
	else {
		Style * s = GetCommonStyle();
		m_color = s->getElement(SYS_SE_COLOR);
	}
}

bool ColorDrawable::setElement(int e_id, HTData value)
{
	if (e_id == SYS_SE_COLOR){
		m_color = value;
		return true;
	}
	return false;
}

HTData ColorDrawable::getElement(int e_id) const
{
	if (e_id == SYS_SE_COLOR){
		return m_color;
	}
	
	return (m_super?m_super->getElement(e_id):0);
}

void ColorDrawable::draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data /*= 0*/, DR_DATA_TYPE type /*= DRDT_NONE*/)
{
	gc->fillRect(rc, GetRValue(m_color), GetGValue(m_color), GetBValue(m_color), GetAValue(m_color));
}

} // namespace hfcl

