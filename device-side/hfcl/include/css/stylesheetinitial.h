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

#ifndef HFCL_CSS_STYLESHEETINITIAL_H_
#define HFCL_CSS_STYLESHEETINITIAL_H_

#include "../common/common.h"
#include "../common/stlalternative.h"
#include "propertyvalue.h"
#include "stylesheet.h"

namespace hfcl {

class StyleSheetInitial : public StyleSheet {
public:
    StyleSheetInitial ();
    ~StyleSheetInitial () {}

    DWORD32 getPropertyFlags (PropertyIds pid) { return m_flags[pid]; }
    virtual bool getProperty (PropertyIds pid, DWORD32 *value,
            HTData *data = NULL) {
        if (value) {
            *value = m_values[pid];
        }
        if (data) {
            *data = m_data[pid];
        }
        return true;
    }
    virtual bool setProperty (PropertyIds pid, DWORD32 value,
            HTData data = 0) {
        m_values[pid] = value;
        m_data[pid] = data;
        return true;
    }

    static StyleSheetInitial* getSingleton(void) {
        if (s_singleton == NULL)
            s_singleton = HFCL_NEW_EX(StyleSheetInitial, ());
        return s_singleton;
    }

private:
    static StyleSheetInitial* s_singleton;

    DWORD32 m_values[MAX_PID];
    DWORD32 m_flags[MAX_PID];
    HTData m_data[MAX_PID];
};

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_STYLESHEETINITIAL_H_ */

