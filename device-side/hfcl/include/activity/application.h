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


#ifndef __NGUX_APPLICATION_H__
#define __NGUX_APPLICATION_H__

#include "nguxwindow.h"
#include "baseapp.h"
#include "nguxmenu.h"

NAMESPACE_BEGIN

class GraphicsContext;
class IntRect;
class KeyEvent;
class Intent;

class Application : public BaseApp, public Window
{
    public:
        Application();
        virtual ~Application();

        virtual void onCreate(ContextStream* contextStream, Intent* intent);
		virtual void onWakeup(void);

        virtual NGBool onKey(NGInt keyCode, KeyEvent* event);
        virtual void onClick(POINT pt, Event::EventType type);	
        virtual void onBack(void) { }
		virtual void onIdle(void) { }
		virtual void onMove2Top(void);
		virtual void bindMenu(Menu* menuInstance){m_menu = menuInstance;}
		virtual Menu* menu(void){ return m_menu ;}
		
        virtual HPlatformOwner getPlatformOwner(void);

        void updateAppNow(void);
        static Application* app(HWND hwnd);

        virtual EventListener* getEventHandle() { return NULL; };

        virtual void setFullScreen(NGBool isFullScreen);

	protected:
		View * getViewParent(NGInt view_id) { return (View*)this; }

	private:
		Menu * m_menu;
};


class FullScreenApp : public Application {
    public:
        FullScreenApp() : Application() { }

        virtual ~FullScreenApp() { }
       
        virtual void onCreate(ContextStream* contextStream, Intent* intent); 
};

NAMESPACE_END

#endif /* __NGUX_APPLICATION_H__ */

