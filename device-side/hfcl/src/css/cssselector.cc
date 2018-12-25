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

#include "css/cssselector.h"

#include <string.h>

#include "view/view.h"

namespace hfcl {

/*
 * So far, only the following syntax is supported:
 *
 *  - single selector: [tag | *][.class | #id][<.class>*][<:pseudo-class | :pseudo-element>*]
 *  - multiple selectors: <selector>[, selector]*
 *
 */
CssSelectorGroup::~CssSelectorGroup()
{
    clear();
}

void CssSelectorGroup::clear()
{
    CssSelectorVec::iterator it;
    for (it = m_group.begin(); it != m_group.end(); ++it) {
        CssSelector* one = *it;
        _DBG_PRINTF ("~CssSelectorGroup: deleting %p\n", one);
        HFCL_DELETE(one);
    }

    m_group.clear();
}

static string* get_tag (char* scan)
{
    string* str = new string;
    return str;
}

static string* get_class (char* scan)
{
    string* str = new string;
    return str;
}

static string* get_id (char* scan)
{
    string* str = new string;
    return str;
}

static string* get_attr (char* scan)
{
    string* str = new string;
    return str;
}

static string* get_pseudo_name (char* scan)
{
    string* str = new string;
    return str;
}

bool CssSelectorGroup::compileSingle(char* token, CssSelector& one)
{
    char* scan = token;
    bool tag_set = false;
    string* str;

    while (*scan) {
        while (isspace (*scan)) {
            scan++;
        }

        switch (*scan) {
        case '*':
            str = new string("");
            one.setTag(*str);
            tag_set = true;
            scan++;
            break;

        case '.':
            if (!tag_set) {
                str = new string("");
                one.setTag(*str);
                tag_set = true;
            }

            scan++;
            str = get_class(scan);
            if (str) {
                one.addClass(*str);
            }
            break;

        case '#':
            if (!tag_set) {
                str = new string("");
                one.setTag(*str);
                tag_set = true;
            }

            scan++;
            str = get_id(scan);
            if (str) {
                one.addIdentifier(*str);
            }
            break;

        case '[':
            if (!tag_set) {
                str = new string("");
                one.setTag(*str);
                tag_set = true;
            }

            scan++;
            str = get_attr(scan);
            if (str) {
                one.addAttribute(*str);
            }
            break;

        case ':':
            if (!tag_set) {
                str = new string("");
                one.setTag(*str);
                tag_set = true;
            }

            scan++;
            str = get_pseudo_name(scan);
            if (str) {
                switch (Css::getKeywordType (*str)) {
                case Css::CSS_KW_PSEUDO_ELEMENT:
                    one.addPseudoElements(*str);
                    break;
                case Css::CSS_KW_PSEUDO_CLASS:
                    one.addPseudoClasses(*str);
                    break;
                default:
                    _DBG_PRINTF("CssSelectorGroup::compileSingle: Syntax error: %s\n", token);
                    return false;
                }
            }
            break;

        default:
            if (isalpha(*scan)) {
                str = get_tag(scan);
                if (str) {
                    one.setTag (*str);
                }
            }
            else {
                _DBG_PRINTF("CssSelectorGroup::compileSingle: Syntax error: %s\n", token);
                return false;
            }
            break;
        }
    }

    return true;
}

bool CssSelectorGroup::compile(const char* selector)
{
    char* copied = strdup (selector);
    char* context;

    if (copied == NULL)
        return false;

    clear ();

    do {
        char* token;

        token = strtok_r(copied, ",", &context);
        if (token == NULL)
            break;

        CssSelector* one = HFCL_NEW(CssSelector);
        if (compileSingle(token, *one)) {
            m_group.push_back (one);
        }
        else {
            HFCL_DELETE(one);
        }
    } while (1);

    free (copied);
    return true;
}

/*
 * returns:
 *  0: not matched
 *  CSS_STATIC: static css
 *  CSS_DYNAMIC: dynamic css, for pseudo classes
 */
int CssSelectorGroup::match(const View* view) const
{
    return 0;
}

} // namespace hfcl

