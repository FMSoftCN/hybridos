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

#include "view/scrollbar.h"

#include <minigui/fixedmath.h>

#include "view/view.h"
#include "graphics/graphicscontext.h"
#include "services/timerservice.h"

#define DEF_SCROLL_MARGIN       1
#define DEF_SCROLL_WIDTH        4

namespace hfcl {

ScrollBar::ScrollBar(View *owner)
    : m_owner(NULL)
    , m_barWidth(DEF_SCROLL_WIDTH)
    , m_barMargin(DEF_SCROLL_MARGIN)
    , m_vStep(10)
    , m_hStep(10)
    , m_offx(0)
    , m_offy(0)
    , m_timerId(0)
{
    setBarVisible(true);
    setAutoShowBar(true);
    setScrollBarAlwaysShow(false);
    setScrollBarFloating(true);
    setVScrollBarOnLeft(true);
}

ScrollBar::~ScrollBar(void)
{
    if (m_timerId)
        removeTimer(m_timerId);
}

void ScrollBar::setBarWidth(int barwidth)
{
    int w, h;

    m_owner->getSize(&w, &h);

    if (barwidth >= 0 && barwidth <= MIN(w, h)) {
        m_barWidth = (Uint16)barwidth;
    }
}

void ScrollBar::setBarMargin(int barmargin)
{
    int w, h;

    m_owner->getSize (&w, &h);

    if (barmargin >= 0 && barmargin <= MIN(w, h)) {
        m_barMargin = (Uint16)barmargin;
    }
}

#define MIN_BAR  (5<<16)

bool ScrollBar::scrollVisible(int * barWidth, bool isVertical)
{
    fixed contw, conth;
    fixed vieww, viewh;
    IntRect rc;

    if((!vBarVisible() && isVertical) || (!hBarVisible() && !isVertical))
        return FALSE;

    if( !m_owner)
        return FALSE;

    if(barWidth)
        *barWidth = m_barWidth;

    rc = m_owner->getRect();
    vieww = itofix(rc.width());
    viewh = itofix(rc.height());
    contw = itofix(m_owner->getRect().width());
    conth = itofix(m_owner->getRect().height());

    if(isVertical && vBarVisible() && conth > viewh) {
        return TRUE;
    }
    else if(!isVertical && hBarVisible() && contw > vieww) {
        return TRUE;
    }

    return FALSE;
}

void ScrollBar::draw(GraphicsContext* context, IntRect &rc)
{
    fixed contw, conth;
    fixed vieww, viewh;

    int barWidth = m_barWidth;
    int barmargin = m_barMargin;

    bool isdraw = false;

    if (!vBarVisible() && !hBarVisible())
        return;

    if (!autoShowBar())
        return;

    if (!m_owner || loopScroll())
        return;

    if (barWidth <= 0)
        return;

    context->setMapView(m_owner);

    vieww = itofix(rc.width());
    viewh = itofix(rc.height());
    contw = itofix(m_owner->getRect().width());
    conth = itofix(m_owner->getRect().height());

    if (hBarVisible() && contw > vieww) {
        IntRect rcBar, rcBg;

        fixed bar_len = itofix(rc.width() * rc.width() / m_owner->getRect().width());
        fixed bar_pos = itofix(m_offx * rc.width() / m_owner->getRect().width());

        bar_len = bar_len < MIN_BAR ? MIN_BAR : bar_len;

        if (isScrollBarFloating()) {
            rcBar.m_left    = rc.left() + fixceil(bar_pos);
            rcBar.m_top     = rc.bottom() - barWidth;
            rcBar.m_right   = rcBar.left() + fixceil(bar_len);
            rcBar.m_bottom  = rcBar.top() + barWidth;

        } else {
            IntRect rcBg;

            rcBg.m_left     = rc.left();
            rcBg.m_top      = rc.bottom() - barWidth;
            rcBg.m_right    = rc.right();
            rcBg.m_bottom   = rc.bottom();

            //m_drset->draw(context, DR_BGHSCROLLBAR, DRAWSTATE_NORMAL, rcBg);

            rcBar.m_left    = rcBg.left() + fixceil(bar_pos);
            rcBar.m_top     = rcBg.top() + barmargin;
            rcBar.m_right   = rcBar.left() + fixceil(bar_len);
            rcBar.m_bottom  = rcBg.bottom() - barmargin;
        }

        //m_drset->draw(context, DR_HSCROLLBAR, DRAWSTATE_NORMAL, rcBar);

        isdraw = true;
    }

    if(vBarVisible() && conth > viewh) {
        IntRect rcBar, rcBg;

        fixed bar_len = itofix(rc.height() * rc.height() / m_owner->getRect().height());
        fixed bar_pos = itofix(m_offy * rc.height() / m_owner->getRect().height());

        bar_len = bar_len < MIN_BAR ? MIN_BAR : bar_len;

        if (isScrollBarFloating()) {
            rcBar.m_left     = isVScrollBarOnLeft() ? rc.left() : (rc.right() - barWidth);
            rcBar.m_top     = rc.top() + fixceil(bar_pos);
            rcBar.m_right     = rcBar.left() + barWidth;
            rcBar.m_bottom     = rcBar.top() + fixceil(bar_len);
        } else {
            IntRect rcBg;

            rcBg.m_left     = isVScrollBarOnLeft() ? rc.left() : (rc.right() - barWidth);
            rcBg.m_top         = rc.top();
            rcBg.m_right     = rcBg.left() + barWidth;
            rcBg.m_bottom     = rc.bottom();

            //m_drset->draw(context, DR_BGVSCROLLBAR, DRAWSTATE_NORMAL, rcBg);

            rcBar.m_left     = rcBg.left() + barmargin;
            rcBar.m_top     = rcBg.top() + fixceil(bar_pos);
            rcBar.m_right     = rcBg.right() - barmargin;
            rcBar.m_bottom     = rcBar.top() + fixceil(bar_len);
        }

        //m_drset->draw(context, DR_VSCROLLBAR, DRAWSTATE_NORMAL, rcBar);

        isdraw = true;
    }

    if (isdraw && !isScrollBarAlwaysShow()) {
        if (m_timerId)
            removeTimer(m_timerId);

        m_timerId = registerTimer (2000, "ScrollBar");
    }
}

bool ScrollBar::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER
            && m_timerId > 0
            && m_timerId == ((TimerEvent *)event)->timerID())
    {
        removeTimer(m_timerId);
        m_timerId = 0;

        setAutoShowBar(false);
        m_owner->updateView();

        return STOP_DISPATCH;
    }

    return GOON_DISPATCH;
}

void ScrollBar::updateContentRect()
{
#if 0
    if (m_owner) {
        IntRect rc;
        m_owner->autoFitSize();
        rc = m_owner->getRect();
        m_owner->setRect(0, rc.top(), rc.right(), rc.bottom());
    }

    moveViewport (m_offx, m_offy);
#endif
}

void ScrollBar::moveViewport (int x, int y, bool bupdate)
{
    if (m_owner) {
        const IntRect & ownerrc = m_owner->getRect();
        const IntRect & viewportrc = m_owner->getViewportRect();

        int   move_w = ownerrc.width() - viewportrc.width();
        int   move_h = ownerrc.height() - viewportrc.height();

        if(loopScroll()) {
            while(y < 0)
                y += ownerrc.height();
            y %= ownerrc.height();

            while(x < 0)
                x += ownerrc.width();
            x %= ownerrc.width();
        } else {
            if(move_w <= 0)
                x = 0;
            else {
                if(x < 0)
                    x = 0;
                else if(x > move_w)
                    x = move_w;
            }

            if(move_h <= 0) {
                y = 0;
            } else {
                if(y < 0)
                    y = 0;
                else if(y > move_h)
                    y = move_h;
            }
        }

        m_offx = x;
        m_offy = y;

        m_owner->setPosition(-m_offx, -m_offy, bupdate);
    }

    if(bupdate) {
        setAutoShowBar(true);
        m_owner->updateView();
    }
}

void ScrollBar::moveOffsetH(int x)
{
    moveViewport(m_offx + x, m_offy);
}

void ScrollBar::moveOffsetV(int y)
{
    moveViewport(m_offx, m_offy + y);
}

#if 0
bool ScrollBar::dispatchEvent(Event* event)
{
    if (STOP_DISPATCH == PanelView::dispatchEvent(event))
        return STOP_DISPATCH;

    switch(event->eventType()) {
    case Event::KEY_DOWN:
    case Event::KEY_LONGPRESSED:
    case Event::KEY_ALWAYSPRESS:
        switch(((KeyEvent *)event)->keyCode()) {
        case KeyEvent::KEYCODE_CURSOR_UP:
            moveOffsetV(-m_vStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_DOWN:
            moveOffsetV(m_vStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_LEFT:
            moveOffsetH(-m_hStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_RIGHT:
            moveOffsetH(m_hStep);
            break;
        default:
            return GOON_DISPATCH;
        }
        return STOP_DISPATCH;

    default:
        return GOON_DISPATCH;
    }

    return GOON_DISPATCH;
}
#endif

} // namespace hfcl
