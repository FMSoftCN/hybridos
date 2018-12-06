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


#ifndef _HFCL_SimpleItemView_h
#define _HFCL_SimpleItemView_h

#include "itemview.h"
#include "../graphics/image.h"

namespace hfcl {

class SimpleItemView : public ItemView
{
public:
	SimpleItemView();
	SimpleItemView(View *p_parent);
    SimpleItemView(View* p_parent, DrawableSet* drset);
	virtual ~SimpleItemView();

	virtual void setText(int strid);
	virtual void setText(const char* str);
	virtual void setText(const string& str);
	char * getText();

	void setImage(Image *img);
	Image* getImage()         { return  m_img; }
	int  getImageWidth()      { return m_imgWidth; }
	int  getImageHeight()     { return m_imgHeight; }
	void setImageWidth(int w) { m_imgWidth = w; }
	void setImageHeight(int h){ m_imgHeight = h; }

	void setCheck(int check) { setSelected (check == CHECKED);  }
	int  getCheck()          { return isSelected() ? CHECKED : UNCHECKED; }
	
	void setAlign(int align) { m_align = align; }
	int  getAlign()          { return m_align; }

	bool hasImage()          { return m_hasImage; }
	void enableImage(bool b) { m_hasImage = b; }
	bool hasText()           { return m_hasText; }
	void enableText(bool b)  { m_hasText = b; }
	void setImagePos(int pos){ m_imagePos = pos; }
	int  imagePos()  		 { return m_imagePos; }

	void setGap(unsigned int gap) 	{ m_gap = gap; }
	unsigned int gap(void) { return m_gap; }
	void setTextRightGap(unsigned int gap) 	{ m_textRightGap = gap; }
	unsigned int textRightGap(void) { return m_textRightGap; }
	
	void setRollProp(bool roll) { stopRoll(); m_bRoll = roll; }
	bool getRollProp() { return m_bRoll; }

	void startRoll();
	void stopRoll();
	void resetRoll();
	
	bool isNeedRoll();
	void setRolling(bool b) { m_bRolling = b; }
	bool isRolling() { return m_bRolling; }

	virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

protected:
	virtual void calcRect(const IntRect& rc, IntRect& txtRc, IntRect& imgRc);

	DECLARE_CLASS_NAME(SimpleItemView)

protected:
	int   m_stringId;
	string  m_simpleString;
	Image*  m_img; //the icon of Item
	unsigned int  m_align:2;
	unsigned int  m_hasImage:1;
	unsigned int  m_hasText:1;
	unsigned int  m_unsed:1;
	unsigned int  m_imgWidth:12; //the width of image
	unsigned int  m_imgHeight:12;
	unsigned int  m_imagePos;  // 0 is left , 1 is right , other is error
	unsigned int  m_gap;
	unsigned int  m_textRightGap;
	bool  m_bRoll;	
	bool  m_bRolling;
};

} // namespace hfcl
#endif

