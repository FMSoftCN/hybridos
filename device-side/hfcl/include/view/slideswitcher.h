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


#ifndef  _SLIDESWITCHER_H 
#define  _SLIDESWITCHER_H

#include "panelview.h"

NAMESPACE_BEGIN

class SlideSwitcher : public PanelView  
{
    public:
        typedef enum __enumSlideStyle
        {
            Vertical = 0,
            Horizontal
        }SlideStyle;
        
        /**
         * Constructor/Destructor
         */
        SlideSwitcher(View* parent);
		SlideSwitcher(View* parent, DrawableSet* drset);
		SlideSwitcher(View* parent, int id, int x, int y, int w, int h, SlideStyle style, int gap_w);
        ~SlideSwitcher();
        /**
         * Add/Remove A Item To/From SlideSwitcher
         */
        bool addItem(View *item, bool toRefresh = true);
        bool removeItem(View *item);
        /**
         * Set/Get ItemWidth ItemHeight GapWidth functions
         */
        void setItemWidth(int w) { m_item_width = w; }
        int itemWidth(void) { return m_item_width; }
        void setItemHeight(int h) { m_item_height = h; }
        int itemHeight(void) { return m_item_height; }
        void setGapWidth(int w) { m_gap_w = w; }
		int gapWidth(){ return m_gap_w; }
        /**
         * Set/Get Style
         */
        void setSlideStyle(SlideStyle style){ m_style = style; }
        SlideStyle slideStyle(void){ return m_style; }
        /**
         * Dispatch Events From Parent
         */
        virtual bool dispatchEvent(Event* event);
        /**
         * Relayout The SlideSwitcher
         */
		void reLayout(void);
        /**
         * @brief Calculate Item's Rect according to Item's offset index to Center(Focus) Item
         *
         * @param[in] offindex Offset index to Center(Focus) Item
         * @parem[in] forward Forward or Backward
         * @param[out] rect Result Rect Calculated
         */
        void calcuItemRect(int offindex, IntRect& rect, bool forward);
        /**
         * @brief Get Item According That Offset By Center (Focus) Item
         *
         * @param[in] offindex Item Index Offset To Center (Focus) Item
         * @param[in] forward Forward/Backward
         */
        View* getItem(int offindex, bool forward);
        /**
         * SetFocusItem
         *
         * @param[in] item Item To Focus
         */
        void setFocusItem(View* item);
        /**
         * GetFocusItem
         */
        View* focusItem(void) { return m_curr_focus_view; };

        //bool handleEvent(Event* event);     // GT_jyseo rollback 

    protected:
        /**
         * focusXXXItem
         */
        void focusPrevItem();
        void focusNextItem();
     
        bool onKeyPressed(int keyCode);
    private:
        /**
         * Keep GapWidth
         */
        Uint16 m_gap_w;
        /**
         * Keep Style
         */
        SlideStyle m_style;
        /**
         * Keep ItemWidth/ItemHeight
         */
        int m_item_width;
        int m_item_height;
        /**
         * Current Focus View
         */
        View* m_curr_focus_view;
	
        DECLARE_CLASS_NAME(SlideSwitcher)
};

NAMESPACE_END

#endif
