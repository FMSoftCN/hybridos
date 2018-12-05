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

namespace hfcl {

TimerService::TimerService()
{
}

TimerService::~TimerService()
{
    stop();   
}

BOOL TimerService::TimerProc (HWND listener, LINT id, DWORD data)
{
	Event::EventType eventType = Event::TIMER;
	TimerEvent event(eventType, id);

    if(listener != 0)
        ((TimerEventListener *)(listener))->handleEvent(&event);

    return TRUE;
}

BOOL TimerService::OneShotTimerProc (HWND listener, LINT id, DWORD data)
{
	Event::EventType eventType = Event::TIMER;
	TimerEvent event(eventType, id);

    if(listener != 0)
        ((TimerEventListener *)(listener))->handleEvent(&event);

    return FALSE;
}

int TimerService::addTimerListener (int interval, TimerEventListener* listener,
        TimerPriority priority, const char *listenername)
{
    if (interval <= 0)
        return 0;

    if (NULL != listener) {
#ifdef __TIMER_DEBUG__
        _DBG_PRINTF ("TimerService::addTimerListener: %p, %p, %s\n",
                this, listener, listenername);
#endif
        LINT id = (LINT)listenername;
	    if (SetTimerEx ((HWND)listener, id, interval/10,
                TimerService::TimerProc)) {
            return id;
        }
        else {
            _ERR_PRINTF ("TimerService::addTimerListener: Failed to call SetTimerEx SOS\n");
        }
    }

    return 0;
}

int TimerService::addTimerListenerSingle(int interval,
        TimerEventListener* listener,
        TimerPriority priority, const char *listenername)
{
    if (interval <= 0)
        return 0;

    if (NULL != listener) {
#ifdef __TIMER_DEBUG__
        _DBG_PRINTF ("TimerService::addTimerListener: %p, %p, %s\n",
                this, listener, listenername);
#endif
        LINT id = (LINT)listenername;
	    if (SetTimerEx ((HWND)listener, id, interval/10,
                TimerService::OneShotTimerProc))
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

} // namespace hfcl

