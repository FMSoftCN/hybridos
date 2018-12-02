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


#ifndef _NGUX_ListView_h
#define _NGUX_ListView_h

#include "scrollview.h"
#include "itemview.h"

NAMESPACE_BEGIN

class Drawable;
class ScrollView;
class ItemView;
		
LIST(MGEFF_ANIMATION, AniList);

class ListView : public ScrollView 
{
    public:

        ListView(View *p_parent);
        ListView(View *p_parent, DrawableSet* drset);
        virtual ~ListView(void);

        /* \brief item count.
         * * \return item count.
         * */
        NGInt       itemCount(void)              { return m_content ? m_content->viewCount():0; }
        ItemView *hilightItem(void)            {return m_hilightItem; };
        NGInt       hilightItemIndex(void)       {return indexFromItem(m_hilightItem); };
        ItemView *itemFromIndex(NGInt index)     { return m_content ? (ItemView*)m_content->getChildByIndex(index):NULL; }
        NGInt       indexFromItem(ItemView *item){ return m_content ? m_content->getChildIndex(item): -1; }
        View* firstEnableItem(void) { return m_content ? m_content->firstEnableChild():NULL;  }

        //NGInt  headGap(void)      {return m_headGap;};
        //void setHeadGap(NGInt gap){m_headGap = gap;};

        NGInt  vGap(void)      {return m_vGap;};
        NGInt  hGap(void)      {return m_hGap;};
        void setVGap(NGInt gap){m_vGap = gap;};
        void setHGap(NGInt gap){m_hGap = gap;};
		void setScrollGap(NGInt gapPixel){m_scrollGap = gapPixel;}
		NGInt scrollGap(void){return m_scrollGap;}
        //NGInt  tailGap(void)      {return m_tailGap;};
        //void setTailGap(NGInt gap){m_tailGap = gap;};

        /* \brief judge is a item is added to listview 
         * * \param[in] item the item pointer.
         * * \return NGBool true if is, or false
         * */
        NGBool isItem(ItemView *item) { return isChild(item); }

        /* \brief add a item.By default, add at the list tail.
         * * \param[in] item the item pointer to add.
         * * \return NGBool true if success , or false
         * */
        NGBool addItemByIndex(ItemView *item, NGInt index);
        NGBool addItem(ItemView *item) { return addItemByIndex(item, -1); }
        NGBool addItem(View *item)     { return addItem ((ItemView*)item);}
        NGBool addItemWithAppend(ItemView *item);
        /* \brief remove a item at by index. 
         * * \param[in] index the item index to remove.
         * * \return NGBool true if success , or false
         * */
        NGBool removeItemByIndex(NGInt index, NGBool bRelease = true) { return removeItem(itemFromIndex(index)); }
		NGBool removeItem(ItemView* item, NGBool bRelease = true);
        void clear(void);

        /* \brief hilight a item.
         * * \param[in] NGInt the index of item to hilight.
         * * \return NGBool true if success , or false
         * */
		virtual NGBool hilight(ItemView *view);
        virtual NGBool hilight(NGInt index) { return hilight(itemFromIndex(index)); }
//        NGBool selectedItem(NGInt index);

        /* \brief select a item.
         * * \param[in] NGInt the index of item to select.
         * * \return NGBool true if success , or false
         * */
        NGBool select(NGInt index, bool toSelect);
        NGBool singleSelect(NGInt index);
        ItemView* singleSelectItem(void) { return m_curr_single_selected; }

        void deSelectAll() { selectAll(false); }
        void selectAll(NGBool bselect=true);


        NGBool         dispatchEvent(Event *event);
		virtual void drawContent(GraphicsContext* context, IntRect &rc, NGInt status /*= Style::NORMAL*/);

        /* \brief this function set the separator width.
         * * \param[in] w the width of separator.
         * */
        void      setSeparatorWidth(NGInt w);
        //Drawable *separator(void) { return (Drawable*)m_drawable.getElement(SYS_SE_DRSEPERATOR); }
        //void      setSeparator(Drawable *separator) { m_drawable.setElement(SYS_SE_DRSEPERATOR, (DWORD)separator); }

        void freeze(NGBool freeze);

        void drawSeparator(GraphicsContext* context, NGInt status/* = Style::NORMAL*/);

		ItemView* firstItem()const { return m_content ? (ItemView*) m_content->firstChild() : NULL; }
		ItemView* lastItem()const  { return m_content ? (ItemView*) m_content->lastChild() : NULL; }

		ItemView* itemAt(NGInt x, NGInt y, NGInt *pidx)const;
		ItemView* itemAt(NGInt x, NGInt y) const { return itemAt(x, y, NULL); }
		ItemView* firstVisibleItem()const  { return itemAt(m_offx + m_hGap, m_offy + m_vGap + m_separatorWidth); }
		ItemView* lastVisibleItem()const   { return itemAt(m_offx + m_hGap, m_offy + getRect().height() - m_vGap - m_separatorWidth); }

		ItemView* itemFromId(NGInt iid) { return (ItemView*) (m_content ? m_content->getView(iid) : NULL); }

		NGInt itemIndexAt(NGInt x, NGInt y) const { 
			NGInt idx;
			itemAt(x, y, &idx);
			return idx;
		}
		NGInt firstVisibleItemIndex() const { return itemIndexAt(m_offx + m_hGap, m_offy + m_vGap + m_separatorWidth); }
		NGInt lastVisibleItemIndex() const  { return itemIndexAt(m_offx + m_hGap, m_offy + getRect().height() - m_vGap - m_separatorWidth); }
        
       // virtual NGBool handleEvent(Event* event);     // GT_jyseo rollback 

	protected:
		class ItemIterator {
		protected:
			ItemIterator(ItemView* first) : m_current (first) { }	
			virtual NGBool check(ItemView *view) = 0;
			void init(const ItemIterator& it) { m_current = it.m_current; }

			ItemView* m_current;
		public:
			ItemIterator(const ItemIterator& it) { init(it); }
			ItemIterator(): m_current(NULL) { }

			NGBool isEnd() { return m_current == NULL; }
			void prev() { 
				while(m_current && !check((m_current = (ItemView*)m_current->previousSibling()))) ;
			}
			void next() { 
				while(m_current && !check((m_current = (ItemView*)m_current->nextSibling()))) ;
			}

			ItemView* operator->() { return m_current; }
			ItemView* operator*() { return m_current; }
		};

	public:
		class VisibleItemIterator : public ItemIterator{
			friend class ListView;
			NGInt m_bottom;
			VisibleItemIterator(ItemView* first, NGInt bottom) : ItemIterator(first), m_bottom(bottom) { }

			NGBool check(ItemView* view) { 
				if(!view) 
					return false;
				if(view->getRect().bottom() > m_bottom) {
					m_current = NULL;
					return false;
				}
				return true;
			}
		public:
			VisibleItemIterator() : m_bottom(0) { }
			VisibleItemIterator(const VisibleItemIterator & it) : ItemIterator(it) { m_bottom = it.m_bottom; }
			VisibleItemIterator& operator=(const VisibleItemIterator& it) { 
				ItemIterator::init(it);
				m_bottom = it.m_bottom;
				return *this;
			}
		};
		VisibleItemIterator getVisiableItems() const { return VisibleItemIterator(firstVisibleItem(), getRect().height() + m_offy); }

		class SelectedItemIterator : public ItemIterator{
			friend class ListView;
			SelectedItemIterator(ItemView* first) : ItemIterator(first) { 
			}
			NGBool check(ItemView* view) {
				return view && view->isSelected();
			}
		public:
			SelectedItemIterator(const SelectedItemIterator& it) : ItemIterator(it) { }
			SelectedItemIterator() { }
			SelectedItemIterator& operator=(const SelectedItemIterator& it) { ItemIterator::init(it); return *this; }
		};

		ItemView* selectedItem(NGInt *pidx = NULL) const ;
		NGInt       selectedItemIndex() const { NGInt idx; selectedItem(&idx); return idx; }

		SelectedItemIterator getSelectedItems() const { return SelectedItemIterator(selectedItem()); }


		void setHilightTop(NGBool b) { setFlag(b, HILIGHT_TOP); }
		NGBool hilightTop()          { return (m_flags&HILIGHT_TOP) == HILIGHT_TOP; }

		enum {
			NOTIFY_BEGIN = CustomEvent::CUS_MAX,
			NOTIFY_SELCHANGED = CustomEvent::CUS_SELCHANGED,
			NOTIFY_ITEMDELETED,
			NOTIFY_ITEMCREATED,
		};

		void relayout(NGBool isScrollGap = FALSE);//relayout the content

        void upFindItem(void);

        void downFindItem(void);

		virtual void makeHilightVisible();

    protected:
        //void initDrawable(DrawableSelector* selector);

        void init(void);

		void updateContent();
		//void makeHilightVisible();

        void hideAll();
#if 0   // GT_jyseo rollback
		//virtual void onGetFocus();
		virtual void onLoseFocus();
#endif        
        NGBool onKeyPressed(NGInt keyCode);

		enum {
			FREEZED = (1 << ScrollView::FLAG_SHIFT),
			HILIGHT_TOP = (2 << ScrollView::FLAG_SHIFT),
			SCROLLANIMATION = (4 << ScrollView::FLAG_SHIFT),
			OPERATION_SHIFT = ScrollView::FLAG_SHIFT + 3,
			OPERATION_MODE = (0x3 << OPERATION_SHIFT),
			FLAG_SHIFT = (3 + ScrollView::FLAG_SHIFT)
		};

		void setFreezed(NGBool b) { setFlag(b, FREEZED); }
		NGBool freezed()          { return ((m_flags & FREEZED) == FREEZED); }

		void drawCenterHilight(GraphicsContext* gc);
 //               NGBool isKeyFreeze() {return m_pressedKey != 0;}      // GT_jyseo rollback 1873

	public:
		void setScrollAnimation(NGBool b) { setFlag(b, SCROLLANIMATION); }
		NGBool scrollAnimation(){ return (m_flags&SCROLLANIMATION) == SCROLLANIMATION; }

		enum { //the operation mode
			OPM_NORMAL = 0, //the normal operation
			OPM_BROWSER,    //browser the items, don't show hilight
			OPM_EDIT,       //changed the dispatch in the child item
		};

		NGInt operationMode() { return (m_flags & OPERATION_MODE) >> OPERATION_SHIFT; }
		void  setOperationMode(NGInt mode) { m_flags = (m_flags & (~OPERATION_MODE)) | ((mode & 0x3) << OPERATION_SHIFT) ; }

//		AniList m_aniList;
    protected:
		ItemView* m_hilightItem;
    
    private:
		ItemView* m_curr_single_selected;

        NGUInt32 m_vGap:12;
        NGUInt32 m_hGap:12;
		NGUInt32 m_separatorWidth:8;
		NGUInt32 m_scrollGap;
#if 0    // GT_jyseo rollback 
        NGInt m_lstTimerId;
        NGInt m_pressedKey;
        NGInt m_timerCount;
#endif
		void animationScroll(NGInt offy);

		static void _set_y_offset(NGUInt handle, ListView* _this, NGInt id, NGInt *value);
		static void _anim_finish_cb(MGEFF_ANIMATION handle);

		DECLARE_CLASS_NAME(ListView)

		virtual NGBool nextFocus();
		virtual NGBool prevFocus();
};

NAMESPACE_END

#endif /* _NGUX_ListView_h */
