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
    , m_focusView(0)
    , m_firstChild(NULL)
    , m_lastChild(NULL)
    , m_childCount(0)
{
}

ViewContainer::~ViewContainer()
{
    clean();
}

View* ViewContainer::firstEnableChild(void)
{
    View* view = NULL;
    if (viewCount() < 1)
        return NULL;
    view = m_firstChild;

    while(view) {
        if (!view->isDisabled())
            break;;
        view = view->nextSibling();
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

    child->setPrevSlibling(view);
    if(view)
    {
        next = view->nextSibling();
        view->setNextSlibling(child);
    }

    if(next)
    {
        next->setPrevSlibling(child);
    }
    child->setNextSlibling(next);

    if(view == m_lastChild)
        m_lastChild = child;

    if(!m_firstChild)
        m_firstChild = child;

    child->setParent(this);

    m_childCount ++;
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

    child->setNextSlibling(view);
    if(view)
    {
        prev = view->prevSibling();
        view->setPrevSlibling(child);
    }

    if(prev)
    {
        prev->setNextSlibling(child);
    }
    child->setPrevSlibling(prev);

    if(view == m_firstChild)
        m_firstChild = child;

    if(!m_lastChild)
        m_lastChild = child;

    child->setParent(this);

    m_childCount ++;
    return true;
}

int ViewContainer::detachChild(int index)
{
    return detachChild(getChildByIndex(index));
}

bool ViewContainer::isChild(View* view) const
{
    return view && view->getParent() == this;
}

int ViewContainer::removeChild(int index, bool bRelease)
{
    return removeChild(getChildByIndex(index), bRelease);
}

int ViewContainer::removeAll()
{
    View *  view = m_firstChild;
    while(view)
    {
        View *tmp = view;
        view = view->nextSibling();
        //FIXED ME: should call the release method
        // delete tmp;
        HFCL_DELETE(tmp);
    }

    m_firstChild = NULL;
    m_lastChild = NULL;
    m_childCount = 0;

    if(m_focusView)
        m_focusView = NULL;

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

    View * prev = view->prevSibling();
    View * next = view->nextSibling();

    if(prev)
        prev->setNextSlibling(next);
    if(next)
        next->setPrevSlibling(prev);

    if(view == m_firstChild) {
        m_firstChild = next;
    }

    if(view == m_lastChild) {
        m_lastChild = prev;
    }

    m_childCount --;

    view->setParent(NULL);

    if(m_focusView == view) {
        m_focusView = NULL;
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

View* ViewContainer::getChild(int i_id) const
{
    for(View *view = firstChild(); view ; view = view->nextSibling()) {
        if(view->getId() == i_id)
            return view;
    }
    return NULL;
}

View* ViewContainer::getChildByIndex(int idx) const
{
    View *view = NULL;
    if(idx < 0)
        return lastChild();

    if(idx > (m_childCount>>1)) //from tail
        for(view = lastChild(), idx = m_childCount - idx - 1; view  && idx > 0; view = view->prevSibling(), idx --);
    else
        for(view = firstChild(); view  && idx > 0; view = view->nextSibling(), idx --);

    return view;
}

int ViewContainer::getChildIndex(View *view) const {
    if(!isChild(view))
        return -1;

    int idx = 0;
    for(View *c = firstChild(); c && c != view; c = c->nextSibling(), idx++);

    return idx;
}


View* ViewContainer::getView (int i_id) const
{
    for(View *view = firstChild(); view ; view = view->nextSibling()) {
        if(view->getId() == i_id)
            return view;
        else if(view->isContainer())
        {
            View *c = ((ViewContainer*)view)->getView(i_id);
            if(c)
                return c;
        }

    }
    return NULL;
}

View *ViewContainer::getChildByPosition(int x_pos, int y_pos) const
{
    for(View *view = lastChild(); NULL != view ; view = view->prevSibling())
    {
        IntRect irc = view->getRect();
        if (irc.contains(x_pos, y_pos)){
            if(view->isContainer()) {
                return ((ViewContainer*)view)->getChildByPosition(x_pos - irc.left(), y_pos - irc.top());
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
    for (View *view = firstChild(); view; view = view->nextSibling()) {
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

    for (View *view = firstChild(); view; view = view->nextSibling()) {
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
            view = view->nextSibling();
        }
        break;
    }

    case Event::KEY_DOWN:
    case Event::KEY_LONGPRESSED:
    case Event::KEY_ALWAYSPRESS:
    case Event::KEY_UP:
    case Event::KEY_CHAR: {
        if (m_focusView) {
            return m_focusView->dispatchEvent(event);
        }
        break;
    }

    default:
        break;
    }

    return GOON_DISPATCH;
}

void ViewContainer::setFocusView(View* view)
{
    View* old_focusView = NULL;
    if(view == NULL)
        return ;

    if(m_focusView == view)
        return ;

    old_focusView = m_focusView;
    m_focusView = view;

    if(old_focusView)
        old_focusView->onLostFocus();

    setFocusable(true);

    m_focusView->onGotFocus();

    if (m_focusView->isContainer()){
        View * focus = ((ViewContainer *)m_focusView)->focusView();
        if(focus)
            m_focusView->setFocus(focus);
    }
}

void ViewContainer::releaseFocusView(void)
{
    View* old_focusView = NULL;
    if (m_focusView == (View *)0){
        return;
    }

    old_focusView = m_focusView;
    m_focusView = NULL;

    old_focusView->onLostFocus();
    if (old_focusView->isContainer()){
        ((ViewContainer *)old_focusView)->releaseFocusView();
    }
    setFocusable(false);

    // FIXME
    /*
    CustomEvent event(Event::CUSTOM_NOTIFY, (int)VN_LOSTFOCUS, (int)m_focusView);
    raiseEvent(&event);
    */
}

void ViewContainer::autoFitSize(bool auto_child_fit)
{
    IntRect rc;
    for(View *view = firstChild(); view ; view = view->nextSibling()) {
        rc.join(0, 0, view->getRect().right(), view->getRect().bottom());
    }
    setRect(rc);
}

} // namespace hfcl
