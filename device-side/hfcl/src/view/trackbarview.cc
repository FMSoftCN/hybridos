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


#include "graphicscontext.h"
#include "trackbarview.h"

namespace hfcl {

DEFINE_CLASS_NAME(TrackBarView)

void TrackBarView::initTrackBarView()
{
    m_type = TrackBarView::HORIZONTAL;
    m_curValue = 0;
    m_maxValue = 100;
    m_minValue = 0;
    m_stepValue = 5;
    m_border = 1;
    //m_startImg = NULL;
    //m_pauseImg = NULL;
    //m_drTrackBar = HFCL_NEW(ImageDrawable);
    //m_drThumbBlock = HFCL_NEW(ImageDrawable);
}

TrackBarView::TrackBarView()
{
    initTrackBarView();
}

TrackBarView::TrackBarView(View* p_parent):
    View(p_parent, DEFAULT_VIEW_DRAWABLESET(TrackBarView))
{
    initTrackBarView();
}

TrackBarView::TrackBarView(View* p_parent, DrawableSet* drset):
    View(p_parent, drset)
{
    initTrackBarView();
}

TrackBarView::TrackBarView(int i_id, int x, int y, int w, int h):
    View(i_id, x, y, w, h)
{
    initTrackBarView();
}

TrackBarView::~TrackBarView()
{
}

/*void TrackBarView::initDrawable(DrawableSelector* selector)
{
    if (NULL == selector) {
        return;
    }
    View::initDrawable(selector);
}*/

void TrackBarView::stepForward()
{
    setPosition(m_curValue + m_stepValue);
}

void TrackBarView::stepBackward()
{
    setPosition(m_curValue - m_stepValue);
}

void TrackBarView::setPosition(int value)
{
    if (value < m_minValue)
        value = m_minValue;
    if (value > m_maxValue)
        value = m_maxValue;
    if (m_curValue == value)
        return;
    m_curValue = value;
    updateView();
}

/*
void TrackBarView::setTrackBarImage(Image* img)
{
    if (NULL == m_drTrackBar)
        return;
    m_drTrackBar->setImage(img);
    //updateView();
}

void TrackBarView::setThumbBlockImage(Image* img)
{
    if (NULL == m_drThumbBlock)
        return;
    //m_drThumbBlock->setImage(img);
	//m_drset->setDrawableElement();
    updateView();
}*/

void TrackBarView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
    if(m_maxValue == 0)
    {
        LOGE("the maxValue is %d", m_maxValue);
        return ;
    }
    int w = 0, h = 0;
    int track_l = 0, track_w = 0, track_b = 0, track_h = 0;
    IntRect irc, thumb_irc;

	m_drset->calcDrawableSize(DR_THUMB, DRAWSTATE_NORMAL, w, h);
	
	if (m_type == TrackBarView::VERTICAL)
	{
		track_b = rc.bottom() - h / 2;
		track_h = (rc.height() - h ) * m_curValue / m_maxValue;
		irc.setRect(rc.m_left, track_b - track_h, rc.m_right, track_b);
	}
	else
	{
		track_l = rc.m_left  + w / 2;
		track_w = (rc.width() - w) * m_curValue / m_maxValue;
		//irc.setRect(track_l, rc.m_top, track_l + w, rc.m_bottom);
		irc.setRect(track_l, rc.m_top, track_l + track_w, rc.m_bottom);
	}
	//TODO m_drTrackBar->draw(context, irc, status);
	m_drset->draw(context, DR_CONTENT, DRAWSTATE_NORMAL, irc);

	if (m_type == TrackBarView::VERTICAL)
		irc.setRect(rc.m_left, track_b - track_h - h / 2, rc.m_right, track_b - track_h + h / 2);
	else
		irc.setRect(track_l + track_w - w / 2, rc.m_top, track_l + track_w + w / 2, rc.m_bottom);
	m_drset->draw(context, DR_THUMB, DRAWSTATE_NORMAL, irc);


    if (m_border > 0) {
        if (m_type == TrackBarView::HORIZONTAL)
        {
            irc.setRect(rc.m_left, rc.m_top, rc.m_right, rc.m_top+m_border);
            m_drset->draw(context, DR_CONTENT2, DRAWSTATE_NORMAL, irc);
            irc.setRect(rc.m_left, rc.m_bottom-m_border, rc.m_right, rc.m_bottom);
            m_drset->draw(context, DR_CONTENT2, DRAWSTATE_NORMAL, irc);
            irc.setRect(rc.m_left, rc.m_top, rc.m_left+m_border, rc.m_bottom);
            m_drset->draw(context, DR_CONTENT2, DRAWSTATE_NORMAL, irc);
            irc.setRect(rc.m_right-m_border, rc.m_top, rc.m_right, rc.m_bottom);
            m_drset->draw(context, DR_CONTENT2, DRAWSTATE_NORMAL, irc);
	#if 0
            for (int i=1; i<m_maxValue; i++)
            {
                track_w = (rc.width() - w) * i / m_maxValue;
                //irc.setRect(track_l, rc.m_top, track_l + w, rc.m_bottom);
                irc.setRect(track_l + track_w-m_border, rc.m_top, track_l + track_w, rc.m_bottom);
                m_drset->draw(context, DR_CONTENT2, DRAWSTATE_NORMAL, irc);
            }
	#endif
        }
    }
}
} // namespace hfcl {
