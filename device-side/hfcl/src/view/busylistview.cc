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


#include "common/event.h"
#include "graphics/graphicscontext.h"
#include "drawable.h"
#include "panelview.h"
#include "itemview.h"
#include "resource/respkgmanager.h"
#include "scrollview.h"
#include "bitmark.h"
#include "busylistview.h"

#include "hal_misc.h"

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

 
BusyListView::BusyListView(View* p_parent)
    :PanelView(p_parent)
{
#ifdef __MMI_SAMSUNG_GT_FEATURE__
	setScrollBarFloating(false);
#endif
    init();
}

BusyListView::BusyListView(View* p_parent, DrawableSet* drset)
    :PanelView(p_parent, drset)
{
#ifdef __MMI_SAMSUNG_GT_FEATURE__
	setScrollBarFloating(false);
#endif
    init();
}

BusyListView:: ~BusyListView()
{
}

void BusyListView::reInit(void)
{
    m_item_count = 0;
    m_visible_item_count = 0;
    // this mark clean then add, not first time add.
    m_hi_index = -2;
    m_up_index = -1;
    m_itemRowGap = 0;
    m_down_index = -1;
    m_items_enough = false;
	m_freeze = false;
	m_scrollGap = 0;
}

void BusyListView::init(void)
{
    m_item_count = 0;
    m_visible_item_count = 0;
    m_item_templete_id = -1;
    m_nm_height = 40;
    m_hi_height = 40;
    m_hi_index = -1;
    m_up_index = -1;
    m_itemRowGap = 0;
    m_down_index = -1;
	m_special_item_index = -1;
    m_items_enough = false;
    m_v_scrollbar_width = DEF_SCROLL_WIDTH;
    m_v_scrollbar_margin_right = DEF_SCROLL_MARGIN;
	m_freeze = false;
	m_focus = true;
}

int BusyListView::reLoadData(void)
{
    View* item = NULL;
    int i = 0;
    // check
    if(m_item_count <= 0 || m_visible_item_count <= 0)
        return 0;

    item = firstChild();
    for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
        onLoadItemData(i, item);
    }
	return 0;
}

void BusyListView::clear(void)
{
	clean();
    reInit();
}
void BusyListView::initSelectMark(int count)
{
	if(m_select_mark.count() > 0)
		return;
	m_select_mark.init(count);
}

bool BusyListView::isItemSelected(int index)
{
	return m_select_mark.getMark(index);
}

void BusyListView::selectItem(int index, bool sel)
{
	m_select_mark.setMark(index, sel);
}

void BusyListView::select(int index, bool sel)
{
	ItemView* item = (ItemView*)itemFromIndex(index);
	selectItem(index, sel);

	if(item) {
		item->setSelected(sel);
	}
}

void BusyListView::selectAll(bool sel)
{
    for(int i = 0; i < itemCount(); i++){
        if(i < getVisibleItemCountNeed()){
            ItemView* item = (ItemView*)(getChildByIndex(i));
            item->setSelected(sel);
        }
        
	    selectItem(i, sel);
    }
    updateView();
}


void BusyListView::initItemAddData(int count)
{
	if(m_data_keeper.count() > 0)
		return;
	
	m_data_keeper.init(count);
}

void BusyListView::setItemAddData(int index, void* data)
{
	m_data_keeper.setPointer(index, data);
}

void* BusyListView::itemAddData(int index)
{
	return m_data_keeper.getPointer(index);
}

void BusyListView::freeze(bool f)
{
	//TODO
	m_freeze = f;
}

View* BusyListView::hilightItem(void)
{
	return getChildByIndex(m_hi_index - m_up_index);
}

View* BusyListView::itemFromIndex(int index)
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

bool BusyListView::addItemByIndex(int index, void* add_data)
{
	// check
	if(m_data_keeper.count() < 1)
		return false;

	// adjust data before set data
	if(m_data_keeper.count() > 0) {
		m_data_keeper.move(index, index+1, -1);
	}

	m_data_keeper.setPointer(index, add_data);

	// first add
	addItemByIndex(index);
	return true;
}


bool BusyListView::addItemByIndex(int index)
{
    View* item = NULL;
    int i = 0;
    // check range
    if(index < 0 || index > m_item_count)
        return false;

    // add after m_down_index? just addItem
    if (index > m_down_index)
        return addItem();

    // when m_up_index == 0, we need NOT to adjust cursor.
    if (0 != m_up_index) {
        // if index <= m_up_index, we need to adjust cursor
        if (index <= m_up_index) {
            // adjust cursor
            --m_up_index;
            --m_down_index;
            --m_hi_index;
        }
        // if index > m_up_index, we need to judge index and m_hi_index
        /*
        else {
            if (index <= m_hi_index) {
                --m_hi_index;
            }
        }
        */
    }

    ++m_item_count;
    if(!m_items_enough) {
        prepareVisibleItems();
    }

	// adjust selec status
	if(m_select_mark.count() > 0) {
		m_select_mark.move(index, index+1, -1);
		m_select_mark.setMark(index ,false);
	}
    
    // we need to adjust when non-full list and we have hilight one
    if(m_visible_item_count >= m_item_count && m_hi_index != -2) {
        layoutUpToDown(0, m_hi_index-m_up_index+1);
        ++m_down_index;
    }

    // load
    if (!m_freeze) {
	    item = firstChild();
	    for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
	        onLoadItemData(i, item);
	    }

	    // we need hilight 0 if clean then add
	    if(m_hi_index == -2)
	        hilight(0);

	    // update for scrollbar
	    updateView();
    }

	reLayout();
	
	return true;
}

bool BusyListView::addItem(void* add_data)
{
	// check
	if(m_data_keeper.count() < 1)
		return false;

	// add data !!! we need do it first.
	m_data_keeper.setPointer(m_item_count, add_data);

	// first add
	addItem();
	return true;
}

void BusyListView::reLayout(void)
{
	IntRect listRc = getRect();
	
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
		IntRect rc = view->getRect();;

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
		view->setRect(rc);

		view = view->nextSibling();
	}
}


bool BusyListView::addItem(void)
{
    View* item = NULL;
    int i = 0;
    // counter
    ++m_item_count;
    if (!m_items_enough)
        prepareVisibleItems();

	// adjust selec status
	if(m_select_mark.count() > 0) {
		m_select_mark.setMark(m_item_count - 1 ,false);
	}
	
    // we need to adjust when non-full list and we have hilight one
    if(m_visible_item_count >= m_item_count && m_hi_index != -2) {
        layoutUpToDown(0, m_hi_index-m_up_index+1);
        ++m_down_index;
    }

    // load
    if (!m_freeze) {
	    item = firstChild();
	    for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
			onLoadItemData(i, item);
	    }

	    // we need hilight 0 if clean then add
	    if(m_hi_index == -2)
	        hilight(0);

	    // update for scrollbar
	    updateView();
   	}

	//reLayout();
    return true;
}

bool BusyListView::removeItem(int index)
{
    int i = 0;
    View* item = NULL;
    // right range?
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

    // visible ?
    if (index >= m_up_index && index <= m_down_index) {
        // TODO
        // full list -> non-full list
        if(m_visible_item_count >= m_item_count) {
            // 1. remove a visible item
            removeChild(lastChild());
            // 2. m_hi_index is last?move up
            // 3. sub m_down_index
            --m_down_index;
            // 4. adjust data
            --m_item_count;
            --m_visible_item_count;
            //... we need add real item next time addItem
            m_items_enough = false;
                        
            if(m_hi_index == m_down_index){
                setHilightView(getChildByIndex(m_hi_index-m_up_index - 1));
                --m_hi_index;
            }
            else
               setHilightView(getChildByIndex(m_hi_index-m_up_index)); 
               
            // 5. top layout
            layoutUpToDown(0, m_hi_index-m_up_index+1);
            // 6. load data
            item = firstChild();
            for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
                onLoadItemData(i, item);
            }

        }
        // full list -> full list
        else {
            // down index is last one? or index to remove is m_up_index? we need to adjust cursor
            if(m_down_index == m_item_count -1 || (index == m_up_index && 0 != m_up_index)) {
                --m_up_index;
                --m_down_index;
                setHilightView(getChildByIndex(m_hi_index-m_up_index-1));
                --m_hi_index;
            }
            else
                setHilightView(getChildByIndex(m_hi_index-m_up_index));

            --m_item_count;
            
            // load data FIXME we can optimize this.
            item = firstChild();
            for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
                onLoadItemData(i, item);
            }
        }
    }
    // non-visible
    else {
        --m_item_count;
        if(index < m_up_index) {
            --m_up_index;
            --m_down_index;
            --m_hi_index;

            // load data and non-need to layout item
            item = firstChild();
            for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
                onLoadItemData(i, item);
            }
        }
    }

    // for scrollbar
    updateView();

    // mark
    if(m_item_count == 0)
        reInit();

    return true;
}

int BusyListView::itemCount(void)
{
    return m_item_count;
}
bool BusyListView::setItemHeight(int height)
{
    m_nm_height = height;
    return true;
}
int BusyListView::itemHeight(void)
{
    return m_nm_height;
}
bool BusyListView::setHilightItemHeight(int height)
{
    m_hi_height = height;
    return true;
}
int BusyListView::hilightItemHeight(void)
{
    return m_hi_height;
}
int BusyListView::hilightItemIndex(void)
{
    return m_hi_index;
}
void BusyListView::setScrollbarWidth(int w)
{
    m_v_scrollbar_width = w;
}
int BusyListView::scrollbarWidth(void)
{
    return m_v_scrollbar_width;
}
void BusyListView::setScrollbarMarginRight(int m)
{
    m_v_scrollbar_margin_right = m;
}
int BusyListView::scrollbarMarginRight(void)
{
    return m_v_scrollbar_margin_right;
}
void BusyListView::setItemTemplateId(int item_res_id)
{
    if (m_item_templete_id == (HTResId)-1)
        m_item_templete_id = item_res_id; 
}

void BusyListView::layoutDownToUp(int from_height, int hilight_index)
{
    int h = 0, w = 0, n = 1;
    View* item = lastChild();
    w = getRect().width();
    //h = getRect().height();
    h = (-1 == from_height ? getRect().height() : from_height);
	
	View* fixedItem = NULL;

	if(m_special_item_index >= 0 && m_special_item_index >= m_up_index && m_special_item_index <= m_down_index)
	{
		fixedItem = getChildByIndex(m_special_item_index - m_up_index);
	}
		
    while(item) {

		if(m_special_item_index >= 0 && fixedItem == item )
		{
		    item->setRect(0, h- m_nm_height/2, w, h);
            h -= m_nm_height/2;
		}
		else
		{
	        if(n == hilight_index) {
	            item->setRect(0, h - m_hi_height, w, h);
	            h -= m_hi_height;
	        }
	        else {
	            item->setRect(0, h - m_nm_height, w, h);
	            h -= m_nm_height;
	        }
		}
        ++n;
        item = item->previousSibling();
    }
    // mark dock status
    m_dock_top = false;
}

void BusyListView::layoutUpToDown(int from_height, int hilight_index)
{
    int h = 0, w = 0, n = 1;
    View* item = firstChild();
    h = from_height;
    w = getRect().width();
	View* specialItem = NULL;
 
	if(m_special_item_index >= 0 && m_special_item_index >= m_up_index && m_special_item_index <= m_down_index)
	{
		specialItem = getChildByIndex(m_special_item_index - m_up_index);
	}
		
    while(item) {
		if(m_special_item_index >= 0 && specialItem == item)
		{
				item->setRect(0, h, w, h + m_nm_height/2);
            	h += m_nm_height/2;
		}
		else
		{
	         if(n == hilight_index) {
	            item->setRect(0, h, w, h + m_hi_height);
	            h += m_hi_height;
	        }
	        else {
	            item->setRect(0, h, w, h + m_nm_height);
	            h += m_nm_height;
	        }
		} 
        ++n;
        item = item->nextSibling();
    }
    // mark dock status
    m_dock_top = true;
}
void BusyListView::onLoadItemData(int index, View* view)
{
    // check
    if(index < 0 || index > m_item_count - 1)
        return;

    CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_LOAD_DATA, (int)this);
    event.setExParam((int)view, index);
    raiseEvent(&event);
}

View* BusyListView::firstVisibleItem(void)
{
    return firstChild();
}

View* BusyListView::lastVisibleItem(void)
{
    return lastChild();
}

int BusyListView::getVisibleItemCountNeed(void)
{
    return ((getRect().height() - m_hi_height + (m_nm_height - 1))/m_nm_height + 1);
}

void BusyListView::hilight(int index)
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
        int i = 0;
        // not enough one screen
        if(m_visible_item_count < getVisibleItemCountNeed()) {
            m_up_index = 0;
            // m_down_index = m_visible_item_count;
            m_down_index = m_visible_item_count - 1;
            // from height NOT bottom
            layoutUpToDown(0, index+1);
        }
        // if from index to end can NOT fill a screen, we count from list bottom item
        // or we put index item the top
        else {
            // fill a screen
            if (m_item_count - index >= getVisibleItemCountNeed()) {
#ifdef __MMI_SPREADSPECTRUM_STYLE__
				if(index > 0 && (index +1) != m_item_count)
				{
					m_up_index = index -1; // it set hilight index is 1, show the first item.
					m_down_index = m_up_index + getVisibleItemCountNeed() - 1;
					layoutUpToDown(0,2);
				}
				else  // it is other style 
#endif	
				{				
					m_up_index = index;
					m_down_index = m_up_index + getVisibleItemCountNeed() - 1;
					layoutUpToDown();
				}
            }
            // NOT fill a screen
            else {
#ifdef __MMI_SPREADSPECTRUM_STYLE__
				if(index > 0 && (index +1) != m_item_count)
				{
					m_up_index = index -1; // it set hilight index is 1, show the first item.
					m_down_index = m_up_index + getVisibleItemCountNeed() - 1;
					layoutUpToDown(0,2);
				}
				else
#endif
				{
					m_down_index = m_item_count - 1;
                	m_up_index = m_down_index - m_visible_item_count  + 1;
					
                layoutDownToUp(getRect().height(), 
                        m_visible_item_count - (getVisibleItemCountNeed() - (m_item_count - index)));
                // from height bottom
				}
            }
        }

        // item = getChildByIndex(m_up_index);
        item = firstChild();
        // request all data
        for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
            onLoadItemData(i, item);
        }

		// update for scrollbar
	    updateView();

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

        // request data
        onLoadItemData(index, first);
        ++m_up_index;
        ++m_down_index;
		// layout
        layoutDownToUp();
    }
    // up item
    else if(index < m_up_index) {
        View* last = lastChild();
        // make order, first -> last
        detachChild(last);
        addChildHead(last);

        // request data
        onLoadItemData(index, last);
        --m_up_index;
        --m_down_index;

		// layout
        layoutUpToDown();
    }
    // last visible
    else if(index == m_down_index && m_visible_item_count == getVisibleItemCountNeed()) {
        // layout
        layoutDownToUp();
    }
    // first visible
    else if(index == m_up_index) {
        // layout
        layoutUpToDown();
    }
    // visible middle
    else {
        item = getChildByIndex(index - m_up_index);
        rect = item->getRect();
        /////////// grow up when move down, grow down when move up
        // move down ?
        if(m_hi_index < index) {
            if (last_hilight_item) {
                last_hilight_item->setRect(last_hilight_rect.left(), 
                        last_hilight_rect.top(), 
                        last_hilight_rect.right(),
                        last_hilight_rect.top() + m_nm_height); 
            }

            item->setRect(rect.left(), rect.bottom() - m_hi_height, rect.right(), rect.bottom());
        }
        // move up ?
        else {
            if (last_hilight_item) {
                last_hilight_item->setRect(last_hilight_rect.left(), 
                        last_hilight_rect.bottom() + m_itemRowGap - m_nm_height, 
                        last_hilight_rect.right(),
                        last_hilight_rect.bottom() + m_itemRowGap); 
            }

            item->setRect(rect.left(), rect.top(), rect.right(), rect.top() + m_hi_height);
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

void BusyListView::setHilightView(View* view)
{
    IntRect rect = view->getRect();
    view->setRect(rect.left(), rect.top(), rect.right(), rect.top() + m_hi_height - m_itemRowGap ); 
    setFocusView(view);
}

void BusyListView::hilight(int index, bool focus)
{
    View* last_hilight_item = NULL, *item = NULL;
    bool jump = false;
    IntRect last_hilight_rect, rect;
    ///////////////////// we assume that : all VisibleItem is in screen.
    // check
    if(index < 0 || index >= m_item_count)
        return;

	m_focus = focus;
	// if index is current hilight index, we just send a notify.
	if(m_hi_index == index) {
		CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_SELCHANGED, (int)this);
		raiseEvent(&event);
		return;
	}

    if (!(m_hi_index - index == 1 || m_hi_index - index == -1))
        jump = true;

    // first time or jump here? request all data, reset m_up_index and m_down_index
    if(-1 == m_hi_index || -2 == m_hi_index || jump) {
        int i = 0;
        // not enough one screen
        if(m_visible_item_count < getVisibleItemCountNeed()) {
            m_up_index = 0;
            // m_down_index = m_visible_item_count;
            m_down_index = m_visible_item_count - 1;
            // from height NOT bottom
            layoutUpToDown(0, index+1);
        }
        // if from index to end can NOT fill a screen, we count from list bottom item
        // or we put index item the top
        else {
            // fill a screen
            if (m_item_count - index >= getVisibleItemCountNeed()) {
                m_up_index = index;
                m_down_index = index + getVisibleItemCountNeed() - 1;
                layoutUpToDown();
            }
            // NOT fill a screen
            else {
                m_down_index = m_item_count - 1;
                m_up_index = m_down_index - m_visible_item_count  + 1;
                // from height bottom
                layoutDownToUp(getRect().height(), 
                        m_visible_item_count - (getVisibleItemCountNeed() - (m_item_count - index)));
            }
        }

        // item = getChildByIndex(m_up_index);
        item = firstChild();
        // request all data
        for(i = m_up_index; i <= m_down_index && item; ++i, item = item->nextSibling()) {
            onLoadItemData(i, item);
        }
		
		// update for scrollbar
	    updateView();

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

        // request data
        onLoadItemData(index, first);
        // layout
        layoutDownToUp();
        ++m_up_index;
        ++m_down_index;
    }
    // up item
    else if(index < m_up_index) {
        View* last = lastChild();
        // make order, first -> last
        detachChild(last);
        addChildHead(last);

        // request data
        onLoadItemData(index, last);
        // layout
        layoutUpToDown();
        --m_up_index;
        --m_down_index;
    }
    // last visible
    else if(index == m_down_index && m_visible_item_count == getVisibleItemCountNeed()) {
        // layout
        layoutDownToUp();
    }
    // first visible
    else if(index == m_up_index) {
        // layout
        layoutUpToDown();
    }
    // visible middle
    else {
        item = getChildByIndex(index - m_up_index);
        rect = item->getRect();
        /////////// grow up when move down, grow down when move up
        // move down ?
        if(m_hi_index < index) {
            if (last_hilight_item) {
                last_hilight_item->setRect(last_hilight_rect.left(), 
                        last_hilight_rect.top(), 
                        last_hilight_rect.right(),
                        last_hilight_rect.top() + m_nm_height ); 
            }

            item->setRect(rect.left(), rect.bottom() - m_hi_height, rect.right(), rect.bottom());
        }
        // move up ?
        else {
            if (last_hilight_item) {
                last_hilight_item->setRect(last_hilight_rect.left(), 
                        last_hilight_rect.bottom() - m_nm_height + m_itemRowGap, 
                        last_hilight_rect.right(),
                        last_hilight_rect.bottom() + m_itemRowGap); 
            }

            item->setRect(rect.left(), rect.top(), rect.right(), rect.top() + m_hi_height );
        }
    }

hilight_end:

    // hilight
    item = getChildByIndex(index - m_up_index);
    if(NULL == item)
        item = firstChild();

    if(m_focus)
    	setHilightView(item);
    m_hi_index = index;
    
    CustomEvent event(Event::CUSTOM_NOTIFY, (int)CustomEvent::CUS_SELCHANGED, (int)this);
    raiseEvent(&event);
}
void BusyListView::prepareVisibleItems(bool add_one, bool clean_all)
{
    int c = 0, n = 0;
    IntRect new_rc, last_rc;

	// we clean all first
    if(clean_all) {
        clean();
        m_visible_item_count = 0;
    }

    // calculate how many item to be visible FIXME
    // n = (getRect().height() - m_hi_height)/m_nm_height + 2;
    n = getVisibleItemCountNeed();

    // no visible item
    if(n < 0)
        return;

    // item enough
    if(m_visible_item_count >= n)
        return;

    // re-add visible item
    if(m_item_templete_id != (HTResId)-1) {
        //add all here
        if (!add_one) {
            for(c = 0; c < n; ++c) {
                ////////////////////////// parent is me, no viewcontext, no contentprovider
                CreateViewFromRes(m_item_templete_id, this, NULL, NULL);
                m_visible_item_count = n;
            }
        }
        // add one
        else {
            CreateViewFromRes(m_item_templete_id, this, NULL, NULL);
            ++m_visible_item_count;
        }
    }

    if(m_visible_item_count == n)
        m_items_enough = true;
	
}

bool BusyListView::dispatchEvent(Event* event)
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

bool BusyListView::onKeyPressed(int keyCode)
{
	if((!m_focus)&&(keyCode!=KeyEvent::KEYCODE_CURSOR_DOWN))
	{
		return false;
	}
	else if((!m_focus)&&(keyCode==KeyEvent::KEYCODE_CURSOR_DOWN))
	{
		hilight(m_hi_index);
		return true;
	}
    if(keyCode == KeyEvent::KEYCODE_CURSOR_UP) {
        hilight( m_hi_index-1 >= 0 ? m_hi_index-1 : m_item_count-1 );
		return true;
    }
    else if(keyCode == KeyEvent::KEYCODE_CURSOR_DOWN) {
        hilight(m_hi_index+1 <= m_item_count-1 ? m_hi_index+1 : 0 );
		return true;
	}
	else
	    return false;
}

void BusyListView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
	reLayout();
    PanelView::drawContent(context, rc, status);
}

#define MIN_BAR_OF_BLST  (5<<16)

void BusyListView::drawScroll(GraphicsContext* context, IntRect &rc, int status)
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
