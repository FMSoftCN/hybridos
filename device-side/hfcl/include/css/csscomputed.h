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
#include "../graphics/font.h"

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
    // inherit CSS for anonymous box
    void inherit(const CssComputed* css);
    // inherit a specified property
    void inherit(const CssComputed* css, int pid);
    // inherit a specified property from a view
    void inherit(const View* parent, int pid);
    bool makeAbsolute(const View* view);
    bool validate(const View* view);

    bool getParentPropertyValue(const View* view, CssPropertyIds pid,
            Uint32* value, HTPVData* data) const;

    /* methods for visual formatting */
    bool isFloat() const {
        return (CSS_PPT_VALUE_NOFLAGS(m_values[PID_FLOAT]) != PV_NONE);
    }
    bool isLtr() const {
        return (CSS_PPT_VALUE_NOFLAGS(m_values[PID_DIRECTION]) == PV_LTR);
    }
    bool isAbsolutelyPositioned() const {
        Uint32 value = CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]);
        return (value == PV_FIXED || value == PV_ABSOLUTE);
    }
    bool isPositioned() const {
        Uint32 value = CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]);
        return (value == PV_RELATIVE || value == PV_FIXED ||
                value == PV_ABSOLUTE);
    }
    bool isFixed() const {
        return (CSS_PPT_VALUE_NOFLAGS(m_values[PID_POSITION]) == PV_FIXED);
    }

    bool getZIndex(int& zindex) const;
    unsigned char getOpacity() const;
    bool getHSize(const CssBox* ctnBlock, int pid, HTReal& s) const;
    bool getVSize(const CssBox* ctnBlock, int pid, HTReal& s) const;
    bool getMinWidth(const CssBox* ctnBlock, HTReal& minw) const;
    bool getMaxWidth(const CssBox* ctnBlock, HTReal& maxw) const;
    bool getMinHeight(const CssBox* ctnBlock, HTReal& minh) const;
    bool getMaxHeight(const CssBox* ctnBlock, HTReal& maxh) const;

    const Font* getFont() const;
    // if getFont() const returns NULL, one can call createFont
    Font* createFont() const;

    const Font* getFont();

    void calcPaddings(const CssBox* ctnBlock,
        HTReal* pl, HTReal* pt, HTReal* pr, HTReal* pb) const;

    void autoHMarginsAsZero(const CssBox* ctnBlock,
            HTReal& ml, HTReal& mr) const;

    void autoVMarginsAsZero(const CssBox* ctnBlock,
            HTReal& mt, HTReal& mb) const;

    bool calcWidthsMargins(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);
    bool calcHeightsMargins(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);

    /* overloaded virtual functions */
    virtual bool getProperty(CssPropertyIds pid, Uint32 *value,
            HTPVData *data) const;
    virtual bool setProperty(CssPropertyIds pid, Uint32 value,
            HTPVData data);

private:
    enum {
        PVT_INTERNAL_FONT = MAX_CSS_PVT,
        MAX_CSS_PVT_COMPUTED,
    };

    enum {
        PID_INTERNAL_WIDTH = MAX_CSS_PID,
        PID_INTERNAL_HEIGHT,
        PID_INTERNAL_FONT,
        MAX_CSS_PID_COMPUTED,
    };

    Uint32 m_values[MAX_CSS_PID_COMPUTED];
    HTPVData m_data[MAX_CSS_PID_COMPUTED];

    bool convertArray(int pid, int t);
    bool convertArray(int pid, int t, const RealRect& viewport);
    bool convertArray(int pid, int t, const Font* font, const View* root);
    void freeArray();

    void setFont(Font* font);
    void releaseFont();

    void validateBorderWidth(int pid);
    void validateNotNegative(int pid);

    void handleColor(int pid);
    void handleFontSize(const View* view);
    void handleFontWeight(const View* view);
    bool handleFont();
    void handleTabSize(const Font* font);
    void handleSpacing();
    void handleBorders();
    void handleTextAlign(const View* view, int pid);

    /* methods for widths caculation */
    void calcWidthForIR(const View* view, const CssBox* ctnBlock,
        HTReal& w);
    void calcWidthForBlock(const CssBox* ctnBlock,
        HTReal& w, HTReal& ml, HTReal& mr, bool calced_w);

    void calcWidthsForAPNR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox);
    void calcWidthsForAPR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox);

    bool resolveWHForRIR(const View* view, const CssBox* ctnBlock,
        HTReal& w, HTReal& h);

    bool doCalcWidthsMargins(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);

    /* methods for heights caculation */
    void calcHeightForIR(const View* view, const CssBox* ctnBlock,
        HTReal &w, HTReal& h);
    void calcHeightsForAPNR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox);
    void calcHeightsForAPR(const View* view, const CssBox* ctnBlock,
        CssBox* CssBox);

    void calcHeightForBlockVNR(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);

    void resolveAutoHeights(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);
    bool doCalcHeightsMargins(const View* view, const CssBox* ctnBlock,
            CssBox* cssBox);

#if 0
    bool hasComputedMarginLeft(const RealRect& ctnBlock, HTReal& ml);
    bool hasComputedMarginRight(const RealRect& ctnBlock, HTReal& mr);
    bool hasComputedWidth(const RealRect& ctnBlock, HTReal& w);
    bool hasComputedHeight(const RealRect& ctnBlock, HTReal& h);
#endif

};

} // namespace hfcl

#endif /* HFCL_CSS_CSSCOMPUTED_H_ */
