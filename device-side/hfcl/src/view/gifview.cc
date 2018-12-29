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

#include "view/gifview.h"

#include "graphics/graphicscontext.h"
#include "graphics/gifanimate.h"
#include "services/timerservice.h"

namespace hfcl {

GifView::GifView(const char* cssClass, const char* name, int id)
    : View(cssClass, name, id)
    , m_elapsed_10ms(0)
{
    m_animate = NULL;
    m_timer_id = 0;
    m_state = Stop;
    m_loopType = NoLoop;
    m_playType = AutoPlay;
}

GifView::~GifView()
{
    stop();
    if (m_timer_id != 0)
        removeTimer(m_timer_id);
    m_timer_id = 0;

    if (NULL != m_animate){
        HFCL_DELETE(m_animate);
        m_animate = NULL;
    }
}

bool GifView::start(void)
{
    if (NULL == m_animate)
        return false;

    if (Play == m_state)
        return false;

    m_timer_id = registerTimer(10, "GifView");

    if (m_timer_id == 0)
        return false;

    m_state = Play;
    m_start_ticks = GetTickCount();
    return true;
}

bool GifView::pause(void)
{
    if (Play != m_state)
        return false;

    if (m_timer_id != 0)
    {
        removeTimer(m_timer_id);
        m_timer_id = 0;
    }

    m_state = Pause;
    return true;
}

bool GifView::resume(void)
{
    if (NULL == m_animate)
        return false;

    if (Play == m_state)
        return false;

    if (m_timer_id != 0)
    {
        removeTimer(m_timer_id);
    }
    m_timer_id = registerTimer(10, "GifView");

    if (m_timer_id == 0)
        return false;

    m_state = Play;
    m_start_ticks = GetTickCount();
    return true;
}

bool GifView::stop(void)
{
    if (m_timer_id != 0)
    {
        removeTimer(m_timer_id);
        m_timer_id = 0;
    }
    if(m_state == Stop) return false;
    m_state = Stop;

    if(m_animate== NULL) return false;
    m_animate->firstFrame();
    // stop notity event
    ViewEvent event(NOTIFY_GIFANIMATE_STOP, this);
    raiseViewEvent(&event);

    return true;
}

bool GifView::reset(void)
{
    if (NULL == m_animate)
        return false;

    if (!stop())
        return false;

    m_animate->firstFrame();

    return start();
}


void GifView::setGifFile(const char* animateFile)
{
    if (NULL != m_animate) {
        HFCL_DELETE(m_animate);
        m_animate = NULL;
    }

    if (NULL == animateFile)
        return;

    m_animate = HFCL_NEW_EX(GifAnimate, ());

    if (NULL != m_animate) {
        m_animate->createGifAnimateFromFile(animateFile);
    }

    if (NULL != m_animate
            && m_animate->frameCount() > 0
            && AutoPlay == m_playType) {
        start();
    }
}

void GifView::setGifAnimate(GifAnimate* animate)
{
    if (NULL != m_animate && m_animate != animate) {
        HFCL_DELETE(m_animate);
        m_animate = NULL;
    }

    m_animate = animate;

    if (NULL != m_animate
            && m_animate->frameCount() > 0
            && AutoPlay == m_playType) {
        start();
    }
}

void GifView::onPaint(GraphicsContext* context)
{
    IntRect rc = getRect();
    RECT mapRc(rc);
    if (NULL == m_animate) {
        return;
    }

    context->mapRect(mapRc);

    m_animate->drawOneFrame(context, mapRc);
}

bool GifView::handleEvent(Event* event)
{
    if (NULL == m_animate)
        return GOON_DISPATCH;

    // check state
    if (Stop == m_state)
        return STOP_DISPATCH;

    if (event->eventType() == Event::ET_TIMER
            && m_timer_id == ((TimerEvent *)event)->timerID()) {
        if(m_animate->frameCount() == 1) {
            updateView(false);
            stop();
        }
        else {
            if (m_elapsed_10ms >= m_animate->currentFrameDelay()) {
                if (!(m_animate->isLastFrame()) || Loop == m_loopType) {
                    if((Loop == m_loopType)&&(m_animate->isLastFrame())) {
                         m_animate->firstFrame();
                    }
                    else {
                        m_animate->nextFrame();
                    }
                    m_elapsed_10ms = 0;
                    m_start_ticks = GetTickCount();
                    updateView(false);
                }
                else {
                    // last frame and not loop
                    stop();
                }
            }
            else {
                Sint32 cur_tick = GetTickCount();
                m_elapsed_10ms = (Uint32) (cur_tick - m_start_ticks)*10;
            }
        }
    }

    return GOON_DISPATCH;
}

} // namespace hfcl
