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
#include "statetextdrawable.h"
#include "graphics/graphicscontext.h"
#include "graphics/font.h"


namespace hfcl {

void StateTextDrawable::draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data/* = 0*/, DR_DATA_TYPE type/* = DRDT_NONE*/)
{
	const char* str = NULL; 
	HTData format;
	if(type == DRDT_TEXT) {
		format = 0;
		format = TextMode::setAlign(format, getElement(SYS_SE_TEXTALIGN));
		format = TextMode::setVAlign(format, getElement(SYS_SE_TEXTVALIGN));
		format = TextMode::setBreak(format, getElement(SYS_SE_TEXTBREAK));
		//format = TextMode::setTextOutMode(format, getElement(SYS_SE_TEXTOUTMODE));
		format = TextMode::setTextOutLineMode(format, getElement(SYS_SE_TEXTOUTLINEMODE));
		format |= getElement(SYS_SE_TEXTOUTMODE);

		str = (const char*)data;
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

	Logfont *font  = (Logfont *)getElement(SYS_GET_STATE_FONT_ID(draw_state));
	if(!font)
		font = (Logfont *)getElement(SYS_SE_FONT);
	HTData color = getElement(SYS_GET_STATE_COLOR_ID(draw_state));
	gc->drawText(str, rc, color, font, format);
}

bool StateTextDrawable::calcDrawableSize(int draw_state, int& w, int& h, DWORD data, DR_DATA_TYPE type)
{
	const char* str = NULL; 
	HTData format;

	if(type == DRDT_TEXT)
	{
		format = 0;
		format = TextMode::setAlign(format, getElement(SYS_SE_TEXTALIGN));
		format = TextMode::setVAlign(format, getElement(SYS_SE_TEXTVALIGN));
		format = TextMode::setBreak(format, getElement(SYS_SE_TEXTBREAK));
		//format = TextMode::setTextOutMode(format, getElement(SYS_SE_TEXTOUTMODE));
		format |= getElement(SYS_SE_TEXTOUTMODE);

		str = (const char*)data;
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


	if(format & TextMode::SingleLine)
	{
		w = GraphicsContext::screenGraphics()->drawTextToGetLenght(str);
	}
	else
	{
		IntRect rc(0, 0, w, h);
		Logfont *font  = (Logfont *)getElement(SYS_GET_STATE_FONT_ID(draw_state));
		if(!font)
		    font = (Logfont *)getElement(SYS_SE_FONT);
	
		h = GraphicsContext::screenGraphics()->drawTextToCalcRect(str, rc, format,font);
	}
	return true;
}

} // namespace hfcl

