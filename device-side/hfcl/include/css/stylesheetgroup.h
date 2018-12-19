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

#ifndef HFCL_CSS_STYLESHEETGROUP_H_
#define HFCL_CSS_STYLESHEETGROUP_H_

#include "../css/stylesheet.h"
#include "../common/object.h"

namespace hfcl {

class StyleSheetGroup : public RefCount {
public:
    StyleSheetGroup(){}
    virtual ~StyleSheetGroup();

    virtual bool appendCss(StyleSheet* css);

private:
    VECTOR(StyleSheet*, StyleSheetVec);

    StyleSheetVec m_css_vec;
};

#if 0
class SimpleStyleSheetGroup : public StyleSheetGroup {
public:
    SimpleStyleSheetGroup () { }
    SimpleStyleSheetGroup (StyleSheetGroup* s) : StyleSheetGroup (s) { }
    SimpleStyleSheetGroup (SimpleStyleSheetGroup* s)
            : StyleSheetGroup ((StyleSheetGroup*)s) { }
    virtual ~SimpleStyleSheetGroup();

    friend bool operator== (StyleSheetNode &dn1, StyleSheetNode &dn2) {
        if ((dn1._id == dn2._id) && (dn1._dr == dn2._dr))
            return true;
        return false;
    }

    virtual StyleSheet* getStyleSheet (int dr_id) const;
    virtual bool setStyleSheet (int dr_id, StyleSheet* dr);

    CopyOnWriteable* clone() {
        return (CopyOnWriteable*)(HFCL_NEW_EX(SimpleStyleSheetGroup, (this)));
    }

private:
    StyleSheetList m_drawables;
    StyleSheetNode* find (int dr_id) const;
};

class HashedStyleSheetGroup : public StyleSheetGroup {
public:
    HashedStyleSheetGroup() { }
    HashedStyleSheetGroup(StyleSheetGroup* s) : StyleSheetGroup(s) { }
    HashedStyleSheetGroup(HashedStyleSheetGroup* s) : StyleSheetGroup(s) { }

    virtual bool setStyleSheet(int dr_id, StyleSheet* dr);
    virtual StyleSheet* getStyleSheet(int dr_id) const;

    CopyOnWriteable * clone() {
        return (CopyOnWriteable*) (HFCL_NEW_EX(HashedStyleSheetGroup, (this)));
    }

private:
    MAP (int, StyleSheet*, StyleSheetGroupMap)
    StyleSheetGroupMap m_drawables;
};
#endif

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_STYLESHEETGROUP_H_ */

