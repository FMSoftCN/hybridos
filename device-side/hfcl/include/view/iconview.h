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


#ifndef _HFCL_IconView_h
#define _HFCL_IconView_h

#include "scrollview.h"
#include "gridview.h"

namespace hfcl {

class Drawable;
class ScrollView;
class GridView;

class IconView : public ScrollView
{
    public:
        IconView(View *parent);
        IconView(View *parent, DrawableSet *drset);
        virtual ~IconView(void);

        inline int addView(View *view) {
            if (m_gridView) return m_gridView->addView(view);
            return 0;
        }
        inline int remove(View *view) {
            if (m_gridView) return m_gridView->remove(view);
            return 0;
        }
        inline int remove(int row, int col) {
            if (m_gridView) return m_gridView->remove(row, col);
            return 0;
        }
        inline void setRows (int row) {
            if (m_gridView) m_gridView->setRows(row);
        }
        inline void setCols (int col) {
            if (m_gridView) m_gridView->setCols(col);
        }
        inline int rows() {
            if (m_gridView) return m_gridView->rows();
            return 0;
        }
        inline int cols(){
            if (m_gridView) return m_gridView->cols();
            return 0;
        }
        inline int setFocusItem(int row, int col){
            if (m_gridView) return m_gridView->setFocusItem(row, col);
            return 0;
        }
        inline int setFocusItem(int index){
            if (m_gridView) return m_gridView->setFocusItem(index);
            return 0;
        }
        inline int focusItem(void) {
            if (m_gridView) return m_gridView->focusItemIdx();
            return 0;
        }
        inline int focusItem(int* row, int* col){
            if (m_gridView) return m_gridView->focusItemPosition(row, col);
            return 0;
        }
        inline void setGridsRect (int left, int top, int right, int bottom) {
            if (m_gridView) m_gridView->setRect(left, top, right, bottom);
        }
        inline void setAutoConvert(bool bCon) {
            if (m_gridView) m_gridView->setAutoConvert(bCon);
        }

        void makeHilightVisible();

        virtual bool handler(Event* event);

        bool dispatchEvent(Event*);

    protected:
        virtual void onGotFocus(){
            if (m_gridView) m_gridView->focusMe();
        }

    private:
        GridView *m_gridView;
};

} // namespace hfcl

#endif //_HFCL_IconView_h
