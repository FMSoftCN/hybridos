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
#include "../css/cssbox.h"

namespace hfcl {

typedef struct _CssBoxWithZIndex {
    CssBox*    cssbox;
    int        zindex;
} CssBoxWithZIndex;

class CssStackingContext {
public:
    CssStackingContext();
    virtual ~CssStackingContext();

    bool append(CssBox* cssbox, int zindex);
    void sort();

    const std::vector<CssBoxWithZIndex>& getVec() const {
        return m_boxes;
    }

    std::vector<CssBoxWithZIndex>& getVec() {
        return m_boxes;
    }

protected:

private:
    std::vector<CssBoxWithZIndex> m_boxes;
};

} // namespace hfcl

#endif /* HFCL_CSS_CSSSTACKINGCONTEXT_H_ */

