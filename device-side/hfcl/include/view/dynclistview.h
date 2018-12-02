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


#ifndef  _DYNCLISTVIEW_H 
#define  _DYNCLISTVIEW_H

NAMESPACE_BEGIN

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
class DyncListView : public PanelView
{
    public:
        ///////////// Constructor/Destructor
        DyncListView(View* parent);
		DyncListView(View* parent, DrawableSet* drset);
        virtual ~DyncListView();

        ///////////// item ops
		NGBool addItem(void* addData);
		NGBool addItem(void);
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
        //setting the start and end range which can set hilight;
  
        void hilight(NGInt index);
        NGInt hilightItemIndex(void);
        
        NGBool isFocus(void){return m_focus;}
        
		View* hilightItem(void);
		View* itemFromIndex(NGInt index);
		
		NGBool prepareVisualitems(void);
        ////////////// dispatch event
        virtual NGBool dispatchEvent(Event* event);
        //virtual NGBool setRect(const IntRect& irc);
		void reLayout(void);
		void itemLayout(NGInt hilight_index);
		
		 NGBool setRect(NGInt left, NGInt top, NGInt right, NGInt bottom) {
			return setRect(IntRect(left, top, right, bottom));
		}
		
		virtual NGBool setRect(const IntRect& irc);

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
		void  updateListView(void);
		void setItemRect(View * item,NGInt left, NGInt top, NGInt right, NGInt bottom);
		
		void setScrollBarFloating(NGBool b) { m_bScrollFloating = b; }
		NGBool isScrollBarFloating(void) { return m_bScrollFloating; }
		void setSpecialItemIndex(NGInt index){m_special_item_index = index;}
    protected:
        void init(void);
        void reInit(void);

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
        View * m_firstitem;
        View * m_lastitem;
		NGBool  m_focus;
		NGBool  m_bScrollFloating;
        DECLARE_CLASS_NAME(DyncListView)
};

NAMESPACE_END

#endif
