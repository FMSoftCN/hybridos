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


#include "formateditview.h"
#include "resource/respkgmanager.h"

extern BOOL IsArabicSet(void);

namespace hfcl {

#define EDIT_FILED_WIDTH 16
#define EDIT_FIELD_GAP 5
#define EDIT_FIELD_V_MARGIN 1

FormatEditView::FormatEditView(View * p_parent, DrawableSet* drset)
    : View(p_parent, drset)
{
    init();
}

FormatEditView::FormatEditView(View * p_parent)
    : View(p_parent, DEFAULT_VIEW_DRAWABLESET(FormatEditView))
{
    init();
}

FormatEditView::FormatEditView(int i_id, int x, int y, int w, int h)
    : View(i_id, x, y, w, h)
{
    init();
}

FormatEditView::~FormatEditView()
{
}

void FormatEditView::init(void)
{
    m_field_count = 0;
    m_current_index = -1;
    m_show_current_hilight = false;
    m_auto_add_zero = true;
    m_auto_next = true;
    memset (m_field_max_char, 0, sizeof(m_field_max_char));
}

void FormatEditView::focusNext(void)
{
}

void FormatEditView::focusPrev(void)
{
}

void FormatEditView::drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/)
{
    int i = 0, current_left = 0, current_top = 0, length = 0, height = visibleHeight();
    HTResId resid = SS_AM;
    IntRect rect;

    current_left = m_margin_left;
    current_top = m_margin_top;

    if(isDisabled())
        status = DRAWSTATE_DISABLED;

    // foreach field do draw
    for (i = 0; i < m_field_count; ++i) {
        if (!m_field_content[i].compare("AM") ||!m_field_content[i].compare("PM"))
        {
            if (!m_field_content[i].compare("AM"))
                resid = SS_AM;
            else
                resid = SS_PM;
            // 1. field content
            // calc
            m_drset->calcDrawableSize(DR_CONTENT, status, length, height,
                    (DWORD)GetText(resid), DRDT_TEXT);

            // adjust rect
            rect.setRect(rc.left() + current_left,
                    rc.top() + current_top,
                    rc.left() + current_left + length,
                    rc.top() + current_top + height);

            // hilight bkg
            if (m_show_current_hilight && i == m_current_index) {
                m_drset->draw(context, DR_CONTENT2, status, rect);
                // draw hilight text
                m_drset->draw(context, DR_CONTENT, DRAWSTATE_SELECTED, rect,
                        (DWORD)GetText(resid), DRDT_TEXT);
            }
            else {
                // draw normal
                m_drset->draw(context, DR_CONTENT, status, rect,
                        (DWORD)GetText(resid), DRDT_TEXT);
            }
        }
        else
        {
            // 1. field content
            // calc
            m_drset->calcDrawableSize(DR_CONTENT, status, length, height,
                    (DWORD)m_field_content[i].c_str(), DRDT_TEXT);

            // adjust rect
            rect.setRect(rc.left() + current_left,
                    rc.top() + current_top,
                    rc.left() + current_left + length,
                    rc.top() + current_top + height);

            // hilight bkg
            if (m_show_current_hilight && i == m_current_index) {
                m_drset->draw(context, DR_CONTENT2, status, rect);
                // draw hilight text
                m_drset->draw(context, DR_CONTENT, DRAWSTATE_SELECTED, rect,
                        (DWORD)m_field_content[i].c_str(), DRDT_TEXT);
            }
            else {
                // draw normal
                m_drset->draw(context, DR_CONTENT, status, rect,
                        (DWORD)m_field_content[i].c_str(), DRDT_TEXT);
            }
        }

        // adjust current_left
        current_left += length;

        // 2. formator
        // calc
        m_drset->calcDrawableSize(DR_CONTENT, status, length, height,
                (DWORD)m_formators[i].c_str(), DRDT_TEXT);

        // adjust rect
        rect.setRect(rc.left() + current_left,
                rc.top() + current_top,
                rc.left() + current_left + length,
                rc.top() + current_top + height);

        // draw
        m_drset->draw(context, DR_CONTENT, status, rect, (DWORD)m_formators[i].c_str(), DRDT_TEXT);

        // adjust current_left
        current_left += length;
    }
}

int FormatEditView::visibleWidth(void)
{
    int len = 0, i = 0;
    int length = 0, height = 0;
    HTResId resid = SS_AM;

    for (i = 0; i < m_field_count; ++i) {

        if (!m_field_content[i].compare("AM") ||!m_field_content[i].compare("PM"))
        {
            if (!m_field_content[i].compare("AM"))
                resid = SS_AM;
            else
                resid = SS_PM;

            // 1. field content
            // calc
            if (m_field_content[i].length() > 0) {
                m_drset->calcDrawableSize(DR_CONTENT, 0, length, height,
                        (DWORD)GetText(resid), DRDT_TEXT);

                len += length;
            }
        }
        else
        {
            // 1. field content
            // calc
            if (m_field_content[i].length() > 0) {
                m_drset->calcDrawableSize(DR_CONTENT, 0, length, height,
                        (DWORD)m_field_content[i].c_str(), DRDT_TEXT);

                len += length;
            }
        }

        // 2. formator
        // calc
        if (m_formators[i].length() > 0) {
            m_drset->calcDrawableSize(DR_CONTENT, 0, length, height,
                    (DWORD)m_formators[i].c_str(), DRDT_TEXT);

            len += length;
        }
    }

    return len;
}

int FormatEditView::visibleHeight(void)
{
    // FIXME
    return getRect().height() - 2;
}

void FormatEditView::initEditor(int field_count,
        const char ** p_content_array, const char ** p_formator_array, char* p_content_max_nr)
{
    int i = 0;
    // 1. Get Data
    for (i = 0; i < field_count; ++i) {
        m_field_content[i] = p_content_array[i];
        m_formators[i] = p_formator_array[i];
        m_field_max_char[i] = p_content_max_nr[i];
    }

    m_field_count = field_count;

    reLayout();
}

void FormatEditView::addZero(void)
{
    int has_len = 0, need_len = 0;
    if (-1 == m_current_index)
        return;

    has_len = m_field_content[m_current_index].length();
    need_len = m_field_max_char[m_current_index];


    // want to add zero
    if (m_auto_add_zero) {
        // need add zero
        if (has_len < need_len) {
            // content = zeros + old_content
            m_field_content[m_current_index] = getAlignZero(need_len-has_len)+m_field_content[m_current_index];
        }
    }
}

void FormatEditView::moveToNext(void)
{
    if (m_current_index < m_field_count - 1) {
        ++m_current_index;
        updateView();
    }
}

void FormatEditView::moveToPrev(void)
{
    if (m_current_index > 0) {
        --m_current_index;
        updateView();
    }
}

void FormatEditView::nextFiled(void)
{
    if (-1 == m_current_index)
        return;

    // need add zero
    if (m_field_content[m_current_index].length() < m_field_max_char[m_current_index]) {
        addZero();
        checkContent();
    }

    moveToNext();
}

void FormatEditView::prevFiled(void)
{
    if (-1 == m_current_index)
        return;

    // need add zero
    if (m_field_content[m_current_index].length() < m_field_max_char[m_current_index]) {
        addZero();
        checkContent();
    }

    moveToPrev();
}

void FormatEditView::updateZero(void)
{
    if (-1 == m_current_index)
        return;

    // need add zero
    if (m_field_content[m_current_index].length() < m_field_max_char[m_current_index]) {
        addZero();
        checkContent();
        //updateView();
    }
}

bool FormatEditView::dispatchEvent(Event* event)
{
    switch(event->eventType())
    {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_CLICK:
            // TODO, for mouse support.
            break;
        case Event::KEY_UP:
            {
                switch(((KeyEvent *)event)->keyCode())
                {
                    case KeyEvent::KEYCODE_CURSOR_UP:
                        {
                        }
                        break;
                    case KeyEvent::KEYCODE_CURSOR_DOWN:
                        {
                        }
                        break;
                    case KeyEvent::KEYCODE_CURSOR_LEFT:
                        {
                            prevFiled();
                        }
                        break;
                    case KeyEvent::KEYCODE_CURSOR_RIGHT:
                        {
                            nextFiled();
                        }
                        break;
                        // numbers
                    case KeyEvent::KEYCODE_0:
                    case KeyEvent::KEYCODE_1:
                    case KeyEvent::KEYCODE_2:
                    case KeyEvent::KEYCODE_3:
                    case KeyEvent::KEYCODE_4:
                    case KeyEvent::KEYCODE_5:
                    case KeyEvent::KEYCODE_6:
                    case KeyEvent::KEYCODE_7:
                    case KeyEvent::KEYCODE_8:
                    case KeyEvent::KEYCODE_9:
                        {
                            processNumberKey(((KeyEvent *)event)->keyCode());
                            return STOP_DISPATCH;
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        case Event::KEY_DOWN:
            break;
        default:
            return View::dispatchEvent(event);
    }
    return GOON_DISPATCH;
}

const char * FormatEditView::getAlignZero(int count)
{
    // 20 zeros
    const char *zero = "00000000000000000000";

    if (count <= 20 && count >= 0)
        return (zero + 20 - count);
    else
        return NULL;
}

const char * FormatEditView::getInputChar(int key_code)
{
    switch(key_code)
    {
        case KeyEvent::KEYCODE_0:
            return "0";
        case KeyEvent::KEYCODE_1:
            return "1";
        case KeyEvent::KEYCODE_2:
            return "2";
        case KeyEvent::KEYCODE_3:
            return "3";
        case KeyEvent::KEYCODE_4:
            return "4";
        case KeyEvent::KEYCODE_5:
            return "5";
        case KeyEvent::KEYCODE_6:
            return "6";
        case KeyEvent::KEYCODE_7:
            return "7";
        case KeyEvent::KEYCODE_8:
            return "8";
        case KeyEvent::KEYCODE_9:
            return "9";
        default:
            return "\0";
    }

    return "\0";
}

bool FormatEditView::processNumberKey(int key_code)
{
    if (-1 == m_current_index)
        return false;

    // < means no input enough char in current field
    if (m_field_content[m_current_index].length() < m_field_max_char[m_current_index]) {
        if (onChangeField()) {
            m_field_content[m_current_index] = m_field_content[m_current_index] + getInputChar(key_code);
        }
        else
            return false;
    }
    else {
        if (onChangeField()) {
            m_field_content[m_current_index] = "";
            m_field_content[m_current_index] = getInputChar(key_code);
        }
        else
            return false;
    }

    if (checkContent() && m_auto_next)
        moveToNext();

    updateView();
    return true;
}

bool FormatEditView::checkContent(void)
{
    if (-1 == m_current_index)
        return false;

    if (m_field_content[m_current_index].length() == m_field_max_char[m_current_index]) {
        return true;
    }
    else
        return false;
}

void FormatEditView::onGotFocus()
{
    m_show_current_hilight = true;
    //if (-1 == m_current_index)

        if (getAmPos() == 0)
            m_current_index = 1;
        else
            m_current_index = 0;
}

void FormatEditView::onLostFocus()
{
    updateZero();
    m_show_current_hilight = false;
    m_current_index = -1;
}

int FormatEditView::getAmPos()
{
    int i = 0;

    for (i = 0; i < m_field_count; i++)
    {
        if (m_field_content[i].length() > 0
            && (!m_field_content[i].compare("AM") || !m_field_content[i].compare("PM")))
            return i;
    }

    return -1;
}

void FormatEditView::setAlignMargin(void)
{
    // TODO, we should support Left/Middle/Right align, for now Just Middle.
    int w = visibleWidth();
    int h = visibleHeight();
    int align = m_drset->getDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN);

#ifdef ENABLE_BIDI_LAGUAGE
    if(IsArabicSet()) {
#else
    if(false) {
#endif
        if (align == ALIGN_LEFT)
            align = ALIGN_RIGHT;
        else if (align == ALIGN_RIGHT)
            align = ALIGN_LEFT;
    }

    // Calc margin left
    if (getRect().width() <= w) {
        m_margin_left = 0;
    } else {
        if (align == ALIGN_LEFT)
            m_margin_left = 0;
        else if (align == ALIGN_RIGHT)
            m_margin_left = getRect().width() - w;
        else
            m_margin_left = (getRect().width() - w)/2;
    }

    // Calc margin top
    if (getRect().height() <= h) {
        m_margin_top = 0;
    }
    else {
        m_margin_top = (getRect().height() - h)/2;
    }

}

void FormatEditView::reLayout(void)
{
    // margin
    setAlignMargin();

    // update
    updateView();
}

void FormatEditView::setTextFont(unsigned int font)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_FONT, (HTData)GetFontRes(font)))
       updateView();
}

DEFINE_CLASS_NAME(FormatEditView)

} // namespace hfcl

