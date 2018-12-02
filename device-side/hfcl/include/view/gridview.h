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

#ifndef  _GRIDVIEW_H 
#define  _GRIDVIEW_H

#include "panelview.h"

NAMESPACE_BEGIN

#define _GRIDVIEW_TEST 1


class PairLayoutView;

class GridView : public PanelView 
{
    public:
        typedef enum 
		{              
            GRIDVIEW_KEY_LEFT,      
            GRIDVIEW_KEY_RIGHT,     
            GRIDVIEW_KEY_UP,        
            GRIDVIEW_KEY_DOWN,
        } KEY_DIRECTION;
        
        GridView(View* parent);
		GridView(View* parent, DrawableSet* drset);
		GridView(View* parent, NGInt id, NGInt x, NGInt y, NGInt w, NGInt h, 
                NGInt row, NGInt col, NGInt gap_w, NGInt gap_h);
        ~GridView();
        
        NGInt addView(View *view);
        NGInt remove(View *view);
        NGInt remove(NGInt row, NGInt col);
        
        void setRows (NGInt row) ;
        void setCols (NGInt col) ;
        NGInt rows(){ return m_row; }
        NGInt cols(){ return m_col; }

        virtual void setGapWidth (NGInt gap_w);
        virtual void setGapHeight (NGInt gap_h); 
		NGInt gapWidth(){ return m_gap_w; }
        NGInt gapHeight(){ return m_gap_h; }
        
        NGInt setFocusItem(NGInt row, NGInt col);
        NGInt setFocusItem(NGInt index);
        NGInt focusItemIdx(void) { return m_focusItem; }
        NGInt focusItemPosition(NGInt* row, NGInt* col);
		NGInt oldFocusItemIdx(void) { return m_oldFocusItem; }
		View * getOldFocusItem();

		void onGetFocus();
		void onLoseFocus();

        virtual NGBool setRect(const IntRect& irc);
        NGBool setRect(NGInt left, NGInt top, NGInt right, NGInt bottom){
			return setRect(IntRect(left, top, right, bottom));
		}

        virtual NGBool dispatchEvent(Event* event);

		NGBool setOnBoundaryListener(EventListener* listener){
			m_boundary_listener = listener; 
			return true;
		}

		void setAutoConvert(NGBool bCon) { m_bNeedConvert = bCon; }

        // NGBool handleEvent(Event* event);        // GT_jyseo rollbakc

    protected:
        NGInt gridWidth(){ return m_grid_w; }
        NGInt gridHeight(){ return m_grid_h; }
        
		virtual void changeFocusIndex(KEY_DIRECTION direction);
        
		void reLayout(void);
        
		NGBool notify2Parent(KEY_DIRECTION direction);
        
        NGBool onKeyPressed(NGInt keyCode);

    protected:
		enum { GRIDVIEW_FOCUS_INVALID = -1};

        NGUInt16 m_col;
        NGUInt16 m_row;
        NGUInt16 m_grid_w;
        NGUInt16 m_grid_h;
        NGUInt16 m_gap_w;
        NGUInt16 m_gap_h;

        EventListener* m_boundary_listener;
        
		NGInt m_focusItem;
		NGInt m_oldFocusItem;

		NGBool m_bNeedConvert;

		DECLARE_CLASS_NAME(GridView)

        friend class TableBarView;
};

NAMESPACE_END

#endif
