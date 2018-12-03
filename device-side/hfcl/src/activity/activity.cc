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

#include "activity/activity.h"

#include "common/intrect.h"
#include "common/event.h"
#include "activity/log.h"
#include "activity/intent.h"

namespace hfcl {

Activity::Activity()
    : Window()
{
	m_menu = NULL;
}

Activity::~Activity()
{

}

void Activity::onCreate(ContextStream* contextStream, Intent* intent)
{
    createMainWindow(0, _ngux_phonebar_h, _ngux_screen_w, _ngux_screen_h - _ngux_phonebar_h);
}

void Activity::onWakeup(void)
{
	setActiveWindow (viewWindow());
	show();
};

void Activity::onMove2Top(void) 
{
    if (NULL != m_menu)
    {
        //m_menu->closeMenu(m_menu);
        m_menu->closeAllMenu();
        m_menu = NULL;
    }
	setActiveWindow (viewWindow());
    show();
}

HPlatformOwner Activity::getPlatformOwner(void)
{
    return m_viewWindow;
}

bool Activity::onKey(int keyCode, KeyEvent* event)
{
    View* _focus = NULL;
    bool ret = true;

    if ((View *)0 != (_focus = focusView())) {
        ret = _focus->dispatchEvent(event);
#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__
        return ret;
#endif
    }

    return ret;
}

void Activity::onClick(POINT pt, Event::EventType type)
{
    RECT rc;
    View* child = firstChild();

    while (child != (View *)0) {
        IntRect irc = child->getRect();
        rc.left   = irc.left();
        rc.top    = irc.top();
        child->viewToWindow(&rc.left, &rc.top);
        rc.right = rc.left + irc.width();
        rc.bottom = rc.top + irc.height();

        //current cursor pos is related with screen.
        ScreenToClient(m_viewWindow, &pt.x, &pt.y);

        if (PtInRect(&rc, pt.x, pt.y)) {
            //Event::EventType eventType = Event::MOTION_CLICK;
            MouseEvent event(type, pt.x - rc.left, pt.y - rc.top);
            child->dispatchEvent(&event);
            break;
        }

        child = child->nextSibling();
    }
}

Activity* Activity::app(HWND hwnd)
{
    return reinterpret_cast<Activity*>(window(hwnd));
}

void Activity::updateAppNow(void)
{
    updateWindow();
}


void Activity::setFullScreen(bool isFullScreen)
{
    if (isFullScreen)
        setAppRect(IntRect(0, 0, _ngux_screen_w, _ngux_screen_h));
    else
		setAppRect(IntRect(0, _ngux_phonebar_h, _ngux_screen_w, _ngux_screen_h));
}


void FullScreenActivity::onCreate(ContextStream* contextStream, Intent* intent)
{
    createMainWindow();
}

} // namespace hfcl {

