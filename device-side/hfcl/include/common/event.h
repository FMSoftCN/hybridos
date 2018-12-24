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

#ifndef HFCL_COMMON_EVENT_H_
#define HFCL_COMMON_EVENT_H_

#define EVENT_HANDLED      1
#define EVENT_NOT_HANDLED  0

#include "../common/common.h"
#include "../common/stlalternative.h"
#include "../common/object.h"

// GOON_DISPATCHING to continue dispatching the event;
// STOP_DISPATCHING to stop dispatching the event.
#define GOON_DISPATCH   false
#define STOP_DISPATCH       true

namespace hfcl {

class Event {
public:
    enum EventType {
        KEY_DOWN,
        KEY_UP,
        KEY_LONGPRESSED,
        KEY_ALWAYSPRESS,
        KEY_CHAR,
        MOUSE_DOWN,
        MOUSE_UP,
        MOUSE_MOVE,
        MOUSE_MOVEIN,
        MOUSE_MOVEOUT,
        MOUSE_CLICKED,
        MOUSE_DBLCLICKED,
        TIMER,
        CUSTOM_NOTIFY,
    };

    Event (EventType type, HTData eventSource = 0)
        : m_eventType (type)
        , m_eventSource (eventSource) {
    }

    virtual ~Event() {
    }

    inline HTData getSource() const {
        return m_eventSource;
    }

    inline void setSource (HTData eventSource) {
        m_eventSource = eventSource;
    }

    EventType eventType() {
        return m_eventType;
    }

private:
    EventType m_eventType;
    HTData m_eventSource;
};

class CustomEvent : public Event {
public:
    enum CustomParam {
        CUS_BOUNDARY_LEFT,
        CUS_BOUNDARY_RIGHT,
        CUS_BOUNDARY_UP,
        CUS_BOUNDARY_DOWN,
        CUS_DLG_KEY_SL,
        CUS_DLG_KEY_SR,
        CUS_DLG_SHOW,
        CUS_DLG_HIDE,
        CUS_SELCHANGED,
        CUS_GIFANIMATE_STOP,
        CUS_PAGECHANGED,
        CUS_FREQCHANGED,
        // BusyListView use for load data from user.
        CUS_LOAD_DATA,
        CUS_MAX,
    };

    CustomEvent (EventType type, HTData wparam, HTData lparam,
            HTData exParam1 = 0, HTData exParam2 = 0)
        : Event (type)
        , m_wParam (wparam)
        , m_lParam (lparam)
        , m_exParam1 (exParam1)
        , m_exParam2 (exParam2) {
    }

    virtual ~CustomEvent () {
    }

    void setExParam (HTData exParam1, HTData exParam2) {
        m_exParam1 = exParam1;
        m_exParam2 = exParam2;
    }

    inline HTData customWParam () const {
        return m_wParam;
    }

    inline HTData customLParam () const {
        return m_lParam;
    }

    inline HTData customExParam1 () const {
        return m_exParam1;
    }

    inline HTData customExParam2 () const {
        return m_exParam2;
    }

private:
    HTData m_wParam;
    HTData m_lParam;
    HTData m_exParam1;
    HTData m_exParam2;
};

class KeyEvent : public Event {
public:
    enum KeyCode {
        KEYCODE_0 = 0,  // SCANCODE_1
        KEYCODE_1,      // SCANCODE_2
        KEYCODE_2,      // SCANCODE_3
        KEYCODE_3,      // SCANCODE_4
        KEYCODE_4,      // SCANCODE_5
        KEYCODE_5,      // SCANCODE_6
        KEYCODE_6,      // SCANCODE_7
        KEYCODE_7,      // SCANCODE_8
        KEYCODE_8,      // SCANCODE_9
        KEYCODE_9,      // SCANCODE_0

        KEYCODE_STAR,   // SCANCODE_F1: "*"
        KEYCODE_POUND,  // SCANCODE_F2: "#"
        KEYCODE_SOFTKEY_LEFT,   // SCANCODE_F3: left soft key
        KEYCODE_SOFTKEY_RIGHT,  // SCANCODE_F4: right soft key
        KEYCODE_CALL,   // SCANCODE_F5:
        KEYCODE_MUSIC,  // SCANCODE_F6:
        KEYCODE_CALL2,  // SCANCODE_F7:
        KEYCODE_CAMERA, // SCANCODE_F8:
        KEYCODE_FM,     // SCANCODE_F9:
        KEYCODE_SMS,    // SCANCODE_F10:

        KEYCODE_START,  // SCANCODE_F11:
        KEYCODE_STOP,   // SCANCODE_F12:
        KEYCODE_PAUSE,  // SCANCODE_PAUSE

        KEYCODE_HOME,           // SCANCODE_HOME
        KEYCODE_CURSOR_UP,      // SCANCODE_CURSORBLOCKUP
        KEYCODE_VOL_UP,         // SCANCODE_PAGEUP
        KEYCODE_CURSOR_LEFT,    // SCANCODE_CURSORBLOCKLEFT
        KEYCODE_CURSOR_RIGHT,   // SCANCODE_CURSORBLOCKRIGHT
        KEYCODE_END,            // SCANCODE_END
        KEYCODE_CURSOR_DOWN,    // SCANCODE_CURSORBLOCKDOWN
        KEYCODE_VOL_DOWN,       // SCANCODE_PAGEDOWN

        KEYCODE_SPACE,      // SCANCODE_SPACE
        KEYCODE_BACK,       // SCANCODE_ESCAPE

        KEYCODE_ENTER,      // SCANCODE_ENTER
        KEYCODE_POWER,      // SCANCODE_POWER

        KEYCODE_BACKSPACE,  // SCANCODE_BACKSPACE
        KEYCODE_REMOVE,     // SCANCODE_REMOVE

        KEYCODE_SOFTKEY_CENTER = KEYCODE_ENTER,

        KEYCODE_UNKNOWN,
    };

    KeyEvent (EventType type, int keyCode, int scanCode, unsigned int keyStatus)
        : Event(type)
        , m_keyCode(keyCode)
        , m_scanCode(scanCode)
        , m_keyStatus(keyStatus) {
    }

    virtual ~KeyEvent() {
    }

    inline int scanCode() const {
        return m_scanCode;
    }

    inline int keyCode() const {
        return m_keyCode;
    }

    inline unsigned int keyStatus() const {
        return m_keyStatus;
    }

    inline void setKeyStatus (unsigned int keyStatus) {
        m_keyStatus = keyStatus;
    }

private:
    int m_keyCode;
    int m_scanCode;
    unsigned int m_keyStatus;
};

class MouseEvent : public Event {
public:
    MouseEvent (EventType type, int x, int y, unsigned int keyStatus = 0)
        : Event(type)
        , m_x(x)
        , m_y(y)
        , m_keyStatus(keyStatus) {
    }

    virtual ~MouseEvent() {
    }

    inline int x() const {
        return m_x;
    }
    inline int y() const {
        return m_y;
    }

private:
    int m_x;
    int m_y;
    unsigned int m_keyStatus;
};

class TimerEvent : public Event {
public:
    TimerEvent (EventType type, int timerId)
        : Event(type)
        , m_timerId(timerId) {
    }

    virtual ~TimerEvent() {
    }

    inline int timerID() const {
        return m_timerId;
    }

private:
    int m_timerId;
};

class EventListener : public RefCount {
public:
    EventListener() : RefCount (0) { }

    virtual bool handleEvent(Event* event) = 0;

protected:
    EventListener(int start_ref) : RefCount(start_ref) { }
};

class EventBroadcaster {
public:
    EventBroadcaster() {
        m_current_raise = NULL;
        m_want_to_remove = NULL;
    }
    virtual ~EventBroadcaster() {
        releaseEventListeners();
    }

public:
    virtual bool raiseEvent(Event* event);
    void addEventListener(EventListener* listener);
    void removeEventListener(EventListener* listener);

protected:
    void releaseEventListeners();

    // notice : dont unref for these event listeners
    // becuase the add interface haven't add the reference
    //
    LIST(EventListener *, EventListenerList);
    EventListenerList m_listeners;

    EventListener* m_current_raise;
    EventListener* m_want_to_remove;
};

} // namespace hfcl

#endif /* HFCL_COMMON_EVENT_H_ */

