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


#ifndef HFC_VIEW_SCROLLVIEW_H_
#define HFC_VIEW_SCROLLVIEW_H_

#include "panelview.h"
#include "../services/timerservice.h"

namespace hfcl {

class ScrollView : public PanelView, public TimerEventListener
{
public:
    ScrollView(View *p_parent);
    ScrollView(View *p_parent, DrawableSet* drset);
    virtual ~ScrollView(void);

    /* \brief this function move the view port to the specific position.
     * * \param[in] x the destination X coordinate.
     * * \param[in] y the destination Y coordinate.
     * */
    void moveViewport(int x, int y, bool bupdate =true);

    void updateContentRect(void);

    void setContent(ContainerView* content);
    inline ContainerView* content(void) const {return m_content;}
   
    virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);
    virtual void drawScroll(GraphicsContext* context, IntRect &rc, int status/* = Style::NORMAL*/);

    void getContentSize(int *pw, int *ph) {
        if(pw)
            *pw = m_content ? m_content->getRect().width() : 0;
        if(ph)
            *ph = m_content ? m_content->getRect().height() : 0;
    }
    
    bool dispatchEvent(Event*);
    bool handleEvent(Event* event);
    
    /*
     * set vertical scrollbar and horizontal scrollbar width.
     */
    void setBarWidth(int barwidth);
    void setBarMargin(int barmargin);

    /*
     * set vertical scrollbar and horizontal scrollbar is visible.
     */
    void setVBarVisible(bool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE); }
    void setHBarVisible(bool b_isVisible) { setFlag(b_isVisible, HBAR_VISIBLE); }
    void setBarVisible(bool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE | HBAR_VISIBLE); }
    
    bool vBarVisible(void) { return (m_flags & VBAR_VISIBLE) == VBAR_VISIBLE; }
    bool hBarVisible(void) { return (m_flags & HBAR_VISIBLE) == HBAR_VISIBLE; }

    void setLoopScroll(bool b) { setFlag(b, LOOP_SCROLL); }
    bool loopScroll() { return m_flags & LOOP_SCROLL; }
    bool autoShowBar() { return m_flags& AUTOSHOWBAR; }
    bool scrollVisiable(int * barWidth, bool isVertical = TRUE);
    
    void setAutoShowBar(bool b) { setFlag(isScrollBarAlwaysShow() ? true : b, AUTOSHOWBAR); }
    
    void setScrollBarAlwaysShow(bool b) { setFlag(b, ALWAYS_SHOW); }
    bool isScrollBarAlwaysShow(void) { return (m_flags & ALWAYS_SHOW) == ALWAYS_SHOW; }

    void setScrollBarFloating(bool b) { setFlag(b, BAR_FLOATING); }
    bool isScrollBarFloating(void) { return (m_flags & BAR_FLOATING) == BAR_FLOATING; }

    void setVScrollBarOnLeft(bool b) { setFlag(b, VBAR_ON_LEFT); }
    bool isVScrollBarOnLeft(void) { return (m_flags & VBAR_ON_LEFT) == VBAR_ON_LEFT; }
    
    static void SetAllScrollBarOnLeft(bool b){ g_is_all_scrollbar_on_left = b; }
    static bool IsAllScrollBarOnLeft(void){ return g_is_all_scrollbar_on_left; }
    void setVStep(Uint16 vstep){m_vStep = vstep;}
    void setHStep(Uint16 hstep){m_hStep = hstep;}
    
protected:
    enum {
        LOOP_SCROLL   = (0x01 << (PanelView::FLAG_SHIFT)), //scroll the content with loop
        HBAR_VISIBLE  = (0x01 << (PanelView::FLAG_SHIFT + 1)),
        VBAR_VISIBLE  = (0x01 << (PanelView::FLAG_SHIFT + 2)),
        AUTOSHOWBAR   = (0x01 << (PanelView::FLAG_SHIFT + 3)),
        ALWAYS_SHOW   = (0x01 << (PanelView::FLAG_SHIFT + 4)),
        BAR_FLOATING  = (0x01 << (PanelView::FLAG_SHIFT + 5)),
        VBAR_ON_LEFT  = (0x01 << (PanelView::FLAG_SHIFT + 6)),
        FLAG_SHIFT 	  = (PanelView::FLAG_SHIFT + 7)
    };
    
protected:
    void onChildSizeChanged(View *child) {
        if(child == m_content)
            updateContentRect();
    }

protected:
    Uint16 m_barWidth;
    Uint16 m_barMargin;
    Uint16 m_vStep;
    Uint16 m_hStep;
    int m_offx;
    int m_offy;
    ContainerView* m_content;

private:
    void moveOffsetH(int x);
    void moveOffsetV(int y);

    void drawScrollContent(GraphicsContext *gc, int status);

private:
    int m_timerId;
    static bool g_is_all_scrollbar_on_left;
    
    DECLARE_CLASS_NAME(ScrollView)
};

} // namespace hfcl

#endif /* HFC_VIEW_SCROLLVIEW_H_ */
