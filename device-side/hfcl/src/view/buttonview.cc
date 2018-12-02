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


#include "buttonview.h"
#include "respkgmanager.h"
namespace hfcl {

#define BTN_DEFAULT_WIDTH    40
#define BTN_DEFAULT_HEIGHT   20

ButtonView::ButtonView(View* p_parent)
	: View(p_parent, DEFAULT_VIEW_DRAWABLESET(ButtonView))
	, m_isLong(false)
	, m_stringId(-1)
{
	m_text[0] = '\0';
	setRect(IntRect(0, 0, BTN_DEFAULT_WIDTH, BTN_DEFAULT_HEIGHT));
	setFocusValid(true);
}

ButtonView::ButtonView(View* p_parent, DrawableSet* drset)
 	: View(p_parent, drset)
 	, m_isLong(false)
 	, m_stringId(-1)
{
	m_text[0] = '\0';
	setRect(IntRect(0, 0, BTN_DEFAULT_WIDTH, BTN_DEFAULT_HEIGHT));
	setFocusValid(true);
}

ButtonView::ButtonView(int i_id, int x, int y, int w, int h)
	: View(NULL, DEFAULT_VIEW_DRAWABLESET(ButtonView))
{
	m_text[0] = '\0';
	setRect(IntRect(x, y, x + w, y + h));
	setId(i_id);
	setFocusValid(true);
}

ButtonView::~ButtonView()
{

}


void ButtonView::setText(const char* str)
{
	int len = 0;

	if (NULL == str) {
        str = "";
    }
	
	m_stringId = -1;
	len = strlen (str);
	if (len > TEXT_BUFFER_LEN_OF_DEFAULT) {
	    m_buttonString = str;
	    m_isLong = true;	
	}
	else {
	    // short text
		memcpy (m_text, str, len);
		m_text[len] = '\0';
	}
}

const char* ButtonView::getText(void)
{
	if (m_stringId > 0) {
        return GetTextRes(m_stringId);
    }
    else if(m_isLong) {
		return m_buttonString.c_str();
    }
    else {
	    return m_text;
    }
}

bool ButtonView::dispatchEvent(Event* event)
{
    if (isDisabled()) {
        return View::dispatchEvent(event);
    }

    switch (event->eventType()) {
        case Event::MOTION_DOWN:
            lButtonDown(event);
            break;
        case Event::MOTION_UP:
            lButtonUp(event);
            break;
        case Event::MOTION_CLICK:
            lButtonClick(event);
            break;
        case Event::KEY_DOWN:
            keyDown(event);
            break;
        case Event::KEY_UP:
            keyUp(event);
            break;
        case Event::KEY_LONGPRESSED:
            keyLongPress(event);
            break;
        default:
            return View::dispatchEvent(event);
    }

    updateView();
    return DISPATCH_STOP_MSG;
}


//process key down
void ButtonView::keyDown(Event* event)
{
	setState(PUSHED);
}

//process key up
void ButtonView::keyUp(Event* event)
{
	setState(HILIGHT);
}

//process key pressed
void ButtonView::keyLongPress(Event* event)
{
	setState(HILIGHT);
}

//process mouse down
void ButtonView::lButtonDown(Event* event)
{
	setState(PUSHED);
}

//process mouse up
void ButtonView::lButtonUp(Event* event)
{
	setState(HILIGHT);
}

//process click event
void ButtonView::lButtonClick(Event* event)
{
	setState(HILIGHT);
}

void ButtonView::onGetFocus()
{
	if(isDisabled())
		return;
	if(getState() == PUSHED)
		setState(HILIGHT);
	
//    LOGDEBUG("ButtonView::setFocusView!!!\n");
    updateView();
}

void ButtonView::onLoseFocus()
{

//    LOGDEBUG("ButtonView::onLoseFocus!!!\n");
	if(isDisabled())
		return ;

	setState(NORMAL);

    updateView();
}


DEFINE_CLASS_NAME(ButtonView)

} // namespace hfcl {

