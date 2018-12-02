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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef _NGUX_WrapperView_h
#define _NGUX_WrapperView_h

#include "control.h"
#include "view.h"
#include "font.h"

NAMESPACE_BEGIN

class WrapperView : public View     // GT_jyseo rollback  , public EventListener
{
public:
	WrapperView(View *p_parent):View(p_parent, DEFAULT_VIEW_DRAWABLESET(WrapperView)) {  }
	WrapperView(View *p_parent, DrawableSet* drset)
		:View(p_parent, drset) {  }

	~WrapperView();
    void hide();
    void show();
    void showCaret(NGBool show);
    NGBool isWrapprVisible ();
	//virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
protected:
	typedef NGUInt HANDLE;
	HANDLE m_wrapwnd; // the minigui window handle 

	virtual void createWnd(NGCPStr wnd_class);
	void detachWnd();

private:
	static void _notify_proc(HANDLE handle, NGInt id, NGInt nc, DWORD add_data);
	NGBool onKeyPressed(NGInt keyCode);
#if 0   // GT_jyseo rollback
	NGBool handleEvent(Event* event);
	void closeTimer(void);
#endif    
	void calcWndRect(IntRect& r);
#if 0   // GT_jyseo rollback
	NGInt m_timerId;
	NGInt m_timerCount;
#endif  	
	NGInt m_pressedKey;
	NGInt m_keyStatus;

public:
    enum{
        TEXT_READONLY = 0x00000800L,
        TEXT_AUTOWRAP = 0x00002000L,
        TEXT_BASELINE = 0x00001000L,
        TEXT_LINEHEIGHT = 0x0000F0BEL,
        TEXT_LIMIT = 0x0000F0C5L,
        TEXT_ALIGN_RIGHT = 0x00000002L,
        TEXT_ALIGN_LEFT = 0x00000000L,
        TEXT_ALIGN_CENTER = 0x00000001L,
        TEXT_STYLE_PASSWD = 0x00000020L,
        TEXT_STYLE_PASSWD_CHAR = 0xF0CC,
        TEXT_INSERT = 0x000F0C0L,
        TEXT_MSG_CHAR = 0X11,
    };
	enum {
		NOTIFY_BEGIN = CustomEvent::CUS_MAX,
		NOTIFY_EDIT_CHANGED = 1000,
		NOTIFY_EDIT_ENTER,  // only in single-line edit
		NOTIFY_EDIT_MAXTEXT,
	};
	void setText(NGCPStr str);
	void setText(string str) { setText(str.c_str()); }
	string getText();
    void setViewStyle(NGInt dwStyle, NGBool isInclde);
    void setIncludeStyle(NGInt dwStyle) { setViewStyle(dwStyle, true); }
    void setExcludeStyle(NGInt dwStyle) { setViewStyle(dwStyle, false); }
    void setTextMaxLimit(NGInt max);
	void insertText(const char *text);
	void replacePrevChar(const char *ch);
    NGULong  viewStyle(void);
	virtual HPlatformOwner getPlatformOwner() { return m_wrapwnd; }
	virtual void getOwnerOffset(NGInt &x, NGInt &y) { /* stop the offset  */}

    NGBool setRect(const IntRect& pirc);
	// to void drived class overide the base class 's function
    NGBool setRect(NGInt left, NGInt top, NGInt right, NGInt bottom) { 
		return View::setRect(left, top, right, bottom); 
	}

	void setFont (Font *f) {
		SetWindowFont(m_wrapwnd, f->getLogfont());
	}

	virtual NGBool dispatchEvent(Event *event);

	virtual void onGetFocus(void);
	virtual void onLoseFocus(void);

	NGBool isWrapperView() { return true;}

protected:
	virtual NGInt notifyToEventType(NGInt nc_code) { return 0; }

	DECLARE_CLASS_NAME(WrapperView)
};


NAMESPACE_END


#endif

