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

// General block box
// Either for anonymous block-level boxes or normal block-level
// or inline-block boxes
class CssBox {
public:
    CssBox(CssComputed* css = NULL, bool anonymous = false);
    virtual ~CssBox();

    void setContentWidth(HTReal w) {
        m_cw = w;
    }
    HTReal getContentWidth() const { return m_cw; }

    void setContentHeight(HTReal h) {
        m_ch = h;
    }
    HTReal getContentHeight() const { return m_ch; }

    void setHPosition(HTReal l, HTReal r) {
        m_l = l; m_r = r;
    }
    void getHPosition(HTReal& l, HTReal& r) const {
        l = m_l; r = m_r;
    }

    void setVPosition(HTReal t, HTReal b) {
        m_t = t; m_b = b;
    }
    void getVPosition(HTReal& t, HTReal& b) const {
        t = m_t; b = m_b;
    }

    void setHMargins(HTReal ml, HTReal mr) {
        m_ml = ml; m_mr = mr;
    }
    void getHMargins(HTReal& ml, HTReal& mr) const {
        ml = m_ml; mr = m_mr;
    }

    void setVMargins(HTReal mt, HTReal mb) {
        m_mt = mt; m_mb = mb;
    }
    void getVMargins(HTReal& mt, HTReal& mb) const {
        mt = m_mt; mb = m_mb;
    }

    void setHPaddings(HTReal pl, HTReal pr) {
        m_pl = pl; m_pr = pr;
    }
    void getHPaddings(HTReal& pl, HTReal& pr) const {
        pl = m_pl; pr = m_pr;
    }

    void setVPaddings(HTReal pt, HTReal pb) {
        m_pt = pt; m_pb = pb;
    }
    void getVPaddings(HTReal& pt, HTReal& pb) const {
        pt = m_pt; pb = m_pb;
    }

    void setHBorderWidths(HTReal blw, HTReal brw) {
        m_blw = blw; m_brw = brw;
    }
    void getHBorderWidths(HTReal& blw, HTReal& brw) const {
        blw = m_blw; brw = m_brw;
    }

    void setVBorderWidths(HTReal btw, HTReal bbw) {
        m_btw = btw; m_bbw = bbw;
    }
    void getVBorderWidths(HTReal& btw, HTReal& bbw) const {
        btw = m_btw; bbw = m_bbw;
    }

    CssComputed* getCss() { return m_css; }
    const CssComputed* getCss() const { return m_css; }

protected:
    // content width and height
    HTReal m_cw, m_ch;
    // position
    HTReal m_l, m_t, m_r, m_b;
    // margins
    HTReal m_ml, m_mt, m_mr, m_mb;
    // paddings
    HTReal m_pl, m_pt, m_pr, m_pb;
    // border widths
    HTReal m_blw, m_btw, m_brw, m_bbw;

    CssComputed* m_css;
};

// Inline box either for anonymous inline-level boxes
// or normal inline-level boxes
class CssInlineBox : public CssBox {
public:
    CssInlineBox(CssComputed* css, const View* view, bool anonymous = false)
        : CssBox(css, anonymous), m_view(view), m_split_ctxt(0) {
    };
    virtual ~CssInlineBox() {};

private:
    const View* m_view;
    // the splitting context of the view
    const void* m_split_ctxt;
};

// Line box
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssLineBox {
public:
    CssLineBox() : m_w(0), m_h(0) { }
    virtual ~CssLineBox();

protected:
    HTReal m_w, m_h;

private:
    VECTOR(CssInlineBox*, CssInlineBoxVec);
    CssInlineBoxVec m_inlines;
};

// The block box acts as a line box container
// A line box container only contains inline-level boxes
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssLineBoxContainer : public CssBox {
public:
    CssLineBoxContainer(CssComputed* css, const View* first, const View* last,
            bool anonymous = false)
        : CssBox(css, anonymous), m_first(first), m_last(last) {};
    virtual ~CssLineBoxContainer();

    const View* getFirstView() const { return m_first; }
    const View* getLastView() const { return m_last; }

protected:

private:
    const View* m_first;
    const View* m_last;
    VECTOR(CssLineBox*, CssLineBoxVec);
    CssLineBoxVec m_lines;
};

// The box acts as a inline box container
//
// A box container contains only inline-level boxes.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssInlineBoxContainer : public CssBox {
public:
    CssInlineBoxContainer(CssComputed* css)
        : CssBox(css) {};
    virtual ~CssInlineBoxContainer();

    void addBox(CssBox* box);

protected:

private:
    VECTOR(CssBox*, CssInlineBoxVec);
    CssInlineBoxVec m_boxes;
};

// The block box acts as a block box container
//
// A block box container contains only block-level boxes.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#block-boxes
class CssBlockBoxContainer : public CssBox {
public:
    CssBlockBoxContainer(CssComputed* css)
        : CssBox(css) {};
    virtual ~CssBlockBoxContainer();

    void addBox(CssBox* box);

protected:

private:
    VECTOR(CssBox*, CssBlockBoxVec);
    CssBlockBoxVec m_blocks;
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSBOX_H_ */

