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

// GOON_DISPATCH to continue dispatching the event;
// STOP_DISPATCH to stop dispatching the event.
#define GOON_DISPATCH       false
#define STOP_DISPATCH       true

namespace hfcl {

class Event {
public:
    enum EventType {
        ET_KEY,
        ET_MOUSE,
        ET_MOUSE_WHEEL,
        ET_TIMER,
        ET_WINDOW,
        ET_VIEW,
        ET_USER,
        ET_ALL,
    };

    Event(EventType type, HTData eventSource = 0)
        : m_eventType (type)
        , m_eventSource (eventSource) {
    }
    virtual ~Event() { }

    HTData getSource() const { return m_eventSource; }
    void setSource(HTData eventSource) {
        m_eventSource = eventSource;
    }

    EventType eventType() { return m_eventType; }

private:
    EventType m_eventType;
    HTData m_eventSource;
};

class KeyEvent : public Event {
public:
    enum KeyEventType {
        KEY_DOWN,
        KEY_UP,
        KEY_LONGPRESSED,
        KEY_ALWAYSPRESS,
        KEY_CHAR,
    };

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

    KeyEvent (KeyEventType type, KeyCode keyCode, int scanCode, unsigned int keyStatus)
        : Event(ET_KEY)
        , m_subType(type)
        , m_keyCode(keyCode)
        , m_scanCode(scanCode)
        , m_keyStatus(keyStatus) {
    }

    virtual ~KeyEvent() { }

    KeyEventType subType() { return m_subType; }
    KeyCode keyCode() const { return m_keyCode; }
    int scanCode() const { return m_scanCode; }
    unsigned int keyStatus() const { return m_keyStatus; }

    void setKeyStatus (unsigned int keyStatus) {
        m_keyStatus = keyStatus;
    }

private:
    KeyEventType m_subType;
    KeyCode m_keyCode;
    int m_scanCode;
    unsigned int m_keyStatus;
};

class MouseEvent : public Event {
public:
    enum MouseEventType {
        MOUSE_MOVE_IN,
        MOUSE_MOVE,
        MOUSE_MOVE_OUT,
        MOUSE_L_DOWN,
        MOUSE_L_UP,
        MOUSE_L_CLICKED,
        MOUSE_L_DBLCLICKED,
        MOUSE_M_DOWN,
        MOUSE_M_UP,
        MOUSE_M_CLICKED,
        MOUSE_M_DBLCLICKED,
        MOUSE_R_DOWN,
        MOUSE_R_UP,
        MOUSE_R_CLICKED,
        MOUSE_R_DBLCLICKED,
    };

    MouseEvent(MouseEventType type, int x, int y, unsigned int keyStatus = 0)
        : Event(ET_MOUSE)
        , m_subType(type)
        , m_x(x)
        , m_y(y)
        , m_keyStatus(keyStatus) {
    }

    virtual ~MouseEvent() {
    }

    MouseEventType subType() const { return m_subType; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    unsigned int keyStatus() const { return m_keyStatus; }

private:
    MouseEventType m_subType;
    int m_x, m_y;
    unsigned int m_keyStatus;
};

class MouseWheelEvent : public Event {
public:
    MouseWheelEvent(int delta, int x, int y, unsigned int keyStatus = 0)
        : Event(ET_MOUSE_WHEEL)
        , m_delta(delta)
        , m_x(x)
        , m_y(y)
        , m_keyStatus(keyStatus) {
    }

    virtual ~MouseWheelEvent() { }

    int delta() const { return m_delta; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    unsigned int keyStatus() const { return m_keyStatus; }

private:
    int m_delta;
    int m_x, m_y;
    unsigned int m_keyStatus;
};

class TimerEvent : public Event {
public:
    TimerEvent(intptr_t timerId)
        : Event(ET_TIMER)
        , m_timerId(timerId) {
    }

    virtual ~TimerEvent() { }

    intptr_t timerID() const { return m_timerId; }

private:
    intptr_t m_timerId;
};

class Window;

class WindowEvent : public Event {
public:
    enum WindowEventCode {
        WND_GOTFOCUS,
        WND_LOSTFOCUS,
        WND_SHOWN,
        WND_HIDDEN,
        WND_ENABLED,
        WND_DISABLED,
        WND_SIZECHANGED,
        WND_MAXIMIZED,
        WND_MINIMIZED,
        WND_RESTORED,
    };

    WindowEvent(WindowEventCode code, const Window* window,
            const Window* other = NULL)
        : Event(ET_WINDOW)
        , m_code(code)
        , m_window(window)
        , m_other(other) {
    }
    virtual ~WindowEvent() { }

    WindowEventCode code() const { return m_code; }
    const Window* window() const { return m_window; }
    const Window* other() const { return m_other; }

private:
    WindowEventCode m_code;
    const Window* m_window;
    const Window* m_other;
};

class View;

class ViewEvent : public Event {
public:
    ViewEvent(int code, const View* view)
        : Event(ET_VIEW)
        , m_code(code)
        , m_view(view) {
    }
    virtual ~ViewEvent() { }

    int code() const { return m_code; }
    const View* view() const { return m_view; }

private:
    int m_code;
    const View* m_view;
};

class UserEvent : public Event {
public:
    UserEvent(int type, HTData param, HTData exParam = 0)
        : Event(ET_USER)
        , m_type(type)
        , m_param(param)
        , m_exParam(exParam) {
    }

    virtual ~UserEvent() { }

    void setParams(HTData param, HTData exParam) {
        m_param = param;
        m_exParam = exParam;
    }

    HTData param() const { return m_param; }
    HTData exParam() const { return m_exParam; }

private:
    int m_type;
    HTData m_param;
    HTData m_exParam;
};

class EventListener : public RefCount {
public:
    EventListener() : RefCount(0) { }

    virtual bool handler(Event* event) = 0;

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

