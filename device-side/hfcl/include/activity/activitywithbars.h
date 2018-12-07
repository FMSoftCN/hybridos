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

#ifndef HFCL_ACTIVITY_ACTIVITYWITHBARS_H_
#define HFCL_ACTIVITY_ACTIVITYWITHBARS_H_

#include "window.h"
#include "intent.h"
#include "baseactivity.h"
#include "../common/event.h"
#include "../components/menu.h"

class ActivityClient : public ControllerClient {
public:
    // used for getdate form itemlist template
    enum{
        APP_ITEMLIST_IMAGE,
        APP_ITEMLIST_TEXT1,
        APP_ITEMLIST_TEXT2,
        APP_ITEMLIST_TEXT3
    };

    ActivityClient(Controller* owner, int view_id, View* parent)
        : ControllerClient(owner, view_id, parent, 0, 0) { }

    virtual void active() {
        ControllerClient::active();
        if (isTip()) {
            setMenuBarStyle (TIP_MENUBAR);
        } else {
            setMenuBarStyle (DEF_MENUBAR);
        }
    }

    virtual void onMenuCommand (CustomEvent* event) { }
    virtual void onFullScreen (bool bfullscreen) { }

    virtual bool isTip () { return false; }

    /**
     * Ops for statusbar
     */
    bool isStatusBarShown () {
        return m_owner != NULL ?
                (!((ActivityWithBars *)m_owner)->isFullScreen()) : false;
    }
    virtual bool showStatusBar (bool show) {
        if(m_owner)
            ((ActivityWithBars *)m_owner)->setFullScreen(!show);
        return true;
    }

    /**
     * Ops for menubar
     */
    bool isMenuBarShown() {
        return m_owner != NULL ?
                ((ActivityWithBars *)m_owner)->isMenuBarShown() : false;
    }
    virtual bool showMenuBar(bool show) {
        if(m_owner)
            ((ActivityWithBars *)m_owner)->showMenuBar(show);
        return true;
    }

    void setMenuBarStyle(MenuBarStyle menuBarType) {
        if (m_owner)
            ((ActivityWithBars *)m_owner)->setMenuBarStyle(menuBarType);
    }
    MenuBarStyle getMenuBarStyle() {
        if (m_owner)
            return ((ActivityWithBars *)m_owner)->getMenuBarStyle();
    }

    /**
     * Lock/Unlock update
     */
    void lockActivityUpdate() {
        if (m_owner)
            ((Window*)(m_owner))->setUpdateLock(true);
    }
    void unlockActivityUpdate() {
        if (m_owner)
            ((Window*)(m_owner))->setUpdateLock(false);
    }

    /**
     * Ops for Tip View
     */
    unsigned int showTipView (int view_id, NGParam param1, NGParam param2) {
        if(m_owner)
            return ((ActivityWithBars *)m_owner)->showTipView (view_id,
                    param1, param2);
        return 0;
    }
    void exitTip(int endCode = TipDlg::TIP_DESTROY) {
        if (m_owner)
            ((ActivityWithBars *)m_owner)->exitTip(endCode);
    }

    virtual unsigned int showMsgTipDlg (NGCPStr text_info, unsigned int type,
            int second = 0) {
        if (m_owner)
            return ((ActivityWithBars *)m_owner)->showMsgTipDlg (
                    text_info, type, second);
        return 0;
    }
    virtual unsigned int showMsgTextDlg (NGCPStr text_info, unsigned int type,
            int icon_id = 0, int text_style = 0, int second = 0) {
        if (m_owner)
            return ((ActivityWithBars *)m_owner)->showMsgTextDlg (
                    text_info, type, icon_id, text_style, second);
        return 0;
    }

    void exitHome() {
        if (m_owner)
            ((ActivityWithBars *)m_owner)->exitHome();
    }
    void exitCall() {
        if (m_owner)
            ((ActivityWithBars *)m_owner)->exitCall();
    }
};

class PopClient : public ActivityClient {
public:
    PopClient(Controller* owner, int view_id, View*parent)
        : ActivityClient(owner, view_id, parent) { }
    virtual ~PopClient() {}

    virtual bool isTip() { return true; }

    virtual void active() {
        if (m_owner->getClientCount() == 0) {
            Activity *act = (Activity *)(ActivityManager::getInstance()->getTopActivity(1));
            if (act != NULL) {
                int old = act->setLayer(1);
                act->show();
                act->updateWindow();
                act->setLayer(old);
            }
        }

        ActivityClient::active();

        m_mennBarShownBeforeTip = isMenuBarShown();
        showMenuBar(true);
    }

    virtual void inactive() {
        showMenuBar (m_mennBarShownBeforeTip);
        ActivityClient::inactive ();
    }

private:
    bool m_mennBarShownBeforeTip;
};

class ActivityWithBars: public FullScreenActivity, public TimerEventListener {
public:
    ActivityWithBars ();
    ActivityWithBars (bool fullscreen);
    virtual ~ActivityWithBars();

    virtual void onCreate (ContextStream* contextstream, Intent* intent);
    virtual void onWakeup () {
        if (getTopActivityClient() == NULL) { // this act has only tip Client
            BaseActivity *back = ActivityManager::getInstance()->getTopActivity(1);
            if (back != NULL) {
                back->onWakeup();
            }
        }
        FullScreenActivity::onWakeup();
        refreshBars();
    }
    virtual void onSleep() {
        closeTimer();
    }
    virtual Intent *onDestroy (ContextStream *contextStream) {
        return NULL;
    }

    // when open pop, this function will be call.
    virtual void hold(){}

    // when close(exit) pop, this function will be call.
    virtual void resume(){}

    virtual unsigned int onClientCommand(int sender, unsigned int cmd_id,
            HTData param1, HTData param2);

    virtual void drawBackground(GraphicsContext* context, IntRect &rc, int status);

    bool handleEvent (Event * event);
    virtual bool onKey (int keyCode, KeyEvent* event);

    MenuBarView* getMenuBar();
    void setMenuBarStyle(MenuBarStyle menuBarType) {
        if (m_menuBar != NULL)
            m_menuBar->setMenuBarStyle(menuBarType);
    }
    MenuBarStyle getMenuBarStyle() {
        return m_menuBar != NULL ? m_menuBar->getMenuBarStyle() : DEF_MENUBAR;
    }

    unsigned int setMode(ControllerModeList* modelist);
    virtual unsigned int showView(int view_id, NGParam param1, NGParam param2);
    virtual unsigned int backView(unsigned int endcode = 0);
    unsigned int showModalView(int view_id, NGParam param1, NGParam param2);
    unsigned int showTipView(int view_id, NGParam param1, NGParam param2);
    virtual void exitTip(int endCode = TipDlg::TIP_DESTROY);
    virtual unsigned int showMsgTipDlg(NGCPStr text_info, unsigned int type, int second = 0);
    virtual unsigned int showMsgTextDlg(NGCPStr text_info, unsigned int type,
            int icon_id = 0, int text_style = 0, int second = 0);

    virtual void onLockingShow();
    virtual void onLockingExit();

    bool isFullScreen() { return m_isFullScreen; }
    virtual void setFullScreen(bool bfullScreen);
    void setFullScreen(bool bfullScreen, bool bhidemenubar);

    void showMenuBar(bool show = true);
    void showIndicator(bool show = true);

    bool isMenuBarShown () { return m_menuBarShown; }
    bool isIndicatorShown () { return !m_isFullScreen; }

    virtual void exitHome();
    virtual void exitCall();
    void refreshBars ();

    void refreshIndicator ();
    void refreshMenubar ();

    void setCursor(int cursor_index, int current_value);
    int getCursor(int cursor_index);

    /* VincentWei: make this method be virtual */
    virtual bool beInLockFrame();

    ActivityClient *getTopClient(bool bPop);
    ActivityClient *getTopActivityClient() { return getTopClient(false); }
    PopClient *getTopPopClient() { return (PopClient *)getTopClient(true); }
#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__
    virtual void setShowLockPopup(bool lockShow);
    bool isShowLockPopup() {return m_isShowLockPopup;}
#endif

protected:
    virtual bool onEndKeyHome () {
        return true;
    }

    virtual void onMenuShow (Menu *menuToShow) {}
    void onLanguageChanged (int langId);

    void openTimer();
    void closeTimer();

    void showMenu();
    virtual bool doCommand(int cmd);

    DECLARE_CONTROLLER_CLIENTS

protected:
    MenuBar *       m_menuBar;
    Indicator *     m_indicator;
    bool            m_isFullScreen;
    bool            m_menuBarShown;
    HTResId         m_menuId;
    unsigned int    m_menubarCmds[3];
    int             m_timerId; /* VincentWei: unsigned int to int */
    bool            m_keyUp;
    int             m_usedLang;
    ActivityClient *m_keyDownTarget;
    int             m_keyDownCode;
    bool            m_tipShown; /* VincentWei: avoid re-entrance of TipView */
#ifdef __SUPPORT_MANUAL_KEYPAD_LOCK_IN_ALLSCREEN__
    bool            m_isShowLockPopup;
#endif
};

#endif // HFCL_ACTIVITY_ACTIVITYWITHBARS_H_

