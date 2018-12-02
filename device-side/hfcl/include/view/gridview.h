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
		GridView(View* parent, int id, int x, int y, int w, int h, 
                int row, int col, int gap_w, int gap_h);
        ~GridView();
        
        int addView(View *view);
        int remove(View *view);
        int remove(int row, int col);
        
        void setRows (int row) ;
        void setCols (int col) ;
        int rows(){ return m_row; }
        int cols(){ return m_col; }

        virtual void setGapWidth (int gap_w);
        virtual void setGapHeight (int gap_h); 
		int gapWidth(){ return m_gap_w; }
        int gapHeight(){ return m_gap_h; }
        
        int setFocusItem(int row, int col);
        int setFocusItem(int index);
        int focusItemIdx(void) { return m_focusItem; }
        int focusItemPosition(int* row, int* col);
		int oldFocusItemIdx(void) { return m_oldFocusItem; }
		View * getOldFocusItem();

		void onGetFocus();
		void onLoseFocus();

        virtual bool setRect(const IntRect& irc);
        bool setRect(int left, int top, int right, int bottom){
			return setRect(IntRect(left, top, right, bottom));
		}

        virtual bool dispatchEvent(Event* event);

		bool setOnBoundaryListener(EventListener* listener){
			m_boundary_listener = listener; 
			return true;
		}

		void setAutoConvert(bool bCon) { m_bNeedConvert = bCon; }

        // bool handleEvent(Event* event);        // GT_jyseo rollbakc

    protected:
        int gridWidth(){ return m_grid_w; }
        int gridHeight(){ return m_grid_h; }
        
		virtual void changeFocusIndex(KEY_DIRECTION direction);
        
		void reLayout(void);
        
		bool notify2Parent(KEY_DIRECTION direction);
        
        bool onKeyPressed(int keyCode);

    protected:
		enum { GRIDVIEW_FOCUS_INVALID = -1};

        Uint16 m_col;
        Uint16 m_row;
        Uint16 m_grid_w;
        Uint16 m_grid_h;
        Uint16 m_gap_w;
        Uint16 m_gap_h;

        EventListener* m_boundary_listener;
        
		int m_focusItem;
		int m_oldFocusItem;

		bool m_bNeedConvert;

		DECLARE_CLASS_NAME(GridView)

        friend class TableBarView;
};

NAMESPACE_END

#endif
