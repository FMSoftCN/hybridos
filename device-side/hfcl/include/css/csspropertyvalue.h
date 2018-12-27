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

#ifndef HFCL_CSS_CSSPROPERTYVALUE_H_
#define HFCL_CSS_CSSPROPERTYVALUE_H_

#include "../common/common.h"
#include "../common/stlalternative.h"

namespace hfcl {

#define MAKE_CSS_PPT_VALUE(type, keyword)    \
    ((Uint32)(                               \
        ((Uint32)((keyword) & 0xFFFF)) |     \
        ((Uint32)((type) & 0x0FFF) << 16)    \
    ))

#define CSS_PPT_VALUE_NOFLAGS(value)    ((value) & 0x0FFFFFFF)
#define CSS_PPT_VALUE_TYPE(value)       (((value) & 0x0FFF0000) >> 16)
#define CSS_PPT_VALUE_KEYWORD(value)    ((value) & 0x0000FFFF)

#define CSS_PPT_VALUE_FLAG_INHERITED        0x80000000
#define CSS_PPT_VALUE_FLAG_NOT_INHERITED    0x00000000
#define CSS_PPT_VALUE_FLAG_COMPUTED         0x40000000
#define CSS_PPT_VALUE_FLAG_IMPORTANT        0x20000000

#define MAKE_CSS_PPT_INITIAL_VALUE(inherited, type, keyword)    \
    ((Uint32)(                                                  \
        ((Uint32)((keyword) & 0xFFFF)) |                        \
        ((Uint32)((type) & 0x0FFF) << 16) |                     \
        (inherited)                                             \
    ))

#define IS_CSS_PPT_VALUE_INHERITED(value) \
    ((value) & CSS_PPT_VALUE_FLAG_INHERITED)
#define MARK_CSS_PPT_VALUE_INHERITED(value) \
    ((value) |= CSS_PPT_VALUE_FLAG_INHERITED)

#define IS_CSS_PPT_VALUE_COMPUTED(value) \
    ((value) & CSS_PPT_VALUE_FLAG_COMPUTED)
#define MARK_CSS_PPT_VALUE_COMPUTED(value) \
    ((value) |= CSS_PPT_VALUE_FLAG_COMPUTED)

#define IS_CSS_PPT_VALUE_IMPORTANT(value) \
    ((value) & CSS_PPT_VALUE_FLAG_IMPORTANT)
#define MARK_CSS_PPT_VALUE_IMPORTANT(value) \
    ((value) |= CSS_PPT_VALUE_FLAG_IMPORTANT)

#include "csspropertyvalue.inc"

// Data types for property value
typedef int             HTInt;
typedef unsigned int    HTUint;
typedef const void*     HTPointer;

typedef union _HTPVData {
    HTInt       i;
    HTUint      u;
    HTReal      r;
    HTPointer   p;
} HTPVData;

class CssPropertyValue {
public:
    CssPropertyValue();
    CssPropertyValue(Uint32 value) {
        m_value = value; m_data.p = 0;
    }
    CssPropertyValue(Uint32 value, HTPVData data) {
        m_value = value; m_data = data;
    }
    CssPropertyValue(Uint32 value, HTInt i) {
        m_value = value; m_data.i = i;
    }
    CssPropertyValue(Uint32 value, HTUint u) {
        m_value = value; m_data.u = u;
    }
    CssPropertyValue(Uint32 value, HTReal r) {
        m_value = value; m_data.r = r;
    }
    CssPropertyValue(Uint32 value, HTPointer p) {
        m_value = value; m_data.p = p;
    }
    ~CssPropertyValue() {};

    bool isInherited() const {
        return (bool)IS_CSS_PPT_VALUE_INHERITED(m_value);
    }
    bool isComputed() const {
        return (bool)IS_CSS_PPT_VALUE_COMPUTED(m_value);
    }
    bool isImportant() const {
        return (bool)IS_CSS_PPT_VALUE_IMPORTANT(m_value);
    }

    Uint32 getValue() const { return m_value; }
    int getType() const { return CSS_PPT_VALUE_TYPE(m_value); }
    int getKeyword() const { return CSS_PPT_VALUE_KEYWORD(m_value); }

    HTPVData getData() const { return m_data; }
    HTInt getInt() const { return m_data.i; }
    HTUint getUint() const { return m_data.u; }
    HTReal getReal() const { return m_data.r; }
    HTPointer getPointer() const { return m_data.p; }

    void setValue(Uint32 value) {
        m_value = value;
        m_data.p = 0;
    }
    void setValue(Uint32 value, HTPVData data) {
        m_value = value;
        m_data = data;
    }
    void setValue(Uint32 value, HTInt i) {
        m_value = value;
        m_data.i = i;
    }
    void setValue(Uint32 value, HTUint u) {
        m_value = value;
        m_data.u = u;
    }
    void setValue(Uint32 value, HTReal r) {
        m_value = value;
        m_data.r = r;
    }
    void setValue(Uint32 value, HTPointer p) {
        m_value = value;
        m_data.p = p;
    }

private:
    Uint32 m_value;
    HTPVData m_data;
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSPROPERTYVALUE_H_ */
