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


#ifndef _HFCL_ButtonView_h
#define _HFCL_ButtonView_h

#include "view/view.h"

namespace hfcl {

class ButtonView : public View {
public:
    ButtonView(View* parent);
    ButtonView(View* parent, DrawableSet* drset);
    ButtonView(int id = 0, int x = 0, int y = 0, int w = 40, int h = 20);
    virtual ~ButtonView();

    virtual bool dispatchEvent (Event* event);

    virtual void onGetFocus();
    virtual void onLoseFocus();

	void setText(int strId) { m_stringId=strId; }
	void setText(const char* str);
	void setText(const string& str) { setText(str.c_str()); }
	const char* getText();

protected:
	
	enum {
		NORMAL = 0,
		PUSHED,
		HILIGHT,
	};

	enum {
		STATE = (0x7 << View::FLAG_SHIFT),
		STATE_SHIFT = View::FLAG_SHIFT,
		FLAG_SHIFT = (3 + View::FLAG_SHIFT),
	};

	void setState(int s) {
		m_flags = (m_flags & ~STATE) | ((s & 0x7)<<STATE_SHIFT);
	}

	int getState() {
		return (m_flags & STATE) >> STATE_SHIFT;
	}

	int getDrawStatus() {
		switch(getState()) {
		case NORMAL:  return isFocus() ? DRAWSTATE_FOCUSED :DRAWSTATE_NORMAL;
		case HILIGHT: return DRAWSTATE_HILIGHT;
		case PUSHED:  return DRAWSTATE_PUSHED;
		}
		return DRAWSTATE_NORMAL;
	}

	void drawBackground(GraphicsContext* context, IntRect&rc, int status /*= Style::NORMAL*/)
	{
		if(m_drset) {
			m_drset->draw(context, DR_BKGND, getDrawStatus(), rc);
		}
	}

	void drawContent(GraphicsContext* context, IntRect&rc, int status /*= Style::NORMAL*/)
	{
		m_drset->draw(context, DR_CONTENT, getDrawStatus(), rc, (DWORD)m_text, DRDT_TEXT);
	}

	char   m_text[TEXT_BUFFER_LEN_OF_DEFAULT+1];

private:
    void keyDown(Event* event);
    void keyUp(Event* event);
    void keyLongPress(Event* event);

    void lButtonDown(Event* event);
    void lButtonUp(Event* event);
    void lButtonClick(Event* event);
	
private:
	bool m_isLong;
	int m_stringId;
	string m_buttonString;
	
	DECLARE_CLASS_NAME(ButtonView)
};

} // namespace hfcl {

#endif /* HFCL_ButtonView_h */
