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


#ifndef HFCL_VIEW_VIEWCONTAINER_H_
#define HFCL_VIEW_VIEWCONTAINER_H_

#include "../view/view.h"
#include "../css/cssselector.h"

namespace hfcl {

class ViewContainer : public View {
public:
    ViewContainer(int id = 0, const char* cssClass = NULL,
            const char* name = NULL);
    virtual ~ViewContainer();

    /* methods specfic to ViewContainer */
    bool isChild(const View* view) const;
    bool insertBefore(View *view, View *child);
    bool insertAfter(View *view, View *child);
    bool insertBefore(int idx, View* child);
    bool insertAfter(int idx, View* child) {
        return child ? insertAfter(getChildByIndex(idx), child) : false;
    }

    bool addChild(View* child) { return insertAfter(m_last, child); }
    bool addChildHead(View* child) { return insertBefore(m_first, child); }
    // by default , we delete the child, but NOT when bRelease = false
    int  removeChild(View* child, bool bRelease = true);
    // by default , we delete the child, but NOT when bRelease = false
    int  removeChild(int index, bool bRelease = true);
    int  removeAll();
    int  detachChild(View* child);
    int  detachChildByIndex(int idx);
    void clean();

    const View* getChildById(int id) const;
    View* getChildById(int id);
    const View* getChildByIndex(int idx) const;
    View* getChildByIndex(int idx);
    const View *getChildByPosition(int x_pos, int y_pos) const;
    View *getChildByPosition(int x_pos, int y_pos);

    int   getChildIndex(const View *view) const;

    const View* findDescendant(int id) const;
    View* findDescendant(int id);

    const View* firstChild() const { return m_first; }
    View* firstChild() { return m_first; }
    const View* lastChild() const { return m_last; }
    View* lastChild() { return m_last; }
    const View* firstEnabledChild() const;
    View* firstEnabledChild();
    int  childrenCount() const { return m_nr_children; }

    void focusChild(View* view);
    void unfocusChild();
    const View* getFocusedChild() const { return m_focused; }
    View* getFocusedChild() { return m_focused; }

    /* overloaded virtual functions */
    virtual bool isContainer() const { return true; }
    virtual void applyCss(CssDeclared* css, const CssSelectorGroup& selector);
    virtual void drawBackground(GraphicsContext* context, IntRect &rc);
    virtual void drawContent(GraphicsContext *context, IntRect &rc);
    virtual bool dispatchEvent(Event* event);

    /* new virtual functions for ViewContainer */
    virtual void onChildAttached(View* view);
    virtual void onChildDetached(View* view);
    virtual void onChildStyleChanged(View* view);
    virtual void onChildSizeChanged(View* child);

    /* to be deprecated */
    virtual void autoFitSize(bool auto_child_fit = false);
    void setAutoSize(bool b) { setFlag(b, AUTOSIZE); }
    bool isAutoSize() { return m_flags & AUTOSIZE; }

    virtual bool onKeyEvent(const KeyEvent* event);
    virtual bool onMouseEvent(const MouseEvent* event);
    virtual bool onMouseWheelEvent(const MouseWheelEvent* event);
    virtual void onPaint(GraphicsContext* context);

protected:
    View *m_focused;
    View *m_first;
    View *m_last;
    int   m_nr_children;

    enum {
        AUTOSIZE = (1 << View::FLAG_SHIFT),
        FLAG_SHIFT = (View::FLAG_SHIFT + 1)
    };

    virtual void computeCss();
};

} // namespace hfcl

#endif /* HFCL_VIEW_VIEWCONTAINER_H_ */

