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

#include "css/css.h"

namespace hfcl {

MAPCLASSKEY(string, Uint32, CssKeywordTypeMap);

static CssKeywordTypeMap _map;

Uint32 Css::getKeywordType(const char* name)
{
    if (_map.size() == 0) {
        string* str;

        str = HFCL_NEW_EX(string, ("first-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_FIRST_CHILD;

        str = HFCL_NEW_EX(string, ("only-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_ONLY_CHILD;

        str = HFCL_NEW_EX(string, ("last-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_LAST_CHILD;

        str = HFCL_NEW_EX(string, ("nth-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_NTH_CHILD;

        str = HFCL_NEW_EX(string, ("nth-last-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_NTH_LAST_CHILD;

        str = HFCL_NEW_EX(string, ("link"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_LINK;

        str = HFCL_NEW_EX(string, ("visited"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_VISITED;

        str = HFCL_NEW_EX(string, ("hover"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_HOVER;

        str = HFCL_NEW_EX(string, ("active"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_ACTIVE;

        str = HFCL_NEW_EX(string, ("focus"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_FOCUS;

        str = HFCL_NEW_EX(string, ("disabled"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_DISABLED;

        str = HFCL_NEW_EX(string, ("enabled"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_ENABLED;

        str = HFCL_NEW_EX(string, ("checked"));
        _map[*str] = Css::CSS_KW_DYNAMIC_PSEUDO_CLASS | Css::CSS_KWST_CHECKED;

        str = HFCL_NEW_EX(string, ("root"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_ROOT;

        str = HFCL_NEW_EX(string, ("empty"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS | Css::CSS_KWST_EMPTY;

        str = HFCL_NEW_EX(string, ("first-line"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT | Css::CSS_KWST_FIRST_LINE;

        str = HFCL_NEW_EX(string, ("first-letter"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT | Css::CSS_KWST_FIRST_LETTER;

        str = HFCL_NEW_EX(string, ("before"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT | Css::CSS_KWST_BEFORE;

        str = HFCL_NEW_EX(string, ("after"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT | Css::CSS_KWST_AFTER;
    }

    CssKeywordTypeMap::iterator it = _map.find(name);
    if (it == _map.end()) {
        return CSS_KW_UNKNOWN;
    }

    return _map[name];
}

} // namespace hfcl

