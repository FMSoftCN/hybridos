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


#include "selectediconitemview.h"

namespace hfcl {

DEFINE_CLASS_NAME(SelectedIconItemView)

SelectedIconItemView::~SelectedIconItemView()
{
	if(m_normalImg) {
		HFCL_DELETE(m_normalImg);
	}
	if(m_selImg) {
		HFCL_DELETE(m_selImg);
	}
}

void SelectedIconItemView::setNormalImage(Image *img) 
{
	if(m_normalImg && m_normalImg != img) {
		HFCL_DELETE(m_normalImg);
		m_normalImg = NULL;
	}
	m_normalImg = img;
}

void SelectedIconItemView::setSelectedImage(Image* img) 
{
	if(m_selImg && m_selImg != img) {
		HFCL_DELETE(m_selImg);
		m_selImg = NULL;
	}
	m_selImg = img; 
}

void SelectedIconItemView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
	ImageFormat format;	
	format.align = ALIGN_CENTER;
	format.valign = VALIGN_MIDDLE;
	format.drawMode = DRAWSTATE_NORMAL;

	if(isSelected()) {
		if(m_selImg != NULL)
			m_selImg->paint(context, rc, format);
	}
	else {
		if(m_normalImg != NULL)
			m_normalImg->paint(context, rc, format);
	}

}


} // namespace hfcl


