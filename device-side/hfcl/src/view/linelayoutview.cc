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


#include "linelayoutview.h"


namespace hfcl {

LineLayoutView::LineLayoutView()
    : m_node(0)
    , m_tailNode(0)
    , m_lineHeight(0)
{
    /*DrawableSelector* selector = DrawableSelector::createDefaultSelector("LineLayoutView");
    if (selector) {
        initDrawable(selector);
        selector->release();
    }*/
}

LineLayoutView::LineLayoutView(View *p_parent)
    : LayoutView(p_parent)
    , m_node(0)
    , m_tailNode(0)
    , m_lineHeight(0)
{
}

LineLayoutView::LineLayoutView(View* p_parent, DrawableSet* drset)
    : LayoutView(p_parent, drset)
    , m_node(0)
    , m_tailNode(0)
    , m_lineHeight(0)
{
    /*
    if (!selector)
        selector = DrawableSelector::createDefaultSelector("LineLayoutView");

    if (selector) {
        initDrawable(selector);
        selector->release();
    }*/
}

LineLayoutView::~LineLayoutView()
{
    removeAll();
}

#if 0
void LineLayoutView::initDrawable(DrawableSelector* selector)
{
    if (NULL == selector) {
        return;
    }
    View::initDrawable(selector);
}
#endif

void LineLayoutView::paint(GraphicsContext* context, int status)
{
    LineLayoutNode* curNode = NULL;

    for (curNode = m_node; curNode; curNode = curNode->next) {
        if ((curNode->s_view)->isVisible()) {
            //FIXEDME : view should take the focus state by itself
            if ((curNode->s_view)->isFocused())
                (curNode->s_view)->onPaint(context, FOCUS);
            else
                (curNode->s_view)->onPaint(context, NORMAL);
        }
    }
}

bool LineLayoutView::dispatchEvent(Event* event)
{
    View::dispatchEvent(event);

    switch (event->eventType()) {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_CLICK: {
            LineLayoutNode* curNode = NULL;
            for (curNode = m_node; curNode; curNode = curNode->next) {
                int _x = ((MouseEvent *)event)->x();
                int _y = ((MouseEvent *)event)->y();

                IntRect irc = (curNode->s_view)->getRect();

                if (irc.contains(_x, _y)) {
                   (curNode->s_view)->focusMe();
                   (curNode->s_view)->dispatchEvent(event);
                }
            }
            break;
        }
        default:
            break;
    }

    return GOON_DISPATCH;
}

bool LineLayoutView::setRect(const IntRect& irc)
{
    View::setRect(irc);

    if (isVertDirection()) {
        verticalRecalc();
    } else {
        horizontalRecalc();
    }
    return true;
}

void LineLayoutView::recalc(void)
{
    if (isVertDirection()) {
        verticalRecalc();
    } else {
        horizontalRecalc();
    }
}

bool LineLayoutView::prependView(View* aView, int aType, int aMinSize, int aMaxSize)
{
    if (NULL == aView)
        return false;

    LineLayoutNode* newNode = newLineLayoutNode(aView, aType, aMinSize, aMaxSize);

    if (NULL == newNode)
        return false;

    newNode->next = NULL;
    newNode->prev = m_node;
    if (NULL != m_node)
        m_node->next = newNode;
    m_node = newNode;
    if (NULL == m_tailNode)
        m_tailNode = newNode;

    return true;
}

bool LineLayoutView::addView(View* aView, int aType, int aMinSize, int aMaxSize)
{
    return appendView(aView, aType, aMinSize, aMaxSize);
}

bool LineLayoutView::appendView(View* aView, int aType, int aMinSize, int aMaxSize)
{
    if (NULL == aView)
        return false;

    LineLayoutNode* newNode = newLineLayoutNode(aView, aType, aMinSize, aMaxSize);

    if (NULL == newNode)
        return false;

    newNode->next = NULL;
    newNode->prev = m_tailNode;
    if (NULL != m_tailNode)
        m_tailNode->next = newNode;
    m_tailNode = newNode;
    if (NULL == m_node)
        m_node = newNode;

    return true;
}

LineLayoutNode* LineLayoutView::findNode(View* aView)
{
    if (NULL == aView)
        return NULL;

    LineLayoutNode* curNode = NULL;

    for (curNode = m_node; curNode; curNode = curNode->next)
        if (curNode->s_view == aView)
            return curNode;

    return NULL;
}

void LineLayoutView::removeAll(void)
{
    while (NULL != m_node) {
        LineLayoutNode* curNode = m_node;
        m_node = m_node->next;
        // TODO: delete View
        // delete curNode->s_view;
        HFCL_DELETE(curNode->s_view);
        // delete curNode;
        HFCL_DELETE(curNode);
    }

    m_node = NULL;
    m_tailNode = NULL;
    m_last = m_first = NULL;
    m_nr_children = 0;
}

int LineLayoutView::getNodeCount(void)
{
    int nodeCount = 0;
    LineLayoutNode* node = m_node;

    while (NULL != node) {
        node = node->next;
        nodeCount++;
    }

    return nodeCount;
}

void LineLayoutView::setMargin(int margin)
{
    if (m_margin == margin)
        return;

    m_margin = margin;
    recalc();
}

void LineLayoutView::setSpace(int space)
{
    if (m_space == space)
        return;
    m_space = space;

    recalc();
}

//===================== begin private function define =========================

LineLayoutNode* LineLayoutView::newLineLayoutNode(View* aView, int aType, int aMinSize, int aMaxSize)
{
    if (NULL == aView)
        return NULL;

    if (isExist(aView))
        return NULL;

    LineLayoutNode* newNode = NULL;

    newNode = HFCL_NEW(LineLayoutNode);
    if (NULL == newNode)
        return NULL;

    newNode->s_view = aView;
    newNode->flags = aType & 0xFF;
    newNode->defMinSize = aMinSize;
    newNode->defMaxSize = aMaxSize;

    return newNode;
}

bool LineLayoutView::isExist(View* aView)
{
    if (NULL == aView)
        return true;

    LineLayoutNode* node = m_node;

    while (NULL != node && node->s_view != aView) {
        if (node && node->s_view == aView)
            return true;
        node = node->next;
    }

    return false;
}

void LineLayoutView::horizontalRecalc(void)
{
    if (isAutoWrapType()) {
        autoHorzRecalc();
    } else {
        fixedHorzRecalc();
    }
}

void LineLayoutView::fixedHorzRecalc(void)
{
    IntRect irc;

    irc.m_left = /*m_rect.left() + */m_margin;
    irc.m_top = /*m_rect.top() + */m_margin;
    irc.m_bottom = m_rect.height() - m_margin;

    int minNodeCount = 0;
    LineLayoutNode* minNode = NULL;
    LineLayoutNode* node = m_node;

    while (node) {
        if (irc.m_left >= m_rect.width() - m_margin)
            break;

        if (node->flags & LayoutView::LVSAT_AUTO) {
            irc.m_right = irc.m_left + node->defMinSize;
            if (NULL == minNode)
                minNode = node;
            minNodeCount++;
        } else {
            IntRect tmp_irc = node->s_view->getRect();
            irc.m_right = irc.m_left + tmp_irc.width();
        }

        //LOGI("%d %d %d %d\n", irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        node->s_view->setRect(irc);
        irc.m_left = irc.m_right + m_space;

        node = node->next;
    }

    if (NULL != node && minNodeCount > 0) {
        int left = m_rect.width() - irc.m_left + m_space - m_margin;
        int offset = 0;
        if (left <= 0)
            goto FIXED_HORZ_RECALC_END;

        int avg = left / minNodeCount;
        for ( ; minNode; minNode = minNode->next) {
            IntRect minIrc = minNode->s_view->getRect();
            minIrc.m_left  += offset;
            minIrc.m_right += offset;
            if (minNode->flags & LayoutView::LVSAT_AUTO) {
                minIrc.m_right += avg;
                offset += avg;
            }
            //LOGI("%d %d %d %d\n", irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
            minNode->s_view->setRect(irc);
        }
    }

FIXED_HORZ_RECALC_END:
    remainingClearedNodeRect(node);
}

void LineLayoutView::autoHorzRecalc(void)
{
    IntRect irc;

    irc.m_left = /*m_rect.left() + */m_margin;
    irc.m_top  = /*m_rect.top() + */m_margin;
    irc.m_bottom = irc.m_left + m_lineHeight;
    if (irc.m_bottom > m_rect.height() - m_margin)
        irc.m_bottom = m_rect.height() - m_margin;

    LineLayoutNode* node = m_node;

    while (node) {
        if (irc.m_left >= m_rect.width() - m_margin) {
            irc.m_left = irc.m_right + m_space;
            if (irc.m_left >= m_rect.height() - m_margin)
                break;
            irc.m_right = irc.m_left + m_lineHeight;
            if (irc.m_bottom > m_rect.height() - m_margin)
                irc.m_bottom = m_rect.height() - m_margin;
            irc.m_left = m_margin;
        }

        if (node->flags & LayoutView::LVSAT_AUTO) {
            int left = m_rect.width() - m_margin - irc.m_left;
            if (node->defMaxSize > 0 && left > node->defMaxSize)
                left = node->defMaxSize;
            irc.m_right = irc.m_left + left;
        } else {
            IntRect tmp_irc = node->s_view->getRect();
            irc.m_right = irc.m_left + tmp_irc.width();
        }

        //LOGD("%d %d %d %d\n", irc.m_left, irc.m_top, irc.m_right, irc.m_bottom);
        node->s_view->setRect(irc);
        irc.m_left = irc.m_right + m_space;

        node = node->next;
    }

    remainingClearedNodeRect(node);
}

void LineLayoutView::verticalRecalc(void)
{
    if (isAutoWrapType()) {
        autoVertRecalc();
    } else {
        fixedVertRecalc();
    }
}

void LineLayoutView::fixedVertRecalc(void)
{
    IntRect irc;

    irc.m_left = /*m_rect.left() + */m_margin;
    irc.m_top = /*m_rect.top() + */m_margin;
    irc.m_right = m_rect.width() - m_margin;

    int minNodeCount = 0;
    LineLayoutNode* minNode = NULL;
    LineLayoutNode* node = m_node;

    while (node) {
        if (irc.m_top >= m_rect.height() + m_margin)
            break;

        if (node->flags & LayoutView::LVSAT_AUTO) {
            irc.m_bottom = irc.m_top + node->defMinSize;
            if (NULL == minNode)
                minNode = node;
            minNodeCount++;
        } else {
            IntRect tmp_irc = node->s_view->getRect();
            irc.m_bottom = irc.m_top + tmp_irc.height();
        }

        node->s_view->setRect(irc);
        irc.m_top = irc.m_bottom + m_space;

        node = node->next;
    }

    if (NULL != node && minNodeCount > 0) {
        int bottom = m_rect.height() - irc.m_top + m_space - m_margin;
        int offset = 0;
        if (bottom <= 0)
            goto FIXED_VERT_RECALC_END;

        int avg = bottom / minNodeCount;
        for ( ; minNode; minNode = minNode->next) {
            IntRect minIrc = minNode->s_view->getRect();
            minIrc.m_top    += offset;
            minIrc.m_bottom += offset;
            if (minNode->flags & LayoutView::LVSAT_AUTO) {
                minIrc.m_bottom += avg;
                offset += avg;
            }
            minNode->s_view->setRect(irc);
        }
    }

FIXED_VERT_RECALC_END:
    remainingClearedNodeRect(node);
}

void LineLayoutView::autoVertRecalc(void)
{
    IntRect irc;

    irc.m_left = /*m_rect.left() + */m_margin;
    irc.m_top = /*m_rect.top() + */m_margin;
    irc.m_right = irc.m_top + m_lineHeight;
    if (irc.m_right > m_rect.width() - m_margin)
        irc.m_right = m_rect.width() - m_margin;

    LineLayoutNode* node = m_node;

    while (node) {
        if (irc.m_top >= m_rect.height() - m_margin) {
            irc.m_top = irc.m_bottom + m_space;
            if (irc.m_left >= m_rect.width() - m_margin)
                break;
            irc.m_bottom = irc.m_top + m_lineHeight;
            if (irc.m_right > m_rect.width() - m_margin)
                irc.m_right = m_rect.width() - m_margin;
            irc.m_top = m_margin;
        }

        if (node->flags & LayoutView::LVSAT_AUTO) {
            int bottom = m_rect.height() - m_margin - irc.m_top;
            if (node->defMaxSize > 0 && bottom > node->defMaxSize)
                bottom = node->defMaxSize;
            irc.m_bottom = irc.m_top + bottom;
        } else {
            // VincentWei: IntRect tmp_irc = node->s_view->getRect();
            irc.m_bottom = irc.m_top + m_rect.height();
        }

        node->s_view->setRect(irc);
        irc.m_top = irc.m_bottom + m_space;

        node = node->next;
    }

    remainingClearedNodeRect(node);
}

void LineLayoutView::remainingClearedNodeRect(LineLayoutNode* node)
{
    while (node) {
        IntRect irc;
        node->s_view->setRect(irc);
        node = node->next;
    }
}

//====================== end private function define ==========================

} // namespace hfcl

