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
** textview.cc: The implementation of TextView class.
**
** Create by WEI Yongming at 2019/04/24
*/

#include "view/textview.h"

#include "view/viewfactory.h"
#include "common/helpers.h"
#include "graphics/textmode.h"
#include "view/transition.h"

namespace hfcl {

/*
 * The TextView is intended to support the text-level tags defined by HTML5:
 *
 * https://www.w3.org/TR/html52/textlevel-semantics.html#textlevel-semantics
 *
 */

TextView::TextView(const char* vtag, const char* vtype,
            const char* vclass, const char* vname, int vid)
    : ViewContainer(vtag, vtype, vclass, vname, vid)
{
}

TextView::~TextView()
{
}

void TextView::onContentChanged()
{
}

AUTO_REGISTER_VIEW(a, TextView);
AUTO_REGISTER_VIEW(em, TextView);
AUTO_REGISTER_VIEW(strong, TextView);
AUTO_REGISTER_VIEW(cite, TextView);
AUTO_REGISTER_VIEW(span, TextView);
AUTO_REGISTER_VIEW(dfn, TextView);
AUTO_REGISTER_VIEW(code, TextView);
AUTO_REGISTER_VIEW(samp, TextView);
AUTO_REGISTER_VIEW(kbd, TextView);
AUTO_REGISTER_VIEW(var, TextView);

} // namespace hfcl

