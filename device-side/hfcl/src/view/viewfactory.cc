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

/*
** viewfactory.cc: The implementation of ViewFactory class.
**
** Create by WEI Yongming at 2019/04/23
*/

#include "view/viewfactory.h"

namespace hfcl {

ViewFactory *ViewFactory::s_single = 0;

bool ViewFactory::registerView(const char *vtag, const char* vtype,
        CB_VIEW_CREATOR creator)
{
    std::string tag_type(vtag);
    if (vtype) {
        tag_type.append("[type=");
        tag_type.append(vtype);
        tag_type.append("]");
    }

    TagViewMap::iterator it = m_map.find(tag_type);
    if (it == m_map.end ()) {
        m_map [tag_type] = creator;
        return true;
    }

    return false;
}

View *ViewFactory::create(const char* vtag, const char* vtype,
        const char* vclass, const char* vname, int vid)
{
    std::string tag_type(vtag);
    if (vtype) {
        tag_type.append("[type=");
        tag_type.append(vtype);
        tag_type.append("]");
    }

    TagViewMap::iterator it = m_map.find(tag_type.c_str());
    if (it == m_map.end ()) {
        return NULL;
    }

    CB_VIEW_CREATOR creator = m_map [tag_type.c_str()];
    return creator(vtag, vtype, vclass, vname, vid);
}

void ViewFactory::list()
{
    TagViewMap::iterator it;

    _MG_PRINTF("List of registered tags: \n");
    for (it = m_map.begin(); it != m_map.end(); it++) {
        _MG_PRINTF("\tview creator for tag (%s): %p\n", it->first.c_str(), it->second);
    }
}

} // namespace hfcl

