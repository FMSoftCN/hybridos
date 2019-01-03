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

#ifndef HFCL_CSS_CSSBOX_H_
#define HFCL_CSS_CSSBOX_H_

#include "../common/common.h"
#include "../common/intrect.h"
#include "../common/realrect.h"
#include "../common/object.h"
#include "../common/stlalternative.h"
#include "../view/view.h"

namespace hfcl {

class CssBox : public Object {
public:
    CssBox();
    virtual ~CssBox() {};

    void setWidth(HTReal l, HTReal w) {
        m_l = l;
        m_r = l + w;
    }
    HTReal getWidth() { return m_r - m_l; }

    void setHeight(HTReal t, HTReal h) {
        m_t = t;
        m_b = t + h;
    }
    HTReal getHeight() { return m_b - m_t; }

    void setHMargins(HTReal ml, HTReal mr) {
        m_ml = ml; m_mr = mr;
    }
    void getHMargins(HTReal& ml, HTReal& mr) {
        ml = m_ml; mr = m_mr;
    }

    void setVMargins(HTReal mt, HTReal mb) {
        m_mt = mt; m_mb = mb;
    }
    void getVMargins(HTReal& mt, HTReal& mb) {
        mt = m_mt; mb = m_mb;
    }

    void setHPaddings(HTReal pl, HTReal pr) {
        m_pl = pl; m_pr = pr;
    }
    void getHPaddings(HTReal& pl, HTReal& pr) {
        pl = m_pl; pr = m_pr;
    }

    void setVPaddings(HTReal pt, HTReal pb) {
        m_pt = pt; m_pb = pb;
    }
    void getVPaddings(HTReal& pt, HTReal& pb) {
        pt = m_pt; pb = m_pb;
    }

    void setHBorderWidths(HTReal blw, HTReal brw) {
        m_blw = blw; m_brw = brw;
    }
    void getHBorderWidths(HTReal& blw, HTReal& brw) {
        blw = m_blw; brw = m_brw;
    }

    void setVBorderWidths(HTReal btw, HTReal bbw) {
        m_btw = btw; m_bbw = bbw;
    }
    void getVBorderWidths(HTReal& btw, HTReal& bbw) {
        btw = m_btw; bbw = m_bbw;
    }

protected:
    HTReal m_l, m_t, m_r, m_b;
    HTReal m_ml, m_mt, m_mr, m_mb;
    HTReal m_pl, m_pt, m_pr, m_pb;
    HTReal m_blw, m_btw, m_brw, m_bbw;
};

// Block box
//
// A block box may contain only block-level boxes
// or contain only inline-level boxes, or it is not a
// container.
//
// HFCL dose not generate anonymous block boxes.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#block-boxes
class CssBoxBlock : public CssBox
{
public:
    CssBoxBlock() {};
    virtual ~CssBoxBlock() {};

protected:

private:
};

// Inline box
//
// HFCL dose not generate anonymous inline boxes.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-boxes
class CssBoxInline : public CssBox
{
public:
    CssBoxInline() {};
    virtual ~CssBoxInline() {};

protected:

private:
};

// Line box
// The inline boxes in a line box will be referred by the view
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssBoxLine : public CssBoxBlock
{
public:
    CssBoxLine() {};
    virtual ~CssBoxLine() {};

protected:

private:
};

// Block box acts as a line box container
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssBoxLineBoxContainer : public CssBoxBlock
{
public:
    CssBoxLineBoxContainer() {};
    virtual ~CssBoxLineBoxContainer();

protected:

private:
    VECTOR(CssBoxLine*, CssBoxLineVec);
    CssBoxLineVec m_lines;
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSBOX_H_ */

