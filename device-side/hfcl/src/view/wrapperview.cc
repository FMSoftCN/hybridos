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


#include "common/common.h"
#include "mgcl/mgcl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "cliprect.h"
#include "gal.h"
#include "internals.h"
#include "menu.h"
#include "ctrlclass.h"

#include "containerview.h"
#include "wrapperview.h"
#include "graphics/graphicscontext.h"

namespace hfcl {

#define EM_ENABLECARET 0xF0D5 // same with minigui

void WrapperView::createWnd(const char * wnd_class)
{
	if(!wnd_class || !m_parent)
		return ;

	HPlatformOwner howner = m_parent->getPlatformOwner();
	
	if(!::IsWindow(howner))
		return;

	IntRect rc = m_rect;
	calcWndRect(rc);

	m_wrapwnd = ::CreateWindowEx( wnd_class, "", 
			WS_VISIBLE|0x2000, WS_EX_TRANSPARENT,   // 0x2000 is ES_AUTO_WRAP 
			-1, rc.left(), rc.top(), rc.width(), rc.bottom(),
			howner, (DWORD)this);
	SetNotificationCallback(m_wrapwnd, _notify_proc);

#if 0	// GT_jyseo rollback
	m_timerId = 0;
	m_timerCount = 0;
#else
	m_pressedKey =0;
	m_keyStatus=0;
#endif
}

void WrapperView::setTextMaxLimit(int max)
{
    SendMessage (m_wrapwnd, TEXT_LIMIT, max, 0);
}


void WrapperView::insertText(const char *text)
{
	SendMessage(m_wrapwnd, TEXT_INSERT, strlen(text),(LPARAM)text);
}

void WrapperView::replacePrevChar(const char *ch)
{
	SendMessage(m_wrapwnd, MSG_CHAR, (WPARAM)127, 0);
	if(ch)
		insertText(ch);
}



void WrapperView ::setViewStyle(int dwStyle, bool isInclude)
{
    if(isInclude)
        IncludeWindowStyle(m_wrapwnd, (unsigned long)dwStyle);
    else
        ExcludeWindowStyle(m_wrapwnd, (unsigned long)dwStyle);
}

unsigned long WrapperView :: viewStyle(void)
{
    return GetWindowStyle(m_wrapwnd);
}
void WrapperView::hide()
{
    if(IsWindowVisible(m_wrapwnd))
    {
        ShowWindow(m_wrapwnd, SW_HIDE);
    }
	View::hide();
}

void WrapperView::show()
{
    if(!IsWindowVisible(m_wrapwnd))
    {
        ShowWindow(m_wrapwnd, SW_SHOWNORMAL);
    }
	View::show();
}

void WrapperView::detachWnd()
{
	//TODO detachWnd
	if(::IsWindow(m_wrapwnd))
		SetWindowAdditionalData(m_wrapwnd, 0);
}

WrapperView::~WrapperView() 
{
    if(::IsWindow(m_wrapwnd) && (WrapperView*)::GetWindowAdditionalData(m_wrapwnd) == this){
        SetWindowAdditionalData(m_wrapwnd, 0);
		::DestroyWindow(m_wrapwnd);
	}
}

bool WrapperView::isWrapprVisible ()
{
    View *v = this;
    while (NULL != v)
    {
        if (!v->isVisible())
            return false;
        v = (View *)v->parent();
    }
    return true;
}

void WrapperView::_notify_proc(HANDLE handle, int id, int nc, DWORD add_data)
{
	//TODO get the notification of the window
	WrapperView* self = (WrapperView*)GetWindowAdditionalData(handle);
    //_DBG_PRINTF("notify.....self= %p............handle=%x....\n", self, handle);
	if(self)
	{
		int enc = self->notifyToEventType(nc);
		if(enc > 0)
		{
			CustomEvent e(Event::CUSTOM_NOTIFY, enc, (int)self);
			self->raiseEvent(&e);
		}
	}
}

void WrapperView::calcWndRect(IntRect &r)
{
	int x = 0, y = 0;
	if(m_parent)
		m_parent->viewToWindow(&x, &y);
	r.offset(x, y);
}


void WrapperView::setText(const char * str)
{
	if(::IsWindow(m_wrapwnd))
	{
		::SetWindowText(m_wrapwnd, str);
	}
}

bool WrapperView::setRect(const IntRect& r)
{
	View::setRect(r);
	if(::IsWindow(m_wrapwnd))
	{
		IntRect rc = r;
		calcWndRect(rc);
		::MoveWindow(m_wrapwnd, rc.left(), rc.top(), rc.width(), rc.height(), false);

        /*
        SendMessage(m_wrapwnd, MSG_ERASEBKGND, 0, 0);
        EmptyClipRgn(&(((PCONTROL)m_wrapwnd)->InvRgn.rgn));
        InvalidateRect(GetParent(m_wrapwnd), NULL, true);
        */
	}
	return true;
}
bool WrapperView::onKeyPressed(int keyCode)
{
    //_DBG_PRINTF("\n wrapperviewonKeyPressed   !!!!  the m_pressedKey is:%x  keyCode is %x\n",m_pressedKey, keyCode);
    switch(keyCode)
    {
	case KeyEvent::KEYCODE_CURSOR_UP :
	case KeyEvent::KEYCODE_CURSOR_DOWN :
	case KeyEvent::KEYCODE_CURSOR_LEFT :
	case KeyEvent::KEYCODE_CURSOR_RIGHT :
	     SendMessage(m_wrapwnd, MSG_KEYDOWN, m_pressedKey == 0?keyCode:m_pressedKey, m_keyStatus);
            break;
	case KeyEvent::KEYCODE_SOFTKEY_RIGHT:
		SendMessage(m_wrapwnd, MSG_CHAR, (WPARAM)127, 0);
        default: 
            return false;
    }
    return true;
}

#if 0	// GT_jyseo rollback
void WrapperView::closeTimer(void)
{
	 if(m_timerId > 0) 
 	{
                TimerService::getInstance()->removeTimerListener(m_timerId);
                m_timerId = 0;
                m_timerCount = 0;
      }
}
#endif
bool WrapperView::dispatchEvent(Event *event)
{
	if(!::IsWindow(m_wrapwnd))
		return DISPATCH_STOP_MSG;

	switch(event->eventType())	
	{
        case Event::KEY_DOWN:
            m_keyStatus = ((KeyEvent*)event)->keyStatus();
            SendMessage(m_wrapwnd, MSG_KEYDOWN, 
                    ((KeyEvent*)event)->keyCode(), 
                    ((KeyEvent*)event)->keyStatus());
			return DISPATCH_STOP_MSG;
	     case Event::KEY_LONGPRESSED:
	     case Event::KEY_ALWAYSPRESS:
            {
#if 0    // GT_jyseo delete timer
                int code = ((KeyEvent *)event)->keyCode();
                if (onKeyPressed(code) && m_pressedKey == 0 && m_timerId == 0) 
                {
                    m_timerId = TimerService::getInstance()->addTimerListener(_EDIT_LOOP_RATE_, this);
                    m_pressedKey = code;
                    m_timerCount = 0;
                    return DISPATCH_STOP_MSG;
                } 
#else	
                m_pressedKey = ((KeyEvent *)event)->keyCode();
                if ( onKeyPressed (m_pressedKey) ) {
                    return DISPATCH_STOP_MSG;
                }
#endif
                break;
            }
         case Event::KEY_UP:
#if 0	// GT_jyseo rollback
            {
                closeTimer();
                m_pressedKey = 0;
                m_keyStatus=0;
            }
#endif
            m_keyStatus=0;
            m_pressedKey =0;
            SendMessage(m_wrapwnd, MSG_KEYUP, 
                    ((KeyEvent*)event)->keyCode(), 
                    ((KeyEvent*)event)->keyStatus());
            return DISPATCH_STOP_MSG;
         case Event::KEY_CHAR:
            SendMessage(m_wrapwnd, MSG_CHAR, (WPARAM)127, 0);
            break;
         default:
            return DISPATCH_CONTINUE_MSG;
	}

	return DISPATCH_CONTINUE_MSG;
}

#if 0 // GT_jyseo rollback
bool WrapperView::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER 
            && m_timerId == ((TimerEvent *)event)->timerID())
    {
        if (m_timerCount++ > 0){
            onKeyPressed(m_pressedKey);
        }
    }
    return DISPATCH_STOP_MSG;
}
#endif

void WrapperView::onGetFocus(void)
{
	::SetFocus(m_wrapwnd);
	// FIXME (wangjian)
	// needn't call funcuctin follow:
	// but there is a bug in minigui textedit.
	// (when set focus the caret can not be show auto.)
	showCaret(true);
	View::onGetFocus();
}

void WrapperView::onLoseFocus(void)
{
//	closeTimer();       // GT_jyseo rollback
	::SetNullFocus (::GetParent(m_wrapwnd));
	showCaret(false);
	View::onLoseFocus();
}

string WrapperView::getText(){
	if(!::IsWindow(m_wrapwnd))
		return string("");

	int len = ::GetWindowTextLength(m_wrapwnd);
	if(len <= 0)
		return string("");

	if(len < 256) {
		char szbuf[256];
		::GetWindowText(m_wrapwnd, szbuf, sizeof(szbuf)-1);
		return string(szbuf);
	}
	else {
		char * text = HFCL_NEW_ARR(char, (len + 1));
		::GetWindowText(m_wrapwnd, text, len);
		string str = text;
		HFCL_DELETE_ARR(text);
		return str;
	}
}

void WrapperView::showCaret(bool bShow)
{
    if (bShow) {
        SendMessage(m_wrapwnd, EM_ENABLECARET, 1, 0);
    } else {
        SendMessage(m_wrapwnd, EM_ENABLECARET, 0, 0);
    }
}

DEFINE_CLASS_NAME(WrapperView)

} // namespace hfcl {

