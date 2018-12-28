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

#include "css/csscomputed.h"

#include "css/cssinitial.h"
#include "view/rootview.h"

namespace hfcl {

CssComputed::CssComputed()
{
    CssInitial* initial = CssInitial::getSingleton();
    memcpy(&m_values, initial->m_values, sizeof(m_values));
    memcpy(&m_data, initial->m_values, sizeof(m_data));
}

CssComputed::CssComputed(const CssComputed& init)
{
    memcpy(&m_values, init.m_values, sizeof(m_values));
    memcpy(&m_data, init.m_data, sizeof(m_data));
}

void CssComputed::reset()
{
    freeArray();

    CssInitial* initial = CssInitial::getSingleton();
    memcpy(&m_values, initial->m_values, sizeof(m_values));
    memcpy(&m_data, initial->m_values, sizeof(m_data));
}

void CssComputed::freeArray()
{
    for (int i = 0; i < MAX_CSS_PID; i++) {
        if (CSS_PPT_VALUE_TYPE(m_values[i]) == PVT_ARRAY_LENGTH_PX &&
            IS_CSS_PPT_VALUE_ALLOCATED(m_values[i])) {
            if (m_data[i].p) {
                delete [] (HTReal*)(m_data[i].p);
                m_data[i].p = NULL;
            }

            CLEAR_CSS_PPT_VALUE_ALLOCATED(m_values[i]);
        }
    }
}

/* absolute lengths */
bool CssComputed::convertArray (int pid, int t)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = new HTReal[n];
    switch (t) {
    /* absolute lengths */
    case PVT_ARRAY_LENGTH_CM:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54);
        }
        break;
    case PVT_ARRAY_LENGTH_IN:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0);
        }
        break;
    case PVT_ARRAY_LENGTH_MM:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54/10.0);
        }
        break;
    case PVT_ARRAY_LENGTH_PC:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/6.0);
        }
        break;
    case PVT_ARRAY_LENGTH_PT:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/72.0);
        }
        break;
    case PVT_ARRAY_LENGTH_Q:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * 96.0/2.54/40);
        }
        break;
    default:
        delete [] p;
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_FLAG_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}

/* viewport-relative lengths */
bool CssComputed::convertArray (int pid, int t, const RealRect& viewport)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = new HTReal[n];
    switch (t) {
    case PVT_ARRAY_LENGTH_VW:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.width()/100.0);
        }
        break;
    case PVT_ARRAY_LENGTH_VH:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.height()/100.0);
        }
        break;
    case PVT_ARRAY_LENGTH_VMAX:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.maxWH()/100.0);
        }
        break;
    case PVT_ARRAY_LENGTH_VMIN:
        for (int i = 0; i < n; i++) {
            p[i] = (old[i] * viewport.minWH()/100.0);
        }
        break;
    default:
        delete [] p;
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_FLAG_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}

/* font-relative lengths */
bool CssComputed::convertArray (int pid, int t, const View& view)
{
    CssInitial* initial = CssInitial::getSingleton();
    Uint8 n = initial->m_arraysize[pid];
    const HTReal* old = (const HTReal*)m_data[pid].p;
    if (n == 0 || old == NULL)
        return false;

    HTReal* p = new HTReal[n];
    switch(t) {
    case PVT_ARRAY_LENGTH_EM:
        for (int i = 0; i < n; i++) {
        }
    case PVT_ARRAY_LENGTH_EX:
        for (int i = 0; i < n; i++) {
        }
    case PVT_ARRAY_LENGTH_CH:
        for (int i = 0; i < n; i++) {
        }
    case PVT_ARRAY_LENGTH_REM:
        for (int i = 0; i < n; i++) {
        }
    default:
        delete [] p;
        return false;
    }

    m_values[pid] = CSS_PPT_VALUE_FLAG_ALLOCATED | PV_ARRAY_LENGTH_PX;
    m_data[pid].p = p;
    return true;
}
/*
 * A specified value can be either absolute (i.e., not relative to
 * another value, as in red or 2mm) or relative (i.e., relative to
 * another value, as in auto, 2em). Computing a relative value
 * generally absolutizes it:
 *  - values with relative units (em, ex, vh, vw) must be made absolute
 *    by multiplying with the appropriate reference size.
 *  - certain keywords (e.g., smaller, bolder) must be replaced according to
 *    their definitions.
 *  - percentages on some properties must be multiplied by a reference value
 *    (defined by the property).
 *  - valid relative URLs must be resolved to become absolute.
 *
 * REF: https://www.w3.org/TR/css-cascade-3/
 */
bool CssComputed::makeAbsolute(const View& view)
{
    const RootView* root = view.getRoot();
    /* viewport should be defined in px */
    const RealRect& viewport = root->viewport();

    /* handle special properties here: e.g., font-size */

    /* handle length and percentage values */
    for (int i = 0; i < MAX_CSS_PID; i++) {

        Uint32 type = CSS_PPT_VALUE_TYPE(m_values[i]);
        switch (type) {
        case PVT_PERCENTAGE:
            break;

        /* absolute lengths */
        case PVT_LENGTH_CM:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54);
            break;
        case PVT_LENGTH_IN:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0);
            break;
        case PVT_LENGTH_MM:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54/10.0);
            break;
        case PVT_LENGTH_PC:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/6.0);
            break;
        case PVT_LENGTH_PT:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/72.0);
            break;
        case PVT_LENGTH_Q:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * 96.0/2.54/40);
            break;

        /* viewport-relative lengths */
        case PVT_LENGTH_VW:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.width()/100.0);
            break;
        case PVT_LENGTH_VH:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.height()/100.0);
            break;
        case PVT_LENGTH_VMAX:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.maxWH()/100.0);
            break;
        case PVT_LENGTH_VMIN:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (m_data[i].r * viewport.minWH()/100.0);
            break;

        /* font-relative lengths */
        case PVT_LENGTH_CH:
            break;
        case PVT_LENGTH_EM:
            break;
        case PVT_LENGTH_EX:
            break;
        case PVT_LENGTH_REM:
            break;

        /* percentages */
        case PVT_ARRAY_PERCENTAGE:
            break;

        /* absolute lengths */
        case PVT_ARRAY_LENGTH_CM:
        case PVT_ARRAY_LENGTH_MM:
        case PVT_ARRAY_LENGTH_IN:
        case PVT_ARRAY_LENGTH_PC:
        case PVT_ARRAY_LENGTH_PT:
        case PVT_ARRAY_LENGTH_Q:
            convertArray (i, type);
            break;

        /* viewport-relative lengths */
        case PVT_ARRAY_LENGTH_VW:
        case PVT_ARRAY_LENGTH_VH:
        case PVT_ARRAY_LENGTH_VMAX:
        case PVT_ARRAY_LENGTH_VMIN:
            convertArray (i, type, viewport);
            break;

        /* font-relative lengths */
        case PVT_ARRAY_LENGTH_EM:
        case PVT_ARRAY_LENGTH_EX:
        case PVT_ARRAY_LENGTH_CH:
        case PVT_ARRAY_LENGTH_REM:
            convertArray (i, type, view);
            break;

        case PVT_ARRAY_LENGTH_PX:
        case PVT_LENGTH_PX:
        default:
            break;
        }
    }

    return true;
}

bool CssComputed::getProperty(CssPropertyIds pid, Uint32 *value,
        HTPVData *data)
{
    if (value) {
        *value = m_values[pid];
    }
    if (data) {
        *data = m_data[pid];
    }
    return true;
}

bool CssComputed::setProperty (CssPropertyIds pid, Uint32 value,
    HTPVData data)
{
    m_values[pid] = value;
    m_data[pid] = data;
    return true;
}

} // namespace hfcl

