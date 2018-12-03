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

#ifndef __HFCL_MENU_H__
#define __HFCL_MENU_H__

#include "nguxwindow.h"
#include "viewcontext.h"
#include "simpleitemview.h"
#include "listview.h"

namespace hfcl {

class Menu;
class MenuBarView;
class Activity;

// =========================== MenuItem =====================================
class MenuItem : public SimpleItemView 
{
    public:
        MenuItem(const char * text, Image* img, int iid, 
				bool disable=false, DrawableSet* drset = NULL);
		MenuItem(int strId, Image* img, int iid, 
				bool disable=false, DrawableSet* drset = NULL);	
		virtual ~MenuItem();

		virtual void onGetFocus();
		virtual void onLoseFocus();

        virtual Menu* getSubMenu() { return NULL; }

        virtual void onMenuEnter(Menu * owner);
        virtual void onMenuRight(Menu * owner) {}

        enum { INVALID_TAG = -1};
        virtual int  getTag() { return INVALID_TAG; }
        virtual void check(Menu* owner, bool bcheck) { }
		virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

        inline void setOriginalText(const char* str) 	{ setText(str);  }
	 	inline void setOriginalText(int strid)	   	{ setText(strid);}
        inline void setOriginalText(const string& str)  { setText(str);  }
 
        char * originalText() { return getText(); }

		virtual void setIndex(int i);

	protected:	
		virtual void calcRect(const IntRect& rc, IntRect& txtRc, IntRect& imgRc);

        DECLARE_CLASS_NAME(MenuItem)
    
    private:
    	char  m_idxText[TEXT_BUFFER_LEN_OF_DEFAULT];
};

class CheckMenuItem : public MenuItem {
    public:
        CheckMenuItem(const char * text, Image* img, int iid,
                bool disable = false, bool checked = false, DrawableSet* drset = NULL);
		CheckMenuItem(int strId, Image* img, int iid,
                bool disable = false, bool checked = false, DrawableSet* drset = NULL);
        void onMenuEnter(Menu* owner);
        virtual void onMenuRight(Menu * owner) {}

        virtual void check(Menu* owner, bool bcheck)
        {
            if (bcheck == getCheck())
                return ;

            setCheck(bcheck);
            updateView();
        }

		virtual void setIndex(int i) { MenuItem::setIndex(0); }

        DECLARE_CLASS_NAME(CheckMenuItem)
};

class RadioMenuItem : public MenuItem {
    public:
        RadioMenuItem(const char * text, Image* img, int iid, int tag = 0,
                bool disable = false, bool checked = false, DrawableSet* drset = NULL);
		RadioMenuItem(int strId, Image* img, int iid, int tag = 0,
                bool disable = false, bool checked = false, DrawableSet* drset = NULL);
        void onMenuEnter(Menu* owner);
        void onMenuRight(Menu * owner) {}
        int getTag() { return m_tag;}

        virtual void check(Menu* owner, bool bstate);
		virtual void setIndex(int i) { MenuItem::setIndex(0); }

    protected:
        int m_tag;

        DECLARE_CLASS_NAME(RadioMenuItem)
};

class SubMenuItem : public MenuItem {
    public:
        SubMenuItem(const char * text, Image *img, int iid, Menu* submenu,
                bool disable = false, DrawableSet* drset = NULL);
		SubMenuItem(int strId, Image *img, int iid, Menu* submenu,
                bool disable = false, DrawableSet* drset = NULL);	
        void onMenuEnter(Menu* owner);
        void onMenuRight(Menu * owner);
		
		virtual void drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/);

        ~SubMenuItem();

        Menu* getSubMenu() { return m_subMenu; }

        DECLARE_CLASS_NAME(SubMenuItem)

    protected:
        Menu* m_subMenu;
};


// ============================= Menu =======================================
typedef struct _MenuQueryItem {
    int  menu_id;
    int  menu_tag; //only for radio
    char bdisable;
    char checked;
} MenuQueryItem;

class Menu : public Window, ViewContext {
    public:
        typedef enum {
            CUSTOM_MENU_KEY_SL,
            CUSTOM_MENU_KEY_SR,
            CUSTOM_MENU_QUERY_ITEM,
        } MenuKeyCustomType;

        enum {
            MAINMENU_ID = -1, // the main menu id
        };

        Menu(HTResId resid = 0);
		Menu(int list_theme_drset_id, int item_theme_drset_id, int menubar_theme_drset_id, HTResId resid = 0);
        virtual ~Menu();

        void create(HTResId resid);
        Intent* onDestroy(ContextStream* contextstream);

        bool setRect(const IntRect& irc)
        {
            setAppRect(irc);
            IntRect rc (0, 0, irc.width(), irc.height());
            Window::setRect(rc);
            updateSize();

            return true;
        }

        bool setMenuRect(int left, int top, int right, int bottom)
        {
            return setRect(IntRect(left, top, right, bottom));
        }

        bool onKey(int keyCode, KeyEvent* event);
		virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status);
        virtual void drawContent(GraphicsContext* context, IntRect &rc, int status);

		void onClick(POINT pt, Event::EventType type);
        void setMenuLeftButtonText(const char * text);
	 	void setMenuLeftButtonText(int strid);
        //const char * getMenuLeftButtonText(void) const;

        void setMenuRightButtonText(const char * text);
	 	void setMenuRightButtonText(int strid);
       //const char * getMenuRightButtonText(void) const;

        static Menu* getCurrentMenu(void);
        void showMenu(Activity *app = NULL);
        void hideMenu(void);
		void onMenuEnter(void);
		void onMenuRight(void);
        void closeMenu(void);
        void closeAllMenu(void);
        
		void closeMenu(Menu *menu);
        void freeze(bool bFreeze) {
            if (m_menuItemList){
                m_menuItemList->freeze(bFreeze);
            }
            if (!bFreeze){
                refreshItemIndex();
            }
        }

        bool addMenuItem(MenuItem* mi = NULL);

        bool addMenuItemByIndex(MenuItem* mi = NULL,int index = 0);
        
        bool addCommandMenuItem(const char * text, Image *img, int iid,
                bool bdisable=false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, 0, &bdisable);
            return addMenuItem(HFCL_NEW_EX(MenuItem, (text, img, iid, bdisable, drset)));
        }
                
        bool addCommandMenuItem(int strid, Image *img, int iid,
                bool bdisable=false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, 0, &bdisable);
            return addMenuItem(HFCL_NEW_EX(MenuItem, (strid, img, iid, bdisable, drset)));
        }
		
        bool addCheckMenuItem(const char * text, Image* img, int iid,
                bool bdisable=false, bool checked = false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, 0, &bdisable, &checked);
            return addMenuItem(HFCL_NEW_EX(CheckMenuItem, (text, img, iid, bdisable, checked, drset)));
        }

        bool addCheckMenuItem(int strid, Image* img, int iid,
                bool bdisable=false, bool checked = false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, 0, &bdisable, &checked);
            return addMenuItem(HFCL_NEW_EX(CheckMenuItem, (strid, img, iid, bdisable, checked, drset)));
        }
		
        bool addRadioMenuItem(const char * text, Image* img, int iid, int tag = 0,
                bool bdisable = false, bool checked = false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, tag, &bdisable,&checked);
            return addMenuItem(HFCL_NEW_EX(RadioMenuItem, (text, img, iid, tag, bdisable, checked, drset)));
        }

        bool addRadioMenuItem(int strid, Image* img, int iid, int tag = 0,
                bool bdisable = false, bool checked = false, DrawableSet* drset = NULL)
        {
            queryItemState(iid, tag, &bdisable,&checked);
            return addMenuItem(HFCL_NEW_EX(RadioMenuItem, (strid, img, iid, tag, bdisable, checked, drset)));
        }
		
        bool addSubMenuItem(const char * text, Image* img, int iid, Menu* submenu,
                bool bdisable = false, DrawableSet* drset = NULL)
        {
            if (submenu)
                submenu->setMenuParent(this);
            queryItemState(iid, 0, &bdisable);
            return addMenuItem(HFCL_NEW_EX(SubMenuItem, (text, img, iid, submenu, bdisable, drset)));
        }

        bool addSubMenuItem(int strid, Image* img, int iid, Menu* submenu,
                bool bdisable = false, DrawableSet* drset = NULL)
        {
            if (submenu)
                submenu->setMenuParent(this);
            queryItemState(iid, 0, &bdisable);
            return addMenuItem(HFCL_NEW_EX(SubMenuItem, (strid, img, iid, submenu, bdisable, drset)));
        }
		
        bool addSubMenuItemByIndex(const char * text, Image* img, int iid, Menu* submenu,
                bool bdisable = false, DrawableSet* drset = NULL,int index = 0)
        {
            if (submenu)
                submenu->setMenuParent(this);
            queryItemState(iid, 0, &bdisable);
            return addMenuItemByIndex(HFCL_NEW_EX(SubMenuItem, (text, img, iid, submenu, bdisable, drset)),index);
        }

	 	bool addSubMenuItemByIndex(int strid, Image* img, int iid, Menu* submenu,
                bool bdisable = false, DrawableSet* drset = NULL,int index = 0)
        {
            if (submenu)
                submenu->setMenuParent(this);
            queryItemState(iid, 0, &bdisable);
            return addMenuItemByIndex(HFCL_NEW_EX(SubMenuItem, (strid, img, iid, submenu, bdisable, drset)),index);
        }
				
        Menu* getSubMenu(int iid = 0);

        MenuItem* getMenuItem(int idx)
        {
            return (MenuItem*)(m_menuItemList ? m_menuItemList->itemFromIndex(idx) : NULL);
        }

        MenuItem* getMenuItemByID(int iid)
        {
            return (MenuItem*) ( m_menuItemList ? m_menuItemList->itemFromId(iid) : NULL);
        }

        MenuItem* getCurMenuItem(void)
        {
            return (MenuItem*) ( m_menuItemList ? m_menuItemList->hilightItem() : NULL);
        }

        int getCurMenuItemID(void)
        {
            MenuItem* mi = getCurMenuItem();
            return mi ? mi->id() : -1;
        }

        void hilightFirstEnable(void);

        bool setMenuHilightIndex(int index = -1)
        {
            int count = getItemCount();

            if (index < 0 || index > count - 1)
                return false;
			
			return m_menuItemList ? m_menuItemList->hilight(index) : false;
#if 0
            for (int i = 0; i < count; i++)
            {
                MenuItem *mi = getMenuItem(i);
                if (NULL != mi && mi->isVisible()){
                    //if (index-- == 0){
                        return m_menuItemList ? m_menuItemList->hilight(index) : false;
                    //}
                }
            }
            return false;
#endif
        }

        int getMenuHilightIndex(void)
        {
            return m_menuItemList ? m_menuItemList->hilightItemIndex() : -1;
        }

        inline void setMenuParent(Menu* p_parent = NULL) { 
			m_menuParent = p_parent; 
		}
        inline bool isExitMenuParent(void) const { 
			return m_menuParent != NULL; 
		}
        inline Menu* getMenuParent(void) const { 
			return m_menuParent; 
		}

        int getItemCount(void)
        {
            return m_menuItemList ?  m_menuItemList->itemCount() : 0;
        }

        bool removeMenuItem(int idx)
        {
            return m_menuItemList && m_menuItemList->removeItemByIndex(idx);
        }
/*
        void removeAll(void)
        {
            // if (m_menuItemList)
            //     m_menuItemList->clear();
            m_menuItemList = NULL;
        }
*/
        void uncheckRadioItemExcept(RadioMenuItem *item);

        void setMenuItemChecked(int index, bool bcheck)
        {
            MenuItem* mi = getMenuItem(index);
            if (mi)
                mi->check(this, bcheck);
        }

        bool isMenuItemChecked(int index)
        {
            MenuItem* mi = getMenuItem(index);
            return mi && mi->getCheck();
        }

        void setMenuItemCheckedByID(int iid, bool bcheck)
        {
            MenuItem* mi = getMenuItemByID(iid);
            if (mi) {
                mi->check(this, bcheck);
            }
        }

        bool isMenuItemCheckedByID(int iid)
        {
            MenuItem* mi = getMenuItemByID(iid);
            return mi && mi->getCheck();
        }

        void enableMenuItem(int iid, bool benable)
        {
            MenuItem* mi = getMenuItem(iid);
            if (mi && mi->isDisabled() != benable) {
                mi->disable(!benable);
                updateView();
            }
        }

        bool isMenuItemEnabled(int idx)
        {
            MenuItem* mi = getMenuItem(idx);
            return mi && !mi->isDisabled();
        }

        void enableMenuItemByID(int iid, bool benable)
        {
            MenuItem* mi = getMenuItemByID(iid);
            if (mi && mi->isDisabled() != benable) {
                mi->disable(!benable);
                updateView();
            }
        }

        bool isMenuItemEnabledByID(int idx)
        {
            MenuItem* mi = getMenuItemByID(idx);
            return mi && !mi->isDisabled();
        }

        bool notifyToParent(MenuKeyCustomType mkct, int menuId);

        void setMaxHeight(int max);

        void autoFit(); //auto change it's size

        void setMenuItemHeight(int height)
        {
            m_menuItemHeight = (Uint16) height;
        }

        enum {
            SEPARATOR_TOP,            
            ITEMLIST = 1,
            SEPARATOR_BOTTOM,            
            MENUBAR
        };

        void updateSize(void);

        bool queryItemState(int iid, int tag, bool *pdisable, bool *pcheck = NULL);
        
        void refreshItemIndex();

		IntRect getMenuRect() {return m_menuItemList->getRect(); }
		void setProcessIng(bool isProcessing){ m_isProcessing = isProcessing;}
		bool isProcessing() { return m_isProcessing; }
		Activity* ownApp(void){return m_ownApp;}
	 protected:	
	 	void updateParentMenu(Menu * menu);
		
        static Menu*   m_currentMenu;
		ListView*      m_menuItemList;
        
		Uint16       m_menuItemHeight;
        Uint16       m_maxHeight;

        Menu*          m_menuParent;
        MenuBarView*   m_menuBar;

        PanelView*		m_separator_top;        
        PanelView*		m_separator_bottom;        
        Activity*    m_ownApp;

		int          m_list_themedrset_id;
		int		   m_item_themedrset_id;
		int		   m_menubar_themedrset_id;
		bool		   m_isClose;
		bool 		   m_isProcessing;

        DECLARE_VIEWCONTEXT
        DECLARE_UI_TEMPL
};

#define MENU_COMMAND_LISTENER(owner, clss, method) \
    METHOD_EVENT_HANDLE_EX(owner, clss, method, Event::CUSTOM_NOTIFY)

} // namespace hfcl

#endif /* __HFCL_MENU_H__ */

