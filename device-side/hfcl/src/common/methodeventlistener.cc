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

#include "common/methodeventlistener.h"

namespace hfcl {

MethodEventListener::MethodEventListener (void* ctx, EventHandle event_handle, int event_type)
    :EventListener(0)
{
    m_ctx = ctx;
    m_eventHandle = event_handle;
    m_eventType = event_type;
}

MethodEventListener::~MethodEventListener()
{
}

bool MethodEventListener::handler(Event* event)
{
    if (m_ctx && m_eventHandle && event && (m_eventType == 0 || event->eventType() == m_eventType)) {
        (((MethodEventListener*) m_ctx)->*m_eventHandle)(event);
        return EVENT_HANDLED;
    }
    return EVENT_NOT_HANDLED;
}

} // namespace hfcl
