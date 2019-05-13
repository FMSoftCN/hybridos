/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2019 Beijing FMSoft Technologies Co., Ltd.
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

/*
** dom-event.cc: The implementation of DOM Event, EventTarget interfaces:
**
** Create by WEI Yongming at 2019/05/12
*/

#include "hvml/dom-event.h"

#include <minigui/common.h>
#include <minigui/minigui.h>

#include "hvml/dom.h"

namespace hfcl {
namespace dom {

EventTarget::~EventTarget()
{
    remove_all_event_listener();
}

const EventTargetSeq& Event::composedPath()
{
    m_composed_path.clear();
    if (m_path.empty()) {
        return m_composed_path;
    }

    m_composed_path.push_back(m_current_target);

    int currentTargetIndex = 0;
    int currentTargetHiddenSubtreeLevel = 0;

    size_t index = m_path.size() - 1;
    while (index >= 0) {
        if (m_path[index]->root_of_closed_tree) {
            currentTargetHiddenSubtreeLevel++;
        }

        if (m_path[index]->item == m_current_target) {
            currentTargetIndex = index;
            break;
        }

        if (m_path[index]->slot_in_closed_tree) {
            currentTargetHiddenSubtreeLevel--;
        }

        index--;
    }

    int currentHiddenLevel = currentTargetHiddenSubtreeLevel;
    int maxHiddenLevel = currentTargetHiddenSubtreeLevel;

    index = currentTargetIndex - 1;
    while (index >= 0) {
        if (m_path[index]->root_of_closed_tree)
            currentHiddenLevel++;

        if (currentHiddenLevel <= maxHiddenLevel) {
            m_composed_path.push_front(m_path[index]->item);
        }

        if (m_path[index]->slot_in_closed_tree) {
            currentHiddenLevel--;
            if (currentHiddenLevel < maxHiddenLevel)
                maxHiddenLevel = currentHiddenLevel;
        }

        index--;
    }

    currentHiddenLevel = currentTargetHiddenSubtreeLevel;
    maxHiddenLevel = currentTargetHiddenSubtreeLevel;
    index = currentTargetIndex + 1;
    size_t size = m_path.size();
    while (index < size) {
        if (m_path[index]->slot_in_closed_tree)
            currentHiddenLevel++;

        if (currentHiddenLevel <= maxHiddenLevel) {
            m_composed_path.push_back(m_path[index]->item);
        }

        if (m_path[index]->root_of_closed_tree) {
            currentHiddenLevel--;
            if (currentHiddenLevel < maxHiddenLevel)
                maxHiddenLevel = currentHiddenLevel;
        }

        index++;
    }

    return m_composed_path;
}

bool EventTarget::check_type()
{
    // TODO: If eventTarget’s relevant global object is a
    // ServiceWorkerGlobalScope object and its associated service worker’s
    // script resource’s has ever been evaluated flag is set, then throw
    // a TypeError.

    return true;
}

void EventTarget::add_event_listener(const DOMString& type,
        EventListener callback, const AddEventListenerOptions* options)
{
    if (MG_UNLIKELY (check_type())) {
        throw TypeError();
    }

    if (callback == NULL)
        return;

    DOMString key(type);

    char buff[33];
    memset(buff, 0, 33);
    snprintf(buff, 32, "%p", callback);
    key += buff;
    if (options->capture)
        key += '1';
    else
        key += '0';

    EventListenerList::iterator it = m_event_listener_list.find(key);
    if (it == m_event_listener_list.end()) {
        EventListenerEntry* entry = mg_slice_new(EventListenerEntry);
        entry->type = strdup (type.c_str());
        entry->callback = callback;
        entry->capture = options->capture;
        entry->passive = options->passive;
        entry->once = options->once;
        entry->removed = 0;
        m_event_listener_list[key] = entry;
    }
}

void EventTarget::remove_event_listener(const DOMString& type,
        EventListener callback, const EventListenerOptions* options)
{
    if (MG_UNLIKELY (check_type())) {
        throw TypeError();
    }

    if (callback == NULL)
        return;

    DOMString key(type);

    char buff[33];
    memset(buff, 0, 33);
    snprintf(buff, 32, "%p", callback);
    key += buff;
    if (options->capture)
        key += '1';
    else
        key += '0';

    EventListenerList::iterator it = m_event_listener_list.find(key);
    if (it != m_event_listener_list.end()) {
        EventListenerEntry* entry = it->second;
        m_event_listener_list.erase(key);

        entry->removed = 1;
    }
}

void EventTarget::remove_all_event_listener()
{
    EventListenerList::iterator it;
    for (it = m_event_listener_list.begin();
            it != m_event_listener_list.end(); ++it) {
        EventListenerEntry* entry = it->second;
        m_event_listener_list.erase(it->first);

        entry->removed = 1;
        // TODO: Who is responsible for deleting the entry?
        // free (entry->type);
        // mg_slice_delete(EventListenerEntry, entry);
    }
}

bool EventTarget::dispatchEvent(Event& event)
{
    if (event.m_flags & Event::DISPATCH_FLAG ||
            !(event.m_flags & Event::INITIALIZED_FLAG)) {
        throw DOMException(__func__, "InvalidStateError");
    }

    event.m_is_trusted = false;

    return real_dispatch_event(event, *this);
}

bool EventTarget::real_dispatch_event(Event& event, EventTarget& eventTarget,
        bool legacyTargetOverride, bool legacyOutputDidListenersThrow)
{
    event.m_flags |= Event::DISPATCH_FLAG;

    return false;
}

} // namespace dom
} // namespace hfcl

