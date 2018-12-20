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


#include "pageview.h"

namespace hfcl {

View *PageView::nextPage(void)
{
    View * next = getCurPage();
    if(!next)
        setCurPage(firstChild());
    else
    {
        next = next->nextSibling();
        if(!next && isLoopAble())
            next = firstChild();
        if(next)
            setCurPage(next);
    }

    return getCurPage();

}

View *PageView::prevPage(void)
{
    View *prev = getCurPage();

    if(!prev)
        setCurPage(firstChild());
    else {
        prev = prev->prevSibling();
        if(!prev && isLoopAble())
            prev = lastChild();
        if(prev)
            setCurPage(prev);
    }

    return getCurPage();
}

bool PageView::setCurPage(View *page)
{
    if(NULL != m_cur_page)
        m_cur_page->hide();

    m_cur_page = page;

    if (NULL != m_cur_page)
        m_cur_page->show();

    updateView();

    return false;
}

bool PageView::appendPage(View *page)
{
    IntRect irc(getRect());
    irc.setRect(0, 0, irc.right(), irc.bottom());

    addChild(page);
    if (page)
    {

        page->setRect(irc);
        page->hide();
    }

    return true;
}

bool PageView::addPage(View *page)
{
    IntRect irc(getRect());
    irc.setRect(0, 0, irc.right(), irc.bottom());

    addChildHead(page);
    if (page)
    {

        page->setRect(irc);
        page->hide();
    }

    return false;
}

bool PageView::dispatchEvent(Event *event)
{
    if(Event::CUSTOM_NOTIFY == event->eventType())
    {
        return (!View::dispatchEvent(event));
    }

    PanelView::dispatchEvent(event);

    return DISPATCH_CONTINUE_MSG;

}

bool PageView::setCurPageByIndex(const unsigned int index)
{
    return setCurPage(pageFromIndex(index));
}

int PageView::getCurPageIndex(void)
{
    return indexFromPage(getCurPage());
}

} // namespace hfcl
