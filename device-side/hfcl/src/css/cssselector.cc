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

static string* get_token (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}

#if 0
static string* get_type (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}

static string* get_class (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}

static string* get_id (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}

static string* get_attr (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}

static string* get_pseudo_name (char* scan)
{
    char* token;
    char* saveptr;
    token = strtok_r(scan, " .#[]:", &saveptr);
    if (token == NULL)
        return NULL;

    string* str = new string(token);
    return str;
}
#endif

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
            one.setType(*str);
            tag_set = true;
            scan++;
            break;

        case '.':
            if (!tag_set) {
                str = new string("");
                one.setType(*str);
                tag_set = true;
            }

            scan++;
            str = get_token(scan);
            if (str) {
                one.addClass(*str);
                scan += str->length();
            }
            else {
                goto error;
            }
            break;

        case '#':
            if (!tag_set) {
                str = new string("");
                one.setType(*str);
                tag_set = true;
            }

            scan++;
            str = get_token(scan);
            if (str) {
                one.addIdentifier(*str);
                scan += str->length();
            }
            else {
                goto error;
            }
            break;

        case '[':
            if (!tag_set) {
                str = new string("");
                one.setType(*str);
                tag_set = true;
            }

            scan++;
            str = get_token(scan);
            if (str) {
                one.addAttribute(*str);
                scan += str->length();
            }
            else {
                goto error;
            }
            break;

        case ']':
            scan++;
            break;

        case ':':
            if (!tag_set) {
                str = new string("");
                one.setType(*str);
                tag_set = true;
            }

            scan++;
            str = get_token(scan);
            if (str) {
                switch (Css::getKeywordType (*str)) {
                case Css::CSS_KW_PSEUDO_ELEMENT:
                    one.addPseudoElement(*str);
                    break;
                case Css::CSS_KW_PSEUDO_CLASS:
                    one.addPseudoClass(*str);
                    break;
                case Css::CSS_KW_DYNAMIC_PSEUDO_CLASS:
                    one.addDynamicPseudoClass(*str);
                    break;
                default:
                    goto error;
                }

                scan += str->length();
            }
            else {
                goto error;
            }
            break;

        default:
            if (isalpha(*scan)) {
                str = get_token(scan);
                if (str) {
                    one.setType (*str);
                    scan += str->length();
                    break;
                }
            }

            goto error;
            break;
        }
    }

    return true;

error:
    _DBG_PRINTF("CssSelectorGroup::compileSingle: Syntax error: %s\n", token);
    return false;
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
#define MATCH_TYPE              0x00000001
#define MATCH_ID                0x00000002
#define MATCH_CLASSES           0x00000004
#define MATCH_ATTRIBUTES        0x00000008
#define MATCH_PSEUDO_ELEMENTS   0x00000010
#define MATCH_PSEUDO_CLASSES    0x00000020

int CssSelectorGroup::match(const View* view)
{
    CssSelectorVec::iterator it;
    for (it = m_group.begin(); it != m_group.end(); ++it) {
        CssSelector* one = *it;

        const string& type = one->getType();
        if (type.compare("") && type.compare(view->type()))
            continue;

        // check id: once an id matched
        CssSelectorPieceVec& pieces = one->m_identifiers;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            bool matched = false;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                if (str.compare (view->getName()) == 0) {
                    matched = true;
                    break;
                }
            }

            if (!matched)
                continue;
        }

        // check classes: all class should be matched
        pieces = one->m_classes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                if (!view->checkClass(str)) {
                    continue;
                }
            }
        }

        // check attributes: all attributes should be matched
        pieces = one->m_attributes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                if (!view->checkAttribute(str)) {
                    continue;
                }
            }
        }

        // check pseudo elements: all pseudo elements should be matched
        pieces = one->m_pseudo_elements;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                if (!view->checkPseudoElement(str)) {
                    continue;
                }
            }
        }

        // check pseudo classes: all pseudo classes should be matched
        pieces = one->m_pseudo_classes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                if (!view->checkPseudoClass(str)) {
                    continue;
                }
            }
        }

        // check dynamic pseudo classes: once defined
        pieces = one->m_dynamic_pseudo_classes;
        if (pieces.size() > 0) {
            return CSS_DYNAMIC;
        }

        return CSS_STATIC;
    }

    return 0;
}

} // namespace hfcl

