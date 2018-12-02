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


#ifndef  _BUSYLISTVIEW_H 
#define  _BUSYLISTVIEW_H

NAMESPACE_BEGIN

////////////////////// Features /////////////////////////
class BitMark;
class PointerKeeper;
class PanelView;

////////////////////// Features /////////////////////////
// 1. equal item height.
// 2. special hilight item height.
// 3. same item view template.
// 4. scrollbar support.
// 5. panelview's features.
//
class BusyListView : public PanelView
{
    public:
        ///////////// Constructor/Destructor
        BusyListView(View* parent);
		BusyListView(View* parent, DrawableSet* drset);
        virtual ~BusyListView();

        ///////////// item ops
        NGBool addItem(void);
		NGBool addItem(void* addData);
        NGBool addItemByIndex(NGInt index);
		NGBool addItemByIndex(NGInt index, void* addData);
        NGBool removeItem(NGInt index);
        NGInt itemCount(void);
        ///////////// set/get normal item height
        NGBool setItemHeight(NGInt height);
        NGInt itemHeight(void);
        ////////////// set/get hilight item height
        NGBool setHilightItemHeight(NGInt height);
        NGInt hilightItemHeight(void);
        ////////////// ui item
        void setItemTemplateId(NGInt item_res_id);
        ////////////// hilight item
        void hilight(NGInt index);
        void hilight(NGInt index, NGBool focus);
        NGInt hilightItemIndex(void);
        NGBool isFocus(void){return m_focus;}
		View* hilightItem(void);
		View* itemFromIndex(NGInt index);
        ////////////// dispatch event
        virtual NGBool dispatchEvent(Event* event);
        //virtual NGBool setRect(const IntRect& irc);
        void reLayout(NGInt index);
		void reLayout(void);

		void setScrollGap(NGInt gapWidth){m_scrollGap = gapWidth;}
		NGInt scrollGap(void){return m_scrollGap;}
		
		void setItemRowGap(NGInt gapHeight){m_itemRowGap = gapHeight;}
		NGInt itemRowGap(void){return m_itemRowGap;}
		
        ////////////// scrollbar related
        void setScrollbarWidth(NGInt w);
        NGInt scrollbarWidth(void);
        void setScrollbarMarginRight(NGInt m);
        NGInt scrollbarMarginRight(void);
        ///////////// re-load data
        NGInt reLoadData(void);
		///////////// clear
		void clear(void);
		///////////// freeze
		void freeze(NGBool f);
		///////////// status & addData
		void initSelectMark(NGInt count);
		void selectItem(NGInt index, NGBool sel);
		void select(NGInt index, NGBool sel);
        void selectAll(NGBool sel);
		NGBool isItemSelected(NGInt index);
		void initItemAddData(NGInt count);
		void setItemAddData(NGInt index, void* data);
		void* itemAddData(NGInt index);

		void setScrollBarFloating(NGBool b) { m_bScrollFloating = b; }
		NGBool isScrollBarFloating(void) { return m_bScrollFloating; }
		void setSpecialItemIndex(NGInt index){m_special_item_index = index;}
    protected:
        void init(void);
        void reInit(void);

        virtual void prepareVisibleItems(NGBool add_one = true, NGBool clean = false);

        ////////////// key process
        NGBool onKeyPressed(NGInt keyCode);
        
        void   setHilightView(View* view);

        ////////////// draw ops
		virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status);
		virtual void drawScroll(GraphicsContext* context, IntRect &rc, NGInt status);

        ////////////// layout
        virtual void layoutDownToUp(NGInt from_height = -1, NGInt hilight_index = 1);
        virtual void layoutUpToDown(NGInt from_height = 0, NGInt hilight_index = 1);

        ////////////// load data notify
        virtual void onLoadItemData(NGInt index, View* view);

        ////////////// item ops
        virtual View* firstVisibleItem(void);
        virtual View* lastVisibleItem(void);
        virtual NGInt getVisibleItemCountNeed(void);

    private:

    private:
        /// item count
        NGInt m_item_count;
        /// normal item height
        NGInt m_nm_height;
        /// hilight item height
        NGInt m_hi_height;
        /// special item count
        NGInt m_special_item_index;
        /// visible item count
        NGInt m_visible_item_count;
        /// item view template id
        NGUInt m_item_templete_id;
        /// up index
        NGInt m_up_index;
        /// down index
        NGInt m_down_index;
        /// hi index
        NGInt m_hi_index;
        /// items ready ?
        NGBool m_items_enough;
        /// scrollbar margin right
        NGInt m_v_scrollbar_margin_right;
        /// scrollbar width
        NGInt m_v_scrollbar_width;
        /// top dock - true, bottom dock - false
        NGInt m_dock_top;
		/// freeze mark
		NGBool m_freeze;
		/// select mark
		BitMark m_select_mark;
		/// add data keeper
		PointerKeeper m_data_keeper;
		// the gap from item to scroll.
		NGInt m_scrollGap;
		NGInt m_itemRowGap;

		NGBool  m_focus;
		NGBool  m_bScrollFloating;

        DECLARE_CLASS_NAME(BusyListView)
};

NAMESPACE_END

#endif
