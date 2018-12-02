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


#include "ngux.h"
#include "simpleitemview.h"
#include "respkgmanager.h"
namespace hfcl {

#define SPACE 2         // 3
#define SELECTED_WIDTH 20

DEFINE_CLASS_NAME(SimpleItemView)

//define for roll text
#define ROLLOFFSET 5

SimpleItemView::SimpleItemView()
	:ItemView()
	,m_stringId(-1)
	,m_img(NULL)
	,m_align(ALIGN_LEFT)
	,m_hasImage(0)
	,m_hasText(1)
	,m_imgWidth(0)
	,m_imgHeight(0)
	,m_gap(SPACE)
{
	/*construct*/
}

SimpleItemView::SimpleItemView(View *p_parent) 
	:ItemView(p_parent, DEFAULT_VIEW_DRAWABLESET(SimpleItemView))
	,m_stringId(-1)
	,m_img(NULL)		
	,m_align(ALIGN_LEFT)
	,m_hasImage(0)
	,m_hasText(1)
	,m_imgWidth(0)
	,m_imgHeight(0)
	,m_gap(SPACE)
{ 
	/*construct*/
}

SimpleItemView::SimpleItemView(View* p_parent, DrawableSet* drset) 
	:ItemView(p_parent, drset)
	,m_stringId(-1)
	,m_img(NULL)		
	,m_align(ALIGN_LEFT)
	,m_hasImage(0)
	,m_hasText(1)
	,m_imgWidth(0)
	,m_imgHeight(0)
	,m_gap(SPACE)
{ 
	/*construct*/
}

SimpleItemView::~SimpleItemView()
{
	if(m_img) {
		HFCL_DELETE(m_img);
	}
}

void SimpleItemView::setImage(Image *img) {
	if(m_img && img != m_img){
		HFCL_DELETE(m_img);
	}
	m_img = img;
}

void SimpleItemView:: setText(const char* str) 
{
	if(NULL != str){
		m_stringId = -1;
		m_simpleString = str;
    }
}

void SimpleItemView::setText(int strid)
{ 
	m_stringId = strid; 
	m_simpleString = "";
}

void SimpleItemView::setText(const string& str) 
{ 
	setText(str.c_str()); 
}

char * SimpleItemView::getText(void)
{
	return (char *)(m_stringId > 0 ? GetTextRes(m_stringId) : (m_simpleString.c_str()));
}

void SimpleItemView::calcRect(const IntRect& rc, IntRect& txtRc, IntRect& imgRc)
{
	if(!hasImage() && !hasText())
	{
		return;
	}

	int x = rc.left() + gap();

	if (m_drset->getDrawable(DR_CHECK))
		x += selectMarkRect().width();

	if(hasImage())
	{
		if(imagePos() == 1)
		{
			imgRc.m_left    = rc.right() - 3*gap() - getImageWidth();
			imgRc.m_right   = imgRc.m_left + getImageWidth();
			imgRc.m_top     = (rc.top() + rc.bottom() - getImageHeight()) / 2;
			imgRc.m_bottom  = imgRc.top() + getImageHeight();
			x += gap();
		} else {
            imgRc.m_left    = x;
            imgRc.m_right   = x + getImageWidth();
            imgRc.m_top     = (rc.top() + rc.bottom() - getImageHeight()) / 2;
            imgRc.m_bottom  = imgRc.top() + getImageHeight();
            x = imgRc.right() + gap();
        }
		if(IsAllItemViewNeedConvert() && imgRc.width() > 0){
			int l = imgRc.m_left;
			int r = imgRc.m_right;
			imgRc.m_left = rc.left() + (rc.right() - r);
			imgRc.m_right = rc.right() - (l - rc.left());
		}
	}

	int right = rc.right() - gap();

	if(hasText())
	{
		txtRc.m_left 	= x;
		txtRc.m_top 	= rc.top();
		txtRc.m_bottom 	= rc.bottom();
		txtRc.m_right 	= right;
		
		if(IsAllItemViewNeedConvert() && txtRc.width() > 0){
			int l = txtRc.m_left;
			int r = txtRc.m_right;
			txtRc.m_left = rc.left() + (rc.right() - r) + m_textRightGap;
			txtRc.m_right = rc.right() - (l - rc.left());
		}
		else
		{
			txtRc.m_right 	= right - m_textRightGap;
		}
	}
}


void SimpleItemView::drawContent(GraphicsContext* context, IntRect& rc, int status)
{
	IntRect txtRc, imgRc;
	calcRect(rc, txtRc, imgRc);

	if(hasImage() && m_img != NULL)
	{
		ImageFormat format;
		format.drawMode = DRAWMODE_NORMAL;
		format.align  	= ALIGN_CENTER;
		format.valign 	= VALIGN_MIDDLE; 
		m_img->paint(context, imgRc, format);
	}

	if(hasText()) {
		if (getRollProp() && isRolling()) {
	        if(RollTextTransition::DrawRollText(this, context, m_drset,
	                DR_CONTENT, status, txtRc, (DWORD)getText(), DRDT_TEXT) == FALSE)
            {
                m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTMODE, TextMode::TextOutOmitted|TextMode::SingleLine);
                m_drset->draw(context, DR_CONTENT, status, txtRc, (DWORD)getText(), DRDT_TEXT);
            }
		} else {
	        m_drset->draw(context, DR_CONTENT, status, txtRc, (DWORD)getText(), DRDT_TEXT);
		}
	}
}

void SimpleItemView::startRoll()
{
    if (getRollProp() && isNeedRoll()) {
        AddRollText(this, HFCL_NEW_EX(RollTextTransition, (this, ROLLOFFSET)));
        setRolling(true);
    }
}

void SimpleItemView::stopRoll()
{
    if (getRollProp()) {
        RemoveRollText(this);
        setRolling(false);
    }
}

void SimpleItemView::resetRoll()
{
    if (getRollProp()) {
        ResetRollText(this);
    }
}

bool SimpleItemView::isNeedRoll()
{
	IntRect txtRc, imgRc;
    IntRect rc(0, 0, m_rect.width(), m_rect.height());
	
	calcRect(rc, txtRc, imgRc);
	
	return RollTextTransition::NeedRollText(
		m_drset,
		DR_CONTENT,
		DRAWSTATE_NORMAL,
		txtRc,
		(DWORD)getText(),
		DRDT_TEXT);
}



} // namespace hfcl {

