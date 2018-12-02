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


#ifndef _NGUX_BaseApp_h
#define _NGUX_BaseApp_h

#include "nguxcommon.h"
#include "controller.h"

NAMESPACE_BEGIN

class ContextStream;
class Intent;

class BaseApp : public Controller{
	public:
		typedef enum _tagAPP_STATE
		{
			ORIGIN = 0,
			INIT, 
			RUNNING, 
			SLEEP, 
			SUSPEND, 
		}APP_STATE;

		BaseApp():m_priority(PRIORITY_HIGH), m_name(NULL), m_state(INIT){};
		virtual ~BaseApp();

		NGCPStr name(void);
		void setName(NGCPStr name);
		APP_STATE state(void);
		void setState(APP_STATE state);
		void close(void);

		virtual void onCreate(ContextStream *contextStream, Intent *intent){}
		virtual void onStart(void){}
		virtual void onSleep(void){}
		virtual void onWakeup(void){}
		virtual void onMove2Top(void) {}
		virtual Intent *onDestroy(ContextStream *contextStream){return NULL;}
		virtual NGBool isSuspendable(void);

        /* VincentWei: add this method as virtual */
        virtual NGBool beInLockFrame(void) {return false;};

		virtual void exit() { close(); }

		enum {
			PRIORITY_LOWEST,
			PRIORITY_LOW,
			PRIORITY_MIDDLE,
			PRIORITY_HIGH,
			PRIORITY_HIGHEST,
		};

		void setPriority(int i_priority) { m_priority = i_priority; }
		int priority(void) { return m_priority; }

	protected:
		NGInt m_priority;
		char * m_name;
		APP_STATE m_state;

		View* getViewParent(NGInt view_id) { return NULL; }
};

NAMESPACE_END

#endif /* NGUX_BaseApp_h */
