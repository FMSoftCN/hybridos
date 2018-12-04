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


#ifndef _HFCL_PageView_h
#define _HFCL_PageView_h

#include "panelview.h"

namespace hfcl {

class PageView : public PanelView 
{
	public:
		PageView (View *p_parent, DrawableSet* drset)
			:PanelView(p_parent, drset), m_cur_page(NULL) { 
                setLoopAble(true);setFocusValid(true); 
            }
		PageView (int x, int y, int w, int h, int i_id)
			:PanelView(x, y, w, h, i_id) , m_cur_page(NULL) { 
                setLoopAble(true);setFocusValid(true); 
            }

		PageView (View *p_parent)
			:PanelView(p_parent) , m_cur_page(NULL){ 
                setLoopAble(true);setFocusValid(true); 
            }

		virtual bool dispatchEvent(Event *event);

		View *nextPage(void);
		View *prevPage(void);

		bool setCurPage(View *page);
		View *getCurPage(void) { return m_cur_page; }

		bool setCurPageByIndex(const unsigned int index);
		int getCurPageIndex(void);

		unsigned int indexFromPage(View *page) { return getChildIndex(page); }
		View* pageFromIndex(const unsigned int index) { return getChildByIndex(index); }

		bool addPage(View *);
		bool appendPage(View *);
		bool delPage(View *page) { return removeChild(page) == 0; }

		int getPageCount() {return viewCount();}

		void setLoopAble(bool isLoop) { setFlag(isLoop, LOOPABLE);}
		bool isLoopAble(void) {return m_flags & LOOPABLE;}


	protected:
		enum {
			LOOPABLE = (1 << View::FLAG_SHIFT),
			FLAG_SHIFT = (1 + View::FLAG_SHIFT)
		};

	private:
		View *m_cur_page;
};

} // namespace hfcl

#endif /* HFCL_PageView_h */
