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
#include "view/view.h"
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

bool CssComputed::setProperty(CssPropertyIds pid, Uint32 value,
        HTPVData data)
{
    m_values[pid] = value;
    m_data[pid] = data;
    return true;
}

void CssComputed::autoMarginsForInline(const RealRect& ctnBlock,
        HTReal& ml, HTReal& mr)
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
            ml = ctnBlock.width() * m_data[PID_MARGIN_LEFT].r / 100.0;
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
            mr = ctnBlock.width() * m_data[PID_MARGIN_RIGHT].r / 100.0;
        }
    }
}

bool CssComputed::hasComputedMarginLeft(const RealRect& ctnBlock, HTReal& ml)
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
            ml = ctnBlock.width() * m_data[PID_MARGIN_LEFT].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedMarginRight(const RealRect& ctnBlock, HTReal& mr)
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
            mr = ctnBlock.width() * m_data[PID_MARGIN_RIGHT].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedWidth(const RealRect& ctnBlock, HTReal& w)
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
            w = ctnBlock.width() * m_data[PID_WIDTH].r / 100.0;
        }
    }

    return true;
}

bool CssComputed::hasComputedHeight(const RealRect& ctnBlock, HTReal& h)
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
            h = ctnBlock.height() * m_data[PID_HEIGHT].r / 100.0;
        }
    }

    return true;
}

void CssComputed::calcPaddings(const RealRect& ctnBlock,
        HTReal* pl, HTReal* pt, HTReal* pr, HTReal* pb)
{
    if (pl) {
        Uint32 v = m_values[PID_PADDING_LEFT];
        if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            *pl = m_data[PID_PADDING_LEFT].r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            *pl = ctnBlock.width() * m_data[PID_PADDING_LEFT].r / 100.0;
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
            *pt = ctnBlock.height() * m_data[PID_PADDING_TOP].r / 100.0;
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
            *pr = ctnBlock.width() * m_data[PID_PADDING_RIGHT].r / 100.0;
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
            *pb = ctnBlock.height() * m_data[PID_PADDING_BOTTOM].r / 100.0;
        }
        else {
            *pb = 0.0;
        }

        if (*pb < 0.0)
            *pb = 0.0;
    }
}

// Inline, replaced elements
void CssComputed::calcWidthForIR(const View& view, const RealRect& ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr)
{
    autoMarginsForInline(ctnBlock, ml, mr);

    HTReal inw, inh, inr;
    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO) &&
        CSS_PPT_VALUE_NOFLAGS(m_values[PID_HEIGHT]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        if (view.getIntrinsicWidth(&inw)) {
            w = inw;
        }
        else if (view.getIntrinsicHeight(&inh) &&
                 view.getIntrinsicRatio(&inr)) {
            w = inh * inr;
        }
        else if (view.getIntrinsicRatio(&inr)) {
            w = 0.0; // undefined
        }
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_WIDTH]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_AUTO)) {
        HTReal h;
        if (hasComputedHeight(ctnBlock, h) && view.getIntrinsicRatio(&inr)) {
            w = h * inr;
        }
        else if (view.getIntrinsicWidth(&inw)) {
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

// Block-level, non-replaced elements
void CssComputed::calcWidthForBNR(const RealRect& ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr, bool br)
{
    // NOTE:
    // Computed border width must be absolute length;
    // '0' if the border style is 'none' or 'hidden'.
    HTReal blw = m_data[PID_BORDER_LEFT_WIDTH].r;
    HTReal brw = m_data[PID_BORDER_RIGHT_WIDTH].r;

    HTReal pl, pr;
    calcPaddings(ctnBlock, &pl, NULL, &pr, NULL);

    ml = mr = 0.0;
    bool automl = !hasComputedMarginLeft(ctnBlock, ml);
    bool automr = !hasComputedMarginRight(ctnBlock, mr);
    bool autow;
    if (br) {
        /* w already determined for block and replaced element */
        autow = false;
    }
    else {
        autow = !hasComputedWidth(ctnBlock, w);
    }

    if (!autow && ((ml + blw + pl + w + pr + brw + mr) > ctnBlock.width())) {
        if (automl) {
            ml = ctnBlock.width() - w - mr - brw - blw - pr - pl;
        }
        else if (automr) {
            mr = ctnBlock.width() - w - ml - brw - blw - pr - pl;
        }
        else {
            // over-constrained

            // TODO:
            // if direction of containing block is ltr
            mr = ctnBlock.width() - w - ml - brw - blw - pr - pl;
            // if direction of containing block is rtl
            // ml = ctnBlock.width() - w - mr - brw - blw - pr - pl;
        }
    }
    else if (autow && !automl && !automr) {
        w = ctnBlock.width() - ml - mr - brw - blw - pr - pl;
    }
    else if (!autow && automl && !automr) {
        ml = ctnBlock.width() - w - mr - brw - blw - pr - pl;
    }
    else if (!autow && !automl && automr) {
        mr = ctnBlock.width() - w - ml - brw - blw - pr - pl;
    }
    else if (autow) {
        if (automl) ml = 0.0;
        if (automr) mr = 0.0;
        w = ctnBlock.width() - ml - mr - brw - blw - pr - pl;
    }
    else if (automl && automr) {
        ml = mr = (ctnBlock.width() - w - brw - blw - pr - pl) / 2.0;
    }
}

/*
 * Ref: https://www.w3.org/TR/CSS22/visudet.html#Computing_widths_and_margins
 */
CssBox* CssComputed::calcWidthsMargins(const View& view,
        const RealRect& ctnBlock)
{
    CssBox* box = NULL;

    // determine width and margins
    Uint32 v = m_values[PID_WIDTH];
    HTPVData d = m_data[PID_WIDTH];
    HTReal w, ml, mr;

    if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_INLINE) &&
            !view.isReplaced()) {
        // inline, non-replaced elements
        w = 0.0; /* undefined so far */

        autoMarginsForInline(ctnBlock, ml, mr);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_INLINE) &&
            view.isReplaced()) {
        // inline, replaced elements
        calcWidthForIR(view, ctnBlock, w, ml, mr);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BLOCK) &&
        !view.isReplaced()) {
        // Block-level, non-replaced elements in normal flow
        calcWidthForBNR(ctnBlock, w, ml, mr);
    }
    else if (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DISPLAY]) ==
            MAKE_CSS_PPT_VALUE(PVT_KEYWORD, PVK_BLOCK) &&
        view.isReplaced()) {
        // Block-level, replaced elements in normal flow
        calcWidthForIR(view, ctnBlock, w, ml, mr);
        calcWidthForBNR(ctnBlock, w, ml, mr, true);
    }
    else {
        if (CSS_PPT_VALUE_TYPE(v) == PVT_KEYWORD &&
                CSS_PPT_VALUE_KEYWORD(v) == PVK_AUTO) {
            w = ctnBlock.width();
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_LENGTH_PX) {
            w = d.r;
        }
        else if (CSS_PPT_VALUE_TYPE(v) == PVT_PERCENTAGE) {
            // For absolutely positioned elements whose containing block
            // is based on a block container element, the percentage is
            // calculated with respect to the width of the padding box of
            // that element. This is a change from CSS1, where the percentage
            // width was always calculated with respect to the content box
            // of the parent element.
            w = ctnBlock.width() * d.r / 100.0;
        }
    }

    return box;
}

} // namespace hfcl

