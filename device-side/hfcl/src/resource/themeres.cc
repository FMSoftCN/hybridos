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

#include <string.h>

#include "graphicscontext.h"
#include "drawable.h"
#include "resloader.h"
#include "respkgmanager.h"
#include "themeres.h"

NAMESPACE_BEGIN

ThemeRes::ThemeRes()
{
	m_theme_name = "default_theme";
	m_maptable = NULL;
}

ThemeRes::~ThemeRes()
{
}

void ThemeRes::setDrawableSetMapTable(TRThemeItem* items)
{
	m_maptable = items;
}

DrawableSet* ThemeRes::getDrawableSet(int theme_drset_id)
{
	int drset_id = 0, i = 0;
	if(m_maptable != NULL) {
		while(m_maptable[i].theme_drset_id != -1) {
			if(m_maptable[i].theme_drset_id == theme_drset_id) {
				drset_id = m_maptable[i].drset_id;
				break;
			}
			++i;
		}
		if(drset_id > 0)
			return GetDrawableSetRes(drset_id);
	}

	return GetCommonDrawableSet();
}

NAMESPACE_END

