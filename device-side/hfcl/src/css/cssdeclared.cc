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

CssDeclared::~CssDeclared ()
{
    CssPropertyValueMap::iterator it;
    for (it = m_map.begin(); it != m_map.end(); ++it) {
        CssPropertyValue* pv = it->second;
        HFCL_DELETE(pv);
    }

    m_map.clear ();
}

bool CssDeclared::setMargin (DWORD32 value, HTData addData)
{
    if (addData) {

        HTReal* data = (HTReal*)addData;
        if (!setMarginLeft (value, data[0]))
            goto error;

        if (!setMarginTop (value, data[1]))
            goto error;

        if (!setMarginRight (value, data[2]))
            goto error;

        if (setMarginBottom (value, data[3]))
            goto error;
    }
    else {
        if (!setMarginLeft (value, 0))
            goto error;

        if (!setMarginTop (value, 0))
            goto error;

        if (!setMarginRight (value, 0))
            goto error;

        if (setMarginBottom (value, 0))
            goto error;
    }

    return true;

error:
    return false;
}

bool CssDeclared::setPadding (DWORD32 value, HTData addData)
{
    if (addData) {

        HTReal* data = (HTReal*)addData;
        if (!setPaddingLeft (value, data[0]))
            goto error;

        if (!setPaddingTop (value, data[1]))
            goto error;

        if (!setPaddingRight (value, data[2]))
            goto error;

        if (setPaddingBottom (value, data[3]))
            goto error;
    }
    else {
        if (!setPaddingLeft (value, 0))
            goto error;

        if (!setPaddingTop (value, 0))
            goto error;

        if (!setPaddingRight (value, 0))
            goto error;

        if (setPaddingBottom (value, 0))
            goto error;
    }

    return true;

error:
    return false;
}

bool CssDeclared::getProperty (CssPropertyIds pid, DWORD32 *value,
        HTData *data)
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

bool CssDeclared::setProperty (CssPropertyIds pid, DWORD32 value,
    HTData data)
{
    CssPropertyValue* pv;
    CssPropertyValueMap::iterator it = m_map.find(pid);

    /* TODO: check validity of the value */

    if (it == m_map.end()) {
        pv = HFCL_NEW_EX(CssPropertyValue, (value, data));
        m_map [pid] = pv;
        return true;
    }
    else {
        CssPropertyValue* pv = (CssPropertyValue*)it->second;
        pv->setValue(value, data);
    }

    return false;
}

} // namespace hfcl

