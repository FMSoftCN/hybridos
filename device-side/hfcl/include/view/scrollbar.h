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


#ifndef HFC_VIEW_SCROLLBAR_H_
#define HFC_VIEW_SCROLLBAR_H_

#include "../view/view.h"
#include "../services/timerservice.h"

namespace hfcl {

class ScrollBar : public TimerEventListener {
public:
    ScrollBar(View *owner);
    virtual ~ScrollBar(void);

    /* \brief this function move the view port to the specific position.
     * * \param[in] x the destination X coordinate.
     * * \param[in] y the destination Y coordinate.
     * */
    void moveViewport(int x, int y, bool bupdate =true);

    void updateContentRect(void);

    void setContent(View* owner);
    inline View* owner(void) const {return m_owner;}

    virtual void draw(GraphicsContext* context, IntRect &rc);

    void getContentSize(int *pw, int *ph) {
        if(pw)
            *pw = m_owner ? m_owner->getRect().width() : 0;
        if(ph)
            *ph = m_owner ? m_owner->getRect().height() : 0;
    }

    bool dispatchEvent(Event*);
    bool handleEvent(Event* event);

    /*
     * set vertical scrollbar and horizontal scrollbar width.
     */
    void setBarWidth(int barwidth);
    void setBarMargin(int barmargin);

    /*
     * set vertical scrollbar and horizontal scrollbar is visible.
     */
    void setVBarVisible(bool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE); }
    void setHBarVisible(bool b_isVisible) { setFlag(b_isVisible, HBAR_VISIBLE); }
    void setBarVisible(bool b_isVisible) { setFlag(b_isVisible, VBAR_VISIBLE | HBAR_VISIBLE); }

    bool vBarVisible(void) { return (m_flags & VBAR_VISIBLE) == VBAR_VISIBLE; }
    bool hBarVisible(void) { return (m_flags & HBAR_VISIBLE) == HBAR_VISIBLE; }

    void setLoopScroll(bool b) { setFlag(b, LOOP_SCROLL); }
    bool loopScroll() { return m_flags & LOOP_SCROLL; }
    bool autoShowBar() { return m_flags& AUTOSHOWBAR; }
    bool scrollVisible(int * barWidth, bool isVertical = TRUE);

    void setAutoShowBar(bool b) { setFlag(isScrollBarAlwaysShow() ? true : b, AUTOSHOWBAR); }

    void setScrollBarAlwaysShow(bool b) { setFlag(b, ALWAYS_SHOW); }
    bool isScrollBarAlwaysShow(void) { return (m_flags & ALWAYS_SHOW) == ALWAYS_SHOW; }

    void setScrollBarFloating(bool b) { setFlag(b, BAR_FLOATING); }
    bool isScrollBarFloating(void) { return (m_flags & BAR_FLOATING) == BAR_FLOATING; }

    void setVScrollBarOnLeft(bool b) { setFlag(b, VBAR_ON_LEFT); }
    bool isVScrollBarOnLeft(void) { return (m_flags & VBAR_ON_LEFT) == VBAR_ON_LEFT; }

    void setVStep(Uint16 vstep){m_vStep = vstep;}
    void setHStep(Uint16 hstep){m_hStep = hstep;}

protected:
    enum {
        LOOP_SCROLL   = (0x01 << 0), //scroll the owner with loop
        HBAR_VISIBLE  = (0x01 << 1),
        VBAR_VISIBLE  = (0x01 << 2),
        AUTOSHOWBAR   = (0x01 << 3),
        ALWAYS_SHOW   = (0x01 << 4),
        BAR_FLOATING  = (0x01 << 5),
        VBAR_ON_LEFT  = (0x01 << 6),
    };

    inline bool setFlag(bool b, unsigned int flag) {
        bool old = m_flags & flag;
        if(b)
            m_flags |= flag;
        else
            m_flags &= (~flag);
        return old;
    }

protected:
    View* m_owner;
    Uint16 m_barWidth;
    Uint16 m_barMargin;
    Uint16 m_vStep;
    Uint16 m_hStep;
    int m_offx;
    int m_offy;
    Uint32 m_flags;
    int m_timerId;

private:
    void moveOffsetH(int x);
    void moveOffsetV(int y);
};

} // namespace hfcl

#endif /* HFC_VIEW_SCROLLBAR_H_ */

