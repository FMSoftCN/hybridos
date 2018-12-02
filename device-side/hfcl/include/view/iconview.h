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


#ifndef _NGUX_IconView_h
#define _NGUX_IconView_h

#include "scrollview.h"
#include "gridview.h"

NAMESPACE_BEGIN

class Drawable;
class ScrollView;
class GridView;

class IconView : public ScrollView
{
    public:
        IconView(View *parent);
        IconView(View *parent, DrawableSet *drset);
        virtual ~IconView(void);

        inline NGInt addView(View *view) {
            if (m_gridView) return m_gridView->addView(view);
            return 0;
        }
        inline NGInt remove(View *view) {
            if (m_gridView) return m_gridView->remove(view);
            return 0;
        }
        inline NGInt remove(NGInt row, NGInt col) {
            if (m_gridView) return m_gridView->remove(row, col);
            return 0;
        }
        inline void setRows (NGInt row) {
            if (m_gridView) m_gridView->setRows(row);
        }
        inline void setCols (NGInt col) {
            if (m_gridView) m_gridView->setCols(col);
        }
        inline NGInt rows() {
            if (m_gridView) return m_gridView->rows();
            return 0;
        }
        inline NGInt cols(){
            if (m_gridView) return m_gridView->cols();
            return 0;
        }
        inline NGInt setFocusItem(NGInt row, NGInt col){
            if (m_gridView) return m_gridView->setFocusItem(row, col);
            return 0;
        }
        inline NGInt setFocusItem(NGInt index){
            if (m_gridView) return m_gridView->setFocusItem(index);
            return 0;
        }
        inline NGInt focusItem(void) { 
            if (m_gridView) return m_gridView->focusItemIdx();
            return 0;
        }
        inline NGInt focusItem(NGInt* row, NGInt* col){
            if (m_gridView) return m_gridView->focusItemPosition(row, col);
            return 0;
        }
        inline void setGridsRect (NGInt left, NGInt top, NGInt right, NGInt bottom) {
            if (m_gridView) m_gridView->setRect(left, top, right, bottom);
        }
		inline void setAutoConvert(NGBool bCon) { 
			if (m_gridView) m_gridView->setAutoConvert(bCon); 
		}
		
        void makeHilightVisible();

        virtual NGBool handleEvent(Event* event);

        NGBool dispatchEvent(Event*);

    protected:
        virtual void onGetFocus(){
            if (m_gridView) m_gridView->focusMe();
        }
        
    private:
        GridView *m_gridView;
};

NAMESPACE_END

#endif //_NGUX_IconView_h
