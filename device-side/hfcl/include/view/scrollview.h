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


#ifndef _NGUX_ScrollView_h
#define _NGUX_ScrollView_h

#include "panelview.h"
#include "timerservice.h"

NAMESPACE_BEGIN

class ScrollView : public PanelView, public TimerEventListener
{
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
		
    public:
        ScrollView(View *p_parent);
        ScrollView(View *p_parent, DrawableSet* drset);
        virtual ~ScrollView(void);

        /* \brief this function move the view port to the specific position.
         * * \param[in] x the destination X coordinate.
         * * \param[in] y the destination Y coordinate.
         * */
        void moveViewport(NGInt x, NGInt y, NGBool bupdate =true);

        void updateContentRect(void);

        void setContent(ContainerView* content);
        inline ContainerView* content(void) const {return m_content;}
       
		virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);
        virtual void drawScroll(GraphicsContext* context, IntRect &rc, NGInt status/* = Style::NORMAL*/);

		void getContentSize(NGInt *pw, NGInt *ph) {
			if(pw)
				*pw = m_content ? m_content->getRect().width() : 0;
			if(ph)
				*ph = m_content ? m_content->getRect().height() : 0;
		}
		
		NGBool dispatchEvent(Event*);
        NGBool handleEvent(Event* event);
    	
        /*
         * set vertical scrollbar and horizontal scrollbar width.
         */
        void setBarWidth(NGInt barwidth);
        void setBarMargin(NGInt barmargin);

        /*
         * set vertical scrollbar and horizontal scrollbar is visible.
         */
        void setVBarVisible(NGBool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE); }
        void setHBarVisible(NGBool b_isVisible) { setFlag(b_isVisible, HBAR_VISIBLE); }
        void setBarVisible(NGBool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE | HBAR_VISIBLE); }
		
		NGBool vBarVisible(void) { return (m_flags & VBAR_VISIBLE) == VBAR_VISIBLE; }
		NGBool hBarVisible(void) { return (m_flags & HBAR_VISIBLE) == HBAR_VISIBLE; }

		void setLoopScroll(NGBool b) { setFlag(b, LOOP_SCROLL); }
		NGBool loopScroll() { return m_flags & LOOP_SCROLL; }
		NGBool autoShowBar() { return m_flags& AUTOSHOWBAR; }
		NGBool scrollVisiable(NGInt * barWidth, NGBool isVertical = TRUE);
		
		void setAutoShowBar(NGBool b) { setFlag(isScrollBarAlwaysShow() ? true : b, AUTOSHOWBAR); }
		
		void setScrollBarAlwaysShow(NGBool b) { setFlag(b, ALWAYS_SHOW); }
		NGBool isScrollBarAlwaysShow(void) { return (m_flags & ALWAYS_SHOW) == ALWAYS_SHOW; }

		void setScrollBarFloating(NGBool b) { setFlag(b, BAR_FLOATING); }
		NGBool isScrollBarFloating(void) { return (m_flags & BAR_FLOATING) == BAR_FLOATING; }

		void setVScrollBarOnLeft(NGBool b) { setFlag(b, VBAR_ON_LEFT); }
		NGBool isVScrollBarOnLeft(void) { return (m_flags & VBAR_ON_LEFT) == VBAR_ON_LEFT; }
		
		static void SetAllScrollBarOnLeft(NGBool b){ g_is_all_scrollbar_on_left = b; }
		static NGBool IsAllScrollBarOnLeft(void){ return g_is_all_scrollbar_on_left; }
        void setVStep(NGUInt16 vstep){m_vStep = vstep;}
        void setHStep(NGUInt16 hstep){m_hStep = hstep;}
		
	private:
		void moveOffsetH(NGInt x);
		void moveOffsetV(NGInt y);
	
		void drawScrollContent(GraphicsContext *gc, NGInt status);

	protected:
		void onChildSizeChanged(View *child) {
			if(child == m_content)
				updateContentRect();
		}

    protected:
		NGUInt16 m_barWidth;
		NGUInt16 m_barMargin;
        NGUInt16 m_vStep;
        NGUInt16 m_hStep;
		NGInt m_offx;
		NGInt m_offy;
        ContainerView* m_content;

    private:
        NGInt m_timerId;
		static NGBool g_is_all_scrollbar_on_left;
		
	DECLARE_CLASS_NAME(ScrollView)
};

NAMESPACE_END

#endif /* _NGUX_ScrollView_h */
