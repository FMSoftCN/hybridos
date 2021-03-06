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

#include "bootuplogo.h"

#include <hfcl/common.h>

#include "bootup-res.h"
#include "bootupactivity.h"

using namespace hfcl;

#define HRESNAME(pkg, type, name) R_##pkg##_##type##_##name

BootupLogo::BootupLogo(Controller* owner, int view_id, Window* window,
        HTData param1, HTData param2)
    : ControllerClient(owner, view_id, window)
    , m_animateview(NULL)
{
    View* view = CreateViewFromRes (HRESNAME(bootup, client, logo),
            m_rootView, this, NULL);

    m_animateview->setGifAnimate (
            GetGifAnimateRes (HRESNAME (bootup, img, animation_logo)));

    m_timer_id = 0;
}

BootupLogo::~BootupLogo()
{
    if (m_timer_id != 0) {
        removeTimer(m_timer_id);
    }
}

bool BootupLogo::onGifAnimateNotify(ViewEvent* event)
{
    if (event->code() != AnimatedImageView::NOTIFY_GIFANIMATE_STOP)
        return false;

    // TODO: show greeting words
    return true;
}

bool BootupLogo::handler(Event* event)
{
    if (event == NULL) {
        _WRN_PRINTF("event is NULL");
        return STOP_DISPATCH;
    }

    if (event->eventType() == Event::ET_TIMER
            && m_timer_id == ((TimerEvent *)event)->timerID()) {

        if (((BootupActivity *)m_owner)->getTop() == NULL)
            return false;

        if (m_timer_id != 0) {
            removeTimer(m_timer_id);
            m_timer_id = 0;
        }
    }

    return GOON_DISPATCH;
}

BEGIN_SETVIEW(BootupLogo)
    MAP_VIEW(m_animateview, HRESNAME (bootup, var, view_animation_logo), AnimatedImageView)
END_SETVIEW

BEGIN_GETHANDLE(BootupLogo)
    MAP_HANDLE(BootupLogo, HRESNAME (bootup, var, event_animation_logo), onGifAnimateNotify)
END_GETHANDLE

