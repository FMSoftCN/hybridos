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

#include "view/textview.h"

#include "view/viewfactory.h"
#include "common/helpers.h"
#include "graphics/textmode.h"
#include "view/transition.h"

namespace hfcl {

TextView::TextView(const char* vtag, const char* vtype,
            const char* vclass, const char* vname, int vid)
    : ViewContainer(vtag, vtype, vclass, vname, vid)
{
}

TextView::~TextView()
{
}

/*
 * For TextView, when the content changed, it should destroy
 * all children elements, and rebuild the children elements
 * by parsing the content and splitting the content into one or
 * multiple atomic text views.
 */
void TextView::onContentChanged()
{
}

DEFINE_CLASS_NAME(TextView)

AUTO_REGISTER_VIEW(hvtext, TextView);

// the following tags are supported by TextView
// <p> <span> <em> <strong> <dfn> <code> <samp> <kbd> <var> <cite> <a>

AUTO_REGISTER_VIEW(p, TextView);
AUTO_REGISTER_VIEW(span, TextView);
AUTO_REGISTER_VIEW(em, TextView);
AUTO_REGISTER_VIEW(strong, TextView);
AUTO_REGISTER_VIEW(dfn, TextView);
AUTO_REGISTER_VIEW(code, TextView);
AUTO_REGISTER_VIEW(samp, TextView);
AUTO_REGISTER_VIEW(kbd, TextView);
AUTO_REGISTER_VIEW(var, TextView);
AUTO_REGISTER_VIEW(cite, TextView);
AUTO_REGISTER_VIEW(a, TextView);

} // namespace hfcl

