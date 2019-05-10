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

#ifndef _HFCL_VIEW_PANELVIEW_H_
#define _HFCL_VIEW_PANELVIEW_H_

#include "../view/viewcontainer.h"
#include "../graphics/graphicscontext.h"

namespace hfcl {

class PanelView : public ViewContainer {
public:
    PanelView(const char* vtag, const char* vtype,
            const char* vclass = NULL, const char* vname = NULL, int vid = 0);
    virtual ~PanelView();

protected:

private:
};

} // namespace hfcl

#endif /* HFCL_VIEW_PANELVIEW_H_ */
