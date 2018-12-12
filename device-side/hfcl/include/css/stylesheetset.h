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

#ifndef HFCL_CSS_STYLESHEETSET_H_
#define HFCL_CSS_STYLESHEETSET_H_

#include "../css/stylesheet.h"

namespace hfcl {

class StyleSheetSet : public CopyOnWriteable {
protected:
    bool m_is_common;
    StyleSheetSet*  m_super;

public:
    StyleSheetSet(){ m_is_common = false; }
    StyleSheetSet(StyleSheetSet *s) : m_super(s) { m_is_common = false; }
    virtual ~StyleSheetSet() {
        if (m_super != NULL && !m_super->isCommon()) {
            HFCL_DELETE(m_super);
            m_super = NULL;
        }
    }

    bool isCommon(void) { return m_is_common; }
    void setCommon(bool common) { m_is_common = common; }

    inline void setSuper(StyleSheetSet* s) {
        m_super = s;
    }

    virtual bool setStyleSheet(int id, StyleSheet* dr) = 0;
    virtual StyleSheet* getStyleSheet(int id) const = 0;

    virtual bool setStyleSheet(int dr_id, int se_id, HTData value) = 0;
    HTData getStyleSheet(int dr_id, int se_id) const {
        const StyleSheet* dr = getStyleSheet(dr_id);
        return dr ? dr->getElement(se_id) : 0;
    }
};

class SimpleStyleSheetSet : public StyleSheetSet {
public:
    SimpleStyleSheetSet () { }
    SimpleStyleSheetSet (StyleSheetSet* s) : StyleSheetSet (s) { }
    SimpleStyleSheetSet (SimpleStyleSheetSet* s)
            : StyleSheetSet ((StyleSheetSet*)s) { }
    virtual ~SimpleStyleSheetSet();

    friend bool operator== (StyleSheetNode &dn1, StyleSheetNode &dn2) {
        if ((dn1.id == dn2.id) && (dn1.dr == dn2.dr))
            return true;
        return false;
    }

    virtual StyleSheet* getStyleSheet (int dr_id) const;
    virtual bool setStyleSheet (int dr_id, StyleSheet* dr);

    CopyOnWriteable* clone() {
        return (CopyOnWriteable*)(HFCL_NEW_EX(SimpleStyleSheetSet, (this)));
    }

private:
    struct StyleSheetNode {
        int _id;
        StyleSheet* _dr;

        StyleSheetNode (int id, StyleSheet* dr) : _id (id), _dr (dr) { }
        StyleSheetNode () : _id (0) { }
        StyleSheetNode (const StyleSheetNode& n) {
            _id = n._id;
            _dr = n._dr;
        }
    };

    LIST (StyleSheetNode *, StyleSheetList)

    StyleSheetList m_drawables;
    StyleSheetNode* find (int dr_id) const;
};

class HashedStyleSheetSet : public StyleSheetSet {
public:
    HashedStyleSheetSet() { }
    HashedStyleSheetSet(StyleSheetSet* s) : StyleSheetSet(s) { }
    HashedStyleSheetSet(HashedStyleSheetSet* s) : StyleSheetSet(s) { }

    virtual bool setStyleSheet(int dr_id, StyleSheet* dr);
    virtual StyleSheet* getStyleSheet(int dr_id) const;

    CopyOnWriteable * clone() {
        return (CopyOnWriteable*) (HFCL_NEW_EX(HashedStyleSheetSet, (this)));
    }

private:
    MAP (int, StyleSheet*, StyleSheetSetMap)
    StyleSheetSetMap m_drawables;
};

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_STYLESHEETSET_H_ */

