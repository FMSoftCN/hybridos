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

#include "view/viewcontainer.h"

#include "graphics/graphicscontext.h"

namespace hfcl {

ViewContainer::ViewContainer(int id, const char* cssClass, const char* name)
    : View(id, cssClass, name)
    , m_focused(0)
    , m_first(NULL)
    , m_last(NULL)
    , m_nr_children(0)
{
}

ViewContainer::~ViewContainer()
{
    clean();
}

View* ViewContainer::firstEnabledChild(void)
{
    View* view = NULL;
    if (childrenCount() < 1)
        return NULL;
    view = m_first;

    while(view) {
        if (!view->isDisabled())
            break;;
        view = view->getNext();
    }

    return view;
}

bool ViewContainer::insertAfter(View* view, View *child)
{
    View *next = NULL;
    if(view && view->getParent() != this)
        return false;

    if(!child)
        return false;

    if(isChild(child) || view == child)
        return true;

    if(child->getParent())
        child->getParent()->detachChild(child);

    child->setPrev(view);
    if(view)
    {
        next = view->getNext();
        view->setNext(child);
    }

    if(next)
    {
        next->setPrev(child);
    }
    child->setNext(next);

    if(view == m_last)
        m_last = child;

    if(!m_first)
        m_first = child;

    child->setParent(this);

    m_nr_children ++;
    return true;
}

bool ViewContainer::insertBefore(int idx, View* child)
{
    if(!child)
        return false;
    if(idx < 0)
        return insertAfter(lastChild(), child);
    else
        return insertBefore(getChildByIndex(idx), child);
}

bool ViewContainer::insertBefore(View *view, View* child)
{
    View *prev = NULL;
    if(view && view->getParent() != this)
        return false;

    if(!child)
        return false;

    if(isChild(child) || view == child)
        return true;

    if(child->getParent())
        child->getParent()->detachChild(child);

    child->setNext(view);
    if(view)
    {
        prev = view->getPrev();
        view->setPrev(child);
    }

    if(prev)
    {
        prev->setNext(child);
    }
    child->setPrev(prev);

    if(view == m_first)
        m_first = child;

    if(!m_last)
        m_last = child;

    child->setParent(this);

    m_nr_children ++;
    return true;
}

int ViewContainer::detachChildByIndex(int index)
{
    return detachChild(getChildByIndex(index));
}

bool ViewContainer::isChild(const View* view) const
{
    return view && view->getParent() == this;
}

int ViewContainer::removeChild(int index, bool bRelease)
{
    return removeChild(getChildByIndex(index), bRelease);
}

int ViewContainer::removeAll()
{
    View *  view = m_first;
    while(view)
    {
        View *tmp = view;
        view = view->getNext();
        //FIXED ME: should call the release method
        // delete tmp;
        HFCL_DELETE(tmp);
    }

    m_first = NULL;
    m_last = NULL;
    m_nr_children = 0;

    if(m_focused)
        m_focused = NULL;

   return 0;
}

void ViewContainer::clean()
{
    removeAll();
}

int ViewContainer::detachChild(View* view)
{
    if(!view || view->getParent() != this)
        return -1;

    View * prev = view->getPrev();
    View * next = view->getNext();

    if(prev)
        prev->setNext(next);
    if(next)
        next->setPrev(prev);

    if(view == m_first) {
        m_first = next;
    }

    if(view == m_last) {
        m_last = prev;
    }

    m_nr_children --;

    view->setParent(NULL);

    if(m_focused == view) {
        m_focused = NULL;
    }

    return 0;
}

int ViewContainer::removeChild(View * view, bool bRelease)
{
    if(detachChild(view) == 0) {
        if (bRelease)
            HFCL_DELETE(view);
        return 0;
    }
    return -1;
}

const View* ViewContainer::getChildById(int id) const
{
    const View* view;
    for (view = firstChild(); view ; view = view->getNext()) {
        if (view->getId() == id)
            return view;
    }
    return NULL;
}

View* ViewContainer::getChildById(int id)
{
    View* view;
    for (view = firstChild(); view ; view = view->getNext()) {
        if (view->getId() == id)
            return view;
    }
    return NULL;
}

const View* ViewContainer::getChildByIndex(int idx) const
{
    const View *view = NULL;
    if (idx < 0)
        return lastChild();

    if (idx > (m_nr_children >> 1))
        for (view = lastChild(), idx = m_nr_children - idx - 1;
                view && idx > 0; view = view->getPrev(), idx --);
    else
        for(view = firstChild();
                view && idx > 0; view = view->getNext(), idx --);

    return view;
}

View* ViewContainer::getChildByIndex(int idx)
{
    View *view = NULL;
    if (idx < 0)
        return lastChild();

    if (idx > (m_nr_children >> 1))
        for (view = lastChild(), idx = m_nr_children - idx - 1;
                view && idx > 0; view = view->getPrev(), idx --);
    else
        for(view = firstChild();
                view && idx > 0; view = view->getNext(), idx --);

    return view;
}

int ViewContainer::getChildIndex(const View *view) const
{
    if (!isChild (view))
        return -1;

    int idx = 0;
    for (const View *c = firstChild(); c && c != view; c = c->getNext(), idx++);

    return idx;
}

const View* ViewContainer::findDescendant(int id) const
{
    for (const View *view = firstChild(); view ; view = view->getNext()) {
        if (view->getId() == id)
            return view;
        else if (view->isContainer()) {
            const View *c = ((ViewContainer*)view)->getChildById(id);
            if (c)
                return c;
        }
    }

    return NULL;
}

View* ViewContainer::findDescendant(int id)
{
    for (View *view = firstChild(); view ; view = view->getNext()) {
        if (view->getId() == id)
            return view;
        else if (view->isContainer()) {
            View *c = ((ViewContainer*)view)->getChildById(id);
            if (c)
                return c;
        }
    }

    return NULL;
}

const View *ViewContainer::getChildByPosition(int x_pos, int y_pos) const
{
    const View* view;
    for (view = lastChild(); NULL != view ; view = view->getPrev()) {
        IntRect irc = view->getRect();
        if (irc.contains (x_pos, y_pos)){
            if (view->isContainer()) {
                return ((const ViewContainer*)view)->getChildByPosition (
                        x_pos - irc.left(), y_pos - irc.top());
            }
            else {
                return view;
            }
        }
    }

    return (const View *)this;
}

View *ViewContainer::getChildByPosition(int x_pos, int y_pos)
{
    View* view;
    for (view = lastChild(); NULL != view ; view = view->getPrev()) {
        IntRect irc = view->getRect();
        if (irc.contains (x_pos, y_pos)){
            if (view->isContainer()) {
                return ((ViewContainer*)view)->getChildByPosition (
                        x_pos - irc.left(), y_pos - irc.top());
            }
            else {
                return view;
            }
        }
    }

    return (View *)this;
}

#if 0
void ViewContainer::markDirty(const IntRect& rc)
{
    for (View *view = firstChild(); view; view = view->getNext()) {
        if (view->isVisible()) {
            IntRect rcView = view->getRect();
            IntRect rcInv = rcView;

            if (rcInv.intersect (rc)) {
                rcInv.offset (-rcView.left(), -rcView.top());
                view->markDirty (rcInv);
            }
        }
    }
}
#endif

#undef TRACE_DRAWVIEW

void ViewContainer::drawBackground(GraphicsContext* context, IntRect &rc)
{
    return;
}

void ViewContainer::drawContent(GraphicsContext* context, IntRect& rc)
{
#ifdef TRACE_DRAWVIEW
    int depth = 1;
    View* _p = (View*)getParent();
    while (_p) {
        _p = _p->getParent();
        printf ("\t");
        depth ++;
    }

    HDC hdc = context->context ();
    IntRect rcBounds;
    GetBoundsRect(hdc, (RECT*)&rcBounds);

    printf ("ViewContainer::drawContent: Start to draw %p (parent: %p), %d, %d, %d, %d\n",
                this, getParent(), rcBounds.left(), rcBounds.top(), rcBounds.right(), rcBounds.bottom());
#endif

    for (View *view = firstChild(); view; view = view->getNext()) {
#ifdef TRACE_DRAWVIEW
        for (int i = 0; i < depth; i++) printf ("\t");
        if (view->isVisible() && view->shouldUpdate()) {
            IntRect rcView = view->getRect();
            printf ("ViewContainer::drawContent: draw child %p (%d, %d, %d, %d)\n",
                    view, rcView.left(), rcView.top(), rcView.right(), rcView.bottom());
            view->onPaint(context);
        }
        else {
            printf ("ViewContainer::drawContent: skip child %p, visible (%s), unfrozen (%s)\n", view, view->isVisible()?"TRUE":"FALSE", view->shouldUpdate()?"TRUE":"FALSE");
        }
#else
        if (view->isVisible()) {
            view->onPaint(context);
        }
#endif
    }

#ifdef TRACE_DRAWVIEW
    for (int i = 0; i < depth - 1; i++) printf ("\t");
    printf ("ViewContainer::drawContent: Stop to draw %p\n", this);
#endif
}

bool ViewContainer::dispatchEvent(Event* event)
{
#if 0
    View::dispatchEvent(event);

    switch (event->eventType()) {
    case Event::MOUSE_UP:
    case Event::MOUSE_DOWN:
    case Event::MOUSE_MOVE: {
        View *view = firstChild();
        int x = ((MouseEvent *)event)->x();
        int y = ((MouseEvent *)event)->y();
        focusMe();
        windowToView(&x, &y);
        while (view != (View *)0) {
            if(view->getRect().isIn(x, y)) {
                view->focusMe();
                view->dispatchEvent(event);
                return STOP_DISPATCH;
            }
            view = view->getNext();
        }
        break;
    }

    case Event::KEY_DOWN:
    case Event::KEY_LONGPRESSED:
    case Event::KEY_ALWAYSPRESS:
    case Event::KEY_UP:
    case Event::KEY_CHAR: {
        if (m_focused) {
            return m_focused->dispatchEvent(event);
        }
        break;
    }

    default:
        break;
    }
#endif

    return GOON_DISPATCH;
}

void ViewContainer::focusChild(View* view)
{
    View* old_focusView = NULL;
    if(view == NULL)
        return ;

    if(m_focused == view)
        return ;

    old_focusView = m_focused;
    m_focused = view;

    if(old_focusView)
        old_focusView->onLostFocus();

    setFocusable(true);

    m_focused->onGotFocus();

    if (m_focused->isContainer()){
        View* focus = ((ViewContainer *)m_focused)->getFocusedChild();
        if (focus)
            m_focused->setFocus(focus);
    }
}

void ViewContainer::unfocusChild(void)
{
    View* old_focusView = NULL;
    if (m_focused == (View *)0){
        return;
    }

    old_focusView = m_focused;
    m_focused = NULL;

    old_focusView->onLostFocus();
    if (old_focusView->isContainer()){
        ((ViewContainer *)old_focusView)->unfocusChild();
    }
    setFocusable(false);

    // FIXME
    /*
    CustomEvent event(Event::CUSTOM_NOTIFY, (int)VN_LOSTFOCUS, (int)m_focused);
    raiseEvent(&event);
    */
}

void ViewContainer::autoFitSize(bool auto_child_fit)
{
    IntRect rc;
    for(View *view = firstChild(); view ; view = view->getNext()) {
        rc.join(0, 0, view->getRect().right(), view->getRect().bottom());
    }
    setRect(rc);
}

void ViewContainer::applyCss(CssDeclared* css, const CssSelectorGroup& selector)
{
    View* child = m_first;
    while (child) {
        child->applyCss(css, selector);
        child = child->getNext();
    }

    View::applyCss(css, selector);
}

void ViewContainer::computeCss()
{
    View::computeCss();

    View* child = m_first;
    while (child) {
        child->computeCss();
        child = child->getNext();
    }
}

} // namespace hfcl
