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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef _NGUX_View_h
#define _NGUX_View_h


#include "drawable.h"
#include "nguxevent.h"
#include "intrect.h"
#include "viewcontext.h"

NAMESPACE_BEGIN

class GraphicsContext;
class ContainerView;

typedef unsigned int HPlatformOwner;

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
		View();
        View(View* parent);
		View(View* parent, DrawableSet* drset);
		View(int id, int x, int y, int w, int h);
		virtual ~View();

		enum {
			NOTIFY_BEGIN = CustomEvent::CUS_MAX,
			NOTIFY_GET_FOCUS,
			NOTIFY_LOSE_FOCUS,
			NOTIFY_ON_CLICK,
			NOTIFY_VIEW_MAX
		};

		int id(void) const { return m_id; }
		void setId(int iid) { m_id = iid; }

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
		inline const IntRect& getRect(void) const{ return m_rect; }

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

		void getSize(int *w, int *h) {
			if(w) *w = m_rect.width();
			if(h) *h = m_rect.height();
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

		virtual void paint(GraphicsContext* context, int status /*= Style::NORMAL*/);
		virtual void onPaint(GraphicsContext* context, int status /*= Style::NORMAL*/);
		virtual void changeTheme(void);
		virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
		virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
		virtual void drawScroll(GraphicsContext* context, IntRect &rc, int status /*=Style::NORMAL*/);

		virtual void onGetFocus();
		virtual void onLoseFocus();
		bool setFocus(View *view);

		//return True if the event was handled, false otherwise.
		virtual bool dispatchEvent(Event* event);

		// x,y coordiat from view, e.g (0,0) is same as m_rect.left, m_rect.top
		void updateView(int x, int y, int w, int h);
		void updateView(const IntRect &rc);
		
		virtual void onChildUpdateView(View *child, int x, int y, int w, int h, bool upBackGnd = true);
		void updateViewRect(const IntRect &rc);
		void updateViewRect(void);
		void updateView(bool upBackGnd = true);

		void setParent(ContainerView* view) { m_parent = view; }
		void setPrevSlibling(View* view) { m_prev = view; }
		void setNextSlibling(View* view) { m_next = view; }

		const View* previousSibling(void) const { return m_prev; }
		View* previousSibling(void) { return m_prev; }
		const View* nextSibling(void) const { return m_next; }
		View* nextSibling(void) { return m_next; }
		const ContainerView* parent(void) const { return m_parent; }
		ContainerView* parent(void) { return m_parent; }
		ContainerView* rootView(void);

		virtual void focusMe(void);
		bool isFocus(void);

		virtual bool isTopView(void) { return false; }
		virtual bool isContainerView(void) { return false; }
		virtual bool isWrapperView(void) { return false; }

        void setAlpha(unsigned char trans);
        unsigned char alpha();

		virtual HPlatformOwner getPlatformOwner() { 
			return m_parent ? ((View*)m_parent)->getPlatformOwner() : 0;
		}

        virtual void viewToWindow(int *x, int *y);
        virtual void windowToView(int *x, int *y);

		virtual bool raiseEvent(Event *event);

        void setAddData(void *paddData) {m_addData = paddData;}
        void *addData(void) {return m_addData;}

		virtual void autoFitSize() { }

		DrawableSet* getDrawableSet(void) const;
		void setDrawableSet(DrawableSet* drset);

		bool isDisabled() { return m_flags & DISABLED; }
		void disable(bool b) { return setFlag(b, DISABLED); }

		// theme related
		bool isThemeAble() { return m_flags & THEMEABLE; }
		void themeAble(bool b) { return setFlag(b, THEMEABLE); }
		int themeDrsetId(void) { return m_theme_drset_id; }
		void setThemeDrsetId(int iid) { m_theme_drset_id = iid; }

		enum {
			NORMAL,
			PUSHDOWN,
			DISABLE,
			FOCUS
		};

		virtual void autoFitSize(bool auto_child_fit = false) {  }

    protected:
		bool performClick();

		IntRect m_rect;
		int m_id;
		DrawableSet* m_drset;
        unsigned char m_alpha;
		DWORD m_flags;
		View *m_prev;
		View *m_next;
		ContainerView *m_parent;
        void *m_addData;
		int m_theme_drset_id;
		int m_drawLayer;

        /* VincentWei: Whether the view is opaque; true by default.
         * If it was opaque, the rectangle of the view will be excluded when erasing background of parent.
        bool m_opaque;
        bool isOpaque() { return m_opaque; }
        void setOpaque(bool opaque) { m_opaque = opaque; }
         */

		LISTEX(EventListener *, EventListenerList, do{return *v1 == *v2;}while (0), do{(*n)->unref();} while (0));
		EventListenerList m_listeners;
		void releaseEventListeners();

		enum {
			VISIBLE     = (0x1 << 0),
			DISABLED    = (0x1 << 1),
			FOCUSVALID  = (0x1 << 2),
			FOCUSSTOP   = (0x1 << 3),
			THEMEABLE   = (0x1 << 4),
			FROZEN      = (0x1 << 5),
			FLAG_SHIFT  = 6
		};

		inline void setFlag(bool b, unsigned int flag) {
			if(b) 
				m_flags |= flag;
			else
				m_flags &= (~flag);
		}

		virtual void inner_updateView(int x, int y, int w, int h, bool upBackGnd = true);
		void inner_updateViewRect(int x, int y, int w, int h);

public:
        void freezeUpdate () {
            setFlag (true, FROZEN);
        }
        void unfreezeUpdate (bool update = true) {
            setFlag (false, FROZEN);
            if (update) {
                updateView ();
            }
        }
        bool shouldUpdate () {
            return (m_flags & FROZEN) != FROZEN;
        }

		void setVisible(bool b) { setFlag(b, VISIBLE); }
		bool visible() { return (m_flags & VISIBLE) == VISIBLE; }

		void setFocusValid(bool b) { setFlag(b, FOCUSVALID); }
		bool focusValid() { return (m_flags & FOCUSVALID) == FOCUSVALID; }

		inline bool focusable() { return (isVisible() && !isDisabled()); }

		void setFocusStop(bool b) { setFlag(b, FOCUSSTOP); }
		bool isFocusStop() { return m_flags & FOCUSSTOP; }
		bool focusStopable() { return focusable() && isFocusStop(); }

		int setLayer(int layerNo) { int old = m_drawLayer; m_drawLayer = layerNo; return old; }
		int layer () { return m_drawLayer; }
};

NAMESPACE_END

#endif /* NGUX_View_h */
