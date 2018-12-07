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


#include "pagenavigateview.h"
#include "imageview.h"
#include "pageview.h"

namespace hfcl {

PageNavigateView::PageNavigateView(int i_id,
                    int x, int y, int w, int h,
                    PageView* pageView,
                    const char* normalImageFile,
                    const char* curImageFile,
                    const char* leftImageFile,
                    const char* rightImageFile)
                    :PanelView(i_id, x, y, w, h)
                    ,m_curImageView(NULL)
                    ,m_leftImageView(NULL)
                    ,m_rightImageView(NULL)
                    ,m_pageView(pageView)
                    ,m_panelView(NULL)
{
    setFocusValid(true);
    m_pageView = pageView;
    m_normalImage = Image::loadImage(normalImageFile);
    m_curImage = Image::loadImage(curImageFile);

    if(NULL != leftImageFile)
    {
        m_leftImage = Image::loadImage(leftImageFile);
    }
    else
        m_leftImage = NULL;

    if(NULL != rightImageFile)
    {
        m_rightImage = Image::loadImage(rightImageFile);
    }
    else
        m_rightImage = NULL;

    setScaled(TRUE);

    init();
}

PageNavigateView::PageNavigateView(View *p_parent, DrawableSet* drset)
            :PanelView(p_parent, drset)
            ,m_width(0)
            ,m_normalImage(NULL)
            ,m_curImage(NULL)
            ,m_leftImage(NULL)
            ,m_rightImage(NULL)
            ,m_curImageView(NULL)
            ,m_leftImageView(NULL)
            ,m_rightImageView(NULL)
            ,m_pageView(NULL)
            ,m_panelView(NULL)
{

}

PageNavigateView::PageNavigateView(View *p_parent)
            :PanelView(p_parent)
            ,m_width(0)
            ,m_normalImage(NULL)
            ,m_curImage(NULL)
            ,m_leftImage(NULL)
            ,m_rightImage(NULL)
            ,m_curImageView(NULL)
            ,m_leftImageView(NULL)
            ,m_rightImageView(NULL)
            ,m_pageView(NULL)
            ,m_panelView(NULL)
{

}

PageNavigateView::~PageNavigateView()
{

}

bool PageNavigateView::isInit()
{
    if(!m_pageView)
        return false;

    if(!m_curImageView)
        return false;

    if(!m_panelView)
        return false;

    return true;
}

#if 0
bool PageNavigateView::setScaled(bool isScaled)
{
    int mode;

    if(isScaled)
        mode = DRAWMODE_SCALED;//IMAGE_DRAWMODE_SCALED;
    else
        mode = DRAWMODE_NORMAL; //IMAGE_DRAWMODE_NORMAL;

    if(!m_normalImage)
        return false;
    else
        m_normalImage->setImageDrawMode(mode);

    if(!m_curImage)
        return false;
    else
        m_curImage->setImageDrawMode(mode);

    if(NULL != m_leftImage)
        m_leftImage->setImageDrawMode(mode);

    if(NULL != m_rightImage)
        m_rightImage->setImageDrawMode(mode);

    return true;
}
#endif

bool PageNavigateView::init()
{
    IntRect girc;
    int    i,total;
    ImageView *tmpImageView;

    girc = getRect();

    if(girc.isEmpty())
        return false;

    if(!m_pageView)
        return false;

    if(!m_normalImage)
        return false;

    if(!m_curImage)
        return false;

    if(isInit())
        return false;

    total = m_pageView->getPageCount();

    m_width = girc.width()/(total + 2);

    m_panelView = HFCL_NEW_EX(ContainerView, (0, m_width, 0, girc.width() - m_width*2, girc.height()));

    for(i = 0; i < total; i++)
    {
        tmpImageView = HFCL_NEW_EX(ImageView, (m_normalImage, 0, i * m_width, 0, m_width, girc.height()));
        m_panelView->addChild(tmpImageView);
    }

    m_curImageView = HFCL_NEW_EX(ImageView, (m_curImage, 0, 0, 0, 0, 0));
    m_curImageView->setRect(IntRect(0, 0, m_width, girc.height()));

    m_panelView->addChild(m_curImageView);

    //m_panelView->setBkOpacity(0);

    addChild(m_panelView);

    if(NULL != m_leftImage)
    {
        m_leftImageView = HFCL_NEW_EX(ImageView, (m_leftImage, 0, 0, 0, 0, 0));

        m_leftImageView->setRect(IntRect(0, 0, m_width, girc.height()));
        addChild(m_leftImageView);
        m_leftImageView->hide();
    }

    if(NULL != m_rightImage)
    {
        m_rightImageView = HFCL_NEW_EX(ImageView, (m_rightImage, 0, 0, 0, 0, 0));

        m_rightImageView->setRect(IntRect(girc.width() - m_width, 0, girc.width(), girc.height()));
        addChild(m_rightImageView);
        m_rightImageView->hide();
    }

    //setBkOpacity(0);
    return true;
}

bool PageNavigateView::next()
{
    IntRect irc;
    IntRect girc;
    bool isLoop = true;

    if(!isInit())
        return false;

    isLoop = m_pageView->isLoopAble();

    irc = m_curImageView->getRect();
    girc = m_panelView->getRect();

    if(irc.right() > girc.width() - m_width)
    {
        if(isLoop)
        {
            m_curImageView->setRect(IntRect(0, 0, m_width, irc.bottom()));
            updateView();

            return true;
        }
        else
            return false;
    }

    m_curImageView->setRect(IntRect(irc.left()+m_width, 0, irc.right()+m_width, irc.bottom()));
    updateView();

    return true;
}

bool PageNavigateView::prev()
{
    IntRect irc;
    IntRect girc;
    bool isLoop = true;

    if(!isInit())
        return false;

    isLoop = m_pageView->isLoopAble();

    irc = m_curImageView->getRect();
    girc = m_panelView->getRect();

    if(irc.left() < m_width)
    {
        if(isLoop)
        {
            m_curImageView->setRect(IntRect(girc.width() - m_width, 0, girc.width(), irc.bottom()));
            updateView();

            return true;
        }
        else
            return false;
    }

    m_curImageView->setRect(IntRect(irc.left()-m_width, 0, irc.right()-m_width, irc.bottom()));
    updateView();

    return true;
}

void PageNavigateView::onGetFocus()
{
    if(m_leftImageView)
        m_leftImageView->show();

    if(m_rightImageView)
        m_rightImageView->show();
}

void PageNavigateView::onLoseFocus()
{
    if(m_leftImageView)
        m_leftImageView->hide();

    if(m_rightImageView)
        m_rightImageView->hide();
}
#if 0
bool PageNavigateView::pageViewChanged()
{

    if(!isInit())
        return false;

    removeChild(m_panelView);
    HFCL_DELETE(m_panelView);

    if(m_leftImageView)
    {
        removeChild(m_leftImageView);
        HFCL_DELETE(m_leftImageView);
    }

    if(m_rightImageView)
    {
        removeChild(m_rightImageView);
        HFCL_DELETE(m_rightImageView);
    }

    return init();
}
#endif

bool PageNavigateView::dispatchEvent(Event *event)
{
    if(Event::KEY_DOWN == event->eventType())
    {
        switch(((KeyEvent*)event)->keyCode())
        {
            case KeyEvent::KEYCODE_CURSOR_LEFT:
                if(prev())
                    if(m_pageView)
                        m_pageView->prevPage();
                break;
            case KeyEvent::KEYCODE_CURSOR_RIGHT:
                if(next())
                    if(m_pageView)
                        m_pageView->nextPage();
                break;
            default:
                return View::dispatchEvent(event);
        }
    }

    return DISPATCH_STOP_MSG;
}


} // namespace hfcl
