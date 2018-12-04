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


#include "gridview.h"
#if 0	// GT_jyseo rollback
#include "services/timerservice.h"

#define _LOOP_RATE_ 12
#define _LONGPRESS_ 3
#endif

namespace hfcl {

GridView::GridView(View * p_parent, DrawableSet* drset)
	:PanelView(p_parent, drset)
	, m_col(1), m_row(1)
	, m_grid_w(0), m_grid_h(0)
	, m_gap_w(0), m_gap_h(0)
    , m_boundary_listener(0)
    , m_oldFocusItem(0)
//    , m_timerId(0)        // GT_jyseo rollback 
//    , m_pressedKey(0)     // GT_jyseo rollback
{
	m_focusItem = GRIDVIEW_FOCUS_INVALID;
	setFocusValid(true);
}	

GridView::GridView(View * p_parent)
	:PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(GridView))
	, m_col(1), m_row(1)
	, m_grid_w(0), m_grid_h(0)
	, m_gap_w(0), m_gap_h(0)
	, m_boundary_listener(0)
    , m_oldFocusItem(0)
//    , m_timerId(0)            // GT_jyseo rollback 
//    , m_pressedKey(0)     // GT_jyseo rollback 
{
	m_focusItem = GRIDVIEW_FOCUS_INVALID;
	setFocusValid(true);
}

GridView::GridView(View * p_parent, int i_id, int x, int y, 
		int w, int h, int row, int col, int gap_w, int gap_h)
	: PanelView(i_id, x, y, w, h)
	, m_col(col), m_row(row)
	, m_grid_w(0), m_grid_h(0)
	, m_gap_w(gap_w), m_gap_h(gap_h)
    , m_boundary_listener(0)
    , m_oldFocusItem(0)
//    , m_timerId(0)        // GT_jyseo rollback
//    , m_pressedKey(0)     // GT_jyseo rollback
{
	if(row > 0 && col > 0) {
		m_grid_h = (h - (row + 1) * gap_h) / row;
		m_grid_w = (w - (col + 1) * gap_w) / col;
	}
	
	m_focusItem = GRIDVIEW_FOCUS_INVALID;
}

GridView::~GridView()
{
#if 0   // GT_jyseo rollback
    if(m_timerId > 0)
        removeTimer(m_timerId);
#endif

}

void GridView::setRows (int row)
{
	if (row <= 0 || m_row == row)
		return;

	m_row = row;
	m_grid_h = (getRect().height() - (m_row + 1) * m_gap_h) / m_row;
	
    reLayout();
}

void GridView::setCols (int col)
{
	if (col <= 0 || m_col == col)
		return;

	m_col = col;
	m_grid_w = (getRect().width() - (m_col + 1) * m_gap_w) / m_col;
	
    reLayout();
}

void GridView::setGapWidth (int gap_w) 
{ 
	if (gap_w < 0 || m_gap_w == gap_w)
		return;

	m_gap_w = gap_w;  
	if (m_col > 0) {
		m_grid_w = (getRect().width() - (m_col + 1) * m_gap_w) / m_col;
	}
	reLayout(); 
}

void GridView::setGapHeight (int gap_h) 
{ 
	if (gap_h < 0 || m_gap_h == gap_h )
		return;

	m_gap_h = gap_h;
	if (m_row > 0){
		m_grid_h = (getRect().height() - (m_row + 1) * m_gap_h) / m_row;
	}
	reLayout(); 
}

#define GRID_X0 4
void GridView::reLayout (void)
{
	int x, y;
	View *view = firstChild();
	
	for (int i = 0; view && i < m_row; i++)
	{
		for (int j = 0; view && j < m_col; j++)
		{
			if (m_bNeedConvert) {
				x = GRID_X0 + m_gap_w * ((m_col - j)) + m_grid_w * (m_col - j - 1);
			} else {
				x = m_gap_w * (j + 1) + m_grid_w * j;
			}
			y = m_gap_h * (i + 1) + m_grid_h * i;
			view->setRect(x, y, x + m_grid_w, y + m_grid_h); 
			view = view->nextSibling();
		}
	}

    updateView(); 
}

bool GridView::setRect(const IntRect& irc)
{
    bool ret = PanelView::setRect(irc);

	if (ret && m_col > 0 && m_row > 0) {
		m_grid_w = (irc.width() - (m_col + 1) * m_gap_w) / m_col;
		m_grid_h = (irc.height() - (m_row + 1) * m_gap_h) / m_row;
		
		reLayout();
	}

    return ret;
}

int GridView::addView(View *view)
{
	int index;
	int x, y;

	if (m_col <= 0 || m_row <= 0)
		return -1;

	// add into the tree, 
	// maybe it was in the tree ?
	addChild(view); 
	
	int count = viewCount();

	if(count > m_col * m_row)
		return -1;

	index = count - 1;

	if (m_bNeedConvert) {
		x = GRID_X0 + (m_col - index % m_col - 1) * (m_grid_w + m_gap_w) + m_gap_w;
	} else {
		x = index % m_col * (m_grid_w + m_gap_w) + m_gap_w;
	}
		
	y = index / m_col * (m_grid_h + m_gap_h) + m_gap_h;

	view->setRect(x, y, x + m_grid_w, y + m_grid_h);
	
	return 0;
}

int GridView::remove(View *view)
{
#if 0   // GT_jyseo rollback
	if (0 == removeChild (view)) {
        reLayout();
    }
    // TODO
#endif     
	return 0;
}

int GridView::remove(int row, int col)
{
	int index = row * m_col + col;
	View *view = getChildByIndex(index);
	
	if (NULL != view) {
#if 0     // GT_jyseo rollback
		remove(view);
		return 0;
#else
		removeChild(view);
        reLayout();
		return 0;
#endif
	}

	return -1;
}

void GridView::onGetFocus()
{
#if 0     // GT_jyseo rollback
    if (m_focusItem < 0) {
        m_focusItem = 0;
    }
    setFocusItem(m_focusItem);
#else
    if (m_focusItem < 0) {
        m_focusItem = 0;
        setFocusItem(0,0);   
    }
    else
        setFocusItem(m_focusItem);
#endif

}

void GridView::onLoseFocus()
{
    #if 0   // GT_jyseo rollback
    if(m_timerId > 0)
        removeTimer(m_timerId);
    m_timerId = 0; 
    m_pressedKey = 0;
    #endif   
    updateView(); 
}

int GridView::focusItemPosition(int *row, int *col)
{
    if(m_focusItem < 0 || m_col <= 0)
        return -1;

	if (NULL != row)
		*row = m_focusItem / m_col;
	if (NULL != col)
		*col = m_focusItem % m_col;

    return m_focusItem;
}

bool GridView::notify2Parent(KEY_DIRECTION direction)
{
    int flag = -1;

	if(m_focusItem < 0)
		return false;
    
    Event::EventType eventType = Event::CUSTOM_NOTIFY;
    
    View * p = parent();
	int count = viewCount();

    if(p == NULL || count <= 0)
        return true;

	switch(direction)
	{
		case GRIDVIEW_KEY_LEFT:
            if(m_focusItem%m_col == 0)
            {
                flag = CustomEvent::CUS_BOUNDARY_LEFT;
            }
			break;
		case GRIDVIEW_KEY_RIGHT:
            if(m_focusItem%m_col == m_col-1)
            {
                flag = CustomEvent::CUS_BOUNDARY_RIGHT;
            }
			break;
		case GRIDVIEW_KEY_UP:
            if(m_focusItem < m_col)
            {
                flag = CustomEvent::CUS_BOUNDARY_UP;
            }
			break;
		case GRIDVIEW_KEY_DOWN:
            if(m_focusItem > count - m_col-1)
            {
                flag = CustomEvent::CUS_BOUNDARY_DOWN;
            }
			break;
	}
    if(flag >= 0)
    {
        CustomEvent event(eventType, flag, 0);
        if (m_boundary_listener)
            return m_boundary_listener->handleEvent(&event);
    }
    return true;
}

void GridView::changeFocusIndex(KEY_DIRECTION direction)
{
	if(m_focusItem < 0)
		return ;
   
    if(m_boundary_listener && !notify2Parent(direction)) 
        return ;

	int count = viewCount();

	m_oldFocusItem = m_focusItem;
	
	switch(direction)
	{
		case GRIDVIEW_KEY_LEFT:
			if (m_bNeedConvert)
			{
				if(m_focusItem ==  count - 1)
					m_focusItem = 0;
				else 
					m_focusItem++;
			} else {
				if(m_focusItem == 0)
					m_focusItem = count - 1;
				else 
					m_focusItem --;
			}
			break;
		case GRIDVIEW_KEY_RIGHT:
			if (m_bNeedConvert)
			{
				if(m_focusItem == 0)
					m_focusItem = count - 1;
				else 
					m_focusItem --;
			} else {
				if(m_focusItem ==  count - 1)
					m_focusItem = 0;
				else 
					m_focusItem ++;
			}
			break;
		case GRIDVIEW_KEY_UP:
            if(m_focusItem == 0){    
                m_focusItem = count -1;   
            }
            else if(m_focusItem - m_col < 0 && m_col > 0) 
            {    
                int row = count % m_col ? (count / m_col + 1) : count / m_col;    
                int cur_col = m_focusItem%m_col;    
                int idx = (row-1) * m_col + cur_col-1;    
                while(idx > count)     
                    idx -= m_col;        
                m_focusItem = idx;   
            }    
            else
            {     
                m_focusItem -= m_col;
            } 
			break;
		case GRIDVIEW_KEY_DOWN:
 			if(m_focusItem == (count - 1))
            { 	
                if(m_focusItem == (m_col*m_row-1)){ 
                    m_focusItem  = 0;	  
                } else {
                    m_focusItem %= m_col;
                    m_focusItem +=1;	   
                }	
            }
            else if(m_focusItem + m_col > count - 1)
            {	   
                if(m_focusItem/m_col == (m_row -1)) {		
                    m_focusItem %= m_col;
                    m_focusItem +=1;	  
                } else {	  
                    m_focusItem  = 0;
                }
            }
            else 
            {
                m_focusItem += m_col;
            }
			break;	//jyseo_0404
		default:
			return;
	}

	setFocusItem(m_focusItem);
}

int GridView::setFocusItem(int index)
{	
	View *focus = getChildByIndex(index); 
	
	if(NULL != focus)
	{
        m_focusItem = index;
		setFocusView(focus);
		//raise event
		CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_SELCHANGED, (int)this);
		raiseEvent(&event);
	    return 0;
	}

	return -1;
}

int GridView::setFocusItem(int row, int col)
{
    return setFocusItem(row * m_col + col); 
}

bool GridView::dispatchEvent(Event* event)
{
	switch(event->eventType())
	{
		case Event::MOTION_UP:
		case Event::MOTION_DOWN:      
		case Event::MOTION_CLICK:          
			// TODO, for mouse support.
			break;   
        case Event::KEY_LONGPRESSED:
        case Event::KEY_DOWN:
        case Event::KEY_ALWAYSPRESS:
        {
            int code = ((KeyEvent *)event)->keyCode();
            if ( onKeyPressed (code) ) {

                return DISPATCH_STOP_MSG;
            }
        }
		default:
			return View::dispatchEvent(event);
	}
	return DISPATCH_CONTINUE_MSG;
}

View * GridView::getOldFocusItem()
{	
	return getChildByIndex(m_oldFocusItem);
}

bool GridView::onKeyPressed(int keyCode)
{
    switch(keyCode)
    {
        case KeyEvent::KEYCODE_CURSOR_UP :
            changeFocusIndex(GRIDVIEW_KEY_UP);
            break;
        case KeyEvent::KEYCODE_CURSOR_DOWN :
            changeFocusIndex(GRIDVIEW_KEY_DOWN);
            break;
        case KeyEvent::KEYCODE_CURSOR_LEFT :
            changeFocusIndex(GRIDVIEW_KEY_LEFT);
            break;
        case KeyEvent::KEYCODE_CURSOR_RIGHT :
            changeFocusIndex(GRIDVIEW_KEY_RIGHT);
            break;
        default: 
            return false;
    }
    return true;
}

#if 0   // GT_jyseo rollback
bool GridView::handleEvent(Event* event)
{
    if (event->eventType() == Event::TIMER 
            && m_timerId == ((TimerEvent *)event)->timerID())
    {
        if (m_timerCount++ > _LONGPRESS_){
            onKeyPressed(m_pressedKey);
        }
    }
    return DISPATCH_STOP_MSG;
}
#endif

DEFINE_CLASS_NAME(GridView)


} // namespace hfcl

