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


#include "textview.h"
#include "textmode.h"
#include "containerview.h"
#include "transition.h"
#include "codeconvert.h"

#include "hal_misc.h"

NGUX_BEGIN_EXTERN_C

extern U8 isTransformArabic ( U16 inChar );

NGUX_END_EXTERN_C
NAMESPACE_BEGIN

//define for roll text
#define ROLLOFFSET 5

DEFINE_CLASS_NAME(TextView)

TextView::TextView (View *p_parent)
    : View(p_parent, DEFAULT_VIEW_DRAWABLESET(TextView))
	, m_isLongText(false)
	, m_stringId(-1)
	, m_lineAboveH (0)
	, m_lineBellowH (0)
	, m_margin(0)
	, m_bidicheck (false)
{
	m_textBuf[0] = '\0';
}

TextView::TextView(View* p_parent, DrawableSet* drset)
    : View(p_parent, drset)
	, m_isLongText(false)
	, m_stringId(-1)
	, m_lineAboveH (0)
	, m_lineBellowH (0)
	, m_margin(0)
	, m_bidicheck (false)
{
	m_textBuf[0] = '\0';
}

TextView::~TextView()
{
    stopRoll();
}

NGBool TextView::setLongText(void)
{
	// already is
	if(m_textString.length() > 0 || m_isLongText)
		return false;

	// short text for now, need copy text?
	if(m_textBuf[0] != '\0')
		m_textString = m_textBuf;

	m_textBuf[0] = '\0';
	m_isLongText = true;

	return true;
}

NGBool TextView::isLongText(void)
{
	return m_isLongText;
}

void TextView::setBidiCheck(NGBool bidiflag)
{
	m_bidicheck = true;
}

NGBool TextView::isBidiCheck(void)
{
	return m_bidicheck;
}

void TextView::setText(const string text)
{
	setText(text.c_str());
}

void TextView::setText(const char* text)
{
	NGInt len = 0;
	m_stringId = -1;

    /* VincentWei > OPT: do not update the content if... */
	if (NULL == text) text = "";
    if (!m_isLongText && strcmp (m_textBuf, text) == 0) {
        return;
    }

    len = strlen((char *)text);
    if(len > TEXT_BUFFER_LEN_OF_DEFAULT || m_isLongText) {
        m_textString = text;
        m_isLongText = true;	
    }
    else {
        strcpy (m_textBuf, text);
        m_isLongText = false;
    }

    resetRoll();
    autoFitSize();
    updateView();
}

void TextView::setText(NGInt strId)
{
    /* VincentWei > OPT: Only update the content if... */
    if (!m_isLongText && m_stringId == strId) {
        return;
    }

    m_stringId = strId;
    m_isLongText = false;
    updateView();
}

void TextView::setTextColor(Color *c)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, c->color()))
        updateView();
}

void TextView::setTextColor(unsigned char r, unsigned char g, unsigned char b)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, MakeRGB(r,g,b)))
        updateView();
}

void TextView::setTextColor(DWORD color)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_COLOR, color))
        updateView();
}

char* TextView::getText(void)
{
	if(m_stringId > 0) {
        return (char *)GetTextRes(m_stringId);
    } else if(m_isLongText){
		return (char *)(m_textString.c_str());
    } else {
	    return m_textBuf;
    }
}

NGInt TextView::getTextLength(void)
{
	if(m_stringId > 0) {
		return strlen(GetTextRes(m_stringId));
    } else if(m_isLongText) {
		return m_textString.length();
    } else {
		return strlen(m_textBuf);
    }
}

void TextView::drawContent(GraphicsContext* context, IntRect &rc, NGInt status)
{
	char* buf = NULL;

	if(m_stringId > 0)
        buf = (char *)GetTextRes(m_stringId);
    else if(m_isLongText)
		buf = (char *)(m_textString.c_str());
	else
		buf = m_textBuf;

    {
        char tmpchar[8]={0};
        NGInt len = strlen(buf);
        char *str = buf;
        NGInt *info = (NGInt *)malloc ((len + 1) * sizeof(NGInt));
        NGInt realCount = GetUTF8CharInfo (buf, len, info);	
        info [realCount] = len; 
        for (NGInt i = 1; i <= realCount; /*i++*/)
        {
            memcpy(tmpchar, str + info[i-1], info[i] - info[i-1]);
            tmpchar [info[i] - info[i-1]] = '\0';
            if(isSymbolChar(utf8_to_ucs2((U8 *)tmpchar)) ||isNumberChar(utf8_to_ucs2((U8 *)tmpchar)))
            {
                i++;
            }
            else
            {  
                break;
            }			
        }
        free(info);
        info=NULL;
        if(isTransformArabic(utf8_to_ucs2((U8 *)tmpchar)) ||isArabicSymbol(utf8_to_ucs2((U8 *)tmpchar)))
        {	   	
            context->setBiDiFirstChType (BIDI_CHAR_TYPE_RTL);
        }
        else
        {
            context->setBiDiFirstChType (BIDI_CHAR_TYPE_LTR);
        }
    }

    context->setTextAboveLineExtra(m_lineAboveH);
    context->setTextBellowLineExtra(m_lineBellowH);

    if(isDisabled())
        status = DRAWSTATE_DISABLED;

    rc.inset(m_margin, 0);
	if (getRollProp() && isRolling()) {
        if(RollTextTransition::DrawRollText(this, context, m_drset,
                DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT) == false) {
            m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTMODE, TextMode::TextOutOmitted|TextMode::SingleLine);
            m_drset->draw(context, DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT);
        }
	}
    else {
        m_drset->draw(context, DR_CONTENT, status, rc, (DWORD)buf, DRDT_TEXT);
	}

    context->setBiDiFirstChType (BIDI_CHAR_TYPE_INIT);
}

void TextView::setTextOutMode(NGUInt mode)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTMODE, mode))
        updateView();
}

void TextView::setTextOutLineMode(NGBool mode)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE, mode))
        updateView();
}

void TextView::setTextAlign(NGUInt align)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN, align))
        updateView();
}

NGUInt TextView::getTextAlign()
{
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN);
}

NGUInt TextView::getTextValign()
{
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTVALIGN);
}

void TextView::setTextValign(NGUInt valign)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTVALIGN, valign))
        updateView();
}

void TextView::setTextBreak(NGInt tBreak)
{
	if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTBREAK, tBreak))
        updateView();
}

NGInt TextView::getTextBreak(void)
{
    return m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTBREAK);
}

void TextView::setTextFont(NGUInt font)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_FONT, (NGDword)GetFontRes(font)))
        updateView();
}    

void TextView::autoFitSize(NGBool auto_child_fit)
{
	char* buf = NULL;
    if (!isAutoSize())
        return ;

    NGInt w = m_rect.width();
    NGInt h = m_rect.height();

	if(m_stringId > 0)
        buf = (char *)GetTextRes(m_stringId);
    else if(m_isLongText)
		buf = (char *)(m_textString.c_str());
	else
		buf = m_textBuf;

    GraphicsContext::screenGraphics()->setTextAboveLineExtra(m_lineAboveH);
    GraphicsContext::screenGraphics()->setTextBellowLineExtra(m_lineBellowH);

	if (m_drset->calcDrawableSize(DR_CONTENT, DRAWSTATE_NORMAL, w, h, (DWORD)buf, DRDT_TEXT)) {
	    m_rect.setSize(w + (m_margin*2), h);
	    if (parent())
	        parent()->onChildSizeChanged(this);
	}
}

void TextView::startRoll()
{
    stopRoll();
    if (getRollProp() && isNeedRoll()) {
        AddRollText(this, NGUX_NEW_EX(RollTextTransition, (this, ROLLOFFSET)));
        setRolling(true);
    }
}

void TextView::stopRoll()
{
    if (getRollProp()) {
        RemoveRollText(this);
        setRolling(false);
    }
}

void TextView::resetRoll()
{
    if (getRollProp()) {
        ResetRollText(this);
    }
}

NGBool TextView::isNeedRoll()
{
	char* buf = NULL;
    IntRect rc(0, 0, m_rect.width(), m_rect.height());

	if(m_stringId > 0)
        buf = (char *)GetTextRes(m_stringId);
    else if(m_isLongText)
		buf = (char *)(m_textString.c_str());
	else
		buf = m_textBuf;
	
	return RollTextTransition::NeedRollText(
		m_drset,
		DR_CONTENT,
		DRAWSTATE_NORMAL,
		rc,
		(DWORD)buf,
		DRDT_TEXT);
}

NAMESPACE_END

