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

#include <string.h>

#include "fontres.h"
#include "resloader.h"

namespace hfcl {

FontRes::FontRes(const char *fontname)
    : m_fontname(fontname)
    , m_font(NULL)
{
}

FontRes::FontRes()
    : m_fontname(NULL)
    , m_font(NULL)
{
}

FontRes::~FontRes()
{
    m_fontname = NULL;
    m_font = NULL;
}

Logfont* FontRes::get(/*Style* style = NULL*/)
{
    if (NULL == m_font)
        m_font = ResLoader::getInstance()->getFont(m_fontname/*, style*/);

    return m_font;
}

} // namespace hfcl

