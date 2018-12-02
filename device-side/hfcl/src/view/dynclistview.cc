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


#include "nguxevent.h"
#include "graphicscontext.h"
#include "drawable.h"
#include "panelview.h"
#include "itemview.h"
#include "respkgmanager.h"
#include "scrollview.h"
#include "bitmark.h"
#include "dynclistview.h"

#include "hal_misc.h"

#define D_PRT(x...) 
//_DBG_PRINTF(x)

namespace hfcl {

////////////////////// Features /////////////////////////
// 1. equal item height.
// 2. special hilight item height.
// 3. same item view template.
// 4. scrollbar support.
// 5. panelview's features.
//

#ifdef __MMI_SAMSUNG_GT_FEATURE__
#define DEF_SCROLL_MARGIN 	1
#define DEF_SCROLL_WIDTH	4
#else
#define DEF_SCROLL_MARGIN 	0
#define DEF_SCROLL_WIDTH	2
#endif

 
DyncListView::DyncListView(View* p_parent)
    :PanelView(p_parent)
{
    init();
}

DyncListView::DyncListView(View* p_parent, DrawableSet* drset)
    :PanelView(p_parent, drset)
{
    init();
}

DyncListView:: ~DyncListView()
{
}

void DyncListView::reInit(void)
{
    m_item_count = 0;
    m_visible_item_count = 0;
    m_hi_index = -2;
    m_up_index = -1;
    m_itemRowGap = 0;
    m_down_index = -1;
    m_dock_top = true;
	m_freeze = false;
	m_scrollGap = 0;
}

void DyncListView::init(void)
{
    m_item_count = 0;
    m_visible_item_count = 0;
    m_item_templete_id = -1;
    m_nm_height = 40;
    m_hi_height = 40;
    m_hi_index = -1;
    m_up_index = -1;
    m_dock_top = true;
    m_itemRowGap = 0;
    m_down_index = -1;
    m_firstitem = NULL;
    m_lastitem = NULL;
  
    m_v_scrollbar_width = DEF_SCROLL_WIDTH;
    m_v_scrollbar_margin_right = DEF_SCROLL_MARGIN;
	m_freeze = false;
	m_focus = true;
}

bool DyncListView::setRect(const IntRect& irc)
{
    if (irc == m_rect)
        return false;

    if (m_rect != irc) {
        // VincentWei: call updateView only set a different rect
        int n;
        int oldv = m_visible_item_count;
        m_rect = irc;
        
        if(m_item_count <= 0)
            return true;
            
        n = getVisibleItemCountNeed();
        if(m_item_count > 0 && n > m_item_count)
            m_visible_item_count = m_item_count;
        else
            m_visible_item_count = n;
      
        D_PRT(" DyncListView::setRect  m_visible_item_count=%d,m_item_count=%d", m_visible_item_count,m_item_count);
        if(oldv != m_visible_item_count){
            prepareVisualitems();   
            if(m_item_count > m_visible_item_count)
            {
                if(m_hi_index >= 0)
                {
                    m_down_index = m_up_index + m_visible_item_count - 1;
                }
                else
                {
                    if(m_up_index >= 0)
                        m_down_index = m_up_index + m_visible_item_count - 1;
                    else if(m_down_index >= 0)
                        m_up_index = m_down_index - m_visible_item_count + 1;
                }
            }
            else
            {
                m_up_index = 0;
                m_down_index = m_up_index + m_visible_item_count - 1;
            }
            
            if(m_item_count > 0){
                reLoadData();
                if(m_hi_index >= 0){
                    itemLayout(m_hi_index); 
                    View * v = itemFromIndex(m_hi_index);
                    setHilightView(v);
                }
                else
                {
                    itemLayout(0); 
                    setHilightView(firstChild());
                }
                updateListView();
            } 
        }
    }
    return true;
}

bool DyncListView::prepareVisualitems(void)
{
    int n = m_visible_item_count;
    clean();
    
    if(m_item_count > 0 && n > m_item_count)
        n = m_item_count;
           
    D_PRT("DyncListView reinit itemview");
    if(m_item_templete_id != (HTResId)-1) {
        for(int c = 0; c < n; ++c) {
            ////////////////////////// parent is me, no viewcontext, no contentprovider
            CreateViewFromRes(m_item_templete_id, this, NULL, NULL);
        }
    }
    return true;
}

int DyncListView::reLoadData(void)
{
    View* item = NULL;
    int i = 0;
    // check
    if(m_item_count <= 0 || m_visible_item_count <= 0)
        return 0;
    
    if(m_up_index == -1 && m_down_index == -1)
    {
        m_up_index = 0;
        m_down_index = m_visible_item_count - 1;
    }
            
    item = firstChild();
    for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
        onLoadItemData(i, item);
    }
	return 0;
}

void DyncListView::clear(void)
{
    clean();
    reInit();
    D_PRT("clear>>>");
}

void DyncListView::initSelectMark(int count)
{
	if(m_select_mark.count() > 0)
		return;
	m_select_mark.init(count);
}

bool DyncListView::isItemSelected(int index)
{
	return m_select_mark.getMark(index);
}

void DyncListView::selectItem(int index, bool sel)
{
	m_select_mark.setMark(index, sel);
}

void DyncListView::select(int index, bool sel)
{
	ItemView* item = (ItemView*)itemFromIndex(index);
	selectItem(index, sel);

	if(item) {
		item->setSelected(sel);
	}
}

void DyncListView::updateListView(void)
{
    updateView();
}

void DyncListView::selectAll(bool sel)
{
    for(int i = 0; i < itemCount(); i++){
        if(i < getVisibleItemCountNeed()){
            ItemView* item = (ItemView*)(getChildByIndex(i));
            item->setSelected(sel);
        }
        
	    selectItem(i, sel);
    }
    updateListView();
}


void DyncListView::initItemAddData(int count)
{
	if(m_data_keeper.count() > 0)
		return;
	m_data_keeper.init(count);
}

void DyncListView::setItemAddData(int index, void* data)
{
    
	m_data_keeper.setPointer(index, data);
}

void* DyncListView::itemAddData(int index)
{
	return m_data_keeper.getPointer(index);
}

void DyncListView::freeze(bool f)
{
	m_freeze = f;
}

View* DyncListView::hilightItem(void)
{
	return getChildByIndex(m_hi_index - m_up_index);
}

View* DyncListView::itemFromIndex(int index)
{
	// check
	if(index < m_up_index 
		|| index > m_down_index 
			|| m_visible_item_count < 1
				|| m_item_count < 1)
		return NULL;

	if(index >= m_up_index && index <= m_down_index)
		return getChildByIndex(m_hi_index - m_up_index);
	else
		return NULL;
}

bool DyncListView::addItem(void* add_data)
{
	// check
	if(m_data_keeper.count() < 1)
		return false;
		
	// add data !!! we need do it first.
	m_data_keeper.setPointer(m_item_count, add_data);
	
	addItem();
	return true;
}

bool DyncListView::addItem(void)
{
    if(m_select_mark.count() > 0) {
		m_select_mark.setMark(m_item_count, false);
	}
	
	int n = getVisibleItemCountNeed();
	if(m_item_count < n )
	{
	    CreateViewFromRes(m_item_templete_id, this, NULL, NULL);
	    m_visible_item_count = m_item_count + 1;
	}
	m_item_count++;
	
	D_PRT("DyncListView::addItem>>>item=%d,vs=%d",m_item_count,m_visible_item_count);
	if (!m_freeze) {
	
	     reLoadData();
         if(m_hi_index == -2)
            hilight(0);  
	}
	return true;
}

void DyncListView::reLayout(void)
{
	IntRect listRc = getRect();
	bool bupdate = FALSE;
	bool scrollVisible = FALSE;

	if(m_item_count <= 0)
			return ;

	scrollVisible = m_item_count > m_visible_item_count ? TRUE: FALSE;
	
	View *view = firstChild();
	for (int i = 0; view; i++)
	{
		/*
		 * Every Item has the same height:
		 *	---------------------------		 
		 *	|						  |
		 *	|  item height			  |
		 *	|						  |
		 *	---------------------------
		 *
		 */
		IntRect rc = view->getRect();

		if(!view->isVisible())	 // add by xulei 2012,03,31, hide disvsible Itemview
			continue;
		
		if(m_scrollGap > 0 && scrollVisible)
		{
			if((ScrollView::IsAllScrollBarOnLeft()))
			{
				rc.m_left = m_v_scrollbar_width +1 +m_scrollGap;
				rc.m_right = listRc.m_right ;				
			}
			else
			{
				rc.m_right = listRc.m_right - (m_v_scrollbar_width+1) -m_scrollGap;
				rc.m_left = listRc.m_left;
			}
		}
		else
		{
			rc.m_left = listRc.m_left;
			rc.m_right = listRc.m_right;
		}		
		if(view->setRectNoUpdate(rc.m_left,rc.m_top,rc.m_right,rc.m_bottom))
		    bupdate = TRUE;
		view = view->nextSibling();
	}
	if(bupdate)
	    updateListView();
}

bool DyncListView::removeItem(int index)
{
     D_PRT("DyncListView::removeItem>>>index=%d",index);    
    if (index < 0 || index > m_item_count || -1 == m_hi_index)
        return false;
    
	// adjust select mark
	if(m_select_mark.count() > 0) {
		m_select_mark.move(index+1, index, -1);
	}

	// adjust add data
	if(m_data_keeper.count() > 0) {
		m_data_keeper.move(index+1, index, -1);
	}
	
    if (index >= m_up_index && index <= m_down_index) {
        if(m_item_count <= m_visible_item_count){
            removeChild(lastChild());
            m_visible_item_count--;
            --m_down_index;
            --m_item_count;
            
            if(m_hi_index == m_down_index){
                setHilightView(getChildByIndex(m_hi_index-m_up_index - 1));
                --m_hi_index;
            }
            else
               setHilightView(getChildByIndex(m_hi_index-m_up_index)); 
            
            m_dock_top = true;
            D_PRT("del item for reLayout>>>");
            itemLayout(m_hi_index);
            reLoadData();
        }
        else
        {
             if(m_down_index == m_item_count -1 || (index == m_up_index && 0 != m_up_index)) {
                --m_up_index;
                --m_down_index;
                setHilightView(getChildByIndex(m_hi_index-m_up_index-1));
                --m_hi_index;
            }
            else
                setHilightView(getChildByIndex(m_hi_index-m_up_index));
            D_PRT("del item for reLoadData>>>");
            --m_item_count;
            reLoadData();
        }
    }
    else if(index < m_up_index){
        --m_item_count;
        --m_up_index;
        --m_down_index;
        --m_hi_index;
        D_PRT("del item for no vis reloaddata>>>");
        reLoadData();
    }else{
        --m_item_count;
        D_PRT("del item for no vis>>>");
    }
    updateListView();
    return true;
}

int DyncListView::itemCount(void)
{
    return m_item_count;
}
bool DyncListView::setItemHeight(int height)
{
    m_nm_height = height;
    return true;
}
int DyncListView::itemHeight(void)
{
    return m_nm_height;
}
bool DyncListView::setHilightItemHeight(int height)
{
    m_hi_height = height;
    return true;
}
int DyncListView::hilightItemHeight(void)
{
    return m_hi_height;
}
int DyncListView::hilightItemIndex(void)
{
    return m_hi_index;
}
void DyncListView::setScrollbarWidth(int w)
{
    m_v_scrollbar_width = w;
}
int DyncListView::scrollbarWidth(void)
{
    return m_v_scrollbar_width;
}
void DyncListView::setScrollbarMarginRight(int m)
{
    m_v_scrollbar_margin_right = m;
}
int DyncListView::scrollbarMarginRight(void)
{
    return m_v_scrollbar_margin_right;
}
void DyncListView::setItemTemplateId(int item_res_id)
{
    if (m_item_templete_id == (HTResId)-1)
        m_item_templete_id = item_res_id; 
}

void DyncListView::setItemRect(View * item,int left, int top, int right, int bottom)
{
    item->setRectNoUpdate(left,top,right,bottom);
    /*if(m_histart >= 0 && top < m_histart)
        top = m_histart;
    if(m_hiend >= 0 && bottom > m_hiend)
        bottom = m_hiend;
    if(top <= bottom )
        item->updateView(left,top,right - left + 1,bottom - top + 1);*/
}

void DyncListView::itemLayout(int hilight_index)
{
    if(m_dock_top)
        layoutUpToDown(0,hilight_index);
    else
        layoutDownToUp(-1,hilight_index);
}

void DyncListView::layoutDownToUp(int from_height, int hilight_index)
{
    int h = 0, w = 0, n = 1;
    View* item = lastChild();
    w = getRect().width();
    
    h = (-1 == from_height ? getRect().height() : from_height);
	D_PRT("layoutDownToUp>>>h=%d",h);	
    while(item) {
        if(n == hilight_index) {
            setItemRect(item,0, h - m_hi_height, w, h);
            h -= m_hi_height;
        }
        else {
            setItemRect(item,0, h - m_nm_height, w, h);
            h -= m_nm_height;
        }
        ++n;
        item = item->previousSibling();
    }
}

void DyncListView::layoutUpToDown(int from_height, int hilight_index)
{
    int h = 0, w = 0, n = 1;
    View* item = firstChild();
    h = from_height;
    w = getRect().width();
	D_PRT("layoutUpToDown>>>h=%d.",h);
    while(item) {
         if(n == hilight_index) {
            setItemRect(item,0, h, w, h + m_hi_height);
            h += m_hi_height;
        }
        else {
            setItemRect(item,0, h, w, h + m_nm_height);
            h += m_nm_height;
        }
        ++n;
        item = item->nextSibling();
    }
}
void DyncListView::onLoadItemData(int index, View* view)
{
    // check
    if(index < 0 || index > m_item_count - 1)
        return;

    CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_LOAD_DATA, (int)this);
    event.setExParam((int)view, index);
    raiseEvent(&event);
}

View* DyncListView::firstVisibleItem(void)
{
    return firstChild();
}

View* DyncListView::lastVisibleItem(void)
{
    return lastChild();
}

int DyncListView::getVisibleItemCountNeed(void)
{
    return ((getRect().height() - m_hi_height + (m_nm_height - 1))/m_nm_height + 1);
}

void DyncListView::hilight(int index)
{
    View* last_hilight_item = NULL, *item = NULL;
    bool jump = false;
    IntRect last_hilight_rect, rect;
    ///////////////////// we assume that : all VisibleItem is in screen.
    // check
    if(index < 0 || index >= m_item_count)
        return;

	// if index is current hilight index, we just send a notify.
	if(m_hi_index == index && m_focus) {
		CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_SELCHANGED, (int)this);
		raiseEvent(&event);
		return;
	}

    if (!(m_hi_index - index == 1 || m_hi_index - index == -1))
        jump = true;

    // first time or jump here? request all data, reset m_up_index and m_down_index
    if(-1 == m_hi_index || -2 == m_hi_index || jump) {
        if (m_item_count - index >= getVisibleItemCountNeed()) {//remain is too many one page;
			m_up_index = index;
			m_down_index = m_up_index + getVisibleItemCountNeed() - 1;
        }
        else {//
			m_down_index = m_item_count - 1;
        	m_up_index = m_down_index - m_visible_item_count  + 1;
        }
        
        reLoadData();
        
        itemLayout(index);
		// update for scrollbar
	    updateListView();

        goto hilight_end;
    }

    if (m_hi_index != -1)
        last_hilight_item = getChildByIndex(m_hi_index - m_up_index);

    if (last_hilight_item)
        last_hilight_rect = last_hilight_item->getRect();

    // down item
    if(index > m_down_index) {
        View* first = firstChild();
        // make order, first -> last
        detachChild(first);
        addChild(first);
        D_PRT("add last>>>index=%d,up=%d",index,m_up_index);
        // request data
        onLoadItemData(index, first);
        
        ++m_up_index;
        ++m_down_index;
		// layout
        itemLayout(index);
        updateListView();
    }
    // up item
    else if( index < m_up_index ) {
        View* last = lastChild();
        // make order, first -> last
        detachChild(last);
        addChildHead(last);
        D_PRT("add Head>>>index=%d,up=%d",index,m_up_index);
        
        // request data
        onLoadItemData(index, last);
        --m_up_index;
        --m_down_index;
		// layout
        itemLayout(index);
        updateListView();
    }
    // last visible
    else if(index == m_down_index && m_visible_item_count == getVisibleItemCountNeed()) {
        // layout
        if(m_down_index == m_item_count - 1){
            m_dock_top = false;
            itemLayout(index);
            updateListView();
        }
        else{
            int vbot = getRect().bottom();
            D_PRT("bottom >>>index=%d,up=%d,down=%d",index,m_up_index,m_down_index);    
            item = getChildByIndex(index - m_up_index);
            int bot = item->getRect().bottom();
            if( bot > vbot ){//the last is not full in display;
                int i;
                View* first = firstChild();
                // make order, first -> last
                detachChild(first);
                addChild(first);
                ++m_up_index;
                ++m_down_index;
                if(m_down_index >= m_item_count)
                    i = 0;
                else
                    i = m_down_index;
                onLoadItemData(i, first);
                itemLayout(index);
                updateListView();
            }
            else
                itemLayout(index);
        }
    }
    // first visible
    else if(index == m_up_index) {
        if(index == 0){
            m_dock_top = true;
            itemLayout(index);
            updateListView();
        }
        else{
            item = getChildByIndex(index - m_up_index);
            int top = item->getRect().top();
            D_PRT("top >>>index=%d,up=%d,down=%d",index,m_up_index,m_down_index);     
            if(top < 0){//the top is not full in display
                int i;
                View* last = lastChild();
                detachChild(last);
                addChildHead(last);
                --m_up_index;
                --m_down_index;
                if(m_up_index < 0)
                    i = m_item_count - 1;
                else
                    i = m_up_index;
                onLoadItemData(i, last);
		        // layout
                itemLayout(index);
                updateListView();
            }
            else
                itemLayout(index);
        }
    }
    // visible middle
    else {
        /////////// grow up when move down, grow down when move up
        // move down ?
        item = getChildByIndex(index - m_up_index);
        rect = item->getRect();
        D_PRT("middle >>>index=%d,up=%d,down=%d",index,m_up_index,m_down_index); 
        if(m_hi_index < index) {
            if (last_hilight_item) {
                setItemRect(last_hilight_item,last_hilight_rect.left(), 
                        last_hilight_rect.top(), 
                        last_hilight_rect.right(),
                        last_hilight_rect.top() + m_nm_height); 
            }
            setItemRect(item,rect.left(), rect.bottom() - m_hi_height, rect.right(), rect.bottom());
        }
        // move up ?
        else {
            if (last_hilight_item) {
                setItemRect(last_hilight_item,last_hilight_rect.left(), 
                        last_hilight_rect.bottom() + m_itemRowGap - m_nm_height, 
                        last_hilight_rect.right(),
                        last_hilight_rect.bottom() + m_itemRowGap); 
            }
            setItemRect(item,rect.left(), rect.top(), rect.right(), rect.top() + m_hi_height);
        }
    }

hilight_end:

    // hilight
    item = getChildByIndex(index - m_up_index);
    if(NULL == item)
        item = firstChild();
        
    setHilightView(item);
    m_hi_index = index;
    
    m_focus = true;
    CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_SELCHANGED, (int)this);
    raiseEvent(&event);
}

void DyncListView::setHilightView(View* view)
{
    if(view){
        IntRect rect = view->getRect();
        bool ret = view->setRectNoUpdate(rect.left(), rect.top(), rect.right(), rect.top() + m_hi_height - m_itemRowGap ); 
        setFocusView(view);
        if(ret)
            view->updateView(rect); 
    }
}

bool DyncListView::dispatchEvent(Event* event)
{
    switch (event->eventType()) {
        case Event::KEY_DOWN:
        case Event::KEY_LONGPRESSED:
        case Event::KEY_ALWAYSPRESS:
            {
                int code = ((KeyEvent *)event)->keyCode();
                if ( onKeyPressed (code) ) {
                    return DISPATCH_STOP_MSG;
                }
            }
        default:
            return PanelView::dispatchEvent(event);
    }

    return DISPATCH_CONTINUE_MSG;
}

bool DyncListView::onKeyPressed(int keyCode)
{
	if((!m_focus)&&(keyCode!=KeyEvent::KEYCODE_DOWN))
	{
		return false;
	}
	else if((!m_focus)&&(keyCode==KeyEvent::KEYCODE_DOWN))
	{
		hilight(m_hi_index);
		return true;
	}
    if(keyCode == KeyEvent::KEYCODE_UP) {
        int index = m_hi_index-1 >= 0 ? m_hi_index-1 : m_item_count-1;
        if(index == m_item_count-1)
            m_dock_top = false;
            
        hilight(index);
		return true;
    }
    else if(keyCode == KeyEvent::KEYCODE_DOWN) {
        int index = m_hi_index+1 <= m_item_count-1 ? m_hi_index+1 : 0;
        if(index == 0)
            m_dock_top = true;
            
        hilight(index);
		return true;
	}
	else
	    return false;
}

void DyncListView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
	reLayout();
    PanelView::drawContent(context, rc, status);
}

#define MIN_BAR_OF_BLST  (5<<16)

void DyncListView::drawScroll(GraphicsContext* context, IntRect &rc, int status)
{
    IntRect rcBar, rcBarBk;

    fixed bar_len, bar_pos;

    // need?
    if(m_visible_item_count >= m_item_count)
        return;

    bar_len = itofix(getRect().height()*getRect().height()/(m_nm_height*m_item_count));
    bar_pos = itofix(getRect().height()*m_up_index*m_nm_height/(m_nm_height*m_item_count));

    if(bar_len < MIN_BAR_OF_BLST)
        bar_len = MIN_BAR_OF_BLST;

    context->setMapView(this);

	if (isScrollBarFloating())
	{
		if (ScrollView::IsAllScrollBarOnLeft()) {
			rcBar.setRect(0, 
		            fixceil(bar_pos), 
		            m_v_scrollbar_width, 
		            fixceil(bar_pos)+fixceil(bar_len));
		} else {
		    rcBar.setRect(getRect().width() - m_v_scrollbar_width, 
		            fixceil(bar_pos), 
		            getRect().width(), 
		            fixceil(bar_pos)+fixceil(bar_len));
		}
	    // draw
	    m_drset->draw(context, DR_VSCROLLBAR, DRAWSTATE_NORMAL, rcBar);
	} else {
		if (ScrollView::IsAllScrollBarOnLeft()) {
			rcBarBk.setRect(0, 0, m_v_scrollbar_width,  rc.height());
		} else {
			rcBarBk.setRect(getRect().width() - m_v_scrollbar_width, 0, getRect().width(),	rc.height());
		}
		// draw scroll background
	    m_drset->draw(context, DR_BGVSCROLLBAR, DRAWSTATE_NORMAL, rcBarBk);
		
		rcBar.setRect(rcBarBk.left() + m_v_scrollbar_margin_right, 
	            fixceil(bar_pos), 
	            rcBarBk.right() - m_v_scrollbar_margin_right, 
	            fixceil(bar_pos) + fixceil(bar_len));
	    // draw scroll bar
	    m_drset->draw(context, DR_VSCROLLBAR, DRAWSTATE_NORMAL, rcBar);
	}
}


} // namespace hfcl {
