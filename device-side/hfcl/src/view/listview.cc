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

#undef _DEBUG

#include "view/listview.h"

#include <minigui/fixedmath.h>

#include "graphics/graphicscontext.h"
#include "services/timerservice.h"

namespace hfcl {

void ListView::init(void)
{
    //m_separator = NULL;
    //separatorw = 0;
    m_vGap = 0;
    m_hGap = 0;
    m_separatorWidth = 1;
    m_scrollGap = 0;
    //m_headGap = 0;
    //m_tailGap = 0;

    m_hilightItem = NULL;
    m_curr_single_selected = NULL;
}

ListView::ListView(View *p_parent)
    :ScrollView(p_parent, DEFAULT_VIEW_DRAWABLESET(ListView))
 //   , m_lstTimerId(0)     // GT_jyseo rollback
 //   , m_pressedKey(0)     // GT_jyseo rollback
{
    init();
}

ListView::ListView(View *p_parent, DrawableSet* drset)
    :ScrollView(p_parent, drset)
//    , m_lstTimerId(0)     // GT_jyseo rollback
//    , m_pressedKey(0)     // GT_jyseo rollback
{
    init();
}

ListView::~ListView(void)
{
#if 0
    for (AniList::iterator it = m_aniList.begin(); it != m_aniList.end();) 
    {
        MGEFF_ANIMATION _a = *it;
        it++;
        if (_a /*&& MGEFF_STATE_RUNNING == mGEffAnimationGetProperty(_a, MGEFF_PROP_STATE)*/)
        {
            mGEffAnimationStop(_a);
        }
    }
    m_aniList.clear();
#endif
}

bool ListView::addItemByIndex(ItemView *item, int index)
{
    if (!m_content)
        return false;

    if (m_content->insertBefore(index, item))
        updateContent();
    return true;
}

bool ListView::addItemWithAppend(ItemView *item)
{
    int index;
    
    if(!m_content || !item)
        return false;
    
    index = itemCount() -1;
    index = index <0?0:index;
    
    if(m_content->insertBefore(index, (View*)item))
        updateContent();
    return true;
}

bool ListView::removeItem(ItemView* item, bool bRelease)
{
    if (m_content && m_content->removeChild(item, bRelease) == 0) {
        if (m_hilightItem == item)
            m_hilightItem = NULL;
        updateContent();
        return true;
    }

    return false;
}

void ListView::freeze(bool b_freeze)
{
    setFreezed(b_freeze);
    updateContent();
}

void ListView::clear(void)
{
    if (m_content) {
        m_content->clean();
        m_hilightItem = NULL;
        m_curr_single_selected = NULL;
        //updateContent(); // by dlzhong.
    }
}

bool ListView::hilight(ItemView* view)
{
    // _DBG_PRINTF("ListView::hilight  %p   %p   ",view,m_hilightItem)
    // _DBG_PRINTF("ListView::hilight  %p   %p   ",m_content,m_content->isChild(view))
    if (view == m_hilightItem)
        return true;
    if (!m_content || !m_content->isChild(view)) {
        return false;
    }

    if (m_hilightItem) {
        m_hilightItem->updateView();
        //m_hilightItem->setHilight(false);
    }

    m_hilightItem = view;

    //m_hilightItem->setHilight(true);

    m_content->setFocusView(m_hilightItem);

    if (m_hilightItem) {
        m_hilightItem->updateView();
    }

    makeHilightVisible();

    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)NOTIFY_SELCHANGED, (HTData)this);
    raiseEvent(&event);
    return true;
}

void ListView::hideAll(void)
{
    if (m_content)
        m_content->hide();
}

void ListView::upFindItem(void)
{
    View *view = NULL;
    if (!m_content)
        return;

    if (NULL == m_hilightItem)
        view = m_content->lastChild();
    else {
        view = m_hilightItem->previousSibling();
        if (NULL == view)
            view = m_content->lastChild(); //loop selection
    }

    hilight((ItemView*)view);

    if(!view->isVisible()|| !view->focusValid()) { 
        upFindItem();
    }
}

void ListView::downFindItem(void)
{
    View *view = NULL;

    if (!m_content)
        return ;

    if (NULL == m_hilightItem)
        view = m_content->firstChild();
    else {
        view = m_hilightItem->nextSibling();
        if (NULL == view)
            view = m_content->firstChild();
    }

    hilight((ItemView*)view);
    
    if(!view->isVisible() || !view->focusValid()) {
        downFindItem();
    }
}

bool ListView::singleSelect(int index)
{
    ItemView *view = itemFromIndex(index);

    if (view) {

        if (m_curr_single_selected == view) {
            return false;
        }

        view->setSelected(true);

        if (NULL == m_curr_single_selected) {
            m_curr_single_selected = view;
        }
        else
            m_curr_single_selected->setSelected(false);

        m_curr_single_selected = view;

        return true;
    }
    else
        return false;
}

bool ListView::select(int index, bool toSelect)
{
    ItemView *view = itemFromIndex(index);
    if (view) {
        view->setSelected(toSelect);
        return true;
    }
    else
        return false;
}

void ListView::selectAll(bool bselect)
{
    if (!m_content)
        return;

    for (View *view = m_content->firstChild(); view ; view = view->nextSibling()) {
        ((ItemView*)view)->setSelected(bselect);
    }

    updateView();
}

void ListView::setSeparatorWidth(int width)
{
    if ( width < 0)
        return;

    int w, h;

    getSize(&w, &h);

    if (width <= w && width <= h) {
        m_separatorWidth = (Uint8)width;
    }
}


void ListView::drawSeparator(GraphicsContext* context, int status)
{
    int separatorw = m_separatorWidth;
    if (!m_content || separatorw < 1 )
        return;

    View* view = NULL;
    int bottom = getRect().height();

    IntRect rcSpec( m_hGap, 0, getRect().width(), m_rect.width() - m_hGap);
    for (view = m_content->firstChild(); view; view = view->nextSibling()) {
        int top = view->getRect().bottom() + ((m_vGap) - (m_vGap >> 1)) - m_offy;
        if (top < 0)
            continue;

        rcSpec.m_top = top;
        rcSpec.m_bottom = rcSpec.top() + separatorw;
        //m_separator->draw(context, rcSpec, this, NULL);
        //m_drawable->draw(context, DRAW_SEPARATOR, rcSpec);
        m_drset->draw(context, DR_SEPARATOR, DRAWSTATE_NORMAL, rcSpec);

        if (top >= bottom)
            break;
    }

    //draw from first item
    if (loopScroll() && rcSpec.bottom() < bottom) {
        view = m_content->firstChild();
        rcSpec.m_top = rcSpec.bottom() + (m_vGap>>1);
        while (view) {
            rcSpec.m_top  += view->getRect().height() + m_vGap - (m_vGap >> 1);
            rcSpec.m_bottom = rcSpec.top() + separatorw;

            //m_drawable->draw(context, DRAW_SEPARATOR, rcSpec);
            m_drset->draw(context, DR_SEPARATOR, DRAWSTATE_NORMAL, rcSpec);

            rcSpec.m_top = rcSpec.bottom() + (m_vGap>>1);

            if (rcSpec.top() >= bottom)
                break;

            view = view->nextSibling();
        }
    }
}

void ListView::drawCenterHilight(GraphicsContext* gc)
{
    if (!loopScroll() || !m_hilightItem)
        return ;

    DrawableSet* drset = m_hilightItem->getDrawableSet();
    if (drset) {
        int h;
        IntRect rc(m_hGap, getRect().height() / 2, getRect().width() - m_hGap, 0);
        m_hilightItem->getSize(NULL, &h);
        rc.m_top -= h/2;
        rc.m_bottom = rc.top() + h;

        drset->draw(gc, DR_HILIGHT, DRAWSTATE_HILIGHT, rc);
        if (m_hilightItem->isSelected()) {
            drset->draw(gc, DR_HILIGHT, DRAWSTATE_SELECTED, rc);
        }
    }
}

void ListView::drawContent(GraphicsContext* context, IntRect &rc, int status )
{
    if (!hilightTop())
        drawCenterHilight(context);

    ScrollView::drawContent(context, rc, status | (loopScroll() ? PAINT_NO_HILIGHT: 0));

    //reset map view
    context->setMapView(this);
    drawSeparator(context, status);

    if (hilightTop())
        drawCenterHilight(context);
}

bool ListView::dispatchEvent(Event* event)
{
    if (operationMode() == OPM_BROWSER)
        return ScrollView::dispatchEvent(event);

    switch (event->eventType()) {
        case Event::KEY_ALWAYSPRESS:
        case Event::KEY_DOWN:
        {
            int code = ((KeyEvent *)event)->keyCode();
            if ( DISPATCH_STOP_MSG == onKeyPressed (code) ) {
                return DISPATCH_STOP_MSG;
            }
        }
        default:
            return PanelView::dispatchEvent(event);
    }

    return DISPATCH_STOP_MSG;
}

void ListView::updateContent()
{
    if (freezed())
        return ;

    relayout();
    makeHilightVisible();
}

void ListView::relayout(bool isScrollGap)
{
    if (!m_content)
        return;

    IntRect rc(0, 0, 0, 0);
    int maxwidth = 0;
    int vgap = m_vGap + m_separatorWidth;
    int first_half_vgap = m_vGap >> 1;
    int last_half_vgap = vgap - first_half_vgap;

    fixed conth;
    fixed viewh;

    IntRect listRc = getRect();

    //vieww = itofix(m_rect.width());
    viewh = itofix(m_rect.height());
    //contw = itofix(m_content->getRect().width());
    conth = itofix(m_content->getRect().height());
    
    for (View* view = m_content->firstChild(); view; view = view->nextSibling()) {
        /*
         * Every Item has the same height:
         *   item.height() + m_vGap + separatorw
         *
         *  ----------------------------
         *      first_half_vgap
         *  ---------------------------
         *  |                         |
         *  |  item height            |
         *  |                         |
         *  ---------------------------
         *      last_half_vgap
         *  ==========================    <-- separatorw
         *
         */
        int w, h;

        if(!view->isVisible())   // add by xulei 2012,03,31, hide disvsible Itemview
            continue;

        view->getSize(&w, &h);
        rc.m_top = rc.bottom() + first_half_vgap;
        rc.m_bottom = rc.top() + h;

        if(vBarVisible() && conth > viewh) {
            if(m_scrollGap > 0) {
                if(isVScrollBarOnLeft()) {
                    rc.m_left = listRc.m_left + (m_barWidth+1) + m_scrollGap;
                    rc.m_right = listRc.m_right ;                        
                }
                else {
                    rc.m_right = listRc.m_right - (m_barWidth+1) -m_scrollGap;
                    rc.m_left = listRc.m_left;
                }
            }
            else {
                rc.m_right = rc.left() + w;
            }
        }
        else {
            rc.m_right = rc.left() + w;
        }
        view->setRect(rc);
        rc.m_bottom += last_half_vgap;
        if (maxwidth < rc.right())
            maxwidth = rc.right();
    }

    rc.m_left = 0;
    rc.m_right = maxwidth;
    rc.m_top = 0;
    // maybe this?
    // see ScrollView.moveViewport interface comment
    /*
       rc.m_left = -m_offx;
       rc.m_right = maxwidth;
       rc.m_top = -m_offy;
       */

    m_content->setRect(rc);
}

void ListView::makeHilightVisible()
{
    if (!m_content || !m_hilightItem)
        return;

    int offy = m_offy;
    int separatorw = m_separatorWidth;

    if (loopScroll()) {
        const IntRect& rc = m_hilightItem->getRect();
        offy = (rc.top() + rc.bottom() -  getRect().height()) / 2;
        if (scrollAnimation()) {
            animationScroll(offy);
            return;
        }
    }
    else {
        int vgap = m_vGap + separatorw;
        IntRect rc = m_hilightItem->getRect();
        rc.m_top -= vgap;
        rc.m_bottom += vgap;
        int h;
        getSize(NULL, &h); //get the viewport size

        if ((offy + h) < rc.m_bottom) {
            offy = rc.m_bottom - h;
        }

        if (offy > rc.m_top) {
            offy = rc.m_top;
        }
    }

    // VincentWei: reduce over-redraw
    if (offy != m_offy) {
        _DBG_PRINTF ("ListView::makeHilightVisible: calling moveViewport to %d, %d", m_offx, offy);
        moveViewport (m_offx, offy);
    }
}

ItemView* ListView::itemAt(int x, int y, int* pidx) const
{
    if (pidx)
        *pidx = -1;

    if (!m_content)
        return NULL;

    x += m_offx;
    y += m_offy;

    int right = getRect().width() + m_offx;
    int bottom = getRect().height() + m_offy;

    if (x < 0 || y < 0 || x > right || y > bottom)
        return NULL;

    int idx = 0;
    for (View* view = m_content->firstChild(); view; view = view->nextSibling(), idx++) {
        if (view->getRect().isIn(x, y)) {
            if (pidx)
                *pidx = idx;
            return (ItemView*)view;
        }

        if (y < view->getRect().bottom())
            return NULL;
    }

    return NULL;
}


ItemView* ListView::selectedItem(int* pidx) const
{
    if (pidx)
        *pidx = -1;
    if (!m_content)
        return NULL;

    int idx = 0;
    for (ItemView* view = firstItem(); view; view = (ItemView*)view->nextSibling(), idx++) {
        if (view->isSelected()) {
            if (pidx)
                *pidx = idx;
            return view;
        }
    }

    return NULL;
}

static View* get_next_focus(ContainerView* c);

static View* get_next_focus_from_focused(ContainerView* c)
{
    if (!c)
        return NULL;

    if (c->focusStopable())
        return NULL;

    View *view = c->focusView();

    if (!view)
        return get_next_focus(c);

    if (view->isContainerView()) {
        View* t = get_next_focus_from_focused((ContainerView*)view);
        if (t)
            return t;
    }

    view = view->nextSibling();

    while (view) {
        if (view->focusStopable())
            return view;
        else if (view->isContainerView()) {
            View* t = get_next_focus_from_focused((ContainerView*)view);
            if (t) return t;
        }
        view = view->nextSibling();
    }

    return NULL;
}

static View* get_next_focus(ContainerView* c)
{
    if (!c)
        return NULL;
    if (c->focusStopable())
        return c;

    View* view = c->firstChild();

    while(view) {
        if (view->focusStopable())
            return view;
        else if (view->isContainerView()) {
            View* t = get_next_focus((ContainerView*)view);
            if (t) return t;
        }
        view = view->nextSibling();
    }

    return NULL;
}

bool ListView::nextFocus()
{
    if (focusable()) {
        View *view;
        ItemView *item;
        int loop = 0;

Next:
        loop++;
        //  _DBG_PRINTF("ListView::nextFocus  %d  ",loop)
        if (loop > 2)
            return false;
        //    _DBG_PRINTF("ListView::nextFocus  m_hilightItem %d  ",m_hilightItem)
        if (m_hilightItem) {
            view = get_next_focus_from_focused(m_hilightItem);
            item = m_hilightItem;
        } else {
            item = firstItem();
            view = get_next_focus(item);
        }
        //   _DBG_PRINTF("ListView::nextFocus  m_hilightItem %p  %p  ",view,item)
        while(!view && item) {
            item = (ItemView*)item->nextSibling();
            if (!item)
                break;
            view = get_next_focus(item);
        }
        //   _DBG_PRINTF("ListView::nextFocus  m_hilightItem11 %p  %p  ",view,item)
        if (view) 
        {
            //   _DBG_PRINTF("ListView::nextFocus  m_hilightItem12  ")
            view->focusMe();
            // _DBG_PRINTF("ListView::nextFocus  m_hilightItem13  ")
            if (item  && item != m_hilightItem)
            {
                //      _DBG_PRINTF("ListView::nextFocus  m_hilightItem14  ")
                hilight(item);
            }
        } 
        else if (!item) 
        {   // goto the first item
            m_hilightItem = NULL;
            goto Next;
        }
    }
    //   _DBG_PRINTF("ListView::nextFocus  ret true   ")

    return true;
}


static View* get_prev_focus(ContainerView* c);

static View* get_prev_focus_from_focused(ContainerView* c)
{
    if (!c)
        return NULL;

    if (c->focusStopable())
        return NULL;

    View* view = c->focusView();

    if (!view)
        return get_prev_focus(c);

    if (view->isContainerView()) {
        View* t = get_prev_focus_from_focused((ContainerView*)view);
        if (t)
            return t;
    }

    view = view->previousSibling();

    while (view) {
        if (view->focusStopable())
            return view;
        else if (view->isContainerView()) {
            View *t = get_prev_focus_from_focused((ContainerView*)view);
            if (t) return t;
        }
        view = view->previousSibling();
    }

    return NULL;
}

static View* get_prev_focus(ContainerView* c)
{
    if (!c)
        return NULL;

    if (c->focusStopable())
        return c;

    View* view = c->lastChild();

    while (view) {
        if (view->focusStopable())
            return view;
        else if (view->isContainerView()) {
            View* t = get_prev_focus((ContainerView*)view);
            if (t)
                return t;
        }
        view = view->previousSibling();
    }

    return NULL;
}

bool ListView::prevFocus()
{
    if (focusable()) {
        View *view;
        ItemView *item;
        int loop = 0;

PREV:
        loop++;
        if (loop > 2)
            return false;

        if (m_hilightItem) {
            view = get_prev_focus_from_focused(m_hilightItem);
            item = m_hilightItem;
        } else {
            item = lastItem();
            view = get_prev_focus(item);
        }

        while (!view && item) {
            item = (ItemView*)item->previousSibling();
            if (!item)
                break;
            view = get_prev_focus(item);
        }

        if (view) {
            view->focusMe();
            if (item  && item != m_hilightItem) {
                hilight(item);
            }
        } else if (!item) {   // goto the first item
            m_hilightItem = NULL;
            goto PREV;
        }
    }

    return true;
}

bool ListView::onKeyPressed(int keyCode)
{   
    if (NULL == m_content 
            || m_content->viewCount() < 1) {
        return DISPATCH_CONTINUE_MSG;
    }
    
    if (keyCode ==  KeyEvent::KEYCODE_CURSOR_UP) {
        if (operationMode() == OPM_EDIT) {
            prevFocus();
        }
        else {
            if (NULL != firstEnableChild()) {
                upFindItem();
            }
        }
    }
    else if (keyCode == KeyEvent::KEYCODE_CURSOR_DOWN) {
        if (operationMode() == OPM_EDIT) {
            nextFocus();
        }
        else {
            if (NULL != firstEnableChild()) {
                downFindItem();
            }
        }
    }
    else {
        return DISPATCH_CONTINUE_MSG;
    }

    if (!autoShowBar()) {
        setAutoShowBar(true);
        updateView();
    }

    return DISPATCH_STOP_MSG;
}

////////////////////////////////////////////////////////////////////

#include <mgeff/mgeff.h>

void ListView::_set_y_offset(unsigned int handle, ListView* _this, int id, int *value)
{
    if (NULL != _this)
        _this->moveViewport(_this->m_offx, *value);
}

void ListView::_anim_finish_cb(MGEFF_ANIMATION handle)
{
#if 0
    ListView *_ths = (ListView *)mGEffAnimationGetTarget(handle);
    if (NULL != _ths){
        _ths->m_aniList.remove(handle);
    }
#endif
}

void ListView::animationScroll(int offy)
{
    if (scrollAnimation()) {
#if 0
         MGEFF_ANIMATION _animate = mGEffAnimationCreate(this, (MGEFF_SETPROPERTY_CB)_set_y_offset, 0, MGEFF_INT);
#endif
        const IntRect& contentrc = m_content->getRect();

        int diff = abs(offy - m_offy);
        if (diff > (contentrc.height() >> 1))
            offy = m_offy + contentrc.height() - diff;

#if 0
        mGEffAnimationSetDuration(_animate, 200);
        mGEffAnimationSetStartValue(_animate, &m_offy);
        mGEffAnimationSetEndValue(_animate, &offy);
        mGEffAnimationSetProperty(_animate, MGEFF_PROP_KEEPALIVE, 0);
        mGEffAnimationSetFinishedCb(_animate, (MGEFF_FINISHED_CB)_anim_finish_cb);

        m_aniList.push_back(_animate);

        mGEffAnimationAsyncRun(_animate);
#endif
        //HPlatformOwner hwnd = getPlatformOwner();
        //mGEffAnimationWait(&hwnd ,_animate);
        //mGEffAnimationDelete(_animate);
    }
}

DEFINE_CLASS_NAME(ListView)

} // namespace hfcl {

