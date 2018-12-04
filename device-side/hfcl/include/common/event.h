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

//return True if the event was handled, false otherwise.
#define EVENT_HANDLED      1
#define EVENT_NOT_HANDLED  0

#include "common/common.h"
#include "common/alternativestl.h"
#include "common/object.h"

#define DISPATCH_CONTINUE_MSG  false
#define DISPATCH_STOP_MSG      true

namespace hfcl {

class Event {
public:
    enum EventType {
        KEY_DOWN, 
        KEY_UP, 
        KEY_LONGPRESSED, 
        KEY_ALWAYSPRESS,
        KEY_CHAR,
        MOTION_DOWN,
        MOTION_UP,
        MOTION_MOVE,
        MOTION_MOVEIN,
        MOTION_MOVEOUT,
        MOTION_CLICK,
        TIMER,
        CUSTOM_NOTIFY,
    };

    Event (EventType type, Object* eventSource = NULL)
        : m_eventType (type)
        , m_eventSource (eventSource) {
    }

    virtual ~Event() {
    }

    inline Object* getSource() const {
        return m_eventSource;
    }

    EventType eventType() {
        return m_eventType;
    }

private:
    EventType m_eventType;
    Object* m_eventSource;
};

class CustomEvent : public Event {
public:
    enum CustomParam{
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

    inline HTData customWparam () const {
        return m_wParam;
    }

    inline HTData customLparam () const {
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
        KEYCODE_0 = 0,
        KEYCODE_1,
        KEYCODE_2,
        KEYCODE_3,
        KEYCODE_4,
        KEYCODE_5,
        KEYCODE_6,
        KEYCODE_7,
        KEYCODE_8,
        KEYCODE_9,

        KEYCODE_SPACE,

        KEYCODE_F1,
        KEYCODE_F2,
        KEYCODE_F3,
        KEYCODE_F4,
        KEYCODE_F5,
        KEYCODE_F6,
        KEYCODE_F7,
        KEYCODE_F8,
        KEYCODE_F9,

        KEYCODE_ESCAPE,

        KEYCODE_SOFTKEY_LEFT,
        KEYCODE_SOFTKEY_CENTER,
        KEYCODE_SOFTKEY_RIGHT,
        KEYCODE_START,
        KEYCODE_PAUSE,
        KEYCODE_STOP,

        KEYCODE_CURSOR_LEFT,
        KEYCODE_CURSOR_UP,
        KEYCODE_CURSOR_RIGHT,
        KEYCODE_CURSOR_DOWN,

        KEYCODE_HOME,
        KEYCODE_END,
        KEYCODE_PAGE_UP,
        KEYCODE_PAGE_DOWN,

        KEYCODE_VOL_UP,
        KEYCODE_VOL_DOWN,

        KEYCODE_ENTER,
        KEYCODE_POWER,

        KEYCODE_BACKSPACE,
        KEYCODE_DELETE,

        MAX_SYS_KEYCODE = 255,
    };

    KeyEvent (EventType type, int keyCode, unsigned int keyStatus)
        : Event (type)
        , m_keyCode (keyCode)
        , m_keyStatus (keyStatus) {
    }

    virtual ~KeyEvent() {
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
    EventListener() : RefCount (0) {
    }

    virtual bool handleEvent(Event* event) = 0;

protected:
    EventListener(int start_ref) : RefCount(start_ref) {
    }
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
