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
#include "../drawable/drawable.h"

#define MAX_VIEW_CLASSES    4

namespace hfcl {

class GraphicsContext;
class ContainerView;
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

    View();
    View(View* parent);
    View(View* parent, const char* cssClass, const char* name, int id = 0);
    virtual ~View();

    /* pure virtual functions */
    // return the HVML tag, e.g., hvroot, hvlist, hvimg, hvli, and so on
    virtual const char* tag() const = 0;
    virtual bool isRootView() { return false; }
    virtual bool isContainerView() { return false; }
    virtual bool isWrapperView() { return false; }

    // draw view
    virtual void drawBackground(GraphicsContext* context, IntRect &rc) = 0;
    virtual void drawContent(GraphicsContext* context, IntRect &rc) = 0;
    virtual void drawScroll(GraphicsContext* context, IntRect &rc) = 0;

    /* callbacks to handle the events of Windowing system */
    virtual void onPaint(GraphicsContext* context);
    virtual void onClicked();
    virtual void onDoubleClicked();
    virtual void onViewportChanged();

    /* methods to get/set attributes */
    // identifier (integer) of this view
    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    // additional data
    void setAddData(void *paddData) { m_addData = paddData; }
    void *getAddData() {return m_addData;}

    // name correspond to the id attribute of one HVML element
    const char* getName() const { return m_name; }
    void setName(const char* name);

    // CSS class of one HVML element
    bool applyClass(const char* cssClass);
    bool includeClass(const char* cssClass);
    bool excludeClass(const char* cssClass);

    virtual void onDisabled() = 0;
    virtual void onEnabled() = 0;
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

    virtual void onChecked() = 0;
    virtual void onUnchecked() = 0;
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

    virtual void onFrozen() = 0;
    virtual void onUnfrozen() = 0;
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

    virtual void onGotFocus() = 0;
    virtual void onLostFocus() = 0;
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
    void addEventListener(EventListener* listener);
    void addEventListener(EventListener* listener, int event_type) { }
    void removeEventListener(EventListener* listener);

    //return True if the event was handled, false otherwise.
    virtual bool dispatchEvent(Event* event);

    // x,y coordiat from view, e.g (0,0) is same as m_rect.left, m_rect.top
    void updateView(int x, int y, int w, int h);
    void updateView(const IntRect &rc);

    virtual void onChildUpdateView(View *child, \
            int x, int y, int w, int h, bool upBackGnd = true);
    void updateViewRect(const IntRect &rc);
    void updateViewRect();
    void updateView(bool upBackGnd = true);

    void setParent(ContainerView* view) { m_parent = view; }
    void setPrevSlibling(View* view) { m_prev = view; }
    void setNextSlibling(View* view) { m_next = view; }

    const View* prevSibling() const { return m_prev; }
    View* prevSibling() { return m_prev; }
    const View* nextSibling() const { return m_next; }
    View* nextSibling() { return m_next; }
    const ContainerView* parent() const { return m_parent; }
    ContainerView* parent() { return m_parent; }
    ContainerView* rootView();

    virtual HWND getSysWindow() {
        return m_parent ? ((View*)m_parent)->getSysWindow() : 0;
    }

    virtual void viewToWindow(int *x, int *y);
    virtual void windowToView(int *x, int *y);

    virtual bool raiseEvent(Event *event);

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
        VA_DISABLED         = (0x1 << 0),
        VA_CHECKED          = (0x1 << 1),
        VA_FOCUSED          = (0x1 << 2),
        VA_FOCUSABLE        = (0x1 << 3),
        VA_FOCUSSTOPPABLE   = (0x1 << 4),
        VA_FROZEN           = (0x1 << 5),
        VA_VISIBLE          = (0x1 << 6),
        FLAG_SHIFT          = 8
    };

    int m_id;
    Uint32 m_flags;

    const char* m_name;
    const char* m_css_classes[MAX_VIEW_CLASSES];
    void* m_addData;

    ContainerView *m_parent;
    View *m_prev;
    View *m_next;

    // CSS-related members
    // The computed values of all CSS properties.
    CssComputed* m_css_computed;
    // The pricipal box of this view, either a CssBoxBlock,
    // a CssBoxInline, or a CssBoxLineBoxContainer
    CssBox* m_box_principal;
    // The additional box of this view if Property display is list-item.
    CssBox* m_box_additional;

    IntRect m_rc_viewport;
    IntRect m_rect;

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
};

} // namespace hfcl

#endif /* HFCL_VIEW_VIEW_H_ */
