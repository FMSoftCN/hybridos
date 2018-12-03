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

#undef _DEBUG

#include "application.h"
#include "appmanager.h"
#include "nguxmenu.h"
#include "respkgmanager.h"
#include "menubarview.h"
#include "color.h"

#define MAX_MENUITEM_NUM    5
#define OUT_WIDTH           10

#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__
extern void enterLockFrame(bool bManual /* = false */, bool bAllScreens);
#endif

namespace hfcl {

DEFINE_CLASS_NAME(MenuItem)
DEFINE_CLASS_NAME(CheckMenuItem)
DEFINE_CLASS_NAME(RadioMenuItem)
DEFINE_CLASS_NAME(SubMenuItem)

// =========================== MenuItem =====================================
MenuItem::MenuItem(const char * text, Image* img, int i_id, bool dsbl/*=false*/, DrawableSet* drset/* = NULL*/)
   :SimpleItemView(NULL, drset ? drset : DEFAULT_VIEW_DRAWABLESET(MenuItem))
{
    setGap(0);
    setOriginalText(text);
    enableImage(img != NULL);
    setImage(img);
    setRollProp(true);

    setText(text ? text : "");
    setId(i_id);
    disable(dsbl);
}

MenuItem::MenuItem(int strId, Image* img, int i_id, bool dsbl/*=false*/, DrawableSet* drset/* = NULL*/)
   :SimpleItemView(NULL, drset ? drset : DEFAULT_VIEW_DRAWABLESET(MenuItem))
{
    setGap(0);
    setOriginalText(strId);
    enableImage(img != NULL);
    setImage(img);
    setRollProp(true);

    setText(strId);
    setId(i_id);
    disable(dsbl);
}

MenuItem::~MenuItem()
{   
   stopRoll();
}

void MenuItem::onGetFocus()
{
   SimpleItemView::onGetFocus();
   startRoll();
}

void MenuItem::onLoseFocus()
{
   SimpleItemView::onLoseFocus();
   stopRoll();
}

void MenuItem::setIndex(int i)
{
    if (i > 0){
        snprintf(m_idxText, TEXT_BUFFER_LEN_OF_DEFAULT, "%d", i);
    } else {
        m_idxText[0] = '\0';
    }
}

void MenuItem::calcRect(const IntRect& rc, IntRect& txtRc, IntRect& imgRc)
{
    IntRect simpleItemRc(rc);
#if 0
    if (m_idxText[0] != '\0') {
        if(IsAllItemViewNeedConvert()){
            simpleItemRc.setRect(rc.left(), rc.top(), rc.right() - _ngux_sl_item_h2, rc.bottom());
        } else {
            simpleItemRc.setRect(rc.left() + _ngux_sl_item_h2, rc.top(), rc.right(), rc.bottom());
        }
    }
#endif
    simpleItemRc.setRect(rc.left() + 4, rc.top(), rc.right(), rc.bottom());
    SimpleItemView::calcRect(simpleItemRc, txtRc, imgRc);
}

void MenuItem::drawContent(GraphicsContext* context, IntRect &rc, int status /*= Style::NORMAL*/)
{
    IntRect textRc(rc);
    
    if(isDisabled())
        status = DRAWSTATE_DISABLED;

#if 0
    if (m_idxText[0] != '\0') 
    {
        IntRect idxItemRc(rc);
        _DBG_PRINTF("MenuItem::drawContent>>>%d,%d,%d,%d\n",rc.left(),rc.top(),rc.right(),rc.bottom());
        if(IsAllItemViewNeedConvert())
        {
            idxItemRc.setRect(rc.right() - _ngux_sl_item_h2, rc.top(), rc.right(), rc.bottom());
        } 
        else 
        {
            idxItemRc.setRect(rc.left(), rc.top(), rc.left() + _ngux_sl_item_h2, rc.bottom());
        }
        
        m_drset->draw(context, DR_CONTENT2, status, idxItemRc,(DWORD)m_idxText, DRDT_TEXT);
    }
#endif

    SimpleItemView::drawContent(context, textRc, status);
}

void MenuItem::onMenuEnter(Menu* owner)
{
    Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());
    if(ap != NULL) {
        ap->bindMenu(NULL); 
    }

    if (NULL != owner) {
        owner->hideMenu();
    }
}

CheckMenuItem::CheckMenuItem(const char * text, Image* img, int i_id,
        bool b_disable/*=false*/, bool checked/*=false*/, DrawableSet* drset /*=NULL*/)
    : MenuItem(text, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(CheckMenuItem))
{
    setGap(5);
    setCheck(checked);
}

CheckMenuItem::CheckMenuItem(int strId, Image* img, int i_id,
        bool b_disable/*=false*/, bool checked/*=false*/, DrawableSet* drset /*=NULL*/)
    : MenuItem(strId, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(CheckMenuItem))
{
    setGap(5);
    setCheck(checked);
}

void CheckMenuItem::onMenuEnter(Menu* owner)
{
    setCheck(!getCheck());
    updateView();
    MenuItem::onMenuEnter(owner);
}

RadioMenuItem::RadioMenuItem(const char * text, Image* img, int i_id,
        int tag/*=0*/, bool b_disable/*=false*/, bool checked/* = false*/, DrawableSet* drset/* = NULL*/)
    : MenuItem(text, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(RadioMenuItem))
{
    setGap(5);
    setCheck(checked);
    m_tag = tag;
}

RadioMenuItem::RadioMenuItem(int strId, Image* img, int i_id,
        int tag/*=0*/, bool b_disable/*=false*/, bool checked/* = false*/, DrawableSet* drset/* = NULL*/)
    : MenuItem(strId, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(RadioMenuItem))
{
    setGap(5);
    setCheck(checked);
    m_tag = tag;
}

void RadioMenuItem::onMenuEnter(Menu* owner)
{
    if (!owner)
        return ;

    if (!getCheck()) {
        setCheck(true);
        owner->uncheckRadioItemExcept(this);
    }
   
    MenuItem::onMenuEnter(owner);
    updateView();
}

void RadioMenuItem::check(Menu* owner, bool bstate)
{
    if (!getCheck() && bstate) {
        setCheck(bstate);
        owner->uncheckRadioItemExcept(this);
        updateView();
    }
}

SubMenuItem::SubMenuItem(const char * text, Image *img, int i_id, Menu* submenu, bool b_disable, DrawableSet* drset)
    : MenuItem(text, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(SubMenuItem))
{
    if(img) {
        setImagePos(1);
        enableImage(true);
    }

    m_subMenu = submenu;
}

SubMenuItem::SubMenuItem(int strId, Image *img, int i_id, Menu* submenu, bool b_disable, DrawableSet* drset)
    : MenuItem(strId, img, i_id, b_disable, drset ? drset : DEFAULT_VIEW_DRAWABLESET(SubMenuItem))
{
    if(img) {
        setImagePos(1);
        enableImage(true);
    }

    m_subMenu = submenu;
}

void SubMenuItem::onMenuEnter(Menu* owner)
{
    if (m_subMenu) {
        Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());
        if (ap != NULL) {
            ap->bindMenu(m_subMenu);
        }
        stopRoll();
        m_subMenu->setMenuParent(owner);
        m_subMenu->showMenu();
    }
}

void SubMenuItem::onMenuRight(Menu * owner)
{
    if (!owner) {
        return ;
    }

    if (m_subMenu) {
        Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());
        if (ap != NULL) {
            ap->bindMenu(m_subMenu);
        }
        stopRoll();
        m_subMenu->setMenuParent(owner);
        m_subMenu->showMenu();
        owner->notifyToParent(Menu::CUSTOM_MENU_KEY_SL, id());
    }
}

SubMenuItem::~SubMenuItem()
{
    if (m_subMenu)
        HFCL_DELETE(m_subMenu);
    m_subMenu = NULL;
}

void SubMenuItem::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
    MenuItem::drawContent(context, rc, status);

    if(isDisabled())
        status = DRAWSTATE_DISABLED;

    if(IsAllItemViewNeedConvert()) {
        rc.setRect(rc.left()+3, rc.top()+1, rc.left() + _ngux_sl_item_h1/2, rc.bottom());
        m_drset->draw(context, DR_LEFTARROW, status, rc);
    }
    else {
        rc.setRect(rc.right() - _ngux_sl_item_h1+10, rc.top()+1, rc.right(), rc.bottom());
        m_drset->draw(context, DR_RIGHTARROW, status, rc);
    }
}

// ============================= Menu =======================================
Menu* Menu::m_currentMenu = NULL;

Menu::Menu(HTResId resid)
    : m_menuItemList(NULL)
    , m_menuItemHeight(_ngux_menuitem_h)
    , m_maxHeight(_ngux_menuitem_h*MAX_MENUITEM_NUM)
    , m_menuParent(NULL)
    , m_menuBar(NULL)
    , m_separator_top(NULL)
    , m_separator_bottom(NULL)    
    , m_list_themedrset_id(-1)
    , m_item_themedrset_id(-1)
    , m_menubar_themedrset_id(-1)
    , m_isClose(false)
    , m_isProcessing(true)
{
    setLayer(1);
    create(resid);
    _DBG_PRINTF ("MENU TRACE :: create menu [%p],", this);
}

Menu::Menu(int list_theme_drset_id, int item_theme_drset_id, int menubar_theme_drset_id, HTResId resid)
    : m_menuItemList(NULL)
    , m_menuItemHeight(_ngux_menuitem_h)
    , m_maxHeight(_ngux_menuitem_h*MAX_MENUITEM_NUM)
    , m_menuParent(NULL)
    , m_menuBar(NULL)
    , m_separator_top(NULL)
    , m_separator_bottom(NULL)
    , m_list_themedrset_id(list_theme_drset_id)
    , m_item_themedrset_id(item_theme_drset_id)
    , m_menubar_themedrset_id(menubar_theme_drset_id)
    , m_isClose(false)
    , m_isProcessing(true)
{
    setLayer(1);
    create(resid);

    _DBG_PRINTF ("MENU TRACE :: create menu [%p],", this);
}

Menu::~Menu()
{
    if (m_ownApp != NULL && ActivityManager::getInstance()->isExist(m_ownApp))
        m_ownApp->setLayer(-1);
    // removeAll();

    _DBG_PRINTF ("MENU TRACE :: delete menu [%p],", this);
}

void Menu::create(HTResId resid)
{
    // hide the current active menu
    hideMenu();

    // create an invisible main window for this menu.
    int h = MAX_MENUITEM_NUM * _ngux_menuitem_h + _ngux_menubar_h;
    int y = _ngux_screen_h - h;
    createMainWindow (0, y, _ngux_screen_w, h, false);

    CreateViewFromRes (resid == 0 ? UI_TEMPL : resid, this, this, NULL);
    // theme listview
    if (m_list_themedrset_id != -1 && m_menuItemList) {
        m_menuItemList->themeAble(true);
        m_menuItemList->setThemeDrsetId(m_list_themedrset_id);         
        m_menuItemList->setDrawableSet(GetDrawableSetResFromTheme(m_list_themedrset_id));
    }

    // theme menubar
    if (m_menubar_themedrset_id != -1 && m_menuBar) {
        m_menuBar->themeAble(true);
        m_menuBar->setThemeDrsetId(m_menubar_themedrset_id);
        m_menuBar->setDrawableSet(GetDrawableSetResFromTheme(m_menubar_themedrset_id));
    }

    int idx = getMenuHilightIndex();

    // find first enable item
    if (idx == -1) {
        m_menuItemList->hilight(0);
    }
    else {
        m_menuItemList->hilight(idx);
    }
}

Intent* Menu::onDestroy(ContextStream* contextstream)
{
    return NULL;
}

static bool g_needUpdateBkg = FALSE;

void Menu::drawContent(GraphicsContext* context, IntRect &rc, int status)
{
    if (m_currentMenu == this)
        Window::drawContent(context, rc, status);
}

void Menu::drawBackground(GraphicsContext* context, IntRect &rc, int status)
{
    Activity* app = (Activity*)(ActivityManager::getInstance()->getCurrentApp());

    if (app->menu() != m_currentMenu) {
        return;
    }

    // update background only once. background will not update durning draw parent menu; the flag reset in showMenu. 
    if (g_needUpdateBkg && m_currentMenu == this) {
        g_needUpdateBkg = FALSE;

        Menu *_prt = getMenuParent();

        if (app != NULL) {
            int old = app->setLayer(0);

            //IncludeWindowStyle((unsigned int)app->viewWindow(), WS_VISIBLE);
            ShowWindow((unsigned int)app->viewWindow(), SW_SHOWNORMAL);
            app->updateWindow();// draw cur app to layer 0.
            app->setLayer(old);
        }

        if ( _prt != NULL) {
            updateParentMenu(_prt);// draw all parent menu to layer 0.
        }

        GraphicsContext::screenGraphics()->setLayer(0);
        GraphicsContext::screenGraphics()->setMapView(this);

#if 0 /* VincentWei: no use code */
        int y, h;
        int itemnum = m_menuItemList->itemCount();
        if(itemnum > MAX_MENUITEM_NUM)
            itemnum = MAX_MENUITEM_NUM;
        else if(itemnum < 2)
            itemnum = 2;

        h = itemnum * _ngux_menuitem_h + _ngux_menubar_h;
        y = _ngux_screen_h - h;

        IntRect rc(0, y, m_rect.width(), _ngux_screen_h);

        //getAppClientRect(rc);
        // fill transparent color to layer 0.
        GraphicsContext::screenGraphics()->fillRect(rc, 0x00, 0x00, 0x00, 0x80);
        GraphicsContext::screenGraphics()->setLayer(1);
        // clear layer 1 frame buffer for draw the topest menu.
        GraphicsContext::screenGraphics()->fillRect(rc, 0xFF, 0xFF, 0xFF, 0xFF);

        //IncludeWindowStyle((unsigned int)this->viewWindow(), WS_VISIBLE); // reset the menu clip rect for draw menu context
#endif
        ShowWindow((unsigned int)this->viewWindow(), SW_SHOWNORMAL);
    }
}

bool Menu::onKey(int keyCode, KeyEvent* event)
{
    if(m_menuBar)
        m_menuBar->dispatchEvent(event);

    if(keyCode == KeyEvent::KEYCODE_UP
            || keyCode == KeyEvent::KEYCODE_DOWN) 
    {
        m_menuItemList->dispatchEvent(event);
        return DISPATCH_STOP_MSG;
    }

    if (event->eventType() == Event::KEY_UP) 
    {
        switch (keyCode) 
        {
            case KeyEvent::KEYCODE_0:
                keyCode = KeyEvent::KEYCODE_9+1;
            case KeyEvent::KEYCODE_1:
            case KeyEvent::KEYCODE_2:
            case KeyEvent::KEYCODE_3:
            case KeyEvent::KEYCODE_4:
            case KeyEvent::KEYCODE_5:
            case KeyEvent::KEYCODE_6:
            case KeyEvent::KEYCODE_7:
            case KeyEvent::KEYCODE_8:
            case KeyEvent::KEYCODE_9:
            //case KeyEvent::KEYCODE_0:
            case KeyEvent::KEYCODE_STAR:
                if ( (keyCode - KeyEvent::KEYCODE_1) < m_menuItemList->itemCount()  && !m_isProcessing) 
                {
                    setMenuHilightIndex(keyCode - KeyEvent::KEYCODE_1);
                    updateWindow();
                    onMenuEnter();
                }
                break;
            case KeyEvent::KEYCODE_ENTER: 
            if (!m_isProcessing) {
                   onMenuEnter();
            }
            return DISPATCH_STOP_MSG;
            case KeyEvent::KEYCODE_SL:
            case KeyEvent::KEYCODE_LEFT:
#if 0
            if (!m_isProcessing && ItemView::IsAllItemViewNeedConvert()) 
            {
               MenuItem* mi = getCurMenuItem();
                    if (mi->isDisabled())
                        break;
                    mi->onMenuRight(this);
            }
                break;
#endif
            case KeyEvent::KEYCODE_RIGHT:
            if (!m_isProcessing && !ItemView::IsAllItemViewNeedConvert()) 
            {
                    MenuItem* mi = getCurMenuItem();
                    if (mi->isDisabled())
                        break;
                    mi->onMenuRight(this);
            }
                break;
            case KeyEvent::KEYCODE_SR: 
                {
                    hide();
                    //FIXME: cancel or leave from menu
                    notifyToParent(CUSTOM_MENU_KEY_SR, getCurMenuItemID());
                    closeMenu();
                }
                break;
            case KeyEvent::KEYCODE_STOP: 
                {
                    Activity* app = (Activity*)(ActivityManager::getInstance()->getCurrentApp());
                    hideMenu();
                    event->setSource((Object*)getCurMenuItemID()); // indicate key event come from menu
                    closeAllMenu();
                    app->onKey(keyCode, event);
                }
            break;
            default:
                break;
        }
    }
#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__   
    else if(event->eventType() == Event::KEY_LONGPRESSED && keyCode == KeyEvent::KEYCODE_STAR)
    {
        enterLockFrame(true, true);
        return DISPATCH_STOP_MSG;
    }
#endif

    return DISPATCH_CONTINUE_MSG;
}

void Menu::onMenuEnter(void)
{
    MenuItem* mi = getCurMenuItem();
   int miId = mi->id();
   bool hasSub = mi->getSubMenu() != NULL;
   
    if (mi->isDisabled())
        return;

   m_isProcessing = true;
   
    mi->onMenuEnter(this);
   notifyToParent(Menu::CUSTOM_MENU_KEY_SL, miId);
   
   if(m_currentMenu != NULL && !hasSub && !m_isClose)
   {
      closeAllMenu();
   }
   
}

void Menu::onClick(POINT pt, Event::EventType type)
{
    LOGI("\n");
}

void Menu::setMenuLeftButtonText(const char * text)
{
    if (NULL != m_menuBar) {
        m_menuBar->setLeftText(text);
    }
}

void Menu::setMenuLeftButtonText(int strid)
{
    if (NULL != m_menuBar) {
        m_menuBar->setLeftText(strid);
    }
}

#if 0
const char * Menu::getMenuLeftButtonText(void) const
{
    return m_menuBar ? m_menuBar->leftText().c_str() : NULL;
}
#endif
void Menu::setMenuRightButtonText(const char * text)
{
    if (NULL != m_menuBar) {
        m_menuBar->setRightText(text);
    }
}

void Menu::setMenuRightButtonText(int strid)
{
    if (NULL != m_menuBar) {
        m_menuBar->setRightText(strid);
    }
}
#if 0
const char * Menu::getMenuRightButtonText(void) const
{
    return m_menuBar ? m_menuBar->rightText().c_str() : NULL;
}
#endif
Menu* Menu::getCurrentMenu(void)
{
    return m_currentMenu;
}

void Menu::updateParentMenu(Menu * menuP)
{
   if(!menuP)
      return ;

   menuP->setLayer(0);
   //IncludeWindowStyle((unsigned int)menuP->viewWindow(), WS_VISIBLE);
   ShowWindow((unsigned int)menuP->viewWindow(), SW_SHOWNORMAL);
   menuP->updateWindow(false);   

   Menu* parent = menuP->getMenuParent();
   if(parent){
      updateParentMenu(parent);
   }
}

void Menu::showMenu(Activity *app)
{
    int barWidth;

    if(!m_menuItemList) {
        return ;
    }

    if (m_currentMenu != NULL && m_currentMenu != this) {
        if(m_currentMenu != getMenuParent()) {
            // if the current menu is not my parent
            // close it
            Activity *_app = m_currentMenu->ownApp();
            m_currentMenu->closeMenu(m_currentMenu);
            if (_app != NULL) {
                _app->bindMenu(NULL);
            }
        }
        else {
            m_currentMenu->hide();
        }
    }
  
    if (app) {
        m_ownApp = app;
        app->bindMenu(this);
    }
    else {
        Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());   
        ap->bindMenu(this);
        m_ownApp = ap;
    }

    if (m_ownApp) {
        HWND owner = m_ownApp->viewWindow();
        if (IsWindowEnabled (owner)) {
            EnableWindow (owner, FALSE);
            InvalidateRect (owner, NULL, TRUE);
            _DBG_PRINTF ("Disable and update app window here: 0x%x", owner);
        }
    }

    g_needUpdateBkg = TRUE;
    setLayer(1);
    setActiveWindow((unsigned int)viewWindow());

    m_currentMenu = this;
    m_isProcessing = false;

    bool ret = m_menuItemList->scrollVisiable(&barWidth);
    if(ret) {
        int cnt = m_menuItemList->itemCount();
        for(int i=0; i< cnt; i++)
        {
            MenuItem* item = (MenuItem*)m_menuItemList->itemFromIndex(i);
            if(item->getSubMenu())
                continue;
            else
                item->setTextRightGap(barWidth+3);
        }
    }

    autoFit();
    show();

    if (m_menuItemList && 0 > m_menuItemList->hilightItemIndex()) {
        m_menuItemList->hilight(0);
    }
    else {
        MenuItem* item = (MenuItem *)m_menuItemList->hilightItem();
        if (item != NULL) {
            item->startRoll();
        }
    }
}

void Menu::hideMenu(void)
{
    Menu *_prt = getMenuParent();
    if (_prt != NULL) {
        _prt->hideMenu();
    }

    if (m_ownApp != NULL)
        m_ownApp->setLayer(-1);

    hide();

    _DBG_PRINTF ("MENU TRACE :: hideMenu _prt is:%p m_currentMenu is:%p \n", _prt, m_currentMenu);
}

void Menu::closeAllMenu(void)
{
    Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());
   
    _DBG_PRINTF ("MENU TRACE :: closeAllMenu menu is : [%s] \n", ap->name());

    if (ap != NULL && ap == m_ownApp) {
        HWND owner = ap->viewWindow();
        EnableWindow (owner, TRUE);
        InvalidateRect (owner, NULL, TRUE);
        _DBG_PRINTF ("Enable app window here: 0x%x", owner);

        ap->setLayer(-1);
        setActiveWindow((unsigned int)ap->viewWindow());
        ap->bindMenu(NULL);
    }
   
    if (NULL != m_currentMenu) {
        Menu *root = m_currentMenu;
        while (root->isExitMenuParent()){
            root = root->getMenuParent();
        }
        m_currentMenu = NULL;
        HFCL_DELETE (root);
    }
}

void Menu::closeMenu(Menu *menu)
{
    Menu *root = menu;
    _DBG_PRINTF ("MENU TRACE :: closeMenu menu is:%p m_currentMenu is:%p \n", menu, m_currentMenu);

    m_isClose = TRUE;
    while (root && root->isExitMenuParent()) {
        root = root->getMenuParent();
    }

    HFCL_DELETE (root);
}

void Menu::closeMenu(void)
{
    Activity *ap = (Activity *)(ActivityManager::getInstance()->getCurrentApp());

    _DBG_PRINTF ("MENU TRACE :: after Menu closeMenu m_currentMenu is:%p\n", m_currentMenu);
    if (NULL == m_currentMenu) {
        m_currentMenu = ap->menu();
    }

    if (NULL != m_currentMenu) {
        Menu* p = NULL;

        if (m_currentMenu->isExitMenuParent()){
            p = m_currentMenu->getMenuParent();
            m_currentMenu->hideMenu();
        }
        else {
            HFCL_DELETE (m_currentMenu);
            m_currentMenu = NULL;
        }

        if (NULL != p && p->ownApp() == ap) {
            _DBG_PRINTF ("MENU TRACE :: after Menu closeMenu p is:%p\n", p);
            m_currentMenu = p;
            p->showMenu();
        } 
        else {
            _DBG_PRINTF ("MENU TRACE :: after Menu closeMenu app is:%p  app name is:(%s)\n", ap, ap == NULL ? "null":ap->name() );
            if (NULL != ap) {
                HWND owner = ap->viewWindow();
                EnableWindow (owner, TRUE);
                InvalidateRect (owner, NULL, TRUE);
                _DBG_PRINTF ("Enable app window here: 0x%x", owner);
                setActiveWindow((unsigned int)ap->viewWindow());
            }
            ap->bindMenu(NULL);
        }
    }
}

Menu* Menu::getSubMenu(int i_id)
{
    MenuItem* mi = getMenuItemByID(i_id);
    if (NULL != mi)
        return mi->getSubMenu();

    return NULL;
}

bool Menu::addMenuItem(MenuItem* mi)
{
    bool ret = false;

    if (NULL == mi || NULL == m_menuItemList)
        return ret;

    mi->setRect(0, 0, m_menuItemList->getRect().width(), m_menuItemHeight);
    if(mi->getSubMenu())
        mi->setTextRightGap(_ngux_sl_item_h2);
    else
        mi->setTextRightGap(0);

    // we do NOT theme item if item has its own theme.
    if (!mi->isThemeAble() && m_item_themedrset_id != -1) {
        mi->themeAble(true);
        mi->setThemeDrsetId(m_item_themedrset_id);      
        mi->setDrawableSet(GetDrawableSetResFromTheme(m_item_themedrset_id));
    }

    ret = m_menuItemList->addItem(mi);
    refreshItemIndex();
    return ret;
}

bool Menu::addMenuItemByIndex(MenuItem* mi,int index)
{
    bool ret = false;

    if (NULL == mi || NULL == m_menuItemList)
        return ret;

    mi->setRect(0, 0, m_menuItemList->getRect().width(), m_menuItemHeight);

    if(mi->getSubMenu())
        mi->setTextRightGap(_ngux_sl_item_h2);
    else
        mi->setTextRightGap(0);

    // we do NOT theme item if item has its own theme.
    if (!mi->isThemeAble() && m_item_themedrset_id != -1) {
        mi->themeAble(true);
        mi->setThemeDrsetId(m_item_themedrset_id);
        mi->setDrawableSet(GetDrawableSetResFromTheme(m_item_themedrset_id));
    }

    ret = m_menuItemList->addItemByIndex(mi,index);

    refreshItemIndex();

    return ret;
}

bool Menu::notifyToParent(MenuKeyCustomType mkct, int menuId)
{   
    if (menuId >= 0) {
        CustomEvent event(Event::CUSTOM_NOTIFY, mkct, menuId);
        raiseEvent(&event);
    }
    return true;
}

void Menu::uncheckRadioItemExcept(RadioMenuItem* item)
{
    if (!m_menuItemList)
        return;

    int tag = item->getTag();
    if (tag == MenuItem::INVALID_TAG)
        return;

    for (MenuItem* subitem = (MenuItem*)m_menuItemList->firstItem();
            subitem; subitem = (MenuItem*)subitem->nextSibling()) {
        if (subitem != item
                && subitem->getTag() == tag
                && subitem->getCheck()) {
            subitem->setCheck(false);
            subitem->updateView();
            //FIXME: raiseEvent
            break;
        }
    }
}

void Menu::setMaxHeight(int max)
{
    m_maxHeight = (Uint16) max;
    autoFit();
}

void Menu::updateSize()
{
    IntRect rcMenuBar = m_rect;
    rcMenuBar.offset(-rcMenuBar.left(), -rcMenuBar.top());
    if (m_menuBar) {
        rcMenuBar.m_top = rcMenuBar.m_bottom - m_menuBar->getRect().height();
        m_menuBar->setRect(rcMenuBar);
    }
    if (m_menuItemList) {
        int h;
        int itemnum = m_menuItemList->itemCount();
        if(itemnum > MAX_MENUITEM_NUM)
            itemnum = MAX_MENUITEM_NUM;
        else if(itemnum < 2)
            itemnum = 2;

        h = itemnum * _ngux_menuitem_h;
        IntRect rc(0, 0, m_rect.width(), h);
        m_menuItemList->setRect(rc);
    }
}

void Menu::hilightFirstEnable(void)
{
    View* view = NULL;
    int idx = -1;
    if (NULL != m_menuItemList) {
        view = m_menuItemList->firstEnableItem();
        if (view) {
            idx = m_menuItemList->indexFromItem((ItemView *)view);
            if (-1 != idx)
                m_menuItemList->hilight(idx);
        }
    }
}

//auto change it's size
void Menu::autoFit()
{
    int h;
    int itemnum = m_menuItemList->itemCount();

    if(itemnum > MAX_MENUITEM_NUM)
        itemnum = MAX_MENUITEM_NUM;
    else if(itemnum < 1)
        itemnum = 1;
      
    h = itemnum * _ngux_menuitem_h + _ngux_menubar_h;

    int y = _ngux_screen_h - h;
    MoveWindow (m_viewWindow, 0, y, _ngux_screen_w, h, FALSE);  
    ShowWindow (m_viewWindow, SW_SHOWNORMAL);

    // top seperator line 
    m_separator_top->setRect(0, 0, _ngux_screen_w, 1);

    // menu list rect
    if (m_menuItemList && itemnum > 0) {
        m_menuItemList->setRect(0, 0, _ngux_screen_w, h - _ngux_menubar_h);
        m_menuItemList->updateView();
    }

    // bottom seperator line
    m_separator_bottom->setRect(0, h-1, _ngux_screen_w, 1);
    
    // menubar rect
    if (m_menuBar) {
        m_menuBar->setRect(0, h - _ngux_menubar_h, m_rect.width(), h);
    }
}

bool Menu::queryItemState(int i_id, int tag, bool *pdisable, bool *pcheck)
{
    if (!pdisable && !pcheck)
        return false;

    MenuQueryItem mqi = {
        i_id,
        tag,
        pdisable ? *pdisable : false,
        pcheck ? *pcheck : false
    };

    CustomEvent event(Event::CUSTOM_NOTIFY, CUSTOM_MENU_QUERY_ITEM, (int)(&mqi));

    raiseEvent(&event);

    if (pdisable)
        *pdisable = mqi.bdisable;
    if (pcheck)
        *pcheck = mqi.checked;

    return true;
}

void Menu::refreshItemIndex()
{
    for (register int i = 0; i < getItemCount(); /* i++ */) {
        MenuItem *it = getMenuItem(i);
        if (it != NULL && it->isVisible()) {
            it->setIndex(++i);
        }
    }
}

DEFINE_UI_TEMPL(Menu)

BEGIN_SETVIEW(Menu)
    MAP_VIEW(m_separator_top, SEPARATOR_TOP, PanelView)
    MAP_VIEW(m_menuItemList, ITEMLIST, ListView)
    MAP_VIEW(m_separator_bottom, SEPARATOR_BOTTOM, PanelView)    
    MAP_VIEW(m_menuBar, MENUBAR, MenuBarView)
END_SETVIEW

BEGIN_GETHANDLE(Menu)

END_GETHANDLE

} // namespace hfcl
