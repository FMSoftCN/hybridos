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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _NGUX_ArrowTextView_h
#define _NGUX_ArrowTextView_h

#include "view.h"
#include "image.h"
#include "textmode.h"

NAMESPACE_BEGIN

class ArrowTextView : public View
{
    public:
        ArrowTextView() : View()
			, m_isLongText(false)
			, m_stringId(-1)
            , m_limg(NULL)
            , m_hasImage(1)
            , m_hasText(1)
            , m_imgWidth(0)
            , m_imgHeight(0)
            , m_textOutLine(0)
        {
        	m_text[0] = '\0';
        }

        ArrowTextView(View *p_parent)
            : View(p_parent, DEFAULT_VIEW_DRAWABLESET(ArrowTextView))
            , m_isLongText(false)
			, m_stringId(-1)
            , m_limg(NULL)            
            , m_hasImage(1)
            , m_hasText(1)
            , m_imgWidth(0)
            , m_imgHeight(0)
            , m_textOutLine(0)
        {
			m_text[0] = '\0';
		}

        ArrowTextView(View *p_parent, DrawableSet* drset)
            : View(p_parent, drset)
            , m_isLongText(false)
			, m_stringId(-1)
            , m_limg(NULL)            
            , m_hasImage(1)
            , m_hasText(1)
            , m_imgWidth(0)
            , m_imgHeight(0) 
            , m_roll(false)
            , m_textOutLine(0)
        {
		m_text[0] = '\0';
	}

	virtual ~ArrowTextView();

	void setText(NGInt strId) {m_stringId=strId; }
	void setText(const char* str); 
	void setText(const string& str) { 	setText(str.c_str()); }
	NGPStr getText();

	void setLeftImage(Image *img);
	Image* getLeftImage() { return  m_limg; }
	void setRightImage(Image *img);
	Image* getRightImage() { return  m_rimg; }

	int  getImageWidth()      { return m_imgWidth; }
	int  getImageHeight()     { return m_imgHeight; }
	void setImageWidth(int w) { m_imgWidth = w; }
	void setImageHeight(int h){ m_imgHeight = h; }

	bool hasImage()          { return m_hasImage; }
	void enableImage(bool b) { m_hasImage = b; }
	bool hasText()           { return m_hasText; }
	void enableText(bool b)  { m_hasText = b; }
	void setTextOutLine(bool b) { m_textOutLine = (b)?TextMode::OutLine:0x0; }

	virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

	void onGetFocus() { if(!m_roll) startRollText(); }
	void onLoseFocus() { if(!m_roll) stopRollText(); }

	void setRollProp(NGBool roll) { m_roll = roll; }

	void startRollText();
	void stopRollText();

	void setTextFont(NGUInt font);//GT_mspark_20130916_for set font
    
    private:
	NGChar  m_text[TEXT_BUFFER_LEN_OF_DEFAULT+1]; //the text 
	string  m_textstring;
	NGBool  m_isLongText;
	NGInt   m_stringId;
	Image*  m_limg; //the left arrow
	Image*  m_rimg; //the right arrow 
	NGUInt  m_hasImage:1;
	NGUInt  m_hasText:1;
	NGUInt  m_imgWidth:30; //the width of image
	NGUInt  m_imgHeight:30;
	NGBool  m_roll;
	NGUInt  m_textOutLine;

	void calcRect(const IntRect& rc, IntRect& txtRc, IntRect& leftArrowRc, IntRect& rightArrowRc);

	DECLARE_CLASS_NAME(ArrowTextView)
};


NAMESPACE_END

#endif /* _NGUX_ArrowTextView_h */
