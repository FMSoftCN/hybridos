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

namespace hfcl {
namespace dom {

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

void EventTarget::addEventListener(const DOMString& type, EventListener callback,
            bool passive, bool once)
{
}

void EventTarget::removeEventListener(const DOMString& type, EventListener callback,
            bool capture)
{
}

bool EventTarget::dispatchEvent(const Event& event)
{
    return false;
}

EventTarget::~EventTarget()
{
    std::list<EventListenerEntry*>::iterator it;
    for (it = m_event_listener_list.begin();
            it != m_event_listener_list.end(); ++it) {
        EventListenerEntry* entry = *it;
        delete entry;
    }
}

} // namespace dom
} // namespace hfcl

