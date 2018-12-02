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

#ifndef _HFCL_MethodEventListener_h
#define _HFCL_MethodEventListener_h

#include "nguxcommon.h"
#include "nguxevent.h"

namespace hfcl {

class Event;
class EventListener;

class MethodEventListener : public EventListener
{
public:
	typedef bool (MethodEventListener::*EventHandle)(Event* event);
#ifdef WIN32
	MethodEventListener() {}
#endif
	MethodEventListener(void* ctx, EventHandle event_handle, int event_type);
	virtual ~MethodEventListener();

	bool handleEvent(Event* event);

private:
	void* m_ctx;
	EventHandle m_eventHandle;
	int m_eventType;
};

#define METHOD_EVENT_HANDLE_EX(owner, Class, eventHandle, event_type) \
	(HFCL_NEW_EX(MethodEventListener, ((void*)(owner), (MethodEventListener::EventHandle)(&Class::eventHandle), event_type)))

#define METHOD_EVENT_HANDLE(Class, eventHandle) \
	METHOD_EVENT_HANDLE_EX(this, Class, eventHandle, 0)


} // namespace hfcl {

#endif /* _HFCL_MethodEventListener_h */
