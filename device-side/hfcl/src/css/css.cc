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

MAPCLASSKEY(string, int, CssKeywordTypeMap);

static CssKeywordTypeMap _map;

int Css::getKeywordType(string& name)
{
    if (_map.size() == 0) {
        string* str;

        str = HFCL_NEW_EX(string, ("first-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("only-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("last-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("nth-child"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("link"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("visited"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("hover"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("active"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("focus"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("disabled"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("enabled"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("checked"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("root"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("empty"));
        _map[*str] = Css::CSS_KW_PSEUDO_CLASS;

        str = HFCL_NEW_EX(string, ("first-line"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT;

        str = HFCL_NEW_EX(string, ("first-letter"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT;

        str = HFCL_NEW_EX(string, ("before"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT;

        str = HFCL_NEW_EX(string, ("after"));
        _map[*str] = Css::CSS_KW_PSEUDO_ELEMENT;
    }

    CssKeywordTypeMap::iterator it = _map.find(name);
    if (it == _map.end()) {
        return CSS_KW_UNKNOWN;
    }

    return _map[name];
}

} // namespace hfcl

