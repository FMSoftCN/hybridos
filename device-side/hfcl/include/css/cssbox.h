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

#ifndef HFCL_CSS_CSSBOX_H_
#define HFCL_CSS_CSSBOX_H_

#include "../common/common.h"
#include "../common/object.h"
#include "../common/stlalternative.h"
#include "../view/view.h"

namespace hfcl {

class CssBox : public Object {
public:
    CssBox();
    virtual ~CssBox() {};

protected:
    struct Box {
        int left;
        int top;
        int right;
        int bottom;
    };

    struct Box m_box;
};

class CssBoxPrincipal : public CssBox {
public:
    CssBoxPrincipal(View* view) : m_view(view) {}
    virtual ~CssBoxPrincipal();

protected:
    bool appendChildBox(CssBox* child);
    CssBox* getChild(int idx);

private:
    VECTOR(CssBox*, CssBoxVec);

    View* m_view;
    CssBoxVec m_children;
};

namespace css {

} // namespace css

} // namespace hfcl

#endif /* HFCL_CSS_CSSBOX_H_ */

