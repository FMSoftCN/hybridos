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

#ifdef _UNIT_TEST

#include <string>

#include "css/css.h"
#include "css/cssdeclared.h"
#include "css/cssdeclaredgroup.h"

namespace hfcl {

class View {
public:
    View(const char* type, const char* cls, const char* name,
         const char* attr, const char* ps_ele, const char* ps_cls)
        : m_id(0), m_type(type), m_cls(cls), m_name(name)
        , m_attr(attr), m_ps_ele(ps_ele), m_ps_cls(ps_cls) {
        m_cls += ' ';
    }
    virtual ~View() {}

    const char* type() const { return m_type.c_str(); }
    const char* getName() const { return m_name.c_str(); }
    void genSelector(std::string& str) const;

    bool checkClass(const char* cls) const;
    bool checkAttribute(const char* attr) const;
    bool checkPseudoElement(const char* pseudoEle) const;
    bool checkPseudoClass(const char* pseudoCls) const;

    int m_id;
    std::string m_type;
    std::string m_cls;
    std::string m_name;
    std::string m_attr;
    std::string m_ps_ele;
    std::string m_ps_cls;
};

/* make sure there is a space at the head and the tail */
static void add_space (std::string& str)
{
    if (str.empty ()) {
        str = ' ';
    }
    else {
        if (str.front() != ' ') {
            str = ' ' + str;
        }
        if (str.back() != ' ') {
            str += ' ';
        }
    }
}

void View::genSelector(std::string& str) const
{
    str = m_type;
    str += "#";
    str += m_name;
    str += ".";
    str += m_cls;
    str += "[";
    str += m_attr;
    str += "]";
    str += ":";
    str += m_ps_ele;
    str += ":";
    str += m_ps_cls;
}

bool View::checkClass (const char* cls) const
{
    std::string tmp = cls;
    add_space (tmp);

    if (strcasestr (tmp.c_str(), m_cls.c_str())) {
        return true;
    }

    return false;
}

bool View::checkAttribute(const char* attr) const
{
    if (strcasecmp (m_attr.c_str(), attr) == 0) {
        return true;
    }

    return false;
}

bool View::checkPseudoElement(const char* pseudoEle) const
{
    if (strcasecmp (m_ps_ele.c_str(), pseudoEle) == 0) {
        return true;
    }

    return false;
}

bool View::checkPseudoClass(const char* pseudoCls) const
{
    if (strcasecmp (m_ps_cls.c_str(), pseudoCls) == 0) {
        return true;
    }

    return false;
}

} // namespace hfcl

#else
#include "view/view.h"
#endif

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
    char* tmp = scan;

    do {
        int c = *tmp;
        if (c && strchr (" .#[]:", c)) {
            break;
        }

        tmp++;
    } while (*tmp);

    if (tmp == scan)
        return NULL;

    int n = (int)(tmp - scan);
    string* str = new string(scan, n);
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

        str = NULL;

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
                Uint32 kw_type = Css::getKeywordType (str->c_str());
                if (kw_type & Css::CSS_KW_PSEUDO_ELEMENT) {
                    one.addPseudoElement(*str);
                }
                else if (kw_type & Css::CSS_KW_PSEUDO_CLASS) {
                    one.addPseudoClass(*str);
                }
                else if (kw_type & Css::CSS_KW_DYNAMIC_PSEUDO_CLASS) {
                    one.addDynamicPseudoClass(*str);
                }
                else {
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
                    tag_set = true;
                    break;
                }
            }

            goto error;
            break;
        }

        if (str) {
            delete str;
            str = NULL;
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

    char* token;

    token = strtok_r(copied, ",", &context);
    while (token) {
        CssSelector* one = HFCL_NEW(CssSelector);
#ifdef _UNIT_TEST
        printf ("To compile a single selector: %s\n", token);
#endif
        if (compileSingle(token, *one)) {
            m_group.push_back (one);
        }
        else {
            HFCL_DELETE(one);
        }

        token = strtok_r(NULL, ",", &context);
    };

    free (copied);
    return true;
}

/*
 * returns:
 *  CSS_NOT_MATCHED: not matched
 *  CSS_STATIC: static css
 *  CSS_DYNAMIC: dynamic css, for pseudo classes
 */
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
                if (!view->checkClass(str.c_str())) {
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
                if (!view->checkAttribute(str.c_str())) {
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
                if (!view->checkPseudoElement(str.c_str())) {
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
                if (!view->checkPseudoClass(str.c_str())) {
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

    return CSS_NOT_MATCHED;
}

} // namespace hfcl

#ifdef _UNIT_TEST

namespace hfcl {

void CssSelectorGroup::_print()
{
    CssSelectorVec::iterator it;
    for (it = m_group.begin(); it != m_group.end(); ++it) {
        CssSelector* one = *it;

        printf ("One selector: \n");
        printf ("   Type: %s\n", one->getType().c_str());

        // check id: once an id matched
        CssSelectorPieceVec& pieces = one->m_identifiers;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Identifier: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Identifier: <NONE>\n");
        }

        // check classes: all class should be matched
        pieces = one->m_classes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Class: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Class: <NONE>\n");
        }

        // check attributes: all attributes should be matched
        pieces = one->m_attributes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Attribute: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Attribute: <NONE>\n");
        }

        // check pseudo elements: all pseudo elements should be matched
        pieces = one->m_pseudo_elements;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Pseudo Element: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Pseudo Element: <NONE>\n");
        }

        // check pseudo classes: all pseudo classes should be matched
        pieces = one->m_pseudo_classes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Pseudo Class: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Pseudo Class: <NONE>\n");
        }

        // check dynamic pseudo classes: once defined
        pieces = one->m_dynamic_pseudo_classes;
        if (pieces.size() > 0) {
            CssSelectorPieceVec::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it) {
                string& str = *it;
                printf ("   Dynamic Pseudo Class: %s\n", str.c_str());
            }
        }
        else {
            printf ("   Dynamic Pseudo Class: <NONE>\n");
        }
    }
}

} // namespace hfcl

static const char* selectors [] = {
    "*",
    "hvtext",
    ".active",
    "hvtext.active",
    "hvtext#foo",
    "hvtext.active.vip:focus",
    "hvtext#bar:focus:hover",
    "hvtext[foo=bar][copyright='FMSoft Technologies']",
    "hvtext:active[foo=bar], hvimg.active.vip:focus, hvli#foo",
};

int main ()
{
    using namespace hfcl;

    CssSelectorGroup csg;
    View view_a ("hvtext", "active", "foo", "foo=bar", "first-child", "focus");

    for (size_t i = 0; i < TABLESIZE(selectors); i++) {
        printf("Compiling \"%s\"\n", selectors[i]);
        csg.compile(selectors[i]);

        printf("Compiled result for \"%s\"\n", selectors[i]);
        csg._print();

        std::string selector;
        view_a.genSelector(selector);
        switch (csg.match(&view_a)) {
        case CssSelectorGroup::CSS_STATIC:
            printf("View (%s) matches staticly\n", selector.c_str());
            break;
        case CssSelectorGroup::CSS_DYNAMIC:
            printf("View (%s) matches dynamicly\n", selector.c_str());
            break;
        default:
            printf("View (%s) does not match\n", selector.c_str());
            break;
        }

        printf("DONE\n\n");
    }

    return 0;
}

#endif

