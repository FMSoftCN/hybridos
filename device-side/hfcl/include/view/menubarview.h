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


#ifndef _NGUX_MenubarView_h
#define _NGUX_MenubarView_h

#include "panelview.h"
#include "graphicscontext.h"
#include "image.h"

NAMESPACE_BEGIN

class MenuBarView : public PanelView
{
public:
    MenuBarView(View* parent);
    MenuBarView(View* parent, DrawableSet* dr); 
    virtual ~MenuBarView();

    void setMargin(NGInt margin) { m_margin = margin; }
    void setImageBorderLen(NGInt len) { m_border_len = len; }

    void setLeftText(NGInt strid);
    void setMiddleText(NGInt strid);
    void setRightText(NGInt strid);

    void setLeftText(const char* txt);
    void setMiddleText(const char* txt);
    void setRightText(const char* txt);

    const string& leftText()  { 
        if (m_left_strid > 0) 
            m_left = GetTextRes(m_left_strid);
        return m_left; 
    }
    const string& middleText()  { 
        if (m_middle_strid > 0) 
            m_middle = GetTextRes(m_middle_strid);
        return m_middle; 
    }
    const string& rightText()  { 
        if (m_right_strid > 0) 
            m_right = GetTextRes(m_right_strid);
        return m_right; 
    }
    
    void setLeftImage(Image* image);
    void setMiddleImage(Image* image);
    void setRightImage(Image *image);
    NGBool IsLeftTextNull(void)  { 
        if((m_left_strid <= 0) && (0 == m_left.length()))
        {
            return true;
        }
        return false;
    }

    virtual NGBool dispatchEvent(Event* event);

private:
    string m_left;
    string m_middle;
    string m_right;

    NGInt m_left_strid;
    NGInt m_middle_strid;
    NGInt m_right_strid;

    Image* m_left_image;
    Image* m_middle_image;
    Image* m_right_image;

    NGUInt16 m_margin;
    NGUInt16 m_border_len;
    NGUInt16 m_selItem; 
    
    enum {
        SEL_NONE = 0,
        SEL_LEFT,
        SEL_MIDDLE,
        SEL_RIGHT
    };

    IntRect& calcRect (IntRect& rc, NGInt item, NGBool bWhole = false)
    {
        if (bWhole) {
            rc.m_top    = 0;
            rc.m_bottom = m_rect.height();
            rc.m_left   = 0;
            rc.m_right  = m_rect.width();
        } 
        else {
            NGInt middle_width = m_rect.width() / 3;
            NGInt spacing = (m_rect.width() % 3) / 2;

            rc.m_top    = 0;
            rc.m_bottom = m_rect.height();

            switch (item) {
            case SEL_NONE:
                rc.m_left   = 0;
                rc.m_right  = 0;
                break;

            case SEL_LEFT:
                rc.m_left   = 0;
                rc.m_right  = middle_width + spacing;
                break;

            case SEL_MIDDLE:
                rc.m_left   = middle_width + spacing;
                rc.m_right  = rc.m_left + middle_width;
                break;

            case SEL_RIGHT:
                rc.m_left   = middle_width * 2 + spacing;
                rc.m_right  = rc.m_left + middle_width + spacing;
                break;

            default:
                rc.m_left   = 0;
                rc.m_right  = m_rect.width();
                break;
            }
        }
        return rc;
    }


protected:
    virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);

    void drawText(GraphicsContext* context, const string &str, NGInt item);
    void drawText(GraphicsContext* context, NGInt strid, NGInt item);
    void drawImage(GraphicsContext* context, NGInt item);

    void pressItem(NGInt item, NGBool bpress);

    DECLARE_CLASS_NAME(MenuBarView)
};

NAMESPACE_END

#endif  // _NGUX_MenubarView_h

