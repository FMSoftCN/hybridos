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

#include <math.h>

#include "css/cssinitial.h"
#include "view/view.h"
#include "view/rootview.h"

namespace hfcl {

#if SIZEOF_PTR == 8
inline HTReal realmax(HTReal x, HTReal y)
{
    return fmax(x, y);
}
inline HTReal realmin(HTReal x, HTReal y)
{
    return fmin(x, y);
}
#define HTHUGE_VAL HUGE_VAL
#else
inline HTReal realmax(HTReal x, HTReal y)
{
    return fmaxf(x, y);
}
inline HTReal realmin(HTReal x, HTReal y)
{
    return fminf(x, y);
}
#define HTHUGE_VAL HUGE_VALF
#endif

CssComputed::CssComputed()
{
    CssInitial* initial = CssInitial::getSingleton();
    for (int i = 0; i < MAX_CSS_PID; i++) {
        m_values[i] = CSS_PPT_VALUE_NOFLAGS(initial->m_values[i]);
    }
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
    for (int i = 0; i < MAX_CSS_PID; i++) {
        m_values[i] = CSS_PPT_VALUE_NOFLAGS(initial->m_values[i]);
    }
    memcpy(&m_data, initial->m_values, sizeof(m_data));
}

void CssComputed::inherit(const CssComputed* css)
{
    if (css == NULL)
        return;

    CssInitial* initial = CssInitial::getSingleton();
    for (int i = 0; i < MAX_CSS_PID; i++) {

        if (IS_CSS_PPT_VALUE_INHERITED(initial->m_values[i])) {
            if (CSS_PPT_VALUE_TYPE(css->m_values[i])
                    == PVT_ARRAY_LENGTH_PX &&
                IS_CSS_PPT_VALUE_ALLOCATED(css->m_values[i])) {

                Uint8 n = initial->m_arraysize[i];
                HTReal* p = new HTReal[n];
                memcpy (p, css->m_data[i].p, sizeof(HTReal)*n);

                m_values[i] = css->m_values[i];
                m_data[i].p = p;
                // MARK_CSS_PPT_VALUE_ALLOCATED(m_values[i]);
            }
            else {
                m_values[i] = css->m_values[i];
                m_data[i] = css->m_data[i];
            }
        }
        else {
            m_values[i] = initial->m_values[i];
            m_data[i] = initial->m_data[i];
        }
    }
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
bool CssComputed::convertArray (int pid, int t, const View* view)
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

void CssComputed::validateBorderWidth(int pid)
{
    if (m_values[pid] == PV_BW_THIN) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 1.0;
    }
    else if (m_values[pid] == PV_BW_MEDIUM) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 2.0;
    }
    else if (m_values[pid] == PV_BW_THICK) {
        m_values[pid] = PV_LENGTH_PX;
        m_data[pid].r = 4.0;
    }
    else if (m_values[pid] == PV_LENGTH_PX &&
            m_data[pid].r < 0.0) {
        m_data[pid].r = 0.0;
    }
    else {
        _ERR_PRINTF ("%s: invalid property value: %d\n",
            __FUNCTION__, pid);
    }
}

void CssComputed::validateNotNegative(int pid)
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[pid]);
    if ((type == PVT_PERCENTAGE || type == PVT_LENGTH_PX) &&
            m_data[pid].r < 0.0) {
        m_data[pid].r = 0.0;
    }
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
bool CssComputed::makeAbsolute(const View* view)
{
    const RootView* root = view->getRoot();
    /* viewport should be defined in px */
    const RealRect& viewport = root->viewport();

    /* handle special properties here: e.g., font-size */
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_LEFT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_LEFT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_LEFT_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_LEFT_WIDTH].r = 0.0;
    }
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_TOP_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_TOP_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_TOP_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_TOP_WIDTH].r = 0.0;
    }
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_RIGHT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_RIGHT_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_RIGHT_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_RIGHT_WIDTH].r = 0.0;
    }
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_BOTTOM_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_NONE) ||
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_BORDER_BOTTOM_STYLE]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BS_HIDDEN)) {
        m_values[PID_BORDER_BOTTOM_WIDTH] = PV_LENGTH_PX;
        m_data[PID_BORDER_BOTTOM_WIDTH].r = 0.0;
    }

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

bool CssComputed::validate(const View* view)
{
    /* make sure border widths are not negative */
    validateBorderWidth(PID_BORDER_LEFT_WIDTH);
    validateBorderWidth(PID_BORDER_TOP_WIDTH);
    validateBorderWidth(PID_BORDER_RIGHT_WIDTH);
    validateBorderWidth(PID_BORDER_BOTTOM_WIDTH);

    /* make sure paddings are not negative */
    validateNotNegative(PID_PADDING_LEFT);
    validateNotNegative(PID_PADDING_TOP);
    validateNotNegative(PID_PADDING_RIGHT);
    validateNotNegative(PID_PADDING_BOTTOM);

    /*
     * adjust display, position, and float
     * NOTE: HFCL only supports the following display values:
     *  inline block inline-block
     */
    if (view->isRoot()) {
        m_values[PID_DISPLAY] = PV_BLOCK;
    }
    else {
        if (m_values[PID_POSITION] == PV_FIXED ||
                m_values[PID_POSITION] == PV_ABSOLUTE) {
            m_values[PID_FLOAT] = PV_NONE;
            if (m_values[PID_DISPLAY] == PV_INLINE ||
                    m_values[PID_DISPLAY] == PV_INLINE_BLOCK) {
                m_values[PID_DISPLAY] = PV_BLOCK;
            }
        }
        else if (m_values[PID_FLOAT] != PV_NONE) {
            if (m_values[PID_DISPLAY] == PV_INLINE ||
                    m_values[PID_DISPLAY] == PV_INLINE_BLOCK) {
                m_values[PID_DISPLAY] = PV_BLOCK;
            }
        }
    }

    return true;
}

bool CssComputed::getProperty(CssPropertyIds pid, Uint32 *value,
        HTPVData *data) const
{
    if (value) {
        *value = CSS_PPT_VALUE_NOFLAGS(m_values[pid]);
    }
    if (data) {
        *data = m_data[pid];
    }
    return true;
}

bool CssComputed::setProperty(CssPropertyIds pid, Uint32 value,
        HTPVData data)
{
    m_values[pid] = value;
    m_data[pid] = data;
    return true;
}

/* return true for auto */
bool CssComputed::getZIndex(int& zindex) const
{
    if (m_values[PID_Z_INDEX] == PV_AUTO)
        return true;

    zindex = m_data[PID_Z_INDEX].i;
    return false;
}

void CssComputed::autoHMarginsAsZero(const CssBox* ctnBlock,
        HTReal& ml, HTReal& mr) const
{
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_MARGIN_LEFT]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        ml = 0.0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_LEFT]) ==
            PVT_LENGTH_PX) {
            ml = m_data[PID_MARGIN_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_LEFT]) ==
            PVT_PERCENTAGE) {
            ml = ctnBlock->getContentWidth() * m_data[PID_MARGIN_LEFT].r / 100.0;
        }
    }

    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_MARGIN_RIGHT]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mr = 0.0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_RIGHT]) ==
            PVT_LENGTH_PX) {
            mr = m_data[PID_MARGIN_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_RIGHT]) ==
            PVT_PERCENTAGE) {
            mr = ctnBlock->getContentWidth() * m_data[PID_MARGIN_RIGHT].r / 100.0;
        }
    }
}

void CssComputed::autoVMarginsAsZero(const CssBox* ctnBlock,
        HTReal& mt, HTReal& mb) const
{
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_MARGIN_TOP]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mt = 0.0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_TOP]) ==
            PVT_LENGTH_PX) {
            mt = m_data[PID_MARGIN_TOP].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_TOP]) ==
            PVT_PERCENTAGE) {
            mt = ctnBlock->getContentWidth() * m_data[PID_MARGIN_TOP].r / 100.0;
        }
    }

    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_MARGIN_BOTTOM]) ==
        MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        mb = 0.0;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_BOTTOM]) ==
            PVT_LENGTH_PX) {
            mb = m_data[PID_MARGIN_BOTTOM].r;
        }
        else if (CSS_PPT_VALUE_TYPE(m_values[PID_MARGIN_BOTTOM]) ==
            PVT_PERCENTAGE) {
            mb = ctnBlock->getContentWidth() * m_data[PID_MARGIN_BOTTOM].r / 100.0;
        }
    }
}

#if 0
bool CssComputed::hasComputedMarginLeft(const CssBox* ctnBlock, HTReal& ml)
{
    Uint32 v = m_values[PID_MARGIN_LEFT];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            ml = m_data[PID_MARGIN_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            ml = ctnBlock->getContentWidth() * m_data[PID_MARGIN_LEFT].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedMarginRight(const CssBox* ctnBlock, HTReal& mr)
{
    Uint32 v = m_values[PID_MARGIN_RIGHT];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            mr = m_data[PID_MARGIN_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            mr = ctnBlock->getContentWidth() * m_data[PID_MARGIN_RIGHT].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedWidth(const CssBox* ctnBlock, HTReal& w)
{
    Uint32 v = m_values[PID_WIDTH];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            w = m_data[PID_WIDTH].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            w = ctnBlock->getContentWidth() * m_data[PID_WIDTH].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedHeight(const CssBox* ctnBlock, HTReal& h)
{
    Uint32 v = m_values[PID_HEIGHT];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return false;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            h = m_data[PID_HEIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            h = ctnBlock->getContentHeight() * m_data[PID_HEIGHT].r / 100.0;
        }
    }

    return true;
}
#endif

/* return true for auto
 *
 * TODO:
 * For absolutely positioned elements whose containing block
 * is based on a block container element, the percentage is
 * calculated with respect to the width of the padding box of
 * that element. This is a change from CSS1, where the percentage
 * width was always calculated with respect to the content box
 * of the parent element.
 */
bool CssComputed::getHSize(const CssBox* ctnBlock, int pid, HTReal& s) const
{
    Uint32 v = m_values[pid];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return true;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            s = m_data[pid].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            s = ctnBlock->getContentWidth() * m_data[pid].r / 100.0;
        }
    }

    return false;
}

/* return false if min-width was illegal */
bool CssComputed::getMinWidth(const CssBox* ctnBlock, HTReal& minw) const
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_MIN_WIDTH]);

    if (type == PVT_PERCENTAGE) {
        minw = ctnBlock->getContentWidth() * m_data[PID_MIN_WIDTH].r / 100.0;
    }
    else if (type == PVT_LENGTH_PX) {
        minw = m_data[PID_MIN_WIDTH].r;
    }
    else {
        minw = 0;
        return false;
    }

    if (minw < 0) {
        minw = 0;
        return false;
    }

    return true;
}

/* return true if max-width is defined and not negative */
bool CssComputed::getMaxWidth(const CssBox* ctnBlock, HTReal& maxw) const
{
    Uint32 v = m_values[PID_MAX_WIDTH];

    maxw = HTHUGE_VAL;
    if (CSS_PPT_VALUE_NOFLAGS(v) == PV_NONE) {
        return false;
    }

    Uint32 type = CSS_PPT_VALUE_TYPE(v);
    if (type == PVT_PERCENTAGE) {
        maxw = ctnBlock->getContentWidth() * m_data[PID_MAX_WIDTH].r / 100.0;
    }
    else if (type == PVT_LENGTH_PX) {
        maxw = m_data[PID_MAX_WIDTH].r;
    }
    else {
        return false;
    }

    if (maxw < 0) {
        return false;
    }

    return true;
}

/* return false if min-height was illegal */
bool CssComputed::getMinHeight(const CssBox* ctnBlock, HTReal& minw) const
{
    Uint32 type = CSS_PPT_VALUE_TYPE(m_values[PID_MIN_HEIGHT]);

    if (type == PVT_PERCENTAGE) {
        minw = ctnBlock->getContentHeight() * m_data[PID_MIN_HEIGHT].r / 100.0;
    }
    else if (type == PVT_LENGTH_PX) {
        minw = m_data[PID_MIN_HEIGHT].r;
    }
    else {
        minw = 0;
        return false;
    }

    if (minw < 0) {
        minw = 0;
        return false;
    }

    return true;
}

/* return true if max-height is defined and not negative */
bool CssComputed::getMaxHeight(const CssBox* ctnBlock, HTReal& maxh) const
{
    Uint32 v = m_values[PID_MAX_HEIGHT];

    maxh = HTHUGE_VAL;
    if (CSS_PPT_VALUE_NOFLAGS(v) == PV_NONE) {
        return false;
    }

    Uint32 type = CSS_PPT_VALUE_TYPE(v);
    if (type == PVT_PERCENTAGE) {
        maxh = ctnBlock->getContentHeight() * m_data[PID_MAX_HEIGHT].r / 100.0;
    }
    else if (type == PVT_LENGTH_PX) {
        maxh = m_data[PID_MAX_HEIGHT].r;
    }
    else {
        return false;
    }

    if (maxh < 0) {
        return false;
    }

    return true;
}

/* return true for auto */
bool CssComputed::getVSize(const CssBox* ctnBlock, int pid, HTReal& s) const
{
    Uint32 v = m_values[pid];

    if (CSS_PPT_VALUE_NOFLAGS(v) == MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        return true;
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            s = m_data[pid].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            s = ctnBlock->getContentHeight() * m_data[pid].r / 100.0;
        }
        else
            s = 0;
    }

    return false;
}

void CssComputed::calcPaddings(const CssBox* ctnBlock,
        HTReal* pl, HTReal* pt, HTReal* pr, HTReal* pb) const
{
    if (pl) {
        Uint32 v = m_values[PID_PADDING_LEFT];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pl = m_data[PID_PADDING_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pl = ctnBlock->getContentWidth() * m_data[PID_PADDING_LEFT].r / 100.0;
        }
        else {
            *pl = 0.0;
        }

        if (*pl < 0.0)
            *pl = 0.0;
    }

    if (pt) {
        Uint32 v = m_values[PID_PADDING_TOP];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pt = m_data[PID_PADDING_TOP].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pt = ctnBlock->getContentHeight() * m_data[PID_PADDING_TOP].r / 100.0;
        }
        else {
            *pt = 0.0;
        }

        if (*pt < 0.0)
            *pt = 0.0;
    }

    if (pr) {
        Uint32 v = m_values[PID_PADDING_RIGHT];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pr = m_data[PID_PADDING_RIGHT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pr = ctnBlock->getContentWidth() * m_data[PID_PADDING_RIGHT].r / 100.0;
        }
        else {
            *pr = 0.0;
        }

        if (*pr < 0.0)
            *pr = 0.0;
    }

    if (pb) {
        Uint32 v = m_values[PID_PADDING_BOTTOM];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pb = m_data[PID_PADDING_BOTTOM].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pb = ctnBlock->getContentHeight() * m_data[PID_PADDING_BOTTOM].r / 100.0;
        }
        else {
            *pb = 0.0;
        }

        if (*pb < 0.0)
            *pb = 0.0;
    }
}

// Inline, replaced elements
void CssComputed::calcWidthForIR(const View* view, const CssBox* ctnBlock,
        HTReal& w)
{
    HTReal inw, inh, inr;
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_INTERNAL_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_INTERNAL_HEIGHT]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        if (view->getIntrinsicWidth(&inw)) {
            w = inw;
        }
        else if (view->getIntrinsicHeight(&inh) &&
                 view->getIntrinsicRatio(&inr)) {
            w = inh * inr;
        }
        else if (view->getIntrinsicRatio(&inr)) {
            w = 0.0; // undefined
        }
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_INTERNAL_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        HTReal h;
        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (!autoh && view->getIntrinsicRatio(&inr)) {
            w = h * inr;
        }
        else if (view->getIntrinsicWidth(&inw)) {
            w = inw;
        }
        else {
            w = 300.0;
            // TODO:
            // If 300px is too wide to fit the device,
            // UAs should use the width of the largest rectangle
            // that has a 2:1 ratio and fits the device instead.
        }
    }
}

// Inline, replaced elements
void CssComputed::calcHeightForIR(const View* view, const CssBox* ctnBlock,
        HTReal &w, HTReal& h)
{
    HTReal inh, inr;
    bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
    if (autoh) {
        bool autoinh = view->getIntrinsicHeight(&inh);
        bool autoinr = view->getIntrinsicRatio(&inr);
        if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_INTERNAL_WIDTH]) ==
                MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) && autoinh) {
            h = inh;
        }
        else if (autoinr) {
            h = w / inr;
        }
        else if (autoinh) {
            h = inh;
        }
        else {
            // the used value of 'height' must be set to the height of
            // the largest rectangle that has a 2:1 ratio, has a height
            // not greater than 150px, and has a width not greater than
            // the device width.
            h = w / 2.0;
            if (h > 150) {
                h = 150;
            }
        }
    }
}

void CssComputed::calcHeightForBlockVNR(const View* view,
        const CssBox* ctnBlock, CssBox* cssBox)
{
    HTReal h;

    bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
    if (autoh) {
        // TODO
        h = 0;
    }

    cssBox->setContentHeight(h);
}

void CssComputed::calcWidthForBlock(const CssBox* ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr, bool calced_w)
{
    // NOTE:
    // Computed border width must be absolute length;
    // '0' if the border style is 'none' or 'hidden'.
    HTReal blw = m_data[PID_BORDER_LEFT_WIDTH].r;
    HTReal brw = m_data[PID_BORDER_RIGHT_WIDTH].r;

    HTReal pl, pr;
    calcPaddings(ctnBlock, &pl, NULL, &pr, NULL);

    ml = mr = 0.0;
    bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
    bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);
    bool autow;
    if (calced_w) {
        /* w already determined for block and replaced element */
        autow = false;
    }
    else {
        autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
    }

    if (!autow && ((ml + blw + pl + w + pr + brw + mr) > ctnBlock->getContentWidth())) {
        if (automl) {
            ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
        }
        else if (automr) {
            mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
        }
        else {
            // over-constrained
            if (ctnBlock->getCss()->isLtr()) {
                // if direction of containing block is ltr
                mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
            }
            else {
                // if direction of containing block is rtl
                ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
            }
        }
    }
    else if (autow && !automl && !automr) {
        w = ctnBlock->getContentWidth() - ml - mr - brw - blw - pr - pl;
    }
    else if (!autow && automl && !automr) {
        ml = ctnBlock->getContentWidth() - w - mr - brw - blw - pr - pl;
    }
    else if (!autow && !automl && automr) {
        mr = ctnBlock->getContentWidth() - w - ml - brw - blw - pr - pl;
    }
    else if (autow) {
        if (automl) ml = 0.0;
        if (automr) mr = 0.0;
        w = ctnBlock->getContentWidth() - ml - mr - brw - blw - pr - pl;
    }
    else if (automl && automr) {
        ml = mr = (ctnBlock->getContentWidth() - w - brw - blw - pr - pl) / 2.0;
    }
}

void CssComputed::calcWidthsForAPNR(const View* view, const CssBox* ctnBlock,
        CssBox* box)
{
    HTReal w, ml, mr, l = 0, r = 0;
    bool autol = getHSize(ctnBlock, PID_LEFT, l);
    bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
    bool autor = getHSize(ctnBlock, PID_RIGHT, r);

    HTReal blw, brw;
    box->getHBorderWidths(blw, brw);

    HTReal pl, pr;
    box->getHPaddings(pl, pr);

    HTReal ctnw = ctnBlock->getContentWidth();

    if (autol && autow && autor) {
        autoHMarginsAsZero(ctnBlock, ml, mr);

        if (ctnBlock->getCss()->isLtr()) {
            // TODO: left is static position
            l = 0.0;

            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - l - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else {
            // TODO: right is static position
            r = 0.0;

            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr - r;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
    }
    else if (!autol && !autow && !autor) {
        bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
        bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);
        if (automl && automr) {
            ml = (ctnw - l - blw - pl - pr - brw - r) / 2.0;
            mr = ml;
            if (ml < 0) {
                if (ctnBlock->getCss()->isLtr()) {
                    ml = 0;
                    mr = ctnw - l - ml - blw - pl - pr - brw - r;
                }
                else {
                    mr = 0;
                    ml = ctnw - l - blw - pl - pr - brw - mr - r;
                }
            }
        }
        else if (automl) {
            ml = ctnw - l - blw - pl - pr - brw - mr - r;
        }
        else if (automr) {
            mr = ctnw - l - ml - blw - pl - pr - brw - r;
        }
        else {
            if (ctnBlock->getCss()->isLtr()) {
                // ignore right
                r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
            }
            else {
                // ignore left
                l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
            }
        }
    }
    else {
        autoHMarginsAsZero(ctnBlock, ml, mr);

        if (autol && autow && !autor) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr - r;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
        else if (autol && autor && !autow) {
            if (ctnBlock->getCss()->isLtr()) {
                // TODO: left is the static position
                l = 0.0;
                r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
            }
            else {
                // TODO: right is the static position
                r = 0.0;
                l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
            }
        }
        else if (autow && autor && !autol) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - l - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);

            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else if (autol && !autow && !autor) {
            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
        else if (!autol && autow && !autor) {
            w = ctnw - l - ml - blw - pl - pr - brw - mr - r;
        }
        else if (!autol && !autow && autor) {
            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
    }
}

void CssComputed::calcWidthsForAPR(const View* view, const CssBox* ctnBlock,
        CssBox* box)
{
    HTReal w, ml, mr, l = 0, r = 0;
    bool autol = getHSize(ctnBlock, PID_LEFT, l);
    bool autor = getHSize(ctnBlock, PID_RIGHT, r);
    bool automl = getHSize(ctnBlock, PID_MARGIN_LEFT, ml);
    bool automr = getHSize(ctnBlock, PID_MARGIN_RIGHT, mr);

    HTReal blw, brw;
    box->getHBorderWidths(blw, brw);

    HTReal pl, pr;
    box->getHPaddings(pl, pr);

    HTReal ctnw = ctnBlock->getContentWidth();

    calcWidthForIR(view, ctnBlock, w);

    if (automl || automr) {
        if (autol && autor) {
            if (automl) {
                ml = 0;
                automl = false;
            }
            if (automr) {
                mr = 0;
                automr = false;
            }

            if (ctnBlock->getCss()->isLtr()) {
                // TODO: left is static position
                l = 0.0;
                autol = false;
                r = ctnw - ml - blw - pl - w - pr - brw - mr;
                autor = false;
            }
            else {
                // TODO: right is static position
                r = 0.0;
                autor = false;
                l = ctnw - ml - blw - pl - w - pr - brw - mr;
                autor = false;
            }
        }
        else if (autol && automl) {
            ml = 0;
            automl = false;
        }
        else if (autor && automr) {
            mr = 0;
            automr = false;
        }
    }

    if (automl && automr) {
        ml = (ctnw - l - blw - pl - pr - brw - r) / 2.0;
        mr = ml;
        if (ml < 0) {
            if (ctnBlock->getCss()->isLtr()) {
                ml = 0;
                mr = ctnw - l - blw - pl - pr - brw - r;
            }
            else {
                mr = 0;
                ml = ctnw - l - blw - pl - pr - brw - r;
            }
        }
    }

    if (autol) {
        l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
    }
    else {
        if (ctnBlock->getCss()->isLtr()) {
            // ignore right
            r = ctnw - l - ml - blw - pl - w - pr - brw - mr;
        }
        else {
            // ignore left
            l = ctnw - ml - blw - pl - w - pr - brw - mr - r;
        }
    }
}

/*
 * Ref: https://www.w3.org/TR/CSS22/visudet.html#Computing_widths_and_margins
 * Return false for undefined width
 */
bool CssComputed::doCalcWidthsMargins(const View* view,
        const CssBox* ctnBlock,
        CssBox* cssBox)
{
    HTReal ctnw = ctnBlock->getContentWidth();

    HTReal blw = m_data[PID_BORDER_LEFT_WIDTH].r;
    HTReal brw = m_data[PID_BORDER_RIGHT_WIDTH].r;
    cssBox->setHBorderWidths(blw, brw);

    HTReal pl, pr;
    calcPaddings(ctnBlock, &pl, NULL, &pr, NULL);
    cssBox->setHPaddings(pl, pr);

    // determine width and margins
    HTReal w, ml, mr;

    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE &&
            !view->isReplaced()) {
        // inline, non-replaced elements

        w = 0.0; /* undefined so far */
        autoHMarginsAsZero(ctnBlock, ml, mr);
        cssBox->setHMargins(ml, mr);
        return false;
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE &&
            view->isReplaced()) {
        // inline, replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            !view->isReplaced()) {
        // Block-level, non-replaced elements in normal flow
        calcWidthForBlock(ctnBlock, w, ml, mr, false);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            view->isReplaced()) {
        // Block-level, replaced elements in normal flow
        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
        calcWidthForBlock(ctnBlock, w, ml, mr, true);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_FLOAT]) != PV_NONE &&
            !view->isReplaced()) {
        // Floating, non-replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
        if (autow) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);

            HTReal available = ctnw - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);
        }
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_FLOAT]) != PV_NONE &&
            view->isReplaced()) {
        // Floating, replaced elements

        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else if ((CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_FIXED) &&
            !view->isReplaced()) {
        // Absolutely positioned, non-replaced elements

        calcWidthsForAPNR(view, ctnBlock, cssBox);
    }
    else if ((CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_FIXED) &&
            view->isReplaced()) {
        // Absolutely positioned, replaced elements

        calcWidthsForAPR(view, ctnBlock, cssBox);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY])
                == PV_INLINE_BLOCK && !view->isReplaced()) {
        // 'Inline-block', non-replaced elements in normal flow

        autoHMarginsAsZero(ctnBlock, ml, mr);
        bool autow = getHSize(ctnBlock, PID_INTERNAL_WIDTH, w);
        if (autow) {
            HTReal preferred, minimum;
            view->getShrinkToFitWhidth(&preferred, &minimum);
            HTReal available =
                ctnw - ml - blw - pl - pr - brw - mr;
            w = realmax(minimum, available);
            w = realmin(w, preferred);
        }
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY])
                == PV_INLINE_BLOCK && view->isReplaced()) {
        autoHMarginsAsZero(ctnBlock, ml, mr);
        calcWidthForIR(view, ctnBlock, w);
    }
    else {
        _ERR_PRINTF("%s: why we are here?", __FUNCTION__);
        return false;
    }

    return true;
}

bool CssComputed::resolveWHForRIR(const View* view, const CssBox* ctnBlock,
        HTReal& w, HTReal& h)
{
    HTReal inw, inh, inr;

    if (view->getIntrinsicWidth(&inw)) {
        w = inw;
    }
    else if (view->getIntrinsicHeight(&inh) &&
            view->getIntrinsicRatio(&inr)) {
        w = inh * inr;
    }
    else if (view->getIntrinsicRatio(&inr)) {
        w = 0.0; // undefined
        return false;
    }

    if (view->getIntrinsicHeight(&inh)) {
        h = inh;
    }
    else if (view->getIntrinsicWidth(&inw) &&
             view->getIntrinsicRatio(&inr)) {
        h = inw / inr;
    }
    else if (view->getIntrinsicRatio(&inr)) {
        h = 0.0; // undefined
        return false;
    }

    HTReal minw, maxw, minh, maxh;
    bool valid_minw = getMinWidth(ctnBlock, minw);
    bool valid_maxw = getMaxWidth(ctnBlock, maxw);
    bool valid_minh = getMinHeight(ctnBlock, minh);
    bool valid_maxh = getMaxHeight(ctnBlock, maxh);

    maxw = realmax(minw, maxw);
    maxh = realmax(minh, maxh);

    if (valid_maxw && w > maxw && valid_maxh && h > maxh) {
        if ((maxw/w) <= (maxh/h)) {
            w = maxw;
            h = realmax(minh, (maxw*h/w));
        }
        else {
            w = realmax(minw, (maxw*w/h));
            h = maxh;
        }
    }
    else if (valid_minw && w < minw && valid_minh && h < minh) {
        if ((minw / w) <= (minh / h)) {
            w = realmin(maxw, (minh * w / h));
            h = minh;
        }
        else {
            w = minw;
            h = realmin(maxh, (minw * h / w));
        }
    }
    else if (valid_minw && w < minw && valid_maxh && h > maxh) {
        w = minw;
        h = maxh;
    }
    else if (valid_maxw && w > maxw && valid_minh && h < minh) {
        w = maxw;
        h = minh;
    }
    else if (valid_maxw && w > maxw) {
        w = maxw;
        h = realmax((maxw * h / w), minh);
    }
    else if (valid_minw && w < minw) {
        w = minw;
        h = realmin((minw * h / w), maxh);
    }
    else if (valid_maxh && h > maxh) {
        w = realmax((maxh * w / h), minw);
        h = maxh;
    }
    else if (valid_minh && h < minh) {
        w = realmin((minh * w / h), maxw);
        h = minh;
    }

    return true;
}

void CssComputed::resolveAutoHeights(const View* view,
            const CssBox* ctnBlock,
            CssBox* cssBox)
{
    // TODO
}

/*
 * Ref: https://www.w3.org/TR/CSS22/visudet.html#Computing_heights_and_margins
 * Return false for undefined height
 */
bool CssComputed::doCalcHeightsMargins(const View* view,
        const CssBox* ctnBlock, CssBox* cssBox)
{
    //HTReal ctnh = ctnBlock->getContentHeight();

    HTReal btw = m_data[PID_BORDER_TOP_WIDTH].r;
    HTReal bbw = m_data[PID_BORDER_BOTTOM_WIDTH].r;
    cssBox->setVBorderWidths(btw, bbw);

    HTReal pt, pb;
    calcPaddings(ctnBlock, NULL, &pt, NULL, &pb);
    cssBox->setVPaddings(pt, pb);

    HTReal h, mt, mb;

    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE &&
            !view->isReplaced()) {
        // inline, non-replaced elements

        h = 0; /* undefined so far */
        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);
        return false;
    }
    else if ((CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_BLOCK ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE_BLOCK ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_FLOAT]) != PV_NONE) &&
            view->isReplaced()) {
        // Inline replaced elements, block-level replaced elements
        // in normal flow, 'inline-block' replaced elements in normal flow,
        // and floating replaced elements

        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);

        HTReal w = cssBox->getContentWidth();
        calcHeightForIR(view, ctnBlock, w, h);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_BLOCK &&
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_OVERFLOW]) == PV_VISIBLE &&
            !view->isReplaced()) {

        // Block-level non-replaced elements in normal flow
        // when 'overflow' computes to 'visible'
        // NOTE:
        // This section also applies to block-level non-replaced elements
        // in normal flow when 'overflow' does not compute to 'visible'
        // but has been propagated to the viewport.

        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);
        calcHeightForBlockVNR(view, ctnBlock, cssBox);
    }
    else if ((CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_ABSOLUTE ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_FIXED)) {

        if (view->isReplaced()) {
            // Absolutely positioned, replaced elements
            calcHeightsForAPR(view, ctnBlock, cssBox);
        }
        else {
            // Absolutely positioned, non-replaced elements
            calcHeightsForAPNR(view, ctnBlock, cssBox);
        }
    }
    else if (!view->isReplaced() &&
            ((CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_BLOCK &&
             CSS_PPT_VALUE_NOFLAGS(m_values[PID_OVERFLOW]) != PV_VISIBLE) ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) == PV_INLINE_BLOCK ||
            CSS_PPT_VALUE_NOFLAGS(m_values[PID_FLOAT]) != PV_NONE)) {

        // Block-level, non-replaced elements in normal flow when 'overflow'
        // does not compute to 'visible' (except if the 'overflow' property's
        // value has been propagated to the viewport).
        //
        // 'Inline-block', non-replaced elements.
        //
        // Floating, non-replaced elements.
        autoVMarginsAsZero(ctnBlock, mt, mb);
        cssBox->setVMargins(mt, mb);

        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (autoh) {
            resolveAutoHeights(view, ctnBlock, cssBox);
        }
    }
    else {
        bool autoh = getVSize(ctnBlock, PID_INTERNAL_HEIGHT, h);
        if (autoh) {
            resolveAutoHeights(view, ctnBlock, cssBox);
        }
    }

    cssBox->setContentHeight(h);
    return true;
}

bool CssComputed::calcWidthsMargins(const View* view, const CssBox* ctnBlock,
        CssBox* cssBox)
{
    // use PID_INTERNAL_WIDTH for the actually used property of width.
    // use PID_INTERNAL_HEIGHT for the actually used property of height.
    m_values[PID_INTERNAL_WIDTH] = m_values[PID_WIDTH];
    m_data[PID_INTERNAL_WIDTH] = m_data[PID_WIDTH];
    m_values[PID_INTERNAL_HEIGHT] = m_values[PID_HEIGHT];
    m_data[PID_INTERNAL_HEIGHT] = m_data[PID_HEIGHT];

    if (doCalcWidthsMargins(view, ctnBlock, cssBox)) {
        // check min-width, and max-width
        HTReal minw, maxw;
        bool valid_minw = getMinWidth(ctnBlock, minw);
        bool valid_maxw = getMaxWidth(ctnBlock, maxw);
        HTReal inr, rw, rh;

        if (valid_minw && cssBox->getContentWidth() < minw) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_WIDTH] = m_values[PID_MIN_WIDTH];
                m_data[PID_INTERNAL_WIDTH] = m_data[PID_MIN_WIDTH];
            }

            doCalcWidthsMargins(view, ctnBlock, cssBox);
        }

        if (valid_maxw && cssBox->getContentWidth() > maxw) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_WIDTH] = m_values[PID_MAX_WIDTH];
                m_data[PID_INTERNAL_WIDTH] = m_data[PID_MAX_WIDTH];
            }

            doCalcWidthsMargins(view, ctnBlock, cssBox);
        }

        return true;
    }

    return false;
}

bool CssComputed::calcHeightsMargins(const View* view, const CssBox* ctnBlock,
        CssBox* cssBox)
{
    // use PID_INTERNAL_WIDTH for the actually used property of width.
    // use PID_INTERNAL_HEIGHT for the actually used property of height.
    m_values[PID_INTERNAL_WIDTH] = m_values[PID_WIDTH];
    m_data[PID_INTERNAL_WIDTH] = m_data[PID_WIDTH];
    m_values[PID_INTERNAL_HEIGHT] = m_values[PID_HEIGHT];
    m_data[PID_INTERNAL_HEIGHT] = m_data[PID_HEIGHT];

    if (doCalcHeightsMargins(view, ctnBlock, cssBox)) {
        // check min-height, and max-height
        HTReal minh, maxh;
        bool valid_minh = getMinHeight(ctnBlock, minh);
        bool valid_maxh = getMaxHeight(ctnBlock, maxh);
        HTReal inr, rw, rh;

        if (valid_minh && cssBox->getContentHeight() < minh) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_HEIGHT] = m_values[PID_MIN_HEIGHT];
                m_data[PID_INTERNAL_HEIGHT] = m_data[PID_MIN_HEIGHT];
            }

            doCalcHeightsMargins(view, ctnBlock, cssBox);
        }

        if (valid_maxh && cssBox->getContentHeight() > maxh) {
            if (view->isReplaced() &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                CSS_PPT_VALUE_NOFLAGS(m_values[PID_HEIGHT]) ==
                    MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
                view->getIntrinsicRatio(&inr)) {

                if (resolveWHForRIR(view, ctnBlock, rw, rh)) {
                    m_values[PID_INTERNAL_WIDTH] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_WIDTH].r = rw;
                    m_values[PID_INTERNAL_HEIGHT] = PV_LENGTH_PX;
                    m_data[PID_INTERNAL_HEIGHT].r = rh;
                }
            }
            else {
                m_values[PID_INTERNAL_HEIGHT] = m_values[PID_MAX_HEIGHT];
                m_data[PID_INTERNAL_HEIGHT] = m_data[PID_MAX_HEIGHT];
            }

            doCalcHeightsMargins(view, ctnBlock, cssBox);
        }

        return true;
    }

    return false;
}

} // namespace hfcl

