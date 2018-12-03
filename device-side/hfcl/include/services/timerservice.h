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

#ifndef _HFCL_TimerService_h
#define _HFCL_TimerService_h

#include "common/common.h"
#include "mgcl/mgcl.h"
#include "common/log.h"
#include "service.h"
#include "common/event.h"

typedef enum {
    TIMER_PRIORITY_LOW       = 1,
    TIMER_PRIORITY_MIDDLE    = 2,
    TIMER_PRIORITY_HIGH      = 3,   
    TIMER_PRIORITY_QTY
} TimerPriority;

namespace hfcl {

#define SERVICE_TIMER_ID 		0xEF
#define SERVICE_TIMER_INTERVAL 	10   // 16384=1S 1638=100MS


class TimerEventListener;


class TimerService : public Service 
{
    private:
        TimerService();
    private:
        BOOL m_bPause;
       
    public:
        static inline TimerService* getInstance() {
            static  TimerService* m_singleton = NULL;
            if (NULL == m_singleton)
                m_singleton = HFCL_NEW_EX(TimerService, ());
            return m_singleton;
        }
        ~TimerService();

        bool canStop(void);
        void start(void);
        void stop(void);
        void pause();
        void resume();

        int addTimerListenerPriority (int interval, TimerEventListener* listener, char *listenername) {
            return addTimerListener (interval, listener, TIMER_PRIORITY_LOW, listenername);
        }

        int addTimerListener (int interval, TimerEventListener* listener, TimerPriority priority, const char *listenername);
        int addTimerListenerSingle (int interval, TimerEventListener* listener, TimerPriority priority, const char *listenername);
        void removeTimerListener (TimerEventListener* listener, int id);

#ifdef USE_PLATFORM_TIMER /* VincentWei: Always use MiniGUI's timer API */
        UINT32 queryTimer (int id);
        BOOL TimerProc (unsigned int ths, int id, DWORD data);
#else
    private:
        static BOOL TimerProc (HWND listener, int id, DWORD data);
        static BOOL OneShotTimerProc (HWND listener, int id, DWORD data);
#endif
};

#define LISTENER_TIMER_MAX 16

class TimerEventListener : public EventListener 
{
	protected:
		TimerEventListener(int start_ref) : EventListener(start_ref) { 
			memset(m_timerIdArray, 0, LISTENER_TIMER_MAX * sizeof(int)); 
		}

	public :
		TimerEventListener() : EventListener() {
			memset(m_timerIdArray, 0, LISTENER_TIMER_MAX * sizeof(int));
		}

		virtual ~TimerEventListener() {
			for (int i = 0; i < LISTENER_TIMER_MAX; i++) {
				if (m_timerIdArray[i] > 0) {
					TimerService::getInstance()->removeTimerListener(this, m_timerIdArray[i]);
					m_timerIdArray[i] = 0;
				}
			}
		}
		int registerTimer(int interval, const char *listenername, TimerPriority priority = TIMER_PRIORITY_LOW) {
			for (int i = 0; i < LISTENER_TIMER_MAX; i++) {
				if (m_timerIdArray[i] == 0) {
					m_timerIdArray[i] = TimerService::getInstance()->addTimerListener(interval, this, priority, listenername);
					return m_timerIdArray[i];
				}
			}
            return 0;
		}
        
        int registerTimerSingle(int interval, const char *listenername, TimerPriority priority = TIMER_PRIORITY_LOW)
        {
            return TimerService::getInstance()->addTimerListenerSingle(interval, this, priority, listenername);
        }

		void removeTimer(int timerId) {
			TimerService::getInstance()->removeTimerListener(this, timerId);
			for (int i = 0; i < LISTENER_TIMER_MAX; i++) {
				if (m_timerIdArray[i] == timerId) {
					m_timerIdArray[i] = 0;
				}
			}
		}
	private :
		int m_timerIdArray[LISTENER_TIMER_MAX];
};


} // namespace hfcl

#endif //_HFCL_TimerService_h

