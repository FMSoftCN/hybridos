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


#include "graphicscontext.h"
#include "containerview.h"

#include "hal_misc.h"

NAMESPACE_BEGIN

ContainerView::ContainerView()
    : View()
    , m_focusView(0)
    , m_firstChild(NULL)
    , m_lastChild(NULL)
    , m_childCount(0)
{
}

ContainerView::ContainerView(View* p_parent)
    : View(p_parent)
    , m_focusView(0)
    , m_firstChild(NULL)
    , m_lastChild(NULL)
    , m_childCount(0)
{
}

ContainerView::ContainerView(View* p_parent, DrawableSet* drset)
    : View(p_parent, drset)
    , m_focusView(0)
    , m_firstChild(NULL)
    , m_lastChild(NULL)
    , m_childCount(0)
{
}

ContainerView::ContainerView(int i_id, int x, int y, int w, int h)
    : View(i_id, x, y, w, h)
    , m_focusView(0)
    , m_firstChild(NULL)
    , m_lastChild(NULL)
    , m_childCount(0)
{
}

ContainerView::~ContainerView()
{
    clean();
}

View* ContainerView::firstEnableChild(void)
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

bool ContainerView::insertAfter(View* view, View *child)
{
    View *next = NULL;
    if(view && view->parent() != this)
        return false;
    
    if(!child)
        return false;

    if(isChild(child) || view == child)
        return true;

    if(child->parent())
        child->parent()->detachChild(child);
    
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

bool ContainerView::insertBefore(int idx, View* child)
{ 
    if(!child)
        return false;
    if(idx < 0) 
        return insertAfter(lastChild(), child);
    else
        return insertBefore(getChildByIndex(idx), child); 
}

bool ContainerView::insertBefore(View *view, View* child)
{
    View *prev = NULL;
    if(view && view->parent() != this)
        return false;

    if(!child)
        return false;

    if(isChild(child) || view == child)
        return true;

    if(child->parent())
        child->parent()->detachChild(child);

    child->setNextSlibling(view);
    if(view)
    {
        prev = view->previousSibling();
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

int ContainerView::detachChild(int index)
{
    return detachChild(getChildByIndex(index));
}

bool ContainerView::isChild(View* view) const
{
    return view && view->parent() == this;
}

int ContainerView::removeChild(int index, bool bRelease)
{
    return removeChild(getChildByIndex(index), bRelease);
}

int ContainerView::removeAll()
{
    View *  view = m_firstChild;
    while(view)
    {
        View *tmp = view;
        view = view->nextSibling();
        //FIXED ME: should call the release method
        // delete tmp;
        NGUX_DELETE(tmp);
    }

    m_firstChild = NULL;
    m_lastChild = NULL;
    m_childCount = 0;

    if(m_focusView)
        m_focusView = NULL;

   return 0;
}

void ContainerView::clean()
{
    removeAll();
}

int ContainerView::detachChild(View* view)
{
    if(!view || view->parent() != this)
        return -1;

    View * prev = view->previousSibling();
    View * next = view->nextSibling();

    if(prev)
        prev->setNextSlibling(next);
    if(next)
        next->setPrevSlibling(prev);

    if(view == m_firstChild)
    {
        m_firstChild = next;
    }

    if(view == m_lastChild)
    {
        m_lastChild = prev;
    }

    m_childCount --;

    view->setParent(NULL);

    if(m_focusView == view)
    {
        m_focusView = NULL;    
    }

    return 0;
}

int ContainerView::removeChild(View * view, bool bRelease)
{
    if(detachChild(view) == 0) {
        if (bRelease)
            NGUX_DELETE(view);
        return 0;
    }
    return -1;
}

View* ContainerView::getChild(int i_id) const
{
    for(View *view = firstChild(); view ; view = view->nextSibling()) {
        if(view->id() == i_id)
            return view;
    }
    return NULL;
}

View* ContainerView::getChildByIndex(int idx) const
{
    View *view = NULL;
    if(idx < 0)
        return lastChild();

    if(idx > (m_childCount>>1)) //from tail
        for(view = lastChild(), idx = m_childCount - idx - 1; view  && idx > 0; view = view->previousSibling(), idx --);
    else
        for(view = firstChild(); view  && idx > 0; view = view->nextSibling(), idx --);

    return view;
}

int ContainerView::getChildIndex(View *view) const {
    if(!isChild(view))
        return -1;

    int idx = 0;
    for(View *c = firstChild(); c && c != view; c = c->nextSibling(), idx++);

    return idx;
}


View* ContainerView::getView (int i_id) const
{
    for(View *view = firstChild(); view ; view = view->nextSibling()) {
        if(view->id() == i_id)
            return view;
        else if(view->isContainerView())
        {
            View *c = ((ContainerView*)view)->getView(i_id);
            if(c)
                return c;
        }
            
    }
    return NULL;
}

View *ContainerView::getChildByPosition(int x_pos, int y_pos) const
{
    for(View *view = lastChild(); NULL != view ; view = view->previousSibling()) 
    {
        IntRect irc = view->getRect();
        if (irc.contains(x_pos, y_pos)){
            if(view->isContainerView()) {
                return ((ContainerView*)view)->getChildByPosition(x_pos - irc.left(), y_pos - irc.top());
            }
            else {
                return view;
            }
        }
    }
    return (View *)this;
}

#if 0
void ContainerView::markDirty(const IntRect& rc)
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

void ContainerView::drawBackground(GraphicsContext* context, IntRect &rc, int status)
{
    return;
}

void ContainerView::drawContent(GraphicsContext* context, IntRect& rc, int status)
{
#ifdef TRACE_DRAWVIEW
    int depth = 1;
    View* _p = (View*)parent();
    while (_p) {
        _p = _p->parent();
        printf ("\t");
        depth ++;
    }

    HDC hdc = context->context ();
    IntRect rcBounds;
    GetBoundsRect(hdc, (RECT*)&rcBounds);

    printf ("ContainerView::drawContent: Start to draw %p (parent: %p), %d, %d, %d, %d\n",
                this, parent(), rcBounds.left(), rcBounds.top(), rcBounds.right(), rcBounds.bottom());
#endif

    for (View *view = firstChild(); view; view = view->nextSibling()) {
#ifdef TRACE_DRAWVIEW
        for (int i = 0; i < depth; i++) printf ("\t");
        if (view->isVisible() && view->shouldUpdate()) {
            IntRect rcView = view->getRect();
            printf ("ContainerView::drawContent: draw child %p (%d, %d, %d, %d)\n",
                    view, rcView.left(), rcView.top(), rcView.right(), rcView.bottom());
            view->onPaint(context, status);
        }
        else {
            printf ("ContainerView::drawContent: skip child %p, visible (%s), unfrozen (%s)\n", view, view->isVisible()?"TRUE":"FALSE", view->shouldUpdate()?"TRUE":"FALSE");
        }
#else
        if (view->isVisible() && view->shouldUpdate()) {
            view->onPaint(context, status);
        }
#endif
    }

#ifdef TRACE_DRAWVIEW
    for (int i = 0; i < depth - 1; i++) printf ("\t");
    printf ("ContainerView::drawContent: Stop to draw %p\n", this);
#endif
}

void ContainerView::changeTheme(void)
{
    View::changeTheme();
    for(View *view = firstChild(); view; view = view->nextSibling()) {
        view->changeTheme();
    }
}


bool ContainerView::dispatchEvent(Event* event)
{
    View::dispatchEvent(event);

    switch(event->eventType())
    {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_MOVE:
            {
                View *view = firstChild();
                int x = ((MouseEvent *)event)->x();
                int y = ((MouseEvent *)event)->y();
                focusMe();
                windowToView(&x, &y);
                while (view != (View *)0) 
                {
                    if(view->getRect().isIn(x, y))
                    {
                        view->focusMe();
                        view->dispatchEvent(event);
                        return DISPATCH_STOP_MSG;
                    }
                    view = view->nextSibling();
                }

                break;
            }
        case Event::KEY_DOWN:
        case Event::KEY_LONGPRESSED:
        case Event::KEY_ALWAYSPRESS:        // GT_yhkang add longpress event    
        case Event::KEY_UP:
        case Event::KEY_CHAR:
            {
                if (m_focusView) {
                    return m_focusView->dispatchEvent(event);
                }
            }
            break;
        default:
            break;
    }
    return DISPATCH_CONTINUE_MSG;
}

void ContainerView::setFocusView(View* view)
{
    View* old_focusView = NULL;
    if(view == NULL)
        return ;

    if(m_focusView == view)
        return ;

    old_focusView = m_focusView;
    m_focusView = view;

    if(old_focusView)
        old_focusView->onLoseFocus();

    setFocusValid(true);
    
    m_focusView->onGetFocus();

    if (m_focusView->isContainerView()){
        View * focus = ((ContainerView *)m_focusView)->focusView();
        if(focus)
            m_focusView->setFocus(focus);
    }
}

void ContainerView::releaseFocusView(void)
{
    View* old_focusView = NULL;
    if (m_focusView == (View *)0){
        return;
    }

    old_focusView = m_focusView;
    m_focusView = NULL;
    
    old_focusView->onLoseFocus();
    if (old_focusView->isContainerView()){
        ((ContainerView *)old_focusView)->releaseFocusView();
    }
    setFocusValid(false);
    
    // FIXME ylwang changed at 2012-04-06
    /*
    CustomEvent event(Event::CUSTOM_NOTIFY, (int)NOTIFY_LOSE_FOCUS, (int)m_focusView);
    raiseEvent(&event);
    */
}

void ContainerView::autoFitSize(bool auto_child_fit) 
{
    IntRect rc;
    for(View *view = firstChild(); view ; view = view->nextSibling()) 
    {
        if(auto_child_fit)
            view->autoFitSize(true);
        rc.join(0, 0, view->getRect().right(), view->getRect().bottom());
    }
    setRect(rc);
}

DEFINE_CLASS_NAME(ContainerView)
NAMESPACE_END
