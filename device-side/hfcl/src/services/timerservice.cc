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

#include "services/timerservice.h"

#include "lcdservice.h"

#include "hal_misc.h"
#include "hal_task.h"
#include "cos.h"

//#define __TIMER_DEBUG__

namespace hfcl {

#ifdef USE_PLATFORM_TIMER /* VincentWei: Always use MiniGUI's timer API */

#define TIMER_LISTENER_NR       128
#define LISTENER_TIMER_ID_BASE (COS_MMI_TIMER_ID_BASE+1)

static int g_timerMask = 0x000003FF;

typedef struct _LISTENER_TIMERTABLE_T
{
	TimerEventListener 		*timerListener;
	unsigned int			TimeDuration;
	unsigned short			timer_id;
    unsigned char          	TimerModle;        
	unsigned char          	priority;
	string   				listenername;
	bool    				bePause;
} LISTENER_TIMERTABLE_T;

static LISTENER_TIMERTABLE_T g_timerListenerTable[TIMER_LISTENER_NR];

static int g_prevTimerSlotId = -1;

/* this function should be called by target platform in the event loop. */
extern "C" int DispatchListenerTimerEvent(int timer_id)
{
    return TimerService::getInstance()->TimerProc(0, timer_id & g_timerMask, 0);
}

static inline unsigned short get_free_timer_id() 
{
    int index;

    for (int slot = 0; slot < TIMER_LISTENER_NR; slot++) 
    {
        index = (g_prevTimerSlotId+slot+1)%TIMER_LISTENER_NR;
        if (g_timerListenerTable[index].timerListener == NULL)
        {  
            g_prevTimerSlotId = index;
            return LISTENER_TIMER_ID_BASE + index;
        }
    }

    return 0;
}

#ifdef __TIMER_DEBUG__
static int get_timer_count()
{
	int _cnt = 0;
	
	for (int slot = 0; slot < TIMER_LISTENER_NR; slot++) {
		if (g_timerListenerTable[slot].timerListener != NULL)
		{
			_cnt++;
		}
	}
	return _cnt;
}
#endif
///////////////////////////////////////////////////////////////////////

TimerService::TimerService()
{
    for (int slot = 0; slot < TIMER_LISTENER_NR; slot++) 
    {
        g_timerListenerTable[slot].timerListener = NULL;
        g_timerListenerTable[slot].TimerModle 	 = COS_TIMER_MODE_SINGLE;	   
		g_timerListenerTable[slot].priority 	 = TIMER_PRIORITY_LOW;	   
		g_timerListenerTable[slot].listenername  = " ";
		g_timerListenerTable[slot].bePause 	  	 = false;
		g_timerListenerTable[slot].timer_id = 0;
    }

	m_bPause = false;
}

TimerService::~TimerService()
{
    stop();   
}

BOOL TimerService::TimerProc(unsigned int ths, int timer_id, DWORD data)
{
    if (timer_id < LISTENER_TIMER_ID_BASE || timer_id > LISTENER_TIMER_ID_BASE + TIMER_LISTENER_NR) 
        return FALSE;

    if(g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener != NULL) 
    {
		if(m_bPause && (g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority == TIMER_PRIORITY_LOW)) 
		{
		    if(g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimerModle != COS_TIMER_MODE_SINGLE) 
		    {
				COS_KillTimerEX(getMMITask(), timer_id);
				g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].bePause = true;
		    }	
		}
		else
		{
			TimerEvent event(Event::TIMER, timer_id);
			g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener->handleEvent(&event);
			//hal_HstSendEvent((UINT32)(g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener) );
		}
    }
    else
    {
        COS_KillTimerEX(getMMITask(),  timer_id); //fixed by xulei 20130701
    }

    if(g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimerModle == COS_TIMER_MODE_SINGLE) 
    {
        g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener  = NULL;
        COS_KillTimerEX(getMMITask(), timer_id);
    }

    return TRUE;
}

int TimerService::addTimerListener(int interval, TimerEventListener* listener, TimerPriority priority, const char *listenername)
{
    if(interval > 0 && NULL != listener) 
    {
        unsigned short timer_id = get_free_timer_id();

        if(timer_id  <= 0 ) return 0; 
		
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener 	= listener;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimerModle 		= COS_TIMER_MODE_PERIODIC;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timer_id 		= timer_id;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimeDuration 	= interval;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority 		= priority;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].listenername  	= (const char*)listenername;

		if(m_bPause && (g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority == TIMER_PRIORITY_LOW)) 
		{
			g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].bePause = true;
#ifdef __TIMER_DEBUG__
			_DBG_PRINTF ("TimeService :: add [periodic] timer [%d] <%s> ---- count = %d  [succeed / pause]", timer_id, listenername, get_timer_count());
#endif
			return timer_id;
		}
		else 
		{
            if(FALSE == COS_SetTimerEX(getMMITask(), timer_id, COS_TIMER_MODE_PERIODIC, interval*16.3)) {
#ifdef __TIMER_DEBUG__
				_DBG_PRINTF ("TimeService :: add [periodic] timer [%d] <%s> ---- count = %d  [failure]", timer_id, listenername, get_timer_count());
#endif
                return  0;
            }
#ifdef __TIMER_DEBUG__
			_DBG_PRINTF ("TimeService :: add [periodic] timer [%d] <%s> ---- count = %d  [succeed]", timer_id, listenername, get_timer_count());
#endif
            return timer_id;  
        }
    }
    return 0;
}

int TimerService::addTimerListenerSingle(int interval, TimerEventListener* listener, TimerPriority priority,const char *listenername)
{
	if(interval > 0 && NULL != listener) 
	{
		unsigned short timer_id = get_free_timer_id();

		if(timer_id  <= 0 ) return 0; 
		
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener 	= listener;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimerModle 		= COS_TIMER_MODE_SINGLE;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timer_id 		= timer_id;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimeDuration 	= interval;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority 		= priority;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].listenername 	= (const char*)listenername;

		if(m_bPause&& (g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority == TIMER_PRIORITY_LOW)) 
		{
			g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].bePause = true;
#ifdef __TIMER_DEBUG__
			_DBG_PRINTF ("TimeService :: add [single] timer [%d] <%s> ---- count = %d  [succeed/pause]", timer_id, listenername, get_timer_count());
#endif
			return timer_id;
		}
		else 
		{
			if(FALSE == COS_SetTimerEX(getMMITask(), timer_id, COS_TIMER_MODE_SINGLE, interval*16.3)) {
#ifdef __TIMER_DEBUG__
				_DBG_PRINTF ("TimeService :: add [single] timer [%d] <%s> ---- count = %d  [failure]", timer_id, listenername, get_timer_count());
#endif
				return 0;
			}
#ifdef __TIMER_DEBUG__
			_DBG_PRINTF ("TimeService :: add [single] timer [%d] <%s> ---- count = %d  [succeed]", timer_id, listenername, get_timer_count());
#endif
		 	return timer_id; 
	 	}
    }
    return 0;
}

// we need to convert 'tick' to ms here
// so unit is MS
UINT32 TimerService::queryTimer(int timer_id)
{
	UINT32 tick  = 0;
    if(timer_id >= LISTENER_TIMER_ID_BASE) 
    {
    	tick = COS_QueryTimerEX(getMMITask(), (UINT16)timer_id);

		return (tick / 16.3);
    }

	return 0;
}

void TimerService::removeTimerListener(int timer_id)
{
    if(timer_id >= LISTENER_TIMER_ID_BASE) 
    {
     	COS_KillTimerEX(getMMITask(), (UINT16)timer_id);
		
#ifdef __TIMER_DEBUG__
		_DBG_PRINTF ("TimeService :: remove timer [%d] <%s>---- count = %d ", timer_id, 
			g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].listenername.c_str(), get_timer_count());
#endif

		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timerListener 	= NULL;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].TimerModle 	= COS_TIMER_MODE_SINGLE;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].priority              = TIMER_PRIORITY_LOW;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].listenername 	= " " ;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].bePause 		= false;
		g_timerListenerTable[timer_id - LISTENER_TIMER_ID_BASE].timer_id  = 0;
    }
}

void TimerService::start(void)
{
}

void TimerService::stop(void)
{
}

bool TimerService::canStop(void)
{
    return true;
}

void TimerService::pause()
{
	m_bPause = true;
#ifdef __TIMER_DEBUG__
	_DBG_PRINTF ("TimeService :: pause ---- ");
#endif
}

void TimerService::resume()
{
	m_bPause = false;
	
	for (int slot = 0; slot < TIMER_LISTENER_NR; slot++) 
	{
		if(g_timerListenerTable[slot].bePause && (NULL != g_timerListenerTable[slot].timerListener) ) 
		{
		   COS_SetTimerEX(getMMITask(), 
				g_timerListenerTable[slot].timer_id, 
				g_timerListenerTable[slot].TimerModle,
				g_timerListenerTable[slot].TimeDuration*16.3);

		   g_timerListenerTable[slot].bePause = false;
		}
	}
#ifdef __TIMER_DEBUG__
	_DBG_PRINTF ("TimeService :: resume ---- ");
#endif
}

#else /* USE_PLATFORM_TIMER */

///////////////////////////////////////////////////////////////////////

TimerService::TimerService()
{
}

TimerService::~TimerService()
{
    stop();   
}

BOOL TimerService::TimerProc (HWND listener, int id, DWORD data)
{
	Event::EventType eventType = Event::TIMER;
	TimerEvent event(eventType, id);

    if(listener != 0)
        ((TimerEventListener *)(listener))->handleEvent(&event);

    return TRUE;
}

BOOL TimerService::OneShotTimerProc (HWND listener, int id, DWORD data)
{
	Event::EventType eventType = Event::TIMER;
	TimerEvent event(eventType, id);

    if(listener != 0)
        ((TimerEventListener *)(listener))->handleEvent(&event);

    return FALSE;
}

int TimerService::addTimerListener (int interval, TimerEventListener* listener, TimerPriority priority, const char *listenername)
{
    if (interval <= 0)
        return 0;

    if (NULL != listener) {
#ifdef __TIMER_DEBUG__
        _DBG_PRINTF ("TimerService::addTimerListener: %p, %p, %s\n", this, listener, listenername);
#endif
        int id = (int)listenername;
	    if (SetTimerEx ((HWND)listener, id, interval/10, TimerService::TimerProc)) {
            return id;
        }
        else {
            _ERR_PRINTF ("TimerService::addTimerListener: Failed to call SetTimerEx SOS\n");
        }
    }

    return 0;
}

int TimerService::addTimerListenerSingle(int interval, TimerEventListener* listener, TimerPriority priority, const char *listenername)
{
    if (interval <= 0)
        return 0;

    if (NULL != listener) {
#ifdef __TIMER_DEBUG__
        _DBG_PRINTF ("TimerService::addTimerListener: %p, %p, %s\n", this, listener, listenername);
#endif
        int id = (int)listenername;
	    if (SetTimerEx ((HWND)listener, id, interval/10, TimerService::OneShotTimerProc))
            return id;
        else {
            _ERR_PRINTF ("TimerService::addTimerListenerSingle: Failed to call SetTimerEx SOS\n");
        }
    }

    return 0;
}

void TimerService::removeTimerListener(TimerEventListener* listener, int id)
{
    if (id != 0)
	    KillTimer((HWND)listener, id);
}

void TimerService::pause()
{
	//TODO
}

void TimerService::resume()
{
	//TODO
}

void TimerService::start(void)
{
	//TODO
}

void TimerService::stop(void)
{
	//TODO
}

bool TimerService::canStop(void)
{
    return true;
}

#endif /* !USE_PLATFORM_TIMER */

} // namespace hfcl
