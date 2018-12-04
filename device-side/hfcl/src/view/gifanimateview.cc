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

#include "view/gifanimateview.h"

#include "graphics/graphicscontext.h"
#include "graphics/gifanimate.h"
#include "services/timerservice.h"

#if 0
#include "common/common.h"
#include "mgcl/mgcl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#endif

namespace hfcl {

GifAnimateView::GifAnimateView()
	:View() ,m_elapsed_10ms(0)
{
    m_animate = NULL;
    m_timer_id = 0;
    m_state = Stop;
    m_loopType = NoLoop;
    m_playType = AutoPlay;
    m_drawLayer = (LAYER_MAXNUM - 1);
}

GifAnimateView::GifAnimateView(View* p_parent)
	:View(p_parent)
{
    m_animate = NULL;
    m_timer_id = 0;
    m_state = Stop;
    m_loopType = NoLoop;
    m_playType = AutoPlay;
    m_drawLayer = (LAYER_MAXNUM - 1);
}

GifAnimateView::GifAnimateView(View* p_parent, DrawableSet* drset)
	:View(p_parent, drset)
{
    m_animate = NULL;
    m_timer_id = 0;
    m_state = Stop;
    m_loopType = NoLoop;
    m_playType = AutoPlay;
    m_drawLayer = (LAYER_MAXNUM - 1);
}

GifAnimateView::GifAnimateView(int i_id, int x, int y, int w, int h)
	:View(i_id, x, y, w, h)
{
    m_animate = NULL;
    m_timer_id = 0;
    m_state = Stop;
    m_loopType = NoLoop;
    m_playType = AutoPlay;
    m_drawLayer = (LAYER_MAXNUM - 1);
}

GifAnimateView::~GifAnimateView()
{
	stop();
    if (m_timer_id != 0)
	    removeTimer(m_timer_id);
	m_timer_id = 0;

    if (NULL != m_animate){
	#ifdef _HFCL_INCORE_BMPDATA
		 m_animate->~GifAnimate();
    #else
        HFCL_DELETE(m_animate);
		m_animate = NULL;
    #endif
	}
}

bool GifAnimateView::start(void)
{
    if (NULL == m_animate)
        return false;

    if (Play == m_state)
        return false;

    m_timer_id = registerTimer(10, "GifAnimateView");

    if (m_timer_id == 0)
        return false;

    m_state = Play;
    m_start_ticks = GetTickCount();
    return true;
}

bool GifAnimateView::pause(void)
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

bool GifAnimateView::resume(void)
{
    if (NULL == m_animate)
        return false;

    if (Play == m_state)
        return false;

    if (m_timer_id != 0)
    {
        removeTimer(m_timer_id);
    }
    m_timer_id = registerTimer(10, "GifAnimateView");

    if (m_timer_id == 0)
        return false;

    m_state = Play;
    m_start_ticks = GetTickCount();
    return true;
}

bool GifAnimateView::stop(void)
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
	CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)NOTIFY_GIFANIMATE_STOP, (HTData)this);
	raiseEvent(&event);

    return true;
}

bool GifAnimateView::reset(void)
{
    if (NULL == m_animate)
        return false;

    if (!stop())
        return false;

    m_animate->firstFrame();

    return start();
}


void GifAnimateView::setGifFile(const char* animateFile)
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

void GifAnimateView::setGifAnimate(GifAnimate* animate)
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

void GifAnimateView::onPaint(GraphicsContext* context, int status)
{
    IntRect rc = getRect();
    RECT mapRc(rc);    
    if (NULL == m_animate) { 
        return;
    }

    context->setLayer(m_drawLayer);
    context->mapRect(mapRc);
        
    m_animate->drawOneFrame(context, mapRc);
}

bool GifAnimateView::handleEvent(Event* event)
{
    if (NULL == m_animate)
        return DISPATCH_CONTINUE_MSG;

    // check state
    if (Stop == m_state)
        return DISPATCH_STOP_MSG;

    if (event->eventType() == Event::TIMER
			&& m_timer_id == ((TimerEvent *)event)->timerID())
	{
		if(m_animate->frameCount() == 1)
		{
			updateView(false);
			stop();
		}
		else
		{
			if (m_elapsed_10ms >= m_animate->currentFrameDelay())
			{
				if (!(m_animate->isLastFrame()) || Loop == m_loopType)
				{
#ifdef _HFCL_INCORE_BMPDATA
	                if((Loop == m_loopType)&&(m_animate->isLastFrame()))
					{
					     m_animate->firstFrame();
					}
	                else
#endif
					{
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
	            m_elapsed_10ms = (Uint64) (cur_tick - m_start_ticks)*10;
			}
		}
	}

    return DISPATCH_CONTINUE_MSG;
}

bool GifAnimateView::dispatchEvent(Event* event)
{
    return View::dispatchEvent(event);
}

} // namespace hfcl
