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

#include "view/animateimageview.h"

#include "services/timerservice.h"
#include "resource/respkgmanager.h"

namespace hfcl {

AnimateImageView::AnimateImageView(View* p_parent)
    : View(p_parent, NULL)
{
    init();
}

AnimateImageView::AnimateImageView(View* p_parent, DrawableSet* drset)
    : View(p_parent,drset)
{
    init();
}

AnimateImageView::AnimateImageView(Image* pImage,
                     int i_id,
                     int x, int y, int w, int h)
    : View(i_id, x, y, w, h)
{
    init();
    m_image = pImage;
}

AnimateImageView::AnimateImageView(const char * filePath,
                     int i_id,
                     int x, int y, int w, int h,
                     int mode,
                     int align,
                     int valign)
    : View(i_id, x, y, w, h)
{
	init();
    m_image = Image::loadImage(filePath);
    setImageDrawMode(mode);
    setImageAlign(align);
    setImageVAlign(valign);
}

AnimateImageView::~AnimateImageView()
{
	stop();
	if(m_image) {
		HFCL_DELETE(m_image);
	}
}

void AnimateImageView::drawBackground(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/)
{
	View::drawBackground(context, rc, status);
}

void AnimateImageView::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
	if(m_format.rotationAngle != 0){
		m_format.drawMode = DRAWMODE_ROTATIONANGLE;
	}
	if(m_image != NULL){
        m_image->paint(context, rc, m_format, m_xo, m_yo);
	}
} 

bool AnimateImageView::setImage(Image *pImg)
{
	if (pImg == m_image) {
		return true;
	}

	if (m_image != NULL) {
		HFCL_DELETE(m_image);
	}

	m_image = pImg;
	updateView();
    return true;
}

bool AnimateImageView::setReplaceColor(const DWORD color)
{
    m_image->setReplaceColor(color);
    updateView();
    return true;
}

void AnimateImageView::setPartBoxXoYo(int xo, int yo)
{
    m_xo = xo;
    m_yo = yo;
}

void AnimateImageView::getPartBoxXoYo(int& xo, int& yo)
{
    xo = m_xo;
    yo = m_yo;
}

void AnimateImageView::setFromImgRect(bool bSet)
{
    if (!bSet)
        return;

    if (m_image == NULL)
        return;

    setRect(0, 0, getImageWidth(), getImageHeight());
}

int AnimateImageView::getImageWidth(void)
{
    if (m_image != NULL)
        return m_image->width();

    return 0;
}

int AnimateImageView::getImageHeight(void)
{
    if (m_image != NULL)
        return m_image->height();

    return 0;
}

bool AnimateImageView::start(void)
{
    if (Play == m_state)
        return false;

    m_timer_id = registerTimer(m_timeDuration, "AnimateImageView");

    if (m_timer_id < 0)
        return false;

    m_state = Play;
    m_elapsed_10ms = 0;
    m_imageId_array_index = 0;
    return true;
}

bool AnimateImageView::stop(void)
{
    if (m_timer_id >= 0)
    {
        removeTimer(m_timer_id);
        m_timer_id = -1;
    }
	if(m_state == Stop) return false;
    m_state = Stop;

    return true;
}

bool AnimateImageView::setImageIdArray(unsigned int * array, unsigned int num, int timeDuration,const DWORD color)
{
    m_imageId_array = array;
    m_imageId_array_num = num;
    m_timeDuration = timeDuration;
    setImage(GetImageRes(m_imageId_array[0]));
    m_replaceColor = color;
    setReplaceColor(color);
    return true;
}

bool AnimateImageView::handleEvent(Event* event)
{
    // check state
    if (Stop == m_state)
        return DISPATCH_STOP_MSG;

    if (event->eventType() == Event::TIMER
			&& m_timer_id == ((TimerEvent *)event)->timerID())
	{
		if(m_elapsed_10ms >= m_timeDuration)
		{
			if(m_imageId_array_index < (m_imageId_array_num-1))
			{
				m_imageId_array_index++;
			}
			else{
				m_imageId_array_index = 0;
			}
			setImage(GetImageRes(m_imageId_array[m_imageId_array_index]));
			setReplaceColor(m_replaceColor);
			m_elapsed_10ms = 0;
		}
		m_elapsed_10ms = m_elapsed_10ms + 10;
	}

    return DISPATCH_CONTINUE_MSG;
}

DEFINE_CLASS_NAME(AnimateImageView)

} // namespace hfcl {

