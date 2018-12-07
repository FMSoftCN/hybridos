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


#ifndef __HFCL_PAIRLAYOUTVIEW_H__
#define __HFCL_PAIRLAYOUTVIEW_H__

#include "layoutview.h"

namespace hfcl {

class PairLayoutView : public LayoutView {
    public:
        enum __tag_MaskStyleType {
            PLV_NULL_MASK      = 0x00000000,
            PLV_REVERSED_MASK  = 0x10000000,
        };

        PairLayoutView()
            : m_firstView(0)
            , m_secondView(0)
            , m_maskStyle(PLV_NULL_MASK)
            , m_firstSize(0)
        {

        }

        PairLayoutView(View *p_parent)
            : LayoutView(p_parent)
            , m_firstView(0)
            , m_secondView(0)
            , m_maskStyle(PLV_NULL_MASK)
            , m_firstSize(0)
        {

        }

        PairLayoutView(View* aFirV, View* aSecV,
                int aStyle = PLV_NULL_MASK, int aSize = 0);

        ~PairLayoutView();

        void paint(GraphicsContext* context, int status/* = Style::NORMAL*/);
        bool dispatchEvent(Event* event);
        bool setRect(const IntRect& pirc);
        // to void drived class overide the base class 's function
        bool setRect(int left, int top, int right, int bottom) { return View::setRect(left, top, right, bottom); }

        // IntRect& getCalcRect(void);
        void switchFirstSecond(void);

        void setReversed(void) { m_maskStyle |= PLV_REVERSED_MASK; }
        void cancelReversed(void) { m_maskStyle &= ~PLV_REVERSED_MASK; }
        bool isReversed(void) { return m_maskStyle & PLV_REVERSED_MASK; }

        void setFirstViewSize(int aSize = 0) { m_firstSize = aSize; }
        int  getFirstViewSize(void) const { return m_firstSize; }

        void  setFirstView(View* aView = NULL) { m_firstView = aView; }
        View* getFirstView(void) const { return m_firstView; }
        void  setSecondView(View* aView = NULL) { m_secondView = aView; }
        View* getSecondView(void) const { return m_secondView; }
        void  removeAll(void);

    private:
        void calcVertFirstSecondSize(const IntRect& pirc, IntRect& fIrc, IntRect& sIrc, bool bReversed = false);
        void calcHorzFirstSecondSize(const IntRect& pirc, IntRect& fIrc, IntRect& sIrc, bool bReversed = false);

        View* m_firstView;
        View* m_secondView;
        int   m_maskStyle;
        int   m_firstSize;
};

} // namespace hfcl

#endif  /* __HFCL_PAIRLAYOUTVIEW_H__ */

