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


#ifndef _HFCL_IconItemView_h
#define _HFCL_IconItemView_h

#include "itemview.h"
#include "graphics/image.h"

namespace hfcl {

class IconItemView : public ItemView
{
public:
	IconItemView() 
        : m_textId(-1)
        ,m_isLong(false)
        , m_imgHeight(20)
        , m_imageWithoutText(false)
        , m_image(NULL) 
    { 	
        m_text[0] = '\0';
    }
	IconItemView(View *p_parent)
		:ItemView(p_parent, DEFAULT_VIEW_DRAWABLESET(IconItemView))
        , m_textId(-1)
        ,m_isLong(false)
        , m_imgHeight(20)
        , m_imageWithoutText(false)
        , m_image(NULL) 
    { 
    }
	IconItemView(View *p_parent, DrawableSet* drset)
		:ItemView(p_parent, drset)
        , m_textId(-1)
        ,m_isLong(false)
        , m_imgHeight(20)
        , m_imageWithoutText(false)
        , m_image(NULL) 
    {	
        m_text[0] = '\0'; 
    }

	virtual ~IconItemView();

	void setText(const char* str);
	void setText(const string& str);
	void setText(const int stringId);
	const char * getText();
	const int getTextId();

	void setImage(Image* img);
	Image* getImage() { return m_image; }

	void setImageWithoutText(bool imageWithoutText) { m_imageWithoutText = imageWithoutText; }

	void setImageHeight(int height) { m_imgHeight = height; }
	int  getImageHeight() { return m_imgHeight; }

	void drawContent(GraphicsContext* context, IntRect& rc, int status);

protected:
	char m_text[TEXT_BUFFER_LEN_OF_DEFAULT+1];
	int  m_textId;
	string m_buttonString;
	bool m_isLong;
	int  m_imgHeight;
	bool m_imageWithoutText;
	Image* m_image;

	DECLARE_CLASS_NAME(IconItemView)
};


} // namespace hfcl

#endif



