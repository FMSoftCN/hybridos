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


#ifndef __NGUX_LAYOUTVIEW_H__
#define __NGUX_LAYOUTVIEW_H__

#include "containerview.h"

NAMESPACE_BEGIN

class LayoutView : public ContainerView {
    public:
        /**
         * herizontal and vertical layout type flags
         * layout size allot type flags
         */
        enum __tag_LVDirectionStyleSizeAllotType {
            LVDST_HORZ    = 0x00000000,  /* the direction of view is herizontal */
            LVDST_VERT    = 0x01000000,  /* the direction of view is vertical */

            LVSAT_FIXED   = 0x01 << 0,   /* the size of view is fixed */
            LVSAT_PERCENT = 0x01 << 1,   /* the size of view is percent */
            LVSAT_AUTO    = 0x01 << 2,   /* the size of view is auto */
        };

        LayoutView(View* parent, DrawableSet* drset);

        LayoutView();

        LayoutView(int margin, int space = 0, int direction = LVDST_HORZ | LVSAT_AUTO);

        virtual ~LayoutView();

        void paint(GraphicsContext* context, int status/* = Style::NORMAL*/) { }
        bool dispatchEvent(Event* event) { return DISPATCH_CONTINUE_MSG; }

        void setHorzVertDirection(int direction = LVDST_HORZ)
        {
            m_direction &= 0x00ffffff;
            m_direction |= (0xff000000 & direction);
        }

        int  getHorzVertDirection(void) const { return m_direction & 0xff000000; }
        bool isVertDirection(void) const { return m_direction & LVDST_VERT; }

        void setAutoWrapType(int aAutoWrap = LVSAT_AUTO)
        {
            m_direction &= 0xffffff00;
            m_direction |= (0x000000ff & aAutoWrap);
        }

        int  getAutoWrapType(void)   const { return m_direction & 0x000000ff; }
        bool isFixedWrapType(void)   const { return m_direction & LVSAT_FIXED; }
        bool isPercentWrapType(void) const { return m_direction & LVSAT_PERCENT; }
        bool isAutoWrapType(void)    const { return m_direction & LVSAT_AUTO; }

        virtual void setMargin(int margin = 0) { m_margin = margin; }
        int getMargin(void) const { return m_margin; }

        virtual void setSpace(int space = 0) { m_space = space; }
        int getSpace(void) const { return m_space; }

    protected:
        LayoutView(View *parent);
        //virtual void initDrawable(DrawableSelector* selector) { }

        int          m_margin;
        int          m_space;
        unsigned int m_direction;
};

NAMESPACE_END

#endif  /* __NGUX_LAYOUTVIEW_H__ */

