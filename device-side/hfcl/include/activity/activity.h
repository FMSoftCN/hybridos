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

#ifndef HFCL_ACTIVITY_ACTIVITY_H_
#define HFCL_ACTIVITY_ACTIVITY_H_

#include "common/event.h"
#include "activity/window.h"
#include "activity/intent.h"
#include "activity/baseactivity.h"
#include "activity/activitystack.h"
#include "components/menu.h"

namespace hfcl {

class Activity : public BaseActivity, public Window {
public:
    Activity();
    virtual ~Activity();

    virtual void onCreate(ContextStream* contextStream, Intent* intent);
    virtual void onWakeup(void);

    virtual bool onKey(int keyCode, KeyEvent* event);
    virtual void onClick(POINT pt, Event::EventType type);	
    virtual void onBack(void) { }
    virtual void onIdle(void) { }
    virtual void onMove2Top(void);
    virtual void bindMenu(Menu* menuInstance){m_menu = menuInstance;}
    virtual Menu* menu(void){ return m_menu ;}
    virtual HWND getSysWindow(void);

    void updateNow(void);
    static Activity* activity(HWND hwnd);

    virtual EventListener* getEventHandle() { return NULL; };

    virtual void setFullScreen(bool isFullScreen);

protected:
    View * getViewParent(int view_id) { return (View*)this; }

private:
    Menu * m_menu;
    IntRect m_old_rect;
};

class FullScreenActivity : public Activity {
public:
    FullScreenActivity() : Activity() { }
    virtual ~FullScreenActivity() { }
   
    virtual void onCreate(ContextStream* contextStream, Intent* intent); 
};

} // namespace hfcl

#endif /* HFCL_ACTIVITY_ACTIVITY_H_ */

