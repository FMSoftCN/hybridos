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

#include "common/helpers.h"

#include <string.h>

#include <string>

#include "view/viewcontainer.h"
#include "view/rootview.h"

#include "graphics/graphicscontext.h"
#include "resource/respackage.h"
#include "resource/respkgmanager.h"

namespace hfcl {

/* make sure there is a space at the head and the tail */
static void add_spaces (std::string& str)
{
    if (str.empty ()) {
        str = ' ';
    }
    else {
        if (str.front() != ' ') {
            str = ' ' + str;
        }
        if (str.back() != ' ') {
            str += ' ';
        }
    }
}

View::View(int id, const char* cssClass, const char* name)
    : m_id(id)
    , m_cssCls(cssClass)
    , m_name(name)
    , m_addData(0)
    , m_flags(0)
    , m_parent(0)
    , m_prev(0)
    , m_next(0)
{
    add_spaces (m_cssCls);
    m_cssd_user = HFCL_NEW_EX(CssDeclared, (""));
}

View::~View()
{
    m_cssd_user->unref();
}

bool View::attach(ViewContainer* parent)
{
    if (parent) {
        if (parent->addChild(this)) {
            m_parent = parent;
            m_parent->onChildAttached(this);
            return true;
        }
    }

    return false;
}

bool View::detach()
{
    if (m_parent) {
        if (m_parent->removeChild(this, false)) {
            m_parent = NULL;
            m_parent->onChildDetached(this);
            return true;
        }
    }

    return false;
}

bool View::setName(const char* name)
{
    if (name && m_name.length() > 0) {
        if (strcasecmp (name, m_name.c_str())) {
            goto ok;
        }
    }
    else if (m_name.length() == 0 && name) {
        goto ok;
    }

    return false;

ok:
    m_name = name;
    onNameChanged();
    return true;
}

bool View::setClasses(const char* cssClasses)
{
    std::string tmp = cssClasses;
    add_spaces (tmp);

    if (strcasecmp (tmp.c_str(), m_cssCls.c_str()) == 0) {
        return false;
    }

    m_cssCls = ' ';
    m_cssCls += cssClasses;
    m_cssCls += ' ';
    onClassChanged();
    return true;
}

bool View::includeClass(const char* cssClass)
{
    std::string tmp = cssClass;
    add_spaces (tmp);

    if (strcasestr (m_cssCls.c_str(), tmp.c_str())) {
        return false;
    }

    m_cssCls += cssClass;
    m_cssCls += ' ';
    onClassChanged();
    return true;
}

bool View::excludeClass(const char* cssClass)
{
    std::string tmp = cssClass;
    add_spaces (tmp);

    const char* full = m_cssCls.c_str();
    const char* found;
    found = strcasestr (full, tmp.c_str());
    if (found == NULL) {
        return false;
    }

    size_t pos = (size_t)(found - full);
    m_cssCls.erase (pos, tmp.length() - 1);
    onClassChanged();
    return true;
}

const char* View::getAttribute(const char* attrKey) const
{
    AttributesMap::const_iterator it = m_attrs.find(attrKey);
    if (it != m_attrs.end()) {
        it->second.c_str();
    }

    return NULL;
}

bool View::setAttribute(const char* attrKey, const char* attrValue)
{
    m_attrs[attrKey] = attrValue;
    return true;
}

bool View::checkClass(const char* cssClass) const
{
    std::string tmp = cssClass;
    add_spaces (tmp);

    const char* full = m_cssCls.c_str();
    if (strcasestr (full, tmp.c_str()))
        return true;

    return false;
}

template <class Container>
static void split_attribute(const std::string& str, Container& cont,
              char delim = '=')
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

bool View::checkAttribute(const char* attrPair) const
{
    std::string str(attrPair);

    std::vector<std::string> words;
    split_attribute(str, words);
    if (words.size() == 2) {
        AttributesMap::const_iterator it = m_attrs.find(words[0].c_str());
        if (it != m_attrs.end()) {
            return strcmp(words[1].c_str(), it->second.c_str()) == 0;
        }
    }
    else {
        _DBG_PRINTF("View::checkAttribute: bad attribute: %s\n", attrPair);
    }

    return false;
}

bool View::checkAttribute(const char* attrKey, const char* attrValue) const
{
    AttributesMap::const_iterator it = m_attrs.find(attrKey);
    if (it != m_attrs.end()) {
        return strcmp(attrValue, it->second.c_str()) == 0;
    }

    return false;
}

bool View::checkPseudoElement(const char* pseudoEle) const
{
    /* TODO */
    _DBG_PRINTF("View::checkPseudoElement: NOT IMPLEMENTED\n");
    return false;
}

bool View::checkPseudoClass(const char* pseudoCls) const
{
    std::string tmp = pseudoCls;
    tolower(tmp);

    Uint32 type = Css::getKeywordType(tmp.c_str());
    Uint16 subtype = type & 0xFFFF;
    if (type & Css::CSS_KW_DYNAMIC_PSEUDO_CLASS) {
        switch (subtype) {
        case Css::CSS_KWST_HOVER:
            return m_flags & VA_HOVER;
        case Css::CSS_KWST_ACTIVE:
            return m_flags & VA_ACTIVE;
        case Css::CSS_KWST_FOCUS:
            return m_flags & VA_FOCUSED;
        case Css::CSS_KWST_DISABLED:
            return m_flags & VA_DISABLED;
        case Css::CSS_KWST_ENABLED:
            return !(m_flags & VA_DISABLED);
        case Css::CSS_KWST_CHECKED:
            return m_flags & VA_CHECKED;
        default:
            break;
        }
    }
    else if (type & Css::CSS_KW_PSEUDO_CLASS) {
        switch (subtype) {
        case Css::CSS_KWST_FIRST_CHILD:
            if (m_parent && m_parent->firstChild() == this)
                return true;
            break;

        case Css::CSS_KWST_ONLY_CHILD:
            if (m_parent && m_parent->firstChild() == this &&
                    m_parent->lastChild() == this)
                return true;
            break;

        case Css::CSS_KWST_LAST_CHILD:
            if (m_parent && m_parent->lastChild() == this)
                return true;
            break;

        case Css::CSS_KWST_NTH_CHILD:
            break;

        case Css::CSS_KWST_NTH_LAST_CHILD:
            break;

        case Css::CSS_KWST_ROOT:
            if (isRoot())
                return true;
            break;

        case Css::CSS_KWST_EMPTY:
            if (isContainer()) {
                const ViewContainer* cnt
                    = static_cast<const ViewContainer*>(this);
                if (cnt->childrenCount() == 0)
                    return true;
            }
            else {
                return true;
            }
            break;

        case Css::CSS_KWST_LINK:
        case Css::CSS_KWST_VISITED:
        default:
            break;
        }
    }

    return false;
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
        drawScrollBar(context, rc);
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
    ViewContainer *p = getParent();
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
    ViewContainer *p = getParent();
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
    if (event->eventType() == Event::MOUSE_CLICKED) {
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

    for (i = m_listeners.begin(); i != m_listeners.end(); ++i) {
        if ((*i)->handleEvent(event))
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
    ViewContainer *p = NULL;

    if (getRoot() == this) {
        return;
    }

    if (NULL != (p = getParent())) {
        if (p->isFocused()){
            if(NULL != p->getFocusedChild() && this != p->getFocusedChild())
                p->unfocusChild();
        }
        else {
            p->focusMe();
        }
        p->focusChild(this);
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
        return ((m_parent->getFocusedChild() == this)
                && m_parent->focusValid());
    }

    return false;
}
#endif

RootView* View::getRoot()
{
    View *p = this;

    while (p != NULL) {
        if (p->isRoot())
            return (RootView *)p;
        p = p->getParent();
    }

    return NULL;
}

HWND View::getSysWindow()
{
    RootView* root = getRoot();
    if (root) {
        return root->getSysWindow();
    }

    return HWND_INVALID;
}

void View::applyCss(CssDeclared* css, const CssSelectorGroup& selector)
{
    switch (selector.match(this)) {
    case CssSelectorGroup::CSS_STATIC:
        m_cssdg_static.append(css);
        break;
    case CssSelectorGroup::CSS_DYNAMIC:
        m_cssdg_dynamic.append(css);
        break;
    default:
        break;
    }
}

} // namespace hfcl

