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

 /*
 ** Copyright (C) 2002 ~ 2011 Beijing FMSoft Technology Co., Ltd.
 ** All rights reserved by FMSoft.
 **
 ** FileName        : TableBarView.h
 ** FileDescription : TODO
 **
 ** $ Current Version    : V.0001 $
 ** $ Current Maintainer : xulei-laptop $
 ** $ Completion Date    : 2011-09-07 09:33:05 $
 **
 ** History Version : V.0001
 ** Author          : xulei-laptop
 ** Create Date     : 星期三 九月 14, 2011
 */ 

#ifndef _HFCL_TABLEBARVIEW_H
#define _HFCL_TABLEBARVIEW_H

#include "compositeview.h"
#include "pageview.h"
#include "gridview.h"
#include "itemview.h"
#include "graphics/image.h"


namespace hfcl {

class TableBarView : public CompositeView{

    public:

        enum{
            SEL_TABLE_CHANGE =  1001 
        };

        enum{
            TABLE_POS_TOP ,
            TABLE_POS_BOTTOM
        };

		enum {
			DRSET_TAB = CompositeView::DRSET_MAX,
			DRSET_PAGE,
			DRSET_TABITEM,
			DRSET_MAX
		};

        TableBarView(View* p_parent, DrawableSetGroup* g = NULL);
        TableBarView(View* p_parent, DrawableSet* drset, DrawableSetGroup* g = NULL);
        TableBarView(int i_id, int x, int y, int w, int h);
        ~TableBarView();
        
        void setTableHeight(int   height);
        int tableHeight(void);
        
        void setTableGapH(int value);
        void setTableGapW(int value);
        
        //bool setSelectTableBkg(Image* img);
        //bool setSelectTableBkg(const char * file);
       
        //bool setTableBkg(Image* img);
        //bool setTableBkg(const char * file);
        
        //bool setTablePageBkg(Image* img);
        //bool setTablePageBkg(const char * file);
        
        bool addTablePage(View* table, View* tableInfo ); 
        //bool addTablePage(View* table,View* tableSelect, View* tableInfo ); 
        bool addTablePage(const char * tableFile,const char* tableSelectFile, View* tableInfo ) ;
        bool addTablePage(Image* tableImg, Image* selectTableImg, View* tableInfo ); 
        bool addTablePage(ItemView* tabitem , View* tableInfo ); 
        
        bool delTablePage(View* table);
        bool delTablePage(int index );
        
        View* tableInfoView(int index);
        View* tableInfoView(View *view);
        
        bool  setSelectTableByIndex(int index);
        
        int  selectTableIndex(void);
        
        bool setTablePos(int pos);  // pos = 0; table pos is TOP, pos = 1; table pos is BOTTOM, other is failed;
        
        bool  dispatchEvent (Event* event);

		void setPageDrawableSet(DrawableSet* drset) {
			if(m_pages)
				m_pages->setDrawableSet(drset);
		}

		void setTabDrawableSet(DrawableSet* drset) {
			if(m_tablesGrid)
				m_tablesGrid->setDrawableSet(drset);
		}
    protected:
        //void initDrawable(DrawableSelector* selector);

    private:
       // void relayoutTableBkgRect(void);
        void relayout(void);
        bool init(void);
        bool relayoutRect(void);
        int    m_tableHeight; 
        int    m_tablePos; 
        GridView* m_tablesGrid;
        PageView * m_pages;

		virtual void updateDrawableSets(int i_id) {
			CompositeView::updateDrawableSets(i_id);
			SETDRAWABLESET(m_tablesGrid, i_id, DRSET_TAB);
			SETDRAWABLESET(m_pages, i_id, DRSET_PAGE);
			if(m_tablesGrid && (i_id == -1 || i_id == DRSET_TABITEM)) {
				DrawableSet* drset = getDrawableSet(DRSET_TABITEM);
				if(drset)
				{
					for(View* view = m_tablesGrid->firstChild();
							view != NULL; view = view->nextSibling())
					{
						view->setDrawableSet(drset);
					}
				}
			}
		}
};

class TableItemView : public ItemView 
{
private:
	Image* m_normalImg;
	Image* m_selImg;
public:
	TableItemView(View *p_parent)
		:ItemView(p_parent)
		,m_normalImg(NULL),m_selImg(NULL) { }
	TableItemView(View *p_parent, DrawableSet* drset) 
		:ItemView(p_parent, drset)
		,m_normalImg(NULL),m_selImg(NULL) { }

	virtual ~TableItemView();

	void setNormalImage(Image* img);
	void setSelImage(Image* img);
	Image* selImage() { return m_selImg; }
	Image* normalImage() { return m_normalImg; }

	void drawContent(GraphicsContext* context, IntRect& rc, int status)
	{
		Image *img = (status == DRAWSTATE_NORMAL ? m_normalImg : m_selImg);
		if(img) {
			ImageFormat format;
			format.drawMode = DRAWMODE_NORMAL;
			format.align  = ALIGN_CENTER;
			format.valign = VALIGN_MIDDLE; 
			img->paint(context, rc, format);
		}
	}
};

} // namespace hfcl {

#endif 
