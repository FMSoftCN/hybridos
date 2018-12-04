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

#include "drawable/imagedrawable.h"

namespace hfcl {

ImageDrawable::ImageDrawable(ImageDrawable* drimg)
	: Drawable(drimg)
	, m_image(NULL)
{
	if(m_image != NULL) {
		HFCL_DELETE(m_image);
		m_image = NULL;
	}
	if (drimg != NULL) {
		m_image = drimg->m_image;
		m_format = drimg->m_format;
	}
}

ImageDrawable::~ImageDrawable(void)
{
	if (m_image != NULL){		
		HFCL_DELETE(m_image);
	}
}

void ImageDrawable::init(const TRStyleElement* style_res)
{
	Style * s = GetCommonStyle();

	m_format.drawMode = s->getElement(SYS_SE_IMAGEDRAWMODE);
	m_format.align    = s->getElement(SYS_SE_IMAGEALIGN);
	m_format.valign   = s->getElement(SYS_SE_IMAGEVALIGN);

	if(style_res)
	{
		for(int i = 0; style_res[i].id != -1; i++)
		{
			setElement(style_res[i].id, ResValueToElement(style_res[i].id, style_res[i].value));
		}
	}
}

bool ImageDrawable::setElement(int e_id, HTData value)
{
	switch(e_id){
		case SYS_SE_IMAGE:
			m_image = (Image*)(value);
			break;
		case SYS_SE_IMAGEDRAWMODE:
			m_format.drawMode = value;
			break;
		case SYS_SE_IMAGEALIGN:
			m_format.align = value;
			break;
		case SYS_SE_IMAGEVALIGN:
			m_format.valign = value;
			break;
		default:
			return false;
	}
	return true;
}

HTData ImageDrawable::getElement(int e_id) const
{
	switch(e_id){
		case SYS_SE_IMAGE:          return (HTData)m_image;
		case SYS_SE_IMAGEDRAWMODE:  return m_format.drawMode;
		case SYS_SE_IMAGEALIGN:     return m_format.align;
		case SYS_SE_IMAGEVALIGN:    return m_format.valign;
	}
	return (m_super ? m_super->getElement(e_id) : 0);
}

void ImageDrawable::draw(GraphicsContext* gc, int draw_id, const IntRect &rc, HTData data/* = 0*/, DR_DATA_TYPE type/* = DRDT_NONE*/)
{
	Image *img = NULL;
	
	if(type == DRDT_IMAGE && data != 0)
		img = (Image*)data;
	else
		img = m_image;
	if(img != NULL)
	{
		img->paint(gc, rc, m_format);
	}
}

} // namespace hfcl


