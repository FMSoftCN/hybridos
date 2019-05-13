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

#ifndef HFCL_HVML_DOM_EVENT_H_
#define HFCL_HVML_DOM_EVENT_H_

#include <map>
#include <vector>
#include <list>

#include "hvml/dom.h"

namespace hfcl {
namespace dom {

class Event;

typedef void (*EventListener)(const Event& event);

typedef struct _EventListenerEntry {
    const char*         type;
    EventListener       callback;
    unsigned char       capture:1;
    unsigned char       passive:1;
    unsigned char       once:1;
    unsigned char       removed:1;
} EventListenerEntry;

typedef std::map<DOMString, EventListenerEntry*> EventListenerList;

typedef struct _EventListenerOptions {
    bool capture;
} EventListenerOptions;

typedef struct _AddEventListenerOptions {
    bool capture;
    bool passive;
    bool once;
} AddEventListenerOptions;

class EventTarget {
public:
    EventTarget() {};
    virtual ~EventTarget();

    void addEventListener(const DOMString& type, EventListener callback,
            const AddEventListenerOptions* options) {
        add_event_listener(type, callback, options);
    }

    // for compatibility
    void addEventListener(const DOMString& type, EventListener callback,
            bool option) {
        AddEventListenerOptions options;
        options.capture = option;
        options.passive = false;
        options.once = false;
        add_event_listener(type, callback, &options);
    }

    void removeEventListener(const DOMString& type, EventListener callback,
            const EventListenerOptions* options) {
        remove_event_listener(type, callback, options);
    }

    // for compatibility
    void removeEventListener(const DOMString& type, EventListener callback,
            bool option) {
        EventListenerOptions options;
        options.capture = option;
        remove_event_listener(type, callback, &options);
    }

    bool dispatchEvent(Event& event);

    static bool real_dispatch_event(Event& event, EventTarget& eventTarget,
            bool legacyTargetOverride = false,
            bool legacyOutputDidListenersThrow = false);

private:
    bool check_type();
    void add_event_listener(const DOMString& type, EventListener callback,
            const AddEventListenerOptions* options);
    void remove_event_listener(const DOMString& type, EventListener callback,
            const EventListenerOptions* options);
    void remove_all_event_listener();

    EventListenerList m_event_listener_list;
};

typedef std::list<EventTarget*> EventTargetSeq;

typedef struct _EventPathEntry {
    EventTarget*    item;
    EventTarget*    target;
    EventTarget*    related_target;
    EventTargetSeq  touch_target_list;
    unsigned char   in_shadow_tree:1;
    unsigned char   root_of_closed_tree:1;
    unsigned char   slot_in_closed_tree:1;
} EventPathEntry;

typedef std::vector<EventPathEntry*> EventPath;

class Event {
public:
    Event () {
        m_flags = 0;
        m_target = NULL;
        m_related_target = NULL;
        m_current_target = NULL;
        m_event_phase = NONE;
        m_is_trusted = false;

        m_type = "";
    }

    Event (const DOMString& type,
            bool bubbles = false, bool cancelable = false,
            bool composed = false) {
        m_flags = INITIALIZED_FLAG;
        m_target = NULL;
        m_related_target = NULL;
        m_current_target = NULL;
        m_event_phase = NONE;
        m_is_trusted = false;

        m_type = type;
        m_bubbles = bubbles;
        m_cancelable = cancelable;
    }

    virtual ~Event() {
        m_composed_path.clear();
        m_path.clear();
    }

    // readonly attribute DOMString type;
    const DOMString& getType() const {
        return m_type;
    };

    // readonly attribute EventTarget? target;
    const EventTarget* getTarget() const {
        return m_target;
    }

#ifdef _HFCL_INCLUDE_HISTORICAL_DOM_INTERFACES
    // readonly attribute EventTarget? srcElement; // historical
    const EventTarget* getSrcElement() const {
        return m_target;
    }
#endif

    // readonly attribute EventTarget? currentTarget;
    const EventTarget* getCurrentTarget() const {
        return m_current_target;
    }

    const EventTargetSeq& composedPath();

    const unsigned short NONE = 0;
    const unsigned short CAPTURING_PHASE = 1;
    const unsigned short AT_TARGET = 2;
    const unsigned short BUBBLING_PHASE = 3;
    // readonly attribute unsigned short eventPhase;
    unsigned short getEventPhase() const {
        return m_event_phase;
    }

    void stopPropagation() {
        m_flags |= STOP_PROPAGATION_FLAG;
    }

#ifdef _HFCL_INCLUDE_HISTORICAL_DOM_INTERFACES
    // attribute boolean cancelBubble; // historical alias of .stopPropagation
    bool getCancelBubble() const {
        return (m_flags & STOP_PROPAGATION_FLAG);
    }
    void setCancelBubble(bool cancelBubble) {
        if (cancelBubble)
            m_flags |= STOP_PROPAGATION_FLAG;
    }
#endif

    void stopImmediatePropagation() {
        m_flags |= STOP_PROPAGATION_FLAG;
        m_flags |= STOP_IMMEDIATE_PROPAGATION_FLAG;
    };

    // readonly attribute boolean bubbles;
    bool getBubbles() const {
        return m_bubbles;
    }

    // readonly attribute boolean cancelable;
    bool getCancelable() const {
        return m_cancelable;
    }

#ifdef _HFCL_INCLUDE_HISTORICAL_DOM_INTERFACES
    // attribute boolean returnValue;  // historical
    bool getReturnValue() const {
        if (m_flags & CANCELED_FLAG)
            return false;
        return true;
    }
    void setReturnValue(bool returnValue) const {
        if (returnValue) {
            m_flags |= CANCELED_FLAG;
        }
    }
#endif

    void preventDefault() {
        m_flags |= CANCELED_FLAG;
    }

    // readonly attribute boolean defaultPrevented;
    bool getDefaultPrevented() const {
        if (m_flags & CANCELED_FLAG)
            return true;
        return false;
    }

    // readonly attribute boolean composed;
    bool getComposed() const {
        if (m_flags & COMPOSED_FLAG)
            return true;
        return false;
    }

    // [Unforgeable] readonly attribute boolean isTrusted;
    virtual bool getIsTrusted() const {
        return m_is_trusted;
    }

    // readonly attribute DOMHighResTimeStamp timeStamp;
    DOMHighResTimeStamp getTimeStamp() const {
        return m_time_stamp;
    }

#ifdef _HFCL_INCLUDE_HISTORICAL_DOM_INTERFACES
    // historical
    void initEvent(const DOMString& type,
            bool bubbles = false, bool cancelable = false) {
        if (m_flags & DISPATCH_FLAG)
            return;
        m_type = type;
        m_bubbles = bubbles;
        m_cancelable = cancelable;
    }
#endif

    friend class EventTarget;

private:
    DOMString       m_type;
    EventTarget*    m_target;
    EventTarget*    m_related_target;
    EventTarget*    m_current_target;

    bool            m_bubbles;
    bool            m_cancelable;
    bool            m_is_trusted;

    enum {
        STOP_PROPAGATION_FLAG           = (1 << 0),
        STOP_IMMEDIATE_PROPAGATION_FLAG = (1 << 1),
        CANCELED_FLAG                   = (1 << 2),
        IN_PASSIVE_LISTENER_FLAG        = (1 << 3),
        COMPOSED_FLAG                   = (1 << 4),
        INITIALIZED_FLAG                = (1 << 5),
        DISPATCH_FLAG                   = (1 << 6),
    };

    unsigned char   m_flags;
    unsigned short  m_event_phase;

    DOMHighResTimeStamp m_time_stamp;

    EventTargetSeq  m_composed_path;
    EventPath       m_path;
};

} // namespace dom
} // namespace hfcl

#endif /* HFCL_HVML_DOM_EVENT_H_ */

