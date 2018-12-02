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


#include "timeeditor.h"

extern BOOL IsArabicSet(void);

NAMESPACE_BEGIN

#define FIELD0 0
#define FIELD1 1
#define FIELD2 2

TimeEditor::TimeEditor(View * p_parent, DrawableSet* drset)
    : FormatEditView(p_parent, drset)
{
    timeEditorInit();
}

TimeEditor::TimeEditor(View * p_parent)
    : FormatEditView(p_parent, DEFAULT_VIEW_DRAWABLESET(TimeEditor))
{
    timeEditorInit();
}

TimeEditor::TimeEditor(NGInt i_id, NGInt x, NGInt y, NGInt w, NGInt h)
    : FormatEditView(i_id, x, y, w, h)
{
    timeEditorInit();
}

TimeEditor::~TimeEditor()
{
}

void TimeEditor::timeEditorInit(void)
{
    const char *content[] =
    {
        "00",
        "00",
        "AM"
    };

    const char *arbcontent[] =
    {
        "AM",
        "00",
        "00"
    };

    const char *formator[] =
    {
        ":",
        " ",
        ""
    };

    const char *arbformator[] =
    {
        " ",
        ":",
        ""
    };

    NGChar max_char[] =
    {
        2,
        2,
        2
    };

    m_format = F12HOUR;
    m_max_h = 12;
    m_min_h = 1;
    m_max_m = 59;
    m_min_m = 0;

#ifdef ENABLE_BIDI_LANGUAGE
    if (IsArabicSet() && m_format == F12HOUR)
#else
    if (false && m_format == F12HOUR)
#endif
    { 
        m_field_ap = FIELD0;
        m_field_h = FIELD1;
        m_field_m = FIELD2;
        
        initEditor(3, arbcontent, arbformator, max_char); 
    }
    else
    {
        m_field_h = FIELD0;
        m_field_m = FIELD1;
        m_field_ap = FIELD2;
        
        initEditor(3, content, formator, max_char); 
    }
}

void TimeEditor::setHour(NGInt h)
{
    char buf[64];
    NGInt tmp = m_current_index;
    sprintf(buf, "%02d", h);

    m_field_content[m_field_h] = buf;

    m_current_index = m_field_h;
    checkContent();
    m_current_index = tmp;
}

NGInt TimeEditor::hour(void)
{
    return atoi(m_field_content[m_field_h].c_str());
}

void TimeEditor::setMinute(NGInt m)
{
    char buf[64];
    NGInt tmp = m_current_index;
    sprintf(buf, "%02d", m);

    m_field_content[m_field_m] = buf;

    m_current_index = m_field_m;
    checkContent();
    m_current_index = tmp;
}

NGInt TimeEditor::minute(void)
{
    return atoi(m_field_content[m_field_m].c_str());
}

void TimeEditor::setAm(NGBool am)
{
    if (am) {
        m_field_content[m_field_ap] = "AM";
//        m_max_h = 12;
//        m_min_h = 1;
//        m_max_m = 59;
//        m_min_m = 0;
    }
    else {
        m_field_content[m_field_ap] = "PM";
//        m_max_h = 12;
//        m_min_h = 1;
//        m_max_m = 59;
//        m_min_m = 0;
    }
}

NGBool TimeEditor::isAm(void)
{
    if (m_field_content[m_field_ap].length() > 1
            && m_field_content[m_field_ap].c_str()[0] == 'A') {
        return true;
    }
    return false;
}

void TimeEditor::setTimeFormat(TimeFormat format)
{ 
    NGInt num = 0;
    char buf[64];
    int i;

    if (m_format == format) {
        refresh();
        return;
    }

    m_format = format;
    // change from F24HOUR
    if (F12HOUR == format) {
        m_max_h = 12;
        m_min_h = 1;
        m_max_m = 59;
        m_min_m = 0;

        m_field_count = 3;
        
        num = atoi(m_field_content[m_field_h].c_str());

        // pm
        if (num >= 12) {
            m_field_content[m_field_ap] = "PM";
            if (12 != num) {
                sprintf(buf, "%02d", num - 12);
                m_field_content[m_field_h] = buf;
            }
        }
        else {
            m_field_content[m_field_ap] = "AM";
            if (0 == num) {
                sprintf(buf, "%02d", 12);
                m_field_content[m_field_h] = buf;
            }
        } 

#ifdef ENABLE_BIDI_LANGUAGE
        if (IsArabicSet())
#else
        if (false)
#endif
        {
            string content[3];
            string temp;
    
            content[FIELD0] = m_field_content[m_field_ap];
            content[FIELD1] = m_field_content[m_field_h];
            content[FIELD2] = m_field_content[m_field_m];

            m_field_ap = FIELD0;
            m_field_h = FIELD1;
            m_field_m = FIELD2;
            
            for (i = 0; i < 3; ++i) {
                m_field_content[i] = content[i];
            }

            temp = m_formators[FIELD0];
            m_formators[FIELD0] = m_formators[FIELD1];
            m_formators[FIELD1] = temp;
        }
                    
    }
    // change from F12HOUR
    else {
        m_max_h = 23;
        m_min_h = 0;
        m_max_m = 59;
        m_min_m = 0;

        m_field_count = 2;

        num = atoi(m_field_content[m_field_h].c_str());

        // pm
        if (m_field_content[m_field_ap].c_str()[0] == 'P') {
            if (12 != num) {
                sprintf(buf, "%02d", num + 12);
                m_field_content[m_field_h] = buf;
            }
        }
        // am : do nothing.
        else {
            if (12 == num) {
                sprintf(buf, "%02d", 0);
                m_field_content[m_field_h] = buf;
            }
        }

#ifdef ENABLE_BIDI_LANGUAGE
        if (IsArabicSet())
#else
        if (false)
#endif
        {
            string content[3];
            string temp;

            content[FIELD0] = m_field_content[m_field_h];
            content[FIELD1] = m_field_content[m_field_m];
            content[FIELD2] = m_field_content[m_field_ap];

            m_field_h = FIELD0;
            m_field_m = FIELD1;
            m_field_ap = FIELD2;

            for (i = 0; i < 3; ++i) {
                m_field_content[i] = content[i];
            }

            temp = m_formators[FIELD0];
            m_formators[FIELD0] = m_formators[FIELD1];
            m_formators[FIELD1] = temp;
        }
        
    }

    refresh(); 
}

void TimeEditor::refresh(void)
{
    reLayout();
}

NGBool TimeEditor::dispatchEvent(Event* event)
{
    // 2 means am/pm field
    if (m_current_index == m_field_ap
            && event->eventType() == Event::KEY_UP 
            && ((KeyEvent*)event)->keyCode() != KeyEvent::KEYCODE_LEFT
            && ((KeyEvent*)event)->keyCode() != KeyEvent::KEYCODE_RIGHT) {

        if (m_field_content[m_current_index].c_str()[0] == 'A') {
            m_field_content[m_current_index] = "PM";
            m_max_h = 12;
            m_min_h = 1;
        }
        else {
            m_field_content[m_current_index] = "AM";
            m_max_h = 12;
            m_min_h = 1;
        }

        setAlignMargin();
        updateView();

        return DISPATCH_STOP_MSG;
    }
    else
        return FormatEditView::dispatchEvent(event);
}

NGBool TimeEditor::onChangeField(void)
{ 
    return true;
}

NGBool TimeEditor::checkContent(void)
{
    NGInt num = 0, tmp = 0;
    char buf[64];
    
    if (-1 == m_current_index || m_field_ap == m_current_index)
        return false;

    if (m_field_content[m_current_index].length() == m_field_max_char[m_current_index]) {
        num = atoi(m_field_content[m_current_index].c_str());
        tmp = num;

        // hour
        if (m_field_h == m_current_index) {
            if (tmp > m_max_h) {
                tmp = m_max_h;
            }
            else if (tmp < m_min_h) {
                if (F12HOUR == m_format)
                    tmp = m_max_h;
                else
                    tmp = m_min_h;
            }
        }
        // minute
        else if (m_field_m == m_current_index) {
            if (tmp > m_max_m) {
                tmp = m_max_m;
            }
            else if (tmp < m_min_m) {
                tmp = m_min_m;
            }
        }
        // update
        if (num != tmp) {
            sprintf(buf, "%02d", tmp);
            m_field_content[m_current_index] = buf;

            return false;
        }
    }

    return FormatEditView::checkContent();
}

DEFINE_CLASS_NAME(TimeEditor)

NAMESPACE_END

