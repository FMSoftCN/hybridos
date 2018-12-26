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

#include "../css/cssdeclared.h"
#include "../css/cssdeclaredgroup.h"
#include "../css/cssselector.h"
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
    enum {
        VN_BEGIN = CustomEvent::CUS_MAX,
        VN_GOTFOCUS,
        VN_LOSTFOCUS,
        VN_CLICKED,
        VN_DBLCLICKED,
        VN_VIEW_MAX,
    };

    View(int id = 0, const char* cssClass = NULL, const char* name = NULL);
    virtual ~View();

    bool attach(ViewContainer* parent);
    bool detach();

    /* public methods to get/set attributes */
    // identifier (integer) of this view
    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    // additional data
    void setAddData(const void *addData) { m_addData = addData; }
    const void *getAddData() const { return m_addData; }

    // name correspond to the id attribute of one HVML element
    const char* getName() const { return m_name.c_str(); }
    bool setName(const char* name);

    // Operators for CSS class of this view
    const char* getClass() { return m_cssCls.c_str(); }
    bool setClasses(const char* cssClasses);
    bool includeClass(const char* cssClass);
    bool excludeClass(const char* cssClass);

    bool checkClass(const char* cls) const;
    bool checkAttribute(const char* attr) const;
    bool checkPseudoElement(const char* pseudoEle) const;
    virtual bool checkPseudoClass(const char* pseudoCls) const;

    /* virtual functions for rendering */
    // return the HVML type, e.g., hvroot, hvtext, hvimg, hvli, and so on
    virtual const char* type() const = 0;
    virtual void applyCss(CssDeclared* css,
            CssSelectorGroup& selector);
    virtual void computeCss(int w, int h);
    virtual void onNameChanged() = 0;
    virtual void onClassChanged() = 0;
    virtual void onDisabled() = 0;
    virtual void onEnabled() = 0;
    virtual void onChecked() = 0;
    virtual void onUnchecked() = 0;
    virtual void onActive() = 0;
    virtual void onInactive() = 0;
    virtual void onFrozen() = 0;
    virtual void onUnfrozen() = 0;
    virtual void onGotFocus() = 0;
    virtual void onLostFocus() = 0;
    virtual void onViewportSizeChanged();

    virtual bool isContainer() const { return false; }
    virtual bool isRoot() const { return false; }

    // virtual functions for drawing the view
    virtual void drawBackground(GraphicsContext* context, IntRect &rc) = 0;
    virtual void drawContent(GraphicsContext* context, IntRect &rc) = 0;
    virtual void drawScrollBar(GraphicsContext* context, IntRect &rc) = 0;
    virtual void onPaint(GraphicsContext* context);

    // return True if the event was handled, false otherwise.
    virtual bool dispatchEvent(Event* event);
    virtual bool raiseEvent(Event *event);

    /* callbacks to handle the interaction events */
    virtual bool onKeyDown(int keyCode, KeyEvent* event) { return false; }
    virtual bool onChar(const char* mchar) { return false; }
    virtual bool onKeyUp(int keyCode, KeyEvent* event) { return false; }

    virtual bool onMouseDown(int x, int y, DWORD keyStatus) { return false; }
    virtual bool onMouseUp(int x, int y, DWORD keyStatus) { return false; }
    virtual bool onMouseIn(int x, int y, DWORD keyStatus) { return false; }
    virtual bool onMouseMove(int x, int y, DWORD keyStatus) { return false; }
    virtual bool onMouseOut(int x, int y, DWORD keyStatus) { return false; }

    /* high-level events */
    virtual void onClicked();
    virtual void onDoubleClicked();
    virtual void onIdle();

    /* method to get/set attributes (flags) of the view */
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

    /* methods for silbing travelling */
    const View* prevSibling() const { return m_prev; }
    View* prevSibling() { return m_prev; }
    void setPrevSlibling(View* view) { m_prev = view; }

    const View* nextSibling() const { return m_next; }
    View* nextSibling() { return m_next; }
    void setNextSlibling(View* view) { m_next = view; }

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

    RootView* getRoot();
    virtual HWND getSysWindow();

    virtual void viewToWindow(int *x, int *y);
    virtual void windowToView(int *x, int *y);

    void addEventListener(EventListener* listener);
    void removeEventListener(EventListener* listener);

    // x,y coordiat from view, e.g (0,0) is same as m_rect.left, m_rect.top
    void updateView(int x, int y, int w, int h);
    void updateView(const IntRect &rc);

    virtual void onChildUpdateView(View *child,
            int x, int y, int w, int h, bool upBackGnd = true);
    void updateViewRect(const IntRect &rc);
    void updateViewRect();
    void updateView(bool upBackGnd = true);

    /* to be deprecated */
    virtual bool isWrapperView() { return false; }

    virtual void focusMe();
    bool setFocus(View *view);

    bool isVisible();

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

    /* to be deprecated */
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

    int m_id;
    std::string m_cssCls;
    std::string m_name;
    const void* m_addData;
    Uint32 m_flags;

    ViewContainer *m_parent;
    View *m_prev;
    View *m_next;

    /* CSS-related members */
    // user-defined CSS (properties specified on the fly)
    CssDeclared* m_cssd_user;
    // All selected CssDeclared objects for this view
    CssDeclaredGroup m_cssdg_static;
    CssDeclaredGroup m_cssdg_dynamic;
    // The computed values of all CSS properties.
    CssComputed* m_css_computed;
    // The pricipal box of this view, either a CssBoxBlock,
    // a CssBoxInline, or a CssBoxLineBoxContainer
    CssBox* m_box_principal;
    // The additional box of this view if Property display is list-item.
    CssBox* m_box_additional;

    IntRect m_rc_viewport;
    IntRect m_rect;
    ScrollBar* m_vsb;
    ScrollBar* m_hsb;

    LISTEX(EventListener *, EventListenerList,
            do{return *v1 == *v2;}while (0), do{(*n)->unref();} while (0));
    EventListenerList m_listeners;
    void releaseEventListeners();

    inline bool setFlag(bool b, unsigned int flag) {
        bool old = m_flags & flag;
        if(b)
            m_flags |= flag;
        else
            m_flags &= (~flag);
        return old;
    }

    virtual void inner_updateView(int x, int y, int w, int h,
            bool upBackGnd = true);
    void inner_updateViewRect(int x, int y, int w, int h);

private:
    int matchCssSelector (const char* selector);
};

} // namespace hfcl

#endif /* HFCL_VIEW_VIEW_H_ */
