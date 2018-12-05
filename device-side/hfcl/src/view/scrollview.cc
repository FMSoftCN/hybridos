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

#include "view/scrollview.h"

#include <minigui/fixedmath.h>

#include "graphics/graphicscontext.h"
#include "services/timerservice.h"

#ifdef __MMI_SAMSUNG_GT_FEATURE__
#define DEF_SCROLL_MARGIN 	1
#define DEF_SCROLL_WIDTH	4
#else
#define DEF_SCROLL_MARGIN 	0
#define DEF_SCROLL_WIDTH	2
#endif

namespace hfcl {

bool ScrollView::g_is_all_scrollbar_on_left = false;

ScrollView::ScrollView(View *p_parent)
    :PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(ScrollView))
    ,m_barWidth(DEF_SCROLL_WIDTH)
    ,m_barMargin(DEF_SCROLL_MARGIN)
    ,m_vStep(10)
    ,m_hStep(10)
    ,m_offx(0)
    ,m_offy(0)
    ,m_content(NULL)
    ,m_timerId(0)
{
    setContent(HFCL_NEW_EX(ContainerView, ()));
	setBarVisible(true);
	setAutoShowBar(true);
	setFocusValid(true);
#ifdef __MMI_SAMSUNG_GT_FEATURE__
    setScrollBarAlwaysShow(true);
	setScrollBarFloating(false);
#else
    setScrollBarAlwaysShow(false);
	setScrollBarFloating(true);
#endif
	if (IsAllScrollBarOnLeft())
		setVScrollBarOnLeft(true);
}

ScrollView::ScrollView(View *p_parent, DrawableSet* drset)
    :PanelView(p_parent, drset)
    ,m_barWidth(DEF_SCROLL_WIDTH)
    ,m_barMargin(DEF_SCROLL_MARGIN)
    ,m_vStep(10)
    ,m_hStep(10)
    ,m_offx(0)
    ,m_offy(0)
	,m_content(NULL)
    ,m_timerId(0)
{
    setContent(HFCL_NEW_EX(ContainerView, ()));
	setBarVisible(true);
	setAutoShowBar(true);
	setFocusValid(true);
#ifdef __MMI_SAMSUNG_GT_FEATURE__
    setScrollBarAlwaysShow(true);
	setScrollBarFloating(false);
#else
    setScrollBarAlwaysShow(false);
	setScrollBarFloating(true);
#endif
	if (IsAllScrollBarOnLeft())
		setVScrollBarOnLeft(true);
}

ScrollView::~ScrollView(void)
{
    if(m_timerId)
        removeTimer(m_timerId);
}

void ScrollView::setBarWidth(int barwidth)
{
    int w,h;

    getSize(&w, &h);

    if(barwidth >= 0 && barwidth <= MIN(w, h))
	{
	   m_barWidth = (Uint16)barwidth;
	}
}

void ScrollView::setBarMargin(int barmargin)
{
    int w, h;

	getSize(&w, &h);

    if (barmargin >= 0 && barmargin <= MIN(w, h))
	{
		m_barMargin = (Uint16)barmargin;
	}
}

#define MIN_BAR  (5<<16)

bool ScrollView::scrollVisiable(int * barWidth, bool isVertical)
{
	fixed contw, conth;
	fixed vieww, viewh;
	IntRect rc;

    if((!vBarVisible() && isVertical) || (!hBarVisible() && !isVertical))
        return FALSE;

    if( !m_content)
        return FALSE;

	if(barWidth)
		*barWidth = m_barWidth;

	rc = getRect();
	vieww = itofix(rc.width());
	viewh = itofix(rc.height());
	contw = itofix(m_content->getRect().width());
	conth = itofix(m_content->getRect().height());

	if(isVertical && vBarVisible() && conth > viewh) 
	{
		return TRUE;
	}
	else if(!isVertical && hBarVisible() && contw > vieww)
	{
		return TRUE;
	}

	return FALSE;
}

void ScrollView::drawScroll(GraphicsContext* context, IntRect &rc, int status)
{
	fixed contw, conth;
	fixed vieww, viewh;
	
	int barWidth = m_barWidth;
	int barmargin = m_barMargin; 
	
	bool isdraw = false;

    if(!vBarVisible() && !hBarVisible())
        return;

    if(!autoShowBar())
        return;

    if( !m_content || loopScroll())
        return;

    if(barWidth <= 0)
        return;

	context->setMapView(this);

	vieww = itofix(rc.width());
	viewh = itofix(rc.height());
	contw = itofix(m_content->getRect().width());
	conth = itofix(m_content->getRect().height());
	
	if(hBarVisible() && contw > vieww)
	{
		IntRect rcBar, rcBg;
		
        fixed bar_len = itofix(rc.width() * rc.width() / m_content->getRect().width());
        fixed bar_pos = itofix(m_offx * rc.width() / m_content->getRect().width());
		
		bar_len = bar_len < MIN_BAR ? MIN_BAR : bar_len;
		

		if (isScrollBarFloating())
		{
			rcBar.m_left	= rc.left() + fixceil(bar_pos);
			rcBar.m_top 	= rc.bottom() - barWidth;
			rcBar.m_right	= rcBar.left() + fixceil(bar_len);
			rcBar.m_bottom	= rcBar.top() + barWidth;

		} else {
			IntRect rcBg;
			
			rcBg.m_left 	= rc.left();
			rcBg.m_top 		= rc.bottom() - barWidth;
			rcBg.m_right 	= rc.right();
			rcBg.m_bottom 	= rc.bottom();
			
			m_drset->draw(context, DR_BGHSCROLLBAR, DRAWSTATE_NORMAL, rcBg);
			
			rcBar.m_left 	= rcBg.left() + fixceil(bar_pos);
			rcBar.m_top 	= rcBg.top() + barmargin;
			rcBar.m_right 	= rcBar.left() + fixceil(bar_len);
			rcBar.m_bottom 	= rcBg.bottom() - barmargin;
		}

		m_drset->draw(context, DR_HSCROLLBAR, DRAWSTATE_NORMAL, rcBar);
		
		isdraw = true;	
	}

	if(vBarVisible() && conth > viewh) 
	{		
		IntRect rcBar, rcBg;
		
		fixed bar_len = itofix(rc.height() * rc.height() / m_content->getRect().height());		
        fixed bar_pos = itofix(m_offy * rc.height() / m_content->getRect().height());
		
		bar_len = bar_len < MIN_BAR ? MIN_BAR : bar_len;

		if (isScrollBarFloating())
		{
			rcBar.m_left 	= isVScrollBarOnLeft() ? rc.left() : (rc.right() - barWidth);
			rcBar.m_top 	= rc.top() + fixceil(bar_pos);
			rcBar.m_right 	= rcBar.left() + barWidth;
			rcBar.m_bottom 	= rcBar.top() + fixceil(bar_len);
		} else {
			IntRect rcBg;
			
			rcBg.m_left 	= isVScrollBarOnLeft() ? rc.left() : (rc.right() - barWidth);
			rcBg.m_top 		= rc.top();
			rcBg.m_right 	= rcBg.left() + barWidth;
			rcBg.m_bottom 	= rc.bottom();
			
			m_drset->draw(context, DR_BGVSCROLLBAR, DRAWSTATE_NORMAL, rcBg);
			
			rcBar.m_left 	= rcBg.left() + barmargin;
			rcBar.m_top 	= rcBg.top() + fixceil(bar_pos);
			rcBar.m_right 	= rcBg.right() - barmargin;
			rcBar.m_bottom 	= rcBar.top() + fixceil(bar_len);
		}
		
		m_drset->draw(context, DR_VSCROLLBAR, DRAWSTATE_NORMAL, rcBar);
		
		isdraw = true;
	}

	if(isdraw && !isScrollBarAlwaysShow())
    {
        if(m_timerId)
            removeTimer(m_timerId);

        m_timerId = registerTimer(2000, "ScrollView");
    }
}

bool ScrollView::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER
		    && m_timerId > 0 
		    && m_timerId == ((TimerEvent *)event)->timerID())
    {
	    removeTimer(m_timerId);
	    m_timerId = 0; 
	    
	    setAutoShowBar(false);
	    updateView();

	    return DISPATCH_STOP_MSG;
    }

    return DISPATCH_CONTINUE_MSG;
}

void ScrollView::setContent(ContainerView* p_content)
{
    if (m_content)
       removeChild(m_content);
    m_content = p_content;
    if (NULL != p_content){
        addChild(m_content);
		m_content->setAutoSize(true);
	}
}

void ScrollView::updateContentRect(void)
{
	if(m_content)
	{
		IntRect rc;
		m_content->autoFitSize();
		 rc= m_content->getRect();
		m_content->setRect(0, rc.top(), rc.right(), rc.bottom());
	}

	//VincentWei: const IntRect & contentrc = m_content->getRect();
	moveViewport(m_offx, m_offy);
}

void ScrollView::moveViewport(int x, int y, bool bupdate)
{
    if (m_content) 
	{
		const IntRect & contentrc = m_content->getRect();
		const IntRect & viewportrc = getRect();

		int   move_w = contentrc.width() - viewportrc.width();
		int   move_h = contentrc.height() - viewportrc.height();
		
		if(loopScroll()) {
			while(y < 0)
				y += contentrc.height();
			y %= contentrc.height();

			while(x < 0)
				x += contentrc.width();
			x %= contentrc.width();
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

		
	    // BUG.ListView ylwang:changed 2012-04-06
	    // ListView.relayout interface change m_content's rect to (0, 0, x, x), but ScrollView's m_offx and m_offy
	    // did NOT change, following code will cause wrong position when use ListView.relayout single.
	    // When we set m_content's postion every time, we can avoid this kind of problem.
	    /*
		if(m_offx == x && m_offy == y)
			return;
	    */
		
		m_offx = x;
		m_offy = y;

        m_content->setPosition(-m_offx, -m_offy, bupdate);
    }

	if(bupdate) {
		setAutoShowBar(true);
		updateView();
	}

}

void ScrollView::drawScrollContent(GraphicsContext *gc, int status)
{
    m_content->onPaint(gc, status);

    if (!loopScroll())
        return;

    if (m_offx != 0 || m_offy != 0) {
        //content info 
        const IntRect& rcCont = m_content->getRect();
        int cx = rcCont.right();
        int cy = rcCont.bottom();

        //view info
        const IntRect& rcView = getRect();
        int vieww = rcView.width();
        int viewh = rcView.height();

        if (cx >= vieww && cy >= viewh) {
            return;
        } 

        if (cx < vieww) {
            IntRect clipRc(cx, 0, vieww, viewh);
            gc->setMapView(this);
            if (gc->rectVisible(clipRc)) {
                gc->save(); 
                gc->clip(clipRc); 

                m_content->setPosition(cx, -m_offy, false);
                m_content->onPaint(gc, status);
                gc->restore();
            }
        }

        if (cy < viewh) {
            IntRect clipRc(0, cy, vieww, viewh);
            gc->setMapView(this);
            if (gc->rectVisible(clipRc)) {
                gc->save();
                gc->clip(clipRc);

                m_content->setPosition(-m_offx, cy, false);
                m_content->onPaint(gc, status);
                gc->restore();
            }
        }

        if (cx < vieww && cy < viewh) {
            IntRect clipRc(cx, cy, vieww, viewh);
            gc->setMapView(this);
            if (gc->rectVisible(clipRc)) {
                gc->save();
                gc->clip(clipRc);

                m_content->setPosition(cx, cy, false);
                m_content->onPaint(gc, status);
                gc->restore();
            }
        }

        m_content->setPosition(-m_offx, -m_offy, false);
    }
}

void ScrollView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
	for (View *view = firstChild(); view; view = view->nextSibling()) {
		if(!view->isVisible() || view->getRect().isEmpty())
			continue;

		if(view == m_content) {
            //draw content if exist
			drawScrollContent(context, status);
		}
        else { 
            //draw other views
			view->onPaint(context, status);
		}
	}
}

void ScrollView::moveOffsetH(int x)
{
    moveViewport(m_offx + x, m_offy); 
}

void ScrollView::moveOffsetV(int y)
{
    moveViewport(m_offx, m_offy + y); 
}

bool ScrollView::dispatchEvent(Event* event)
{
    if (DISPATCH_STOP_MSG == PanelView::dispatchEvent(event))
        return DISPATCH_STOP_MSG;

    switch(event->eventType()) {
    case Event::KEY_DOWN:
    case Event::KEY_LONGPRESSED:
    case Event::KEY_ALWAYSPRESS:
        switch(((KeyEvent *)event)->keyCode()) {
        case KeyEvent::KEYCODE_CURSOR_UP :
            moveOffsetV(-m_vStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_DOWN :
            moveOffsetV(m_vStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_LEFT :
            moveOffsetH(-m_hStep);
            break;
        case KeyEvent::KEYCODE_CURSOR_RIGHT :
            moveOffsetH(m_hStep);
            break;
        default:
            return DISPATCH_CONTINUE_MSG;
        }
        return DISPATCH_STOP_MSG;

    default:
        return DISPATCH_CONTINUE_MSG;
    }

    return DISPATCH_CONTINUE_MSG;
}

DEFINE_CLASS_NAME(ScrollView)

} // namespace hfcl
