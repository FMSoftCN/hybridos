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


#ifndef HFCL_VIEW_LISTVIEW_H_
#define HFCL_VIEW_LISTVIEW_H_

#include "view/scrollview.h"
#include "view/itemview.h"

namespace hfcl {

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
    int       itemCount(void)              { return m_content ? m_content->viewCount():0; }
    ItemView *hilightItem(void)            {return m_hilightItem; };
    int       hilightItemIndex(void)       {return indexFromItem(m_hilightItem); };
    ItemView *itemFromIndex(int index)     { return m_content ? (ItemView*)m_content->getChildByIndex(index):NULL; }
    int       indexFromItem(ItemView *item){ return m_content ? m_content->getChildIndex(item): -1; }
    View* firstEnableItem(void) { return m_content ? m_content->firstEnableChild():NULL;  }

    //int  headGap(void)      {return m_headGap;};
    //void setHeadGap(int gap){m_headGap = gap;};

    int  vGap(void)      {return m_vGap;};
    int  hGap(void)      {return m_hGap;};
    void setVGap(int gap){m_vGap = gap;};
    void setHGap(int gap){m_hGap = gap;};
    void setScrollGap(int gapPixel){m_scrollGap = gapPixel;}
    int scrollGap(void){return m_scrollGap;}
    //int  tailGap(void)      {return m_tailGap;};
    //void setTailGap(int gap){m_tailGap = gap;};

    /* \brief judge is a item is added to listview 
     * * \param[in] item the item pointer.
     * * \return bool true if is, or false
     * */
    bool isItem(ItemView *item) { return isChild(item); }

    /* \brief add a item.By default, add at the list tail.
     * * \param[in] item the item pointer to add.
     * * \return bool true if success , or false
     * */
    bool addItemByIndex(ItemView *item, int index);
    bool addItem(ItemView *item) { return addItemByIndex(item, -1); }
    bool addItem(View *item)     { return addItem ((ItemView*)item);}
    bool addItemWithAppend(ItemView *item);
    /* \brief remove a item at by index. 
     * * \param[in] index the item index to remove.
     * * \return bool true if success , or false
     * */
    bool removeItemByIndex(int index, bool bRelease = true) { return removeItem(itemFromIndex(index)); }
    bool removeItem(ItemView* item, bool bRelease = true);
    void clear(void);

    /* \brief hilight a item.
     * * \param[in] int the index of item to hilight.
     * * \return bool true if success , or false
     * */
    virtual bool hilight(ItemView *view);
    virtual bool hilight(int index) { return hilight(itemFromIndex(index)); }
//        bool selectedItem(int index);

    /* \brief select a item.
     * * \param[in] int the index of item to select.
     * * \return bool true if success , or false
     * */
    bool select(int index, bool toSelect);
    bool singleSelect(int index);
    ItemView* singleSelectItem(void) { return m_curr_single_selected; }

    void deSelectAll() { selectAll(false); }
    void selectAll(bool bselect=true);


    bool         dispatchEvent(Event *event);
    virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

    /* \brief this function set the separator width.
     * * \param[in] w the width of separator.
     * */
    void      setSeparatorWidth(int w);
    //Drawable *separator(void) { return (Drawable*)m_drawable.getElement(SYS_SE_DRSEPERATOR); }
    //void      setSeparator(Drawable *separator) { m_drawable.setElement(SYS_SE_DRSEPERATOR, (DWORD)separator); }

    void freeze(bool freeze);

    void drawSeparator(GraphicsContext* context, int status/* = Style::NORMAL*/);

    ItemView* firstItem()const { return m_content ? (ItemView*) m_content->firstChild() : NULL; }
    ItemView* lastItem()const  { return m_content ? (ItemView*) m_content->lastChild() : NULL; }

    ItemView* itemAt(int x, int y, int *pidx)const;
    ItemView* itemAt(int x, int y) const { return itemAt(x, y, NULL); }
    ItemView* firstVisibleItem()const  { return itemAt(m_offx + m_hGap, m_offy + m_vGap + m_separatorWidth); }
    ItemView* lastVisibleItem()const   { return itemAt(m_offx + m_hGap, m_offy + getRect().height() - m_vGap - m_separatorWidth); }

    ItemView* itemFromId(int iid) { return (ItemView*) (m_content ? m_content->getView(iid) : NULL); }

    int itemIndexAt(int x, int y) const { 
        int idx;
        itemAt(x, y, &idx);
        return idx;
    }
    int firstVisibleItemIndex() const { return itemIndexAt(m_offx + m_hGap, m_offy + m_vGap + m_separatorWidth); }
    int lastVisibleItemIndex() const  { return itemIndexAt(m_offx + m_hGap, m_offy + getRect().height() - m_vGap - m_separatorWidth); }
    
public:
    class ItemIterator {
    protected:
        ItemIterator(ItemView* first) : m_current (first) { }	
        virtual bool check(ItemView *view) = 0;
        void init(const ItemIterator& it) { m_current = it.m_current; }

        ItemView* m_current;
    public:
        ItemIterator(const ItemIterator& it) { init(it); }
        ItemIterator(): m_current(NULL) { }

        bool isEnd() { return m_current == NULL; }
        void prev() { 
            while(m_current && !check((m_current = (ItemView*)m_current->previousSibling()))) ;
        }
        void next() { 
            while(m_current && !check((m_current = (ItemView*)m_current->nextSibling()))) ;
        }

        ItemView* operator->() { return m_current; }
        ItemView* operator*() { return m_current; }
    };

    class VisibleItemIterator : public ItemIterator {
        friend class ListView;
        int m_bottom;
        VisibleItemIterator(ItemView* first, int bottom) : ItemIterator(first), m_bottom(bottom) { }

        bool check(ItemView* view) { 
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

    class SelectedItemIterator : public ItemIterator {
        friend class ListView;
        SelectedItemIterator(ItemView* first) : ItemIterator(first) { 
        }
        bool check(ItemView* view) {
            return view && view->isSelected();
        }
    public:
        SelectedItemIterator(const SelectedItemIterator& it) : ItemIterator(it) { }
        SelectedItemIterator() { }
        SelectedItemIterator& operator=(const SelectedItemIterator& it) { ItemIterator::init(it); return *this; }
    };

    ItemView* selectedItem(int *pidx = NULL) const ;
    int       selectedItemIndex() const { int idx; selectedItem(&idx); return idx; }

    SelectedItemIterator getSelectedItems() const { return SelectedItemIterator(selectedItem()); }

    void setHilightTop(bool b) { setFlag(b, HILIGHT_TOP); }
    bool hilightTop()          { return (m_flags&HILIGHT_TOP) == HILIGHT_TOP; }

    enum {
        NOTIFY_BEGIN = CustomEvent::CUS_MAX,
        NOTIFY_SELCHANGED = CustomEvent::CUS_SELCHANGED,
        NOTIFY_ITEMDELETED,
        NOTIFY_ITEMCREATED,
    };

    void relayout(bool isScrollGap = FALSE);//relayout the content

    void upFindItem(void);

    void downFindItem(void);

    virtual void makeHilightVisible();

public:
    void setScrollAnimation(bool b) { setFlag(b, SCROLLANIMATION); }
    bool scrollAnimation(){ return (m_flags&SCROLLANIMATION) == SCROLLANIMATION; }

    enum { //the operation mode
        OPM_NORMAL = 0, //the normal operation
        OPM_BROWSER,    //browser the items, don't show hilight
        OPM_EDIT,       //changed the dispatch in the child item
    };

    int operationMode() { return (m_flags & OPERATION_MODE) >> OPERATION_SHIFT; }
    void  setOperationMode(int mode) { m_flags = (m_flags & (~OPERATION_MODE)) | ((mode & 0x3) << OPERATION_SHIFT) ; }

protected:
    ItemView* m_hilightItem;

    void init(void);

    void updateContent();

    void hideAll();
    bool onKeyPressed(int keyCode);

    enum {
        FREEZED = (1 << ScrollView::FLAG_SHIFT),
        HILIGHT_TOP = (2 << ScrollView::FLAG_SHIFT),
        SCROLLANIMATION = (4 << ScrollView::FLAG_SHIFT),
        OPERATION_SHIFT = ScrollView::FLAG_SHIFT + 3,
        OPERATION_MODE = (0x3 << OPERATION_SHIFT),
        FLAG_SHIFT = (3 + ScrollView::FLAG_SHIFT)
    };

    void setFreezed(bool b) { setFlag(b, FREEZED); }
    bool freezed()          { return ((m_flags & FREEZED) == FREEZED); }

    void drawCenterHilight(GraphicsContext* gc);

private:
    ItemView* m_curr_single_selected;

    Uint32 m_vGap:12;
    Uint32 m_hGap:12;
    Uint32 m_separatorWidth:8;
    Uint32 m_scrollGap;
    void animationScroll(int offy);

    virtual bool nextFocus();
    virtual bool prevFocus();

    static void _set_y_offset(unsigned int handle, ListView* _this, int id, int *value);
    static void _anim_finish_cb(MGEFF_ANIMATION handle);

    DECLARE_CLASS_NAME(ListView)
};

} // namespace hfcl

#endif /* HFCL_VIEW_LISTVIEW_H_ */
