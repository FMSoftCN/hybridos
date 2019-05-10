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

#ifndef HFCL_CSS_CSSSTACKINGCONTEXT_H_
#define HFCL_CSS_CSSSTACKINGCONTEXT_H_

#include <vector>

#include "../common/common.h"
#include "../graphics/graphicscontext.h"

namespace hfcl {

class CssStackingContext {
public:
    CssStackingContext(const View* view, int zindex,
            unsigned char opacity = 0xFF);
    virtual ~CssStackingContext();

    int getZIndex() const { return m_zindex; }
    bool append(CssStackingContext* child) {
        m_children.push_back(child);
        return true;
    }

    void sort();

protected:
    const View*         m_view;
    int                 m_zindex;
    unsigned char       m_opacity;
    GraphicsContext*    m_mem_gc;
    std::vector<CssStackingContext*> m_children;
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSSTACKINGCONTEXT_H_ */

