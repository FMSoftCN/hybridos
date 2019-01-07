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
#include "../common/realrect.h"
#include "../css/css.h"
#include "../css/csspropertyvalue.h"
#include "../css/cssbox.h"

#include "../view/view.h"

namespace hfcl {

#define NR_PID_INTERNAL_USE     2

class CssComputed : public Css {
public:
    CssComputed();
    CssComputed(const CssComputed& init);
    ~CssComputed() { freeArray(); }

    /* public methods */
    void reset();
    void inherit(const CssComputed* css);
    bool makeAbsolute(const View& view);
    bool validate(const View& view);

    /* methods for visual formatting */
    bool isAbsolutelyPositioned() const {
        return (m_values[PID_POSITION] == PV_FIXED ||
                m_values[PID_POSITION] == PV_ABSOLUTE);
    }
    bool isFloating() const {
        return m_values[PID_FLOAT] != PV_NONE;
    }
    bool isLtr() const {
        return (m_values[PID_DIRECTION] == PV_LTR);
    }

    bool getHSize(const CssBox& ctnBlock, int pid, HTReal& s) const;
    bool getVSize(const CssBox& ctnBlock, int pid, HTReal& s) const;
    bool getMinWidth(const CssBox& ctnBlock, HTReal& minw) const;
    bool getMaxWidth(const CssBox& ctnBlock, HTReal& maxw) const;
    bool getMinHeight(const CssBox& ctnBlock, HTReal& minh) const;
    bool getMaxHeight(const CssBox& ctnBlock, HTReal& maxh) const;

    void calcPaddings(const CssBox& ctnBlock,
        HTReal* pl, HTReal* pt, HTReal* pr, HTReal* pb) const;

    void autoHMarginsAsZero(const CssBox& ctnBlock,
            HTReal& ml, HTReal& mr) const;

    void autoVMarginsAsZero(const CssBox& ctnBlock,
            HTReal& mt, HTReal& mb) const;

    void calcBox(const View& view, const CssBox& ctnBlock,
        const CssBox* prevBox, CssBox* currBox);

    /* overloaded virtual functions */
    virtual bool getProperty(CssPropertyIds pid, Uint32 *value,
            HTPVData *data) const;
    virtual bool setProperty(CssPropertyIds pid, Uint32 value,
            HTPVData data);

private:
    enum {
        PID_INTERNAL_WIDTH = MAX_CSS_PID,
        PID_INTERNAL_HEIGHT,
        MAX_CSS_PID_COMPUTED,
    };

    Uint32 m_values[MAX_CSS_PID_COMPUTED];
    HTPVData m_data[MAX_CSS_PID_COMPUTED];

    bool convertArray(int pid, int t);
    bool convertArray(int pid, int t, const RealRect& viewport);
    bool convertArray(int pid, int t, const View& view);
    void freeArray();

    void validateBorderWidth(int pid);
    void validateNotNegative(int pid);

    /* methods for widths caculation */
    void calcWidthForIR(const View& view, const CssBox& ctnBlock,
        HTReal& w);
    void calcWidthForBlock(const CssBox& ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr, bool calced_w);

    void calcWidthsForAPNR(const View& view, const CssBox& ctnBlock,
        CssBox* box);
    void calcWidthsForAPR(const View& view, const CssBox& ctnBlock,
        CssBox* box);

    bool resolveWHForRIR(const View& view, const CssBox& ctnBlock,
        HTReal& w, HTReal& h);

    bool calcWidthsMargins(const View& view, const CssBox& ctnBlock,
            const CssBox* prevBox, CssBox* currBox);

    /* methods for heights caculation */
    void calcHeightForIR(const View& view, const CssBox& ctnBlock,
        HTReal &w, HTReal& h);
    void calcHeightsForAPNR(const View& view, const CssBox& ctnBlock,
        CssBox* box);
    void calcHeightsForAPR(const View& view, const CssBox& ctnBlock,
        CssBox* box);

    void calcHeightForBlockVNR(const View& view, const CssBox& ctnBlock,
            CssBox* currBox);

    void resolveAutoHeights(const View& view, const CssBox& ctnBlock,
            const CssBox* prevBox, CssBox* currBox);
    bool calcHeightsMargins(const View& view, const CssBox& ctnBlock,
            const CssBox* prevBox, CssBox* currBox);

#if 0
    bool hasComputedMarginLeft(const RealRect& ctnBlock, HTReal& ml);
    bool hasComputedMarginRight(const RealRect& ctnBlock, HTReal& mr);
    bool hasComputedWidth(const RealRect& ctnBlock, HTReal& w);
    bool hasComputedHeight(const RealRect& ctnBlock, HTReal& h);
#endif

};

} // namespace hfcl

#endif /* HFCL_CSS_CSSCOMPUTED_H_ */
