/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018, 2019 Beijing FMSoft Technologies Co., Ltd.
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

/*
** contentview.cc: The implementation of ContentView class.
**
** Create by WEI Yongming at 2019/04/25
*/

#include "view/contentview.h"

#include "view/viewfactory.h"
#include "common/helpers.h"
#include "graphics/textmode.h"
#include "view/transition.h"

namespace hfcl {

/*
 * The ContentView is intended to support the tags which are
 * defined as 'Grouping content' (such as `p`) by HTML5:
 *
 * https://www.w3.org/TR/html52/grouping-content.html#grouping-content
 *
 * In HVML, the following tags are supported by ContentView:
 *
 * - `hvctn`:
 *
 * By using ContentView, you can use most of HTML5 text-level tags to
 * format the content, for examples:
 *
 *      Welcome to the world of <em>HybridOS</em>!
 *
 * Please see the following webpage for the text-level tags:
 *
 * https://www.w3.org/TR/html52/textlevel-semantics.html#textlevel-semantics
 *
 * You can also use some tags to embed the other elements into
 * the text flow, such as `img`, `button`, `input`, `marquee`, `meter`,
 * and `progress`.
 *
 * This view will parse the content and generate the children views
 * automatically.
 */

ContentView::ContentView(const char* vtag, const char* vtype,
            const char* vclass, const char* vname, int vid)
    : ViewContainer(vtag, vtype, vclass, vname, vid)
{
}

ContentView::~ContentView()
{
}

/*
 * For ContentView, when the content changed, it should destroy
 * all children elements, and rebuild the children elements
 * by parsing the content and splitting the content into one or
 * multiple text views.
 */
void ContentView::onContentChanged()
{
}

AUTO_REGISTER_VIEW(hvctn, ContentView);

} // namespace hfcl

