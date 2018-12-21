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
#include "../common/object.h"
#include "../common/stlalternative.h"
#include "../view/view.h"

namespace hfcl {

class CssBox : public Object {
public:
    CssBox();
    virtual ~CssBox() {};

protected:
    IntRect m_margin_box;
    IntRect m_content_box;

    /* left, top, right, bottom */
    int m_margins[4];
    int m_borders[4];
    int m_paddings[4];
};

// Block-level box
//
// A block-level box may contain only block-level boxes
// or contain only inline-level boxes, or it is not a
// container.
// HFCL dose not generate anonymous block boxes.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#block-boxes
class CssBoxBlock : public CssBox
{
public:
    CssBoxBlock(View* view);
    virtual ~CssBoxBlock();

protected:

private:
    View* m_view;
};

// Inline-level boxes
//
// In HFCL, all inline-level boxes are atomic inline-level boxes.
// HFCL dose not generate anonymous inline boxes either.
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-boxes
class CssBoxInline : public CssBox
{
public:
    CssBoxInline(View* view);
    virtual ~CssBoxInline();

protected:

private:
    View* m_view;
};

// Line box
// The inline boxes in a line box will be referred by the view
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssBoxLine : public CssBoxBlock
{
public:
    CssBoxLine(View* view);
    virtual ~CssBoxLine();

protected:

private:
};

// Block-level box acts as a line box container
//
// Ref: https://www.w3.org/TR/CSS22/visuren.html#inline-formatting
class CssBoxLineBoxContainer : public CssBoxBlock
{
public:
    CssBoxLineBoxContainer(View* view);
    virtual ~CssBoxLineBoxContainer();

protected:

private:
    VECTOR(CssBoxLine*, CssBoxLineVec);
    CssBoxLineVec m_lines;
};

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_CSSBOX_H_ */

