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


#include "slideswitcher.h"

NAMESPACE_BEGIN

SlideSwitcher::SlideSwitcher(View * p_parent, DrawableSet* drset)
	:PanelView(p_parent, drset)
	, m_gap_w(0)
    , m_style(Horizontal)
    , m_item_width(0)
    , m_item_height(0)
    , m_curr_focus_view(NULL)
//    , m_timerId(0)    // GT_jyseo rollback
//    , m_pressedKey(0)     // GT_jyseo rollback 
{
}

SlideSwitcher::SlideSwitcher(View * p_parent)
	:PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(SlideSwitcher))
	, m_gap_w(0)
    , m_style(Horizontal)
    , m_item_width(0)
    , m_item_height(0)
    , m_curr_focus_view(NULL)
//    , m_timerId(0)        // GT_jyseo rollback
//    , m_pressedKey(0)     // GT_jyseo rollback
{
}

SlideSwitcher::SlideSwitcher(View * p_parent, NGInt i_id, NGInt x, NGInt y, 
		NGInt w, NGInt h, SlideStyle style, NGInt gap_w)
	: PanelView(i_id, x, y, w, h)
	, m_gap_w(gap_w)
    , m_style(style)
    , m_item_width(0)
    , m_item_height(0)
    , m_curr_focus_view(NULL)
//    , m_timerId(0)        // GT_jyseo rollback
//    , m_pressedKey(0)     // GT_jyseo rollback
{
}

SlideSwitcher::~SlideSwitcher()
{
}

View* SlideSwitcher::getItem(NGInt offindex, NGBool forward)
{
    View* tmp = NULL;
    tmp = m_curr_focus_view;
    // Forward
    if (forward) {
        while(offindex--) {
            if (NULL != tmp->nextSibling())
                tmp = tmp->nextSibling();
            else
                tmp = firstChild();
        }

        return tmp;
    }
    // Backward
    else {
        while(offindex--) {
            if (NULL != tmp->previousSibling())
                tmp = tmp->previousSibling();
            else
                tmp = lastChild();
        }

        return tmp;
    }
}

void SlideSwitcher::calcuItemRect(NGInt offindex, IntRect& rect, NGBool forward)
{
    // adjust ItemHeight/ItemWidth, at least on item can be see
    if (m_item_height > getRect().height()) {
        m_item_height = getRect().height();
    }
    if (m_item_width > getRect().width()) {
        m_item_width = getRect().width();
    }
    // FocusItem
    if (0 == offindex) {
        // Horizontal style
        if (Horizontal == m_style) {
            rect.setRect(
                    // left
                    getRect().width()/2 - m_item_width/2, 
                    // top
                    (getRect().height() - m_item_height)/2, 
                    // right
                    getRect().width()/2 + m_item_width/2, 
                    // bottom
                    getRect().height() - ((getRect().height() - m_item_height)/2));
        }
        // TODO : Vertical style
        else {
            rect.setRect(
                    // left
                    (getRect().width() - m_item_width)/2, 
                    // top
                    getRect().height()/2 - m_item_height/2, 
                    // right
                    getRect().width() - (getRect().width() - m_item_width)/2, 
                    // bottom
                    getRect().height()/2 + m_item_height/2
                    );
        }

        return;
    }

    // Forward
    if (forward) {
        // Horizontal style
        if (Horizontal == m_style) {
            rect.setRect(
                    // left
                    m_curr_focus_view->getRect().right() + offindex*m_gap_w + (offindex - 1)*m_item_width, 
                    // top
                    m_curr_focus_view->getRect().top(), 
                    // right
                    m_curr_focus_view->getRect().right() + offindex*m_gap_w + offindex*m_item_width, 
                    // bottom
                    m_curr_focus_view->getRect().bottom());
        }
        // TODO : Vertical style
        else {
            rect.setRect(
                    // left
                    m_curr_focus_view->getRect().left(), 
                    // top
                    m_curr_focus_view->getRect().bottom() + offindex*m_gap_w + (offindex - 1)*m_item_height, 
                    // right
                    m_curr_focus_view->getRect().right(),
                    // bottom
                    m_curr_focus_view->getRect().bottom() + offindex*m_gap_w + offindex*m_item_height);
        }
    }
    // Backward
    else {
        // Horizontal style
        if (Horizontal == m_style) {
            rect.setRect(
                    // left
                    m_curr_focus_view->getRect().left() - offindex*m_gap_w - offindex*m_item_width, 
                    // top
                    m_curr_focus_view->getRect().top(), 
                    // right
                    m_curr_focus_view->getRect().left() - offindex*m_gap_w - (offindex - 1)*m_item_width, 
                    // bottom
                    m_curr_focus_view->getRect().bottom());
        }
        // TODO : Vertical style
        else {
            rect.setRect(
                    // left
                    m_curr_focus_view->getRect().left(),
                    // top
                    m_curr_focus_view->getRect().top() - offindex*m_gap_w - offindex*m_item_height, 
                    // right
                    m_curr_focus_view->getRect().right(),
                    // bottom
                    m_curr_focus_view->getRect().top() - offindex*m_gap_w - (offindex - 1)*m_item_height);
        }
    }
}

void SlideSwitcher::reLayout (void)
{
    View* item = NULL;
    IntRect rect;
    NGInt count = 0, left_top = 0, right_bottom = 0, i = 0;
    if (NULL == m_curr_focus_view)
        return;

    // 1. Center (Fucos) Item
    calcuItemRect(0, rect, false);
    m_curr_focus_view->hide();
    m_curr_focus_view->setRect(rect);
    m_curr_focus_view->show();

    count = viewCount();

    // 2. seprate Items By Left/Top and Right/Bottom
    if (0 == count%2) {
        left_top = count/2 - 1;
        right_bottom = count/2;
    }
    else {
        left_top = right_bottom = count/2;
    }

    // 3. adjust Left/Top, Here Can Be optimized : Not All Item cause some can NOT be seen.
    for ( i = 1; i <= left_top; ++i) {
        item = getItem(i, false);
        if (NULL != item) {
            calcuItemRect(i, rect, false);
            item->hide();
            item->setRect(rect);
            item->show();
        }
    } 

    // 4. adjust Right/Bottom, Here Can Be optimized : Not All Item cause some can NOT be seen.
    for ( i = 1; i <= right_bottom; ++i) {
        item = getItem(i, true);
        if (NULL != item) {
            calcuItemRect(i, rect, true);
            item->hide();
            item->setRect(rect);
            item->show();
        }
    } 

    updateView(); 
}

void SlideSwitcher::focusNextItem()
{
    if (NULL == m_curr_focus_view)
        return;

    if (NULL != m_curr_focus_view->nextSibling()) {
        setFocusItem(m_curr_focus_view->nextSibling());
    }
    else {
        setFocusItem(firstChild());
    }
}

void SlideSwitcher::focusPrevItem()
{
    if (NULL == m_curr_focus_view)
        return;

    if (NULL != m_curr_focus_view->previousSibling()) {
        setFocusItem(m_curr_focus_view->previousSibling());
    }
    else {
        setFocusItem(lastChild());
    }
}

void SlideSwitcher::setFocusItem(View* item)
{
    if (NULL == item)
        {
         m_curr_focus_view = NULL;
        return;
        }

    m_curr_focus_view = item;

    setFocusView(m_curr_focus_view);

    //raise event
    CustomEvent event(Event::CUSTOM_NOTIFY, (NGInt)CustomEvent::CUS_SELCHANGED, (NGInt)this);
    raiseEvent(&event);

    reLayout();
}

NGBool SlideSwitcher::addItem(View *view, NGBool toRefresh)
{
    if (NULL == view)
        return false;
    // add into the tree, 
    // maybe it was in the tree ?
    addChild(view); 

    if (NULL == m_curr_focus_view) {
        m_curr_focus_view = view;
        setFocusItem(m_curr_focus_view);
    }

    view->hide();

    if (toRefresh)
        reLayout();

    return true;
}

NGBool SlideSwitcher::removeItem(View *item)
{
    View* _tmp_item = NULL, *_tmp_next = NULL;
	if (NULL != item) {
        _tmp_item = item;
        _tmp_next = item->nextSibling();
        removeChild(item);

        //adjust focus
        if (m_curr_focus_view == _tmp_item) {
            // not the last one item
            if (viewCount() > 0) {
                if (NULL != _tmp_next)
                    m_curr_focus_view = _tmp_next;
                else
                    m_curr_focus_view = firstChild();
            }
            else
                m_curr_focus_view = NULL;
        }

        setFocusItem(m_curr_focus_view);

        if (viewCount() < 1)
            updateView();

        return true;
    }

    return false;
}

NGBool SlideSwitcher::dispatchEvent(Event* event)
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
            NGInt code = ((KeyEvent *)event)->keyCode();
            if ( onKeyPressed (code) ) {

                return DISPATCH_STOP_MSG;
        	}
        }
        default:
            return View::dispatchEvent(event);
    }
    return DISPATCH_CONTINUE_MSG;
}

NGBool SlideSwitcher::onKeyPressed(NGInt keyCode)
{
    switch(keyCode)
    {
        case KeyEvent::KEYCODE_UP :
            {
                if (Vertical == m_style)
                    focusPrevItem();
                break;
            }
        case KeyEvent::KEYCODE_DOWN :
            {
                if (Vertical == m_style)
                    focusNextItem();
                break;
            }
        case KeyEvent::KEYCODE_LEFT :
            {
                if (Horizontal == m_style)
                    focusPrevItem();
                break;
            }
        case KeyEvent::KEYCODE_RIGHT :
            {
                if (Horizontal == m_style)
                    focusNextItem();
                break;
            }
        default: 
            return false;
    }
    return true;
}

DEFINE_CLASS_NAME(SlideSwitcher)

NAMESPACE_END

