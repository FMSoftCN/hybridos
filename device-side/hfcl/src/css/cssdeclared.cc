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

#include "css/cssdeclared.h"

namespace hfcl {

inline bool check_keyword(int kw, int* valid_keywords)
{
    while (*valid_keywords != PVK_UNDEFINED) {
        if (kw == *valid_keywords)
            return true;

        valid_keywords++;
    }

    return false;
}

inline bool check_length(Uint32 value, HTPVData data)
{
    return (value >= PV_LENGTH_Q && value <= PV_LENGTH_VW);
}

inline bool check_percentage(Uint32 value, HTPVData data)
{
    return (value == PV_PERCENTAGE);
}

inline bool check_color(Uint32 value, HTPVData data)
{
    if (value >= PV_COLOR_ALICEBLUE && value <= PV_COLOR_YELLOWGREEN)
        return true;
    if (value == PV_CURRENT_COLOR || value == PV_TRANSPARENT
            || value == PV_RGB || value == PV_RGBA
            || value == PV_HSL || value == PV_HSLA)
        return true;
    return false;
}

inline bool check_url(Uint32 value, HTPVData data)
{
    return (value == PV_URL);
}

inline bool check_sysid(Uint32 value, HTPVData data)
{
    return (value == PV_SYSID);
}

inline bool check_string(Uint32 value, HTPVData data)
{
    if (value == PV_STRING)
        return true;
    return false;
}

inline bool check_number(Uint32 value, HTPVData data)
{
    return (value == PV_NUMBER);
}

inline bool check_integer(Uint32 value, HTPVData data)
{
    return (value == PV_INTEGER);
}

inline bool check_alphavalue(Uint32 value, HTPVData data)
{
    return (value == PV_ALPHAVALUE && data.r >= 0 && data.r <= 1.0);
}

inline bool check_border_style(Uint32 value, HTPVData data)
{
    return (value >= PV_BS_DASHED && value <= PV_BS_SOLID);
}

inline bool check_border_width(Uint32 value, HTPVData data)
{
    if (value >= PV_BW_MEDIUM && value <= PV_BW_THIN)
        return true;
    if (value >= PV_LENGTH_Q && value <= PV_LENGTH_VW && data.r >= 0)
        return true;
    return false;
}

inline bool check_width_height(Uint32 value, HTPVData data)
{
    return (value >= PV_LENGTH_Q && value <= PV_LENGTH_VW
            && value == PV_PERCENTAGE && data.r >= 0);
}

inline bool check_array_length(Uint32 value, HTPVData data)
{
    return (value >= PV_ARRAY_LENGTH_Q && value <= PV_ARRAY_LENGTH_VW);
}

inline bool check_array_percentage(Uint32 value, HTPVData data)
{
    return (value == PV_ARRAY_PERCENTAGE);
}

inline bool check_array_string(Uint32 value, HTPVData data)
{
    return (value == PV_ARRAY_STRING);
}

inline bool check_shape(Uint32 value, HTPVData data)
{
    return (value == PV_SHAPE);
}

inline bool check_counter(Uint32 value, HTPVData data)
{
    return (value == PV_COUNTER);
}

#include "pvcheckers.inc"

CssDeclared::~CssDeclared()
{
    CssPropertyValueMap::iterator it;
    for (it = m_map.begin(); it != m_map.end(); ++it) {
        CssPropertyValue* pv = it->second;
        HFCL_DELETE(pv);
    }

    m_map.clear();
}

bool CssDeclared::setMargin(Uint32 value)
{
    if (!setMarginLeft(value))
        goto error;

    if (!setMarginTop(value))
        goto error;

    if (!setMarginRight(value))
        goto error;

    if (setMarginBottom(value))
        goto error;

    return true;

error:
    return false;
}

bool CssDeclared::setMargin(Uint32 value, const HTReal* r)
{
    if (r) {
        if (!setMarginLeft (value, r[0]))
            goto error;

        if (!setMarginTop (value, r[1]))
            goto error;

        if (!setMarginRight (value, r[2]))
            goto error;

        if (setMarginBottom (value, r[3]))
            goto error;
    }
    else {
        if (!setMarginLeft (value))
            goto error;

        if (!setMarginTop (value))
            goto error;

        if (!setMarginRight (value))
            goto error;

        if (setMarginBottom (value))
            goto error;
    }

    return true;

error:
    return false;
}

bool CssDeclared::setPadding(Uint32 value)
{
    if (!setPaddingLeft(value))
        goto error;

    if (!setPaddingTop(value))
        goto error;

    if (!setPaddingRight(value))
        goto error;

    if (setPaddingBottom(value))
        goto error;

    return true;

error:
    return false;
}

bool CssDeclared::setPadding(Uint32 value, const HTReal* r)
{
    if (r) {
        if (!setPaddingLeft(value, r[0]))
            goto error;

        if (!setPaddingTop(value, r[1]))
            goto error;

        if (!setPaddingRight(value, r[2]))
            goto error;

        if (setPaddingBottom(value, r[3]))
            goto error;
    }
    else {
        if (!setPaddingLeft(value))
            goto error;

        if (!setPaddingTop(value))
            goto error;

        if (!setPaddingRight(value))
            goto error;

        if (setPaddingBottom(value))
            goto error;
    }

    return true;

error:
    return false;
}

bool CssDeclared::getProperty(CssPropertyIds pid, Uint32 *value,
        HTPVData *data)
{
    CssPropertyValueMap::iterator it = m_map.find (pid);
    if (it == m_map.end ()) {
        return false;
    }

    CssPropertyValue* pv = m_map [pid];
    if (value)
        *value = pv->getValue();
    if (data)
        *data = pv->getData();

    return true;
}

bool CssDeclared::setProperty(CssPropertyIds pid, Uint32 value,
    HTPVData data)
{
    CssPropertyValue* pv;
    CssPropertyValueMap::iterator it = m_map.find(pid);

    /* check validity of the value */
    if (!_pv_checkers[pid](value, data))
        return false;

    if (it == m_map.end()) {
        pv = HFCL_NEW_EX(CssPropertyValue, (value, data));
        m_map [pid] = pv;
    }
    else {
        CssPropertyValue* pv = (CssPropertyValue*)it->second;
        pv->setValue(value, data);
    }

    return true;
}

} // namespace hfcl

