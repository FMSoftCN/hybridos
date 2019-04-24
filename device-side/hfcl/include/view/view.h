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

#ifndef HFCL_VIEW_VIEW_H_
#define HFCL_VIEW_VIEW_H_

#include "../view/viewcontext.h"
#include "../common/event.h"
#include "../common/intrect.h"
#include "../common/realrect.h"

#include "../css/cssdeclared.h"
#include "../css/cssdeclaredgroup.h"
#include "../css/cssselector.h"
#include "../css/cssstackingcontext.h"
#include "../drawable/drawable.h"

#include <string>

namespace hfcl {

class GraphicsContext;
class ViewContainer;
class RootView;
class ScrollBar;
class CssComputed;
class CssBox;

enum {
    PAINT_STATUS_SHIFT = 24,
    PAINT_STATUS_MASK  = ((1 << PAINT_STATUS_SHIFT) - 1),
    PAINT_NO_HILIGHT = 0x80000000
};

typedef struct {
    View *view;
    int x;
    int y;
} ViewClickEventStruct;

class View : public Object {
public:
    View(const char* tag, const char* cssCls = NULL,
            const char* name = NULL, int id = 0);
    virtual ~View();

    bool attach(ViewContainer* parent);
    bool detach();

    /* operators for view relationship */
    const View* getPrev() const { return m_prev; }
    View* getPrev() { return m_prev; }
    void setPrev(View* view) { m_prev = view; }

    const View* getNext() const { return m_next; }
    View* getNext() { return m_next; }
    const View* getNextVisible() const { return m_next; }

    void setNext(View* view) { m_next = view; }

    const ViewContainer* getParent() const { return m_parent; }
    ViewContainer* getParent() { return m_parent; }
    void setParent(ViewContainer* view) {
        if (m_parent && m_parent != view) {
            detach();
        }
        if (view) {
            attach(view);
        }
    }

    const RootView* getRoot() const;
    RootView* getRoot();
    const CssComputed* getCssComputed() const { return m_css_computed; }
    CssComputed* getCssComputed() { return m_css_computed; }

    // return the HVML tag, e.g., hvroot, hvtext, hvimg, hvli, and so on
    const char* tag() const { return m_tag; }

    // methods to get/set identifier (integer) of the view
    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    // methods to get/set additional data of the view
    void setAddData(const void *addData) { m_addData = addData; }
    const void *getAddData() const { return m_addData; }

    // methods to get/set name of the view
    // name corresponds to the id attribute of one HVML element
    const char* getName() const { return m_name.c_str(); }
    bool setName(const char* name);

    // methods to get/set content of the view
    // return the text content of this view
    virtual const char* getContent() const { return NULL; }
    virtual bool setContent(const char* content) { return TRUE; }

    // Operators for CSS class of the view
    const char* getClasses() { return m_cssCls.c_str(); }
    bool setClasses(const char* cssClses);
    bool includeClass(const char* cssCls);
    bool excludeClass(const char* cssCls);

    /* Operators to get/set flags (intrinsic attributes) of the view */
    bool isDisabled() { return m_flags & VA_DISABLED; }
    void disable() {
        bool old = setFlag(true, VA_DISABLED);
        if (old)
            onDisabled();
    }

    bool isEnabled() { return !(m_flags & VA_DISABLED); }
    void enable() {
        bool old = setFlag(false, VA_DISABLED);
        if (!old)
            onEnabled();
    }

    bool isChecked() { return m_flags & VA_CHECKED; }
    void check() {
        bool old = setFlag(true, VA_CHECKED);
        if (!old)
            onChecked();
    }
    void uncheck() {
        bool old = setFlag(false, VA_CHECKED);
        if (old)
            onUnchecked();
    }

    bool isActive() { return m_flags & VA_ACTIVE; }
    void activate() {
        bool old = setFlag (true, VA_ACTIVE);
        if (!old) {
            onActive();
        }
    }
    void deactivate(bool update = true) {
        bool old = setFlag (false, VA_ACTIVE);
        if (old) {
            onInactive();
        }
    }

    bool isFrozen() { return m_flags & VA_FROZEN; }
    void freeze() {
        bool old = setFlag (true, VA_FROZEN);
        if (!old) {
            onFrozen();
        }
    }
    void unfreeze(bool update = true) {
        bool old = setFlag (false, VA_FROZEN);
        if (old) {
            onUnfrozen();
        }
        if (update) {
            updateView ();
        }
    }

    bool isFocused() { return m_flags & VA_FOCUSED; }
    void focus() {
        bool old = setFlag(true, VA_FOCUSED);
        if (!old)
            onGotFocus();
    }
    void unfocus() {
        bool old = setFlag(false, VA_FOCUSED);
        if (!old)
            onLostFocus();
    }

    bool isFocusable() { return isEnabled() && m_flags & VA_FOCUSABLE; }
    bool setFocusable(bool b) { return setFlag(b, VA_FOCUSABLE); }

    bool isFocusStoppable() {
        return isFocusable() && m_flags & VA_FOCUSSTOPPABLE;
    }
    bool setFocusStoppable(bool b) { return setFlag(b, VA_FOCUSSTOPPABLE); }

    // Operators for user-defined attributes of the view
    const char* getAttribute(const char* attrKey) const;
    bool setAttribute(const char* attrKey, const char* attrValue);

    // Operators to check various attributes
    bool checkClass(const char* cssCls) const;
    bool checkPseudoElement(const char* pseudoEle) const;
    bool checkPseudoClass(const char* pseudoCls) const;
    bool checkAttribute(const char* attrKey, const char* attrValue) const;
    bool checkAttribute(const char* attrPair) const;

    virtual bool isContainer() const { return false; }
    virtual bool isRoot() const { return false; }
    virtual bool isReplaced() const { return false; }
    virtual bool getIntrinsicWidth(HTReal* v) const { return false; }
    virtual bool getIntrinsicHeight(HTReal* v) const { return false; }
    virtual bool getIntrinsicRatio(HTReal* v) const { return false; }
    virtual bool getShrinkToFitWhidth(HTReal* preferred,
            HTReal* minimum) const { return false; }

    // return the text content of this view
    virtual const char* getTextContent() const { return 0; }

    enum {
        VN_GOTFOCUS,
        VN_LOSTFOCUS,
        VN_HOVERED,
        VN_LEFT,
        VN_ACTIVE,
        VN_INACTIVE,
        VN_CLICKED,
        VN_DBLCLICKED,
        VN_VIEW_MAX,
    };

    /* virtual functions for attribute changes */
    virtual void onContainingBlockChanged();
    virtual void onNameChanged();
    virtual void onClassChanged();
    virtual void onDisabled();
    virtual void onEnabled();
    virtual void onChecked();
    virtual void onUnchecked();
    virtual void onGotFocus();
    virtual void onLostFocus();
    virtual void onHovered();
    virtual void onLeft();
    virtual void onActive();
    virtual void onInactive();

    virtual void onFrozen();
    virtual void onUnfrozen();
    virtual void onClicked();
    virtual void onDoubleClicked();
    virtual void onIdle();

    // virtual functions for drawing the view
    virtual void drawBackground(GraphicsContext* context, IntRect &rc);
    virtual void drawContent(GraphicsContext* context, IntRect &rc);
    virtual void drawScrollBar(GraphicsContext* context, IntRect &rc);
    virtual void onPaint(GraphicsContext* context);

    /*
     * virtual callbacks to handle the interaction events
     * Return false if the view is not intersted in the event.
     */
    virtual bool onKeyEvent(const KeyEvent& evt) { return false; }
    virtual bool onMouseEvent(const MouseEvent& evt) { return false; }
    virtual bool onMouseWheelEvent(const MouseWheelEvent& evt) { return false; }
    virtual bool onChar(const char* mchar) { return false; }

    virtual void viewToWindow(int *x, int *y);
    virtual void windowToView(int *x, int *y);

    void addEventListener(EventListener* listener);
    void removeEventListener(EventListener* listener);

    // the style sheet specified on the fly
    CssDeclared* myCss() { return &m_cssd_user; }

    // Change style on the fly
    bool getProperty(CssPropertyIds pid, Uint32 *value,
            HTPVData* data);
    bool setProperty(CssPropertyIds pid, Uint32 value,
            HTPVData data);

    // x,y coordiat from view, e.g (0,0) is same as m_rect.left, m_rect.top
    void updateView(int x, int y, int w, int h);
    void updateView(const IntRect &rc);

    virtual void onChildUpdateView(View *child,
            int x, int y, int w, int h, bool upBackGnd = true);
    void updateViewRect(const IntRect &rc);
    void updateViewRect();
    void updateView(bool upBackGnd = true);

    /* to be deprecated */
    virtual void focusMe();
    bool setFocus(View *view);

    virtual bool isWrapperView() { return false; }

    bool isVisible() { return m_flags & VA_VISIBLE; }

    void getSize(int *w, int *h) {
        if (w) *w = m_rect.width();
        if (h) *h = m_rect.height();
    }

    const IntRect& getRect() const { return m_rect; }
    const IntRect& getViewportRect() const { return m_rc_viewport; }

    void setPosition(int x, int y, bool update) {
        IntRect rc(x, y, m_rect.width() + x, m_rect.height() + y);

        if (update)
            setRect(rc);
        else
            m_rect = rc;
    }

    void getPosition(int *x, int *y) {
        if (x) *x = m_rect.left();
        if (y) *y = m_rect.top();
    }

    virtual void show() {
        setVisible(true);
        updateView();
    }
    virtual void hide() {
        setVisible(false);
        updateView();
    }

    View(View* parent, DrawableSet* drset);
    View(int id, int x, int y, int w, int h);

    bool setVisible(bool b) { return setFlag(b, VA_VISIBLE); }

    bool setRect(int left, int top, int right, int bottom) {
        return setRect(IntRect(left, top, right, bottom));
    }

    bool setRectWH(int left, int top, int width, int height) {
        return setRect(IntRect(left, top, width + left, height + top));
    }

    bool setRectNoUpdate(int left, int top, int right, int bottom) {
        IntRect irc = IntRect(left, top, right, bottom);
        if (irc == m_rect)
            return false;

        if (m_rect.width() == 0 || m_rect.height() == 0) {
            // VincentWei: for the initial set, do not call updateView()
            m_rect = irc;
            return false;
        }
        else if (m_rect != irc) {
            // VincentWei: call updateView only set a different rect
            m_rect = irc;
            return true;
        }
        return false;
    }

    virtual bool setRect(const IntRect& irc) {
        if (irc == m_rect)
            return false;

        if (m_rect.width() == 0 || m_rect.height() == 0) {
            // VincentWei: for the initial set, do not call updateView()
            m_rect = irc;
        }
        else if (m_rect != irc) {
            // VincentWei: call updateView only set a different rect
            m_rect = irc;
            updateView ();
        }
        return true;
    }

    friend class ViewContainer;
    friend class RootView;

protected:
    enum {
        VA_DISABLED         = (0x01 << 0),
        VA_HOVER            = (0X01 << 1),
        VA_ACTIVE           = (0x01 << 2),
        VA_CHECKED          = (0x01 << 3),
        VA_FOCUSED          = (0x01 << 4),
        VA_FOCUSABLE        = (0x01 << 5),
        VA_FOCUSSTOPPABLE   = (0x01 << 6),
        VA_FROZEN           = (0x01 << 7),
        VA_VISIBLE          = (0x01 << 8),
        FLAG_SHIFT          = 9
    };

    inline bool setFlag(bool b, unsigned int flag) {
        bool old = m_flags & flag;
        if(b)
            m_flags |= flag;
        else
            m_flags &= (~flag);
        return old;
    }

    const char* m_tag;
    int m_id;
    std::string m_cssCls;
    std::string m_name;
    const void* m_addData;
    Uint32 m_flags;

    ViewContainer *m_parent;
    View *m_prev;
    View *m_next;

    MAPCLASSKEY(utf8string, utf8string, AttributesMap);
    AttributesMap m_attrs;

    /* CSS-related members */
    // user-defined CSS (properties specified on the fly)
    CssDeclared m_cssd_user;
    // All selected CssDeclared objects for the view
    CssDeclaredGroupWithSpecificity m_cssdg_static;
    CssDeclaredGroupWithSpecificity m_cssdg_dynamic;
    // The computed values of all CSS properties.
    CssComputed* m_css_computed;

    /* virtual functions for rendering */
    // compute CSS property values
    virtual void applyCss(CssDeclared* css, const CssSelectorGroup& selector);
    virtual void computeCss();

    virtual void makeCssBox();
    virtual void makeStackingContext(CssStackingContext* cssSc);
    virtual void layOut(CssBox* ctnBlock);

    // The pricipal box of the view, either a CssBox,
    // a CssLineBoxContainer, or a CssBlockBoxContainer
    CssBox* m_cssbox_principal;
    // The additional box of the view if Property display is list-item.
    CssBox* m_cssbox_additional;

    IntRect m_rc_viewport;
    IntRect m_rect;

    LISTEX(EventListener *, EventListenerList,
            do { return *v1 == *v2; } while (0),
            do { (*n)->unref(); } while (0));
    EventListenerList m_listeners;
    void releaseEventListeners();

    bool raiseViewEvent(ViewEvent *event);

    /* Members related to rendering */
    virtual void inner_updateView(int x, int y, int w, int h,
            bool upBackGnd = true);
    void inner_updateViewRect(int x, int y, int w, int h);

private:

    /* Members for scroll bars */
    ScrollBar* m_vsb;
    ScrollBar* m_hsb;
};

} // namespace hfcl

#endif /* HFCL_VIEW_VIEW_H_ */
