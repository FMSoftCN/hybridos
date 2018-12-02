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


#include "textmode.h"
#include "respkgmanager.h"
#include "menubarview.h"
NAMESPACE_BEGIN

#define MENUBAR_MARGIN  0
#define MENUBAR_BORDER_LEN 16

MenuBarView::MenuBarView(View* p_parent) 
	: PanelView(p_parent, DEFAULT_VIEW_DRAWABLESET(MenuBarView))
	, m_left_strid(-1)
	, m_middle_strid(-1)
	, m_right_strid(-1)
	, m_left_image(NULL)
	, m_middle_image(NULL)
	, m_right_image(NULL)
	, m_margin (MENUBAR_MARGIN)
	, m_border_len(MENUBAR_BORDER_LEN)
{

}

MenuBarView::MenuBarView(View* p_parent, DrawableSet* drset) 
    : PanelView(p_parent, drset)
    , m_left_image(NULL)
    , m_middle_image(NULL)
    , m_right_image(NULL)
    , m_margin (MENUBAR_MARGIN)
    , m_border_len(MENUBAR_BORDER_LEN)
{
}

MenuBarView::~MenuBarView()
{
	if(m_left_image) {
		NGUX_DELETE(m_left_image);
	}
	if(m_middle_image) {
		NGUX_DELETE(m_middle_image);
	}
	if(m_right_image) {
		NGUX_DELETE(m_right_image);
	}
}

void MenuBarView::setLeftText(NGInt strid)
{
    if (m_left_strid == strid)
        return;

    m_left_strid = strid;

    IntRect rc;
    updateView (calcRect (rc, SEL_LEFT));
}

void MenuBarView::setMiddleText(NGInt strid)
{
    if (m_middle_strid == strid)
        return;

    m_middle_strid = strid;

    IntRect rc;
    updateView (calcRect (rc, SEL_MIDDLE));
}

void MenuBarView::setRightText(NGInt strid)
{
    if (m_right_strid == strid)
        return;

    m_right_strid = strid;

    IntRect rc;
    updateView (calcRect (rc, SEL_RIGHT));
}

void MenuBarView::setLeftText (const char* txt)
{ 
    if (txt == NULL)
        txt = "";

    if (m_left.c_str() && strcmp (m_left.c_str(), txt) == 0)
        return;

    m_left = txt;
    m_left_strid = -1;

    IntRect rc;
    updateView (calcRect (rc, SEL_LEFT));
}

void MenuBarView::setMiddleText(const char* txt)
{ 
    if (txt == NULL)
        txt = "";

    if (m_middle.c_str() && strcmp (m_middle.c_str(), txt) == 0)
        return;

    m_middle = txt;
    m_middle_strid = -1;

    IntRect rc;
    updateView (calcRect (rc, SEL_MIDDLE));
}

void MenuBarView::setRightText(const char* txt)
{ 
    if (txt == NULL)
        txt = "";

    if (m_right.c_str() && strcmp (m_right.c_str(), txt) == 0)
        return;

    m_right = txt;
    m_right_strid = -1;

    IntRect rc;
    updateView (calcRect (rc, SEL_RIGHT));
}

void MenuBarView::drawContent(GraphicsContext* context, IntRect& rc, NGInt status)
{
	PanelView::drawContent(context, rc, status);

	if(m_left_strid>0)
		drawText(context, m_left_strid, SEL_LEFT);
	else
		drawText(context, m_left, SEL_LEFT);
	
    drawImage(context, SEL_LEFT);

    if(m_right_strid>0)
		drawText(context, m_right_strid, SEL_RIGHT);
	else
		drawText(context, m_right, SEL_RIGHT);
	
    drawImage(context, SEL_RIGHT);
	
    if(m_middle_strid>0)
		drawText(context, m_middle_strid, SEL_MIDDLE);
	else
		drawText(context, m_middle, SEL_MIDDLE);
	
    drawImage(context, SEL_MIDDLE);
}

void MenuBarView::drawText(GraphicsContext* context, const string &str, NGInt item)
{
	if(str.length() > 0) {
		IntRect rc;
		NGInt state = DRAWSTATE_NORMAL;

		calcRect(rc, item);

        if (item != SEL_MIDDLE)
            m_drset->setDrawableElement (DR_CONTENT, SYS_SE_FONT, (DWORD)GetSystemFont (SYSLOGFONT_CAPTION));
        else
            m_drset->setDrawableElement (DR_CONTENT, SYS_SE_FONT, (DWORD)GetSystemFont (SYSLOGFONT_DEFAULT));

		if (m_selItem == item) {
			state = DRAWSTATE_PUSHED; //draw hilight if need
            _DBG_PRINTF ("MenuBarView::drawText: hilight item: %d\n", item);
			m_drset->draw(context, DR_HILIGHT, state, rc);
		}

		FormatText formatText = {
			str.c_str(),
			(item - 1) | TextMode::ValignMiddle | TextMode::SingleLine | m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE),
		};

#if 0
		NGInt w, h;
		if (m_drset->calcDrawableSize(DR_CONTENT, 
				DRAWSTATE_NORMAL, w, h, (DWORD)&formatText, DRDT_FORMATTEXT))
		{
			if (item == SEL_LEFT)
				rc.setRect(rc.left(), rc.top(), rc.left() + w, rc.bottom());
			else if (item == SEL_RIGHT)
				rc.setRect(rc.right() - w, rc.top(), rc.right(), rc.bottom());
		}
#endif
		m_drset->draw(context, DR_CONTENT, state, rc, (DWORD)&formatText, DRDT_FORMATTEXT);
	}
}

void MenuBarView::drawText(GraphicsContext* context, NGInt strId, NGInt item)
{
    IntRect rc;
    NGInt state = DRAWSTATE_NORMAL;

	calcRect (rc, item);

    if (item != SEL_MIDDLE)
        m_drset->setDrawableElement (DR_CONTENT, SYS_SE_FONT, (DWORD)GetSystemFont (SYSLOGFONT_CAPTION));
    else
        m_drset->setDrawableElement (DR_CONTENT, SYS_SE_FONT, (DWORD)GetSystemFont (SYSLOGFONT_DEFAULT));

	if (m_selItem == item) {
        state = DRAWSTATE_PUSHED; //draw hilight if need
		m_drset->draw (context, DR_HILIGHT, state, rc);
    }

    FormatText formatText = {
        GetTextRes(strId),
        (item == SEL_LEFT ? TextMode::AlignLeft : (item == SEL_RIGHT ? TextMode::AlignRight : TextMode::AlignCenter)) 
        | TextMode::ValignMiddle | TextMode::SingleLine | TextMode::CharBreak 
        | m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE),
    };

    m_drset->draw (context, DR_CONTENT, state, rc, (DWORD)&formatText, DRDT_FORMATTEXT);

#if 0
	NGInt w, h;
	if (m_drset->calcDrawableSize(DR_CONTENT, DRAWSTATE_NORMAL, w, h, (DWORD)&formatText, DRDT_FORMATTEXT))
	{
		NGInt mid_img_w = 14;
		
		if (item == SEL_LEFT)
		{
			if(m_right_strid > 0) // has right text
			{
				NGInt w_tmp = 0;
				FormatText formatText_r = {
					GetTextRes(m_right_strid), 
					TextMode::AlignRight | TextMode::ValignMiddle | TextMode::SingleLine | m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE)
				};

				m_drset->calcDrawableSize(DR_CONTENT, DRAWSTATE_NORMAL, w_tmp, h, (DWORD)&formatText_r, DRDT_FORMATTEXT);
				if((NULL == m_middle_image) && (0 == m_middle_strid))
				{
					if(w > (_ngux_screen_w>>1) &&  (w + w_tmp + (m_margin*3)) > _ngux_screen_w)
						rc.setRect(rc.left(), rc.top(), ((_ngux_screen_w>>1) - (m_margin*1.5)), rc.bottom());
					else
						rc.setRect(rc.left(), rc.top(), rc.left() + w, rc.bottom());
				}
				else
				{
					if((rc.left() + w) > ((_ngux_screen_w - mid_img_w - m_margin) >> 1))	
						rc.setRect(rc.left(), rc.top(), ((_ngux_screen_w - mid_img_w - m_margin) >> 1), rc.bottom());
					else
						rc.setRect(rc.left(), rc.top(), rc.left() + w, rc.bottom());
				}
			}
			else  // no right text
			{
				if((NULL == m_middle_image) && (0 == m_middle_strid)) {
					rc.setRect(rc.left(), rc.top(), rc.left() + w, rc.bottom());
				} else {
					if (w < ((_ngux_screen_w - mid_img_w - m_margin * 3) >> 1)) {						
						rc.setRect(rc.left(), rc.top(), rc.left() + w, rc.bottom());
					} else {
						rc.setRect(rc.left(), rc.top(), ((_ngux_screen_w - mid_img_w - m_margin * 3) >> 1), rc.bottom());
					}
				}
			}	
		}
		else if (item == SEL_RIGHT)
		{
			if(m_left_strid > 0) // has left text 
			{
				NGInt w_tmp = 0;
				FormatText formatText_r = {
					GetTextRes(m_left_strid), 
					TextMode::AlignLeft | TextMode::ValignMiddle | TextMode::SingleLine | m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTOUTLINEMODE)
				};
				
				m_drset->calcDrawableSize(DR_CONTENT, DRAWSTATE_NORMAL, w_tmp, h, (DWORD)&formatText_r, DRDT_FORMATTEXT);
				if((NULL == m_middle_image) && (0 == m_middle_strid))
				{
					if(w > (_ngux_screen_w>>1) && (w + w_tmp + (m_margin*3)) > _ngux_screen_w)
						rc.setRect((_ngux_screen_w>>1)+(m_margin*1.5),rc.top(),_ngux_screen_w - m_margin,rc.bottom());
					else
						rc.setRect(rc.right() - w, rc.top(), rc.right(), rc.bottom());
				}
				else
				{
					if((_ngux_screen_w - w - m_margin) < ((_ngux_screen_w + mid_img_w + m_margin) >> 1))
						rc.setRect((_ngux_screen_w + mid_img_w + m_margin) >> 1, rc.top(), rc.right(), rc.bottom());
					else
						rc.setRect(rc.right() - w, rc.top(), rc.right(), rc.bottom());
				}
			}
			else // no left text
			{
				if((NULL == m_middle_image) && (0 == m_middle_strid)) { // no middle image and text
					rc.setRect(rc.right() - w, rc.top(), rc.right(), rc.bottom());
				} else {
					if (w < ((_ngux_screen_w - mid_img_w - m_margin * 3) >> 1)) {
						rc.setRect(rc.right() - w, rc.top(), rc.right(), rc.bottom());
					} else {
						rc.setRect(rc.right() - ((_ngux_screen_w - mid_img_w - m_margin * 3) >> 1), rc.top(), rc.right(), rc.bottom());
					}
				}
			}
		}
	}
    m_drset->draw(context, DR_CONTENT, state, rc, (DWORD)&formatText, DRDT_FORMATTEXT);
#endif
}

void MenuBarView::drawImage(GraphicsContext* context, NGInt item)
{
    IntRect rc;
	ImageFormat f;

	calcRect(rc, item);

    // m_border_len is bigger than View's height
    if (m_border_len > rc.height()) {
        m_border_len = rc.height();
    }

    if (SEL_LEFT == item && NULL != m_left_image) {
		f.drawMode = DRAWMODE_NORMAL;
		f.align = ALIGN_LEFT;
		f.valign = VALIGN_MIDDLE;
        rc.setRect(
                rc.left(), 
                rc.top() + (rc.height() - m_border_len)/2, 
                rc.left() + m_border_len, 
                rc.top() + (rc.height() - m_border_len)/2 + m_border_len);
		m_left_image->paint(context, rc,f);
    }
    else if (SEL_MIDDLE == item && NULL != m_middle_image) {
		f.drawMode = DRAWMODE_NORMAL;
		f.align = ALIGN_CENTER;
		f.valign = VALIGN_MIDDLE;
			
        rc.setRect(
                rc.left() + (rc.right() - rc.left())/2 - m_border_len/2, 
                rc.top() + (rc.height() - m_border_len)/2, 
                rc.left() + (rc.right() - rc.left())/2 + m_border_len/2, 
                rc.top() + (rc.height() - m_border_len)/2 + m_border_len);
		m_middle_image->paint(context, rc,f);
    }
    else if (SEL_RIGHT == item && NULL != m_right_image) {
		f.drawMode = DRAWMODE_NORMAL;
		f.align = ALIGN_RIGHT;
		f.valign = VALIGN_MIDDLE;
        rc.setRect(
                rc.right() - m_border_len, 
                rc.top() + (rc.height() - m_border_len)/2, 
                rc.right(), 
                rc.top() + (rc.height() - m_border_len)/2 + m_border_len);
		m_right_image->paint(context, rc,f);
    }
}

void MenuBarView::setLeftImage(Image* image)
{
	if (m_left_image == image)
		return;
	
	if (m_left_image != NULL) {
		NGUX_DELETE(m_left_image);
	}

	m_left_image = image;

    IntRect rc;
    updateView (calcRect (rc, SEL_LEFT));
}

void MenuBarView::setMiddleImage(Image* image)
{
	if (m_middle_image == image)
		return;
	
	if (m_middle_image != NULL) {
		NGUX_DELETE(m_middle_image);
	}

    IntRect rc;
    updateView (calcRect (rc, SEL_MIDDLE));
}

void MenuBarView::setRightImage(Image* image)
{
	if (m_right_image == image)
		return;
	
	if (m_right_image != NULL) {
		NGUX_DELETE(m_right_image);
	}

    IntRect rc;
    updateView (calcRect (rc, SEL_RIGHT));
}

NGBool MenuBarView::dispatchEvent(Event* event)
{
    NGInt t = event->eventType();

    if (t == Event::KEY_DOWN || t == Event::KEY_UP) {
        NGInt item = SEL_NONE;
        switch(((KeyEvent*)event)->keyCode()) {
        case KeyEvent::KEYCODE_SL:
            item = SEL_LEFT;
            break;
        case KeyEvent::KEYCODE_SR:
            item = SEL_RIGHT;
            break;
        case KeyEvent::KEYCODE_ENTER:
            item = SEL_MIDDLE;
            break;
        default:
            return DISPATCH_CONTINUE_MSG;
        }

        pressItem (item, t == Event::KEY_DOWN);

        return DISPATCH_STOP_MSG;
    }
    return DISPATCH_CONTINUE_MSG;
}


void MenuBarView::pressItem(NGInt item, NGBool bpress)
{
    IntRect rc;
    NGInt newitem = bpress ? item : SEL_NONE;

    if(newitem != m_selItem) {
        if(m_selItem != SEL_NONE && m_selItem != item) {
            updateView(calcRect(rc, m_selItem));
        }

        m_selItem = newitem;
        updateView(calcRect(rc, item));
    }
}

DEFINE_CLASS_NAME(MenuBarView)

NAMESPACE_END

