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


#include "arrowtextview.h"
#include "transition.h"
#include "resource/respkgmanager.h"
namespace hfcl {

#define SPACE 2

DEFINE_CLASS_NAME(ArrowTextView)

ArrowTextView:: ~ArrowTextView()
{
	if(m_limg) {
		HFCL_DELETE(m_limg);
	}

	if(m_rimg) {
		HFCL_DELETE(m_rimg);
	}
	
	stopRollText();
}


void ArrowTextView::setText(const char* str)
{
	int len = 0;

	if(NULL == str){
        m_text[len] = '\0';
        return;
	}
	
	m_stringId = -1;
	len = strlen(str);
	
	if(len > TEXT_BUFFER_LEN_OF_DEFAULT ||  m_isLongText)
	{
		m_textstring = str;
	    m_isLongText = true;	
	}
	else
	{
		memcpy(m_text,str, len);
		m_text[len] = '\0';
	}
}

char* ArrowTextView::getText(void)
{
	if(m_stringId > 0) {
        return (char *)GetTextRes(m_stringId);
    } else if(m_isLongText){
		return (char *)(m_textstring.c_str());
    } else {
	    return m_text;
    }	
}

void ArrowTextView::setRightImage(Image *img) 
{ 
	if(m_rimg && img != m_rimg) {
		HFCL_DELETE(m_rimg);
		m_rimg = NULL;
	}
	m_rimg = img;
}

void ArrowTextView::setLeftImage(Image *img) 
{ 
	if(m_limg && img != m_limg) {
		HFCL_DELETE(m_limg);
		m_limg = NULL;
	}
	m_limg = img;
}

void ArrowTextView::calcRect(const IntRect& rc, IntRect& txtRc, IntRect& leftArrowRc, IntRect& rightArrowRc)
{	
    if(!hasImage() && !hasText())
	{
		return;
	}

	int left = rc.left()/* + SPACE */;
	int right = rc.right() /* - SPACE */;

	if(hasImage())
	{
        leftArrowRc.m_left = left;
        leftArrowRc.m_right = left + getImageWidth();
        leftArrowRc.m_top = (rc.top() + rc.bottom() - getImageHeight()) / 2;
        leftArrowRc.m_top += ((rc.top() + rc.bottom() - getImageHeight()) % 2) ? 1 : 0;
        leftArrowRc.m_bottom = leftArrowRc.top() + getImageHeight();

        left = leftArrowRc.right()/* + SPACE */;

        rightArrowRc.m_left = right - getImageWidth();
        rightArrowRc.m_right = right;
        rightArrowRc.m_top = (rc.top() + rc.bottom() - getImageHeight()) / 2;
        rightArrowRc.m_top += ((rc.top() + rc.bottom() - getImageHeight()) % 2) ? 1 : 0;
        rightArrowRc.m_bottom = rightArrowRc.top() + getImageHeight();

        right = rightArrowRc.left()/* + SPACE */;
	}

	if(hasText())
	{
		txtRc.m_left = left;
		// GT_choph 130719  "On" text  in Alarm activation tab align 1 pixel down as per alignment of wheels.
		// this problem is every where wheels are placed for roatating text.
		txtRc.m_top = rc.top() - 1;       
		txtRc.m_bottom = rc.bottom() -1;
		txtRc.m_right = right;
	}
}

void ArrowTextView::drawContent(GraphicsContext* context, IntRect& rc, int status)
{
	IntRect txtRc, limgRc, rimgRc;
	calcRect(rc, txtRc, limgRc, rimgRc);
	int state = status;

	if (hasImage()) {

		if (isFocus())
			state = DRAWSTATE_HILIGHT;
		
		// left image
		m_drset->draw(context, DR_CONTENT2, state, limgRc);

		// right image
		m_drset->draw(context, DR_CONTENT3, state, rimgRc);
	}

    if(hasText())
    {
	    FormatText ft;

	    ft.text = getText();
	    ft.textFormat = TextMode::ValignMiddle | TextMode::SingleLine | m_textOutLine;
        
		if(hasImage())
            ft.textFormat = ft.textFormat | TextMode::AlignCenter;
        else
            ft.textFormat = ft.textFormat | TextMode::AlignLeft;
        
        if(!((m_roll
            || status == DRAWSTATE_HILIGHT)
			&& RollTextTransition::DrawRollText(
				this,
				context,
				m_drset,
				DR_CONTENT,
				status,
				txtRc,
				(DWORD)&ft,
				DRDT_FORMATTEXT))) 
		{
		    ft.textFormat |=  TextMode::TextOutOmitted;
			ft.textFormat |= TextMode::AlignCenter;
		    m_drset->draw(context, DR_CONTENT, status, txtRc, (DWORD)&ft, DRDT_FORMATTEXT);
	    }

    }

}

void ArrowTextView::startRollText()
{
	FormatText ft;
	ft.text = getText();
	ft.textFormat = TextMode::AlignLeft | TextMode::ValignMiddle | TextMode::SingleLine | TextMode::TextOutOmitted;
	
	IntRect rc(0, 0, m_rect.width(), m_rect.height());
	IntRect txtRc, limgRc, rimgRc;
	calcRect(rc, txtRc, limgRc, rimgRc);

	if(RollTextTransition::NeedRollText(m_drset,
				DR_CONTENT,
				DR_HILIGHT,
				txtRc,
				(HTData)&ft,
				DRDT_FORMATTEXT))
	{
		AddRollText(this, HFCL_NEW_EX(RollTextTransition, (this, 2)));
	}
}

void ArrowTextView::stopRollText()
{
	RemoveRollText(this);
}

//GT_mspark_20130916_for set font
void ArrowTextView::setTextFont(unsigned int font)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_FONT, (HTData)GetFontRes(font)))
        updateView();
}    

} // namespace hfcl
