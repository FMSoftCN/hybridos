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

#include "textmode.h"
#include "textdrawable.h"
#include "graphicscontext.h"
#include "font.h"


namespace hfcl {

TextDrawable::TextDrawable(TextDrawable* dr)
:Drawable(dr)
{
	m_font = dr->m_font;
	m_color = dr->m_color;
	m_format = dr->m_format;
}

void TextDrawable::init(const TRStyleElement* elements)
{
	Style * s = GetCommonStyle();
	m_format = TextMode::SingleLine | TextMode::AlignCenter | TextMode::ValignMiddle;
	m_font = NULL;

	Style::init(elements);
	if(m_font == NULL)
		m_font = (Logfont*)s->getElement(SYS_SE_FONT);
}

bool TextDrawable::setElement(int e_id, HTData value)
{
	switch(e_id) {
	case SYS_SE_FONT:
		m_font = (Logfont *)value;
		break;
	case SYS_SE_COLOR:
		m_color = value;
		break;
	case SYS_SE_TEXTALIGN:
		m_format = TextMode::setAlign(m_format, value);
		break;
	case SYS_SE_TEXTVALIGN:
		m_format = TextMode::setVAlign(m_format, value);
		break;
	case SYS_SE_TEXTBREAK:
		m_format = TextMode::setBreak(m_format, value);
		break;
	case SYS_SE_TEXTOUTMODE:
		m_format = TextMode::setTextOutMode(m_format, value);
        break;
    case SYS_SE_TEXTOUTLINEMODE:
        m_format = TextMode::setTextOutLineMode(m_format, value);
        break;
	default:
		return false;
	}
	return true;
}

HTData TextDrawable::getElement(int e_id) const
{
	switch(e_id) {
	case SYS_SE_FONT:       return (HTData) m_font;
	case SYS_SE_COLOR:      return m_color;
	case SYS_SE_TEXTALIGN:  return TextMode::getAlign(m_format);
	case SYS_SE_TEXTVALIGN: return TextMode::getVAlign(m_format);
	case SYS_SE_TEXTBREAK:  return TextMode::getBreak(m_format);
	case SYS_SE_TEXTOUTMODE:return TextMode::getTextOutMode(m_format);
    case SYS_SE_TEXTOUTLINEMODE:return TextMode::getTextOutLineMode(m_format);
	}

	return (m_super?m_super->getElement(e_id):0);

}

void TextDrawable::draw(GraphicsContext* gc, int draw_id, 
        const IntRect &rc, HTData data /*= 0*/, DR_DATA_TYPE type /*= DRDT_NONE*/)
{
	const char * str = NULL; 
	HTData format;
	if(type == DRDT_TEXT) {
		format = m_format;
		str = (const char *)data;
	}
	else if(type == DRDT_FORMATTEXT) {
		FormatText * ft = (FormatText*)data;
		if(ft) {
			str = ft->text;
			format = ft->textFormat;
		}
	}

	if(!str)
		return;

    gc->drawText(str, rc, m_color, m_font, format);
}

bool TextDrawable::calcDrawableSize(int draw_state, int& w, int& h, DWORD data/* = 0*/, DR_DATA_TYPE type /*= DRDT_NONE*/)
{
	const char * str = NULL; 
	HTData format;
	if(type == DRDT_TEXT)
	{
		format = m_format;
		str = (const char *)data;
	}
	else if(type == DRDT_FORMATTEXT)
	{
		FormatText * ft = (FormatText*)data;
		if(ft) {
			str = ft->text;
			format = ft->textFormat;
		}
	}

	if(!str)
		return false;

	if(format & TextMode::SingleLine) {
		int width = GraphicsContext::screenGraphics()->drawTextToGetLenght(str);
		w = width;
	}
	else {
		IntRect rc(0, 0, w, h);
		GraphicsContext::screenGraphics()->drawTextToCalcRect(str, rc, format, m_font);
		w = rc.width();
		h = rc.height();
	}

	return true;
}

} // namespace hfcl

