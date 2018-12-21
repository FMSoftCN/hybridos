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

#ifndef HFCL_CSS_CSSCOMPUTED_H_
#define HFCL_CSS_CSSCOMPUTED_H_

#include "../common/common.h"
#include "../common/stlalternative.h"
#include "../css/css.h"
#include "../css/csspropertyvalue.h"

#include "../view/view.h"

namespace hfcl {

class CssComputed : public Css {
public:
    CssComputed () {}
    ~CssComputed ();

    virtual bool getProperty (CssPropertyIds pid, DWORD32 *value,
            HTData *data = NULL);
    virtual bool setProperty (CssPropertyIds pid, DWORD32 value,
            HTData data = 0);

private:
    DWORD32 m_values[MAX_CSS_PID];
    HTData m_data[MAX_CSS_PID];
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSCOMPUTED_H_ */
