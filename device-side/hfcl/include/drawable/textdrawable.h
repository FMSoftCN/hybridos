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

#ifndef _HFCL_TextDrawable_h
#define _HFCL_TextDrawable_h

#include "drawable.h"
#include "graphicscontext.h"

namespace hfcl {

class TextDrawable : public Drawable
{
public:
	TextDrawable() { init(); }
	TextDrawable(const TRStyleElement* elements):Drawable(elements) { init(elements); }
	TextDrawable(Style* s) : Drawable(s) { init(); }
	TextDrawable(TextDrawable* s) ;

	CopyOnWriteable* clone() {
		return HFCL_NEW_EX(TextDrawable, (this));
	}

	void draw(GraphicsContext* gc, int draw_id, const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE); 
	bool setElement(int e_id, HTData value);
	HTData getElement(int e_id) const;
	bool calcDrawableSize(int draw_state, int& w, int& h, DWORD data = 0, DR_DATA_TYPE type = DRDT_NONE);

protected:
	void init(const TRStyleElement *elements = NULL);

	Logfont * m_font;
	HTData  m_color;
	HTData  m_format;
};


DECLARE_DRFACTORY(TextDrawable)


//pushable text drawable
class PushableTextDrawable : public TextDrawable
{
public:
	PushableTextDrawable() {}
	PushableTextDrawable(const TRStyleElement* elements) : TextDrawable(elements) { }
	PushableTextDrawable(Style* s) : TextDrawable(s) { }
	PushableTextDrawable(PushableTextDrawable* s) :TextDrawable((TextDrawable*)s) { }
	
	CopyOnWriteable* clone() {
		return HFCL_NEW_EX(PushableTextDrawable, (this));
	}
	
	
	void draw(GraphicsContext* gc, int draw_state, const IntRect &rc, HTData data = 0, DR_DATA_TYPE type = DRDT_NONE)
	{
		IntRect rcoff = rc;
		if(draw_state == DRAWSTATE_PUSHED)
			rcoff.offset(0, 2);
		TextDrawable::draw(gc, draw_state, rcoff, data, type);
	}

	bool calcDrawableSize(int draw_state, int& w, int& h, DWORD data = 0, DR_DATA_TYPE type = DRDT_NONE)
	{
		bool bret = TextDrawable::calcDrawableSize(draw_state, w, h, data, type);
		if(bret)
			h += 2;
		return bret;
	}

};

DECLARE_DRFACTORY(PushableTextDrawable)

} // namespace hfcl
#endif
