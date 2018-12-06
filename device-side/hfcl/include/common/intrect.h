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

#ifndef __HFCL_INTRECT_H__
#define __HFCL_INTRECT_H__

#include "../common/common.h"

namespace hfcl {

class IntRect {
    public:
        IntRect ()
            : m_left (0)
            , m_top (0)
            , m_right (0)
            , m_bottom (0)
        {
        }

        IntRect (int left, int top = 0, int right = 0, int bottom = 0)
            : m_left (left)
            , m_top (top)
            , m_right (right)
            , m_bottom (bottom)
        {
        }

        IntRect (const IntRect& rc)
            : m_left (rc.m_left)
            , m_top (rc.m_top)
            , m_right (rc.m_right)
            , m_bottom (rc.m_bottom)
        {
        }

        IntRect (const RECT& rc)
            : m_left (rc.left)
            , m_top (rc.top)
            , m_right (rc.right)
            , m_bottom (rc.bottom)
        {
        }

        operator RECT() const 
        {
            RECT rect = {left(), top(), right(), bottom()};
            return rect;
        }

        int left (void)     const { return m_left;   }
        int top (void)      const { return m_top;    }
        int right (void)    const { return m_right;  }
        int bottom (void)   const { return m_bottom; }

        int width (void)    const { return m_right - m_left; }
        int height (void)   const { return m_bottom - m_top; }

        bool isEmpty (void) const
        {
            return m_left >= m_right || m_top >= m_bottom;
        }

        void setEmpty (void)
        {
            m_left = m_right = m_top = m_bottom = 0;
        }

        void setRect (int left, int top, int right, int bottom)
        {
            m_left   = left;
            m_top    = top;
            m_right  = right;
            m_bottom = bottom;
        }

        void setRect (const IntRect& irc)
        {
            m_left   = irc.m_left;
            m_top    = irc.m_top;
            m_right  = irc.m_right;
            m_bottom = irc.m_bottom;
        }

        void getRect (int* left, int* top, int* right, int* bottom)
        {
            if (left)
                *left   = m_left;
            if (top)
                *top    = m_top;
            if (right)
                *right  = m_right;
            if (bottom)
                *bottom = m_bottom;
        }

		void setWidth (int width) {
			m_right = m_left + width;
		}

		void setHeight (int height) {
			m_bottom = m_top + height;
		}

		void setSize (int width, int height) {
			m_right = m_left + width;
			m_bottom = m_top + height;
		}

        const IntRect& getRect (void) const
        {
            return *this;
        }

        IntRect& operator = (const IntRect& irc)
        {
            if (&irc == this)
                return *this;

            m_left   = irc.m_left;
            m_top    = irc.m_top;
            m_right  = irc.m_right;
            m_bottom = irc.m_bottom;

            return *this;
        }

        IntRect& operator = (const IntRect* irc)
        {
            if (irc == this)
                return *this;

            m_left   = irc->m_left;
            m_top    = irc->m_top;
            m_right  = irc->m_right;
            m_bottom = irc->m_bottom;

            return *this;
        }

        bool operator == (const IntRect& irc) const
        {
            if (&irc == this)
                return true;

            if (m_left   != irc.m_left  ||
                    m_top    != irc.m_top   ||
                    m_right  != irc.m_right ||
                    m_bottom != irc.m_bottom)
                return false;

            return true;
        }

        bool operator != (const IntRect& irc) const
        {
            if (&irc == this)
                return false;

            if (m_left   != irc.m_left  ||
                    m_top    != irc.m_top   ||
                    m_right  != irc.m_right ||
                    m_bottom != irc.m_bottom)
                return true;

            return false;
        }

        /**
         * Offset set the rectangle by adding x to its left and right,
         * and adding y to its top and bottom.
         */
        void offset (int x, int y)
        {
            m_left   += x;
            m_top    += y;
            m_right  += x;
            m_bottom += y;
        }

        /**
         * Inset the rectangle y (x,y). If dx is positive, then the sides are moved inwards,
         * making the rectangle narrower. If x is negative, then the sides are moved outwards,
         * making the rectangle wider. The same hods true for y and the top and bottom.
         */
        void inset (int x, int y)
        {
            m_left   += x;
            m_top    += y;
            m_right  -= x;
            m_bottom -= y;
        }

		void inset (int left, int top, int right, int bottom) {
			m_left += left;
			m_top  += top;
			m_right -= right;
			m_bottom -= bottom;
		}

        bool contains (int x, int y) const
        {
            return (unsigned)(x - m_left) <= (unsigned)(m_right - m_left) &&
                   (unsigned)(y - m_top) <= (unsigned)(m_bottom - m_top);
        }

        bool contains (int left, int top, int right, int bottom) const
        {
            return left < right && top < bottom && !isEmpty() &&
                   m_left <= left && m_top <= top &&
                   m_right >= right && m_bottom >= bottom;
        }

        bool contains (const IntRect& irc) const
        {
            return !irc.isEmpty() && !isEmpty() &&
                    m_left <= irc.m_left && m_top <= irc.m_top &&
                    m_right >= irc.m_right && m_bottom >= irc.m_bottom;
        }

        bool intersect (const IntRect& irc)
        {
            return intersect(irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        }

        bool intersect (const IntRect& a, const IntRect& b)
        {
            if (!a.isEmpty() && !b.isEmpty() &&
                    a.m_left < b.m_right && b.m_left < a.m_right &&
                    a.m_top < b.m_bottom && b.m_top < a.m_bottom) {

                m_left   = (a.m_left   > b.m_left  ) ? a.m_left   : b.m_left;
                m_top    = (a.m_top    > b.m_top   ) ? a.m_top    : b.m_top;
                m_right  = (a.m_right  < b.m_right ) ? a.m_right  : b.m_right;
                m_bottom = (a.m_bottom < b.m_bottom) ? a.m_bottom : b.m_bottom;
                return true;
            }
            return false;
        }
        
        bool intersect (int left, int top, int right, int bottom)
        {
            if (left < right && top < bottom && !this->isEmpty() &&
                    m_left < right && left < m_right && m_top < bottom && top < m_bottom) {

                if (m_left < left) m_left = left;
                if (m_top < top) m_top = top;
                if (m_right > right) m_right = right;
                if (m_bottom > bottom) m_bottom = bottom;
                return true;
            }
            return false;
        }
    
        /**
         * Update this rectangle to enclose itself and the specified rectangle.
         * If this rectangle is empty, just set it to the specified rectangle. If the specified
         * rectangle is empty, do nothing.
         */
        void join (int left, int top, int right, int bottom);

        /**
         * Update this rectangle to enclose itself and the specified rectangle.
         * If this rectangle is empty, just set it to the specified rectangle. If the specified
         * rectangle is empty, do nothing.
         */
        void join (const IntRect& irc)
        {
            join (irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        }

        /**
         * Swap top/bottom or left/right if there are flipped.
         * This can be called if the edges are computed separately,
         * and may have crossed over each other.
         * When this returns, left <= right && top <= bottom
         */
        void sort();

		bool isIn (int x, int y) const
        {
			return (x >= m_left && x < m_right
					&& y >= m_top && y < m_bottom);
		}
    
    public:
        int m_left;
        int m_top;
        int m_right;
        int m_bottom;
};

} // namespace hfcl

#endif /* __HFCL_INTRECT_H__ */

