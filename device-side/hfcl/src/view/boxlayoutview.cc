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


#include <string.h>

#include "boxlayoutview.h"


namespace hfcl {

BoxLayoutView::BoxLayoutView()
    : m_count(0)
    , m_cells(0)
{

}

BoxLayoutView::BoxLayoutView(View *p_parent)
    : LayoutView(p_parent)
    , m_count(0)
    , m_cells(0)
{

}

BoxLayoutView::BoxLayoutView(View* p_parent, DrawableSet* drset)
    : LayoutView(p_parent, drset)
    , m_count(0)
    , m_cells(0)
{

}

BoxLayoutView::BoxLayoutView(int aCount) : m_count(aCount)
{
    if (aCount > 0)
        resizeBoxLayout(aCount);
}

BoxLayoutView::~BoxLayoutView()
{
    removeAllCell();
}

void BoxLayoutView::paint(GraphicsContext* context, int status)
{
    for (int i = 0; i < m_count; i++) {
        if (NULL != m_cells[i].s_view) {
				//FIXEDME: View should take focus by itself
            if ((m_cells[i].s_view)->isFocus())
                (m_cells[i].s_view)->onPaint(context, FOCUS);
            else
                (m_cells[i].s_view)->onPaint(context, NORMAL);
        }
    }
}

bool BoxLayoutView::dispatchEvent(Event* event)
{
    View::dispatchEvent(event);

    switch (event->eventType()) {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_CLICK: {
            for (int i = 0; i < m_count; i++) {
                if (NULL != m_cells[i].s_view) {
                    int _x = ((MouseEvent *)event)->x();
                    int _y = ((MouseEvent *)event)->y();
                   
                    IntRect irc = (m_cells[i].s_view)->getRect();

                    if (irc.contains(_x, _y)) {
                       (m_cells[i].s_view)->focusMe();    
                       (m_cells[i].s_view)->dispatchEvent(event);
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return DISPATCH_CONTINUE_MSG;
}

bool BoxLayoutView::setRect(const IntRect& irc)
{
    View::setRect(irc);
    recalBox();

    return true;
}

int BoxLayoutView::setCellInfo(int idx, int size, int type, bool bupdate)
{
    if (idx < 0 || idx >= m_count)
        return 0;

    m_cells[idx].s_size = size;
    m_cells[idx].s_type = type;

    if (bupdate)
        recalBox();

    return 1;
}

int BoxLayoutView::getCellAutoSize(int idx)
{
    if (idx < 0 || idx >= m_count || NULL == m_cells[idx].s_view)
        return 0;

    IntRect irc = m_cells[idx].s_view->getRect();

    if (isVertDirection())
        return irc.height();
    else
        return irc.width();
}

View* BoxLayoutView::setCell(int idx, View* aView, int size, int type, bool bupdate)
{
    if (idx < 0 || idx >= m_count)
        return NULL;

    View* oldView = m_cells[idx].s_view;
    m_cells[idx].s_view = aView;

    m_cells[idx].s_size = size;
    m_cells[idx].s_type = type;

    if (bupdate)
        recalBox();

    return oldView;
}

View* BoxLayoutView::setCell(int idx, View* aView)
{
    if (idx < 0 || idx >= m_count)
        return NULL;

    View* oldView = m_cells[idx].s_view;
    m_cells[idx].s_view = aView;

    return oldView;
}

View* BoxLayoutView::getCell(int idx)
{
    if (idx < 0 || idx >= m_count)
        return NULL;

    return m_cells[idx].s_view;
}

void BoxLayoutView::removeAllCell(void)
{
    for (int i = 0; i < m_count; i++) {
        // delete m_cells[i].s_view;
        HFCL_DELETE(m_cells[i].s_view);
        m_cells[i].s_view = NULL;
    }

    deleteCells();
}

void BoxLayoutView::resizeBoxLayout(int count)
{
    if (count == m_count)
        return;

    BoxLayoutNode* cells = HFCL_NEW_ARR(BoxLayoutNode, count);
    if (NULL != cells && m_count > 0) {
        memset(cells, 0x00, count * sizeof(BoxLayoutNode));
        memcpy(cells, m_cells, (m_count < count ? m_count : count) * sizeof(BoxLayoutNode));
        for (int i = count; i < m_count; i++) {
            HFCL_DELETE(m_cells[i].s_view);
            m_cells[i].s_view = NULL;
        }
    }

    deleteCells();

    m_count = count;
    m_cells = cells;
}

void BoxLayoutView::recalBox(void)
{
    if (isVertDirection()) {
        recalVertBox();
    } else {
        recalHorzBox();
    }
}

void BoxLayoutView::setMargin(int margin)
{
    m_margin = margin;
    recalBox();
}

void BoxLayoutView::setSpace(int space)
{
    m_space = space;
    recalBox();
}

//==================== begin protected function define ========================

bool BoxLayoutView::calcSize(int totalSize, int* aSizes)
{
    if (NULL == aSizes)
        return false;

    if (m_space > 0 && m_count > 1)
        totalSize -= m_space * (m_count - 1);
    totalSize -= m_margin * 2;

    int autoCount = 0;
    int tmpSize = totalSize;

    for (int i = 0; i < m_count; i++) {
        int size = m_cells[i].s_size;
        if (size > 0) {
            switch (m_cells[i].s_type) {
                case LayoutView::LVSAT_FIXED:
                    aSizes[i] = size;
                    break;

                case LayoutView::LVSAT_PERCENT:
                    aSizes[i] = (totalSize * size) / 100;
                    break;

                case LayoutView::LVSAT_AUTO:
                    aSizes[i] = getCellAutoSize(i);
                    break;

                default:
                    break;
            }
        } else {
            aSizes[i] = 0;
            autoCount++;
        }
        tmpSize -= aSizes[i];
    }

    if (tmpSize > 0 && autoCount > 0) {
        int avg = tmpSize / autoCount;
        for (int i = 0; i < m_count; i++) {
            if (0 == aSizes[i])
                aSizes[i] = avg;
        }
    }

    return true;
}

//===================== end protected function define =========================

//===================== begin private function define =========================

void BoxLayoutView::recalVertBox(void)
{
    if (m_count <= 0)
        return;

    int* sizes = HFCL_NEW_ARR(int, m_count);
    if (NULL == sizes)
        return;

    if (calcSize(m_rect.height(), sizes)) {
        // IntRect irc(m_rect.m_left + m_margin, m_rect.m_top + m_margin, m_rect.m_right - m_margin, 0);
        IntRect irc(m_margin, m_margin, m_rect.width() - m_margin, 0);
        for (int i = 0; i < m_count; i++) {
            irc.m_bottom = irc.m_top + sizes[i];
            if (NULL != m_cells[i].s_view)
                m_cells[i].s_view->setRect(irc);
            irc.m_top = irc.m_bottom + m_space;
            LOGD("%d %d %d %d\n", irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        }
    }

    if (NULL != sizes)
        HFCL_DELETE_ARR(sizes);
    sizes = NULL;
}

void BoxLayoutView::recalHorzBox(void)
{
    if (m_count <= 0)
        return;

    int* sizes = HFCL_NEW_ARR(int, m_count);
    if (NULL == sizes)
        return;

    if (calcSize(m_rect.width(), sizes)) {
        // IntRect irc(m_rect.m_left + m_margin, m_rect.m_top + m_margin, 0, m_rect.m_bottom - m_margin);
        IntRect irc(m_margin, m_margin, 0, m_rect.height() - m_margin);
        for (int i = 0; i < m_count; i++) {
            irc.m_right = irc.m_left + sizes[i];
            if (NULL != m_cells[i].s_view)
                m_cells[i].s_view->setRect(irc);
            irc.m_left = irc.m_right + m_space;
            LOGD("%d %d %d %d\n", irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        }
    }

    if (NULL != sizes)
        HFCL_DELETE_ARR(sizes);
    sizes = NULL;
}

void BoxLayoutView::deleteCells(void)
{
    if (NULL != m_cells && m_count > 0) {
        HFCL_DELETE_ARR(m_cells);
        m_cells = NULL;
        m_count = 0;
    }
}

//====================== end private function define ==========================

} // namespace hfcl

