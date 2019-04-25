/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018, 2019 Beijing FMSoft Technologies Co., Ltd.
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

/*
** view.cc: The implementation of View class.
**
** Create by WEI Yongming at 2018/10/27
*/

#include "view/view.h"

#include "common/helpers.h"

#include <stdlib.h>
#include <string.h>

#include <string>

#include "css/csscomputed.h"
#include "css/cssbox.h"
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

View::View(const char* vtag, const char* vtype,
        const char* vclass, const char* vname, int vid)
    : m_cssCls(vclass ? vclass : "")
    , m_contentStrId(-1)
    , m_contentResId(0)
    , m_titleStrId(-1)
    , m_addData(0)
    , m_flags(0)
    , m_parent(0)
    , m_prev(0)
    , m_next(0)
    , m_css_computed(0)
    , m_cssbox_principal(0)
{
    if (vtag != NULL && vtag[0] != 0)
        m_tag = strdup(vtag);
    else
        m_tag = NULL;

    if (vtype != NULL && vtype[0] != 0)
        m_type = strdup(vtype);
    else
        m_type = NULL;

    setName(vname);

    setId(vid);

    add_spaces(m_cssCls);
}

View::~View()
{
    if (m_tag)
        free(m_tag);
    if (m_type)
        free(m_type);

    m_cssd_user.unref();
    if (m_css_computed)
        delete m_css_computed;
    if (m_cssbox_principal)
        delete m_cssbox_principal;
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

const char* View::getAttribute(const char* attrKey) const
{
    if (strcasecmp(VIEW_COMMON_ATTR_CLASS, attrKey) == 0) {
        return getClass();
    }
    else if (strcasecmp(VIEW_COMMON_ATTR_TITLE, attrKey) == 0) {
        return getTitle();
    }
    else if (strcasecmp(VIEW_COMMON_ATTR_ALT, attrKey) == 0) {
        return getTextContent();
    }

    AttributesMap::const_iterator it = m_attrs.find(attrKey);
    if (it != m_attrs.end()) {
        it->second.c_str();
    }

    return NULL;
}

static bool is_content_attribute(const char* attr)
{
    if (strcasecmp(VIEW_COMMON_ATTR_DIR, attr) == 0) {
        return true;
    }

    if (strcasecmp(VIEW_COMMON_ATTR_LANG, attr) == 0) {
        return true;
    }

    return false;
}

static bool is_style_attribute(const char* attr)
{
    if (strcasecmp(VIEW_COMMON_ATTR_ID, attr) == 0) {
        return true;
    }

    if (strcasecmp(VIEW_COMMON_ATTR_NAME, attr) == 0) {
        return true;
    }

    if (strcasecmp(VIEW_COMMON_ATTR_CLASS, attr) == 0) {
        return true;
    }

    return false;
}

bool View::setAttribute(const char* attrKey, const char* attrValue)
{
    if (strcasecmp(VIEW_COMMON_ATTR_CLASS, attrKey) == 0) {
        return setClass(attrValue);
    }
    else if (strcasecmp(VIEW_COMMON_ATTR_TITLE, attrKey) == 0) {
        return setTitle(attrValue);
    }
    else if (strcasecmp(VIEW_COMMON_ATTR_ALT, attrKey) == 0) {
        return setTextContent(attrValue);
    }

    const char* old_value = NULL;
    AttributesMap::iterator it = m_attrs.find(attrKey);
    if (it != m_attrs.end()) {
        if (attrValue == NULL) {
            // erase the attribute
            m_attrs.erase(attrKey);
            goto changed;
        }

        old_value = it->second.c_str();
    }

    if (old_value && strcasecmp(attrValue, old_value)) {
        m_attrs[attrKey] = attrValue;
        goto changed;
    }
    else if (attrValue) {
        m_attrs[attrKey] = attrValue;
        goto changed;
    }

    return false;

changed:
    if (is_content_attribute(attrKey)) {
        onContentChanged(attrKey);
    }
    else if (is_style_attribute(attrKey)) {
        onStyleChanged(attrKey);
    }
    else if (strcasecmp(VIEW_COMMON_ATTR_HIDDEN, attrKey) == 0) {
        if (old_value && strcasecmp("hidden", old_value) == 0) {
            onShown();
        }
        else {
            onHidden();
        }
    }

    return true;
}

int View::getId() const
{
    const char* id = getAttribute(VIEW_COMMON_ATTR_ID);
    if (id == NULL)
        return 0;

    return atoi(id);
}

bool View::setId(int id)
{
    char buff[64];
    snprintf(buff, 63, "%d", id);

    return setAttribute(VIEW_COMMON_ATTR_ID, buff);
}

int View::getTabIndex() const
{
    const char* tabindex = getAttribute(VIEW_COMMON_ATTR_TABINDEX);
    if (tabindex == NULL)
        return 0;

    return atoi(tabindex);
}

bool View::setTabIndex(int tabIndex)
{
    char buff[64];
    snprintf(buff, 63, "%d", tabIndex);

    return setAttribute(VIEW_COMMON_ATTR_TABINDEX, buff);
}

int View::getDir() const
{
    const char* dir = getAttribute(VIEW_COMMON_ATTR_DIR);
    if (dir == NULL)
        return VIEW_TEXT_DIR_LTR;

    if (strcasecmp(dir, "ltr") == 0) {
        return VIEW_TEXT_DIR_LTR;
    }
    else if (strcasecmp(dir, "rtl") == 0) {
        return VIEW_TEXT_DIR_RTL;
    }
    else if (strcasecmp(dir, "auto") == 0) {
        return VIEW_TEXT_DIR_AUTO;
    }

    return VIEW_TEXT_DIR_LTR;
}

bool View::setDir(int dir)
{
    const char* _dir;

    switch (dir) {
    case VIEW_TEXT_DIR_LTR:
    default:
        _dir = "ltr";
        break;
    case VIEW_TEXT_DIR_RTL:
        _dir = "rtl";
        break;
    case VIEW_TEXT_DIR_AUTO:
        _dir = "auto";
        break;
    }

    return setAttribute(VIEW_COMMON_ATTR_DIR, _dir);
}

const char* View::getTitle() const
{
    if (m_titleStrId >= 0) {
        return GetText(m_titleStrId);
    }
    else {
        return m_titleStr.c_str();
    }
}

bool View::setTitle(const char* title)
{
    if (NULL == title)
        title = "";

    m_titleStrId = -1;
    m_titleStr = title;

    return true;
}

bool View::setTitle(HTStrId strId)
{
    if (strId < 0 || strId == m_titleStrId) {
        return false;
    }

    m_titleStrId = strId;
    m_titleStr.clear();

    return true;
}

bool View::setClass(const char* cssClses)
{
    std::string tmp = cssClses;
    add_spaces (tmp);

    if (strcasecmp (tmp.c_str(), m_cssCls.c_str()) == 0) {
        return false;
    }

    m_cssCls = ' ';
    m_cssCls += cssClses;
    m_cssCls += ' ';
    onStyleChanged(VIEW_COMMON_ATTR_CLASS);
    return true;
}

bool View::includeClass(const char* cssCls)
{
    std::string tmp = cssCls;
    add_spaces (tmp);

    if (strcasestr (m_cssCls.c_str(), tmp.c_str())) {
        return false;
    }

    m_cssCls += cssCls;
    m_cssCls += ' ';
    onStyleChanged(VIEW_COMMON_ATTR_CLASS);
    return true;
}

bool View::excludeClass(const char* cssCls)
{
    std::string tmp = cssCls;
    add_spaces (tmp);

    const char* full = m_cssCls.c_str();
    const char* found;
    found = strcasestr (full, tmp.c_str());
    if (found == NULL) {
        return false;
    }

    size_t pos = (size_t)(found - full);
    m_cssCls.erase (pos, tmp.length() - 1);
    onStyleChanged(VIEW_COMMON_ATTR_CLASS);
    return true;
}

size_t View::getTextContentLength() const
{
    if (m_contentStrId >= 0) {
        return strlen(GetText(m_contentStrId));
    }
    else {
        return m_contentStr.length();
    }
}

const char* View::getTextContent() const
{
    if (m_contentStrId >= 0) {
        return GetText(m_contentStrId);
    }
    else {
        return m_contentStr.c_str();
    }
}

bool View::setTextContent(const char* content)
{
    if (NULL == content)
        content = "";

    m_contentStrId = -1;
    m_contentStr = content;

    if (m_contentResId == 0)
        onContentChanged(NULL);

    return true;
}

bool View::setTextContent(HTStrId strId)
{
    if (strId < 0 || strId == m_contentStrId) {
        return false;
    }

    m_contentStrId = strId;
    m_contentStr.clear();

    if (m_contentResId == 0)
        onContentChanged(NULL);

    return true;
}

bool View::setResContent(HTResId resId)
{
    if (resId > 0 && m_contentResId != resId) {
        m_contentResId = resId;
        onContentChanged(NULL);
        return true;
    }

    return false;
}

bool View::checkClass(const char* cssCls) const
{
    std::string tmp = cssCls;
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

static std::string get_value_in_brackets(const char* scan)
{
    const char* start = strchr(scan, '(');
    const char* end = strrchr(scan, ')');
    if (start == end)
        return std::string();

    return std::string(start, end - start);
}

bool View::checkPseudoClass(const char* pseudoCls) const
{
    std::string tmp = pseudoCls;
    const char* bracket = strchr(pseudoCls, '(');
    if (bracket) {
        tmp = std::string(pseudoCls, bracket - pseudoCls);
    }
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
            if (m_parent) {
                int my_idx = m_parent->getChildIndex(this) + 1;
                std::string value = get_value_in_brackets(pseudoCls);
                if (value == "odd") {
                    if (my_idx % 2 != 0)
                        return true;
                }
                else if (value == "even") {
                    if (my_idx % 2 == 0)
                        return true;
                }
                else {
                    int idx = atoi (value.c_str());
                    if (idx == my_idx)
                        return true;
                }
            }
            break;

        case Css::CSS_KWST_NTH_LAST_CHILD:
            if (m_parent) {
                std::string value = get_value_in_brackets(pseudoCls);
                int my_last_idx = m_parent->childrenCount() -
                                    m_parent->getChildIndex(this);
                if (value == "odd") {
                    if (my_last_idx % 2 != 0)
                        return true;
                }
                else if (value == "even") {
                    if (my_last_idx % 2 == 0)
                        return true;
                }
                else {
                    int idx = atoi (value.c_str());
                    if (idx == my_last_idx)
                        return true;
                }
            }
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

void View::drawBackground(GraphicsContext* context, IntRect &rc)
{
}

void View::drawContent(GraphicsContext* context, IntRect &rc)
{
}

void View::drawScrollBar(GraphicsContext* context, IntRect &rc)
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

#if 0
//return True if the event was handled, false otherwise.
bool View::dispatchEvent(Event* event)
{
    if (event->eventType() == Event::ET_VIEW) {
        ViewEvent e(VN_CLICKED, this);
        return raiseViewEvent(&e);
    }

    return raiseEvent(event);
}
#endif

bool View::raiseViewEvent(ViewEvent *event)
{
    EventListenerList::iterator i;

    for (i = m_listeners.begin(); i != m_listeners.end(); ++i) {
        if ((*i)->handler(event))
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

    if (isRoot()) {
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

#if 0
    ViewEvent event(VN_GOTFOCUS, (HTData)this);
    raiseViewEvent(&event);
#endif
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

const RootView* View::getRoot() const
{
    const View *p = this;

    while (p != NULL) {
        if (p->isRoot())
            return (const RootView *)p;
        p = p->getParent();
    }

    return NULL;
}

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

#if 0
Window* View::getSysWindow() const
{
    const RootView* root = getRoot();
    if (root) {
        return root->getSysWindow();
    }

    return NULL;
}
#endif

void View::applyCss(CssDeclared* css, const CssSelectorGroup& selector)
{
    DWORD specif;

    switch (selector.match(this, specif)) {
    case CssSelectorGroup::CSS_STATIC:
        m_cssdg_static.append(css, specif);
        break;
    case CssSelectorGroup::CSS_DYNAMIC:
        m_cssdg_dynamic.append(css, specif);
        break;
    default:
        break;
    }
}

void View::computeCss()
{
    if (m_css_computed) {
        m_css_computed->reset();
    }
    else {
        m_css_computed = new CssComputed();
    }

    const std::vector<CssWithSpecificity>& vec = m_cssdg_static.getVec();
    std::vector<CssWithSpecificity>::const_iterator it;
    for (it = vec.begin(); it != vec.end(); ++it) {
        const CssWithSpecificity& css_specif = *it;
        const CssPropertyValueMap& pv_map = css_specif.css->getMap();

        CssPropertyValueMap::const_iterator pv_it;
        for (pv_it = pv_map.begin(); pv_it != pv_map.end(); ++pv_it) {
            CssPropertyIds pid = (CssPropertyIds)(pv_it->first);
            const CssPropertyValue* pv = pv_it->second;

            Uint32 value = pv->getValue();
            HTPVData data = pv->getData();

            /* check inherit */

            // The used value of the ‘currentColor’ keyword is the computed
            // value of the ‘color’ property. If the ‘currentColor’ keyword
            // is set on the ‘color’ property itself, it is treated as
            // ‘color: inherit’.
            if (pid == PID_COLOR &&
                    CSS_PPT_VALUE_NOMARKS(value) == PV_CURRENT_COLOR) {
                if (m_parent && m_parent->m_css_computed) {
                    m_css_computed->inherit(m_parent->m_css_computed, pid);
                }
                else {
                    _ERR_PRINTF("View::computeCss: No parent CssComputed\n");
                }
            }
            else if (CSS_PPT_VALUE_NOMARKS(value) == PV_INHERIT) {
                if (m_parent && m_parent->m_css_computed) {
                    m_css_computed->inherit(m_parent->m_css_computed, pid);
                }
                else {
                    _ERR_PRINTF("View::computeCss: No parent CssComputed\n");
                }
            }
            else {
                m_css_computed->setProperty(pid, value, data);
            }
        }
    }

    // make length/URI and others to be absolute here
    m_css_computed->makeAbsolute(this);
    m_css_computed->validate(this);
}

void View::makeCssBox()
{
    if (m_cssbox_principal) {
        delete m_cssbox_principal;
        m_cssbox_principal = 0;
    }

    Uint32 pv;
    m_css_computed->getProperty(PID_DISPLAY, &pv, NULL);
    if (pv == PV_NONE)
        return;

    if (!isReplaced() && (pv == PV_BLOCK || pv == PV_LIST_ITEM ||
            pv == PV_INLINE_BLOCK)) {
        // inline box
        m_cssbox_principal = new CssInlineBox(m_css_computed);
    }
    else if (!isReplaced() && pv == PV_INLINE) {
        // line box
        m_cssbox_principal = new CssInlineBox(m_css_computed);
    }
    else if (isReplaced() && (pv == PV_INLINE || pv == PV_INLINE_BLOCK)) {
        // atomic inline-level box
        m_cssbox_principal = new CssBox(m_css_computed);
    }
    else {
        // block box
        m_cssbox_principal = new CssBox(m_css_computed);
    }
}

void View::makeStackingContext(CssStackingContext* cssSc)
{
    if (m_cssbox_principal && m_css_computed->isPositioned()) {
        int zindex;
        bool autozindex = m_css_computed->getZIndex(zindex);
        if (!autozindex || (autozindex && m_css_computed->isFixed())) {
            unsigned char opacity = m_css_computed->getOpacity();
            CssStackingContext* csc;
            csc = new CssStackingContext(this, zindex, opacity);
            cssSc->append(csc);
        }
    }
}

void View::layOut(CssBox* ctnBlock)
{
    if (m_cssbox_principal) {
        m_cssbox_principal->calcBox(this, ctnBlock);
    }
    else {
        _ERR_PRINTF("%s: should not be here\n", __FUNCTION__);
    }
}

void View::onContainingBlockChanged()
{
}

void View::onStyleChanged(const char* attrKey)
{
}

void View::onContentChanged(const char* attrKey)
{
}

void View::onShown()
{
}

void View::onHidden()
{
}

void View::onDisabled()
{
}

void View::onEnabled()
{
}

void View::onChecked()
{
}

void View::onUnchecked()
{
}

void View::onFrozen()
{
}

void View::onUnfrozen()
{
}

void View::onHovered()
{
}

void View::onLeft()
{
}

void View::onActive()
{
    ViewEvent event(VN_ACTIVE, this);
    raiseViewEvent(&event);
}

void View::onInactive()
{
    ViewEvent event(VN_INACTIVE, this);
    raiseViewEvent(&event);
}

void View::onGotFocus()
{
    ViewEvent event(VN_GOTFOCUS, this);
    raiseViewEvent(&event);
    updateView();
}

void View::onLostFocus()
{
    ViewEvent event(VN_LOSTFOCUS, this);
    raiseViewEvent(&event);
    updateView();
}

void View::onClicked()
{
    ViewEvent event(VN_CLICKED, this);
    raiseViewEvent(&event);
}

void View::onDoubleClicked()
{
    ViewEvent event(VN_DBLCLICKED, this);
    raiseViewEvent(&event);
}

void View::onIdle()
{
}

} // namespace hfcl

