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



#include "iconitemview.h"
#include "resource/respkgmanager.h"

namespace hfcl {

IconItemView:: ~IconItemView() 
{ 
	if(m_image) {
		HFCL_DELETE(m_image); 
	}
}

void IconItemView::setText(const char* str) 
{
	int len = 0;

	if(NULL == str){
        m_text[len] = '\0';
        return;
    }

    	m_textId = -1;
	len = strlen((char *)str);
	if(len > TEXT_BUFFER_LEN_OF_DEFAULT)
	{
	    m_buttonString = str;
	    m_isLong = true;	
	}
	// short text
	else 
	{
		memcpy(m_text, str, len);
		m_text[len] = '\0';
	}
}

void IconItemView::setImage(Image* img) 
{
	if(m_image && img != m_image) {
		HFCL_DELETE(m_image);
		m_image = NULL;
	}
	m_image = img;

	updateView();
}

void IconItemView::setText(const string& str) 
{
	setText(str.c_str());
}

void IconItemView::setText(const int stringId) 
{
    m_text[0] = '\0'; 
    m_textId = stringId;	
}

const char * IconItemView::getText() 
{
	if(m_textId > 0) {
        return (char *)GetText(m_textId);
    } else if(m_isLong){
		return (char *)(m_buttonString.c_str());
    } else {
	    return m_text;
    }
}

const int IconItemView::getTextId() 
{
	return m_textId; 
}

void IconItemView::drawContent(GraphicsContext* context, IntRect& rc, int status)
{
	IntRect rctmp = rc;

	if(!m_imageWithoutText)
		rctmp.m_bottom -= m_imgHeight;

	//draw image
	if(m_image != NULL)
	{
		ImageFormat format;
		format.align = ALIGN_CENTER;
		format.valign = VALIGN_MIDDLE;
		format.drawMode = DRAWSTATE_NORMAL;

		m_image->paint(context, rctmp, format);
	}

	if(!m_imageWithoutText)
	{
		const char *txt = getText();
		rctmp.m_top = rctmp.bottom();
		rctmp.m_bottom = rc.bottom();
		m_drset->draw(context, DR_CONTENT, status, rctmp, (DWORD)txt, DRDT_TEXT);
	}
}


DEFINE_CLASS_NAME(IconItemView)

} // namespace hfcl


