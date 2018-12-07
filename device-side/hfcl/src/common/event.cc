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

#include "common/event.h"

namespace hfcl {

bool EventBroadcaster::raiseEvent (Event* event)
{
    EventListenerList::iterator it, next;

    if(m_listeners.size() <= 0)
        return true;

    for (it = m_listeners.begin(); it != m_listeners.end(); ){
        m_current_raise = *it;
        next = ++it;

        // hal_HstSendEvent(0xcc9cFFFF);
        if(m_current_raise) {
             // hal_HstSendEvent((UINT32)(m_current_raise));
            m_current_raise->handleEvent(event);
        }

        // in handleEvent, some may be removed, so we need to see.
        if(m_want_to_remove) {
            // 1. remove current one, we just remove it here
            if(m_want_to_remove == m_current_raise) {
                m_listeners.remove(m_want_to_remove);
            }
            // 2. remove next one, we just jump over "next", then remove "next"
            // it now is next, we so
            else if(m_want_to_remove == (*next)) {
                // end? we are done, no need to remove
                if(next == m_listeners.end())
                    break;

                // not end? we should move next->next
                ++it;

                // then real remove "next"
                m_listeners.remove(*next);
            }
            // 3. remove other, we just remove here, everything is OK
            else {
                m_listeners.remove(m_want_to_remove);
            }

            m_want_to_remove = NULL;
        }
    }

    m_want_to_remove = NULL;
    m_current_raise = NULL;
    return true;
}

void EventBroadcaster::addEventListener(EventListener* listener)
{
    if (NULL == listener) {
        return;
    }

    // we push back that if add when raiseEvent it is OK
    m_listeners.push_back(listener);
}

void EventBroadcaster::removeEventListener(EventListener* listener)
{
    if (NULL == listener) {
        return;
    }

    // FIXME, remove when raiseEvent, we need to avoid abort
    // 1. if we are in raiseEvent, we can NOT remove, we just mark the object
    // we will remove in raiseEvent.
    if (m_current_raise != NULL) {
        m_want_to_remove = listener;
    }
    // 2. if we are not in raiseEvent, we just remove it.
    else {
        m_listeners.remove(listener);
    }
}

void EventBroadcaster::releaseEventListeners()
{
    EventListenerList::iterator _listener;
    EventListenerList::iterator _temp;

    if (m_listeners.size() <= 0) {
        return;
    }

    _listener = m_listeners.begin();
    _temp = _listener;
    do {
        ++_listener;
        EventListener* child = *_temp;
        if (child != NULL) {
            // child->unref();
            m_listeners.erase(_temp);
        }
        _temp = _listener;
        if (m_listeners.size() == 0) {
            break;
        }
    }
    while (true);

    m_current_raise = NULL;
    m_want_to_remove = NULL;
}

} // namespace hfcl

