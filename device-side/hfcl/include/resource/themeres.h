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

#ifndef HFCL_RESOURCE_THEMERES_H_
#define HFCL_RESOURCE_THEMERES_H_

#include "../common/common.h"
#include "../common/stlalternative.h"
#include "../resource/restypes.h"
#include "../drawable/drawable.h"

namespace hfcl {

class ThemeRes {
public:
    ThemeRes();
    ~ThemeRes();

    void setDrawableSetMapTable(TRThemeItem* items);
    DrawableSet* getDrawableSet(int theme_drset_id);

private:
    TRThemeItem* m_maptable;
    string m_theme_name;
};

} // namespace hfcl

#endif /* HFCL_RESOURCE_THEMERES_H_ */

