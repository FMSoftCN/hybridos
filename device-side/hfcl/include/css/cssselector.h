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

#ifndef HFCL_CSS_CSSSELECTOR_H_
#define HFCL_CSS_CSSSELECTOR_H_

#include "../common/common.h"
#include "../common/stlalternative.h"

namespace hfcl {

VECTORCLASS(utf8string, CssSelectorPieceVec);

class CssSelectorGroup;

class CssSelector {
public:
    CssSelector() {}
    ~CssSelector() {}

    const utf8string& getType() const { return m_type; }
    void setType(utf8string& type) {
        m_type = type;
    }

    void addIdentifier(utf8string& str) {
        m_identifiers.push_back(str);
    }

    void addClass(utf8string& str) {
        m_classes.push_back(str);
    }

    void addAttribute(utf8string& str) {
        m_attributes.push_back(str);
    }

    void addPseudoElement(utf8string& str) {
        m_pseudo_elements.push_back(str);
    }

    void addPseudoClass(utf8string& str) {
        m_pseudo_classes.push_back(str);
    }

    void addDynamicPseudoClass(utf8string& str) {
        m_dynamic_pseudo_classes.push_back(str);
    }

friend class CssSelectorGroup;

private:
    utf8string m_type;
    CssSelectorPieceVec m_identifiers;
    CssSelectorPieceVec m_classes;
    CssSelectorPieceVec m_attributes;
    CssSelectorPieceVec m_pseudo_elements;
    CssSelectorPieceVec m_pseudo_classes;
    CssSelectorPieceVec m_dynamic_pseudo_classes;
};

VECTOR(CssSelector*, CssSelectorVec);

class View;
class CssSelectorGroup {
public:
    CssSelectorGroup() {}
    ~CssSelectorGroup();

    enum {
        CSS_NOT_MATCHED = 0,
        CSS_STATIC,
        CSS_DYNAMIC,
    };

    bool compile(const char* selector);
    int match(const View* view);
    void clear();

#ifdef _UNIT_TEST
    void _print();
#endif

private:
    bool compileSingle(char* token, CssSelector& one);
    CssSelectorVec m_group;
};

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_CSSSELECTOR_H_ */

