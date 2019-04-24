/*
** HFCL - HybridOS Foundation Class Library
**
** Copyright (C) 2018,2019 Beijing FMSoft Technologies Co., Ltd.
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

#include "view/atomictextview.h"

namespace hfcl {

//define for roll text
#define ROLLOFFSET 5

DEFINE_CLASS_NAME(AtomicTextView)

AtomicTextView::AtomicTextView(const char* vclass, const char* vname, int vid)
    : View(NULL, NULL, vclass, vname, vid)
{
}

AtomicTextView::~AtomicTextView()
{
}

const char* AtomicTextView::getContent() const
{
    return NULL;
}

bool AtomicTextView::setContent(const char* content)
{
    return false;
}

void AtomicTextView::drawContent(GraphicsContext* context, IntRect &rc)
{
}

} // namespace hfcl

