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


#include "multifieldeditor.h"
#include "datetime.h"

extern const unsigned char daysInMonth[12];

#ifdef __CALENDAR_ARABIC__
extern const unsigned char hijri_daysInMonth[12];
extern const unsigned char hijri_1421_1440_DaysMonth[20][12];
extern const unsigned char shamsi_daysInMonth[12];
#endif

NAMESPACE_BEGIN

#define TM_STARTYEAR            2000
#define TM_ENDYEAR              2127

#define YYYYMMDD  1

MultiFieldEditor::MultiFieldEditor(View * p_parent, DrawableSet* drset)
    : FormatEditView(p_parent, drset)
{
}

MultiFieldEditor::MultiFieldEditor(View * p_parent)
    : FormatEditView(p_parent, DEFAULT_VIEW_DRAWABLESET(MultiFieldEditor))
{
}

MultiFieldEditor::MultiFieldEditor(NGInt i_id, NGInt x, NGInt y, NGInt w, NGInt h)
    : FormatEditView(i_id, x, y, w, h)
{
}

MultiFieldEditor::~MultiFieldEditor()
{
}

void MultiFieldEditor::multiFieldEditorInit(NGInt field_count, NGCPStr* p_content_array, 
                NGCPStr* p_formator_array, NGChar* p_content_max_nr, NGInt* p_value_range, FIELD_FORMAT field_format, NGInt date_system
                )
{
    NGInt i = 0;
    NGInt* p = 0;
    const char *content[] =
    {
        "01",
        "01",
        "1987"
    };

    const char *formator[] =
    {
        "/",
        "/",
        ""
    };

    NGChar max_char[] =
    {
        2,
        2,
        4
    };
    
    NGInt range[] = 
    {
        1, 12,
        1, 31,
        TM_STARTYEAR, TM_ENDYEAR
    };

    // FormatEditView init
    initEditor(field_count, p_content_array?p_content_array:content, 
            p_formator_array?p_formator_array:formator, 
            p_content_max_nr?p_content_max_nr:max_char
            );

    m_field_format = field_format;

    if(p_content_max_nr[0] == 4)
        m_dateFormat = YYYYMMDD;
    else
        m_dateFormat = 0;

    m_dateSystem = date_system;

    // range
    if (p_value_range)
        p = p_value_range;
    else
        p = range;

    for (i = 0; i < field_count; ++i) {
        m_min[i] = p[i*2];
        m_max[i] = p[i*2+1];
    }
}

void MultiFieldEditor::refresh(void)
{
    reLayout();
}

NGBool MultiFieldEditor::dispatchEvent(Event* event)
{
    return FormatEditView::dispatchEvent(event);
}

NGBool MultiFieldEditor::onChangeField(void)
{ 
    return true;
}

NGBool MultiFieldEditor::setRange(NGInt index, NGInt min, NGInt max)
{
    if (index < 0 || index > m_field_count)
        return false;

    m_max[index] = max;
    m_min[index] = min;

    return true;
}

NGInt MultiFieldEditor::getValue(NGInt index)
{
    if (index < 0 || index > m_field_count)
        return false;

    return atoi(m_field_content[index].c_str());
}

NGBool MultiFieldEditor::setValue(NGInt index, NGInt value)
{
    char buf[64];
    NGInt tmp = 0;
    if (index < 0 || index > m_field_count)
        return false;

    sprintf(buf, "%d", value);
    m_field_content[index] = buf;
    tmp = m_current_index;
    m_current_index = index;
    addZero();
    m_current_index = tmp;
    return true;
}

void MultiFieldEditor::setTextAlign(NGUInt align)
{
    if (m_drset->setDrawableElement(DR_CONTENT, SYS_SE_TEXTALIGN, align))
        updateView();
}

void MultiFieldEditor::checkDays(void)
{
    NGInt day = 0;
    NGInt month = 0;
    NGInt year = 0;
    NGInt temp = 0;

    if(m_dateFormat == YYYYMMDD)
    {
         day = atoi(m_field_content[2].c_str());
         month = atoi(m_field_content[1].c_str());
         year = atoi(m_field_content[0].c_str());
    }
    else
    {
         day = atoi(m_field_content[0].c_str());
         month = atoi(m_field_content[1].c_str());
         year = atoi(m_field_content[2].c_str());
    }

    temp = day;

#ifdef __CALENDAR_ARABIC__
    if(m_dateSystem == DATE_HIJRI)
    {
        if(month != 12 && day >= 29){
            if(year >=1421 && year <= 1440)
            {
                if(day > hijri_1421_1440_DaysMonth[year-1421][month - 1])
                    day = hijri_1421_1440_DaysMonth[year-1421][month - 1];
            }
            else
            {
                if(day > hijri_daysInMonth[month - 1])
                    day = hijri_daysInMonth[month - 1];
            }
        }
        else if(month == 12 && day == 30){
            if(!Date::IsHijriLeapYear(year))
                day = 29;
        }
        else if(month == 12 && day >= 31){
            if(Date::IsHijriLeapYear(year))
                day = 30;
            else
                day = 29;
        }
    }
    else if(m_dateSystem == DATE_SHAMSI)
    {
        if(day <= 0)
            day = 1;
        else if(month != 12 && day >= 29){
            if(day > shamsi_daysInMonth[month - 1])
                day = shamsi_daysInMonth[month - 1];
        }
        else if(month == 12 && day == 30){
            if(!Date::IsShamsiLeapYear(year))
                day = 29;
        }
        else if(month == 12 && day >= 31){
            if(Date::IsShamsiLeapYear(year))
                day = 30;
            else
                day = 29;
        }
    }
    else
#endif
    {
        if(month != 2 && day >= 30){
            if(day > daysInMonth[month - 1])
                day = daysInMonth[month - 1];
        }
        else if(month == 2 && day == 29){
            if (!((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
                day = 28;
        }
        else if(month == 2 && day >= 30){
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
                day = 29;
            else
                day = 28;
        }
    }

    if(temp != day)
    {
        char buf[64];

        sprintf(buf, "%d", day);
        if(m_dateFormat == YYYYMMDD)
            m_field_content[2] = buf;
        else
            m_field_content[0] = buf;

        addZero();        
    }
}

NGBool MultiFieldEditor::checkContent(void)
{
    NGInt num = 0, tmp = 0;
    char buf[64];
    if (-1 == m_current_index)
        return false;

    if (m_field_content[m_current_index].length() == m_field_max_char[m_current_index]) {
        num = atoi(m_field_content[m_current_index].c_str());
        tmp = num;

        // check
        if (tmp > m_max[m_current_index]) {
            tmp = m_max[m_current_index];
        }
        else if (tmp < m_min[m_current_index]) {
            tmp = m_min[m_current_index];
        }
        // update
        if (num != tmp) {
            sprintf(buf, "%d", tmp);
            m_field_content[m_current_index] = buf;
            addZero();

            if(m_field_format == DATE_FORMAT)
                checkDays();

            return false;
        }

        if(m_field_format == DATE_FORMAT)
            checkDays();
    }
    return FormatEditView::checkContent();
}

DEFINE_CLASS_NAME(MultiFieldEditor)

NAMESPACE_END

