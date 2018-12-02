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

#ifndef _NGUX_NguxEventh
#define _NGUX_NguxEventh
//return True if the event was handled, false otherwise.
#define EVENT_HANDLED      1
#define EVENT_NOT_HANDLED  0

#include "nguxobject.h"
#include "hal_misc.h"

NAMESPACE_BEGIN

#define DISPATCH_CONTINUE_MSG  false
#define DISPATCH_STOP_MSG      true

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
            MEDIA,
            CUSTOM_NOTIFY,
            PROTOCOL_EVENT,
            CHARGEEARPHONE_EVENT,
        };

        Event(EventType type)
        :m_eventType(type)
        ,m_eventSource(NULL)
        {
        }
        virtual ~Event() { }
        Object* getSource(){return m_eventSource;}
        void setSource(Object *source){m_eventSource = source;}
        EventType eventType() { return m_eventType; }

    private:
        EventType m_eventType;
        Object* m_eventSource;
};

class CustomEvent : public Event{

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
        CustomEvent(EventType type, int wparam, int lparam)
        :Event(type)
        ,m_customWparam(wparam)
        ,m_customLparam(lparam)
        {}
        virtual ~CustomEvent() { }
        int customWparam(){return m_customWparam;}
        int customLparam(){return m_customLparam;}
        int customExParam1(){return m_customExParam1;}
        int customExParam2(){return m_customExParam2;}
        void setExParam(int param1, int param2){m_customExParam1=param1;m_customExParam2=param2;}
    private:
        int m_customWparam;
        int m_customLparam;
        int m_customExParam1;
        int m_customExParam2;

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
            KEYCODE_SL,     // SCANCODE_F3: left soft key
            KEYCODE_SR,     // SCANCODE_F4: right soft key
            KEYCODE_CALL,   // SCANCODE_F5:
            KEYCODE_STOP,   // SCANCODE_F6:
            KEYCODE_CALL2,  // SCANCODE_F7:
            KEYCODE_CAMERA, // SCANCODE_F8:
            KEYCODE_FM,     // SCANCODE_F9:
            KEYCODE_SMS,    // SCANCODE_F10:
            KEYCODE_MUSIC,  // SCANCODE_F11:

            KEYCODE_HOME,   // SCANCODE_HOME
            KEYCODE_UP,     // SCANCODE_CURSORBLOCKUP
            KEYCODE_VOL_UP, // SCANCODE_PAGEUP
            KEYCODE_LEFT,   // SCANCODE_CURSORBLOCKLEFT
            KEYCODE_RIGHT,  // SCANCODE_CURSORBLOCKRIGHT
            KEYCODE_END,    // SCANCODE_END
            KEYCODE_DOWN,   // SCANCODE_CURSORBLOCKDOWN
            KEYCODE_VOL_DOWN,   // SCANCODE_PAGEDOWN

            KEYCODE_ENTER,  // SCANCODE_ENTER
            KEYCODE_POWER,  // SCANCODE_POWER

            MAX_KEYCODE,
            KEYCODE_INVALID = 0xFE
        };

        KeyEvent(EventType type, int scancode, unsigned int keystatus)
        :Event(type)
        ,m_keyStatus(keystatus)
        {
            if (scancode >= SCANCODE_1 && scancode <= SCANCODE_9) {
                m_keyCode = KEYCODE_1 + (scancode - SCANCODE_1);
            }
            else if (scancode >= SCANCODE_F1 && scancode <= SCANCODE_F10) {
                m_keyCode = KEYCODE_STAR + (scancode - SCANCODE_F1);
            }
            else if (scancode >= SCANCODE_HOME && scancode <= SCANCODE_PAGEDOWN) {
                m_keyCode = KEYCODE_HOME + (scancode - SCANCODE_HOME);
            }
            else {
                switch (scancode) {
                case SCANCODE_0:
                    m_keyCode = KEYCODE_0;
                    break;

                case SCANCODE_F11:
                    m_keyCode = KEYCODE_MUSIC;
                    break;

                case SCANCODE_ENTER:
                    m_keyCode = KEYCODE_ENTER;
                    break;

                case SCANCODE_POWER:
                    m_keyCode = KEYCODE_POWER;
                    break;

                default:
                    m_keyCode = KEYCODE_INVALID;
                    break;
                }
            }
        }

        int keyCode() {return m_keyCode;}
        virtual ~KeyEvent() { }
        inline int keyCode() const { return m_keyCode; }
        inline unsigned int keyStatus() const { return m_keyStatus; }
        inline void  setKeyStatus(unsigned int st){ m_keyStatus = st; }

    private:
        int m_keyCode;
        unsigned int m_keyStatus;

};

class MouseEvent : public Event {
    public:
        MouseEvent(EventType type, int ix, int iy)
        :Event(type)
         ,m_x(ix), m_y(iy)
        {

        }
        virtual ~MouseEvent() {}

        inline int x() const { return m_x; }
        inline int y() const { return m_y; }

    private:
        int m_x;
        int m_y;
};

class TimerEvent : public Event {
    public:
        TimerEvent(EventType type, int timerId)
        :Event(type)
         ,m_timerId(timerId)
        {
        }

        virtual ~TimerEvent() {}

        inline int timerID() const { return m_timerId; }

    private:
        int m_timerId;
};

class ChargeEarPhoneEvent : public Event {
    public:
        ChargeEarPhoneEvent(EventType type, int evevtId, int info)
        :Event(type)
         ,m_eventID(evevtId)
         ,m_info(info)
        {
        }

        virtual ~ChargeEarPhoneEvent() {}

        inline int eventID() const { return m_eventID; }
        inline int Info()const{return m_info;}

    private:
        int m_eventID;
        int m_info;
};


class MediaEvent : public Event {
    public:
        MediaEvent(EventType type, int istate, int idata)
        :Event(type)
        , m_state(istate)
        , m_data(idata)
        {
        }

        enum MediaState{
            OPEN,       // media device open.
            CLOSE,      // media device close.
            FINISHED,   // play finished.
            PAUSED,     // paused play.
            RESUME,     // resume play.
            PLAY,       // start play
            STOP,       // stop play
            BREAKED,
            PROGRESS,   // progress.
            TIME,
            RELOATION,  // seek
            OPENFAILED,
            OPENSUCCEED,
            DEVHOLD,   // dev hold state.
            DEVRESUME  // dev resume state.
        };

        virtual ~MediaEvent() {}
        inline int state() const {return m_state;}
        inline int data() const {return m_data;}

    private:
        int m_state;
        int m_data;
};

class ProtocolEvent : public Event {
    public: 
        ProtocolEvent()
             :Event(PROTOCOL_EVENT),
             eventId(-1),
             buffer(NULL),
             param1(NULL),
             param2(NULL),
             simId(0)
        {
        }

        virtual ~ProtocolEvent(){}

        // datas
        int eventId;
        void* buffer;
        void* param1;
        void* param2;
        int simId;
};

class EventListener : public RefCount 
{
    public:
        EventListener(): RefCount(0) { }
        virtual bool handleEvent(Event* event) = 0; 
    protected:
        EventListener(int start_ref) : RefCount(start_ref) { }
};

class EventBroadcaster
{
    public:
        EventBroadcaster() { m_current_raise = NULL; m_want_to_remove = NULL; }
        virtual ~EventBroadcaster() { releaseEventListeners(); }

    public:
        // apis
        virtual bool raiseEvent(Event* event);
        void addEventListener(EventListener* listener);
        void removeEventListener(EventListener* listener);

    protected:

        void releaseEventListeners();

        // notice : dont unref for these event listeners
        // becuase the add interface haven't add the reference
        // 
        LIST(EventListener *, EventListenerList);
        //LISTEX(EventListener *, EventListenerList, do{return *v1 == *v2;}while (0), do{(*n)->unref();} while (0));
        EventListenerList m_listeners;

        EventListener* m_current_raise;
        EventListener* m_want_to_remove;        
};


NAMESPACE_END

#endif /* NGUX_NguxEvent.h */
