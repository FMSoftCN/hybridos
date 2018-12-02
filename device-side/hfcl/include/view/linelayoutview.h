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


#ifndef __NGUX_LINELAYOUTVIEW_H__
#define __NGUX_LINELAYOUTVIEW_H__

#include "layoutview.h"

NAMESPACE_BEGIN

typedef struct __tag_LineLayoutNode {
    View* s_view;
    NGUInt flags : 8;
    NGUInt defMinSize : 12;
    NGUInt defMaxSize : 12;
    struct __tag_LineLayoutNode* prev;
    struct __tag_LineLayoutNode* next;
} LineLayoutNode, *PLineLayoutNode;

class LineLayoutView : public LayoutView {
    public:
        LineLayoutView();

		LineLayoutView(View* parent, DrawableSet* drset);

        ~LineLayoutView();

		void paint(GraphicsContext* context, NGInt status /*= Style::NORMAL*/);
		NGBool dispatchEvent(Event* event);
		void paint(GraphicsContext* context = NULL);
		void paint(GraphicsContext* context, const IntRect& irc);
        NGBool setRect(const IntRect& pirc);
		// to void drived class overide the base class 's function
        NGBool setRect(NGInt left, NGInt top, NGInt right, NGInt bottom) { return View::setRect(left, top, right, bottom); }

        void recalc(void);
        NGBool prependView(View* aView = NULL, NGInt aType = 0, NGInt aMinSize = 0, NGInt aMaxSize = 0);
        NGBool addView(View* aView = NULL, NGInt aType = 0, NGInt aMinSize = 0, NGInt aMaxSize = 0);
        NGBool appendView(View* aView = NULL, NGInt aType = 0, NGInt aMinSize = 0, NGInt aMaxSize = 0);
        LineLayoutNode* findNode(View* aView = NULL);
        void removeAll(void);
        NGInt  getNodeCount(void);

        void setLineHeight(NGInt aLineHeight = 0)
        {
            if (aLineHeight <= 0)
                return;

            m_lineHeight = aLineHeight;
        }

        NGInt  getLineHeight(void) const { return m_lineHeight; }

        void setMargin(NGInt margin = 0);
        void setSpace(NGInt space = 0);

        LineLayoutView(View *parent);
    protected:
        //void initDrawable(DrawableSelector* selector);

    private:
        LineLayoutNode* newLineLayoutNode(View* aView = NULL, NGInt aType = 0, NGInt aMinSize = 0, NGInt aMaxSize = 0);
        NGBool isExist(View* aView = NULL);
        void horizontalRecalc(void);
        void fixedHorzRecalc(void);
        void autoHorzRecalc(void);
        void verticalRecalc(void);
        void fixedVertRecalc(void);
        void autoVertRecalc(void);
        void remainingClearedNodeRect(LineLayoutNode* node);

        LineLayoutNode* m_node;
        LineLayoutNode* m_tailNode;
        NGInt             m_lineHeight;
};

NAMESPACE_END

#endif  /* __NGUX_LINELAYOUTVIEW_H__ */

