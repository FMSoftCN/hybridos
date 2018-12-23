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

#include "view/view.h"

#include "activity/window.h"
#include "graphics/graphicscontext.h"
#include "resource/respackage.h"
#include "resource/respkgmanager.h"

namespace hfcl {

View::View()
    : m_id(0)
    , m_flags(0)
    , m_name(0)
    , m_addData(0)
    , m_parent(0)
    , m_prev(0)
    , m_next(0)
    , m_rect(0, 0, 0, 0)
{
    memset (m_css_classes, 0, sizeof (m_css_classes));
}

View::View(View* p_parent)
    : m_id(0)
    , m_flags(0)
    , m_name(0)
    , m_addData(0)
    , m_parent(0)
    , m_prev(0)
    , m_next(0)
    , m_rect(0, 0, 0, 0)
{
    memset (m_css_classes, 0, sizeof (m_css_classes));
    if (p_parent)
        ((ContainerView *)p_parent)->addChild(this);
    m_parent = (ContainerView*)p_parent;
}

View::View(int i_id, int x, int y, int w, int h)
    : m_id(i_id)
    , m_flags(0)
    , m_name(0)
    , m_addData(0)
    , m_parent(0)
    , m_prev(0)
    , m_next(0)
    , m_rect(x, y, x + w, y + h)
{
    memset (m_css_classes, 0, sizeof (m_css_classes));
}

View::~View()
{
}

void View::onPaint(GraphicsContext* context)
{
    if (isVisible()) {
        IntRect rc(0, 0, m_rect.width(), m_rect.height());

        if (rc.isEmpty())
            return;

        context->setMapView(this);
        if (!context->rectVisible(rc))
            return;

        context->save();
        context->clip(rc);
        drawBackground(context, rc);
        drawContent(context, rc);
        drawScroll(context, rc);
        context->restore();
    }
}

void View::addEventListener(EventListener* listener)
{
    if (NULL == listener) {
        return;
    }
    listener->ref();
    m_listeners.push_front(listener);
}

void View::removeEventListener(EventListener* listener)
{
    if (NULL == listener) {
        return;
    }
    m_listeners.remove(listener);
    listener->unref();
}

void View::releaseEventListeners()
{
    EventListenerList::iterator _listener;
    EventListenerList::iterator _temp;

    if (m_listeners.size() <= 0) {
        return;
    }

    _listener = m_listeners.begin();
    _temp = _listener;
    do {
        ++_listener;
        EventListener* child = *_temp;
          if (child != NULL) {
              child->unref();
              m_listeners.erase(_temp);
          }
        _temp = _listener;
        if (m_listeners.size() == 0) {
            break;
        }
    }
    while (true);
}

void View::onGotFocus()
{
    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)VN_GOTFOCUS, (HTData)this);
    raiseEvent(&event);
    updateView();
}

void View::onLostFocus()
{
    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)VN_LOSTFOCUS, (HTData)this);
    raiseEvent(&event);
    updateView();
}

void View::onClicked()
{
    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)VN_CLICKED, (HTData)this);
    raiseEvent(&event);
}

void View::onDoubleClicked()
{
    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)VN_DBLCLICKED, (HTData)this);
    raiseEvent(&event);
}

bool View::setFocus(View * view)
{
    if(view == NULL)
        return false;

    view->focusMe();
    return true;
}

void View::inner_updateView(int x, int y, int w, int h, bool upBackGnd)
{
    ContainerView *p = parent();
    if (p == NULL)
        return;

    /* VincentWei: disable the invalid update request */
    IntRect rc_invalid (x + m_rect.left(), y + m_rect.top(), x + m_rect.left() + w, y + m_rect.top() + h);
    if (rc_invalid.intersect (m_rect)) {
        p->onChildUpdateView (this, rc_invalid.left(), rc_invalid.top(), rc_invalid.width(), rc_invalid.height());
    }
}

void View::inner_updateViewRect(int x, int y, int w, int h)
{
    ContainerView *p = parent();
    if (p == NULL)
        return;

    /* VincentWei: disable the invalid update request */
    IntRect rc_invalid (x + m_rect.left(), y + m_rect.top(), x + m_rect.left() + w, y + m_rect.top() + h);
    if (rc_invalid.intersect (m_rect)) {
        p->onChildUpdateView (this, rc_invalid.left(), rc_invalid.top(), rc_invalid.width(), rc_invalid.height());
    }
}

void View::updateView(int x, int y, int w, int h)
{
    inner_updateView(x, y, w, h);
}

void View::updateView(const IntRect &rc)
{
    inner_updateView(rc.left(), rc.top(), rc.width(), rc.height());
}

void View::onChildUpdateView(View *child, int x, int y, int w, int h, bool upBackGnd)
{
    inner_updateView(x, y, w, h, upBackGnd);
}

void View::updateViewRect(const IntRect &rc)
{
    inner_updateViewRect(rc.left(), rc.top(), rc.width(), rc.height());
}

void View::updateViewRect()
{
    inner_updateViewRect(0, 0, m_rect.width(), m_rect.height());
}

void View::updateView(bool upBackGnd)
{
    inner_updateView(0, 0, m_rect.width(), m_rect.height(), upBackGnd);
}

//return True if the event was handled, false otherwise.
bool View::dispatchEvent(Event* event)
{
    if (event->eventType() == Event::MOTION_CLICK) {
        ViewClickEventStruct s;
        s.view = this;
        s.x = ((MouseEvent *)event)->x();
        s.y = ((MouseEvent *)event)->y();
        CustomEvent e(Event::CUSTOM_NOTIFY, (HTData)VN_CLICKED, (HTData)&s);
        return raiseEvent(&e);
    }

    return raiseEvent(event);
}

bool View::raiseEvent(Event *event)
{
    EventListenerList::iterator i;

    for (i = m_listeners.begin(); i != m_listeners.end(); ++i){
        if((*i)->handleEvent(event))
            return true;
    }
    return false;
}

void View::viewToWindow(int *x, int *y)
{
    if (x)
        *x += m_rect.left();
    if (y)
        *y += m_rect.top();

    if (m_parent)
        return ((View*)m_parent)->viewToWindow(x, y);
}

void View::windowToView(int *x, int *y)
{
    if (m_parent) {
        int _tmpx, _tmpy;
        ((View*)m_parent)->getPosition(&_tmpx, &_tmpy);

        if(x)
            *x -= _tmpx;
        if(y)
            *y -= _tmpy;
        return ((View*)m_parent)->windowToView(x, y);
    }
}

void View::focusMe()
{
    ContainerView *p = NULL;

    if (rootView() == this) {
        return;
    }

    if (NULL != (p = parent())) {
        if(p->isFocused()){
            if(NULL != p->focusView() && this != p->focusView())
                p->releaseFocusView();
        }
        else {
            p->focusMe();
        }
        p->setFocusView(this);
    }

    /*
    CustomEvent event(Event::CUSTOM_NOTIFY, (HTData)VN_GOTFOCUS, (HTData)this);
    raiseEvent(&event);
    */
}

#if 0
bool View::isFocused()
{
    if (m_parent) {
        return ((m_parent->focusView() == this) && m_parent->focusValid());
    }

    return false;
}
#endif

ContainerView* View::rootView()
{
    View *p = this;

    while (p != NULL){
        if (p->isRootView())
            return (ContainerView *)p;
        p = p->parent();
    }

    return NULL;
}

} // namespace hfcl

