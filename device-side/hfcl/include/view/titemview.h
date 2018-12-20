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


/*
 * This file this the template of ItemViewName
 * Because some plateform's c++ compiler don't support template
 * we use the macro to implement it
 *
 * it same as template<class Base> class TItemView : public Base { .... }
 *
 * use macro, you create a new file, such as my_item.h, and
 *
 * #ifndef _HFCL_MyItemView_h
 * #define _HFCL_MyItemView_h
 *
 * #define ItemViewName MyItemView
 * #define ItemBaseName View
 * #include "titemview.h"
 * #undef ItemViewName
 * #undef ItemBaseName
 *
 * #endif
 *
 */

namespace hfcl {

class ItemViewName : public ItemBaseName
{
public:
    ItemViewName(View* parent)
        : ItemBaseName(parent, GetDrawable(DR_ITEMVIEW)) { }

    ItemViewName(View* parent, Drawable* dr)
        : ItemBaseName(parent, dr) { }
    ItemViewName() : ItemBaseName(NULL, GetDrawable(DR_ITEMVIEW)) { }

    void draw_hilite(GraphicsContext* context, IntRect &rc, int status);
    virtual void drawContent(GraphicsContext* context, IntRect &rc, int status);
    void setItemHeight(int h);
    int getItemHeight() { return getRect().height(); }
    bool isSelected()   { return m_flags & SELECTED; }
    void setSelected(bool select) {
        if(select != isSelected()) {
            setFlag(select, SELECTED);
            updateView();
        }
    }
    void setFocus(bool focus) {
        if(focus != isFocused())
        {
            setFlag(focus, FOCUSED);
            updateView();
        }
    }
    bool isFocused() { return m_flags & FOCUSED; }

    void setPaintHilightFirst(bool b) { setFlag(b, HILIGHT_TOP); }
    bool isPaintHilightFirst()        { return m_flags & HILIGHT_TOP; }

protected:
    enum {
        FOCUSED = (1 << (ItemBaseName::FLAG_SHIFT)), //FIXED ME, Should defined in view
        SELECTED = (2 << (ItemBaseName::FLAG_SHIFT)),
        HILIGHT_TOP = (4 << (ItemBaseName::FLAG_SHIFT)),
        FLAG_SHIFT = (3 + (ItemBaseName::FLAG_SHIFT))
    };

}; //end of ItemViewName

} // namespace hfcl

