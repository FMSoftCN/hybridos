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


#ifndef __HFCL_BOXLAYOUTVIEW_H__
#define __HFCL_BOXLAYOUTVIEW_H__

#include "layoutview.h"

namespace hfcl {

typedef struct __tag_BoxLayoutNode {
    View* s_view;
    unsigned short s_size;
    unsigned short s_type;
} BoxLayoutNode, *PBoxLayoutNode;

class BoxLayoutView : public LayoutView {
    public:
        BoxLayoutView();

        BoxLayoutView(View* parent, DrawableSet* drset);

        BoxLayoutView(int aCount);

        ~BoxLayoutView();

        void paint(GraphicsContext* context, int status /*= Style::NORMAL*/);
        bool dispatchEvent(Event* event);
        bool setRect(const IntRect& pirc);
		// to void drived class overide the base class 's function
        bool setRect(int left, int top, int right, int bottom) { return View::setRect(left, top, right, bottom); }

        int   setCellInfo(int idx = 0, int size = 0, int type = 0, bool bupdate = false);
        int   getCellAutoSize(int idx = -1);
        View* setCell(int idx = -1, View* aView = NULL, int size = 0, int type = 0, bool bupdate = false);
        View* setCell(int idx = -1, View* aView = NULL);
        View* getCell(int idx = -1);
        void  removeAllCell(void);

        void resizeBoxLayout(int count = 0);
        void recalBox(void);

        void setMargin(int margin = 0);
        void setSpace(int space = 0);

        void setCount(int aCount = 0)
        {
            resizeBoxLayout(aCount);
        }
        int getCount(void) const { return m_count; }

    protected:
        BoxLayoutView(View *parent);
        bool calcSize(int totalSize = 0, int* aSizes = NULL);

    private:
        void deleteCells(void);
        void recalHorzBox(void);
        void recalVertBox(void);

        unsigned short m_count;
        BoxLayoutNode* m_cells;
};

} // namespace hfcl


#endif  /* __HFCL_BOXLAYOUTVIEW_H__ */

