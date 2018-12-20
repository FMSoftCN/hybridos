/*
** HFCL Samples - Samples for HybridOS Foundation Class Library
**
** Copyright (C) 2018 Beijing FMSoft Technologies Co., Ltd.
**
** This file is part of HFCL Samples.
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

#include "bootupgreet.h"

#include "bootupactivity.h"

using namespace hfcl;

BootupGreet::BootupGreet(Controller* owner, int view_id, View* parent,
        HTData param1, HTData param2) : ControllerClient(owner, view_id, parent)
{
}

BootupGreet::~BootupGreet()
{
}

bool BootupGreet::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER
            && m_timer_id == ((TimerEvent *)event)->timerID()) {

        if (((BootupActivity *)m_owner)->getTop() == NULL)
            return false;

        if (m_timer_id != 0) {
            removeTimer(m_timer_id);
            m_timer_id = 0;
        }
    }

    return DISPATCH_CONTINUE_MSG;
}

BEGIN_SETVIEW(BootupGreet)
END_SETVIEW

BEGIN_GETHANDLE(BootupGreet)
END_GETHANDLE

