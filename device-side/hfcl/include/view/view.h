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
class CssBoxPrincipal;

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

    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    const char* getName() const { return m_name; }
    void setName(const char* name);

    bool applyClass(const char* cssClass);
    bool includeClass(const char* cssClass);
    bool excludeClass(const char* cssClass);

    void getSize(int *w, int *h) {
        if (w) *w = m_rect.width();
        if (h) *h = m_rect.height();
    }

    virtual void show() {
        setVisible(true);
        updateView();
    }
    virtual void hide() {
        setVisible(false);
        updateView();
    }
    inline bool isVisible() { return visible(); }

    void addEventListener(EventListener* listener);
    void addEventListener(EventListener* listener, int event_type) { }
    void removeEventListener(EventListener* listener);

    virtual void paint(GraphicsContext* context, int status);
    virtual void onPaint(GraphicsContext* context, int status);
    virtual void drawBackground(GraphicsContext* context,
            IntRect &rc, int status);
    virtual void drawContent(GraphicsContext* context, IntRect &rc, int status);
    virtual void drawScroll(GraphicsContext* context, IntRect &rc, int status);

    virtual void focusMe();
    bool setFocus(View *view);
    bool isFocused();

    virtual void onGotFocus();
    virtual void onLostFocus();
    virtual void onClicked();
    virtual void onDoubleClicked();

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

    virtual bool isTopView() { return false; }
    virtual bool isContainerView() { return false; }
    virtual bool isWrapperView() { return false; }

    virtual HWND getSysWindow() {
        return m_parent ? ((View*)m_parent)->getSysWindow() : 0;
    }

    virtual void viewToWindow(int *x, int *y);
    virtual void windowToView(int *x, int *y);

    virtual bool raiseEvent(Event *event);

    void setAddData(void *paddData) {m_addData = paddData;}
    void *addData() {return m_addData;}

    bool isDisabled() { return m_flags & DISABLED; }
    void disable(bool b) { return setFlag(b, DISABLED); }

    void setVisible(bool b) { setFlag(b, VISIBLE); }
    bool visible() { return (m_flags & VISIBLE) == VISIBLE; }

    void setFocusValid(bool b) { setFlag(b, FOCUSVALID); }
    bool focusValid() { return (m_flags & FOCUSVALID) == FOCUSVALID; }
    inline bool focusable() { return (isVisible() && !isDisabled()); }

    void setFocusStop(bool b) { setFlag(b, FOCUSSTOP); }
    bool isFocusStop() { return m_flags & FOCUSSTOP; }
    bool focusStopable() { return focusable() && isFocusStop(); }

    /* to be deprecated */
    View(View* parent, DrawableSet* drset);
    View(int id, int x, int y, int w, int h);

    enum {
        NORMAL,
        PUSHDOWN,
        DISABLE,
        FOCUS
    };

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
    inline const IntRect& getRect() const{ return m_rect; }

    void setPosition(int x, int y, bool bupdate = true) {
        IntRect rc(x, y, m_rect.width()+x, m_rect.height()+y);

        if (bupdate)
            setRect(rc);
        else
            m_rect = rc;
    }

    void getPosition(int *x, int *y) {
        if(x) *x = m_rect.left();
        if(y) *y = m_rect.top();
    }

    void setAlpha(unsigned char trans);
    unsigned char alpha();

    virtual void changeTheme();
    bool isThemeAble() { return m_flags & THEMEABLE; }
    void themeAble(bool b) { return setFlag(b, THEMEABLE); }
    int themeDrsetId() { return m_theme_drset_id; }
    void setThemeDrsetId(int iid) { m_theme_drset_id = iid; }

    virtual void autoFitSize(bool auto_child_fit = false) {  }
    virtual void autoFitSize() { }

    DrawableSet* getDrawableSet() const;
    void setDrawableSet(DrawableSet* drset);

    void freezeUpdate() {
        setFlag (true, FROZEN);
    }
    void unfreezeUpdate (bool update = true) {
        setFlag (false, FROZEN);
        if (update) {
            updateView();
        }
    }
    bool shouldUpdate() {
        return (m_flags & FROZEN) != FROZEN;
    }

    int setLayer(int layerNo) { int old = m_drawLayer; m_drawLayer = layerNo; return old; }
    int layer() { return m_drawLayer; }

protected:
    enum {
        VISIBLE     = (0x1 << 0),
        DISABLED    = (0x1 << 1),
        FOCUSVALID  = (0x1 << 2),
        FOCUSSTOP   = (0x1 << 3),
        THEMEABLE   = (0x1 << 4),
        FROZEN      = (0x1 << 5),
        FLAG_SHIFT  = 6
    };

    int m_id;
    Uint32 m_flags;

    const char* m_name;
    const char* m_css_classes[MAX_VIEW_CLASSES];
    void* m_addData;

    ContainerView *m_parent;
    View *m_prev;
    View *m_next;

    CssComputed* m_css_computed;
    CssBoxPrincipal* m_box_principal;

    IntRect m_rect;

    // to be deprecated
    DrawableSet* m_drset;
    unsigned char m_alpha;
    int m_theme_drset_id;
    int m_drawLayer;

    LISTEX(EventListener *, EventListenerList,
            do{return *v1 == *v2;}while (0), do{(*n)->unref();} while (0));
    EventListenerList m_listeners;
    void releaseEventListeners();

    inline void setFlag(bool b, unsigned int flag) {
        if(b)
            m_flags |= flag;
        else
            m_flags &= (~flag);
    }

    virtual void inner_updateView(int x, int y, int w, int h, bool upBackGnd = true);
    void inner_updateViewRect(int x, int y, int w, int h);

    // to be deprecated
    bool performClick();
};

} // namespace hfcl

#endif /* HFCL_VIEW_VIEW_H_ */
