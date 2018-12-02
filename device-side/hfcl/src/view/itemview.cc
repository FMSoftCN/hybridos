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


#include "itemview.h"

NAMESPACE_BEGIN

bool ItemView::g_is_all_itemview_need_convert = false;

void ItemView::paint(GraphicsContext* context, int status)
{	
	int cont_state = DRAWSTATE_NORMAL;
	IntRect rc(0, 0, m_rect.width(), m_rect.height());

    if (!isPaintHilightFirst())
	{
		draw_hilite(context, rc, status);
	}

	if(isFocus()) {
		cont_state = DRAWSTATE_HILIGHT;
    } else if(isSelected()) {
		cont_state = DRAWSTATE_SELECTED;
    }

	draw_select(context, rc, cont_state);
	drawContent(context, rc, cont_state);

    if (isPaintHilightFirst()){
	    draw_hilite(context, rc, status);
    }

    drawScroll(context, rc, status & PAINT_STATUS_MASK);
}

void ItemView::drawContent(GraphicsContext* context, IntRect& rc, int status)
{
	if ((IsAllItemViewNeedConvert()  && !isConverted())
		|| (!IsAllItemViewNeedConvert()  && isConverted()))
	{
		int w = rc.width();
		//int h = rc.height();
		
		for(View *view = firstChild(); view; view = view->nextSibling()) {
			IntRect vrc = view->getRect();
			view->setRect(w - vrc.right(), vrc.top(), w - vrc.left(), vrc.bottom());
		}
		convertLayout(!isConverted());
	}
	return PanelView::drawContent(context, rc, status);
}


void ItemView::draw_select(GraphicsContext* context, IntRect &rc, int status)
{
    IntRect rect(rc);
	
	if(m_drset == NULL)
		return;

	if (IsAllItemViewNeedConvert())  {
		rect.setRect(rect.right() - m_select_mark_rect.left() - m_select_mark_rect.width(), 
	            rect.top() + m_select_mark_rect.top(), 
	            rect.right() - m_select_mark_rect.left(), 
	            rect.top() + m_select_mark_rect.top() + m_select_mark_rect.height());
	} else {
	    rect.setRect(rect.left() + m_select_mark_rect.left(), 
	            rect.top() + m_select_mark_rect.top(), 
	            rect.left() + m_select_mark_rect.left() + m_select_mark_rect.width(), 
	            rect.top() + m_select_mark_rect.top() + m_select_mark_rect.height());
	}
	
    if (isSelected()) {
		if(isFocus()) {
			status = DRAWSTATE_HILIGHT_SEL;
        } else {
            status = DRAWSTATE_SELECTED;
        }
		m_drset->draw(context, DR_CHECK, status, rect);
    }
    else if(isFocus()){
        status = DRAWSTATE_HILIGHT;
		m_drset->draw(context, DR_CHECK, status, rect);
    }else {
        status = DRAWSTATE_NORMAL;
		m_drset->draw(context, DR_CHECK, status, rect);
    }
}

void ItemView::draw_hilite(GraphicsContext* context, IntRect &rc, int status)
{
	if(!(status & PAINT_NO_HILIGHT))
	{
		status &= PAINT_STATUS_MASK;
		int hilight_state = getHilightDrawState();
		if(hilight_state >= 0) {
			if(m_drset) {
				IntRect _rc = rc;
					if(m_margenWidth > 0)
					{
						if (IsAllItemViewNeedConvert())
						{
							_rc.inset(m_margenWidth, 0, 0, 0);
						}
						else
						{
							_rc.inset(0, 0, m_margenWidth, 0);
						}
					}
				m_drset->draw(context,  DR_HILIGHT, hilight_state, _rc);
			}
		}
	}
}

int ItemView::getHilightDrawState() {
	bool bfocused = isFocus();
	if(bfocused)
		return DRAWSTATE_HILIGHT;
	/*	
	if(isHilight())
	{
		return DRAWSTATE_HILIGHT;
	}
	*/
	else if(isSelected())
	{
		return DRAWSTATE_SELECTED;
	}
	return -1;
}



void ItemView::setItemHeight(int h)
{
	IntRect rect = (IntRect)getRect();
	if(h <= 5)
		h = 5;
	setRect(rect.left(), rect.top(), rect.right(), rect.top() + h);
	updateView();
}

DEFINE_CLASS_NAME(ItemView)

NAMESPACE_END
