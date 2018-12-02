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
** along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef __NGUX_STATUSBARVIEW_H__
#define __NGUX_STATUSBARVIEW_H__

#include "panelview.h"

NAMESPACE_BEGIN

class StatusBarView : public PanelView {
    public:
        enum {
            SHOW_IN_LEFT,
            SHOW_IN_RIGHT
        };

        struct SBViewNode {
            int id;
            int location;
            View* pv;
            int pvWidth;
            int pvHeight;

            SBViewNode(int i_id, int loca, View* v, int w = 0, int h = 0)
                : id(i_id), location(loca), pv(v), pvWidth(w), pvHeight(h) { }
            SBViewNode()
                : id(0), location(SHOW_IN_RIGHT), pv(NULL), pvWidth(0), pvHeight(0) { }
            SBViewNode(const SBViewNode& n) { 
                id = n.id; location = n.location;
                pv = n.pv; pvWidth = n.pvWidth; pvHeight = n.pvHeight; 
            }
        };

        //LIST(SBViewNode *, SBVList)
		LISTEX(SBViewNode *, SBVList, do{return *v1 == *v2;}while (0), do{NGUX_DELETE(*n);} while (0));

        StatusBarView(View* p_parent);
        StatusBarView(View* p_parent, DrawableSet* dr);

        void updateStatusBarView(void);

        void setSpace(int space) { m_space = space; }
        int getSpace(void) { return m_space; }

        void setWidth(int width) { m_width = width; }
        int getWidth(void) { return m_width; }

        virtual bool dispatchEvent(Event* event);

        friend bool operator==(SBViewNode& sbvn1, SBViewNode& sbvn2) {
            if ((sbvn1.id == sbvn2.id) && (sbvn1.location == sbvn2.location) && (sbvn1.pv == sbvn2.pv))
                return true;
            return false;
        }

        bool setSBView(int loca, View* v);
        View* getSBView(int v_id) const;
        SBViewNode* find(int v_id) const;
        void setAutoRecalc(bool bsar = false);
        // void RefreshViewState(int id, int level = 0);

    protected:
        DECLARE_CLASS_NAME(StatusBarView)

    private:
        void autoRecalc(void);
        void resetViewRect(SBViewNode* sbvn);

        SBVList  m_sbwList;
        Uint16 m_space;
        Uint16 m_width;
        int    m_sbvLeft;
        int    m_sbvRight;
};

NAMESPACE_END

#endif  // __NGUX_STATUSBARVIEW_H__

