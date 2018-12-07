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


#include "graphics/graphicscontext.h"
#include "tablebarview.h"

namespace hfcl {

/*
TableBarView:: TableBarView()
{
   m_tableHeight = 0;
}*/

TableBarView:: TableBarView(View * p_parent, DrawableSetGroup* g)
:CompositeView(p_parent, g ? g : DEFAULT_VIEW_DRAWABLESETGROUP(TableBarView))
,m_tablesGrid(NULL)
,m_pages(NULL)
{
    init();
    m_tableHeight = 0;
    m_tablePos = TABLE_POS_BOTTOM;
}

TableBarView:: TableBarView(View * p_parent, DrawableSet * drset, DrawableSetGroup* g)
:CompositeView(p_parent, drset, g ? g : DEFAULT_VIEW_DRAWABLESETGROUP(TableBarView))
,m_tablesGrid(NULL)
,m_pages(NULL)
{
    m_tableHeight = 0;
    m_tablePos = TABLE_POS_BOTTOM;
    init();
}

TableBarView:: TableBarView(int i_id, int x, int y, int w, int h)
:CompositeView(i_id, x, y, w, h, DEFAULT_VIEW_DRAWABLESETGROUP(TableBarView))
,m_tablesGrid(NULL)
,m_pages(NULL)
{
    m_tableHeight = h/5;
    m_tablePos = TABLE_POS_BOTTOM;
    init();
}

TableBarView:: ~TableBarView()
{
    m_tablesGrid    = NULL;
    m_pages         = NULL;
}

void TableBarView ::relayout(void)
{

    if(m_tableHeight <=0 || !m_pages || !m_tablesGrid)
        return ;

    relayoutRect();
}

bool TableBarView ::setTablePos(int pos)
{
    if(pos < TableBarView::TABLE_POS_TOP || pos >TABLE_POS_BOTTOM)
        return false;

    if(m_tablePos != pos)
    {
        m_tablePos = pos;
        relayout();
    }
    return true;
}

void TableBarView ::setTableHeight(int height)
{
    if(height <= 0)
        return ;

    m_tableHeight = height;
    relayout();
}

int TableBarView :: tableHeight(void)
{
    return m_tableHeight;
}
void TableBarView::setTableGapW(int value)
{
    m_tablesGrid->setGapWidth(value);
}

void TableBarView::setTableGapH(int value)
{
    m_tablesGrid->setGapHeight(value);
}

/////////////////////////////////////////////
TableItemView::~TableItemView()
{
    if(m_normalImg) {
        HFCL_DELETE(m_normalImg);
    }
    if(m_selImg) {
        HFCL_DELETE(m_selImg);
    }
}

void TableItemView::setNormalImage(Image* img) {
    if(m_normalImg && img != m_normalImg) {
        HFCL_DELETE(m_normalImg);
        m_normalImg = NULL;
    }
    m_normalImg = img;
}

void TableItemView::setSelImage(Image* img)
{
    if(m_selImg && img != m_selImg) {
        HFCL_DELETE(m_selImg);
        m_selImg = NULL;
    }
    m_selImg = img;
}



#if 0
bool TableBarView::setTablePageBkg(Image * img)
{
    bool ret = false;

    if(m_pages)
    {
        ret = m_pages->setBkImageEx(img);
        m_pages->setImageDrawMode(IMAGE_DRAWMODE_SCALED);
    }
    return ret;
}

bool TableBarView::setTablePageBkg(const char *file)
{
    bool ret = false;

    if(m_pages)
    {
        ret = m_pages->setBkImage(file);
        m_pages->setImageDrawMode(IMAGE_DRAWMODE_SCALED);
    }
    return ret;
}


bool TableBarView::setTableBkg(const char *file)
{
    bool ret = false;

    if(m_tablesGrid)
    {
        ret = m_tablesGrid->setBkImage(file);
        m_tablesGrid->setImageDrawMode(IMAGE_DRAWMODE_SCALED);
    }
    return ret;
}

bool TableBarView::setSelectTableBkg(const char *file)
{
    return setSelectTableBkg(Image::loadImage(file));

}

bool TableBarView::setTableBkg(Image * img)
{
    bool ret = false;

    if(m_tablesGrid)
    {
       ret = m_tablesGrid->setBkImageEx(img);
        m_tablesGrid->setImageDrawMode(IMAGE_DRAWMODE_SCALED);
    }
    return ret;
}

bool TableBarView::setSelectTableBkg(Image * img)
{
    bool ret = false;
    int count =0;
    int i =0;

    if(NULL == img)
        return false;
    count = m_pages->getPageCount();
    for(i = 0; i <count; i++)
    {
        View * view = m_tablesGrid->getChildByIndex(i);
        ret = ((TableItemView*)view)->setBkImageEx(img);
        if(ret == false)
            return false;
        ((TableItemView*)view)->setImageDrawMode(IMAGE_DRAWMODE_SCALED);
    }
    return ret;
}

void TableBarView::relayoutTableBkgRect(void)
{
    IntRect focusRc;
    IntRect tableRc;
    View * gridFocus = NULL;
    int offset_x , offset_y;

    if( !m_tablesGrid)
        return ;
        return ;

    gridFocus = m_tablesGrid->focusView();
    if(!gridFocus)
        return ;

    focusRc = gridFocus->getRect();
    tableRc = m_tablesGrid->getRect();
    offset_x = m_tablesGrid->gapWidth();
    offset_y = m_tablesGrid->gapHeight();
    offset_y = offset_y?offset_y:5;

    IntRect rc = IntRect(tableRc.left() + focusRc.left()-offset_x, tableRc.top(), tableRc.left() + focusRc.right()+offset_x, tableRc.bottom());


   // _DBG_PRINTF("\n BkgRc left is:%d, top is:%d, right is:%d, bottom is:%d \n", rc.left(), rc.top(), rc.right(),  rc.bottom());
   // _DBG_PRINTF("\nfocusRc left is:%d, top is:%d, right is:%d, bottom is:%d \n", focusRc.left(), focusRc.top(), focusRc.right(), focusRc.bottom());
}
#endif

bool TableBarView:: relayoutRect(void)
{
    IntRect tableBarRc;
    IntRect gridRc;
    IntRect pageRc;
    int gap = 0;

   tableBarRc = getRect();
   if(tableBarRc.width()<=0  ||tableBarRc.height()<= 0)
       return false;

   m_tableHeight = m_tableHeight>0?m_tableHeight:tableBarRc.height()/5;
   switch(m_tablePos)
   {
       case TABLE_POS_TOP:
           gridRc = IntRect(0+gap , 0+gap, tableBarRc.width()-gap,m_tableHeight-gap);
           pageRc = IntRect(0+gap, m_tableHeight+gap, tableBarRc.width()-gap, tableBarRc.height()-gap);
           break;
       case TABLE_POS_BOTTOM:
           gridRc = IntRect(0+gap, tableBarRc.height()-m_tableHeight+gap, tableBarRc.width()-gap,tableBarRc.height()-gap);
           pageRc = IntRect(0+gap, 0+gap, tableBarRc.width()-gap, tableBarRc.height() - m_tableHeight-gap);
           break;
   }

   m_tablesGrid->setRect(gridRc);
   m_tablesGrid->setGapWidth(gap);
   m_tablesGrid->setGapHeight(gap);
   m_pages->setRect(pageRc);

    return true;

}

#if 0
bool TableBarView::addTablePage(const char * tableFile, const char* selectTableFile, View * tableInfo)
{
    ImageView* tableView = NULL;
    ImageView* selectView = NULL;

    if(!tableFile || !tableInfo)
        return false;

    tableView = HFCL_NEW_EX(ImageView, ((char *)tableFile, 0, 0, 0, 0, 0, DRAWMODE_NORMAL, ALIGN_CENTER, VALIGN_MIDDLE));
    if(selectTableFile)
        selectView = HFCL_NEW_EX(ImageView, ((char *)selectTableFile, 0, 0, 0, 0, 0, DRAWMODE_NORMAL, ALIGN_CENTER, VALIGN_MIDDLE));
   return addTablePage(tableView, selectView, tableInfo);
}
#endif

bool TableBarView::addTablePage(Image* tableImg, Image* selectTableImg, View * tableInfo)
{
    if(!tableImg && !tableInfo)
        return false;

    TableItemView * item = HFCL_NEW_EX(TableItemView, (NULL, GROUP_GETDRAWABLESET(DRSET_TABITEM, ItemView)));
    item->setSelImage(selectTableImg);
    item->setNormalImage(tableImg);

   return addTablePage(item, tableInfo);
}

bool TableBarView::addTablePage(ItemView* tabitem, View * tableInfo)
{
    if(!tableInfo || !tabitem)
        return false;

    if( false == init())
        return false;

    m_pages->appendPage(tableInfo);
    m_pages->setCurPageByIndex(0);
    int count = m_pages->getPageCount();

    m_tablesGrid->setCols(count);
    m_tablesGrid->addView(tabitem);
    m_tablesGrid->setFocusItem(0);
    return true;
}

bool TableBarView:: init(void)
{
   if(m_tablesGrid == NULL && m_pages == NULL)
   {
       m_tablesGrid = HFCL_NEW_EX(GridView, (this, GROUP_GETDRAWABLESET(DRSET_TAB, GridView)));
       m_pages = HFCL_NEW_EX(PageView, (this, GROUP_GETDRAWABLESET(DRSET_PAGE, PageView)));
       //addChild(m_tablesGrid);
       //addChild(m_pages);
       return relayoutRect();
   }
   return true;
}

bool TableBarView::addTablePage(View * table, View * tableInfo)
{

   if(!table || !tableInfo)
        return false;
    if( false == init())
        return false;

    m_pages->appendPage(tableInfo);
    int count = m_pages->getPageCount();
    m_tablesGrid->setCols(count);
    m_tablesGrid->addView(table);
    m_tablesGrid->setFocusItem(0);

    return true;
}

bool TableBarView:: delTablePage(View * table)
{
    return true;
}

bool TableBarView:: delTablePage(int index)
{
    return true;
}

View * TableBarView ::tableInfoView(int index)
{
    if(m_pages)
        return m_pages->pageFromIndex(index);
    return NULL;
}

View * TableBarView ::tableInfoView(View * tableView)
{
    return NULL;
}

bool TableBarView::setSelectTableByIndex(int index)
{
    if(m_pages&& m_tablesGrid && m_pages->getPageCount()> index)
    {
        m_pages->setCurPageByIndex(index);
        m_tablesGrid->setFocusItem(index);
        return true;
    }

    return false;
}

int TableBarView::selectTableIndex(void)
{
    if(m_tablesGrid)
        return m_tablesGrid->focusItemIdx();
    return -1;
}

bool TableBarView::dispatchEvent(Event * event)
{
    ItemView *item = NULL;
    //View::dispatchEvent(event);
    switch(event->eventType())
    {
        case Event::MOTION_UP:
        case Event::MOTION_DOWN:
        case Event::MOTION_CLICK:
            // TODO, for mouse support.
            break;
        case Event::KEY_DOWN:
        case Event::KEY_LONGPRESSED:
        case Event::KEY_ALWAYSPRESS:
            {
                switch(((KeyEvent *)event)->keyCode())
                {
                    case KeyEvent::KEYCODE_CURSOR_UP :
                    case KeyEvent::KEYCODE_CURSOR_DOWN :
                        if(!m_pages->getCurPage()->isFocus())
                            m_pages->setFocus(m_pages->getCurPage());
                        m_pages->dispatchEvent(event);
 //  add by xulei ; send event to GridView for focus, so that draw
 //  Bkg
                        if(!m_tablesGrid->isFocus())
                            m_tablesGrid->setFocus(m_tablesGrid);

                        item = (ItemView*)m_tablesGrid->focusView();
                        item->setSelected(false); // for fix a bug. why.
                        m_tablesGrid->dispatchEvent(event);
                        item = (ItemView*)m_tablesGrid->focusView();
                        item->setSelected(true);
                        break;
                    case KeyEvent::KEYCODE_CURSOR_LEFT :
                    case KeyEvent::KEYCODE_CURSOR_RIGHT :
                        {
                            int code = ((KeyEvent *)event)->keyCode();
                            if(!m_tablesGrid->isFocus())
                                m_tablesGrid->setFocus(m_tablesGrid);

                            item = (ItemView*)m_tablesGrid->focusView();
                            item->setSelected(false); // for fix a bug. why.
                            m_tablesGrid->onKeyPressed(code);
                            item = (ItemView*)m_tablesGrid->focusView();
                            item->setSelected(true);
                            if(((KeyEvent*)event)->keyCode() == KeyEvent::KEYCODE_CURSOR_LEFT)
                                m_pages->prevPage();
                            else
                                m_pages->nextPage();

                            CustomEvent e_event(Event::CUSTOM_NOTIFY, (int)(CustomEvent::CUS_PAGECHANGED), (int)this);
                            raiseEvent(&e_event);

                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
    return DISPATCH_CONTINUE_MSG;
}

#if 0
void TableBarView::initDrawable(DrawableSelector * selector)
{

}
#endif

#if 0
    TableItemView::TableItemView(View *p_parent)
    :PanelView(p_parent)
    ,m_normalView(NULL)
     ,m_selectView(NULL)
{
    m_isSelect =false;
}

    TableItemView::TableItemView(View *p_parent, DrawableSelector * selector)
    :PanelView(p_parent, selector)
    ,m_normalView(NULL)
     ,m_selectView(NULL)
{
    m_isSelect = false;
}

TableItemView:: ~TableItemView()
{
    m_normalView = NULL;
    m_selectView = NULL;
    m_isSelect = false;
}

bool  TableItemView::addTableItem(View * normal, View * selector)
{
    if(normal)
    {
        m_normalView = normal;
        addChild(m_normalView);
    }

    if(selector)
    {
        m_selectView = selector;
        addChild(m_selectView);
    }
    return true;
}

bool TableItemView::setRect(const IntRect & rc)
{
    if(rc.isEmpty())
        return false;

    //_DBG_PRINTF("\n TableItemView :: setRect!!! w is:%d h is:%d  \n", rc.width(), rc.height());
    PanelView::setRect(rc);
    if(m_normalView)
        m_normalView->setRect(0, 0, rc.width(), rc.height());
    if(m_selectView)
        m_selectView->setRect(0, 0, rc.width(), rc.height());
    return true;
}

#if 1
void TableItemView:: paint(GraphicsContext * context, int status)
{

    IntRect rc = getRect();
    if(itemSelect()||isFocus())
    {
#if 1
        if(NULL != getBkImage())
        {
            IntRect rc1 = m_normalView->getRect();
            PanelView::drawBackground(context, rc1, status);
        }
        else
        {
            context->fillRect(m_selectView->getRect(), 0x80, 0x80, 0x80, 0xff);
        }
#if 0
        _DBG_PRINTF("\nOnFocus ItemView l:%d, t:%d, r:%d, b:%d  \n",
                m_selectView->getRect().left(),
                m_selectView->getRect().top(),
                m_selectView->getRect().right(),
                m_selectView->getRect().bottom());
#endif
#endif
        if(m_selectView)
        {
            m_selectView->onPaint(context, status);
            return;
        }
    }
#if 0
    _DBG_PRINTF("\nItemView l:%d, t:%d, r:%d, b:%d  \n",
            m_normalView->getRect().left(),
            m_normalView->getRect().top(),
            m_normalView->getRect().right(),
            m_normalView->getRect().bottom());
#endif
    m_normalView->onPaint(context, status);
}
#endif
#endif

} // namespace hfcl
