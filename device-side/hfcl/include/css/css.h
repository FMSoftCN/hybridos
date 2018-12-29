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

#ifndef HFCL_CSS_CSS_H_
#define HFCL_CSS_CSS_H_

#include "../common/common.h"
#include "../common/object.h"
#include "../common/stlalternative.h"
#include "../css/csspropertyvalue.h"

namespace hfcl {

class Css : public RefCount {
public:
    Css() {}
    virtual ~Css() {}

    virtual bool getProperty(CssPropertyIds pid, Uint32 *value,
            HTPVData* data) = 0;
    virtual bool setProperty(CssPropertyIds pid, Uint32 value,
            HTPVData data) = 0;

    bool getProperty(CssPropertyIds pid, Uint32 *value) {
        HTPVData data;
        if (getProperty(pid, value, &data)) {
            return true;
        }
        return false;
    }
    bool getProperty(CssPropertyIds pid, Uint32 *value, HTInt* i) {
        HTPVData data;
        if (getProperty(pid, value, &data)) {
            if (i) *i = data.i;
            return true;
        }
        return false;
    }
    bool getProperty(CssPropertyIds pid, Uint32 *value, HTUint* u) {
        HTPVData data;
        if (getProperty(pid, value, &data)) {
            if (u) *u = data.u;
            return true;
        }
        return false;
    };
    bool getProperty(CssPropertyIds pid, Uint32 *value, HTReal* r) {
        HTPVData data;
        if (getProperty(pid, value, &data)) {
            if (r) *r = data.r;
            return true;
        }
        return false;
    }
    bool getProperty(CssPropertyIds pid, Uint32 *value, HTPointer* p) {
        HTPVData data;
        if (getProperty(pid, value, &data)) {
            if (p) *p = data.p;
            return true;
        }
        return false;
    }

    bool setProperty(CssPropertyIds pid, Uint32 value) {
        HTPVData data;
        data.p = 0;
        return setProperty(pid, value, data);
    }
    bool setProperty(CssPropertyIds pid, Uint32 value, HTInt i) {
        HTPVData data;
        data.i = i;
        return setProperty(pid, value, data);
    }
    bool setProperty(CssPropertyIds pid, Uint32 value, HTUint u) {
        HTPVData data;
        data.u = u;
        return setProperty(pid, value, data);
    }
    bool setProperty(CssPropertyIds pid, Uint32 value, HTReal r) {
        HTPVData data;
        data.r = r;
        return setProperty(pid, value, data);
    }
    bool setProperty(CssPropertyIds pid, Uint32 value, HTPointer p) {
        HTPVData data;
        data.p = p;
        return setProperty(pid, value, data);
    }

    // Keyword type (bitmask)
    enum {
        CSS_KW_UNKNOWN = 0,
        CSS_KW_PSEUDO_ELEMENT       = (0x00010000 << 0),
        CSS_KW_PSEUDO_CLASS         = (0x00010000 << 1),
        CSS_KW_DYNAMIC_PSEUDO_CLASS = (0x00010000 << 2),
    };

    // Keyword subtype
    enum {
        CSS_KWST_FIRST_CHILD,
        CSS_KWST_ONLY_CHILD,
        CSS_KWST_LAST_CHILD,
        CSS_KWST_NTH_CHILD,
        CSS_KWST_NTH_LAST_CHILD,
        CSS_KWST_LINK,
        CSS_KWST_VISITED,
        CSS_KWST_HOVER,
        CSS_KWST_ACTIVE,
        CSS_KWST_FOCUS,
        CSS_KWST_DISABLED,
        CSS_KWST_ENABLED,
        CSS_KWST_CHECKED,
        CSS_KWST_ROOT,
        CSS_KWST_EMPTY,
        CSS_KWST_FIRST_LINE,
        CSS_KWST_FIRST_LETTER,
        CSS_KWST_BEFORE,
        CSS_KWST_AFTER,
    };

    static Uint32 getKeywordType(const char* name);
};

} // namespace hfcl

#endif /* HFCL_CSS_CSS_H_ */

