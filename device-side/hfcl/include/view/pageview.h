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


#ifndef _NGUX_PageView_h
#define _NGUX_PageView_h

#include "panelview.h"

NAMESPACE_BEGIN

class PageView : public PanelView 
{
	public:
		PageView (View *p_parent, DrawableSet* drset)
			:PanelView(p_parent, drset), m_cur_page(NULL) { 
                setLoopAble(true);setFocusValid(true); 
            }
		PageView (NGInt x, NGInt y, NGInt w, NGInt h, NGInt i_id)
			:PanelView(x, y, w, h, i_id) , m_cur_page(NULL) { 
                setLoopAble(true);setFocusValid(true); 
            }

		PageView (View *p_parent)
			:PanelView(p_parent) , m_cur_page(NULL){ 
                setLoopAble(true);setFocusValid(true); 
            }

		virtual NGBool dispatchEvent(Event *event);

		View *nextPage(void);
		View *prevPage(void);

		NGBool setCurPage(View *page);
		View *getCurPage(void) { return m_cur_page; }

		NGBool setCurPageByIndex(const NGUInt index);
		NGInt getCurPageIndex(void);

		NGUInt indexFromPage(View *page) { return getChildIndex(page); }
		View* pageFromIndex(const NGUInt index) { return getChildByIndex(index); }

		NGBool addPage(View *);
		NGBool appendPage(View *);
		NGBool delPage(View *page) { return removeChild(page) == 0; }

		NGInt getPageCount() {return viewCount();}

		void setLoopAble(NGBool isLoop) { setFlag(isLoop, LOOPABLE);}
		NGBool isLoopAble(void) {return m_flags & LOOPABLE;}


	protected:
		enum {
			LOOPABLE = (1 << View::FLAG_SHIFT),
			FLAG_SHIFT = (1 + View::FLAG_SHIFT)
		};

	private:
		View *m_cur_page;
};

NAMESPACE_END

#endif /* NGUX_PageView_h */
