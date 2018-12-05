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

#include "drawable/stateimagedrawable.h"

#include "graphics/graphicscontext.h"
#include "graphics/image.h"

namespace hfcl {

void StateImageDrawable::draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data/* = 0*/, DR_DATA_TYPE type/* = DRDT_NONE*/)
{
	Image *img;
	if(type == DRDT_IMAGE && data != 0)
		img = (Image*)data;
	else
	{
		img = (Image*)getElement(SYS_GET_STATE_IMAGE_ID(draw_state));	
	}

	if(!img)
		return ;

	ImageFormat format;
	format.drawMode = getElement(SYS_SE_IMAGEDRAWMODE);
	format.align = getElement(SYS_SE_IMAGEALIGN);
	format.valign = getElement(SYS_SE_IMAGEVALIGN);
	img->paint(gc, rc, format);
}

} // namespace hfcl


