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
    reset();
}

CssComputed::CssComputed(const CssComputed& init)
{
    memcpy(&m_values, init.m_values, sizeof(m_values));
    memcpy(&m_data, init.m_data, sizeof(m_data));
}

void CssComputed::reset()
{
    CssInitial* initial = CssInitial::getSingleton();
    memcpy(&m_values, initial->m_values, sizeof(m_values));
    memcpy(&m_data, initial->m_values, sizeof(m_data));
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
            m_data[i].r = (viewport.width()/100.0);
            break;
        case PVT_LENGTH_VH:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (viewport.height()/100.0);
            break;
        case PVT_LENGTH_VMAX:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (viewport.maxWH()/100.0);
            break;
        case PVT_LENGTH_VMIN:
            m_values[i] = PV_LENGTH_PX;
            m_data[i].r = (viewport.minWH()/100.0);
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
            break;
        case PVT_ARRAY_LENGTH_MM:
            break;
        case PVT_ARRAY_LENGTH_IN:
            break;
        case PVT_ARRAY_LENGTH_PC:
            break;
        case PVT_ARRAY_LENGTH_PT:
            break;
        case PVT_ARRAY_LENGTH_Q:
            break;

        /* viewport-relative lengths */
        case PVT_ARRAY_LENGTH_VW:
            break;
        case PVT_ARRAY_LENGTH_VH:
            break;
        case PVT_ARRAY_LENGTH_VMAX:
            break;
        case PVT_ARRAY_LENGTH_VMIN:
            break;

        /* font-relative lengths */
        case PVT_ARRAY_LENGTH_EM:
            break;
        case PVT_ARRAY_LENGTH_EX:
            break;
        case PVT_ARRAY_LENGTH_CH:
            break;
        case PVT_ARRAY_LENGTH_REM:
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

