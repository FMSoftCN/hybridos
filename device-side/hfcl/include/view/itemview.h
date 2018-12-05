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


#ifndef HFCL_VIEW_ITEMVIEW_H_
#define HFCL_VIEW_ITEMVIEW_H_

#include "view/panelview.h"

namespace hfcl {

class ItemView : public PanelView
{
public:
    ItemView(View* p_parent) 
        : PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(ItemView)) { 
            m_select_mark_rect.setRect(0, 0, 20, 20);
            setFocusValid(true); 
            convertLayout(false);
            m_margenWidth = 0;
        }

    ItemView(View* p_parent, DrawableSet* drset)
        : PanelView(p_parent, drset) { 
            m_select_mark_rect.setRect(0, 0, 20, 20);
            setFocusValid(true); 
            convertLayout(false);
            m_margenWidth = 0;
        }
    ItemView() : PanelView(NULL, DEFAULT_VIEW_DRAWABLESET(ItemView)) { 
        m_select_mark_rect.setRect(0, 0, 20, 20);
        setFocusValid(true); 
        convertLayout(false);
        m_margenWidth = 0;
    }

    virtual ~ItemView(){}

    virtual void drawContent(GraphicsContext *context, IntRect &rc, int status/* = Style::NORMAL*/);

    void draw_hilite(GraphicsContext* context, IntRect &rc, int status);
    /**
     * @brief Draw Select Mark
     *
     * @param context GraphicsContext
     * @param rc IntRect to draw
     * @param status status, we do NOT use this
     */
    void draw_select(GraphicsContext* context, IntRect &rc, int status);
    /**
     * @brief Set Select Mark Rect
     *
     *  Rect is the rect in ItemView
     *
     * @param rect Mark rect in ItemView
     */
    void setSelectMarkRect(int left, int top, int right, int bottom) { 
        m_select_mark_rect.setRect(left, top, right, bottom); 
    }
    void setSelectMarkRect(const IntRect& rect) { m_select_mark_rect = rect; }
    IntRect& selectMarkRect() { return m_select_mark_rect; }

    void paint(GraphicsContext* context, int status);
    
    void setItemHeight(int h);
    int getItemHeight() { return getRect().height(); }
    
    bool isSelected()   { return m_flags & SELECTED; }
    virtual  void setSelected(bool select) { 
        if(select != isSelected()) { 
            setFlag(select, SELECTED); 
            updateView();
        }
    }

    void setPaintHilightFirst(bool b) { setFlag(b, HILIGHT_TOP); }
    bool isPaintHilightFirst(void)    { return m_flags & HILIGHT_TOP; }
    
    static void SetAllItemViewNeedConvert(bool b){ g_is_all_itemview_need_convert = b; }
    static bool IsAllItemViewNeedConvert(void){ return g_is_all_itemview_need_convert; }
    void setMarginWidth(int margin){m_margenWidth = margin;}
    int marginWidth(){return m_margenWidth;}

protected:
    enum {
        SELECTED 		= (1 << (PanelView::FLAG_SHIFT)),
        HILIGHT_TOP 	= (1 << (PanelView::FLAG_SHIFT + 1)),	        
        LAYOUT_CONVERT 	= (1 << (PanelView::FLAG_SHIFT + 2)),
        BE_CONVERTED	= (1 << (PanelView::FLAG_SHIFT + 3)),
        FLAG_SHIFT 		= (4 + (PanelView::FLAG_SHIFT))
    };

    int getHilightDrawState();

private:
    int m_margenWidth;
    IntRect m_select_mark_rect;
    static bool g_is_all_itemview_need_convert;

    void convertLayout(bool b) 	{ setFlag(b, BE_CONVERTED); }
    bool isConverted(void)  		{ return m_flags & BE_CONVERTED; }

    DECLARE_CLASS_NAME(ItemView)
};  // ItemView

}   // namespace hfcl

#endif // HFCL_VIEW_ITEMVIEW_H_

