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


#include "pairlayoutview.h"

namespace hfcl {

PairLayoutView::PairLayoutView(View* aFirV, View* aSecV, int aStyle, int aSize)
    : m_firstView(aFirV)
    , m_secondView(aSecV)
    , m_maskStyle(aStyle)
    , m_firstSize(aSize)
{

}

PairLayoutView::~PairLayoutView()
{
    removeAll();
}

void PairLayoutView::paint(GraphicsContext* context, int status)
{
	//FIXEME : View's onPaint should get the focus by it self
    if (NULL != m_firstView) {
        if (m_firstView->isFocus())
            m_firstView->onPaint(context, FOCUS);
        else
            m_firstView->onPaint(context, NORMAL);
    }

    if (NULL != m_secondView) {
        if (m_secondView->isFocus())
            m_secondView->onPaint(context, FOCUS);
        else
            m_secondView->onPaint(context, NORMAL);
    }
}

bool PairLayoutView::dispatchEvent(Event* event)
{
    View::dispatchEvent(event);

    switch (event->eventType()) {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_CLICK: {
            IntRect irc;
            int _x = ((MouseEvent *)event)->x();
            int _y = ((MouseEvent *)event)->y();

            if (NULL != m_firstView) {
                irc = m_firstView->getRect();
                if (irc.contains(_x, _y)) {
                   m_firstView->focusMe();    
                   m_firstView->dispatchEvent(event);
                    break;
                }
            }
            if (NULL != m_secondView) {
                irc = m_secondView->getRect();
                if (irc.contains(_x, _y)) {
                   m_secondView->focusMe();    
                   m_secondView->dispatchEvent(event);
                    break;
                }
            }

            break;
        }
        default:
            break;
    }

    return DISPATCH_CONTINUE_MSG;
}

bool PairLayoutView::setRect(const IntRect& pirc)
{
    IntRect firstIrc;
    IntRect secondIrc;
    int totalSize = 0;

    if (NULL == m_firstView && NULL == m_secondView)
        return false;

    View::setRect(pirc);
    totalSize = isVertDirection() ? pirc.height() : pirc.width();

    LOGD("%d\n", getAutoWrapType());
    switch (getAutoWrapType()) {
        case LayoutView::LVSAT_PERCENT:
            if (0 == m_firstSize)
                m_firstSize = totalSize / 2;
            else
                m_firstSize = ((totalSize - m_margin * 2 - m_space) * m_firstSize) / 100;
            break;

        case LayoutView::LVSAT_AUTO:
            m_firstSize = 0;
            if (NULL != m_firstView) {
                firstIrc = m_firstView->getRect();
                m_firstSize = isVertDirection() ? firstIrc.height() : firstIrc.width();
                break;
            }
            // dono't break, continue trade 0

        case LayoutView::LVSAT_FIXED:
            if (0 == m_firstSize)
                m_firstSize = totalSize / 2;
            LOGD("%d\n", m_firstSize);
            break;

        default:
            break;
    }

    if (isVertDirection()) {
		calcVertFirstSecondSize(pirc, firstIrc, secondIrc, isReversed());
    } else {
		calcHorzFirstSecondSize(pirc, firstIrc, secondIrc, isReversed());
    }

    LOGD("%d %d %d %d\n", firstIrc.m_left, firstIrc.m_top, firstIrc.m_right, firstIrc.m_bottom);
    if (NULL != m_firstView)
        m_firstView->setRect(firstIrc);

    LOGD("%d %d %d %d\n", secondIrc.m_left, secondIrc.m_top, secondIrc.m_right, secondIrc.m_bottom);
    if (NULL != m_secondView)
        m_secondView->setRect(secondIrc);

	return true;
}

#if 0
IntRect& PairLayoutView::getCalcRect(void)
{
    if (!m_rect.isEmpty()) {
        return m_rect;
    }

    IntRect firstIrc;
    IntRect secondIrc;

    if (NULL != m_firstView)
        firstIrc = m_firstView->getRect();

    if (NULL != m_secondView)
        secondIrc = m_secondView->getRect();

    m_rect = firstIrc;

    if (secondIrc.width() > 0 && secondIrc.height() > 0) {
        if (m_rect.m_left > secondIrc.m_left)
            m_rect.m_left = secondIrc.m_left;
        if (m_rect.m_top < secondIrc.m_top)
            m_rect.m_top = secondIrc.m_top;
        if (m_rect.m_right > secondIrc.m_right)
            m_rect.m_right = secondIrc.m_right;
        if (m_rect.m_bottom < secondIrc.m_bottom)
            m_rect.m_bottom = secondIrc.m_bottom;
    }

    if (m_rect.width() > 0 && m_rect.height() > 0) {
        if (m_margin > 0) {
            m_rect.m_left   -= m_margin;
            m_rect.m_top    -= m_margin;
            m_rect.m_right  += m_margin;
            m_rect.m_bottom += m_margin;
        }
    }

    return m_rect;
}
#endif

void PairLayoutView::switchFirstSecond(void)
{
    IntRect firstIrc;
    IntRect secondIrc;
    IntRect tmpIrc;

    if (NULL != m_firstView)
        firstIrc = m_firstView->getRect();
    if (NULL != m_secondView)
        secondIrc = m_secondView->getRect();

    tmpIrc = firstIrc;
    firstIrc = secondIrc;
    secondIrc = tmpIrc;

    if (NULL != m_firstView)
        m_firstView->setRect(firstIrc);
    if (NULL != m_secondView)
        m_secondView->setRect(secondIrc);
}

void PairLayoutView::removeAll(void)
{
    if (NULL != m_firstView) {
        // delete m_firstView;
        HFCL_DELETE(m_firstView);
        m_firstView = NULL;
    }

    if (NULL != m_secondView) {
        // delete m_secondView;
        HFCL_DELETE(m_secondView);
        m_secondView = NULL;
    }
}

//===================== begin private function define =========================

void PairLayoutView::calcVertFirstSecondSize(const IntRect& pirc, IntRect& fIrc, IntRect& sIrc, bool bReversed)
{
    fIrc.m_left  = /*pirc.m_left + */m_margin;
    fIrc.m_right = pirc.width() - m_margin;
    sIrc.m_left  = fIrc.m_left;
    sIrc.m_right = fIrc.m_right;

    if (bReversed) {
        fIrc.m_bottom = pirc.height() - m_margin;
        fIrc.m_top    = fIrc.m_bottom - m_firstSize;
        sIrc.m_top    = /*pirc.m_top + */m_margin;
        sIrc.m_bottom = fIrc.m_top - m_space;
    } else {
        fIrc.m_top    = /*pirc.m_top + */m_margin;
        fIrc.m_bottom = fIrc.m_top + m_firstSize;
        sIrc.m_top    = fIrc.m_bottom + m_space;
        sIrc.m_bottom = pirc.height() - m_margin;
    }
}

void PairLayoutView::calcHorzFirstSecondSize(const IntRect& pirc, IntRect& fIrc, IntRect& sIrc, bool bReversed)
{
    fIrc.m_top    = /*pirc.m_top + */m_margin;
    fIrc.m_bottom = pirc.height() - m_margin;
    sIrc.m_top    = fIrc.m_top;
    sIrc.m_bottom = fIrc.m_bottom;

    if (bReversed) {
        fIrc.m_right = pirc.width() - m_margin;
        fIrc.m_left  = fIrc.m_right - m_firstSize;
        sIrc.m_left  = /*pirc.m_left + */m_margin;
        sIrc.m_right = fIrc.m_left - m_space;
    } else {
        fIrc.m_left  = /*pirc.m_left + */m_margin;
        fIrc.m_right = fIrc.m_left + m_firstSize;
        sIrc.m_left  = fIrc.m_right + m_space;
        sIrc.m_right = pirc.width() - m_margin;
    }
}

//====================== end private function define ==========================

} // namespace hfcl

